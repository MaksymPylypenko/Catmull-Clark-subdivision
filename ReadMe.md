# Cutmull Clark subdivision
* Author: Maksym Pylypenko
* Email: pylypenm@myumanitoba.ca

![Image](https://github.com/MaksymPylypenko/Catmull-Clark-subdivision/blob/master/subdivision.png)

## How to run?
1. Open a VS project **`opengl.sln`**
2. Press run (a new window should appear)

## Controls
* `W`		- subdivide
* `S`		- undo subdivision
* `SPACE`	- pause/continue
* `Q`		- quit 

### Models
You can also select different models
* `1` - Cube
* `2` - Cross
* `3` - Torus
* `4` - Tetrahedron
* `5` - Dodecahedron <-- [source](https://people.sc.fsu.edu/~jburkardt/data/obj/obj.html)
* `6` - Gourd <-- [source](https://people.sc.fsu.edu/~jburkardt/data/obj/obj.html)

## Implemented features:
* Catmull Clark Subdivision
* Deafault models (e.g Cube, Cross, Torus, Tetrahedron)
- Complex geometry support (from `.obj`)
- Support for non quad polygons (e.g. models 4-6 use triangles instead of quads)

## Notes
The program was successfully tested using:
* OS: Windows 10
* IDE: Visual Studio 2019 16.4
* Graphics card: Intel® HD Graphics 3000
