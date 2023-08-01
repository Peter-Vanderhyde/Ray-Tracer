# C++ Ray Tracer
### Peter Vanderhyde *2022-Current*
**Base code provided by Dr. Jeffrey Brown**

### Description
A C++ ray tracer created from scratch as a school project. This ray tracer is heavily based off of the ***Ray Tracing in One Weekend*** book series by Peter Shirley (https://raytracing.github.io/). I have added several personal features beyond the series and may update the project from time to time.

### Features
- Creation of simple shapes such as spheres, planes, and boxes that can be positioned and rotated in the world.
- Creation of shapes using .obj files.
- Creation of simple fog volumes.
- Multi-threaded ray tracing with a simple time completion estimation.
- A checkpoint system to observe the progress of your render as a PNG image. (Also means that most progress can be saved if `Ctrl-C` is pressed)
- A camera that can be moved and rotated with custom FOV.
- Several lighting options including a light material for objects, point lights, and an optional sun effect through global directional light.
- Apply colors or image textures to objects. (Can handle transparent image textures)
- Create a skysphere using a PNG image texture, use built in simple sky background, or just leave it as default black.
- Apply normal map textures or specular map textures to objects.
- Billboard planes that are only collidable from one side. Allows for see-through objects or light only shining one way.
- Create a custom depth of field effect in renders.
- Create mesh objects with custom specified mesh vertices.
- Creates the resulting render as a PNG image with specified dimensions.

## Setting Up a Scene Text File
*Keep in Mind*
- Positive x is right, positive y is forward, and positive z is up
- Color values are from 0.0 - 1.0, not 0-255.
- `Comments` are made with the `#` symbol before a line
- Often vectors are used for directions and positions. These vectors are represented by `tuples`, those values must be written within parentheses with only spaces separating the values. i.e. The color red can be represented as (1 0 0).
### Parameter Guide
A guide on what types are used for the various parameters within the scene txt file.

- `Fuzziness` : This is a `float` value used in metal materials to determine how clear or "fuzzy" the reflectivity of the metal is.
- `Index Ratio` : This is a `float` value which determines how light refracts through glass materials. 1.0 means the light does not refract at all.
- `Color` : This is an RGB `tuple` of values.
- `Coords` : This is a 3D `vector` represented as an (x y z) tuple.
- `Name` : This is a simple `string`.
- `File name` : This is a simple `string`.
- `UV Coords` : This is a `tuple` of two values which represent the tiling of a shape's texture. By default, the values should be set to (1 1), however if you wanted an image to be tiled across the extend of a shape, the values should be set to the tiling amount horizontally and vertically respectively.
- `Looking At` / `Up Direction` : These are both 3D `vectors`. The first represents the 3D position that the camera is looking at and the second is a vector pointing in the direction of up relative to the camera. ***Note that this direction cannot be the same vector as the direction that the camera is looking.***

### Creating a `Material`
```
material <name> diffuse
material <name> specular
material <name> metal <fuzziness 0.0=mirror>
material <name> glass <index ratio i.e. how much the light bends 1.0=none>  
material <name> light
material <name> flat_glass <index ratio> <thickness>
material <name> gloss <roughness>
material <name> metalic_gloss <roughness> <metalic>
material <name> fog
material <name> point_light
material <name> directional_light <spread angle>
```
### Creating a `Texture`
```
texture <name> solid <color>
texture <name> gradient <primary color> <secondary color>
texture <name> dots <color>
texture <name> swirl <primary color> <secondary color> <num of stripes> <stripe width 0.0-1.0>
texture <name> squares <primary color> <secondary color>
texture <name> checkered <primary color> <secondary color>
texture <name> image <file name>
```
Using a PNG image as a texture requires that the image is placed in the `/build/files/images` folder.
### Creating `Normals` Using Normal Map Image Textures
This requires that you have a normal map PNG image placed in `/build/files/normals`.
```
normal <name> <filename> <flip normal axis direction vector>  # (1 -1 1) flips the y-axis normal direction
```
### Creating `Specular Maps` Using Specular Map Image Textures
This requires that you have a black and white PNG image to use as the specular map placed in `/build/files/speculars`.
```
specular <name> <specular_material> <specular_texture> <non_specular_material> <non_specular_texture> <specular_map_filename>
```
### Creating `Basic Objects`
```
sphere <center> <radius> <rotations> <material> <texture>
triangle <coord 1> <coord 2> <coord 3> <material> <texture>
plane <topleft> <botleft> <botright> <material> <texture>
box <position> <dimensions> <rotations> <material> <texture>
```
The basic sphere allows for an image texture to be used. The other shapes do not by default.

### More `Advanced Objects`
---

`Textured objects` allow image textures to be used
```
textured_sphere <center> <radius> <tile_XY> <rotations> <material> <texture>
textured_triangle <coord 1> <coord 2> <coord 3> <image coord 1> <image coord 2> <image coord 3> <material> <texture>
textured_plane <topleft> <botleft> <botright> <tileXY> <material> <texture>
textured_box <position> <dimensions> <rotations> <tileXY> <material> <texture>
```
---
`Normal objects` allow normal maps to be used along with image textures
```
normal_sphere <center> <radius> <tileXY> <material> <texture> <normal>
normal_specular_sphere <center> <radius> <specular_map> <normal> *
normal_triangle <coord 1> <coord 2> <coord 3> <normal coord 1> <normal coord 2> <normal coord 3> <material> <texture> <normal>
normal_plane <topleft> <botleft> <botright> <tileXY> <material> <texture> <normal>
normal_box <position> <dimensions> <rotations> <tileXY> <material> <texture> <normal>
```
\* See the next section for `Specular Map` objects

---
`Specular objects` are used along with `Specular Maps` to apply multiple materials/textures to the same object
```
specular_sphere <center> <radius> <specular_map>
normal_specular_sphere <center> <radius> <specular_map> <normal_map>
```
---
`Billboards` only collide with rays on one side allowing things like the camera to look into enclosed spaces
```
billboard_triangle <coord 1> <coord 2> <coord 3> <material> <texture>
billboard_plane <topleft> <botleft> <botright> <material> <texture>
```
---
`Fog` objects require that the fog material is used for them
```
fog_sphere <center> <radius> <density> <material> <texture>
fog_box <position> <dimensions> <rotations> <density> <material> <texture>
```

### Creating a Mesh
This feature requires that a mesh file has been created:
1. Create a .txt file in `/build/files/meshes`.
2. Write `vertices` on the first line.
3. Each line after, create a position of a mesh vertex relative to (0 0 0).
4. Write `triangles` after the list of vertices.
5. On each line after, write three numbers separated by a space that specify which vertices to connect together i.e. `0 1 2` would mean that the first, second, and third vertices that you defined should be used to create a triangle.  

Here is an example mesh file for a pyramid:
```
vertices
(-0.5 -0.5 0)
(0.5 -0.5 0)
(0.5 0.5 0)
(-0.5 0.5 0)
(0 0 1)

triangles
0 1 4
1 2 4
2 3 4
3 0 4
```
Using the mesh file in the ray tracer:
```
mesh <position> <filename> <scale vector> <rotation> <material> <texture>
```
### Creating an Object From a `.obj File`
This requires that you have an OBJ file in `/build/files/objs`.
```
obj <position> <file name> <sections to render -1=all> <scale vector> <material name> <texture name>
```
Often, OBJ files are broken up into sections, so the parameter to specify the number of sections to render can be used as a way to get an idea of what the object will look like without having to render the entire large object.
### Creating a Basic `Environment`
```
sun <direction vector> <color> <intensity double>
sky <boolean>
skysphere <tile_XY> <texture_filename>
```
### Other `Scene Settings`
```
rays <bounces> <num of rays per pixel>
threads <num of threads>
checkpoints <num of checkpoints>
camera <position> <looking at vector> <up vector> <fov>
lens <distance percentage> <blur amount>
pixels <image width> <image height>
output <file name>
```
- `Lens` takes two parameters. The first determines where the focus is. When set to 1.0, the focus will be directly at the `looking at` vector camera parameter. 0.5 would focus halfway between the camera position and it's "looking at" vector. The second argument is a double 0.0-inf determining blur amount. 0 makes everything in the scene fully focused.
- `Checkpoints` will save the specified number of checkpoints throughout the render. A new image is created each time and placed in `/build/files/checkpoints`. Each time, all checkpoints so far are read and averaged to create a current progress image that is saved in the renders folder under the specified output name. Upon the user hitting `Ctrl-C` to cancel the render, the checkpoints will be averaged and saved in renders as "cancelled_render.png".

## Example Scene
Scene TXT files must be placed in `/build/files/scene_files`.
```
# Materials
material diff diffuse
material light point_light
material metal metal 0.7
material gloss gloss 0.9
material mirror specular

# Textures
texture bright_white solid (4 4 4)
texture white solid (1 1 1)
texture red solid (0.6 0 0)
texture blue solid (0.2 0.2 0.7)
texture green solid (0.2 0.7 0.2)
texture floor solid (0.05 0.05 0.05)

# Objects

# floor
plane (-100 100 0) (-100 -100 0) (100 -100 0) diff floor

box (2 10 2) (4 4 4) (0 0 30) diff red
sphere (-5 0 2) 2 (1 1) (0 0 0) metal blue
sphere (-4 -16 2) 2 (1 1) (0 0 0) mirror green
sphere (6 -8 2) 2 (1 1) (0 0 0) gloss white

sky true
sun (-20 20 -25) 20 (1 1 1) 10

rays 10 100
threads 7
camera (0 -20 2) (0 -8 2) (0 0 1) 70
lens 1 0
pixels 1280 720
output test.png
```
- **Results are placed in `/build/files/renders`.**

### Example Renders
![13](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/4e301d58-7aac-4800-9862-6f92996c4b37)
![21](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/4ee1c515-2ee8-43d2-8894-94d5b0a52c9d)
![25](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/87d62489-9ec9-4a61-8b4e-f9df79952605)
![27](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/65bac249-b96c-4ee8-8fd7-7c655fa00687)
![30](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/b852210e-16ce-4c41-925e-b722322dca64)
![33](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/39ec069d-9561-4c89-8e38-f2240eea1d2d)
![32](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/9dd526e7-9b8b-4f69-8748-e148d6842f5c)
![38](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/30c22fd9-8394-4a5d-b427-dc6d2d536847)
![36](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/b3d5ac58-7ba2-4504-8712-add6ac29828b)
![40](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/541916bd-f4a8-4f49-bcbf-776d578a672c)
![portal](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/18263c9a-f2e5-42a4-8f0c-36aba3af18c5)
![cornell_dof](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/a5da6bd5-1a2c-4b10-bc82-e1f441013539)
![cornell](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/33912d07-0389-49fe-a111-97b982adfcd5)
![blurred](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/9ff681a5-60fe-40f0-a532-ea3020990931)
![white_pawn2](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/7cf0e476-14c3-4885-bde7-ccae2a3985fb)
![motorcycle](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/6a31b266-7657-446d-a7cd-6c34610a5bc0)
![club_ball_2](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/13cc993c-f431-4706-8f59-b5a9a5180a7e)
![metal_earth](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/e0487f92-5898-4c24-80e1-d2beb0ed5a36)
![metal](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/6b381030-afb5-480d-9a9e-b3edfdd0f9a0)
![green_glass](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/b19fdf27-df62-4d7f-a288-68cd4eda413f)
![gradient](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/efefb7f4-b3ac-4194-b5fe-f88a4597e280)
![combined](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/8542102a-62d6-4728-9ca9-1085d01f073b)
![coffee2](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/23581cba-7f6b-4c1b-91c2-e7aa6248b0eb)
![room_dof](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/6b2778c3-ea04-46c5-bae5-babe65fd8d7b)