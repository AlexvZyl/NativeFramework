#pragma once

/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/


/*=======================================================================================================================================*/
/* Forward declarations                                                                                                                  */
/*=======================================================================================================================================*/

class GraphicsHandler;

/*=======================================================================================================================================*/
/* Class.                                                                                                                                */
/*=======================================================================================================================================*/

// This function initializes the toolbar.

class Toolbar
{
private:
    GraphicsHandler* m_graphicsHandler;

public:

    bool show_app_main_menu_bar;
    bool show_app_documents;

    bool show_app_console;
    bool show_app_log;
    bool show_app_layout;
    bool show_app_property_editor;
    bool show_app_long_text;
    bool show_app_auto_resize;
    bool show_app_constrained_resize;
    bool show_app_simple_overlay;
    bool show_app_fullscreen;
    bool show_app_window_titles;
    bool show_app_custom_rendering;

    bool show_app_metrics;
    bool show_app_style_editor;
    bool show_app_about;

    bool no_titlebar;
    bool no_scrollbar;
    bool no_menu;
    bool no_move;
    bool no_resize;
    bool no_collapse;
    bool no_close;
    bool no_nav;
    bool no_background;
    bool no_bring_to_front;
    bool unsaved_document;
    bool my_tool_active;


    Toolbar();

	// Function that handles which engine should be active.
	void renderToolbar();

};

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/