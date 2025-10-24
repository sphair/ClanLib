
#ifndef file_view
#define file_view

class MainFrame;
class Document;

class View : public CL_Component
{
//! Construction:
public:
	View(MainFrame *mainframe, const std::string &title);

	virtual ~View();

//! Attributes:
public:
	MainFrame *get_mainframe();

	Document *get_document();

	const std::string &get_title() const { return title; }

//! Operations:
public:
	void set_title(const std::string &new_title) { title = new_title; }

//! Implementation:
private:
	MainFrame *mainframe;

	std::string title;
};

#endif
