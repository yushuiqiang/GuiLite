﻿#pragma once

#include "../core_include/api.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/display.h"
#include "../core_include/theme.h"
#include "../core_include/wnd.h"

#define  MAX_COL_NUM  30
#define  MAX_ROW_NUM  30

class c_table: public c_wnd
{
public:
    void set_sheet_align(unsigned int align_type);

    void set_row_num(unsigned int row_num);

    void set_col_num(unsigned int col_num);

    void set_row_height(unsigned int height);

    void set_col_width(unsigned int width);

    int set_row_height(unsigned int index, unsigned int height);

    int set_col_width(unsigned int index, unsigned int width);

    void set_item(int row, int col, char* str, unsigned int color);


    unsigned int get_row_num();
    unsigned int get_col_num();
    c_rect get_item_rect(int row, int col);

protected:
    virtual void pre_create_wnd();

    void draw_item(int row, int col, const char* str, unsigned int color);


	unsigned int m_align_type;	
	unsigned int m_row_num;
	unsigned int m_col_num;
	unsigned int m_row_height[MAX_ROW_NUM];
	unsigned int m_col_width[MAX_COL_NUM];
};
