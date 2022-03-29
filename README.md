# Lumen Design Engine

![Lumen](https://user-images.githubusercontent.com/81622310/160583584-94b8843a-3082-449c-aa62-c6cbbf275228.png)

This is the graphics application that is to be used with the ElecDev software.  It is built to be a standalone executable that can interface with Excel.

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

Port to Vulkan, [https://www.vulkan.org/].

---

*Estimated money to be made = Baie^Baie = Mal Baie.*
