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
