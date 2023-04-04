#ifndef __voxel_labell_processing__
#define __voxel_labell_processing__

#include "geomtools.h"
#include "definitions.h"
std::vector<std::vector<int>> extract_exterior_surface_voxel_index(const int& exterior_labell, const VoxelGrid& voxelgrid);
void output_exterior_surface_to_obj(const std::string &output_filename, const std::vector<std::vector<int>> exterior_voxels,
                                const VoxelGrid& voxelgrid);



#endif
