#include "../widgets_include/keyboard.h"

#ifdef GUILITE_ON

static c_keyboard_button s_key_0, s_key_1, s_key_2, s_key_3, s_key_4, s_key_5, s_key_6, s_key_7, s_key_8, s_key_9;
static c_keyboard_button s_key_A, s_key_B, s_key_C, s_key_D, s_key_E, s_key_F, s_key_G, s_key_H, s_key_I, s_key_J;
static c_keyboard_button s_key_K, s_key_L, s_key_M, s_key_N, s_key_O, s_key_P, s_key_Q, s_key_R, s_key_S, s_key_T;
static c_keyboard_button s_key_U, s_key_V, s_key_W, s_key_X, s_key_Y, s_key_Z;
static c_keyboard_button s_key_dot, s_key_caps, s_key_space, s_key_enter, s_key_del, s_key_esc, s_key_num_switch;

WND_TREE g_key_board_children[] =
{
	//Row 1
	{&s_key_Q, 'Q', 0, POS_X(0), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_W, 'W', 0, POS_X(1), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_E, 'E', 0, POS_X(2), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_R, 'R', 0, POS_X(3), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_T, 'T', 0, POS_X(4), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_Y, 'Y', 0, POS_X(5), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_U, 'U', 0, POS_X(6), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_I, 'I', 0, POS_X(7), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_O, 'O', 0, POS_X(8), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_P, 'P', 0, POS_X(9), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	//Row 2 
	{&s_key_A, 'A', 0, ((KEY_WIDTH / 2) + POS_X(0)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_S, 'S', 0, ((KEY_WIDTH / 2) + POS_X(1)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_D, 'D', 0, ((KEY_WIDTH / 2) + POS_X(2)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_F, 'F', 0, ((KEY_WIDTH / 2) + POS_X(3)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_G, 'G', 0, ((KEY_WIDTH / 2) + POS_X(4)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_H, 'H', 0, ((KEY_WIDTH / 2) + POS_X(5)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_J, 'J', 0, ((KEY_WIDTH / 2) + POS_X(6)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_K, 'K', 0, ((KEY_WIDTH / 2) + POS_X(7)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_L, 'L', 0, ((KEY_WIDTH / 2) + POS_X(8)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	//Row 3
	{&s_key_caps, 0x14,	0, POS_X(0),						POS_Y(2), CAPS_WIDTH,	KEY_HEIGHT},
	{&s_key_Z,	'Z',	0, ((KEY_WIDTH / 2) + POS_X(1)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_key_X,	'X',	0, ((KEY_WIDTH / 2) + POS_X(2)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_key_C,	'C',	0, ((KEY_WIDTH / 2) + POS_X(3)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_key_V,	'V',	0, ((KEY_WIDTH / 2) + POS_X(4)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_key_B,	'B',	0, ((KEY_WIDTH / 2) + POS_X(5)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_key_N,	'N',	0, ((KEY_WIDTH / 2) + POS_X(6)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_key_M,	'M',	0, ((KEY_WIDTH / 2) + POS_X(7)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_key_del,	0x7F,	0, ((KEY_WIDTH / 2) + POS_X(8)),	POS_Y(2), DEL_WIDTH,	KEY_HEIGHT},
	//Row 4
	{&s_key_esc,			0x1B,	0, POS_X(0),						POS_Y(3), ESC_WIDTH,	KEY_HEIGHT},
	{&s_key_num_switch,	0x90,	0, POS_X(2),						POS_Y(3), SWITCH_WIDTH,	KEY_HEIGHT},
	{&s_key_space,		' ',	0, ((KEY_WIDTH / 2) + POS_X(3)),	POS_Y(3), SPACE_WIDTH,	KEY_HEIGHT},
	{&s_key_dot,			'.',	0, ((KEY_WIDTH / 2) + POS_X(6)),	POS_Y(3), DOT_WIDTH,	KEY_HEIGHT},
	{&s_key_enter,		'\n',	0, POS_X(8),						POS_Y(3), ENTER_WIDTH,	KEY_HEIGHT},
	{0,0,0,0,0,0,0}
};

WND_TREE g_number_board_children[] =
{
	{&s_key_1,	'1',	0, POS_X(0), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_2,	'2',	0, POS_X(1), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_3,	'3',	0, POS_X(2), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},

	{&s_key_4,	'4',	0, POS_X(0), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_5,	'5',	0, POS_X(1), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_6,	'6',	0, POS_X(2), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},

	{&s_key_7,	'7',	0, POS_X(0), POS_Y(2), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_8,	'8',	0, POS_X(1), POS_Y(2), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_9,	'9',	0, POS_X(2), POS_Y(2), KEY_WIDTH, KEY_HEIGHT},
	
	{&s_key_esc,	0x1B,	0, POS_X(0), POS_Y(3), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_0,	'0',	0, POS_X(1), POS_Y(3), KEY_WIDTH, KEY_HEIGHT},
	{&s_key_dot,	'.',	0, POS_X(2), POS_Y(3), KEY_WIDTH, KEY_HEIGHT},

	{&s_key_del, 0x7F,	0, POS_X(3), POS_Y(0), KEY_WIDTH, KEY_HEIGHT * 2 + 2},
	{&s_key_enter,'\n',	0, POS_X(3), POS_Y(2), KEY_WIDTH, KEY_HEIGHT * 2 + 2},
	{0,0,0,0,0,0,0}
};

int c_keyboard::connect(c_wnd *user, unsigned short resource_id, KEYBOARD_STYLE style)
{
    c_rect user_rect;
    user->get_wnd_rect(user_rect);
    if (style == STYLE_ALL_BOARD)
    {//Place keyboard at the bottom of user's parent window.
        c_rect user_parent_rect;
        user->get_parent()->get_wnd_rect(user_parent_rect);
        return c_wnd::connect(user, resource_id, 0, (0 - user_rect.m_left), (user_parent_rect.height() - user_rect.m_top - KEYBOARD_HEIGHT - 1), KEYBOARD_WIDTH, KEYBOARD_HEIGHT, g_key_board_children);
    }
    else if (style == STYLE_NUM_BOARD)
    {//Place keyboard below the user window.
        return c_wnd::connect(user, resource_id, 0, 0, user_rect.height(), NUM_BOARD_WIDTH, NUM_BOARD_HEIGHT, g_number_board_children);
    }
    else
    {
        ASSERT(false);
    }
    return -1;
}
void c_keyboard::on_init_children()
{
    c_wnd* child = m_top_child;
    if (0 != child)
    {
        while (child)
        {
            ((c_button*)child)->set_on_click(WND_CALLBACK(&c_keyboard::on_key_clicked));
            child = child->get_next_sibling();
        }
    }
}

KEYBOARD_STATUS c_keyboard::get_cap_status()
{
    return m_cap_status;
}
char* c_keyboard::get_str()
{
    return m_str;
}
void c_keyboard::set_on_click(WND_CALLBACK on_click)
{
    this->on_click = on_click;
}

void c_keyboard::pre_create_wnd()
{
    m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
    m_cap_status = STATUS_UPPERCASE;
    memset(m_str, 0, sizeof(m_str));
    m_str_len = 0;
}
void c_keyboard::on_paint()
{
    c_rect rect;
    get_screen_rect(rect);
    m_surface->fill_rect(rect, GL_RGB(0, 0, 0), m_z_order);
}

void c_keyboard::on_key_clicked(int id, int param)
{
    switch (id)
    {
    case 0x14:
        on_caps_clicked(id, param);
        break;
    case '\n':
        on_enter_clicked(id, param);
        break;
    case 0x1B:
        on_esc_clicked(id, param);
        break;
    case 0x7F:
        on_del_clicked(id, param);
        break;
    default:
        on_char_clicked(id, param);
        break;
    }
}
void c_keyboard::on_char_clicked(int id, int param)
{//id = char ascii code.
    if (m_str_len >= sizeof(m_str))
    {
        return;
    }
    if ((id >= '0' && id <= '9') || id == ' ' || id == '.')
    {
        goto InputChar;
    }

    if (id >= 'A' && id <= 'Z')
    {
        if (STATUS_LOWERCASE == m_cap_status)
        {
            id += 0x20;
        }
        goto InputChar;
    }
    ASSERT(false);
InputChar:
    m_str[m_str_len++] = id;
    (m_parent->*(on_click))(m_id, CLICK_CHAR);
}
void c_keyboard::on_del_clicked(int id, int param)
{
    if (m_str_len <= 0)
    {
        return;
    }
    m_str[--m_str_len] = 0;
    (m_parent->*(on_click))(m_id, CLICK_CHAR);
}
void c_keyboard::on_caps_clicked(int id, int param)
{
    m_cap_status = (m_cap_status == STATUS_LOWERCASE) ? STATUS_UPPERCASE : STATUS_LOWERCASE;
    show_window();
}
void c_keyboard::on_enter_clicked(int id, int param)
{
    memset(m_str, 0, sizeof(m_str));
    (m_parent->*(on_click))(m_id, CLICK_ENTER);
}
void c_keyboard::on_esc_clicked(int id, int param)
{
    memset(m_str, 0, sizeof(m_str));
    (m_parent->*(on_click))(m_id, CLICK_ESC);
}

/****************************************************************************************/
void c_keyboard_button::on_paint()
    {
        c_rect rect;
        get_screen_rect(rect);
        switch (m_status)
        {
        case STATUS_NORMAL:
            m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
            break;
        case STATUS_FOCUSED:
            m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
            break;
        case STATUS_PUSHED:
            m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_PUSHED), m_z_order);
            m_surface->draw_rect(rect, c_theme::get_color(COLOR_WND_BORDER), 2, m_z_order);
            break;
        default:
            ASSERT(false);
            break;
        }

        if (m_id == 0x14)
        {
            return c_word::draw_string_in_rect(m_surface, m_z_order, "Caps", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
        }
        else if (m_id == 0x1B)
        {
            return c_word::draw_string_in_rect(m_surface, m_z_order, "Esc", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
        }
        else if (m_id == ' ')
        {
            return c_word::draw_string_in_rect(m_surface, m_z_order, "Space", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
        }
        else if (m_id == '\n')
        {
            return c_word::draw_string_in_rect(m_surface, m_z_order, "Enter", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
        }
        else if (m_id == '.')
        {
            return c_word::draw_string_in_rect(m_surface, m_z_order, ".", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
        }
        else if (m_id == 0x7F)
        {
            return c_word::draw_string_in_rect(m_surface, m_z_order, "Back", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
        }
        else if (m_id == 0x90)
        {
            return c_word::draw_string_in_rect(m_surface, m_z_order, "?123", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
        }

        char letter[] = { 0, 0 };
        if (m_id >= 'A' && m_id <= 'Z')
        {
            letter[0] = (((c_keyboard*)m_parent)->get_cap_status() == STATUS_UPPERCASE) ? m_id : (m_id + 0x20);
        }
        else if (m_id >= '0' && m_id <= '9')
        {
            letter[0] = (char)m_id;
        }
        c_word::draw_string_in_rect(m_surface, m_z_order, letter, rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
    }
#endif
