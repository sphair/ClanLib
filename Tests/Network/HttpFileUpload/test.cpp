
#include <ClanLib/core.h>
#include <ClanLib/network.h>
#include "http_post_file_upload.h"

int main(int, char**)
{
	CL_SetupCore setup_core;
	CL_SetupNetwork setup_network;
	try
	{
 		HttpPostFileUpload upload;
		upload.add_form_field("MAX_FILE_SIZE", "500000");
		upload.add_form_field("e-mail", "foobar@clanlib.org");
		upload.add_file("file1.txt");
		upload.add_file("file2.txt");
		upload.send("10.0.0.3", "80", "/upload_receiver.php");
	}
	catch (CL_Exception e)
	{
		CL_Console::write_line(e.message);
	}
  	return 0;
}

