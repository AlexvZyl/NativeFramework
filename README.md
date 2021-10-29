# ElecDev Graphics Application

This is the graphics application that is to be used with the ElecDev software.  It is built to be a standalone executable that can interface with Excel.

### Software And Dependencies

* Language: C++, [https://en.wikipedia.org/wiki/C%2B%2B].  
* GUI Library: ImGUI, [https://github.com/ocornut/imgui].  This project uses the docking branch.  
* Graphics Backend: OpenGL, [https://www.opengl.org//].  
* GLFW (Window): [https://www.glfw.org/].  
* OpenGL Loader (GLAD): [https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library#glad_.28Multi-Language_GL.2FGLES.2FEGL.2FGLX.2FWGL_Loader-Generator.29], [https://glad.dav1d.de/].
* OpenGL Maths (GLM): [https://github.com/g-truc/glm].
* Must have program for debugging graphics, [https://renderdoc.org/].

*Note: The ImGUI docs can be found in the ImGUI.h file, it is not online or on the Git repo!*

### OpenGL Learning Resources

* LearnOpenGL: [https://learnopengl.com/Introduction].
* The Cherno: [https://www.youtube.com/user/TheChernoProject]. The best one!
* Udemy course: [https://www.udemy.com/course/graphics-with-modern-opengl/].  Only buy the courses when they are on discount!
* OpenGL Docs: [http://docs.gl/]. Very handy to check functions, versions and deprecations.
* Also has an OpenGL course: [https://www.youtube.com/channel/UCUkRj4qoT1bsWpE_C8lZYoQ].
* Also good: [https://www.youtube.com/c/OGLDEV/featured].

***IMPORTANT**: Do NOT use Legacy OpenGL, only use Modern OpenGL!*

---

## Graphics API

---

## Legend:

* ⬜ = Incomplete.
* 🟨 = No longer required.
* ✅ = Completed.

---

## Fixing & Improving

#### Alex
* ⬜ Mouse events are handled by making the GraphicsHandler object global.  Find another way.
* ⬜ Combine basic, textured and text rendering (VAO's and shaders) to reduce draw calls and shader switching.
* ⬜ Error handler uses deprecated OpenGL functions that causes problems on GPU's that does not have good compatibility.  Replace with another handler (glDebugMessageCallback?).
* ⬜ Fix cursor position calculation with text in the text renderer.
* ⬜ What happens when deltaTime() gets called twice in the same frame in the camera handler?
* ⬜ Make function calling in the GraphicsAPI generic (use function pointers?).

---

## Roadmap

### ElecDev App General

- ⬜ Implement logging system with spdlog.
- ⬜ Overhaul event system and mouse handling. (Mouse events are combined between ImGUI and GLFW.  This might be bad coding)
- ✅ Implement MSAA with the FBO's.

### Base 2D Engine

- ✅ Structure code and setup handlers:
    - ✅ Main app.
    - ✅ GUI Handler.
    - ✅ Graphics Handler (Backend Engines).
- ✅ Setup project with dependancies:
    - ✅ ImGUI.
    - ✅ GLAD.
    - ✅ GLFW.
    - ✅ OpenGL.
    - ✅ GLM.
    - ✅ ImGUI Docking Branch.
- ✅ Implement matrix mathematics.
    - ✅ Translation, rotation & scaling.
    - ✅ Viewport, Model, View and Porjection matrices.
- ✅ Error handler.
- ✅ Shader handler.
  - ✅ Basic Shader.
  - ✅ Static Shader.
  - ✅ Texture Shader.
  - ✅ Loading shaders from .shadere files.
  - ✅ Compiling and using shader via a class.
- ✅ Implement class that handles VAO's:
  - ✅ Lines.
  - ✅ Triangles.
  - ✅ Textured triangles (Includes text).
- ✅ Implement batch rendering.
- ✅ Draw textured elements.
- ✅ Drawing API:
  - ✅ Lines.
  - ✅ Clear circles.
  - ✅ Filled circles.
  - ✅ Clear quads.
  - ✅ Filled quads.
  - ✅ Clear triangles.
  - ✅ Filled triangles.
  - ✅ Text.
- ✅ Mouse event handler.
- ✅ Implement frame buffers.
- ✅ Implement State Machine that controls the application.
- ✅ Interface with Excel:
  - ✅ Requires exe file that has resource files included (Use .rc files).
  - ✅ Write byte encoder/decoder.
  - ✅ Write thread handler from python side for non-pause interface.
  - ✅ Input handler for threads and linked to state machine.
- ✅ Create terminal API that controls the app.
- ✅ *It is up and running!*
- ⬜ Create first basic app with simple GUI interface and simple OpenGL drawing:
  - ✅ Incorporate Base Engine.
  - ⬜ GUI interface controls OpenGL engines.
  - ⬜ Bind all user inputs to OpenGL actions and state machine updates.
- ✅ Add client side copy of the buffer data to be able to read and change the data.
- ✅ Add autoCenter and autoScale functions.
- ✅ Add a demo drawing and some form of benchmark.
- ✅ Redo the window management.  Make window generic so that it can draw any kind of OpenGL engine.
- ⬜ Implement Index Buffers.  Wil increase performance and reduce memory usage.
- ⬜ Implement culling.
- ⬜ Implement stencil buffers.
- ⬜ Expand texture rendering capabilities.

### Design 2D Engine (Child of Base 2D Engine)

- ⬜ Mouse event handler.
- ⬜ Component generation and buffer management.
- ⬜ Cable generation.
- ⬜ Add virtual mouse point that snaps to parts of the drawings to make editing easier.
- ⬜ Port software from the PyQt interface:
  - ⬜ Create a state machine for type of user interface.
- ⬜ Improve engine with a better UI:
  - ⬜ Auto alignment.
  - ⬜ Move components.
- ⬜ Add symbol library.
- ⬜ Add template circuits.
- ⬜ Able to enter a circuit and define it from the inside.  Add as many layers as needed.
- ⬜ Add helper class the draws lines and highlites components that are selected.

### Base 3D Engine

- ✅ Add Base3D engine that can have a window instance.
- ✅ Implement perspective projection matrix.
- ✅ Create a demo drawing.
- ✅ Add a depth buffer to the FBO.
- ✅ Implement 3D mouse handling:
    - ✅ Scrolling.
    - ✅ Moving.
    - ✅ Clicks.
- ✅ Add a camera class that is used in the 3D engine.
- ⬜ Add skybox.
- ⬜ Implement API.
    - ⬜ Text.
    - ✅ Cuboids.
    - ⬜ Quads.
    - ⬜ Triangles.
    - ⬜ Circles.

### Long Term Dreams

- ⬜ VERY long term: Port to Vulkan, [https://www.vulkan.org/].

---

*Estimated money to be made = Baie^Baie = Mal Baie.*
