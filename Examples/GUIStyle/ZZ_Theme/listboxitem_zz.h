#ifndef HEADER_LIST_BOX_ITEM_ZZ_H
#define HEADER_LIST_BOX_ITEM_ZZ_H

#pragma once

#include <ClanLib/gui.h>
#include <string>

class CL_ListBoxItem_ZZ : public CL_ListItem
{
public:
	CL_ListBoxItem_ZZ(std::string const & levelname, bool solved)
		: solved(solved)
	{
		str = levelname;
	}
	virtual ~CL_ListBoxItem_ZZ(){};

public:
	bool get_solved() const
	{
		return solved;
	}

	std::string const & get_levelname() const
	{
		return str;
	}

protected:
	virtual bool less_than(const CL_ListItem & item) const
	{
		// Assume that listbox contain items only this type
		CL_ListBoxItem_ZZ const * other = (CL_ListBoxItem_ZZ const *)&item;

		if (get_solved() == other->get_solved())
			return get_levelname() < other->get_levelname();
		
		return get_solved() == true;
	}
	virtual bool equal(const CL_ListItem & item) const
	{
		// Assume that listbox contain items only this type
		CL_ListBoxItem_ZZ const * other = (CL_ListBoxItem_ZZ const *)&item;

		return get_solved() == other->get_solved()
			&& get_levelname() == other->get_levelname();
	}

private:
	bool solved;
};

#endif // HEADER_LIST_BOX_ITEM_ZZ_H
