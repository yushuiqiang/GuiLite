﻿#include "../../include/core_include/wnd.h"

c_wnd::c_wnd() : m_status(STATUS_NORMAL),
    m_attr((WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS)),
    m_parent(0),
    m_top_child(0), m_prev_sibling(0), m_next_sibling(0),
    m_str(0),
    m_font_color(0), m_bg_color(0),
    m_id(0),
    m_z_order(Z_ORDER_LEVEL_0),
    m_focus_child(0),
    m_surface(0)
{

};
c_wnd::~c_wnd()
{

};
int c_wnd::connect(c_wnd *parent, unsigned short resource_id, const char* str,
    short x, short y, short width, short height, WND_TREE* p_child_tree)
{
    if (0 == resource_id)
    {
        ASSERT(false);
        return -1;
    }

    m_id = resource_id;
    set_str(str);
    m_parent = parent;
    m_status = STATUS_NORMAL;

    if (parent)
    {
        m_z_order = parent->m_z_order;
        m_surface = parent->m_surface;
    }
    if (0 == m_surface)
    {
        ASSERT(false);
        return -2;
    }

    /* (cs.x = x * 1024 / 768) for 1027*768=>800*600 quickly*/
    m_wnd_rect.m_left = x;
    m_wnd_rect.m_top = y;
    m_wnd_rect.m_right = (x + width - 1);
    m_wnd_rect.m_bottom = (y + height - 1);

    pre_create_wnd();

    if (0 != parent)
    {
        parent->add_child_2_tail(this);
    }

    if (load_child_wnd(p_child_tree) >= 0)
    {
        on_init_children();
    }
    return 0;
}
void c_wnd::disconnect()
{
    if (0 == m_id)
    {
        return;
    }

    if (0 != m_top_child)
    {
        c_wnd* child = m_top_child;
        c_wnd* next_child = 0;

        while (child)
        {
            next_child = child->m_next_sibling;
            child->disconnect();
            child = next_child;
        }
    }

    if (0 != m_parent)
    {
        m_parent->unlink_child(this);
    }
    m_focus_child = 0;
    m_id = 0;
}
void c_wnd::on_init_children() {}
void c_wnd::on_paint() {}
void c_wnd::show_window()
{
    if (ATTR_VISIBLE == (m_attr & ATTR_VISIBLE))
    {
        on_paint();
        c_wnd* child = m_top_child;
        if (0 != child)
        {
            while (child)
            {
                child->show_window();
                child = child->m_next_sibling;
            }
        }
    }
}

unsigned short c_wnd::get_id() const
{
    return m_id;
}
int c_wnd::get_z_order()
{
    return m_z_order;
}
c_wnd* c_wnd::get_wnd_ptr(unsigned short id) const
{
    c_wnd* child = m_top_child;

    while (child)
    {
        if (child->get_id() == id)
        {
            break;
        }

        child = child->m_next_sibling;
    }

    return child;
}
unsigned int c_wnd::get_attr() const
{
    return m_attr;
}

void c_wnd::set_str(const char* str)
{
    m_str = str;
}
void c_wnd::set_attr(WND_ATTRIBUTION attr)
{
    m_attr = attr;
}
bool c_wnd::is_focus_wnd() const
{
    return ((m_attr & ATTR_VISIBLE) && (m_attr & ATTR_FOCUS)) ? true : false;
}

void c_wnd::set_font_color(unsigned int color)
{
    m_font_color = color;
}
unsigned int c_wnd::get_font_color()
{
    return m_font_color;
}
void c_wnd::set_bg_color(unsigned int color)
{
    m_bg_color = color;
}
unsigned int c_wnd::get_bg_color()
{
    return m_bg_color;
}
void c_wnd::set_font_type(const FONT_INFO *font_type)
{
    m_font_type = font_type;
}
const FONT_INFO* c_wnd::get_font_type()
{
    return m_font_type;
}

