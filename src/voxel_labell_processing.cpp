#include "voxel_labell_processing.h"

std::vector<std::vector<int>> extract_exterior_surface_voxel_index(const int& exterior_labell, const VoxelGrid& voxelgrid){
    std::vector<std::vector<int>> exterior_voxels;
    for(int x = 0; x <= voxelgrid.max_x; x++){
        for(int y = 0; y <= voxelgrid.max_y; y++){
            for(int z = 0; z <= voxelgrid.max_z; z++){
                
                
                for(auto& neighbour: voxelgrid.get_neighbours(x,y,z)){
                    int x_neighbour = neighbour[0];
                    int y_neighbour = neighbour[1];
                    int z_neighbour = neighbour[2];
                    Voxel neighbour_voxel = voxelgrid(x_neighbour,y_neighbour,z_neighbour);
                    if(neighbour_voxel.label == exterior_labell){
                        std::vector<int> voxel_index = {x,y,z};
                        exterior_voxels.push_back(voxel_index);
                        break;
                    };
                };
            };
        }
    }
    return exterior_voxels;
}

void output_exterior_surface_to_obj(const std::string &output_filename, const std::vector<std::vector<int>> exterior_voxels,
                                const VoxelGrid& voxelgrid){
                                    std::ofstream output_file;
                                    output_file.open(output_filename);
                                    int vertex_index = 1;
                                    for(auto& voxel_index: exterior_voxels){
                                        int x = voxel_index[0];
                                        int y = voxel_index[1];
                                        int z = voxel_index[2];
                                        Voxel voxel = voxelgrid(x,y,z);
                                        Point3 p0 = voxel.corner;
                                        Point3 p1 = p0 + voxelgrid->vx;
                                        Point3 p2 = p0 + voxelgrid->vx+voxelgrid->vy;
                                        Point3 p3 = p0+voxelgrid->vy;
                                        Point3 p4 = p0+voxelgrid->vy+voxelgrid->vz;
                                        Point3 p5 = p0+voxelgrid->vz;
                                        Point3 p6 = p0+voxelgrid->vx+voxelgrid->vz;
                                        Point3 p7 = p0+voxelgrid->vx+voxelgrid->vz+voxelgrid->vy;
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
                                        vertex_index+=8;}
                                        output_file.close();
                                        }

                                        