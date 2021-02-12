#pragma once

#include "../core_include/api.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SURFACE_CNT_MAX	6//root + pages

typedef enum
{
	Z_ORDER_LEVEL_0,//lowest graphic level
	Z_ORDER_LEVEL_1,//middle graphic level
	Z_ORDER_LEVEL_2,//highest graphic level
	Z_ORDER_LEVEL_MAX
}Z_ORDER_LEVEL;

struct EXTERNAL_GFX_OP
{
	void(*draw_pixel)(int x, int y, unsigned int rgb);
	void(*fill_rect)(int x0, int y0, int x1, int y1, unsigned int rgb);
};

class c_surface;
class c_display
{
	friend class c_surface;
public:
	inline c_display(void* phy_fb, int display_width, int display_height, int surface_width, int surface_height, unsigned int color_bytes, int surface_cnt, EXTERNAL_GFX_OP* gfx_op = 0);//multiple surface or surface_no_fb
	inline c_display(void* phy_fb, int display_width, int display_height, c_surface* surface);//single custom surface
	inline c_surface* alloc_surface(Z_ORDER_LEVEL max_zorder, c_rect layer_rect = c_rect());//for multiple surfaces
	inline int swipe_surface(c_surface* s0, c_surface* s1, int x0, int x1, int y0, int y1, int offset);
    int get_width();
    int get_height();

    void* get_updated_fb(int* width, int* height, bool force_update = false);

    int snap_shot(const char* file_name);

private:
	int				m_width;		//in pixels
	int				m_height;		//in pixels
	int				m_color_bytes;	//16 bits, 32 bits only
	void*			m_phy_fb;		//physical framebuffer
	int				m_phy_read_index;
	int				m_phy_write_index;
	c_surface*		m_surface_group[SURFACE_CNT_MAX];
	int				m_surface_cnt;	//surface count
	int				m_surface_index;
};

class c_layer
{
public:
    c_layer();
	void* fb;		//framebuffer
	c_rect 	rect;	//framebuffer area
};

class c_surface
{
	friend class c_display; friend class c_bitmap;
public:
    c_surface(unsigned int width, unsigned int height, unsigned int color_bytes, Z_ORDER_LEVEL max_zorder = Z_ORDER_LEVEL_0, c_rect overlpa_rect = c_rect());

    int get_width();
    int get_height();

    unsigned int get_pixel(int x, int y, unsigned int z_order);

    virtual void draw_pixel(int x, int y, unsigned int rgb, unsigned int z_order);

    virtual void fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order);

    void draw_hline(int x0, int x1, int y, unsigned int rgb, unsigned int z_order);

    void draw_vline(int x, int y0, int y1, unsigned int rgb, unsigned int z_order);

    void draw_line(int x1, int y1, int x2, int y2, unsigned int rgb, unsigned int z_order);

    void draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order, unsigned int size = 1);

    void draw_rect(c_rect rect, unsigned int rgb, unsigned int size, unsigned int z_order);

    void fill_rect(c_rect rect, unsigned int rgb, unsigned int z_order);

    int flush_screen(int left, int top, int right, int bottom);

    bool is_active();
    c_display* get_display();

    int show_layer(c_rect& rect, unsigned int z_order);

    void set_active(bool flag);
protected:
    virtual void fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb);

    virtual void draw_pixel_on_fb(int x, int y, unsigned int rgb);

    void attach_display(c_display* display);

    void set_surface(Z_ORDER_LEVEL max_z_order, c_rect layer_rect);

	int				m_width;		//in pixels
	int				m_height;		//in pixels
	int				m_color_bytes;	//16 bits, 32 bits only
	void*			m_fb;			//frame buffer you could see
	c_layer 		m_layers[Z_ORDER_LEVEL_MAX];//all graphic layers
	bool			m_is_active;	//active flag
	Z_ORDER_LEVEL	m_max_zorder;	//the highest graphic layer the surface will have
	Z_ORDER_LEVEL	m_top_zorder;	//the current highest graphic layer the surface have
	void*			m_phy_fb;		//physical framebufer
	int*			m_phy_write_index;
	c_display*		m_display;
};

class c_surface_no_fb : public c_surface
{//No physical framebuffer, render with external graphic interface
	friend class c_display;
public:
    c_surface_no_fb(unsigned int width, unsigned int height, unsigned int color_bytes,
                    struct EXTERNAL_GFX_OP* gfx_op, Z_ORDER_LEVEL max_zorder = Z_ORDER_LEVEL_0,
                    c_rect overlpa_rect = c_rect()) ;

protected:
    virtual void fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb);


    virtual void draw_pixel_on_fb(int x, int y, unsigned int rgb);

	struct EXTERNAL_GFX_OP* m_gfx_op;//Rendering by external method
};

