/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "UI/precomp.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/System/system.h"
#include "API/UI/View/view.h"
#include "API/UI/TopLevel/view_tree.h"
#include "API/Display/Window/display_window.h"

#if defined(WIN32)

#elif !defined(__APPLE__) && defined(I_LOVE_AUTOHELL_AND_FIXED_THE_GTK_CHECK)
#include <gtk/gtk.h>
#endif

namespace clan
{
	class FileDialogImpl
	{
	public:
		struct Filter
		{
			std::string description;
			std::string extension;
		};

		View *owner;

		bool multi_select;

		std::vector<std::string> filenames;

		std::vector<Filter> filters;
		int filter_index;

		std::string initial_directory;

		std::string title;

		std::string get_filename() const
		{
			if (filenames.empty())
			{
				return std::string();
			}
			return filenames[0];
		}

		void set_filename(const std::string &filename)
		{
			filenames.clear();
			filenames.push_back(filename);
		}

		void add_filter(const std::string &filter_description, const std::string &filter_extension, bool is_default)
		{
			FileDialogImpl::Filter f;
			f.description = filter_description;
			f.extension = filter_extension;
			filters.push_back(f);

			if (is_default)
				filter_index = filters.size() - 1;
		}

		void clear_filters()
		{
			filters.clear();
			filter_index = -1;
		}

		void set_filter_index(int new_filter_index)
		{
			if (new_filter_index >= 0 && new_filter_index < filters.size())
				filter_index = new_filter_index;
		}

		std::wstring get_filter_string() const
		{
			std::wstring filter;

			for (auto & elem : filters)
			{
				filter += StringHelp::utf8_to_ucs2(elem.description);
				filter += (wchar_t) '\0';
				filter += StringHelp::utf8_to_ucs2(elem.extension);
				filter += (wchar_t) '\0';
			}
			filter += (wchar_t) '\0';
			filter += (wchar_t) '\0';

			return filter;
		}

		bool show(bool save_mode)
		{
#if defined(WIN32)
			// THIS REALLY SHOULD BE UPDATED TO USE IFileOpenDialog under Vista or later!
			// See BrowseFolderDialog_Impl for usage example.

			OPENFILENAME ofn;

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);

			ViewTree *tree = owner->view_tree();
			if (tree)
                ofn.hwndOwner = tree->get_display_window().get_handle().hwnd;
			else
				ofn.hwndOwner = 0;

			DataBuffer buffer(64 * 1024 * sizeof(std::wstring::value_type));
			std::wstring::value_type *filename_buffer = (std::wstring::value_type *)buffer.get_data();
			std::wstring title16 = StringHelp::utf8_to_ucs2(title);
			std::wstring filter16 = get_filter_string();
			std::wstring initial_directory16 = StringHelp::utf8_to_ucs2(initial_directory);
			ofn.lpstrFile = filename_buffer;
			ofn.nMaxFile = 64 * 1024;

			ofn.lpstrFilter = filter16.c_str();
			ofn.nFilterIndex = filter_index + 1;

			ofn.lpstrFileTitle = 0;
			ofn.nMaxFileTitle = 0;

			ofn.lpstrInitialDir = initial_directory16.c_str();

			ofn.lpstrTitle = title16.c_str();

			ofn.Flags = OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_EXPLORER;

			if (!save_mode)
			{
				ofn.Flags |= OFN_PATHMUSTEXIST;
				ofn.Flags |= OFN_FILEMUSTEXIST;
			}

			if (multi_select)
				ofn.Flags |= OFN_ALLOWMULTISELECT;

			BOOL success;
			if (save_mode)
				success = GetSaveFileName(&ofn);
			else
				success = GetOpenFileName(&ofn);

			if (success)
			{
				filenames.clear();

				if (multi_select)
				{
					std::string path = StringHelp::ucs2_to_utf8(std::wstring(filename_buffer, ofn.nFileOffset - 1));
					int offset = ofn.nFileOffset;
					while (true)
					{
						std::string filename = StringHelp::ucs2_to_utf8(filename_buffer + offset);
						offset += filename.length() + 1;
						if (filename.empty())
							break;
						filenames.push_back(PathHelp::combine(path, filename));
					}
				}
				else
				{
					filenames.push_back(StringHelp::ucs2_to_utf8(filename_buffer));
				}
			}

			return success;
#elif defined(__APPLE__)

			return false;

#elif defined(I_LOVE_AUTOHELL_AND_FIXED_THE_GTK_CHECK)
			if (!gtk_init_check(NULL, NULL))
				throw Exception("gtk_init_check Failed!");

			GtkWidget *dialog;

			dialog = gtk_file_chooser_dialog_new ("Open File", 
				NULL,
				GTK_FILE_CHOOSER_ACTION_OPEN,
				GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				NULL);

			if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
			{
				char *filename;

				filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
				//printf("%s\n", filename);
				g_free (filename);
			}

			gtk_widget_destroy (dialog);
			unsigned int x_time = System::get_time();
			while(true)
			{
				if(owner)
					XSync(owner->get_display_window().get_display(), True);
				gtk_main_iteration_do(FALSE);
				unsigned int x_time_now = System::get_time();
				if ((x_time_now - x_time) > 250)
					break;
			}

			bool success = false;
			return success;
#else
			return false;
#endif
		}
	};
}
