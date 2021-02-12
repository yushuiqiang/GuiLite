#pragma once

#include "../core_include/api.h"
#include "../core_include/wnd.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/display.h"
#include "../core_include/theme.h"
#include "../widgets_include/button.h"
#include "../widgets_include/label.h"
#include "../widgets_include/keyboard.h"
#include <string.h>

#define MAX_EDIT_STRLEN		32
#define IDD_KEY_BOARD		0x1
class c_edit : public c_wnd
{
	friend class c_keyboard;
public:
    const char* get_text();
    void set_text(const char* str);

    void set_keyboard_style(KEYBOARD_STYLE kb_sytle);
	
protected:
    virtual void pre_create_wnd();

    virtual void on_paint();

    virtual void on_focus();

    virtual void on_kill_focus();

    virtual void on_navigate(NAVIGATION_KEY key);

    virtual void on_touch(int x, int y, TOUCH_ACTION action);

    void on_key_board_click(int id, int param);

private:
    void show_keyboard();

    void on_touch_down(int x, int y);

    void on_touch_up(int x, int y);

	static c_keyboard  s_keyboard;
	KEYBOARD_STYLE m_kb_style;
	char m_str_input[MAX_EDIT_STRLEN];
	char m_str[MAX_EDIT_STRLEN];
};
