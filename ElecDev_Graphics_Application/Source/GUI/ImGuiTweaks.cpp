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
    bool IsWindowHovered(ImGuiWindow* window, ImGuiHoveredFlags flags)
    {
        IM_ASSERT((flags & ImGuiHoveredFlags_AllowWhenOverlapped) == 0);   // Flags not supported by this function
        ImGuiContext& g = *GImGui;
        if (g.HoveredWindow == NULL)
            return false;

        if ((flags & ImGuiHoveredFlags_AnyWindow) == 0)
        {
            // ImGuiWindow* window = g.CurrentWindow;  Remove this line so that we can pass our own window to check.
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