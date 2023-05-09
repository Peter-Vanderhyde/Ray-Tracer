# Ray-Tracer
A C++ ray tracer created as a school project.

## Setting Up a Scene Text File
### Parameter Guide
A guide on what types are used for the various parameters within the scene txt file.
```
<emitting> -> true/false
<fuzziness> -> float
<index_ratio> -> float
<color> -> RGB tuple no commas (r g b)
<coords> -> XYZ tuple no commas (x y z)
<material name> -> The name set in your material under <name>
<texture name> -> The name set in your texture under <name>
<file name> -> string
<looking at>/<up direction> -> Vector tuple no commas (x y z)
```
### Creating a Material
```
material <name> diffuse <emitting bool>
material <name> specular <emitting bool>
material <name> metal <emitting bool> <fuzziness 0.0=clear>
material <name> glass <emitting bool> <index_ratio i.e. how much the light bends 1.0=none>  
```
### Creating a Texture
```
texture <name> solid <color tuple (no commas)>
texture <name> gradient <primary color> <secondary color> <num of stripes> <stripe width 0.0-1.0>
texture <name> squares <primary color> <secondary color>
texture <name> checkered <primary color> <secondary color>
texture <name> image <file name>
```
### Creating a Objects
```
sphere <center coords> <radius> <material name> <texture name>
triangle <coord 1> <coord 2> <coord 3> <material name> <texture name>
plane <coord 1> <coord 2> <center coord> <material name> <texture name>
```
### Creating a Mesh
```
mesh <place coord> <file name> <scale> <rotation> <material name> <texture name>
```
### Creating an Obj
```
obj <place coord> <file name> <sections to render -1=all> <material name> <texture name>
```
### Setting Up Scene
```
rays <bounces> <num of rays>
threads <num of threads>
camera <position coord> <looking at> <up direction> <fov>
pixels <width> <height>
output <file name>
```

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
