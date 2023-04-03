#ifndef __geomtools__
#define __geomtools__

#include "definitions.h"


Plane                 get_best_fitted_plane(const std::vector<Point3> &lspts);
void                  mark_domains(CT& ct);
void                  mark_domains(CT& ct, CT::Face_handle start, int index, std::list<CT::Edge>& border);

std::vector<std::vector<int>>
construct_ct_one_face(const std::vector<std::vector<int>>& lsRings, 
                      const std::vector<Point3>& lspts);

class MyPoint : public Point3 {
public:
    MyPoint(double x, double y, double z) : Point_3(x, y, z) {}

    MyPoint operator+(const MyPoint& other) const {
        return MyPoint(x() + other.x(), y() + other.y(), z() + other.z());
    }
};






struct Voxel{
public:
    Voxel(const Point3& corner,const Vector3& vx,const Vector3& vy,const Vector3& vz):corner(corner){
        label = 0;}
    // label the Voxel during label
    Voxel(const Point3& corner,const Vector3& vx,const Vector3& vy,const Vector3& vz,const int& label_num,const std::vector<Face>& faces,const std::vector<Point3>& vertices)
    :Voxel(corner,vx,vy,vz){
        Point3 p0 = corner;
        Point3 p1 = corner+vx;
        Point3 p2 = corner+vx+vy;
        Point3 p3 = corner+vy;
        Point3 p4 = corner+vy+vz;
        Point3 p5 = corner+vz;
        Point3 p6 = corner+vx+vz;
        Point3 p7 = corner+vx+vy+vz;
        std::array<Point3,8> cube_vertices ={p0,p1,p2,p3,p4,p5,p6,p7}; 

        for(auto face: faces){
            bool intersect_or_not_with_this_face = intersect_faces_cubes(face,cube_vertices,vertices);
            if (intersect_or_not_with_this_face) {
                this->label = label_num;
                std::cout<<"one voxel is labelled"<<std::endl;
                break;
            };

        }

    }
    bool intersect_faces_cubes(const Face& face,const std::array<Point3,8>& cube_vertices,const std::vector<Point3>& vertices){
        
        Point3 p0 = vertices[face.indices[0]];
        Point3 p1 = vertices[face.indices[1]];
        Point3 p2 = vertices[face.indices[2]];
        Triangle3 triangle(p0, p1, p2);
        Point3 end1 =  (cube_vertices[0]+cube_vertices[1]+cube_vertices[2]+cube_vertices[3])/4;
        Point3 start1 =  (cube_vertices[4]+cube_vertices[5]+cube_vertices[6]+cube_vertices[7])/4;
        Point3 end2 =  (cube_vertices[1]+cube_vertices[2]+cube_vertices[7]+cube_vertices[6])/4;
        Point3 start2 =  (cube_vertices[0]+cube_vertices[3]+cube_vertices[4]+cube_vertices[5])/4;
        Point3 end3 =  (cube_vertices[0]+cube_vertices[1]+cube_vertices[6]+cube_vertices[5])/4;
        Point3 start3 =  (cube_vertices[2]+cube_vertices[3]+cube_vertices[4]+cube_vertices[7])/4;
        Segment3 segment1(end1,start1);
        Segment3 segment2(end2,start2);

        Segment3 segment3(end3,start3);
        bool condition1 = CGAL::do_intersect(segment1, triangle);
        bool condition2 = CGAL::do_intersect(segment2, triangle);
        bool condition3 = CGAL::do_intersect(segment3, triangle);
        if (condition1 || condition2 || condition3){
            return true;
        }
        else
           return false;




}
    Point3 corner;
    int label;
};

struct VoxelGrid {

  
  
  public:
    std::vector<Voxel> voxels;
    unsigned int max_x, max_y, max_z;
    double unit;
    Point3 initial_point;
    Vector3 vx,vy,vz;

    //
    VoxelGrid(const std::array<Point3, 8> & oobb, const double& unit) {
       
        vx = oobb[1]-oobb[0];
        vy = oobb[3]-oobb[0];
        vz = oobb[5]-oobb[0];
        // normalize it and calculate length
        double lengthx = CGAL::sqrt(vx.squared_length());
        double lengthy = CGAL::sqrt(vy.squared_length());
        double lengthz = CGAL::sqrt(vz.squared_length());
        max_x = int(lengthx/unit)+3;
        max_y = int(lengthy/unit)+3;
        max_z = int(lengthz/unit)+3;
        vx =  vx*unit/lengthx;
        vy =  vy*unit/lengthy;
        vz =  vz*unit/lengthz;
        initial_point = oobb[0]-vx;
        initial_point -=vy;
        initial_point -=vz;

        int total_voxels = max_x*max_y*max_z;
        //push_voxel(max_x, max_y,max_z);
        //std::cout<<total_voxels<<"total"<<std::endl;
        /*for(int x = 0;x<max_x;++x){
            for(int y= 0;y<max_y;++y){
                for(int z = 0;z<max_z;++z){
                    Point3 corner = initial_point+x*vx+y*vy+z*vz;
        //            std::cout<<corner<<" a corner of voxel is "<<std::endl;
                    voxels.push_back(Voxel(corner,vx,vy,vz));
                }
            }
        }*/


           }
    VoxelGrid(const std::array<Point3, 8> & oobb, const double& unit,const std::vector<Face>& faces, std::vector<Point3> vertices)
    :VoxelGrid(oobb, unit){

    }

