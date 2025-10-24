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
**    Mark Page
*/
#include "precomp.h"
#include "gui.h"

#include <wmsdk.h>
#pragma comment(lib, "wmvcore.lib")
#pragma comment(lib, "winmm.lib")

int App::start(const std::vector<CL_String> &args)
{
//	gui = CL_GUIManager("GUIThemeBasicPacked");
	gui = CL_GUIManager("../../Resources/GUIThemeAeroPacked");

	update_window_worker_thread_started = false;

	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("ClanGrabber - Using ClanLib SDK");
	win_desc.set_size(CL_Size(360, 450), false);
	window = new CL_Window(&gui, win_desc);
	window->func_close().set(this, &App::on_close, window);

	button_1 = new CL_PushButton(window);
	button_1->set_geometry(CL_Rect(10, 10, CL_Size(320, 20)));
	button_1->func_clicked().set(this, &App::on_button_clicked, button_1);
	button_1->set_text("Start writing to ScreenCapture.wmv");

	button_2 = new CL_PushButton(window);
	button_2->set_geometry(CL_Rect(10, 40, CL_Size(320, 20)));
	button_2->func_clicked().set(this, &App::on_button_clicked2, button_2);
	button_2->set_text("Stop writing to ScreenCapture.wmv");
	button_2->set_enabled(false);

	CL_PopupMenu combo_menu_size;
	combo_menu_size.insert_item("800 by 600 pixels");
	combo_menu_size.insert_item("1024 by 768 pixels");
	combo_menu_size.insert_item("1280 by 960 pixels");
	combo_menu_size.insert_item("1280 by 1024 pixels");
	combo_menu_size.insert_item("1440 by 900 pixels");
	combo_menu_size.insert_item("1600 by 900 pixels");
	combo_menu_size.insert_item("1680 by 1050 pixels");
	combo_menu_size.insert_item("1920 by 1080 pixels");
	
	CL_ComboBox *combo = new CL_ComboBox(window);
	combo->set_geometry(CL_Rect(10, 80, CL_Size(140, 21)));
	combo->set_editable(false);
	combo->set_dropdown_height(128);
	combo->set_dropdown_minimum_width(64);
	combo->set_popup_menu(combo_menu_size);
	combo->set_selected_item(1);	// 1024x768
	selected_output_width = 1024;
	selected_output_height = 768;
	combo->func_item_selected().set(this, &App::on_combo_selected_size, combo);

	CL_Label *label = new CL_Label(window);
	CL_Rect combo_geometry = combo->get_geometry();
	label->set_geometry(CL_Rect(combo_geometry.right + 5, combo_geometry.top, CL_Size(256, 17)));
	label->set_text("Output Size");

	CL_PopupMenu combo_menu_compression;
	combo_menu_compression.insert_item("WMV3");
	combo_menu_compression.insert_item("MSS2");

	combo = new CL_ComboBox(window);
	combo->set_geometry(CL_Rect(10, 120, CL_Size(140, 21)));
	combo->set_editable(false);
	combo->set_dropdown_height(128);
	combo->set_dropdown_minimum_width(64);
	combo->set_popup_menu(combo_menu_compression);
	combo->set_selected_item(0);	// WMV3
	use_WMV3_compression = true;
	combo->func_item_selected().set(this, &App::on_combo_selected_compression, combo);

	label = new CL_Label(window);
	combo_geometry = combo->get_geometry();
	label->set_geometry(CL_Rect(combo_geometry.right + 5, combo_geometry.top, CL_Size(256, 17)));
	label->set_text("Compression");

	CL_CheckBox *checkbox = new CL_CheckBox(window);
	checkbox->set_geometry(CL_Rect(10, 160 , CL_Size(240, 16)));
	checkbox->set_text("Resize to fit (whilst maintaining aspect ratio)");
	checkbox->set_checked(false);
	is_resize_to_fit = false;
	checkbox->func_state_changed().set(this, &App::on_checkbox_changed_stretch, checkbox);

	lineedit_left = new CL_LineEdit(window);
	lineedit_left->set_geometry(CL_Rect(10, 200, CL_Size(100, 20)));
	lineedit_left->set_text("0");
	lineedit_left->set_numeric_mode(true);
	label = new CL_Label(window);
	CL_Rect geometry = lineedit_left->get_geometry();
	label->set_geometry(CL_Rect(geometry.right + 5, geometry.top, CL_Size(256, 17)));
	label->set_text("Offset in pixels from LEFT of window");

	lineedit_top = new CL_LineEdit(window);
	lineedit_top->set_geometry(CL_Rect(10, 240, CL_Size(100, 20)));
	lineedit_top->set_text("0");
	lineedit_top->set_numeric_mode(true);
	label = new CL_Label(window);
	geometry = lineedit_top->get_geometry();
	label->set_geometry(CL_Rect(geometry.right + 5, geometry.top, CL_Size(256, 17)));
	label->set_text("Offset in pixels from TOP of window");

	lineedit_right = new CL_LineEdit(window);
	lineedit_right->set_geometry(CL_Rect(10, 280, CL_Size(100, 20)));
	lineedit_right->set_text("0");
	lineedit_right->set_numeric_mode(true);
	label = new CL_Label(window);
	geometry = lineedit_right->get_geometry();
	label->set_geometry(CL_Rect(geometry.right + 5, geometry.top, CL_Size(256, 17)));
	label->set_text("Offset in pixels from RIGHT of window");

	lineedit_bottom = new CL_LineEdit(window);
	lineedit_bottom->set_geometry(CL_Rect(10, 320, CL_Size(100, 20)));
	lineedit_bottom->set_text("0");
	lineedit_bottom->set_numeric_mode(true);
	label = new CL_Label(window);
	geometry = lineedit_bottom->get_geometry();
	label->set_geometry(CL_Rect(geometry.right + 5, geometry.top, CL_Size(256, 17)));
	label->set_text("Offset in pixels from BOTTOM of window");

	lineedit_fps = new CL_LineEdit(window);
	lineedit_fps->set_geometry(CL_Rect(10, 360, CL_Size(100, 20)));
	lineedit_fps->set_text("15");
	lineedit_fps->set_numeric_mode(true);
	label = new CL_Label(window);
	geometry = lineedit_fps->get_geometry();
	label->set_geometry(CL_Rect(geometry.right + 5, geometry.top, CL_Size(256, 17)));
	label->set_text("Maximum number of samples per second");

	try
	{
		gui.exec();
		if (update_window_worker_thread_started)
		{
			abort_writing = true;
			update_window_event_stop.set();
			update_window_worker_thread.join();
		}
	} catch(...)
	{
		if (update_window_worker_thread_started)
		{
			abort_writing = true;
			update_window_event_stop.set();
			update_window_worker_thread.join();
		}
		throw;
	}

	return 0;
}

