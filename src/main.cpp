 /*
  geo1004.2023
  hw02 help code
  Hugo Ledoux <h.ledoux@tudelft.nl>
  2023-03-01
*/

#include <CGAL/Triangulation_3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <typeinfo>
#include "definitions.h"
#include "geomtools.h"
#include "parse_obj.h"
#include <CGAL/Surface_mesh.h>
#include <CGAL/optimal_bounding_box.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
//#include <CGAL/point_generators_3.h>
#include <CGAL/Simple_cartesian.h>


//-- https://github.com/nlohmann/json
//-- used to read and write (City)JSON

//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_3<K> Tetrahedron;
namespace PMP = CGAL::Polygon_mesh_processing;
typedef CGAL::Surface_mesh<Point3> Surface_mesh;
typedef CGAL::Simple_cartesian<double>                           cartesian;
std::vector<Point3> get_coordinates(const json& j, bool translate = true);
void                save2obj(std::string filename, const json& j);
void                enrich_and_save(std::string filename, json& j);


using namespace std;

int main(int argc, const char * argv[]) {
    //-- will read the file passed as argument or 2b.city.json if nothing is passed
    const char* filename = (argc > 1) ? argv[1] : "../data/input.obj";
    std::vector<Point3> vertices;
    auto obj_parse_result = parse_obj(filename,vertices);
    std::vector<Face> faces = obj_parse_result.first;
    stored_faces_obj("face_obj.obj",faces,vertices);
    std::array<Point3, 8> oobb = generate_oobb_building(faces,vertices);
    VoxelGrid voxels = VoxelGrid(oobb,0.5);
    voxels.push_voxel(faces,vertices);
    voxels.out_put_all_voxel_to_obj("output_voxels.obj");
    //for (const auto& point : oobb) {
    //std::cout << point << " ";
    //}
    //std::cout << std::endl;












    /*std::cout << "Processing: " << filename << std::endl;
    std::ifstream input(filename);
    cout<<"json"<<endl;
    json j;
    input >> j; //-- store the content of the file in a nlohmann::json object

    input.close();

    //-- convert each City Object in the file to OBJ and save to a file
    save2obj("out.obj", j);

    //-- enrich with some attributes and save to a new CityJSON
    enrich_and_save("out.city.json", j);*/

    return 0;
}


//-- write the OBJ file
void save2obj(std::string filename, const json& j) {
    std::ofstream ofile(filename);
    //-- fetch all the vertices in real-world coordinates (so "transform" is applied)
    std::vector<Point3> lspts = get_coordinates(j, true);
    for (auto& p : lspts) {
        ofile << std::setprecision(5) << std::fixed << "v " << p.x() << " " << p.y() << " " << p.z() << std::endl;
    }
    //-- iterate over each object in the file and output the CDT
    for (auto& co : j["CityObjects"].items()) {
        for (auto& g : co.value()["geometry"]) {
            if ( (g["type"] == "Solid") && (g["lod"] == "2.2") ) {   //-- LoD2.2 only!!!!!
                ofile << "o " << co.key() << std::endl;
                for (int i = 0; i < g["boundaries"].size(); i++) {
                    for (int j = 0; j < g["boundaries"][i].size(); j++) {
                        std::vector<std::vector<int>> gb = g["boundaries"][i][j];
                        std::vector<std::vector<int>> trs = construct_ct_one_face(gb, lspts);
                        for (auto& tr : trs) {
                            ofile << "f " << (tr[0] + 1) << " " << (tr[1] + 1) << " " << (tr[2] + 1) << std::endl;
                        }
                    }
                }
            }
        }
    }
    ofile.close();
    std::cout << "OBJ file written to disk: " << filename << std::endl;
}



std::vector<Point3> get_coordinates(const json& j, bool translate) {
    std::vector<Point3> lspts;    std::vector<std::vector<int>> lvertices = j["vertices"];
    if (translate) {
        for (auto& vi : lvertices) {
            double x = (vi[0] * j["transform"]["scale"][0].get<double>()) + j["transform"]["translate"][0].get<double>();
            double y = (vi[1] * j["transform"]["scale"][1].get<double>()) + j["transform"]["translate"][1].get<double>();
            double z = (vi[2] * j["transform"]["scale"][2].get<double>()) + j["transform"]["translate"][2].get<double>();
            lspts.push_back(Point3(x, y, z));
        }
    } else {
        //-- do not translate, useful to keep the values low for downstream processing of data
        for (auto& vi : lvertices) {
            double x = (vi[0] * j["transform"]["scale"][0].get<double>());
            double y = (vi[1] * j["transform"]["scale"][1].get<double>());
            double z = (vi[2] * j["transform"]["scale"][2].get<double>());
            lspts.push_back(Point3(x, y, z));
        }
    }
    return lspts;
}
