#pragma once

#include "../core_include/api.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/wnd.h"
#include "../core_include/display.h"
#include "../core_include/theme.h"
#include "../widgets_include/button.h"
#include <string.h>

//Changing key width/height will change the width/height of keyboard
#define KEY_WIDTH          65
#define KEY_HEIGHT         38

#define KEYBOARD_WIDTH		((KEY_WIDTH + 2) * 10)
#define KEYBOARD_HEIGHT		((KEY_HEIGHT + 2) * 4)
#define NUM_BOARD_WIDTH		((KEY_WIDTH + 2) * 4)
#define NUM_BOARD_HEIGHT	((KEY_HEIGHT + 2) * 4)

#define CAPS_WIDTH			(KEY_WIDTH * 3 / 2)
#define DEL_WIDTH			(KEY_WIDTH * 3 / 2 + 1)
#define ESC_WIDTH			(KEY_WIDTH * 2 + 2)
#define SWITCH_WIDTH		(KEY_WIDTH * 3 / 2 )
#define SPACE_WIDTH			(KEY_WIDTH * 3 + 2 * 2)
#define DOT_WIDTH			(KEY_WIDTH * 3 / 2 + 3)
#define ENTER_WIDTH			(KEY_WIDTH * 2 + 2)

#define POS_X(c)			((KEY_WIDTH * c) + (c + 1) * 2)
#define POS_Y(r)			((KEY_HEIGHT * r) + (r + 1) * 2)

#define KEYBORAD_CLICK			0x5014
#define ON_KEYBORAD_UPDATE(func)  \
{MSG_TYPE_WND, KEYBORAD_CLICK, 0,  msgCallback(&func)},

typedef enum
{
	STATUS_UPPERCASE,
	STATUS_LOWERCASE
}KEYBOARD_STATUS;

typedef enum
{
	STYLE_ALL_BOARD,
	STYLE_NUM_BOARD
}KEYBOARD_STYLE;

typedef enum
{
	CLICK_CHAR,
	CLICK_ENTER,
	CLICK_ESC
}CLICK_STATUS;

extern WND_TREE g_key_board_children[];
extern WND_TREE g_number_board_children[];
class c_keyboard: public c_wnd
{
public:
    virtual int connect(c_wnd *user, unsigned short resource_id, KEYBOARD_STYLE style);
    virtual void on_init_children();

    KEYBOARD_STATUS get_cap_status();
    char* get_str();
    void set_on_click(WND_CALLBACK on_click);
protected:
    virtual void pre_create_wnd();
    virtual void on_paint();

    void on_key_clicked(int id, int param);
    void on_char_clicked(int id, int param);
    void on_del_clicked(int id, int param);
    void on_caps_clicked(int id, int param);
    void on_enter_clicked(int id, int param);
    void on_esc_clicked(int id, int param);

private:
	char m_str[32];
	int	 m_str_len;
	KEYBOARD_STATUS m_cap_status;
	WND_CALLBACK on_click;
};

class c_keyboard_button : public c_button
{
protected:
    virtual void on_paint();
};
