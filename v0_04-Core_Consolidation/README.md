# Core Engine Consolidation
In this version, there are a few objectives for the project:
- Make sense of the core engine :heavy_check_mark:
- Sketch a preliminary documentation :o:
	- Camera :heavy_check_mark:
	- IBO´s, VBO´s, VBL´s and VAO´s :heavy_check_mark:
	- Texture :o:
	- Shaders
	- Renderer

	- ImgLoader
	- Entity
	- Sprite-Renderer
	- Bitmap-Font
	- Freetype
- Consolidate the core engine features with the Entity class

This README by itself will be used as such sketch in order to facilitate future understanding of the inner workings of the engine. Thus:

## Development Plan Notes
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

## Core engine Documentation
The engine is structured mainly with an DOP desing in mind.
Here are the purpose and usage of some of the classes:

### Core Engine:
#### - Camera
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

ProcessKeyBoard() expects a direction from the Camera_Movement enum: *FORWARD*, *BACKWARD*, *LEFT*, *RIGHT*.

>[!NOTE]
>:warning: The camera class __does not define which key is responsible for each direction__, that must be handled by the user´s code. A valid diretion command would look as follows:

```C++
if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	camera.ProcessKeyboard(FORWARD, Renderer::GetDeltaTime());
```

ProcessMouseMovement() updates the camera angles based on mouse delta (how much it moved between renderings).\
ProcessMouseScroll() acts like a vertical camera shift, changing the value of y position.
>[!CAUTION]
>The Pitch is constrained by defaut to ±89° to prevent gimbal lock, but can be deactivated by setting *constrainPitch* as *true*.

The following variables are public and available for forced changes if needed:
- glm::vec3 Position;
- glm::vec3 Front;
- glm::vec3 Up;
- glm::vec3 Right;
- glm::vec3 WorldUp;

- float Yaw;
- float Pitch;

- float MovementSpeed;
- float MouseSensitivity;
- float Zoom;

#### - VAO´s, VBO´s, VBL´s and IBO´s:
The idea behind these Buffer Objects is to encase complexity into ease of usage. The pipeline becomes simple and the pratical usage comes with the resulting VAO:

__Pass vertex data to VBO | Describe VBO data on a VBL -> Gather everything into a single VAO | Define VAO with IBO -> Pass info to GPU__

The first step is passing the data. It is performed via VBO´s constructor or Update():

```C++
VertexBuffer(const void* data, unsigned int size, GL_STATIC_DRAW);
void Update(const void* data, unsigned int size, unsigned int offset);
```

*data* should be passed as a C-style array, as OpenGL expects raw memory pointers. However, it is fine to store the data as standard C++ containers for reference when needed. 

*size* byte size of the _data_ container (e.g. _type_ * _n of values on the container_).

*offset* byte size of the vector data to be updated on the buffer from which to start changing values.

A key point is that, while the constructor creates a VBO to store all the vertex data of any object, the Update() function can change a chunk of data points at each vector set. An example of usage would be as follows, where the aim is to pass an array to modify 32 values out of 1000 vertices that ocupy 32 bytes each, stored into a previously created VBO:

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

The VBL class is a helper to avoid having to continously pass information about the data into a VBO with every draw call. In these are stored the stride values for every vertex data set on any VBO. A Push() function is provided for this with type safety baked into it to avoid conflict with *GLtypes* vs *C++ types* (currently includes *float* and *int*).

```C++
void Push(const int count)
const std::vector<VertexAttribute> GetElements() const
unsigned int GetStride() const
```

GetElements() returns a constant vector object containing all the elements added thus far to the VBL.
GetStride() returns how many values each vertex data set has (e.g. a vertex with an offset of 8 values might contain 3 floats for XYZ space; 2 for UV mapping and 3 more for normal lighting)

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
- Provides Bind() and Unbind() methods for explicit state control during setup and rendering.

>[!NOTE]
>When attributing vertex data info into an VAO there´s no need to use Bind() as its executed internally. Unbind() should be used if the need to deal with other VAO´s arise.

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

#### Texture

#### Shader
#### Renderer

### World:
#### Things
#### Entity
#### Model and Mesh

### Modules:
#### Bitmap Font
#### Freetype Font

# References
External sources:
1. Bitmap text rendering was made using an implementation of [CBFG](https://github.com/CodeheadUK/CBFG) and his example code with some changes;
2. [Learn OpenGL](https://learnopengl.com/) book site;
3. The [Cherno`s youtube channel](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw), OpenGL series;
4. [The FreeType Project](https://freetype.org/) (All rights reserved);