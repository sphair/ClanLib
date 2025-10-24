// NS_Profile.cpp: implementation of the NS_Profile class.
//
//////////////////////////////////////////////////////////////////////

#include "NS_Profile.h"
#include <ClanLib/signals.h>
#include <ClanLib/signals.h>
#include <ClanLib/display.h>
#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/core.h>

#include "NS_MessageBox.h"

#include <algorithm>
#include <functional>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NS_Profile::NS_Profile(CL_Component * parent)
    : CL_Window(parent, parent->get_style_manager()),
    success(false)
{
    CL_ListBox * lst_players = new CL_ListBox(CL_Rect(15, 35, 350, 255), get_client_area());
    CL_Button * btn_add = new CL_Button(CL_Point(15, 260), "New Player", get_client_area());
    CL_Button * btn_del = new CL_Button(CL_Point(140, 260), "Delete", get_client_area());
    CL_Button * btn_ok = new CL_Button(CL_Point(325, 260), "OK", get_client_area());

	// Calc dialog position
    set_position(CL_Rect(100, 100, 477, 400));
    set_title("Choose player"); 

    // Add players to list
    lst_players->set_multi_selection(false);

	// if we have players, then show them in dialog
	lst_players->insert_item("zz|sergant");
	lst_players->insert_item("Sphair");
	lst_players->insert_item("Judas");
	lst_players->insert_item("grumbel");
	lst_players->insert_item("Blacky Gray");
	lst_players->insert_item("carbon");
	lst_players->insert_item("Supaplex");
	lst_players->insert_item("Terminator");

	lst_players->set_current_item(0);

	lst_players->sort();        
    lst_players->set_tab_id(1);

    btn_ok->find_preferred_size();
    btn_del->find_preferred_size();
    btn_add->find_preferred_size();

    // Connect signals
    slots.connect(btn_ok->sig_clicked(), this, &NS_Profile::on_ok);
    slots.connect(btn_add->sig_clicked(), this, &NS_Profile::on_new_player);
    slots.connect(btn_del->sig_clicked(), this, &NS_Profile::on_delete_player);

    // Add components to component list
    components["btn_ok"]     = btn_ok;
    components["players"]    = lst_players;
    components["btn_add"]    = btn_add;
    components["btn_del"]    = btn_del;
}

bool NS_Profile::open(CL_Component * parent)
{
    NS_Profile dlg(parent);
    dlg.run();
    return dlg.is_success();
}

bool NS_Profile::is_success() const
{
    return success;
}

void NS_Profile::on_ok()
{
    // Select new default user
    // then quit
    quit();
}

NS_Profile::~NS_Profile()
{
	std::map<std::string, CL_Component * >::iterator it(components.begin());
	for(; it != components.end(); ++it)
		delete it->second;
}