void c_wnd::set_wnd_pos(short x, short y, short width, short height)
{
    m_wnd_rect.m_left = x;
    m_wnd_rect.m_top = y;
    m_wnd_rect.m_right = x + width - 1;
    m_wnd_rect.m_bottom = y + height - 1;
}
void c_wnd::get_wnd_rect(c_rect &rect) const
{
    rect = m_wnd_rect;
}
void c_wnd::get_screen_rect(c_rect &rect) const
{
    int l = 0;
    int t = 0;
    wnd2screen(l, t);
    rect.set_rect(l, t, m_wnd_rect.width(), m_wnd_rect.height());
}

c_wnd* c_wnd::set_child_focus(c_wnd *focus_child)
{
    ASSERT(0 != focus_child);
    ASSERT(focus_child->m_parent == this);

    c_wnd* old_focus_child = m_focus_child;
    if (focus_child->is_focus_wnd())
    {
        if (focus_child != old_focus_child)
        {
            if (old_focus_child)
            {
                old_focus_child->on_kill_focus();
            }
            m_focus_child = focus_child;
            m_focus_child->on_focus();
        }
    }
    return m_focus_child;
}

c_wnd* c_wnd::get_parent() const
{
    return m_parent;
}
c_wnd* c_wnd::get_last_child() const
{
    if (0 == m_top_child)
    {
        return 0;
    }

    c_wnd* child = m_top_child;

    while (child->m_next_sibling)
    {
        child = child->m_next_sibling;
    }

    return child;
}
int	c_wnd::unlink_child(c_wnd *child)
{
    if ((0 == child)
        || (this != child->m_parent))
    {
        return -1;
    }

    if (0 == m_top_child)
    {
        return -2;
    }

    bool find = false;

    c_wnd* tmp_child = m_top_child;
    if (tmp_child == child)
    {
        m_top_child = child->m_next_sibling;
        if (0 != child->m_next_sibling)
        {
            child->m_next_sibling->m_prev_sibling = 0;
        }

        find = true;
    }
    else
    {
        while (tmp_child->m_next_sibling)
        {
            if (child == tmp_child->m_next_sibling)
            {
                tmp_child->m_next_sibling = child->m_next_sibling;
                if (0 != child->m_next_sibling)
                {
                    child->m_next_sibling->m_prev_sibling = tmp_child;
                }

                find = true;
                break;
            }

            tmp_child = tmp_child->m_next_sibling;
        }
    }

    if (true == find)
    {
        if (m_focus_child == child)
        {
            m_focus_child = 0;
        }

        child->m_next_sibling = 0;
        child->m_prev_sibling = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}
c_wnd* c_wnd::get_prev_sibling() const
{
    return m_prev_sibling;
}
c_wnd* c_wnd::get_next_sibling() const
{
    return m_next_sibling;
}

void c_wnd::on_touch(int x, int y, TOUCH_ACTION action)
{
    x -= m_wnd_rect.m_left;
    y -= m_wnd_rect.m_top;

    c_wnd* priority_wnd = 0;
    c_wnd* tmp_child = m_top_child;
    while (tmp_child)
    {
        if ((tmp_child->m_attr & ATTR_PRIORITY) && (tmp_child->m_attr & ATTR_VISIBLE))
        {
            priority_wnd = tmp_child;
            break;
        }
        tmp_child = tmp_child->m_next_sibling;
    }
    if (priority_wnd)
    {
        return priority_wnd->on_touch(x, y, action);
    }

    c_wnd* child = m_top_child;
    while (child)
    {
        if (child->is_focus_wnd())
        {
            c_rect rect;
            child->get_wnd_rect(rect);
            if (true == rect.pt_in_rect(x, y))
            {
                return child->on_touch(x, y, action);
            }
        }
        child = child->m_next_sibling;
    }
}
void c_wnd::on_navigate(NAVIGATION_KEY key)
{
    c_wnd* priority_wnd = 0;
    c_wnd* tmp_child = m_top_child;
    while (tmp_child)
    {
        if ((tmp_child->m_attr & ATTR_PRIORITY) && (tmp_child->m_attr & ATTR_VISIBLE))
        {
            priority_wnd = tmp_child;
            break;
        }
        tmp_child = tmp_child->m_next_sibling;
    }
    if (priority_wnd)
    {
        return priority_wnd->on_navigate(key);
    }

    if (!is_focus_wnd())
    {
        return;
    }
    if (key != NAV_BACKWARD && key != NAV_FORWARD)
    {
        if (m_focus_child)
        {
            m_focus_child->on_navigate(key);
        }
        return;
    }

    // Move focus
    c_wnd* old_focus_wnd = m_focus_child;
    // No current focus wnd, new one.
    if (!old_focus_wnd)
    {
        c_wnd* child = m_top_child;
        c_wnd* new_focus_wnd = 0;
        while (child)
        {
            if (child->is_focus_wnd())
            {
                new_focus_wnd = child;
                new_focus_wnd->m_parent->set_child_focus(new_focus_wnd);
                child = child->m_top_child;
                continue;
            }
            child = child->m_next_sibling;
        }
        return;
    }
    // Move focus from old wnd to next wnd
    c_wnd* next_focus_wnd = (key == NAV_FORWARD) ? old_focus_wnd->m_next_sibling : old_focus_wnd->m_prev_sibling;
    while (next_focus_wnd && (!next_focus_wnd->is_focus_wnd()))
    {// Search neighbor of old focus wnd
        next_focus_wnd = (key == NAV_FORWARD) ? next_focus_wnd->m_next_sibling : next_focus_wnd->m_prev_sibling;
    }
    if (!next_focus_wnd)
    {// Search whole brother wnd
        next_focus_wnd = (key == NAV_FORWARD) ? old_focus_wnd->m_parent->m_top_child : old_focus_wnd->m_parent->get_last_child();
        while (next_focus_wnd && (!next_focus_wnd->is_focus_wnd()))
        {
            next_focus_wnd = (key == NAV_FORWARD) ? next_focus_wnd->m_next_sibling : next_focus_wnd->m_prev_sibling;
        }
    }
    if (next_focus_wnd)
    {
        next_focus_wnd->m_parent->set_child_focus(next_focus_wnd);
    }
}

c_surface* c_wnd::get_surface()
{
    return m_surface;
}
void c_wnd::set_surface(c_surface* surface)
{
    m_surface = surface;
}
void c_wnd::pre_create_wnd()
{

}
void c_wnd::add_child_2_tail(c_wnd *child)
{
    if (0 == child)return;
    if (child == get_wnd_ptr(child->m_id))return;

    if (0 == m_top_child)
    {
        m_top_child = child;
        child->m_prev_sibling = 0;
        child->m_next_sibling = 0;
    }
    else
    {
        c_wnd* last_child = get_last_child();
        if (0 == last_child)
        {
            ASSERT(false);
        }
        last_child->m_next_sibling = child;
        child->m_prev_sibling = last_child;
        child->m_next_sibling = 0;
    }
}

void c_wnd::wnd2screen(int &x, int &y) const
{
    c_wnd* parent = m_parent;
    c_rect rect;

    x += m_wnd_rect.m_left;
    y += m_wnd_rect.m_top;

    while (0 != parent)
    {
        parent->get_wnd_rect(rect);
        x += rect.m_left;
        y += rect.m_top;

        parent = parent->m_parent;
    }
}

int c_wnd::load_child_wnd(WND_TREE *p_child_tree)
{
    if (0 == p_child_tree)
    {
        return 0;
    }
    int sum = 0;

    WND_TREE* p_cur = p_child_tree;
    while (p_cur->p_wnd)
    {
        if (0 != p_cur->p_wnd->m_id)
        {//This wnd has been used! Do not share!
            ASSERT(false);
            return -1;
        }
        else
        {
            p_cur->p_wnd->connect(this, p_cur->resource_id, p_cur->str,
                p_cur->x, p_cur->y, p_cur->width, p_cur->height, p_cur->p_child_tree);
        }
        p_cur++;
        sum++;
    }
    return sum;
}
void c_wnd::set_active_child(c_wnd* child) { m_focus_child = child; }
void c_wnd::on_focus(){};
void c_wnd::on_kill_focus(){};