void App::on_checkbox_changed_stretch(CL_CheckBox *checkbox)
{
	is_resize_to_fit = checkbox->is_checked();
}

void App::on_combo_selected_size(int value, CL_ComboBox *combo)
{
	selected_output_width = 1024;
	selected_output_height = 768;

	switch (value)
	{
		case 0:
			selected_output_width = 800;
			selected_output_height = 600;
			break;
		case 1:
			selected_output_width = 1024;
			selected_output_height = 768;
			break;
		case 2:
			selected_output_width = 1280;
			selected_output_height = 960;
			break;
		case 3:
			selected_output_width = 1280;
			selected_output_height = 1024;
			break;
		case 4:
			selected_output_width = 1440;
			selected_output_height = 900;
			break;
		case 5:
			selected_output_width = 1600;
			selected_output_height = 900;
			break;
		case 6:
			selected_output_width = 1680;
			selected_output_height = 1050;
			break;
		case 7:
			selected_output_width = 1920;
			selected_output_height = 1080;
			break;
	}
}

void App::on_combo_selected_compression(int value, CL_ComboBox *combo)
{
	use_WMV3_compression = (value==0);
}

void App::on_button_clicked2(CL_PushButton *button)
{
	abort_writing = true;
	button_2->set_enabled(false);
	button_1->set_enabled(true);
}

void App::on_button_clicked(CL_PushButton *button)
{
	if (!update_window_worker_thread_started)
	{
		update_window_worker_thread.start(this, &App::update_layered_worker_thread);
		update_window_worker_thread_started = true;
	}
	else
	{
		update_window_event_completed.wait();
		update_window_event_completed.reset();
	}
	selected_area_left = CL_StringHelp::text_to_int(lineedit_left->get_text());
	selected_area_top = CL_StringHelp::text_to_int(lineedit_top->get_text());
	selected_area_right = CL_StringHelp::text_to_int(lineedit_right->get_text());
	selected_area_bottom = CL_StringHelp::text_to_int(lineedit_bottom->get_text());
	if (selected_area_left<0)
		selected_area_left = 0;
	if (selected_area_top<0)
		selected_area_top = 0;
	if (selected_area_right<0)
		selected_area_right = 0;
	if (selected_area_bottom<0)
		selected_area_bottom = 0;

	selected_fps = CL_StringHelp::text_to_int(lineedit_fps->get_text());
	if (selected_fps < 0)
		selected_fps = 1;
	if (selected_fps > 100)
		selected_fps = 100;
	

	abort_writing = false;
	update_window_event_start.set();
	button_1->set_enabled(false);
	button_2->set_enabled(true);
}

