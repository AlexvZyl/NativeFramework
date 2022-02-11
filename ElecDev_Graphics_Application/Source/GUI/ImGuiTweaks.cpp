//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/ImGuiTweaks.h"
#include "Lumen.h"

//==============================================================================================================================================//
//  ImGUI tweaks.																																//
//==============================================================================================================================================//

namespace ImGuiTweaks 
{
    // ----------------- //
    //  V I E W P O R T  //
    // ----------------- //

    // This allows us to seperate the toolbar from the main viewport.
    ImGuiID DockSpaceOverViewport(const ImGuiViewport* viewport, ImGuiDockNodeFlags dockspace_flags, const ImGuiWindowClass* window_class)
    {
        if (viewport == NULL)
            viewport = ImGui::GetMainViewport();

        ImVec2 size = { viewport->WorkSize.x, viewport->WorkSize.y - RIBBON_HEIGHT };
        ImVec2 pos = { viewport->WorkPos.x, viewport->WorkPos.y + RIBBON_HEIGHT };
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags host_window_flags = 0;
        host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
        host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            host_window_flags |= ImGuiWindowFlags_NoBackground;

        char label[32];
        ImFormatString(label, IM_ARRAYSIZE(label), "DockSpaceViewport_%08X", viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin(label, NULL, host_window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, window_class);
        ImGui::End();

        return dockspace_id;
    }

    // ------------- //
    //  W I N D O W  //
    // ------------- //

    // This allows us to check if a window is hovered by passing a window pointer.
    bool IsWindowHovered(ImGuiHoveredFlags flags, ImGuiWindow* window)
    {
        IM_ASSERT((flags & ImGuiHoveredFlags_AllowWhenOverlapped) == 0);   // Flags not supported by this function
        ImGuiContext& g = *GImGui;
        if (g.HoveredWindow == NULL)
            return false;

        if (window == NULL) window = g.CurrentWindow; //  Added this line so that we can pass a window to check.

        if ((flags & ImGuiHoveredFlags_AnyWindow) == 0)
        { 
            switch (flags & (ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows))
            {
            case ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows:
                if (g.HoveredWindow->RootWindow != window->RootWindow)
                    return false;
                break;
            case ImGuiHoveredFlags_RootWindow:
                if (g.HoveredWindow != window->RootWindow)
                    return false;
                break;
            case ImGuiHoveredFlags_ChildWindows:
                if (!ImGui::IsWindowChildOf(g.HoveredWindow, window))
                    return false;
                break;
            default:
                if (g.HoveredWindow != window)
                    return false;
                break;
            }
        }

        if (!IsWindowContentHoverable(g.HoveredWindow, flags))
            return false;
        if (!(flags & ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
            if (g.ActiveId != 0 && !g.ActiveIdAllowOverlap && g.ActiveId != g.HoveredWindow->MoveId)
                return false;
        return true;
    }

    // This function has to be here since it is only declared in imgui.cpp and we can't see it from here.
    static bool IsWindowContentHoverable(ImGuiWindow* window, ImGuiHoveredFlags flags)
    {
        // An active popup disable hovering on other windows (apart from its own children)
        // FIXME-OPT: This could be cached/stored within the window.
        ImGuiContext& g = *GImGui;
        if (g.NavWindow)
            if (ImGuiWindow* focused_root_window = g.NavWindow->RootWindowDockTree)
                if (focused_root_window->WasActive && focused_root_window != window->RootWindowDockTree)
                {
                    // For the purpose of those flags we differentiate "standard popup" from "modal popup"
                    // NB: The order of those two tests is important because Modal windows are also Popups.
                    if (focused_root_window->Flags & ImGuiWindowFlags_Modal)
                        return false;
                    if ((focused_root_window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiHoveredFlags_AllowWhenBlockedByPopup))
                        return false;
                }

        // Filter by viewport
        if (window->Viewport != g.MouseViewport)
            if (g.MovingWindow == NULL || window->RootWindowDockTree != g.MovingWindow->RootWindowDockTree)
                return false;

        return true;
    }
}

//==============================================================================================================================================//
//   EOF.																																	    //
//==============================================================================================================================================//