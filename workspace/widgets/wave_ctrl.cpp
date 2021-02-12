#include "../widgets_include/wave_ctrl.h"

c_wave_ctrl::c_wave_ctrl()
{
    m_wave = 0;
    m_bg_fb = 0;
    m_wave_name_font = m_wave_unit_font = 0;
    m_wave_name = m_wave_unit = 0;
    m_max_data = 500;
    m_min_data = 0;
    m_wave_speed = 1;
    m_wave_data_rate = 0;
    m_wave_refresh_rate = 1000;
    m_frame_len_map_index = 0;

    m_wave_name_color = m_wave_unit_color = m_wave_color = GL_RGB(255, 0, 0);
    m_back_color = GL_RGB(0, 0, 0);
}
void c_wave_ctrl::on_init_children()//should be pre_create
{
    c_rect rect;
    get_screen_rect(rect);

    m_wave_left = rect.m_left + WAVE_MARGIN;
    m_wave_right = rect.m_right - WAVE_MARGIN;
    m_wave_top = rect.m_top + WAVE_MARGIN;
    m_wave_bottom = rect.m_bottom - WAVE_MARGIN;
    m_wave_cursor = m_wave_left;

    m_bg_fb = (unsigned int*)calloc(rect.width() * rect.height(), 4);
}
void c_wave_ctrl::on_paint()
{
    c_rect rect;
    get_screen_rect(rect);
    m_surface->fill_rect(rect, m_back_color, m_z_order);

    //show name
    c_word::draw_string(m_surface, m_z_order, m_wave_name, m_wave_left + 10, rect.m_top, m_wave_name_font, m_wave_name_color, GL_ARGB(0, 0, 0, 0), ALIGN_LEFT);
    //show unit
    c_word::draw_string(m_surface, m_z_order, m_wave_unit, m_wave_left + 60, rect.m_top, m_wave_unit_font, m_wave_unit_color, GL_ARGB(0, 0, 0, 0), ALIGN_LEFT);

    save_background();
}
void c_wave_ctrl::set_wave_name(char* wave_name)
{
    m_wave_name = wave_name;
}
void c_wave_ctrl::set_wave_unit(char* wave_unit)
{
    m_wave_unit = wave_unit;
}

void c_wave_ctrl::set_wave_name_font(const FONT_INFO* wave_name_font_type)
{
    m_wave_name_font = wave_name_font_type;
}
void c_wave_ctrl::set_wave_unit_font(const FONT_INFO* wave_unit_font_type)
{
    m_wave_unit_font = wave_unit_font_type;
}

void c_wave_ctrl::set_wave_name_color(unsigned int wave_name_color)
{
    m_wave_name_color = wave_name_color;
}
void c_wave_ctrl::set_wave_unit_color(unsigned int wave_unit_color)
{
    m_wave_unit_color = wave_unit_color;
}
void c_wave_ctrl::set_wave_color(unsigned int color)
{
    m_wave_color = color;
}
void c_wave_ctrl::set_wave_in_out_rate(unsigned int data_rate, unsigned int refresh_rate)
{
    m_wave_data_rate = data_rate;
    m_wave_refresh_rate = refresh_rate;
    int read_times_per_second = m_wave_speed * 1000 / m_wave_refresh_rate;

    memset(m_frame_len_map, 0, sizeof(m_frame_len_map));
    for (unsigned int i = 1; i < sizeof(m_frame_len_map) + 1; i++)
    {
        m_frame_len_map[i - 1] = data_rate * i / read_times_per_second - data_rate * (i - 1) / read_times_per_second;
    }
    m_frame_len_map_index = 0;
}
void c_wave_ctrl::set_wave_speed(unsigned int speed)
{
    m_wave_speed = speed;
    set_wave_in_out_rate(m_wave_data_rate, m_wave_refresh_rate);
}
void c_wave_ctrl::set_max_min(short max_data, short min_data)
{
    m_max_data = max_data;
    m_min_data = min_data;
}
void c_wave_ctrl::set_wave(c_wave_buffer* wave)
{
    m_wave = wave;
}
c_wave_buffer* c_wave_ctrl::get_wave()
{
    return m_wave;
}
void c_wave_ctrl::clear_data()
{
    if (m_wave == 0)
    {
        ASSERT(false);
        return;
    }
    m_wave->clear_data();
}
bool c_wave_ctrl::is_data_enough()
{
    if (m_wave == 0)
    {
        ASSERT(false);
        return false;
    }
    return (m_wave->get_cnt() - m_frame_len_map[m_frame_len_map_index] * m_wave_speed);
}
void c_wave_ctrl::refresh_wave(unsigned char frame)
{
    if (m_wave == 0)
    {
        ASSERT(false);
        return;
    }

    short max, min, mid;
    for (short offset = 0; offset < m_wave_speed; offset++)
    {
        //get wave value
        mid = m_wave->read_wave_data_by_frame(max, min,
            m_frame_len_map[m_frame_len_map_index++],
            frame, offset);
        m_frame_len_map_index %= sizeof(m_frame_len_map);

        //map to wave ctrl
        int y_min, y_max;
        if (m_max_data == m_min_data)
        {
            ASSERT(false);
        }
        y_max = m_wave_bottom + WAVE_LINE_WIDTH - (m_wave_bottom - m_wave_top) * (min - m_min_data) / (m_max_data - m_min_data);
        y_min = m_wave_bottom - WAVE_LINE_WIDTH - (m_wave_bottom - m_wave_top) * (max - m_min_data) / (m_max_data - m_min_data);
        mid = m_wave_bottom - (m_wave_bottom - m_wave_top) * (mid - m_min_data) / (m_max_data - m_min_data);

        CORRECT(y_min, m_wave_bottom, m_wave_top);
        CORRECT(y_max, m_wave_bottom, m_wave_top);
        CORRECT(mid, m_wave_bottom, m_wave_top);

        if (m_wave_cursor > m_wave_right)
        {
            m_wave_cursor = m_wave_left;
        }
        draw_smooth_vline(y_min, y_max, mid, m_wave_color);
        restore_background();
        m_wave_cursor++;
    }
}
void c_wave_ctrl::clear_wave()
{
    m_surface->fill_rect(m_wave_left, m_wave_top, m_wave_right, m_wave_bottom, m_back_color, m_z_order);
    m_wave_cursor = m_wave_left;
}

