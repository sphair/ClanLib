
#include "precomp.h"
#include "system_dialog.h"
#ifndef WIN32
#include <qt3/qapplication.h>
#include <qt3/qfiledialog.h>
#endif

CL_String SystemDialog::show_save_file_dialog()
{
	CL_String::char_type filename_buffer[FILENAME_MAX];

#ifdef WIN32
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = 0;
	ofn.lpstrFile = filename_buffer;

	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = FILENAME_MAX;
	ofn.lpstrFilter = cl_text("All\0*.*\0XML files\0*.xml\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;

	BOOL success = GetSaveFileName(&ofn);
#else
	bool success = false;
#endif
	if (success)
	{
		return filename_buffer;
	}
	else
	{
		return cl_text("");
	}
}

CL_String SystemDialog::show_open_file_dialog()
{
#ifdef WIN32
	CL_String::char_type filename_buffer[FILENAME_MAX];

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = 0;
	ofn.lpstrFile = filename_buffer;

	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = FILENAME_MAX;
	ofn.lpstrFilter = cl_text("All\0*.*\0XML files\0*.xml\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;

	BOOL success = GetOpenFileName(&ofn);

	if (success)
	{
		return filename_buffer;
	}
	else
	{
		return cl_text("");
	}
#else
	bool success = false;

	int argc;
	char *argv[2] = {"", ""};
	QApplication a( argc, argv );

	QFileDialog dialog( "./" );

	a.setMainWidget( &dialog );
	dialog.show();
	a.exec();

	QString str = dialog.selectedFile();

	return str.ascii();
#endif
}
