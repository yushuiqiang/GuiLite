#pragma once

#include "../core_include/api.h"
#include "../core_include/resource.h"
#include "../core_include/wnd.h"
#include "../core_include/display.h"
#include "../core_include/bitmap.h"
#include "../core_include/word.h"
#include "../core_include/theme.h"
#include "../widgets_include/button.h"
#include <string.h>

#define MAX_ITEM_NUM			4
#define ITEM_HEIGHT				45

class c_list_box : public c_wnd
{
public:
    void set_on_change(WND_CALLBACK on_change);

    short get_item_count();

    int add_item(char* str);

    void clear_item();

    void  select_item(short index);

	
protected:
    virtual void pre_create_wnd();


    virtual void on_paint();

    virtual void on_focus();

    virtual void on_kill_focus();

    virtual void on_navigate(NAVIGATION_KEY key);

    virtual void on_touch(int x, int y, TOUCH_ACTION action);

	
private:
    void update_list_size();

    void show_list();

    void on_touch_down(int x, int y);

    void on_touch_up(int x, int y);


	short			m_selected_item;
	short			m_item_total;
	char*			m_item_array[MAX_ITEM_NUM];
	c_rect			m_list_wnd_rect;	//rect relative to parent wnd.
	c_rect			m_list_screen_rect;	//rect relative to physical screen(frame buffer)
	WND_CALLBACK 	on_change;
};