    void push_voxel(){
        for(int x = 0;x<max_x;++x){
            for(int y= 0;y<max_y;++y){
                for(int z = 0;z<max_z;++z){
                    Point3 corner = initial_point+x*vx+y*vy+z*vz;
        //            std::cout<<corner<<" a corner of voxel is "<<std::endl;
                    voxels.push_back(Voxel(corner,vx,vy,vz));
                }
            }
        }

    }
    void push_voxel(const std::vector<Face>& faces, const std::vector<Point3>& vertices){
        for(int x = 0;x<max_x;++x){
            for(int y= 0;y<max_y;++y){
                for(int z = 0;z<max_z;++z){
                    Point3 corner = initial_point+x*vx+y*vy+z*vz;
        //            std::cout<<corner<<" a corner of voxel is "<<std::endl;
                    voxels.push_back(Voxel(corner,vx,vy,vz,1,faces,vertices));
                }
            }
        }

    }
    void out_put_all_voxel_to_obj(const std::string& filename,const int& extertior_label=0){
        std::ofstream out(filename);
        if (!out.is_open()) {std::cerr << "Error: could not open file \"" << filename << "\" for writing." << std::endl; return;}
        out << "# OBJ file generated by output_voxels_as_obj()" << std::endl;
        int vertex_index = 1;
        for(auto voxel:voxels){
            if (voxel.label==extertior_label) continue;
            Point3 p0 = voxel.corner;
            Point3 p1 = p0 + this->vx;
            Point3 p2 = p0 + this->vx+this->vy;
            Point3 p3 = p0+this->vy;
            Point3 p4 = p0+this->vy+this->vz;
            Point3 p5 = p0+this->vz;
            Point3 p6 = p0+this->vx+this->vz;
            Point3 p7 = p0+this->vx+this->vz+this->vy;


            out << "v " <<  p0.x()<< " " << p0.y() << " " << p0.z()<< std::endl; //p0
            out << "v " <<  p1.x()<< " " << p1.y() << " " << p1.z()<< std::endl; //p1
            out << "v " <<  p2.x()<< " " << p2.y() << " " << p2.z()<< std::endl; //p2
            out << "v " <<  p3.x()<< " " << p3.y() << " " << p3.z()<< std::endl; //p3
            out << "v " <<  p4.x()<< " " << p4.y() << " " << p4.z()<< std::endl; //p4
            out << "v " <<  p5.x()<< " " << p5.y() << " " << p5.z()<< std::endl; //p5
            out << "v " <<  p6.x()<< " " << p6.y() << " " << p6.z()<< std::endl; //p6
            out << "v " <<  p7.x()<< " " << p7.y() << " " << p7.z()<< std::endl; //p7
            out << "f " << vertex_index << " " << vertex_index + 1 << " " << vertex_index + 2 << " " << vertex_index + 3 << std::endl;
            out << "f " << vertex_index << " " << vertex_index + 1 << " " << vertex_index + 6 << " " << vertex_index + 5 << std::endl;
            out << "f " << vertex_index+1 << " " << vertex_index + 2 << " " << vertex_index + 7 << " " << vertex_index + 6 << std::endl;
            out << "f " << vertex_index +3<< " " << vertex_index + 2 << " " << vertex_index + 7 << " " << vertex_index + 4 << std::endl;
            out << "f " << vertex_index << " " << vertex_index + 3 << " " << vertex_index + 4 << " " << vertex_index + 5 << std::endl;
            out << "f " << vertex_index+4 << " " << vertex_index + 7 << " " << vertex_index + 6 << " " << vertex_index + 5 << std::endl;
            vertex_index+=8;
             }
        out.close();

}


    json write_voxels_grid_tojson(const int& unwanted_label = -9999){
        assert(voxels.size()>0);
        json j;
        j["type"] = "CityJSON";
        j["version"] = "1.1";
        j["transform"] = nlohmann::json::object();
        j["transform"]["scale"] = nlohmann::json::array({1.0, 1.0, 1.0});
        j["transform"]["translate"] = nlohmann::json::array({0.0, 0.0, 0.0});
        j["CityObjects"] = nlohmann::json::object();
        return j;
        


        }
    
    
    /*VoxelGrid(unsigned int x, unsigned int y, unsigned int z) {
    max_x = x;
    max_y = y;
    max_z = z;
    Ve
    unsigned int total_voxels = x*y*z;
    voxels.reserve(total_voxels);
    for (unsigned int i = 0; i < total_voxels; ++i) voxels.push_back(0);
  }*/

  
  Voxel &operator()(const unsigned int &x, const unsigned int &y, const unsigned int &z) {
    assert(x >= 0 && x < max_x);
    assert(y >= 0 && y < max_y);
    assert(z >= 0 && z < max_z);
    return voxels[x + y*max_x + z*max_x*max_y];
  }
  
  Voxel operator()(const unsigned int &x, const unsigned int &y, const unsigned int &z) const {
    assert(x >= 0 && x < max_x);
    assert(y >= 0 && y < max_y);
    assert(z >= 0 && z < max_z);
    return voxels[x + y*max_x + z*max_x*max_y];
  }
};

#endif 

