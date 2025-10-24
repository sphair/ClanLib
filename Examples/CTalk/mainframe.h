
#ifndef file_mainframe
#define file_mainframe

#include <vector>

class Document;
class View;
class IRCConnection;
class CL_Button;

class MainFrame : public CL_Frame
{
//! Construction:
public:
	MainFrame(CL_Component *parent, CL_ResourceManager *resources);

	~MainFrame();

//! Attributes:
public:
	Document *get_document();

	int get_num_views();

	View *get_view(int index);

	View *get_active_view();

	CL_Rect get_view_area();

	CL_ResourceManager *get_resources();

//! Operations:
public:
	void set_active_view(View *view);

	void set_active_view(int index);

	void add_view(View *view);

	void remove_view(View *view);

	int find_view_index(View *view);

//! Implementation:
private:
	void arrange_buttons();

	void on_paint();

	void on_view_button_clicked(View *view);

	void on_connection_join(const std::string &nick, const std::string &channel, IRCConnection *connection);

	CL_ResourceManager *resources;

	CL_Frame *view_area;

	Document *document;

	std::vector< std::pair<CL_Button *, View *> > views;

	int active_view;

	CL_SlotContainer slots;
};

#endif
