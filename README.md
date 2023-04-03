# ifc_voxelisation_cityjson_store
vocalisation ifc files
# current progress
## 1. From IFC to OBJ done
## 2. From OBJ to memory done
## 3. The voxel grid done
## 4. Voxelisation of triangles done
current effect:
we can voxelisation all the voxels and output voxels intersected with building's faces:
<img width="807" alt="image" src="https://user-images.githubusercontent.com/60132725/229597556-8247f174-0dac-42ea-95f2-091e32e7c5ca.png">

<img width="803" alt="image" src="https://user-images.githubusercontent.com/60132725/229597660-fe129ea5-8009-4de6-a1b1-86fc802c0964.png">
inside the building: ladder:
<img width="1473" alt="image" src="https://user-images.githubusercontent.com/60132725/229598302-ce10067d-aac6-42f0-8199-4202125fb6f7.png">

#next jobs u can do:
## slightly change the voxel face intersect method:
current method we stored the faces together and then intersect voxels with the whole faces,
change the push_voxel function in voxel class, change the vector<faces> to map<string, object>, and label it according to the targeted labels
## label and voxels which is surface and room, 
iterate the voxels and according to its connected voxel label to label un labelled voxel: label is 0
## speed up voxelisation: 
calculate bounding box of one face and extract sourrounding voxels to speed up, not like current method iterate the whole voxels
## write an function output voxelgrid class to cityjson
with label, object and building according to format and preserve building id, building parts and so on

