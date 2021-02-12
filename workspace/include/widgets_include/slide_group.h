#pragma once

#include "../core_include/api.h"
#include "../core_include/display.h"
#include "../core_include/wnd.h"
#include "../widgets_include/dialog.h"
#include <stdlib.h>

#define MAX_PAGES	5
class c_gesture;
class c_slide_group : public c_wnd
{
public:
	inline c_slide_group();

    int set_active_slide(int index, bool is_redraw = true);

    c_wnd* get_slide(int index);

    c_wnd* get_active_slide();

    int get_active_slide_index();

    int add_slide(c_wnd* slide, unsigned short resource_id, short x, short y, short width, short height, WND_TREE* p_child_tree = 0, Z_ORDER_LEVEL max_zorder =  Z_ORDER_LEVEL_0);

    void disabel_all_slide();

	inline virtual void on_touch(int x, int y, TOUCH_ACTION action);

    virtual void on_navigate(NAVIGATION_KEY key);

protected:
	c_wnd* m_slides[MAX_PAGES];
	int m_active_slide_index;
	c_gesture* m_gesture;
};

//#define SWIPE_STEP			300//for arm
#define SWIPE_STEP			10//for PC & ANDROID
#define MOVE_THRESHOLD		10

typedef enum {
	TOUCH_MOVE,
	TOUCH_IDLE
}TOUCH_STATE;

class c_slide_group;
class c_gesture
{
public:
    c_gesture(c_slide_group* group);

    bool handle_swipe(int x, int y, TOUCH_ACTION action);


private:
    bool on_move(int x);

    bool on_swipe(int x);

    int swipe_left();

    int swipe_right();

    void move_left();

    void move_right();

	int m_down_x;
	int m_down_y;
	int m_move_x;
	int m_move_y;
	TOUCH_STATE m_state;
	c_slide_group* m_slide_group;
};

inline c_slide_group::c_slide_group()
{
	m_gesture = new c_gesture(this);
	for (int i = 0; i < MAX_PAGES; i++)
	{
		m_slides[i] = 0;
	}
	m_active_slide_index = 0;
}

inline void c_slide_group::on_touch(int x, int y, TOUCH_ACTION action)
{
	x -= m_wnd_rect.m_left;
	y -= m_wnd_rect.m_top;

	if (m_gesture->handle_swipe(x, y, action))
	{
		if (m_slides[m_active_slide_index])
		{
			m_slides[m_active_slide_index]->on_touch(x, y, action);
		}
	}
}
