# ElecDev Graphics Application

This is the graphics application that is to be used with the ElecDev software.  It is built to be a standalone executable that can interface with Excel.

### Software And Dependencies

* Language: C++, [https://en.wikipedia.org/wiki/C%2B%2B].  
* GUI Library: ImGUI, [https://github.com/ocornut/imgui].  
* Graphics Backend: OpenGL, [https://www.opengl.org//].  
* GLFW (Window): [https://www.glfw.org/].  
* OpenGL Loader (GLAD): [https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library#glad_.28Multi-Language_GL.2FGLES.2FEGL.2FGLX.2FWGL_Loader-Generator.29], [https://glad.dav1d.de/].
* OpenGL Maths (GLM): [https://github.com/g-truc/glm].

*Note: The ImGUI docs can be found in the ImGUI.h file, it is not online or on the Git repo!*

### OpenGL Learning Resources

* LearnOpenGL: [https://learnopengl.com/Introduction].
* The Cherno: [https://www.youtube.com/user/TheChernoProject].
* Udemy course: [https://www.udemy.com/course/graphics-with-modern-opengl/].  Only buy the courses when they are on discount!
* OpenGL Docs: [http://docs.gl/].

***IMPORTANT**: Do NOT use Legacy OpenGL, only use Modern OpenGL!*

---

## To Do / Fixing

#### Alex
* Mouse events are handled by making the GraphicsHandler object global.  Find another way.
* Seperate OpenGL drawing and window events on different threads (Resizing not working properly).

#### Raynhardt 
* Use the docking branch from ImGUI.

---

## Roadmap

* Setup ImGUI, GLAD, GLFW and OpenGL. ✓
* Create first basic app with simple GUI interface and simple OpenGL drawing. ✓
* Interface with Excel.
* Port software from the PyQt interface.
* Improve engine with a better UI:
  * Auto alignment.
  * Move components.
* Add symbol library.
* Add template circuits.
* Able to enter a circuit and define it from the inside.  Add as many layers as needed.
* 3D Drawings.
* VERY long term: port to Vulkan, [https://www.vulkan.org/].

---

*Estimated money to be made = Baie x Baie = Mal Baie.*
