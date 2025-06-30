# LittleBigEngine: Core Engine Consolidation
![Version](https://img.shields.io/badge/version-v0__04-blue)
![Status](https://img.shields.io/badge/status-in_development-yellow)
![Documentation](https://img.shields.io/badge/documentation-in_development-yellow)
![Language](https://img.shields.io/badge/language-C++11-blue)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3+-orange)

In this version, there are a few objectives for the project:
- Make sense of the core engine :heavy_check_mark:
- Sketch a preliminary documentation :o:
	- [Core](#core-engine)
		- [Camera](#--camera) :heavy_check_mark:
		- [IBO´s, VBO´s, VBL´s and VAO´s](#--vaos-vbos-vbls-and-ibos) :heavy_check_mark:
		- [Texture](#texture) :x:
		- [Shaders](#--shader) :heavy_check_mark:
		- [Renderer](#--renderer) :heavy_check_mark:

	- [World](#world)
		- [Things](#--things)
		- [Entity](#--entity)
		- [Model and Mesh](#--model-and-mesh)

	- [Modules](#modules)
		- [ImgLoader](#--imgloader)
		- [Bitmap-Font](#--bitmap-font) :x:
		- [Freetype-Font](#--freetype-font) :x:
- Consolidate the core engine features with the Entity class :heavy_check_mark:

This README by itself will be used as such sketch in order to facilitate future understanding of the inner workings of the engine. Thus:

# Development Plan Notes
- v0.04: README refactoring
- v0.04: Core engine consolidation
	- v0.04.00: Sketch a working example of usage of the current features :heavy_check_mark:
	- v0.04.01: Consolidate Vertex, Texture and Shader Data with Entity class :o:
	- v0.04.02: Rebuild Texture Class

- v0.05: Module and Third party resources consolidation
	- v0.05.00: Add modular support for classes at compile time
	- v0.05.01: Rebuild bitmap_font class inheriting from Entity
	- v0.05.02: Add Freetype implementation class inheriting from Entity
	- v0.05.03: Implement Batch rendeding for the text classes
	- v0.05.04: Inclusion of Bitmap and Freetype as modules of the core engine
	- v0.05.05: Add OpenAL sound API
	- v0.05.06: Implement sound class
	- v0.05.07: Inclusion of sound class as part of the core engine

# Core engine Documentation
The engine is structured mainly with an DOP desing in mind.
Here are the purpose and usage of some of the classes:

## Core Engine:
### - Camera
This subsystem implements a 3D free-view camera with pitch and yaw controls based on 3 dimensional vectors. The class can be constructed in two ways:

```C++
Camera(glm::vec3 position, glm::vec3 direction, float yaw, float pitch)
Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
```

The first constructor provides standard default values, allowing quick setup - the camera starts at the origin and faces forward (negative Z) according to the right-hand rule.

There are three functions available for camera controls via user input processing:

```C++
void ProcessKeyboard(direction, float deltaTime)
void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
void ProcessMouseScroll(float yOffset)
```

`ProcessKeyBoard()` expects a direction from the Camera_Movement enum: *FORWARD*, *BACKWARD*, *LEFT*, *RIGHT*.

>[!NOTE]
>:warning: The camera class __does not define which key is responsible for each direction__, that must be handled by the user´s code. A valid diretion command would look as follows:

```C++
if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	camera.ProcessKeyboard(FORWARD, Renderer::GetDeltaTime());
```

`ProcessMouseMovement()` updates the camera angles based on mouse delta (how much it moved between renderings).\
`ProcessMouseScroll()` acts like a vertical camera shift, changing the value of y position.
>[!CAUTION]
>The Pitch is constrained by defaut to ±89° to prevent gimbal lock, but can be deactivated by setting *constrainPitch* as *`true`*.

The following variables are public and available for forced changes if needed:
- `glm::vec3` Position;
- `glm::vec3` Front;
- `glm::vec3` Up;
- `glm::vec3` Right;
- `glm::vec3` WorldUp;

- `float` Yaw;
- `float` Pitch;

- `float` MovementSpeed;
- `float` MouseSensitivity;
- `float` Zoom;

---
### - VAO´s, VBO´s, VBL´s and IBO´s:
The idea behind these Buffer Objects is to encase complexity into ease of usage. The pipeline becomes simple and the pratical usage comes with the resulting VAO:

__Pass vertex data to VBO | Describe VBO data on a VBL -> Gather everything into a single VAO | Define VAO with IBO -> Pass info to GPU__

The first step is passing the data. It is performed via VBO´s constructor or `Update()`:

```C++
VertexBuffer(const void* data, unsigned int size, GL_STATIC_DRAW);
void Update(const void* data, unsigned int size, unsigned int offset);
```

*data* should be passed as a C-style array, as OpenGL expects raw memory pointers. However, it is fine to store the data as standard C++ containers for reference when needed. 

*size* byte size of the _data_ container (e.g. _type_ * _n of values on the container_).

*offset* byte size of the vector data to be updated on the buffer from which to start changing values.

A key point is that, while the constructor creates a VBO to store all the vertex data of any object, the `Update()` function can change a chunk of data points at each vector set. An example of usage would be as follows, where the aim is to pass an array to modify 32 values out of 1000 vertices that ocupy 32 bytes each, stored into a previously created VBO:

```C++
unsigned int vertexSize = 32
unsigned int index = 4;
unsigned int offSet = index * vertexSize;

vb.Update(newData, vertexSize, offSet)
```

>[!CAUTION]
>Different vertex formats shoudn´t be mixed in a single VBO.

>[!TIP]
>Multiple vertices can be changed at once by passing a larger size and setting offset to the appropriate start position.

The VBL class is a helper to avoid having to continously pass information about the data into a VBO with every draw call. In these are stored the stride values for every vertex data set on any VBO. A `Push()` function is provided for this with type safety baked into it to avoid conflict with *GLtypes* vs *C++ types* (currently includes *`float`* and *`int`*).

```C++
void Push(const int count)
const std::vector<VertexAttribute> GetElements() const
unsigned int GetStride() const
```

`GetElements()` returns a constant vector object containing all the elements added thus far to the VBL.
`GetStride()` returns how many values each vertex data set has (e.g. a vertex with an offset of 8 values might contain 3 floats for XYZ space; 2 for UV mapping and 3 more for normal lighting)

>[!WARNING]
>After pushing a __new element to a VBL it cannot be removed__.A new VBL must be created to handle a different set of vertex elements.

As a final step, a pair VBO, VBL is added to a VAO using AddBuffer().

```C++
void AddBuffer(const VertexBuffer&, const VertexBufferLayout&);
```

A single VAO may contain as many vertex data/layout pairs as desired. As a rule of thumb, each VBO/VBL should be responsible for storing a single mesh while a VAO might store an entire scene.

IBOs can be used optionally to reduce vertex data repetition and improve performance by defining which vertex points should be used to render which triangles. Its constructor receives a pointer to the array of index values and the number of elements.

```C++
IndexBuffer(const GLuint* data,const GLuint size);
```

Internally each buffer class:
- Automatically generates a unique GPU resource ID in its constructor;
- Implements move constructors and move assignment to transfer ownership of the OpenGL object, transfering ownership and invalidating the source buffer to prevent double deletion or reuse;
- Disables copy semantics to avoid unintended ID duplication, which would otherwise result in undefined behavior or OpenGL errors;
- Uses destructors to safely delete GPU-side resources, ensuring proper cleanup and avoiding memory/resource leaks;
- Provides `Bind()` and `Unbind()` methods for explicit state control during setup and rendering.

>[!NOTE]
>When attributing vertex data info into an VAO there´s no need to use `Bind()` as its executed internally. `Unbind()` should be used if the need to deal with other VAO´s arise.

As an example, a simple setup for rendering a cube with normal and UV data would be as follows:

```C++
std::vector<float> cube_vData = {ALL_THE_CUBE_DATA};
std::vector<unsigned int> indices = {INDEX_DATA};

VertexArray cube_va;
cube_va.Bind();
VertexBuffer cube_vb(&cube_vData[0], sizeof(cube_vData) * cube_vData.size());

VertexBufferLayout cube_vbl;
cube_vbl.Push<float>(3);
cube_vbl.Push<float>(3);
cube_vbl.Push<float>(2);
cube_va.AddBuffer(cube_vb, cube_vbl);

IndexBuffer ib(&indices[0], indices.size());
```

---
### - Texture
__WILL BE REFACTORED ON V0_05__

---
### - Shader
An object of this class is intended to contain shader programs in the form of OpenGL shader objects and handle its lifetime on the GPU. A shader can be instantiated without arguments for later compilation or hot-swapping (the object will internaly handle destruction of any previously compiled shaders) if needed using the `Compile()` function.

```C++
void Compile("res/shaders/<name>.vert", "res/shaders/<name>.frag");
```
- Shader file paths are interpreted as relative to the application location, they should be preferrably stored on the res//shaders/ folder
- Due to a lack of need for the time being geometry shaders were not added to the compilation process and may be included on posterior versions via extension of the Compile function

>[!NOTE]
>All functions run the Use function by default so usually, __there´s no need to manually run `Use()` on a compiled shader program__. 

The `SetUniform()` and `GetUniform()` functions are used to manage (pass and retrieve) data between CPU and GPU shader programs. They accept the uniform´s name (as declared on in the shader file) and the variable itself.
- Both have error handling implemented to prevent invalid type passing or calling;
- Both return a bool indicating success or failure. While the class handles errors by logging them, these return values can be used for manual error handling when needed without execution crashes;
- __The `GetUniform()` will modify the passed variable according to the value currently set on the GPU__; 

Following is an example of usage:
```C++
int uniformValue = 0;
std::string uniformName = "<name>"
bool result = SetUniform(uniformName, uniformValue);

if (result){
	//Do something with the shader
	int uniformNewValue = 0;
	GetUniform(UniformName, uniformNewValue);
	//Do something else with the variable	
} else{
	<Error handling>
}
```

Following is a list of possible variables for passing and getting withouth error throwing:
| Function | Available Types |
| -------- | --------------- |
| `SetUniform()` | `bool`, `int`, `float`, `glm::vec3`, `glm::vec4`, `glm::mat4 `|
| `GetUniform()` | `int`, `float` |

---
### - Renderer
This is the core directly responsible for issuing Draw Calls to the GPU. __It is implemented as a static class managing global rendering states__, including time tracking, space transformations, and rendering mode(2D/3D). Following are the configuration functions to be used:

- `SetRender3D()`:
Enables or disables 3D rendering mode. When `true`, the renderer applies the projections, view, and model matrices. When `false`, rendering occurs directly in screen space.
- `SetProjection(const glm::mat4&)`, `SetView(const glm::mat4&)`, `SetModel(const glm::mat4&)`:
	- __Projection__: Perspective or orthographic camera lens;
	- __View__: Camera transformation (position and orientation)
	- __Model__: Object (mesh/ object) transformation in world space.
- `GetLastFrame()`:
Returns the time elapsed (in seconds) since the last frame rendered;
- `GetDeltaTime()`:
Returns the time difference -delta- (in seconds) between the current frame and the last rendered frame.

>[!Warning]
>As of v0_04: Although `SetRender3D()` defines if matrices are used internaly, __the appropriate OpenGL state must be configured by the user__ to ensure proper rendering (e.g.: `glEnable(DEPTH_TEST)` is necessary for correct 3D depth handling).

>[!Caution]
>As of v0_04: `FrameTimeTracker()` __must be called once per frame__ (typically at the start of the render loop) correctly update both  `GetLastFrame()` and `GetDeltaTime()` otherwise they´ll become outdated with every render call.

- `RenderConfig(const float r = 0.0f, const float& g = 0.0f, const float& b = 0.0f, const float& a = 0.0f)`:
Sets the background color using normalized range `[0.0, 1.0]` RGBA values. Higher values will clamp to maximum saturation `[1.0]`. A default call with no parameters results in a black background.

- `Render()`:
Core rendering function responsible for drawing things __from text to GUI elements and models__  on screen. It should be called by passing references to a *VAO*, *IBO* and a *Shader* objects. 

Following is and example of the simplest call for rendering an object using only the core engine features (from v0_04):

```C++
// vertex and buffers configurations -----------------------------------------
VertexArray example_va;
//VAO configuration with VBOs, VBLs and/or meshes
IndexBuffer example_ib(&data[0], data.size());

// Shader configurations -----------------------------------------
Shader example_shader(
		"example_shader", 
		"res\\shaders\\example_shader.vert", 
		"res\\shaders\\example_shader.frag");
// Shader uniforms setup

// Render loop (happens every frame) -----------------------------------------
while (!glfwWindowShouldClose(window)) {
	// Frame time tracker
	Renderer::FrameTimeTracker();

	// Space configurations and rendering
	Renderer::SetRender3D(true);
	Renderer::RenderConfig();
	glEnable(GL_DEPTH_TEST);

	// World config (Projection setup)
	glm::mat4 projection3D = glm::perspective(
		glm::radians(camera.Zoom), 
		(float)screenWidth / (float)screenHeight, 
		0.1f, 100.0f);
	Renderer::SetProjection(projection3D);

	// Camera config (View)
	glm::mat4 view3D = camera.GetViewMatrix();
	Renderer::SetView(view3D);
	example_shader.SetUniform("viewPos", camera.Position);

	// Model positioning
	glm::mat4 model3D = glm::mat4(1.0f);
	Renderer::SetModel(model3D);

	// Draw call
	example_va.Bind();
	Renderer::Render(example_va, example_ib, example_shader);

	// Check and call events and swap the buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

```

## World:
### - Things

---
### - Entity

---
### - Model and Mesh

## Modules:
### - ImgLoader

### - Bitmap Font
__WILL BE REFACTORED ON V0_05__

---
### - Freetype Font
__WILL BE REFACTORED ON V0_05__

# References
External sources:
1. Bitmap text rendering was made using an implementation of [CBFG](https://github.com/CodeheadUK/CBFG) and his example code with some changes;
2. [Learn OpenGL](https://learnopengl.com/) book site;
3. The [Cherno`s youtube channel](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw), OpenGL series;
4. [The FreeType Project](https://freetype.org/) (All rights reserved);