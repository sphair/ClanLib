
#pragma once

#include "../FileItemType/file_item_type_factory.h"

class DlgAddNewItem : public clan::GUIComponent
{
public:
	DlgAddNewItem(clan::GUIComponent *owner, FileItemTypeFactory &factory, const std::string &default_location);

	std::string get_name() const;
	std::string get_location() const;
	FileItemType *get_fileitemtype() const;

private:
	void on_resized();
	bool on_close();
	void on_button_ok_clicked();
	void on_button_cancel_clicked();
	void on_button_browse_location_clicked();
	void on_list_items_selection_changed(clan::ListViewSelection item);

	void populate(FileItemTypeFactory &factory);

	static clan::GUITopLevelDescription get_toplevel_description();

	clan::Label *label_name;
	clan::LineEdit *lineedit_name;

	clan::Label *label_location;
	clan::LineEdit *lineedit_location;
	clan::PushButton *button_browse_location;
	
	clan::ListView *list_items;
	clan::Label *label_description_header;
	clan::Label *label_description;

	clan::PushButton *button_ok;
	clan::PushButton *button_cancel;

	class ListViewItemData : public clan::ListViewItemUserData
	{
	public:
		ListViewItemData(FileItemType *type) : type(type) { }
		FileItemType *type;
	};
	typedef std::shared_ptr<ListViewItemData> ListViewItemDataPtr;
};
