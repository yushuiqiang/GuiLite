#include "../../include/widgets_include/label.h"

void c_label::on_paint()
{
    c_rect rect;
    unsigned int bg_color = m_bg_color ? m_bg_color : m_parent->get_bg_color();
    get_screen_rect(rect);
    if (m_str)
    {
        m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, bg_color, m_z_order);
        c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, bg_color, ALIGN_LEFT | ALIGN_VCENTER);
    }
}

void c_label::pre_create_wnd()
{
    m_attr = ATTR_VISIBLE;
    m_font_color = c_theme::get_color(COLOR_WND_FONT);
    m_font_type = c_theme::get_font(FONT_DEFAULT);
}
