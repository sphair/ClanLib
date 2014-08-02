
#include "precomp.h"
#include "css_browser.h"
#include "css_view.h"

using namespace clan;

CSSBrowser::CSSBrowser(GUIManager *gui)
: GUIComponent/*Window*/(gui, get_window_description()), label(0), combobox(0), view(0)
{
	view = new CSSView(this);
	func_close() = bind_member(this, &CSSBrowser::on_close);
	func_resized() = bind_member(this, &CSSBrowser::on_resized);
	label = new Label(this);
	combobox = new ComboBox(this);
	combobox->func_item_selected() = bind_member(this, &CSSBrowser::on_combobox_item_selected);
	combobox->func_enter_pressed() = bind_member(this, &CSSBrowser::on_combobox_enter_pressed);
	PopupMenu popup;
	popup.insert_item("http://clanlib.org/");
	popup.insert_item("http://clanlib.org/documentation.html");
	popup.insert_item("http://www.rtsoft.com/forums/forumdisplay.php?13-Official-ClanLib-Game-SDK-Forums");
	popup.insert_item("http://www.csszengarden.com/");
	//for (int i = 130; i < 214; i++)
	//	popup.insert_item(string_format("http://www.csszengarden.com/?cssfile=/%1/%2.css&page=0", i, i));
	popup.insert_item("http://www.csszengarden.com/?cssfile=/105/105.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/109/109.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/110/110.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/111/111.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/125/125.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/162/162.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/199/199.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/200/200.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/201/201.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/202/202.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/203/203.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/204/204.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/205/205.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/206/206.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/207/207.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/208/208.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/209/209.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/210/210.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/211/211.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/212/212.css&page=0");
	popup.insert_item("http://www.csszengarden.com/?cssfile=/213/213.css&page=0");
	popup.insert_item("http://www.w3.org/Style/CSS/Test/CSS1/current/test5526c.htm");
	popup.insert_item("http://www.webstandards.org/files/acid2/test.html");
	popup.insert_item("http://esoteric.clanlib.org/~mbn/temp/test.html");
	popup.insert_item("http://somafm.com/");
	popup.insert_item("http://en.wikipedia.org/wiki/ClanLib");
	popup.insert_item("http://www.dr.dk/nyheder/");
	popup.insert_item("http://politiken.dk/");
	popup.insert_item("http://nyhederne.tv2.dk/");
	popup.insert_item("http://www.bt.no/");
	popup.insert_item("http://www.bbc.co.uk/news/");
	popup.insert_item("http://www.slashdot.org/");
	popup.insert_item("http://www.osnews.com/");
	popup.insert_item("http://www.todopad.org/");
	combobox->set_editable(true);
	combobox->set_popup_menu(popup);
	label->set_text("Address:");
	//load_css_layout("Resources/Theme/css_browser.xml", "Resources/Theme/css_browser.css");
	on_resized();
}

GUITopLevelDescription CSSBrowser::get_window_description()
{
	GUITopLevelDescription desc;
	desc.set_size(Size(1280, 865), false);
	desc.set_title("Carambola Browser");
	desc.set_allow_resize(true);
	return desc;
}

bool CSSBrowser::on_close()
{
	exit_with_code(0);
	return true;
}

void CSSBrowser::on_resized()
{
	label->set_geometry(Rect(11, 11, 200, 31));
	combobox->set_geometry(Rect(200, 11, 1200, 31));
	view->set_geometry(Rect(0, 38, 1200, 800));
}

void CSSBrowser::on_combobox_item_selected(int index)
{
	view->navigate(combobox->get_text());
}

void CSSBrowser::on_combobox_enter_pressed()
{
	view->navigate(combobox->get_text());
}
