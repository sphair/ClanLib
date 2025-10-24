
#include "precomp.h"
#include "listview_ex_document.h"
#include "listview_ex.h"

ListviewExDocument::ListviewExDocument()
: focus_item(-1), hot_item(-1), insert_mark_index(-1), first_selected_item(-1), last_selected_item(-1), num_selected_items(0)
{
	columns.resize(1);
	columns[0].size = 200;
}

ListviewExDocument::~ListviewExDocument()
{
	remove_all_items();
}

int ListviewExDocument::get_column_count() const
{
	return columns.size();
}

int ListviewExDocument::get_column_size(int index) const
{
	return columns[index].size;
}

CL_String ListviewExDocument::get_column_text(int index) const
{
	return columns[index].text;
}

void ListviewExDocument::insert_column(int index, const CL_StringRef &text, int size)
{
	if (columns.size() <= (size_t)index)
		columns.resize(index+1);
	columns[index].text = text;
	columns[index].size = size;
}

ListviewExItem *ListviewExDocument::get_item(int index) const
{
	return items[index].data;
}

int ListviewExDocument::get_item_icon(int index) const
{
	return items[index].icon_index;
}

unsigned int ListviewExDocument::get_item_flags(int index) const
{
	return items[index].flags;
}

CL_StringRef ListviewExDocument::get_item_text(int index, int column) const
{
	return columns[column].item_texts[index];
}

int ListviewExDocument::find_item(ListviewExItem *item)
{
	for (std::vector<ListviewExItem *>::size_type index = 0; index < items.size(); index++)
	{
		if (items[index].data == item)
			return (int) index;
	}
	return -1;
}

int ListviewExDocument::insert_item(int item_index, ListviewExItem *item, const CL_StringRef &text, int group_id)
{
	if (item_index < 0 || (size_t)item_index >= items.size())
	{
		return append_item(item, text, group_id);
	}
	else
	{
		ItemData data;
		data.data = item;
		items.insert(items.begin()+item_index, data);
		for (std::vector<ColumnData>::size_type index = 0; index < columns.size(); index++)
		{
			if (index == 0)
				columns[index].item_texts.insert(columns[index].item_texts.begin()+item_index, text);
			else
				columns[index].item_texts.insert(columns[index].item_texts.begin()+item_index, CL_String());
		}
		return item_index;
	}
}

int ListviewExDocument::append_item(ListviewExItem *item, const CL_StringRef &text, int group_id /*= -1*/)
{
	int item_index = items.size();
	ItemData data;
	data.data = item;
	items.push_back(data);
	for (std::vector<ColumnData>::size_type index = 0; index < columns.size(); index++)
	{
		if (index == 0)
			columns[index].item_texts.push_back(text);
		else
			columns[index].item_texts.push_back(CL_String());
	}
	return item_index;
}

void ListviewExDocument::set_item_text(int item_index, int column, const CL_StringRef &text)
{
	columns[column].item_texts[item_index] = text;
}

void ListviewExDocument::set_item_icon(int item_index, int icon_index)
{
	items[item_index].icon_index = icon_index;
}

void ListviewExDocument::set_item_album(int item_index, bool is_album)
{
	set_item_flag(item_index, album_flag, is_album);
}

void ListviewExDocument::set_item_flag(int item_index, unsigned int flag, bool enabled)
{
	if (enabled)
		items[item_index].flags |= flag;
	else
		items[item_index].flags &= ~flag;
}

void ListviewExDocument::set_item_drop_highlighted(int item_index, bool enabled)
{
	set_item_flag(item_index, drophighlighted_flag, enabled);
}

void ListviewExDocument::remove_all_items()
{
	if (focus_item != -1 && !cb_item_focus_lost.is_null())
		cb_item_focus_lost.invoke(focus_item);

	for (std::vector<ListviewExItem *>::size_type index = 0; index < items.size(); index++)
		delete items[index].data;
	items.clear();
	for (std::vector<ColumnData>::size_type index = 0; index < columns.size(); index++)
		columns[index].item_texts.clear();
	num_selected_items = 0;
	first_selected_item = -1;
	last_selected_item = -1;
	focus_item = -1;
	hot_item = -1;
}

void ListviewExDocument::remove_item(int item_index)
{
	bool was_focused = false;
	if (focus_item == item_index)
	{
		if (focus_item != -1 && !cb_item_focus_lost.is_null())
			cb_item_focus_lost.invoke(focus_item);
		was_focused = true;
	}

	if (items[item_index].flags & selected_flag)
	{
		if (first_selected_item == item_index)
			first_selected_item = get_next_selected_item(first_selected_item);
		if (last_selected_item == item_index)
			last_selected_item = get_prev_selected_item(last_selected_item);
		num_selected_items--;
	}

	delete items[item_index].data;
	items.erase(items.begin() + item_index);
	for (std::vector<ColumnData>::size_type index = 0; index < columns.size(); index++)
		columns[index].item_texts.erase(columns[index].item_texts.begin() + item_index);

	if (first_selected_item > item_index)
		first_selected_item--;
	if (last_selected_item > item_index)
		last_selected_item--;

	if ((size_t)focus_item >= items.size())
	{
		if (items.empty())
			focus_item = -1;
		else
			focus_item = items.size()-1;
	}

	if (was_focused)
	{
		if (focus_item != -1 && !cb_item_focus_gained.is_null())
			cb_item_focus_gained.invoke(focus_item);
	}
}

int ListviewExDocument::get_next_selected_item(int last_index) const
{
	if (last_index == -1)
	{
		return first_selected_item;
	}
	else
	{
		if (last_selected_item == -1)
			return -1;

		for (int index = last_index+1; index <= last_selected_item; index++)
		{
			if (items[index].flags & selected_flag)
				return index;
		}
		return -1;
	}
}

int ListviewExDocument::get_prev_selected_item(int last_index) const
{
	if (last_index == -1)
	{
		return last_selected_item;
	}
	else
	{
		if (first_selected_item == -1)
			return -1;

		for (int index = last_index; index > first_selected_item; index--)
		{
			if (items[index-1].flags & selected_flag)
				return index-1;
		}
		return -1;
	}
}

int ListviewExDocument::get_selected_count() const
{
	return num_selected_items;
}

void ListviewExDocument::select_item(int index)
{
	select_item_range(index, index);
}

void ListviewExDocument::select_item_range(int first_index, int last_index)
{
	for (int index = first_index; index <= last_index; index++)
	{
		if ((items[index].flags & selected_flag) == 0)
		{
			items[index].flags |= selected_flag;
			num_selected_items++;
		}
	}
	if (first_index < first_selected_item || first_selected_item == -1)
		first_selected_item = first_index;
	if (last_index > last_selected_item || last_selected_item == -1)
		last_selected_item = last_index;
}

void ListviewExDocument::clear_selection()
{
	if (num_selected_items > 0)
	{
		for (int index = first_selected_item; index <= last_selected_item; index++)
		{
			items[index].flags &= ~selected_flag;
		}
		first_selected_item = -1;
		last_selected_item = -1;
		num_selected_items = 0;
	}
}

void ListviewExDocument::set_insert_mark(int item_index)
{
	insert_mark_index = item_index;
}

int ListviewExDocument::add_icon(const CL_Image &pixelbuffer)
{
	int index = (int) icons.size();
	icons.push_back(pixelbuffer);
	return index;
}

void ListviewExDocument::remove_all_icons()
{
	icons.clear();
}

const CL_Image &ListviewExDocument::get_icon(int index) const
{
	return icons[index];
}
