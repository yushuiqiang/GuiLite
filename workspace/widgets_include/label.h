#pragma once

#include "../core_include/api.h"
#include "../core_include/wnd.h"
#include "../core_include/display.h"
#include "../core_include/resource.h"
#include "../core_include/bitmap.h"
#include "../core_include/theme.h"
#include "../core_include/word.h"

class c_label : public c_wnd
{
public:
    virtual void on_paint();
protected:
    virtual void pre_create_wnd();
};
