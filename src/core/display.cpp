#include "../../include/core_include/display.h"

int c_display::get_width()
{
    return m_width;
}
int c_display::get_height()
{
    return m_height;
}

void* c_display::get_updated_fb(int* width, int* height, bool force_update)
{
    if (width && height)
    {
        *width = get_width();
        *height = get_height();
    }
    if (force_update)
    {
        return m_phy_fb;
    }
    if (m_phy_read_index == m_phy_write_index)
    {//No update
        return 0;
    }
    m_phy_read_index = m_phy_write_index;
    return m_phy_fb;
}

int c_display::snap_shot(const char* file_name)
{
    if (!m_phy_fb || (m_color_bytes !=2 && m_color_bytes != 4))
    {
        return -1;
    }

    int width = get_width();
    int height = get_height();

    //16 bits framebuffer
    if (m_color_bytes == 2)
    {
        return build_bmp(file_name, width, height, (unsigned char*)m_phy_fb);
    }

    //32 bits framebuffer
    unsigned short* p_bmp565_data = new unsigned short[width * height];
    unsigned int* p_raw_data = (unsigned int*)m_phy_fb;

    for (int i = 0; i < width * height; i++)
    {
        unsigned int rgb = *p_raw_data++;
        p_bmp565_data[i] = GL_RGB_32_to_16(rgb);
    }

    int ret = build_bmp(file_name, width, height, (unsigned char*)p_bmp565_data);
    delete[]p_bmp565_data;
    return ret;
}
/*****************************************************************************************/
c_layer::c_layer()
{
    fb = 0;
}
/*****************************************************************************************/
c_surface::c_surface(unsigned int width, unsigned int height,
                     unsigned int color_bytes,
                     Z_ORDER_LEVEL max_zorder, c_rect overlpa_rect
                     ) : m_width(width), m_height(height),
    m_color_bytes(color_bytes), m_fb(0),
    m_is_active(false), m_top_zorder(Z_ORDER_LEVEL_0),
    m_phy_fb(0), m_phy_write_index(0), m_display(0)
{
    (overlpa_rect == c_rect()) ? set_surface(max_zorder, c_rect(0, 0, width - 1, height - 1)) : set_surface(max_zorder, overlpa_rect);
}

int c_surface::get_width() { return m_width; }
int c_surface::get_height() { return m_height; }

unsigned int c_surface::get_pixel(int x, int y, unsigned int z_order)
{
    if (x >= m_width || y >= m_height || x < 0 || y < 0 || z_order >= Z_ORDER_LEVEL_MAX)
    {
        ASSERT(false);
        return 0;
    }
    if (m_layers[z_order].fb)
    {
        return (m_color_bytes == 4) ? ((unsigned int*)(m_layers[z_order].fb))[y * m_width + x] : GL_RGB_16_to_32(((unsigned short*)(m_layers[z_order].fb))[y * m_width + x]);
    }
    else if (m_fb)
    {
        return (m_color_bytes == 4) ? ((unsigned int*)m_fb)[y * m_width + x] : GL_RGB_16_to_32(((unsigned short*)m_fb)[y * m_width + x]);
    }
    else if (m_phy_fb)
    {
        return (m_color_bytes == 4) ? ((unsigned int*)m_phy_fb)[y * m_width + x] : GL_RGB_16_to_32(((unsigned short*)m_phy_fb)[y * m_width + x]);
    }
    return 0;
}

