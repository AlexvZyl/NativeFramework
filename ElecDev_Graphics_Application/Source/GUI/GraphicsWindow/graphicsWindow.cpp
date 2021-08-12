#include "graphicsWindow.h"



GraphicsWindow::GraphicsWindow() {

}


void GraphicsWindow::renderGraphics(GLFWwindow* window) {


	ImGui::Begin("Scene Window");

	ImGui::GetWindowDrawList()->AddImage(
		(void*)window.getRenderTexture(), ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + window.getWidth() / 2, ImGui::GetCursorScreenPos().y + window.getHeight() / 2), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();

}
