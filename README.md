# Ray-Tracer
A C++ ray tracer created as a school project.

## Setting Up a Scene Text File
*Keep in Mind*
- Positive x is right, positive y is forward, and positive z is up
- Color values are from 0.0 - 1.0
### Parameter Guide
A guide on what types are used for the various parameters within the scene txt file.
```
<fuzziness> -> float
<index_ratio> -> float
<color> -> RGB tuple no commas (r g b) 0.0-1.0
<coords> -> XYZ tuple no commas (x y z)
<name> -> string
<file name> -> string
<u v> -> the tiling for textures on the horizontal and vertical axes of spheres
<looking at>/<up direction> -> Vector tuple no commas (x y z)
```
### Creating a Material
```
material <name> diffuse
material <name> specular
material <name> metal <fuzziness 0.0=clear>
material <name> glass <index ratio i.e. how much the light bends 1.0=none>  
material <name> light
material <name> flat_glass <index ratio> <thickness>
material <name> gloss <roughness>
material <name> metalic_gloss <roughness> <metalic>
material <name> fog
material <name> point_light
material <name> directional_light <spread angle>
```
### Creating a Texture
```
texture <name> solid <color>
texture <name> gradient <primary color> <secondary color>
texture <name> dots <color>
texture <name> swirl <primary color> <secondary color> <num of stripes> <stripe width 0.0-1.0>
texture <name> squares <primary color> <secondary color>
texture <name> checkered <primary color> <secondary color>
texture <name> image <file name>
```
### Creating a Normal
```
normal <name> <filename> <flip normal axis direction vector>  # (1 -1 1) flips the y-axis normal direction
```
### Creating basic Objects
```
sphere <center> <radius> <u v> <material> <texture>
triangle <coord 1> <coord 2> <coord 3> <material> <texture>
plane <topleft> <botleft> <botright> <material> <texture>
box <position> <dimensions> <rotations> <material> <texture>
```
*Sphere allows for image texture to be used*

### More Advanced Objects
- Textured objects allow image textures to be used
```
textured_triangle <coord 1> <coord 2> <coord 3> <image coord 1> <image coord 2> <image coord 3> <material> <texture>
textured_plane <topleft> <botleft> <botright> <tileXY> <material> <texture>
textured_box <position> <dimensions> <rotations> <tileXY> <material> <texture>
```
- Normal objects allow normal maps to be used along with image textures
```
normal_sphere <center> <radius> <tileXY> <material> <texture> <normal>
normal_triangle <coord 1> <coord 2> <coord 3> <normal coord 1> <normal coord 2> <normal coord 3> <material> <texture> <normal>
normal_plane <topleft> <botleft> <botright> <tileXY> <material> <texture> <normal>
normal_box <position> <dimensions> <rotations> <tileXY> <material> <texture> <normal>
```
- Billboards only collide with rays on one side allowing things like the camera to look into enclosed spaces
```
billboard_triangle <coord 1> <coord 2> <coord 3> <material> <texture>
billboard_plane <topleft> <botleft> <botright> <material> <texture>
```
- Fog objects require that the fog material is used for them
```
fog_sphere <center> <radius> <density> <material> <texture>
fog_box <position> <dimensions> <rotations> <density> <material> <texture>
```

### Creating a Mesh
```
mesh <position> <file name> <scale vector> <rotation> <material> <texture>
```
### Creating an Object From a .obj
```
obj <position> <file name> <sections to render -1=all> <scale vector> <material name> <texture name>
```
### Creating a Basic Environment
```
sun <direction vector> <color> <intensity double>
sky <boolean>
```
### Setting Up Scene
```
rays <bounces> <num of rays>
threads <num of threads>
checkpoints <num of checkpoints>
camera <position> <looking at> <up vector> <fov>
lens <distance percentage> <blur amount>
pixels <image width> <image height>
output <file name>
```
- Lens takes two parameters. The first determines where the focus is. When set to 1.0, the focus will be directly at the "looking at" vector camera parameter. 0.5 would focus halfway between the camera position and it's "looking at" vector. The second argument is a double 0.0-inf determining blur amount. 0 makes everything in the scene fully focused.

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
![normal_scene_field](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/dad689d5-743e-4257-b5e0-21314c9281ff)
![portal](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/18263c9a-f2e5-42a4-8f0c-36aba3af18c5)
![motorcycle](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/6a31b266-7657-446d-a7cd-6c34610a5bc0)
![club_ball_2](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/13cc993c-f431-4706-8f59-b5a9a5180a7e)
![metal_earth](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/e0487f92-5898-4c24-80e1-d2beb0ed5a36)
![metal](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/6b381030-afb5-480d-9a9e-b3edfdd0f9a0)
![green_glass](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/b19fdf27-df62-4d7f-a288-68cd4eda413f)
![gradient](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/efefb7f4-b3ac-4194-b5fe-f88a4597e280)
![combined](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/8542102a-62d6-4728-9ca9-1085d01f073b)
![coffee2](https://github.com/Peter-Vanderhyde/Ray-Tracer/assets/71889138/23581cba-7f6b-4c1b-91c2-e7aa6248b0eb)