// New Player
namespace {

class NS_New_Player : public CL_Window
{
public:
    NS_New_Player(CL_Component * parent, std::string const & player_name = "");
    ~NS_New_Player();
public:
    void on_ok();
    void on_cancel();
    static bool open(CL_Component * parent, std::string & player_name);
    std::string const & get_player_name() const;
    void set_focus_on_input();
    bool is_success() const;
    void on_key_down(CL_InputEvent const & key);
private:
    std::map<std::string, CL_Component * > components;
    CL_SlotContainer slots;
    std::string old_player_name;
    bool success;
};

NS_New_Player::NS_New_Player(CL_Component * parent,
        std::string const & player_name)
:
    CL_Window(parent, parent->get_style_manager()),
    old_player_name(player_name),
    success(false)
{
    CL_Button * btn_cancel = new CL_Button(CL_Point(220, 75), "Cancel", get_client_area());
    CL_Button * btn_ok = new CL_Button(CL_Point(180, 75), "OK", get_client_area());
    CL_InputBox * input_name = new CL_InputBox(CL_Rect(15, 40, 270, 65), get_client_area());

    // Calc dialog position
    set_position(CL_Rect(200, 150, 500, 265));
    set_title("Enter player name");

    // Connect signals
    slots.connect(btn_ok->sig_clicked(), this, &NS_New_Player::on_ok);
    slots.connect(btn_cancel->sig_clicked(), this, &NS_New_Player::on_cancel);
    slots.connect(CL_Keyboard::sig_key_down(), this, &NS_New_Player::on_key_down);

    // Add components to component list
    components["btn_ok"]     = btn_ok;
    components["btn_cancel"] = btn_cancel;
    components["input_name"] = input_name;

	input_name->set_focus();
}

NS_New_Player::~NS_New_Player()
{
	std::map<std::string, CL_Component * >::iterator it(components.begin());
	for(; it != components.end(); ++it)
		delete it->second;
}

void NS_New_Player::on_key_down(CL_InputEvent const & key)
{
    if (key.id == CL_KEY_ESCAPE)
        on_cancel();
    else
        if (key.id == CL_KEY_RETURN)
            on_ok();
}

std::string const & NS_New_Player::get_player_name() const
{
    return old_player_name;
}

bool NS_New_Player::is_success() const
{
    return success;
}

void NS_New_Player::set_focus_on_input()
{
    components["input_name"]->set_focus();
}

bool NS_New_Player::open(CL_Component * parent,
                         std::string & player_name)
{
    NS_New_Player dlg(parent, player_name);
    dlg.set_focus_on_input();
    dlg.run();
    player_name = dlg.get_player_name();
    return dlg.is_success();
}

struct CharRemover : std::unary_function<char, bool>
{
    CharRemover(std::string const & wildcard)
        : wildcard(wildcard)
    {
    }
    bool operator()(char ch)
    {
        return wildcard.find(ch) == std::string::npos;
    }
    std::string const & wildcard;
};

void NS_New_Player::on_ok()
{
    // Select new default user
    // then quit
    CL_InputBox * input_name = dynamic_cast<CL_InputBox*>(components["input_name"]);
    if (input_name)
    {
        std::string str = input_name->get_text();
        size_t pos = str.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890(),_");
        if (pos != std::string::npos)
            str.erase(str.begin(), str.begin() + pos);
        pos = str.find_last_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890(),_");
        if (pos != std::string::npos)
            str.erase(str.begin() + pos + 1, str.end());

        str.erase(std::remove_if(str.begin(), str.end(),
            CharRemover("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890(),_ ")),
            str.end());

        if (!str.empty())
            success = true;
        old_player_name = str;
    }
    quit();
}

void NS_New_Player::on_cancel()
{
    // Select new default user
    // then quit
    quit();
}

} // end New Player namespace

void NS_Profile::on_new_player()
{
    CL_ListBox * lst_players = dynamic_cast<CL_ListBox*>(components["players"]);
    if (lst_players)
    {
        std::string old_player_name;
        if (NS_New_Player::open(get_gui_manager(), old_player_name))
        // add player here
        {
            if (!old_player_name.empty())
			{
				lst_players->insert_item(old_player_name);
				lst_players->set_current_item(lst_players->get_count() - 1);
				lst_players->sort();
			}
        }
    }
}

void NS_Profile::on_delete_player()
{
    CL_ListBox * lst_players = dynamic_cast<CL_ListBox*>(components["players"]);
    if (lst_players)
    {
        if (lst_players->get_count() > 0)
		{
			if (lst_players->get_current_item() != -1)
            {
                int result = NS_MessageBox::info(get_gui_manager(), "Deleting Player",
                    "Are you sure ?\nInformation about all completed levels for player '" + lst_players->get_current_text() + "' will be lost!",
                    "OK", "Cancel", "");
                if (result == 1)
                {
                    // delete player
					lst_players->remove_item(lst_players->get_current_item());
                    if (lst_players->get_count())
                    {
                        lst_players->set_current_item(0);
                    }
                    else
                    {
                        lst_players->set_current_item(-1);
                    }
                }
            }
		}
		else
		{
			NS_MessageBox::info(this, "Deleting Player",
				"You should add some players first",
				"OK", "", "");
		}
    }
}
