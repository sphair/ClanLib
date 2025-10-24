/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    Seth A. Robinson
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/System/clipboard.h"
#include "API/Core/System/log.h"

#ifdef __APPLE__
#include <Carbon/carbon.h>
#endif
/////////////////////////////////////////////////////////////////////////////
// CL_Clipboard construction:


/////////////////////////////////////////////////////////////////////////////
// CL_Clipboard attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_Clipboard operations:

std::string CL_Clipboard::get_text()
{

	std::string text;

#ifdef WIN32

	if (OpenClipboard(NULL))
	{

		HANDLE hData = GetClipboardData(CF_TEXT);  //note, if we every support unicode we would use CF_UNICODE
		if (hData)
		{
			text = (char*)GlobalLock(hData);
			GlobalUnlock(hData);
		}

		CloseClipboard();
	}

#elif defined __APPLE__

	ScrapRef scrap = 0;
	Size len = 0;

	if (GetCurrentScrap(&scrap) == noErr && GetScrapFlavorSize(scrap, kScrapFlavorTypeText, &len) == noErr)
	{
			char *pBuffer = new char[len+1];
			pBuffer[len] = 0; //just in case they don't have their own null
			GetScrapFlavorData( scrap, kScrapFlavorTypeText, &len, pBuffer);

			//note, we might want to convert /r's to /n's here or something later

			text = pBuffer;
	}

#endif
	
	return text; //not handled for this platform, add it please!
}

void CL_Clipboard::set_text(const std::string &text)
{
#ifdef WIN32

	if (OpenClipboard(NULL))
	{
		EmptyClipboard();

		char * buffer;
		HGLOBAL clipbuffer;
	    clipbuffer = GlobalAlloc(GMEM_DDESHARE, text.size()+1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, text.c_str());
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}

#elif defined __APPLE__

	ScrapRef myScrap = 0;
	ClearCurrentScrap(); //remove what is there
	GetCurrentScrap(&myScrap);
	PutScrapFlavor( myScrap, kScrapFlavorTypeText, 0, text.size(), text.c_str() );
#endif

}

/////////////////////////////////////////////////////////////////////////////
// CL_Clipboard implementation:
