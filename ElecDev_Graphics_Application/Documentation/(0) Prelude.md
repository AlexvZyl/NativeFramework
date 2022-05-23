## Prelude

We use [GLM](https://github.com/g-truc/glm) for matrices, vectors, linear algebra etc.  Some example usage:

```C++
// Vector examples.
glm::vec2 myVec2(1.f, 1.f);
glm::vec3 myVec3(1.f, 1.f, 1.f);
glm::vec4 myVec4(1.f, 1.f, 1.f, 1.f);

// Matrix examples.
glm::mat4 myMat4(1.f);  // Identity matrix.
glm::mat4 myProjectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);

// Manipulating matrices.
glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
View = glm::rotate(View, 3.f, glm::vec3(-1.0f, 0.0f, 0.0f));
View = glm::rotate(View, 4.f, glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
```

A very handy feature is the fact that glm vectors can be interpreted as position or color data:

```C++
glm::vec4 myVec4(0.1f, 0.2f, 0.3f, 0.4f);

// Position data.
float x = myVec4.x;  // 0.1f
float y = myVec4.y;  // 0.2f
float z = myVec4.z;  // 0.3f
float w = myVec4.w;  // 0.4f

// Color data.
float r = myVec4.r;  // 0.1f
float g = myVec4.g;  // 0.2f
float b = myVec4.b;  // 0.3f
float a = myVec4.a;  // 0.4f
```

[Dear ImGui](https://github.com/ocornut/imgui) is used as the GUI and they use their own vectors, namely `ImVec2` and `ImVec4`.  Lumen overloads these vectors so that they are interchangeable with `glm` vectors.  These examples are all valid:

```C++
ImVec2 vector1(0,0);
glm::vec2 vector2 = vector1;
glm::vec2 cursosPos = ImGui::GetCursosPos();  // Returns an ImVec2.
```

When creating vectors and matrices, stick to `glm`.
