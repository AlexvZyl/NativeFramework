//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Application.h"

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

void Application::pushNotification(NotificationType type, int msTime, const std::string& content, const std::string& title)
{
	// Get the type.
	ImGuiToastType notificationType = 0;
	switch (type) 
	{
	case NotificationType::Success:
		notificationType = ImGuiToastType_Success;
		break;
	case NotificationType::Warning:
		notificationType = ImGuiToastType_Warning;
		break;
	case NotificationType::Info:
		notificationType = ImGuiToastType_Info;
		break;
	case NotificationType::Error:
		notificationType = ImGuiToastType_Error;
		break;
	}

	ImGuiToast notification(notificationType, msTime);
	notification.set_content(content.c_str());
	if (title.size())
		notification.set_title(title.c_str());

	ImGui::InsertNotification(notification);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//