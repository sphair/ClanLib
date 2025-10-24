// NS_Profile.h: interface for the NS_Profile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NS_PROFILE_H__EDFB8C32_158A_4CC6_805E_F8F99EA8E0BF__INCLUDED_)
#define AFX_NS_PROFILE_H__EDFB8C32_158A_4CC6_805E_F8F99EA8E0BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ClanLib/gui.h>
#include <ClanLib/gui.h>
#include <ClanLib/signals.h>
#include <ClanLib/signals.h>

#include <map>
#include <string>

class NS_Profile : public CL_Window
{
public:
    NS_Profile(CL_Component * parent);
    ~NS_Profile();
public:
    void on_ok();
    static bool open(CL_Component * parent);
    void on_sig_activated(int index);
    void on_new_player();
    void on_delete_player();
    bool is_success() const;
private:
    std::map<std::string, CL_Component * > components;
    bool success;
    CL_SlotContainer slots;
};

#endif // !defined(AFX_NS_PROFILE_H__EDFB8C32_158A_4CC6_805E_F8F99EA8E0BF__INCLUDED_)
