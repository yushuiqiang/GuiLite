#pragma once

#include "../core_include/api.h"
#include "../core_include/wnd.h"
#include "../core_include/display.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/theme.h"

class c_surface;
class c_dialog;

typedef struct
{
	c_dialog* 	dialog;
	c_surface*	surface;
} DIALOG_ARRAY;

class c_dialog : public c_wnd
{
public:
    static int open_dialog(c_dialog* p_dlg, bool modal_mode = true);


    static int close_dialog(c_surface* surface);


    static c_dialog* get_the_dialog(c_surface* surface);

protected:
    virtual void pre_create_wnd();

    virtual void on_paint();

private:
    int set_me_the_dialog();

	static DIALOG_ARRAY ms_the_dialogs[SURFACE_CNT_MAX];
};
