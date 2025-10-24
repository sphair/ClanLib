
#pragma once

class MainFrame;
class Document;

class View : public CL_GUIComponent
{
public:
	View(CL_GUIComponent *parent, MainFrame *mainframe, const CL_String &title);
	virtual ~View();

	MainFrame *get_mainframe();
	Document *get_document();
	const CL_String &get_title() const { return title; }

	void set_title(const CL_String &new_title) { title = new_title; }

private:
	MainFrame *mainframe;
	CL_String title;
};
