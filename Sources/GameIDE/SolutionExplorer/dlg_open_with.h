
#pragma once

class DocumentEditorType;

class DlgOpenWith : public clan::GUIComponent
{
public:
	DlgOpenWith(clan::GUIComponent *owner, std::vector<DocumentEditorType *> editors);
	DocumentEditorType *get_editor_type();

private:
	void on_resized();
	bool on_close();
	void on_button_ok_clicked();
	void on_button_cancel_clicked();
	void on_item_doubleclick(const clan::ListViewItem &item);

	void populate(std::vector<DocumentEditorType *> editors);

	static clan::GUITopLevelDescription get_toplevel_description();

	clan::ListView *list_items;

	clan::PushButton *button_ok;
	clan::PushButton *button_cancel;

	class ListViewItemData : public clan::ListViewItemUserData
	{
	public:
		ListViewItemData(DocumentEditorType *type) : type(type) { }
		DocumentEditorType *type;
	};
	typedef std::shared_ptr<ListViewItemData> ListViewItemDataPtr;
};
