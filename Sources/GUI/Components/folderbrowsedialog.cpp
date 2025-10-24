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
#include "API/GUI/Components/folderbrowsedialog.h"
#include "API/Core/Text/string_help.h"

#ifdef WIN32
#include "API/Core/System/comptr.h"
#include <Shlobj.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseFolderDialog_Impl Class:

class CL_BrowseFolderDialog_Impl
{
public:
	CL_GUIComponent *owner;

	CL_String selected_path;

	CL_String initial_directory;

	CL_String title;
	
	bool show()
	{
		#ifdef WIN32

		CL_String16 title16 = CL_StringHelp::utf8_to_ucs2(title);
		CL_String16 initial_directory16 = CL_StringHelp::utf8_to_ucs2(initial_directory);

		if(is_vista_or_later())
		{
			HRESULT result;
			CL_ComPtr<IFileOpenDialog> open_dialog;

			result = CoCreateInstance(__uuidof(FileOpenDialog), NULL, CLSCTX_ALL, __uuidof(open_dialog),  reinterpret_cast<void**>(open_dialog.output_variable()));
			throw_if_failed(result, "CoCreateInstance(FileOpenDialog) failed");

			result = open_dialog->SetTitle(title16.c_str());
			throw_if_failed(result, "IFileOpenDialog.SetTitle failed");

			result = open_dialog->SetOptions(FOS_PICKFOLDERS);
			throw_if_failed(result, "IFileOpenDialog.SetOptions((FOS_PICKFOLDERS) failed");

			if(initial_directory16.length() > 0)
			{
				CL_ComPtr<IShellItem> folder_item;
				result = SHCreateItemFromParsingName(initial_directory16.c_str(), NULL, IID_PPV_ARGS(folder_item.output_variable()));
				throw_if_failed(result, "SHCreateItemFromParsingName failed");

				if(!folder_item.is_null())
				{
					result = open_dialog->SetFolder(folder_item);
					throw_if_failed(result, "IFileOpenDialog.SetFolder failed");
				}
			}

			if(owner)
				result = open_dialog->Show(owner->get_display_window().get_hwnd());
			else
				result = open_dialog->Show(0);

			if(SUCCEEDED(result))
			{
				CL_ComPtr<IShellItem> chosen_folder;
				result = open_dialog->GetResult(chosen_folder.output_variable());
				throw_if_failed(result, "IFileOpenDialog.GetResult failed");

				WCHAR *buffer = 0;
				result = chosen_folder->GetDisplayName(SIGDN_FILESYSPATH, &buffer);
				throw_if_failed(result, "IShellItem.GetDisplayName failed");

				CL_String16 output_directory16;
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
				selected_path = CL_StringHelp::ucs2_to_utf8(output_directory16);
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
		
			CL_String16::char_type path_buffer[FILENAME_MAX] = { 0 };

			TCHAR Buffer[MAX_PATH];
			ZeroMemory(Buffer, MAX_PATH);

			if(owner)
				bi.hwndOwner = owner->get_display_window().get_hwnd();
			else
				bi.hwndOwner = 0;

			bi.pszDisplayName = Buffer;
			bi.lpszTitle = title16.c_str();
			bi.ulFlags = BIF_EDITBOX | BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_SHAREABLE;

			LPCITEMIDLIST pFolder = SHBrowseForFolder(&bi);
			if (pFolder == NULL) 
				return false;

			if (!SHGetPathFromIDList(pFolder, path_buffer)) 
				throw CL_Exception("Bad path for Browse Folder Dialog");

			selected_path = CL_StringHelp::ucs2_to_utf8(path_buffer);

			return true;
		}
#else
		bool success = false;
		return success;
#endif
	}

private:
	#ifdef WIN32
	void throw_if_failed(HRESULT result, const CL_String &error)
	{
		if (FAILED(result))
			throw CL_Exception(error);
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

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseFolderDialog Construction:

CL_BrowseFolderDialog::CL_BrowseFolderDialog(CL_GUIComponent *owner)
: impl(new CL_BrowseFolderDialog_Impl)
{
	impl->owner = owner;
}

CL_BrowseFolderDialog::~CL_BrowseFolderDialog()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenFileDialog Attributes:

CL_String CL_BrowseFolderDialog::get_selected_path() const
{
	return impl->selected_path;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenFileDialog Operations:

void CL_BrowseFolderDialog::set_initial_directory(const CL_String &path)
{
	impl->initial_directory = path;
}

void CL_BrowseFolderDialog::set_title(const CL_String &title)
{
	impl->title = title;
}

bool CL_BrowseFolderDialog::show()
{
	return impl->show();
}
