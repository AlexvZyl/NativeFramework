## The Renderer

In Lumen we have a static `Renderer` class.  This means that there is never going to be an instantation of `Renderer` (the constructor is private).  `Renderer` contains static variables and the functions make use of these variables.  So, instead of having to do this every time:

```C++
Renderer renderer(...);
renderer.function(...);
```

We simply do:

```C++
Renderer::function(...);
```

Lumen supports rendering to multiple scenes (as many as the user requires).  This is implemented by using a `Scene` class.  The contents of `Scene` is not of importance to the developer, but it contains all of the data that a graphics API requires to render a scene.  It includes things such as Vertex Array Objects, Framebuffer Objects, a camera, textures etc.  However, recall that the `Renderer` uses static variables, so this means we have to tell the `Renderer` to which `Scene` it has to render:

```C++
Scene scene(cameraType, width, height);
Renderer::bindScene(&scene);
```

When creating a `Scene` we need to specify what type of `Camera` we require, since Lumen supports 2D as well as 3D.  `width` and `height` are the dimensions of the `Scene` in pixels, but this is not strictly neccessary for the developer, since Lumen handles resizing events as well.  Binding the `Scene` tells the `Renderer` that all of the following render functions should be applied to that specific scene.  (**NOTE**:  Lumen's event system binds the scenes automatically based on certain events, so the developer actually never has to bind the `Scene`.  It is only mentioned here to give a proper description of the `Renderer` works.)

Now we want to start rendering to the `Scene`.  To see a list of the available functions, see [RendererGL.h](https://github.com/AlexEnerdyne/Lumen/blob/Main/ElecDev_Graphics_Application/Source/Graphics/OpenGL/RendererGL.h).  To render a simple circle to the `Scene`, we do this:

```C++
// Setup.
Scene scene(CameraType::Standard2D, 900, 900);
Renderer::bindScene(&scene);

// Rendering.
Renderer::addCircle2D(glm::vec3(0.f, 0.f, 0.f), 0.5f, glm::vec4(0.f, 0.f, 0.f, 1.f));

// Optional cleanup.
Renderer::unbindScene();
```

Now our `Scene` has a black circle, with radius 0.5, around the center.  However, static circles are of little use to us, so let us start manipulating it.

```C++
// Setup.
Scene scene(CameraType::Standard2D, 900, 900);
Renderer::bindScene(&scene);

// Rendering.
Circle* myCircle = Renderer::addCircle2D(glm::vec3(0.f, 0.f, 0.f), 0.5f, glm::vec4(0.f, 0.f, 0.f, 1.f));
// Manipulation.
myCircle->translate(glm::vec2(1.f, 1.f));
myCircle->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
myCircle->scale(glm::vec3(2.f, 2.f, 1.f));

// Optional cleanup.
Renderer::unbindScene();
```

Now we have a white circle, with radius 1, that is centered around (1,1).  To see a list of functions that can be used, take a look at [Primitive.h](https://github.com/AlexEnerdyne/Lumen/blob/Main/ElecDev_Graphics_Application/Source/Graphics/OpenGL/Entities/Primitive.h).  `Primitives` are described as a basic rendering shape, for example circles, polygons, text etc.  `Entities` are seen as a collection of primitives, and these have to be created by the developer.  For example, in the current `Design2DEngine` we have a `Component2D` class that stores pointers to all of the `Primitives` that make up the component.  The `Primitives` are added to the `Scene` in the constructor.  This allows you to only have to interact with a `Component2D` and not a bunch of `Primitives`.
