#pragma once

#include "../core_include/api.h"
#include "../core_include/wnd.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/display.h"
#include "../core_include/theme.h"
#include "../widgets_include/button.h"

#define ID_BT_ARROW_UP      	0x1111
#define ID_BT_ARROW_DOWN    	0x2222

class c_spin_box;
class c_spin_button : public c_button
{
	friend class c_spin_box;
	inline virtual void on_touch(int x, int y, TOUCH_ACTION action);
	c_spin_box* m_spin_box;
};

class c_spin_box : public c_wnd
{
	friend class c_spin_button;
public:
    short get_value();

    void set_value(unsigned short value);

    void set_max_min(short max, short min);

    void set_step(short step);

    short get_min();

    short get_max();

    short get_step();

    void set_value_digit(short digit);

    short get_value_digit();

    void set_on_change(WND_CALLBACK on_change);

protected:
    virtual void on_paint();

    virtual void pre_create_wnd();

    void on_arrow_up_bt_click();

    void on_arrow_down_bt_click();


	short			m_cur_value;
	short			m_value;
	short			m_step;
	short			m_max;
	short			m_min;
	short			m_digit;
	c_spin_button  	m_bt_up;
	c_spin_button  	m_bt_down;
	WND_CALLBACK 	on_change;
};

inline void c_spin_button::on_touch(int x, int y, TOUCH_ACTION action)
{
	if (action == TOUCH_UP)
	{
		(m_id == ID_BT_ARROW_UP) ? m_spin_box->on_arrow_up_bt_click() : m_spin_box->on_arrow_down_bt_click();
	}
	c_button::on_touch(x, y, action);
}
