# Simple OpenGL engine

This engine was built by reading the first few chapters of the [learn opengl](https://learnopengl.com/) book.

## Engine features

### Simple lighting system

* Phong based lighting system
* Basic light types
    * Directional 
    * Point
    * Spot

### Basic Material system

* Opaque or transparent (Alpha blending) materials can be created.
* Texture mapping. (Diffuse, Specular)

### Model system
* Creates primitive Model (Cube, sphere) 
* Imports models using the assimp library
* Supports multi mesh models (Scene graph).

### Basic scene graph

* The camera, the models and the lights are Entities managed by the Scene Class.
* Parent-child transform hierarchy
* Update callback can be added to an Entity, to be then called by the Scene object each frame.

### Simple FPS style Camera

* Controlled with WASD-QE and the mouse.

### Others

* 6 sided Skybox
* Face culling
* Depth buffer debug shader.


![](media/demo.gif)

