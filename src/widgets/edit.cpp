﻿#include "../../include/widgets_include/edit.h"

#ifdef GUILITE_ON

c_keyboard  c_edit::s_keyboard;

const char* c_edit::get_text()
{
    return m_str;
}
void c_edit::set_text(const char* str)
{
    if (str != 0 && strlen(str) < sizeof(m_str))
    {
        strcpy(m_str, str);
    }
}
void c_edit::set_keyboard_style(KEYBOARD_STYLE kb_sytle)
{
    m_kb_style = kb_sytle;
}

void c_edit::pre_create_wnd()
{
    m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
    m_kb_style = STYLE_ALL_BOARD;
    m_font_type = c_theme::get_font(FONT_DEFAULT);
    m_font_color = c_theme::get_color(COLOR_WND_FONT);

    memset(m_str_input, 0, sizeof(m_str_input));
    memset(m_str, 0, sizeof(m_str));
    set_text(c_wnd::m_str);
}
void c_edit::on_paint()
{
    c_rect rect, kb_rect;
    get_screen_rect(rect);
    s_keyboard.get_screen_rect(kb_rect);
    switch (m_status)
    {
    case STATUS_NORMAL:
        if (m_z_order > m_parent->get_z_order())
        {
            s_keyboard.disconnect();
            m_z_order = m_parent->get_z_order();
            m_surface->show_layer(kb_rect, m_z_order);
            m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
        }
        m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
        c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_NORMAL), ALIGN_HCENTER | ALIGN_VCENTER);
        break;
    case STATUS_FOCUSED:
        if (m_z_order > m_parent->get_z_order())
        {
            s_keyboard.disconnect();
            m_z_order = m_parent->get_z_order();
            m_surface->show_layer(kb_rect, m_z_order);
            m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
        }
        m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
        c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_FOCUS), ALIGN_HCENTER | ALIGN_VCENTER);
        break;
    case STATUS_PUSHED:
        if (m_z_order == m_parent->get_z_order())
        {
            m_z_order++;
            m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS | ATTR_PRIORITY);
            show_keyboard();
        }
        m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, c_theme::get_color(COLOR_WND_PUSHED), m_parent->get_z_order());
        m_surface->draw_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, c_theme::get_color(COLOR_WND_BORDER), m_parent->get_z_order(), 2);
        strlen(m_str_input) ? c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str_input, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_PUSHED), ALIGN_HCENTER | ALIGN_VCENTER) :
            c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_PUSHED), ALIGN_HCENTER | ALIGN_VCENTER);
        break;
    default:
        ASSERT(false);
    }
}
void c_edit::on_focus()
{
    m_status = STATUS_FOCUSED;
    on_paint();
}
void c_edit::on_kill_focus()
{
    m_status = STATUS_NORMAL;
    on_paint();
}
void c_edit::on_navigate(NAVIGATION_KEY key)
{
    switch (key)
    {
    case NAV_ENTER:
        (m_status == STATUS_PUSHED) ? s_keyboard.on_navigate(key) : (on_touch(m_wnd_rect.m_left, m_wnd_rect.m_top, TOUCH_DOWN), on_touch(m_wnd_rect.m_left, m_wnd_rect.m_top, TOUCH_UP));
        return;
    case NAV_BACKWARD:
    case NAV_FORWARD:
        return (m_status == STATUS_PUSHED) ? s_keyboard.on_navigate(key) : c_wnd::on_navigate(key);
    }
}
void c_edit::on_touch(int x, int y, TOUCH_ACTION action)
{
    (action == TOUCH_DOWN) ? on_touch_down(x, y) : on_touch_up(x, y);
}
void c_edit::on_key_board_click(int id, int param)
{
    switch (param)
    {
    case CLICK_CHAR:
        strcpy(m_str_input, s_keyboard.get_str());
        on_paint();
        break;
    case CLICK_ENTER:
        if (strlen(m_str_input))
        {
            memcpy(m_str, m_str_input, sizeof(m_str_input));
        }
        m_status = STATUS_FOCUSED;
        on_paint();
        break;
    case CLICK_ESC:
        memset(m_str_input, 0, sizeof(m_str_input));
        m_status = STATUS_FOCUSED;
        on_paint();
        break;
    default:
        ASSERT(false);
        break;
    }
}

void c_edit::show_keyboard()
{
    s_keyboard.connect(this, IDD_KEY_BOARD, m_kb_style);
    s_keyboard.set_on_click(WND_CALLBACK(&c_edit::on_key_board_click));
    s_keyboard.show_window();
}
void c_edit::on_touch_down(int x, int y)
{
    c_rect kb_rect_relate_2_edit_parent;
    s_keyboard.get_wnd_rect(kb_rect_relate_2_edit_parent);
    kb_rect_relate_2_edit_parent.m_left += m_wnd_rect.m_left;
    kb_rect_relate_2_edit_parent.m_right += m_wnd_rect.m_left;
    kb_rect_relate_2_edit_parent.m_top += m_wnd_rect.m_top;
    kb_rect_relate_2_edit_parent.m_bottom += m_wnd_rect.m_top;

    if (m_wnd_rect.pt_in_rect(x, y))
    {//click edit box
        if (STATUS_NORMAL == m_status)
        {
            m_parent->set_child_focus(this);
        }
    }
    else if (kb_rect_relate_2_edit_parent.pt_in_rect(x, y))
    {//click key board
        c_wnd::on_touch(x, y, TOUCH_DOWN);
    }
    else
    {
        if (STATUS_PUSHED == m_status)
        {
            m_status = STATUS_FOCUSED;
            on_paint();
        }
    }
}
void c_edit::on_touch_up(int x, int y)
{
    if (STATUS_FOCUSED == m_status)
    {
        m_status = STATUS_PUSHED;
        on_paint();
    }
    else if (STATUS_PUSHED == m_status)
    {
        if (m_wnd_rect.pt_in_rect(x, y))
        {//click edit box
            m_status = STATUS_FOCUSED;
            on_paint();
        }
        else
        {
            c_wnd::on_touch(x, y, TOUCH_UP);
        }
    }
}

#endif