void c_wave_ctrl::draw_smooth_vline(int y_min, int y_max, int mid, unsigned int rgb)
{
    int dy = y_max - y_min;
    short r = GL_RGB_R(rgb);
    short g = GL_RGB_G(rgb);
    short b = GL_RGB_B(rgb);
    int  index = (dy >> 1) + 2;
    int  y;

    m_surface->draw_pixel(m_wave_cursor, mid, rgb, m_z_order);

    if (dy < 1)
    {
        return;
    }

    unsigned char cur_r, cur_g, cur_b;
    unsigned int cur_rgb;
    for (int i = 1; i <= (dy >> 1) + 1; ++i)
    {
        if ((mid + i) <= y_max)
        {
            y = mid + i;
            cur_r = r * (index - i) / index;
            cur_g = g * (index - i) / index;
            cur_b = b * (index - i) / index;
            cur_rgb = GL_RGB(cur_r, cur_g, cur_b);
            m_surface->draw_pixel(m_wave_cursor, y, cur_rgb, m_z_order);
        }
        if ((mid - i) >= y_min)
        {
            y = mid - i;
            cur_r = r * (index - i) / index;
            cur_g = g * (index - i) / index;
            cur_b = b * (index - i) / index;
            cur_rgb = GL_RGB(cur_r, cur_g, cur_b);
            m_surface->draw_pixel(m_wave_cursor, y, cur_rgb, m_z_order);
        }
    }
}
void c_wave_ctrl::restore_background()
{
    int  x = m_wave_cursor + WAVE_CURSOR_WIDTH;
    if (x > m_wave_right)
    {
        x -= (m_wave_right - m_wave_left + 1);
    }

    c_rect rect;
    get_screen_rect(rect);
    register int width = rect.width();
    register int top = rect.m_top;
    register int left = rect.m_left;
    for (int y_pos = (m_wave_top - 1); y_pos <= (m_wave_bottom + 1); y_pos++)
    {
        (m_bg_fb) ? m_surface->draw_pixel(x, y_pos, m_bg_fb[(y_pos - top) * width + (x - left)], m_z_order) : m_surface->draw_pixel(x, y_pos, 0, m_z_order);
    }
}
void c_wave_ctrl::save_background()
{
    if (!m_bg_fb)
    {
        return;
    }
    c_rect rect;
    get_screen_rect(rect);

    register unsigned int* p_des = m_bg_fb;
    for (int y = rect.m_top; y <= rect.m_bottom; y++)
    {
        for (int x = rect.m_left; x <= rect.m_right; x++)
        {
            *p_des++ = m_surface->get_pixel(x, y, m_z_order);
        }
    }
}
