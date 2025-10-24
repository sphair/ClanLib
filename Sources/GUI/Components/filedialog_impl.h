/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Kenneth Gangstoe
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_help.h"

/////////////////////////////////////////////////////////////////////////////
// CL_FileDialog_Impl Class:

class CL_FileDialog_Impl
{
public:
	struct Filter
	{
		CL_String description;
		CL_String extension;
	};

	CL_GUIComponent *owner;

	bool multi_select;

	std::vector<CL_String> filenames;

	std::vector<Filter> filters;
	int filter_index;

	CL_String initial_directory;

	CL_String title;

	CL_String get_filename() const
	{
		if (filenames.empty())
		{
			return CL_String();
		}
		return filenames[0];
	}

	void set_filename(const CL_String &filename)
	{
		filenames.clear();
		filenames.push_back(filename);
	}

	void add_filter(const CL_String &filter_description, const CL_String &filter_extension, bool is_default)
	{
		CL_FileDialog_Impl::Filter f;
		f.description = filter_description;
		f.extension = filter_extension;
		filters.push_back(f);

		if(is_default)
			filter_index = filters.size() - 1;
	}

	void clear_filters()
	{
		filters.clear();
		filter_index = -1;
	}

	void set_filter_index(int filter_index)
	{
		if(filter_index >= 0 && filter_index < filters.size())
			filter_index = filter_index;
	}

	CL_String16 get_filter_string() const
	{
		CL_String16 filter;

		for(int i = 0; i < filters.size(); ++i)
		{
			filter += CL_StringHelp::utf8_to_ucs2(filters[i].description);
			filter += (wchar_t) '\0';
			filter += CL_StringHelp::utf8_to_ucs2(filters[i].extension);
			filter += (wchar_t) '\0';
		}
		filter += (wchar_t) '\0';
		filter += (wchar_t) '\0';

		return filter;
	}

	bool show(bool save_mode)
	{
		#ifdef WIN32

		// THIS REALLY SHOULD BE UPDATED TO USE IFileOpenDialog under Vista or later!
		// See CL_BrowseFolderDialog_Impl for usage example.

		OPENFILENAME ofn;

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);

		if(owner)
			ofn.hwndOwner = owner->get_display_window().get_hwnd();
		else
			ofn.hwndOwner = 0;

		CL_String16::char_type filename_buffer[FILENAME_MAX] = { 0 };
		CL_String16 title16 = CL_StringHelp::utf8_to_ucs2(title);
		CL_String16 filter16 = get_filter_string();
		CL_String16 initial_directory16 = CL_StringHelp::utf8_to_ucs2(initial_directory);
		ofn.lpstrFile = filename_buffer;
		ofn.nMaxFile = FILENAME_MAX;

		ofn.lpstrFilter = filter16.c_str();
		ofn.nFilterIndex = filter_index + 1;

		ofn.lpstrFileTitle = 0;
		ofn.nMaxFileTitle = 0;

		ofn.lpstrInitialDir = initial_directory16.c_str();

		ofn.lpstrTitle = title16.c_str();

		ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_EXPLORER;

		if(!save_mode)
		{
			ofn.Flags |= OFN_PATHMUSTEXIST;
			ofn.Flags |= OFN_FILEMUSTEXIST;
		}

		if(multi_select)
			ofn.Flags |= OFN_ALLOWMULTISELECT;

		BOOL success;
		if(save_mode)
			success = GetSaveFileName(&ofn);
		else
			success = GetOpenFileName(&ofn);

		if(success)
		{
			filenames.clear();

			if(multi_select)
			{
/*				CL_String path = CL_String(filename_buffer, ofn.nFileOffset);

				int previous_offset = ofn.nFileOffset;	
				int offset = ofn.nFileOffset;
				bool quit = false;
				while(true)
				{
					if(filename_buffer[offset] == '\0' && filename_buffer[offset + 1] == '\0')
					{
						break;
					}
					if(filename_buffer[offset] == '\0')
					{
						CL_String filename;
						filename.append(filename_buffer, offset);
					}
					offset++;
				}
*/			}

			filenames.push_back(CL_StringHelp::ucs2_to_utf8(filename_buffer));
		}

		return success;
#else
		bool success = false;
		return success;
#endif
	}
};
