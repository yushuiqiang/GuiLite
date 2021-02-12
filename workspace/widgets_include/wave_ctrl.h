#pragma once

#include "../core_include/api.h"
#include "../core_include/wnd.h"
#include "../core_include/display.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../widgets_include/wave_buffer.h"
#include <stdlib.h>
#include <string.h>

#define CORRECT(x, high_limit, low_limit)	{\
	x = (x > high_limit) ? high_limit : x;\
	x = (x < low_limit) ? low_limit : x;\
}while(0)

#define WAVE_CURSOR_WIDTH		8
#define	WAVE_LINE_WIDTH			1
#define	WAVE_MARGIN				5

typedef enum
{
	FILL_MODE,
	SCAN_MODE
}E_WAVE_DRAW_MODE;

class c_wave_buffer;
class c_wave_ctrl : public c_wnd
{
public:
    c_wave_ctrl();

    virtual void on_init_children();//should be pre_create

    virtual void on_paint();

    void set_wave_name(char* wave_name);
    void set_wave_unit(char* wave_unit);

    void set_wave_name_font(const FONT_INFO* wave_name_font_type);
    void set_wave_unit_font(const FONT_INFO* wave_unit_font_type);

    void set_wave_name_color(unsigned int wave_name_color);
    void set_wave_unit_color(unsigned int wave_unit_color);
    void set_wave_color(unsigned int color);
    void set_wave_in_out_rate(unsigned int data_rate, unsigned int refresh_rate);

    void set_wave_speed(unsigned int speed);

    void set_max_min(short max_data, short min_data);

    void set_wave(c_wave_buffer* wave);
    c_wave_buffer* get_wave();
    void clear_data();

    bool is_data_enough();

    void refresh_wave(unsigned char frame);

    void clear_wave();

protected:
    void draw_smooth_vline(int y_min, int y_max, int mid, unsigned int rgb);

    void restore_background();

    void save_background();

	char* m_wave_name;
	char* m_wave_unit;

	const FONT_INFO* m_wave_name_font;
	const FONT_INFO* m_wave_unit_font;

	unsigned int m_wave_name_color;
	unsigned int m_wave_unit_color;

	unsigned int m_wave_color;
	unsigned int m_back_color;

	int m_wave_left;
	int m_wave_right;
	int m_wave_top;
	int m_wave_bottom;

	short m_max_data;
	short m_min_data;
	
private:
	c_wave_buffer*	m_wave;
	unsigned int*	m_bg_fb;			//background frame buffer, could be used to draw scale line.
	int 			m_wave_cursor;
	int 			m_wave_speed;		//pixels per refresh
	unsigned int	m_wave_data_rate;	//data sample rate
	unsigned int	m_wave_refresh_rate;//refresh cycle in millisecond
	unsigned char 	m_frame_len_map[64];
	unsigned char 	m_frame_len_map_index;
};
