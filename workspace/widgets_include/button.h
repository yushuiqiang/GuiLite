#pragma once

#include "../core_include/api.h"
#include "../core_include/wnd.h"
#include "../core_include/resource.h"
#include "../core_include/bitmap.h"
#include "../core_include/word.h"
#include "../core_include/display.h"
#include "../core_include/theme.h"

class c_button : public c_wnd
{
public:

    void set_on_click(WND_CALLBACK on_click);
protected:

    virtual void on_paint();

    virtual void on_focus();

    virtual void on_kill_focus();

    virtual void pre_create_wnd();

    virtual void on_touch(int x, int y, TOUCH_ACTION action);

    virtual void on_navigate(NAVIGATION_KEY key);

	WND_CALLBACK on_click;
};
