## Roadmap

### ElecDev App General

- ⬜ Implement logging system with spdlog.
- ⬜ Overhaul event system and mouse handling. (Mouse events are combined between ImGUI and GLFW.  This might be bad coding)
- ✅ Implement MSAA with the FBO's.

### Base 2D Engine

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

### Software And Dependencies

* Language (C++) : [https://en.wikipedia.org/wiki/C%2B%2B].  
* GUI Library (dear imgui) : [https://github.com/ocornut/imgui].  This project uses the docking branch.  
* Why use an immediate mode GUI? : [https://www.youtube.com/watch?v=Z1qyvQsjK5Y&t=102s].
* Graphics Backend (OpenGL) : [https://www.opengl.org//].  
* OpenGL Loader (GLAD) : [https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library#glad_.28Multi-Language_GL.2FGLES.2FEGL.2FGLX.2FWGL_Loader-Generator.29], [https://glad.dav1d.de/].
* Window (GLFW) : [https://www.glfw.org/].  
* Maths library (GLM) : [https://github.com/g-truc/glm].
* Program for debugging graphics (RenderDoc) : [https://renderdoc.org/].

*Note: The ImGUI docs can be found in the ImGui.h & ImGui.cpp files, as well as on some other places in the github repo.*

### OpenGL Learning Resources

* LearnOpenGL : [https://learnopengl.com/Introduction].
* The Cherno : [https://www.youtube.com/user/TheChernoProject]. The best one!
* Udemy course : [https://www.udemy.com/course/graphics-with-modern-opengl/].  Only buy the courses when they are on discount!
* OpenGL Docs : [http://docs.gl/]. Very handy to check functions, versions and deprecations.
* Also has an OpenGL course : [https://www.youtube.com/channel/UCUkRj4qoT1bsWpE_C8lZYoQ].
* Also good : [https://www.youtube.com/c/OGLDEV/featured].

***IMPORTANT**: Do NOT use Legacy OpenGL, only use Modern OpenGL!*

### Dear ImGUI

* From the GitHub repo : "Suggestion: once in a while, add #define IMGUI_DISABLE_OBSOLETE_FUNCTIONS in your imconfig.h file to make sure you are not using to-be-obsoleted symbols."
* We have gradients! : [https://github.com/ocornut/imgui/issues/4722].
* ImPlot could be very nice : [https://github.com/epezent/implot].

### Long Term

Implement Vulkan renderer, [https://www.vulkan.org/].

