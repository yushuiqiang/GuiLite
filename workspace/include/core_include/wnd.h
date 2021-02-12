#pragma once

#include "../core_include/api.h"
#include "../core_include/resource.h"
#include "../core_include/bitmap.h"
#include "../core_include/display.h"

typedef struct struct_font_info		FONT_INFO;
typedef struct struct_color_rect	COLOR_RECT;

class c_wnd;
class c_surface;

typedef enum
{
	ATTR_VISIBLE	= 0x40000000L,
	ATTR_FOCUS		= 0x20000000L,
	ATTR_PRIORITY	= 0x10000000L// Handle touch action at high priority
}WND_ATTRIBUTION;

typedef enum
{
	STATUS_NORMAL,
	STATUS_PUSHED,
	STATUS_FOCUSED,
	STATUS_DISABLED
}WND_STATUS;

typedef enum
{
	NAV_FORWARD,
	NAV_BACKWARD,
	NAV_ENTER
}NAVIGATION_KEY;

typedef enum
{
	TOUCH_DOWN,
	TOUCH_UP
}TOUCH_ACTION;

typedef struct struct_wnd_tree
{
	c_wnd*					p_wnd;//window instance
	unsigned int			resource_id;//ID
	const char*				str;//caption
	short   				x;//position x
	short   				y;//position y
	short   				width;
	short        			height;
	struct struct_wnd_tree*	p_child_tree;//sub tree
}WND_TREE;

typedef void (c_wnd::*WND_CALLBACK)(int, int);

class c_wnd
{
public:
    c_wnd();
    virtual ~c_wnd();
	virtual int connect(c_wnd *parent, unsigned short resource_id, const char* str,
        short x, short y, short width, short height, WND_TREE* p_child_tree = 0);

    void disconnect();

    virtual void on_init_children();
    virtual void on_paint();
    virtual void show_window();

    unsigned short get_id() const;
    int get_z_order();
    c_wnd* get_wnd_ptr(unsigned short id) const;

    unsigned int get_attr() const;

    void set_str(const char* str);
    void set_attr(WND_ATTRIBUTION attr);
    bool is_focus_wnd() const;

    void set_font_color(unsigned int color);
    unsigned int get_font_color();
    void set_bg_color(unsigned int color);
    unsigned int get_bg_color();
    void set_font_type(const FONT_INFO *font_type);
    const FONT_INFO* get_font_type();

    void set_wnd_pos(short x, short y, short width, short height);

    void get_wnd_rect(c_rect &rect) const;
    void get_screen_rect(c_rect &rect) const;

    c_wnd* set_child_focus(c_wnd *focus_child);

    c_wnd* get_parent() const;
    c_wnd* get_last_child() const;

    int	unlink_child(c_wnd *child);

    c_wnd* get_prev_sibling() const;
    c_wnd* get_next_sibling() const;

    virtual void on_touch(int x, int y, TOUCH_ACTION action);

    virtual void on_navigate(NAVIGATION_KEY key);

    c_surface* get_surface();
    void set_surface(c_surface* surface);
protected:
    virtual void pre_create_wnd();

    void add_child_2_tail(c_wnd *child);

    void wnd2screen(int &x, int &y) const;

    int load_child_wnd(WND_TREE *p_child_tree);

    void set_active_child(c_wnd* child);

    virtual void on_focus();
    virtual void on_kill_focus();
protected:
	unsigned short	m_id;
	WND_STATUS		m_status;
	WND_ATTRIBUTION	m_attr;
	c_rect			m_wnd_rect;		//position relative to parent window.
	c_wnd*			m_parent;		//parent window
	c_wnd*			m_top_child;	//the first sub window would be navigated
	c_wnd*			m_prev_sibling;	//previous brother
	c_wnd*			m_next_sibling;	//next brother
	c_wnd*			m_focus_child;	//current focused window
	const char*		m_str;			//caption

	const FONT_INFO*	m_font_type;
	unsigned int		m_font_color;
	unsigned int		m_bg_color;

	int					m_z_order;		//the graphic level for rendering
	c_surface*			m_surface;
};
