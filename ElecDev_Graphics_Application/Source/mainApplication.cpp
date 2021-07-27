#include <GLFW/glfw3.h>

/* ImGUI Includes*/
#include <imconfig.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imstb_rectpack.h>
#include <imstb_textedit.h>
#include <imstb_truetype.h>

//int main(void)
//{
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    /* Create a windowed mode window and its OpenGL context */
//    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;
//    }
//
//
//    ImGui::CreateContext();
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//
//    /* Loop until the user closes the window */
//    while (!glfwWindowShouldClose(window))
//    {
//        /*================================================================================================*/
//        /*Render ImGUI here. */
//        /*================================================================================================*/
//
//        ImGui::NewFrame();
//        ImGui::Begin("ElecDev Graphics");
//
//        ImGui::ShowDemoWindow();
//
//        /*================================================================================================*/
//        /* Render OpenGL here. */
//        /*================================================================================================*/
//
//        //glClear(GL_COLOR_BUFFER_BIT);
//
//        //glBegin(GL_TRIANGLES);
//        //glVertex2f(-0.5f, -0.5f);
//        //glVertex2f(0.0f, 0.5f);
//        //glVertex2f(0.5f, -0.5f);
//        //glEnd();
//
//        //glBegin(GL_LINE_LOOP);
//        //glVertex2f(-0.99, 1.0f);
//        //glVertex2f(0.99f, 0.99f);
//        //glVertex2f(1.0f, -1.0f);
//        //glVertex2f(-1.0f, -1.0f);
//        //glEnd();
//
//        ///* Swap front and back buffers */
//        //glfwSwapBuffers(window);
//
//        ///* Poll for and process events */
//        //glfwPollEvents();
//
//        /*================================================================================================*/
//    }
//
//    glfwTerminate();
//    return 0;
//}