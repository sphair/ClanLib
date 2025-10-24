// NS_MessageBox.cpp: implementation of the NS_MessageBox class.
//
//////////////////////////////////////////////////////////////////////

#include "NS_MessageBox.h"

#include <ClanLib/signals.h>
#include <ClanLib/signals.h>
#include <ClanLib/display.h>
#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/core.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NS_MessageBox::NS_MessageBox(CL_Component * parent,
        std::string const & title,
        std::string const & text,
        std::string const & button1,
        std::string const & button2,
        std::string const & button3)
    : CL_Window(parent, parent->get_style_manager()),
    result(-1),
    title(title),
    text(text),
    button1_text(button1),
    button2_text(button2),
    button3_text(button3),
	font(0)
{
    font = new CL_Font("my_fnt_4", get_style_manager()->get_resources());

    int width = 400;
    int height = 50 + 40 + font->get_height(text, CL_Size(width - 15, 0));

    // Calc dialog position
    set_position(CL_Rect(
        CL_Display::get_width()  / 2 - width / 2,
        CL_Display::get_height() / 2 - height / 2,
        CL_Display::get_width()  / 2 + width / 2,
        CL_Display::get_height() / 2 + height / 2));
    
    set_title(title);

    int num_buttons = 1;
    if (!button2_text.empty())
        num_buttons = 2;
    if (!button3_text.empty())
        num_buttons = 3;
    
    int btn_pos_y = height - 40;

    switch(num_buttons) {
    case 1:
        {
            CL_Button * btn_1 = new CL_Button(CL_Point(width / 2, btn_pos_y), button1_text, get_client_area());
            btn_1->set_position(width / 2 - btn_1->get_width() / 2, btn_pos_y);
            slots.connect(btn_1->sig_clicked(), this, &NS_MessageBox::on_button1);
            btn_1->find_preferred_size();
            components["btn_1"] = btn_1;
        }
    	break;
    case 2:
        {
            CL_Button * btn_1 = new CL_Button(CL_Point(width / 2, btn_pos_y), button1_text, get_client_area());
            CL_Button * btn_2 = new CL_Button(CL_Point(width / 2, btn_pos_y), button2_text, get_client_area());
            btn_1->find_preferred_size();
            btn_2->find_preferred_size();
            {
                int const total_size = btn_1->get_width() + 20 + btn_2->get_width();
                btn_1->set_position(width / 2 - total_size / 2, btn_pos_y);
                btn_2->set_position(width / 2 - total_size / 2 + btn_1->get_width() + 20, btn_pos_y);
            }
            slots.connect(btn_1->sig_clicked(), this, &NS_MessageBox::on_button1);
            slots.connect(btn_2->sig_clicked(), this, &NS_MessageBox::on_button2);
            components["btn_1"] = btn_1;
            components["btn_2"] = btn_2;
        }
    	break;
    case 3:
        {
            CL_Button * btn_1 = new CL_Button(CL_Point(width / 2, btn_pos_y), button1_text, get_client_area());
            CL_Button * btn_2 = new CL_Button(CL_Point(width / 2, btn_pos_y), button2_text, get_client_area());
            CL_Button * btn_3 = new CL_Button(CL_Point(width / 2, btn_pos_y), button3_text, get_client_area());
            btn_1->find_preferred_size();
            btn_2->find_preferred_size();
            btn_3->find_preferred_size();
            {
                int const total_size = btn_1->get_width() + 20 +
                    btn_2->get_width() + 20 + btn_3->get_width();
                btn_1->set_position(width / 2 - total_size / 2, btn_pos_y);
                btn_2->set_position(width / 2 - total_size / 2 +
                    btn_1->get_width() + 20, btn_pos_y);
                btn_3->set_position(width / 2 - total_size / 2 +
                    btn_1->get_width() + 20 +
                    btn_2->get_width() + 20, btn_pos_y);
            }
            slots.connect(btn_1->sig_clicked(), this, &NS_MessageBox::on_button1);
            slots.connect(btn_2->sig_clicked(), this, &NS_MessageBox::on_button2);
            slots.connect(btn_3->sig_clicked(), this, &NS_MessageBox::on_button3);
            components["btn_1"] = btn_1;
            components["btn_2"] = btn_2;
            components["btn_3"] = btn_3;
        }
    	break;
    }

    // Connect signals
    slots.connect(CL_Keyboard::sig_key_down(), this,
        &NS_MessageBox::on_key_down);
    slots.connect(this->sig_end_paint(), this, &NS_MessageBox::on_end_paint);
}

NS_MessageBox::~NS_MessageBox()
{
	std::map<std::string, CL_Component * >::iterator it = components.begin();
	for(; it != components.end(); ++it)
		delete it->second;
	delete font;
}

void NS_MessageBox::on_key_down(CL_InputEvent const & key)
{
    if (key.id == CL_KEY_ESCAPE)
    {
        result = -1;
        quit();
    }
    else
        if (key.id == CL_KEY_RETURN)
        {
            result = 1;
            quit();
        }
}

int NS_MessageBox::info(CL_Component * parent,
        std::string const & title,
        std::string const & text,
        std::string const & button1,
        std::string const & button2,
        std::string const & button3)
{
    NS_MessageBox dlg(parent,
        title,
        text,
        button1,
        button2,
        button3);
    dlg.run();
    return dlg.get_result();
}

int NS_MessageBox::get_result() const
{
    return result;
}

void NS_MessageBox::on_button1()
{
    result = 1;
    quit();
}

void NS_MessageBox::on_button2()
{
    result = 2;
    quit();
}

void NS_MessageBox::on_button3()
{
    result = 3;
    quit();
}

void NS_MessageBox::on_end_paint()
{
    CL_Rect rect = get_position();
    rect.left   += 15 + 15;
    rect.right  -= 15;
    rect.top    += 50;
    rect.bottom -= 40;
    font->draw(rect, text);
}
