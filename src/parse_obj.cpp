#include "parse_obj.h"

std::map<std::string, Object> objects;


using namespace std;

std::pair<std::vector<Face>,std::map<std::string, Object>> parse_obj(const std::string& filename,std::vector<Point3>& vertices){
	
	
	
	std::vector<Shell> shells;
	std::vector<Face> faces;
	std::ifstream input_stream;
	input_stream.open(filename, std::ios::in);
	std::string line;
	Object new_object;
	Shell new_shell;
	std::map<std::string, Object> object_map;
	while (getline(input_stream, line)) {
		std::istringstream iss(line);
		string token;
		iss>>token;
		if (line.substr(0, 2) == "g "){
			string id;
			iss>>id;
			Object another_new_object;
			another_new_object.id = id;
			if (object_map.size() != 0){
				new_object.shells.push_back(new_shell);
				object_map.insert(std::make_pair(new_object.id, new_object));
				new_object = another_new_object;
			}

			}
		else if (line.substr(0, 2) == "s "){
			if(new_object.shells.size()!=0){
			new_object.shells.push_back(new_shell);
			Shell another_new_shell;
			new_shell = another_new_shell;
		}
		}
		else if (line.substr(0, 2) == "us"){
			string usemtl;
			iss>>usemtl;
			new_shell.use_material = usemtl;

		}
        else if (line.substr(0, 2) == "v ") {
            double x,y,z;
            iss>>x>>y>>z;
            
            vertices.push_back(Point3(x,y,z));
        }
        else if (line.substr(0, 2) == "f ") {
            Face temp_face;
            std::vector<int> pt_indices;

            if (line.find('/') != std::string::npos) {
            	
            	
            	
            	int v1,v2,v3;
            	char c;
            	iss>>v1>>c>>c>>v1>>v2>>c>>c>>v2>>v3>>c>>c>>v3;
            	
            	
            	
            	pt_indices.push_back(v1-1);
            	pt_indices.push_back(v2-1);
            	pt_indices.push_back(v3-1);
            	

            }
            else
            	 {  int v1,v2,v3;
            	 	sscanf(line.c_str(), "f %d %d %d", &v1, &v2, &v3);
            	 	pt_indices.push_back(v1);
            	 	pt_indices.push_back(v2);
            	 	pt_indices.push_back(v3);

            	 }

            temp_face.indices = pt_indices;
            new_shell.faces.push_back(temp_face);

            faces.push_back(temp_face);
        }

    }
    new_object.shells.push_back(new_shell);
	object_map.insert(std::make_pair(new_object.id, new_object));

    
    return std::make_pair(faces,object_map);
}
void stored_faces_obj(std::string filename,const std::vector<Face>& faces,const std::vector<Point3>& vertices){
	std::ofstream out(filename);
	for(auto pt:vertices){
		Point3 p0 = pt;
		out << "v " <<  p0.x()<< " " << p0.y() << " " << p0.z()<< std::endl;
	}
	for(auto face:faces){
		std::vector<int> indices = face.indices;
		out << "f " << indices[0]+1<<"//"<< indices[0]+1<<" " << indices[1]+1<<"//"<< indices[1]+1<<" " << indices[2]+1<<"//"<< indices[2]+1<<std::endl;

	}
	
	out.close();
}
std::array<Point3, 8> generate_oobb_building(const std::vector<Face>& faces,const std::vector<Point3>& vertices){
	std::array<Point3, 8> obb_points;
	vector<Point3> BBD_pt;
	for(auto face:faces){
		BBD_pt.push_back(vertices[face.indices[0]]);
		BBD_pt.push_back(vertices[face.indices[1]]);
		BBD_pt.push_back(vertices[face.indices[2]]);

	}
	CGAL::oriented_bounding_box(BBD_pt, obb_points);
	return obb_points;
}