inline c_display::c_display(void* phy_fb, int display_width, int display_height, int surface_width, int surface_height, unsigned int color_bytes, int surface_cnt, EXTERNAL_GFX_OP* gfx_op) : m_width(display_width), m_height(display_height), m_color_bytes(color_bytes), m_phy_fb(phy_fb), m_phy_read_index(0), m_phy_write_index(0), m_surface_cnt(surface_cnt), m_surface_index(0)
{
	ASSERT(color_bytes == 2 || color_bytes == 4);
	ASSERT(m_surface_cnt <= SURFACE_CNT_MAX);
	memset(m_surface_group, 0, sizeof(m_surface_group));
	
	for (int i = 0; i < m_surface_cnt; i++)
	{
		m_surface_group[i] = (phy_fb) ? new c_surface(surface_width, surface_height, color_bytes) : new c_surface_no_fb(surface_width, surface_height, color_bytes, gfx_op);
		m_surface_group[i]->attach_display(this);
	}
}

inline c_display::c_display(void* phy_fb, int display_width, int display_height, c_surface* surface) : m_width(display_width), m_height(display_height), m_phy_fb(phy_fb), m_phy_read_index(0), m_phy_write_index(0), m_surface_cnt(1), m_surface_index(0)
{
	m_color_bytes = surface->m_color_bytes;
	surface->m_is_active = true;
	(m_surface_group[0] = surface)->attach_display(this);
}

inline c_surface* c_display::alloc_surface(Z_ORDER_LEVEL max_zorder, c_rect layer_rect)
{
	ASSERT(max_zorder < Z_ORDER_LEVEL_MAX && m_surface_index < m_surface_cnt);
	(layer_rect == c_rect()) ? m_surface_group[m_surface_index]->set_surface(max_zorder, c_rect(0, 0, m_width - 1, m_height - 1)) : m_surface_group[m_surface_index]->set_surface(max_zorder, layer_rect);
	return m_surface_group[m_surface_index++];
}

inline int c_display::swipe_surface(c_surface* s0, c_surface* s1, int x0, int x1, int y0, int y1, int offset)
{
	int surface_width = s0->get_width();
	int surface_height = s0->get_height();

	if (offset < 0 || offset > surface_width || y0 < 0 || y0 >= surface_height ||
		y1 < 0 || y1 >= surface_height || x0 < 0 || x0 >= surface_width || x1 < 0 || x1 >= surface_width)
	{
		ASSERT(false);
		return -1;
	}

	int width = (x1 - x0 + 1);
	if (width < 0 || width > surface_width || width < offset)
	{
		ASSERT(false);
		return -1;
	}

	x0 = (x0 >= m_width) ? (m_width - 1) : x0;
	x1 = (x1 >= m_width) ? (m_width - 1) : x1;
	y0 = (y0 >= m_height) ? (m_height - 1) : y0;
	y1 = (y1 >= m_height) ? (m_height - 1) : y1;

	if (m_phy_fb)
	{
		for (int y = y0; y <= y1; y++)
		{
			//Left surface
			char* addr_s = ((char*)(s0->m_fb) + (y * (s0->get_width()) + x0 + offset) * m_color_bytes);
			char* addr_d = ((char*)(m_phy_fb)+(y * m_width + x0) * m_color_bytes);
			memcpy(addr_d, addr_s, (width - offset) * m_color_bytes);
			//Right surface
			addr_s = ((char*)(s1->m_fb) + (y * (s1->get_width()) + x0) * m_color_bytes);
			addr_d = ((char*)(m_phy_fb)+(y * m_width + x0 + (width - offset)) * m_color_bytes);
			memcpy(addr_d, addr_s, offset * m_color_bytes);
		}
	}
	else if (m_color_bytes == 4)
	{
		void(*draw_pixel)(int x, int y, unsigned int rgb) = ((c_surface_no_fb*)s0)->m_gfx_op->draw_pixel;
		for (int y = y0; y <= y1; y++)
		{
			//Left surface
			for (int x = x0; x <= (x1 - offset); x++)
			{
				draw_pixel(x, y, ((unsigned int*)s0->m_fb)[y * m_width + x + offset]);
			}
			//Right surface
			for (int x = x1 - offset; x <= x1; x++)
			{
				draw_pixel(x, y, ((unsigned int*)s1->m_fb)[y * m_width + x + offset - x1 + x0]);
			}
		}
	}
	else if (m_color_bytes == 2)
	{
		void(*draw_pixel)(int x, int y, unsigned int rgb) = ((c_surface_no_fb*)s0)->m_gfx_op->draw_pixel;
		for (int y = y0; y <= y1; y++)
		{
			//Left surface
			for (int x = x0; x <= (x1 - offset); x++)
			{
				draw_pixel(x, y, GL_RGB_16_to_32(((unsigned short*)s0->m_fb)[y * m_width + x + offset]));
			}
			//Right surface
			for (int x = x1 - offset; x <= x1; x++)
			{
				draw_pixel(x, y, GL_RGB_16_to_32(((unsigned short*)s1->m_fb)[y * m_width + x + offset - x1 + x0]));
			}
		}
	}

	m_phy_write_index++;
	return 0;
}
