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
