# Lumen Documentation

*The docs are currently outdated due to tremendous development and the occasional refactoring.*

This is the non-official documentation of the Lumen Graphics Engine.  This contains information for new team members to get up and running with rendering inside of Lumen.  No knowledge of the backend is required!

Lumen has its own implementation of a graphics renderer.  We did this so that we are not limited by the API of someone else's work and so that we can change and implement whatever we want.  This turned out to be very beneficial.  For example, we render text based on a new method of Signed Distance Fields that someone developed in his Master's degree in graphics.  We would have no control over this if we had to use someone else's (high level) library.  Currently our renderer only supports an [OpenGL](https://www.opengl.org/) backend, however we have plans to implement a [Vulkan](https://www.vulkan.org/) renderer in the future.

Each section of the Lumen Framework can be seen in this Documentation folder.  See the respective markdown file for each section.