bool App::on_close(CL_Window *win)
{
	win->exit_with_code(0);
	return true;
}


void App::update_layered_worker_thread()
{
	while (true)
	{
		int wakeup_reason = CL_Event::wait(update_window_event_start, update_window_event_stop);

		if (wakeup_reason != 0)
			break;

		update_window_event_start.reset();
		try
		{
			update_layered_worker_thread_process();
		}
		catch(CL_Exception &exception)
		{
			::MessageBox(NULL, exception.message.c_str(), "Exception Caught", MB_OK);
		}
		update_window_event_completed.set();
	}
}

void App::update_layered_worker_thread_process()
{

	int iXRes, iYRes; 

	iXRes = selected_output_width;
	iYRes = selected_output_height;
 
	int fps = 72;

	CL_String selected_mediatype;
	CL_String selected_bicompression;
	if (use_WMV3_compression)
	{
		selected_mediatype = CL_String("{33564D57-0000-0010-8000-00AA00389B71}");
		selected_bicompression = CL_String("WMV3");
	}
	else
	{
		selected_mediatype = CL_String("{3253534D-0000-0010-8000-00AA00389B71}");
		selected_bicompression = CL_String("MSS2");
	}

	CL_String profileString = cl_format(
		"        <profile version=\"589824\" \n"
		"             storageformat=\"1\" \n"
		"             name=\"ScreenGrabber\" \n"
		"             description=\"Screen Grabber\"> \n"
		"                   <streamconfig majortype=\"{73646976-0000-0010-8000-00AA00389B71}\" \n"
		"                   streamnumber=\"1\" \n"
		"                   streamname=\"Video Stream\" \n"
		"                   inputname=\"Video809\" \n"
		"                   bitrate=\"7000\" \n"
		"                   bufferwindow=\"5000\" \n"
		"                   reliabletransport=\"0\" \n"
		"                   decodercomplexity=\"AU\" \n"
		"                   rfc1766langid=\"en-gb\" \n"
		"                           vbrenabled=\"1\" \n"
		"                           vbrquality=\"100\" \n"
		"                           bitratemax=\"0\" \n"
		"                           bufferwindowmax=\"0\"> \n"
		"                     <videomediaprops maxkeyframespacing=\"10000000\" \n"
		"                                     quality=\"35\"/> \n"
		"             <wmmediatype subtype=\"%3\"  \n"
		"                   bfixedsizesamples=\"0\" \n"
		"                   btemporalcompression=\"1\" \n"
		"                   lsamplesize=\"0\"> \n"
		"       <videoinfoheader dwbitrate=\"7000\" \n"
		"                        dwbiterrorrate=\"0\" \n"
		"                        avgtimeperframe=\"400000\"> \n"
		"        <rcsource left=\"0\" \n"
		"                  top=\"0\" \n"
		"                  right=\"%1\" \n"
		"                  bottom=\"%2\"/> \n"
		"        <rctarget left=\"0\" \n"
		"                  top=\"0\" \n"
		"                  right=\"%1\" \n"
		"                  bottom=\"%2\"/> \n"
		"            <bitmapinfoheader biwidth=\"%1\" \n"
		"                              biheight=\"%2\" \n"
		"                              biplanes=\"1\" \n"
		"                              bibitcount=\"24\" \n"
		"                              bicompression=\"%4\" \n"
		"                              bisizeimage=\"0\" \n"
		"                              bixpelspermeter=\"0\" \n"
		"                              biypelspermeter=\"0\" \n"
		"                              biclrused=\"0\" \n"
		"                              biclrimportant=\"0\"/> \n"
		"       </videoinfoheader>\n"
		"            </wmmediatype>\n"
		"            </streamconfig>\n"
		"    </profile> \n"
		" \n"
		"\n", iXRes, iYRes, selected_mediatype, selected_bicompression);

	HRESULT result;
	result = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(result))
		throw CL_Exception("CoInitializeEx failed");

	IWMProfileManager *profileManager = 0;
	result = WMCreateProfileManager(&profileManager);
	if (FAILED(result))
		throw CL_Exception("WMCreateProfileManager failed");

	IWMProfileManager2 *profileManager2 = 0;
	result = profileManager->QueryInterface(IID_IWMProfileManager2, (void**)&profileManager2);
	if (FAILED(result))
		throw CL_Exception("QueryInterface failed");
	result = profileManager2->SetSystemProfileVersion(WMT_VER_9_0);
	if (FAILED(result))
		throw CL_Exception("SetSystemProfileVersion failed");
	profileManager2->Release();

	IWMProfile *profile = 0;
	CL_String16 profileString2 = CL_StringHelp::utf8_to_ucs2(profileString);
	result = profileManager->LoadProfileByData(profileString2.c_str(), &profile);
	if (FAILED(result))
		throw CL_Exception("LoadProfileByData failed");

	IWMWriter *writer = 0;
	result = WMCreateWriter(0, &writer);
	if (FAILED(result))
		throw CL_Exception("WMCreateWriter failed");
	result = writer->SetProfile(profile);
	if (FAILED(result))
		throw CL_Exception("SetProfile failed");
	result = writer->SetOutputFilename(L"ScreenCapture.wmv");
	if (FAILED(result))
		throw CL_Exception("SetOutputFilename failed");

	IWMInputMediaProps *inputMediaProps = 0;
	DWORD numInputs = 0;
	result = writer->GetInputCount(&numInputs);
	if (FAILED(result))
		throw CL_Exception("GetInputCount failed");
	if (numInputs != 1)
		throw CL_Exception("Unexpected amount of input streams");
	result = writer->GetInputProps(0, &inputMediaProps);
	if (FAILED(result))
		throw CL_Exception("GetInputProps failed");

	WMVIDEOINFOHEADER wmVideoInfoHeader = { 0 };
	wmVideoInfoHeader.bmiHeader.biSize = sizeof(wmVideoInfoHeader.bmiHeader);
	wmVideoInfoHeader.bmiHeader.biCompression = BI_RGB;
	wmVideoInfoHeader.bmiHeader.biBitCount = 24;
	wmVideoInfoHeader.bmiHeader.biPlanes = 1;
	wmVideoInfoHeader.bmiHeader.biWidth = iXRes;
	wmVideoInfoHeader.bmiHeader.biHeight = iYRes;
	wmVideoInfoHeader.bmiHeader.biSizeImage = iXRes*iYRes*3;
	wmVideoInfoHeader.rcSource.right = iXRes;
	wmVideoInfoHeader.rcSource.bottom = iYRes;
	wmVideoInfoHeader.rcTarget = wmVideoInfoHeader.rcSource;
	wmVideoInfoHeader.dwBitRate = (wmVideoInfoHeader.bmiHeader.biWidth*wmVideoInfoHeader.bmiHeader.biHeight*wmVideoInfoHeader.bmiHeader.biBitCount*fps);
	wmVideoInfoHeader.AvgTimePerFrame = ((QWORD)1) * 1000 * 1000 / fps; // desired average time per frame in 100-nanosecond units

	WM_MEDIA_TYPE mediaType = { 0 };
	mediaType.majortype = WMMEDIATYPE_Video;
	mediaType.subtype = WMMEDIASUBTYPE_RGB24;
	mediaType.formattype = WMFORMAT_VideoInfo;
	mediaType.bFixedSizeSamples = false;
	mediaType.bTemporalCompression = false;
	mediaType.lSampleSize = 0;
	mediaType.pUnk = 0;
	mediaType.cbFormat = sizeof(WMVIDEOINFOHEADER);
	mediaType.pbFormat = (BYTE*)&wmVideoInfoHeader;

	result = inputMediaProps->SetMediaType(&mediaType);
	if (FAILED(result))
		throw CL_Exception("SetMediaType failed");

	result = writer->SetInputProps(0, inputMediaProps);
	if (FAILED(result))
		throw CL_Exception("SetInputProps failed");

	result = writer->BeginWriting();
	if (FAILED(result))
		throw CL_Exception("BeginWriting failed");

	unsigned int *pixels = 0;
	HDC screenDC = 0;
	HDC memoryDC = 0;
	HBITMAP bitmap = 0;
	HBITMAP oldBitmap = 0;
	HWND selected_window = 0;

	CL_Rect last_window_rect;
	CL_Rect current_window_rect;

	unsigned int time_start = CL_System::get_time();
	unsigned int time_last = time_start;
	int timeMS = 0;
	while(true)
	{
		if (abort_writing)
			break;

		if (selected_window)
		{
			HWND fg_window = GetForegroundWindow();

			RECT rect;
			GetClientRect(fg_window, &rect);
			current_window_rect = CL_Rect(0, 0, rect.right - rect.left, rect.bottom -  rect.top);

			if ((fg_window != selected_window) || (current_window_rect.get_size() != last_window_rect.get_size() ) )
			{
				if (memoryDC)
					SelectObject(memoryDC, oldBitmap);
				if (bitmap)
					DeleteObject(bitmap);
				if (memoryDC)
					DeleteDC(memoryDC);
				if (screenDC)
					ReleaseDC(0, screenDC);

				pixels = 0;
				screenDC = 0;
				memoryDC = 0;
				bitmap = 0;
				oldBitmap = 0;
				selected_window = 0;
				selected_window = NULL;
			}
		}

		if (!selected_window)
		{
			HWND fg_window = GetForegroundWindow();

			RECT rect;
			GetClientRect(fg_window, &rect);
			current_window_rect = CL_Rect(0, 0, rect.right - rect.left, rect.bottom -  rect.top);

			std::vector<char> buffer;
			buffer.resize(256);
			GetWindowText(fg_window, &buffer[0], buffer.size());

			CL_String target_window(&buffer[0]);
			CL_String current_title = window->get_title();
			if (current_title != target_window)
			{
				selected_window = fg_window;
				pixels = 0;
				screenDC = GetDC(selected_window);
				memoryDC = CreateCompatibleDC(screenDC);
				bitmap = CreateDIBSection(screenDC, (BITMAPINFO*)&wmVideoInfoHeader.bmiHeader, DIB_RGB_COLORS, (void**)&pixels, 0, 0);
				oldBitmap = (HBITMAP)SelectObject(memoryDC, bitmap);
			}
		}

		last_window_rect = current_window_rect;

		if (selected_window)
		{
			INSSBuffer *sampleBuffer = 0;
			result = writer->AllocateSample(wmVideoInfoHeader.bmiHeader.biSizeImage, &sampleBuffer);
			if (FAILED(result))
				throw CL_Exception("AllocateSample failed");

			CL_Rect source_rect(current_window_rect);

			source_rect.shrink(selected_area_left, selected_area_top, selected_area_right, selected_area_bottom);

			if (is_resize_to_fit)
			{
			
				int dest_width = iXRes;
				int dest_height = iYRes;

				if (source_rect.get_height() < source_rect.get_width())
				{
					dest_height = (dest_width * source_rect.get_height()) / source_rect.get_width();
				}
				else
				{
					dest_width = (dest_height * source_rect.get_width()) / source_rect.get_height();
				}

				BOOL bltResult = StretchBlt(memoryDC, 0, 0, dest_width, dest_height, screenDC, source_rect.left, source_rect.top,  source_rect.get_width(), source_rect.get_height(), SRCCOPY/*|CAPTUREBLT*/);
			}
			else
			{
				BOOL bltResult = BitBlt(memoryDC, 0, 0, source_rect.get_width(), source_rect.get_height(), screenDC, source_rect.left, source_rect.top, SRCCOPY);
			}

			BYTE *sampleData = 0;
			DWORD sampleLength = 0;
			result = sampleBuffer->GetBufferAndLength(&sampleData, &sampleLength);
			if (FAILED(result))
				throw CL_Exception("GetBufferAndLength failed");
			sampleLength = min(sampleLength, wmVideoInfoHeader.bmiHeader.biSizeImage);
			memcpy(sampleData, pixels, sampleLength);
			result = sampleBuffer->SetLength(sampleLength);
			if (FAILED(result))
				throw CL_Exception("SetLength failed");

			QWORD sampleTime = ((QWORD)timeMS) * 10000;
			result = writer->WriteSample(0, sampleTime, 0, sampleBuffer);
			if (FAILED(result))
				throw CL_Exception("WriteSample failed");
			sampleBuffer->Release();
		}

		unsigned int time_now = CL_System::get_time();
		const int desired_fps = 1000/selected_fps;
		int time_diff = time_now - time_last;
		if (time_diff < desired_fps)
			CL_System::sleep(desired_fps - time_diff);

		if (!selected_window)	// Pause time if the selected window is not active
		{
			time_start += time_now - time_last;
		}

		time_last = time_now;
		
		timeMS = time_now - time_start;
	}

	if (memoryDC)
		SelectObject(memoryDC, oldBitmap);
	if (bitmap)
		DeleteObject(bitmap);
	if (memoryDC)
		DeleteDC(memoryDC);
	if (screenDC)
		ReleaseDC(0, screenDC);

	result = writer->EndWriting();
	if (FAILED(result))
		throw CL_Exception("EndWriting failed");

	writer->Release();
	profile->Release();
	profileManager->Release();

}
