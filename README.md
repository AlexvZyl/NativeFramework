# ElecDev Graphics Application

This is the graphics application that is to be used with the ElecDev software.  It is built to be a standalone executable that can interface with Excel.

### Software And Dependencies

* Language: C++, [https://en.wikipedia.org/wiki/C%2B%2B].  
* GUI Library: ImGUI, [https://github.com/ocornut/imgui].  This project uses the docking branch.  
* Graphics Backend: OpenGL, [https://www.opengl.org//].  
* GLFW (Window): [https://www.glfw.org/].  
* OpenGL Loader (GLAD): [https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library#glad_.28Multi-Language_GL.2FGLES.2FEGL.2FGLX.2FWGL_Loader-Generator.29], [https://glad.dav1d.de/].
* OpenGL Maths (GLM): [https://github.com/g-truc/glm].

*Note: The ImGUI docs can be found in the ImGUI.h file, it is not online or on the Git repo!*

### OpenGL Learning Resources

* LearnOpenGL: [https://learnopengl.com/Introduction].
* The Cherno: [https://www.youtube.com/user/TheChernoProject]. The best one!
* Udemy course: [https://www.udemy.com/course/graphics-with-modern-opengl/].  Only buy the courses when they are on discount!
* OpenGL Docs: [http://docs.gl/]. Very handy to check functions, versions and deprecations.

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
* 🟨 Seperate OpenGL drawing and window events on different threads (Resizing not working properly).
* ⬜ OpenGL has a offset that cannot be removed (can be seen in AutoCAD).  Find a way to fix this.
* ⬜ Unbinding is not necessary and costs performance, but helpful for debugging.  Write code so that it unbinds in debug mode but does not do it in release mode.
* ⬜ GL_LINES & GL_TRIANGLES are being used instead of GL_LINE_LOOP & GL_TRIANGLE_FAN.  Unsure if this will have an impact on performance.  Might just be the use of predefined IBO's.
* ✅ Use the docking branch from ImGUI.
* ✅ Use less draw calls by using less buffers.  Only draw lines and triangles.  Will make buffer management more complex.
* ⬜ Combine basic, textured and text rendering (VAO's and shaders) to reduce draw calls and shader switching.
* ⬜ Currently using an older version of the ImGUI docking banch, because they implemented an OpenGL loader that conflicts with GLAD.  Find out why this is a problem and fix.
* ⬜ Error handler uses deprecated OpenGL functions that causes problems on GPU's that does not have good compatibility.  Replace with another handler (glDebugMessageCallback?).
* ✅ Implement "Improved Alpha-Tested Magnification for Vector Textures and Special Effects" from paper [https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf].
* ⬜ Font rendering is done with .fnt & .png files.  This works, but using .ttf files is more modern.  The fist option is used since it is faster to work with a font atlas.
* ⬜ TextRenderer file parsing is hard coded for one specific file.  This has to be fixed.
* ✅ Type casts throwing warning.  Fix.
* ✅ Buffer sizes are currently static, change to dynamic.
* ⬜ Using index buffers might improve performance.
* ✅ Use SDF (Signed Distance Fields) when rendering text.
* ✅ Create a resource loader function.
* ⬜ Mouse events are combined between ImGUI and GLFW.  This might be bad coding.
* ⬜ Fix abstractions.  Code got messy when we started rushing.
* ✅ Autoresize and scaling does not take into account the length of the text string.
* ⬜ Looks like there is some kind of memory leak when resizing the window.
* ⬜ Add text kerning.
* ⬜ Move over to smart pointers for better memory management.

#### Raynhardt 
* ⬜ Multi View port
* ⬜ Multi vieport docking
* ⬜ Resize event need working on
* ⬜ Implement Input catching
* ⬜ Rebuild IO handling

#### Bugs from Ulrich
* ✅ The MCC must open in the middle of the window.
* ⬜ If a lot of drawings are drawn, the last drawing is not completely finished. I do not know the reason for this, but if I draw a new MCC the old one is drawn completely.
* ⬜ I cannot resize the windows of the drawings.
* ✅ I want to be able to close a drawing window (per MCC).

---

## Roadmap

### Base Engine

* ✅ Structure code and setup handlers:
  * ✅ Main app.
  * ✅ GUI Handler.
  * ✅ Graphics Handler (Backend Engines).
* ✅ Setup project with dependancies:
  * ✅ ImGUI.
  * ✅ GLAD.
  * ✅ GLFW.
  * ✅ OpenGL.
  * ✅ GLM.
  * ✅ ImGUI Docking Branch.
* ✅ Base Engine:
  * ✅ Implement matrix mathematics.
    * ✅ Translation, rotation & scaling.
    * ✅ Viewport, Model, View and Porjection matrices.
  * ✅ Basic shader.
  * ✅ Error handler.
  * ✅ Shader handler.
  * ✅ Implement class that handles VAO's:
    * ✅ Lines.
    * ✅ Triangles.
    * ✅ Textured triangles (Includes text).
  * ✅ Implement batch rendering.
  * ✅ Draw textured elements.
  * ✅ Drawing API:
    * ✅ Lines.
    * ✅ Clear circles.
    * ✅ Filled circles.
    * ✅ Clear quads.
    * ✅ Filled quads.
    * ✅ Clear triangles.
    * ✅ Filled triangles.
    * ✅ Text.
  * ✅ Mouse event handler.
  * ✅ Implement frame buffers.
* ✅ Implement State Machine that controls the application.
* ✅ Interface with Excel:
  * ✅ Requires exe file that has resource files included (Use .rc files).
  * ✅ Write byte encoder/decoder.
  * ✅ Write thread handler from python side for non-pause interface.
  * ✅ Input handler for threads and linked to state machine.
* ✅ Create terminal API that controls the app.

* ✅ It is up and running!* 

* ⬜ Create first basic app with simple GUI interface and simple OpenGL drawing:
  * ✅ Incorporate Base Engine.
  * ⬜ GUI interface controls OpenGL engines.
  * ⬜ Bind all user inputs to OpenGL actions and state machine updates.
* ✅ Add client side copy of the buffer data to be able to read and change the data.
* ✅ Add autoCenter and autoScale functions.
* ⬜ Optimize base engine.
* ⬜ Add a demo drawing and some form of benchmark.
* ⬜ Redo the window management.  Make window generic so that it can draw any kind of OpenGL engine.

### Design Engine (Child of Base Engine)

* ⬜ Mouse event handler.
* ⬜ Component generation and buffer management.
* ⬜ Cable generation.
* ⬜ Add virtual mouse point that snaps to parts of the drawings to make editing easier.
* ⬜ Port software from the PyQt interface:
  * ⬜ Create a state machine for type of user interface.
* ⬜ Improve engine with a better UI:
  * ⬜ Auto alignment.
  * ⬜ Move components.
* ⬜ Add symbol library.
* ⬜ Add template circuits.
* ⬜ Able to enter a circuit and define it from the inside.  Add as many layers as needed.

### Long Term Dreams

* ⬜ 3D Drawings.
* ⬜ VERY long term: Port to Vulkan, [https://www.vulkan.org/].

---

*Estimated money to be made = Baie^Baie = Mal Baie.*
