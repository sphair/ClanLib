/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/UI/SystemDialogs/folder_browse_dialog.h"
#include "API/UI/View/view.h"
#include "API/UI/TopLevel/view_tree.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/system.h"
#include "API/Display/Window/display_window.h"

#if defined(WIN32)
#include "API/Core/System/comptr.h"
#include <Shlobj.h>
#elif defined(__APPLE__)
// TODO Add Cocoa includes here
#elif defined(I_LOVE_AUTOHELL_AND_FIXED_THE_GTK_CHECK)
#include <gtk/gtk.h>
#endif

namespace clan
{
	class BrowseFolderDialogImpl
	{
	public:
		View *owner;

		std::string selected_path;

		std::string initial_directory;

		std::string title;

		bool show()
		{
#if defined(WIN32)

			std::wstring title16 = StringHelp::utf8_to_ucs2(title);
			std::wstring initial_directory16 = StringHelp::utf8_to_ucs2(initial_directory);

			if (is_vista_or_later())
			{
				HRESULT result;
				ComPtr<IFileOpenDialog> open_dialog;

				result = CoCreateInstance(__uuidof(FileOpenDialog), NULL, CLSCTX_ALL, __uuidof(open_dialog), reinterpret_cast<void**>(open_dialog.output_variable()));
				throw_if_failed(result, "CoCreateInstance(FileOpenDialog) failed");

				result = open_dialog->SetTitle(title16.c_str());
				throw_if_failed(result, "IFileOpenDialog.SetTitle failed");

				// Set the options on the dialog.
				DWORD dwFlags;

				// Before setting, always get the options first in order not to override existing options.
				result = open_dialog->GetOptions(&dwFlags);

				// Remove FOS_PATHMUSTEXIST - it hangs my russian windows 8.1 when last saved current dir is symbolic link to downloads with russian locale.
				// Moreover, according to MSDN, FOS_PATHMUSTEXIST is a default value. For debug email to _mag12@yahoo.com Artem Khomenko
				result = open_dialog->SetOptions(dwFlags | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
				throw_if_failed(result, "IFileOpenDialog.SetOptions((FOS_PICKFOLDERS) failed");

				if (initial_directory16.length() > 0)
				{
					LPITEMIDLIST item_id_list = NULL;
					SFGAOF flags = 0;
					result = SHParseDisplayName(initial_directory16.c_str(), NULL, &item_id_list, SFGAO_FILESYSTEM, &flags);
					throw_if_failed(result, "SHParseDisplayName failed");

					ComPtr<IShellItem> folder_item;
					result = SHCreateShellItem(NULL, NULL, item_id_list, folder_item.output_variable());
					ILFree(item_id_list);
					throw_if_failed(result, "SHCreateItemFromParsingName failed");

					/* This code requires Windows Vista or newer:
					ComPtr<IShellItem> folder_item;
					result = SHCreateItemFromParsingName(initial_directory16.c_str(), NULL, IID_PPV_ARGS(folder_item.output_variable()));
					throw_if_failed(result, "SHCreateItemFromParsingName failed");
					*/

					if (!folder_item.is_null())
					{
						result = open_dialog->SetFolder(folder_item);
						throw_if_failed(result, "IFileOpenDialog.SetFolder failed");
					}
				}

				ViewTree *tree = owner->view_tree();
				if (tree)
					result = open_dialog->Show(tree->display_window().get_handle().hwnd);
				else
					result = open_dialog->Show(0);

				if (SUCCEEDED(result))
				{
					ComPtr<IShellItem> chosen_folder;
					result = open_dialog->GetResult(chosen_folder.output_variable());
					throw_if_failed(result, "IFileOpenDialog.GetResult failed");

					WCHAR *buffer = 0;
					result = chosen_folder->GetDisplayName(SIGDN_FILESYSPATH, &buffer);
					throw_if_failed(result, "IShellItem.GetDisplayName failed");

					std::wstring output_directory16;
					try
					{
						output_directory16 = buffer;
					}
					catch (...)
					{
						CoTaskMemFree(buffer);
						throw;
					}

					CoTaskMemFree(buffer);
					selected_path = StringHelp::ucs2_to_utf8(output_directory16);
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				BROWSEINFO bi;
				ZeroMemory(&bi, sizeof(bi));

				std::wstring::value_type path_buffer[FILENAME_MAX] = { 0 };

				WCHAR Buffer[MAX_PATH];
				memset(Buffer, 0, sizeof(WCHAR) * MAX_PATH);

				ViewTree *tree = owner->view_tree();
				if (tree)
					bi.hwndOwner = tree->display_window().get_handle().hwnd;
				else
					bi.hwndOwner = 0;

				bi.pszDisplayName = Buffer;
				bi.lpszTitle = title16.c_str();
				bi.ulFlags = BIF_EDITBOX | BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_SHAREABLE;

				LPCITEMIDLIST pFolder = SHBrowseForFolder(&bi);
				if (pFolder == NULL)
					return false;

				if (!SHGetPathFromIDList(pFolder, path_buffer))
					throw Exception("Bad path for Browse Folder Dialog");

				selected_path = StringHelp::ucs2_to_utf8(path_buffer);

				return true;
			}

#elif defined(__APPLE__)

			// To do: add cocoa code here
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

	private:
#ifdef WIN32
		void throw_if_failed(HRESULT result, const std::string &error)
		{
			if (FAILED(result))
				throw Exception(error);
		}

		bool is_vista_or_later()
		{
			OSVERSIONINFO info = { 0 };
			info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx(&info);
			return info.dwMajorVersion >= 6;
		}
#endif
	};

	BrowseFolderDialog::BrowseFolderDialog(View *owner) : impl(std::make_shared<BrowseFolderDialogImpl>())
	{
		impl->owner = owner;
	}

	std::string BrowseFolderDialog::selected_path() const
	{
		return impl->selected_path;
	}

	void BrowseFolderDialog::set_initial_directory(const std::string &path)
	{
		impl->initial_directory = path;
	}

	void BrowseFolderDialog::set_title(const std::string &title)
	{
		impl->title = title;
	}

	bool BrowseFolderDialog::show()
	{
		return impl->show();
	}
}