void c_surface::draw_pixel(int x, int y, unsigned int rgb, unsigned int z_order)
{
    if (x >= m_width || y >= m_height || x < 0 || y < 0)
    {
        return;
    }
    if (z_order > (unsigned int)m_max_zorder)
    {
        ASSERT(false);
        return;
    }

    if (z_order == m_max_zorder)
    {
        return draw_pixel_on_fb(x, y, rgb);
    }

    if (z_order > (unsigned int)m_top_zorder)
    {
        m_top_zorder = (Z_ORDER_LEVEL)z_order;
    }

    if (m_layers[z_order].rect.pt_in_rect(x, y))
    {
        c_rect layer_rect = m_layers[z_order].rect;
        if (m_color_bytes == 4)
        {
            ((unsigned int*)(m_layers[z_order].fb))[(x - layer_rect.m_left) + (y - layer_rect.m_top) * layer_rect.width()] = rgb;
        }
        else
        {
            ((unsigned short*)(m_layers[z_order].fb))[(x - layer_rect.m_left) + (y - layer_rect.m_top) * layer_rect.width()] = GL_RGB_32_to_16(rgb);
        }
    }

    if (z_order == m_top_zorder)
    {
        return draw_pixel_on_fb(x, y, rgb);
    }

    bool be_overlapped = false;
    for (unsigned int tmp_z_order = Z_ORDER_LEVEL_MAX - 1; tmp_z_order > z_order; tmp_z_order--)
    {
        if (m_layers[tmp_z_order].rect.pt_in_rect(x, y))
        {
            be_overlapped = true;
            break;
        }
    }

    if (!be_overlapped)
    {
        draw_pixel_on_fb(x, y, rgb);
    }
}

void c_surface::fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order)
{
    x0 = (x0 < 0) ? 0 : x0;
    y0 = (y0 < 0) ? 0 : y0;
    x1 = (x1 > (m_width - 1)) ? (m_width - 1) : x1;
    y1 = (y1 > (m_height - 1)) ? (m_height - 1) : y1;

    if (z_order == m_max_zorder)
    {
        return fill_rect_on_fb(x0, y0, x1, y1, rgb);
    }

    if (z_order == m_top_zorder)
    {
        int x, y;
        c_rect layer_rect = m_layers[z_order].rect;
        unsigned int rgb_16 = GL_RGB_32_to_16(rgb);
        for (y = y0; y <= y1; y++)
        {
            for (x = x0; x <= x1; x++)
            {
                if (layer_rect.pt_in_rect(x, y))
                {
                    if (m_color_bytes == 4)
                    {
                        ((unsigned int*)m_layers[z_order].fb)[(y - layer_rect.m_top) * layer_rect.width() + (x - layer_rect.m_left)] = rgb;
                    }
                    else
                    {
                        ((unsigned short*)m_layers[z_order].fb)[(y - layer_rect.m_top) * layer_rect.width() + (x - layer_rect.m_left)] = rgb_16;
                    }
                }
            }
        }
        return fill_rect_on_fb(x0, y0, x1, y1, rgb);
    }

    for (; y0 <= y1; y0++)
    {
        draw_hline(x0, x1, y0, rgb, z_order);
    }
}

void c_surface::draw_hline(int x0, int x1, int y, unsigned int rgb, unsigned int z_order)
{
    for (; x0 <= x1; x0++)
    {
        draw_pixel(x0, y, rgb, z_order);
    }
}

void c_surface::draw_vline(int x, int y0, int y1, unsigned int rgb, unsigned int z_order)
{
    for (; y0 <= y1; y0++)
    {
        draw_pixel(x, y0, rgb, z_order);
    }
}

void c_surface::draw_line(int x1, int y1, int x2, int y2, unsigned int rgb, unsigned int z_order)
{
    int dx, dy, x, y, e;

    (x1 > x2) ? (dx = x1 - x2) : (dx = x2 - x1);
    (y1 > y2) ? (dy = y1 - y2) : (dy = y2 - y1);

    if (((dx > dy) && (x1 > x2)) || ((dx <= dy) && (y1 > y2)))
    {
        x = x2; y = y2;
        x2 = x1; y2 = y1;
        x1 = x; y1 = y;
    }
    x = x1; y = y1;

    if (dx > dy)
    {
        e = dy - dx / 2;
        for (; x1 <= x2; ++x1, e += dy)
        {
            draw_pixel(x1, y1, rgb, z_order);
            if (e > 0) { e -= dx; (y > y2) ? --y1 : ++y1; }
        }
    }
    else
    {
        e = dx - dy / 2;
        for (; y1 <= y2; ++y1, e += dx)
        {
            draw_pixel(x1, y1, rgb, z_order);
            if (e > 0) { e -= dy; (x > x2) ? --x1 : ++x1; }
        }
    }
}

