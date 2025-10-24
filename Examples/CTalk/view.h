
#pragma once

class MainFrame;
class Document;

class View : public CL_GUIComponent
{
//! Construction:
public:
	View(CL_GUIComponent *parent, MainFrame *mainframe, const CL_String &title);
	virtual ~View();

//! Attributes:
public:
	MainFrame *get_mainframe();
	Document *get_document();
	const CL_String &get_title() const { return title; }

//! Operations:
public:
	void set_title(const CL_String &new_title) { title = new_title; }

//! Implementation:
private:
	MainFrame *mainframe;
	CL_String title;
};
