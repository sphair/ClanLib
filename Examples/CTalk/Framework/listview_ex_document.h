
#pragma once

class ListviewExItem;

class ListviewExDocument
{
public:
	ListviewExDocument();
	~ListviewExDocument();

	int get_column_count() const;
	int get_column_size(int index) const;
	CL_String get_column_text(int index) const;
	void insert_column(int index, const CL_StringRef &text, int size);

	int get_item_count() const { return items.size(); }
	ListviewExItem *get_item(int index) const;
	int get_item_icon(int index) const;
	unsigned int get_item_flags(int index) const ;
	CL_StringRef get_item_text(int index, int column) const;

	int find_item(ListviewExItem *item);
	int insert_item(int index, ListviewExItem *item, const CL_StringRef &text, int group_id = -1);
	int append_item(ListviewExItem *item, const CL_StringRef &text, int group_id = -1);
	void set_item_text(int item_index, int column, const CL_StringRef &text);
	void set_item_icon(int item_index, int icon_index);
	void set_item_album(int item_index, bool is_album);
	void set_item_drop_highlighted(int item_index, bool enabled);
	void remove_all_items();
	void remove_item(int item_index);

	int get_next_selected_item(int last_index = -1) const;
	int get_prev_selected_item(int last_index = -1) const;
	int get_selected_count() const;

	void select_item(int index);
	void select_item_range(int first_index, int last_index);
	void clear_selection();

	void set_insert_mark(int item_index);

	int add_icon(const CL_Image &pixelbuffer);
	void remove_all_icons();
	const CL_Image &get_icon(int index) const;

	CL_Callback_v1<int> cb_item_focus_gained;                           // void on_item_focus_gained(index)
	CL_Callback_v1<int> cb_item_focus_lost;                             // void on_item_focus_lost(index)

	enum ItemFlags
	{
		selected_flag        = 0x1,
		drophighlighted_flag = 0x2,
		album_flag           = 0x4
	};

private:
	struct ColumnData
	{
		ColumnData() : size(0) { }
		CL_String text;
		int size;
		std::vector<CL_String> item_texts;
	};

	struct ItemData
	{
		ItemData() : icon_index(-1), data(0), flags(0) { }
		int icon_index;
		ListviewExItem *data;
		unsigned int flags;
	};

	void set_item_flag(int index, unsigned int flag, bool state);
	ItemData &get_item_data(int index) { return items[index]; }
	const ItemData &get_item_data(int index) const { return items[index]; }

	std::vector<ColumnData> columns;
	std::vector<ItemData> items;
	std::vector<CL_Image> icons;

	int first_selected_item;
	int last_selected_item;
	int num_selected_items;
	int focus_item;
	int hot_item;
	int insert_mark_index;
};