void c_surface::draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order, unsigned int size)
{
    for (unsigned int offset = 0; offset < size; offset++)
    {
        draw_hline(x0 + offset, x1 - offset, y0 + offset, rgb, z_order);
        draw_hline(x0 + offset, x1 - offset, y1 - offset, rgb, z_order);
        draw_vline(x0 + offset, y0 + offset, y1 - offset, rgb, z_order);
        draw_vline(x1 - offset, y0 + offset, y1 - offset, rgb, z_order);
    }
}

void c_surface::draw_rect(c_rect rect, unsigned int rgb, unsigned int size, unsigned int z_order)
{
    draw_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, z_order, size);
}

void c_surface::fill_rect(c_rect rect, unsigned int rgb, unsigned int z_order)
{
    fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, z_order);
}

int c_surface::flush_screen(int left, int top, int right, int bottom)
{
    if (left < 0 || left >= m_width || right < 0 || right >= m_width ||
        top < 0 || top >= m_height || bottom < 0 || bottom >= m_height)
    {
        ASSERT(false);
    }

    if (!m_is_active || (0 == m_phy_fb) || (0 == m_fb))
    {
        return -1;
    }

    int display_width = m_display->get_width();
    int display_height = m_display->get_height();

    left = (left >= display_width) ? (display_width - 1) : left;
    right = (right >= display_width) ? (display_width - 1) : right;
    top = (top >= display_height) ? (display_height - 1) : top;
    bottom = (bottom >= display_height) ? (display_height - 1) : bottom;

    for (int y = top; y < bottom; y++)
    {
        void* s_addr = (char*)m_fb + ((y * m_width + left) * m_color_bytes);
        void* d_addr = (char*)m_phy_fb + ((y * display_width + left) * m_color_bytes);
        memcpy(d_addr, s_addr, (right - left) * m_color_bytes);
    }
    *m_phy_write_index = *m_phy_write_index + 1;
    return 0;
}

bool c_surface::is_active() { return m_is_active; }
c_display* c_surface::get_display() { return m_display; }

int c_surface::show_layer(c_rect& rect, unsigned int z_order)
{
    ASSERT(z_order >= Z_ORDER_LEVEL_0 && z_order < Z_ORDER_LEVEL_MAX);

    c_rect layer_rect = m_layers[z_order].rect;
    ASSERT(rect.m_left >= layer_rect.m_left && rect.m_right <= layer_rect.m_right &&
        rect.m_top >= layer_rect.m_top && rect.m_bottom <= layer_rect.m_bottom);

    void* fb = m_layers[z_order].fb;
    int width = layer_rect.width();
    for (int y = rect.m_top; y <= rect.m_bottom; y++)
    {
        for (int x = rect.m_left; x <= rect.m_right; x++)
        {
            unsigned int rgb = (m_color_bytes == 4) ? ((unsigned int*)fb)[(x - layer_rect.m_left) + (y - layer_rect.m_top) * width] : GL_RGB_16_to_32(((unsigned short*)fb)[(x - layer_rect.m_left) + (y - layer_rect.m_top) * width]);
            draw_pixel_on_fb(x, y, rgb);
        }
    }
    return 0;
}
void c_surface::set_active(bool flag) { m_is_active = flag; }

void c_surface::fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb)
{
    int display_width = m_display->get_width();
    int display_height = m_display->get_height();

    if (m_color_bytes == 4)
    {
        int x;
        unsigned int* fb, * phy_fb;
        for (; y0 <= y1; y0++)
        {
            x = x0;
            fb = m_fb ? &((unsigned int*)m_fb)[y0 * m_width + x] : 0;
            phy_fb = &((unsigned int*)m_phy_fb)[y0 * display_width + x];
            *m_phy_write_index = *m_phy_write_index + 1;
            for (; x <= x1; x++)
            {
                if (fb)
                {
                    *fb++ = rgb;
                }
                if (m_is_active && (x < display_width) && (y0 < display_height))
                {
                    *phy_fb++ = rgb;
                }
            }
        }
    }
    else if (m_color_bytes == 2)
    {
        int x;
        unsigned short* fb, * phy_fb;
        rgb = GL_RGB_32_to_16(rgb);
        for (; y0 <= y1; y0++)
        {
            x = x0;
            fb = m_fb ? &((unsigned short*)m_fb)[y0 * m_width + x] : 0;
            phy_fb = &((unsigned short*)m_phy_fb)[y0 * display_width + x];
            *m_phy_write_index = *m_phy_write_index + 1;
            for (; x <= x1; x++)
            {
                if (fb)
                {
                    *fb++ = rgb;
                }
                if (m_is_active && (x < display_width) && (y0 < display_height))
                {
                    *phy_fb++ = rgb;
                }
            }
        }
    }
}

void c_surface::draw_pixel_on_fb(int x, int y, unsigned int rgb)
{
    if (m_fb)
    {
        (m_color_bytes == 4) ? ((unsigned int*)m_fb)[y * m_width + x] = rgb : ((unsigned short*)m_fb)[y * m_width + x] = GL_RGB_32_to_16(rgb);
    }

    if (m_is_active && (x < m_display->get_width()) && (y < m_display->get_height()))
    {
        if (m_color_bytes == 4)
        {
            ((unsigned int*)m_phy_fb)[y * (m_display->get_width()) + x] = rgb;
        }
        else
        {
            ((unsigned short*)m_phy_fb)[y * (m_display->get_width()) + x] = GL_RGB_32_to_16(rgb);
        }
        *m_phy_write_index = *m_phy_write_index + 1;
    }
}

void c_surface::attach_display(c_display* display)
{
    ASSERT(display);
    m_display = display;
    m_phy_fb = display->m_phy_fb;
    m_phy_write_index = &display->m_phy_write_index;
}

void c_surface::set_surface(Z_ORDER_LEVEL max_z_order, c_rect layer_rect)
{
    m_max_zorder = max_z_order;
    if (m_display && (m_display->m_surface_cnt > 1))
    {
        m_fb = calloc(m_width * m_height, m_color_bytes);
    }

    for (int i = Z_ORDER_LEVEL_0; i < m_max_zorder; i++)
    {//Top layber fb always be 0
        ASSERT(m_layers[i].fb = calloc(layer_rect.width() * layer_rect.height(), m_color_bytes));
        m_layers[i].rect = layer_rect;
    }
}


/****************************************************************************************************/
c_surface_no_fb::c_surface_no_fb(unsigned int width, unsigned int height,
                                 unsigned int color_bytes,
                                 struct EXTERNAL_GFX_OP* gfx_op,
                                 Z_ORDER_LEVEL max_zorder,
                                 c_rect overlpa_rect) :
    c_surface(width, height, color_bytes, max_zorder, overlpa_rect), m_gfx_op(gfx_op)
{

}

void c_surface_no_fb::fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb)
{
    if (!m_gfx_op)
    {
        return;
    }
    if (m_gfx_op->fill_rect)
    {
        return m_gfx_op->fill_rect(x0, y0, x1, y1, rgb);
    }

    if (m_gfx_op->draw_pixel && m_is_active)
    {
        for (int y = y0; y <= y1; y++)
        {
            for (int x = x0; x <= x1; x++)
            {
                m_gfx_op->draw_pixel(x, y, rgb);
            }
        }
    }

    if (!m_fb) { return; }
    if (m_color_bytes == 4)
    {
        unsigned int* fb;
        for (int y = y0; y <= y1; y++)
        {
            fb = &((unsigned int*)m_fb)[y0 * m_width + x0];
            for (int x = x0; x <= x1; x++)
            {
                *fb++ = rgb;
            }
        }
    }
    else if (m_color_bytes == 2)
    {
        unsigned short* fb;
        rgb = GL_RGB_32_to_16(rgb);
        for (int y = y0; y <= y1; y++)
        {
            fb = &((unsigned short*)m_fb)[y0 * m_width + x0];
            for (int x = x0; x <= x1; x++)
            {
                *fb++ = rgb;
            }
        }
    }
}

void c_surface_no_fb::draw_pixel_on_fb(int x, int y, unsigned int rgb)
{
    if (m_gfx_op && m_gfx_op->draw_pixel && m_is_active)
    {
        m_gfx_op->draw_pixel(x, y, rgb);
    }

    if (!m_fb) { return; }
    if (m_color_bytes == 4)
    {
        ((unsigned int*)m_fb)[y * m_width + x] = rgb;
    }
    else if (m_color_bytes == 2)
    {
        ((unsigned short*)m_fb)[y * m_width + x] = GL_RGB_32_to_16(rgb);
    }
}
