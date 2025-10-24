/*
	The minimum requirements needed by every ClanLib application.

	Note that the try/catch is not required but should be added 
	to every ClanLib application.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>

class MinimumApp : public CL_ClanApplication
{
public:
	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
		 	CL_SetupCore setup_core;

			printf("Creating test.zip...\n\n");

			CL_Zip_Archive makeZip;
			makeZip.add_file("README.txt");  //the real file
			makeZip.add_file("README.txt", "MyFolder/README.txt", false); //adding another instance in a folder and don't compress it
			makeZip.save("test.zip"); //actually build it

			printf("Opening test.zip and displaying the files:\n\n");

			CL_Zip_Archive zip("test.zip");
			std::vector<CL_Zip_FileEntry> &file_list = zip.get_file_list();
			int size = file_list.size();
			for (int i=0; i<size; i++)
			{
				std::cout << file_list[i].get_filename().c_str() << std::endl;
			}

			//actually opening the readme.txt file and displaying it:
			
			printf("\n\nLet's open readme.txt from our zip and display it:\n\n");

			CL_InputSource *fileSource = zip.open_source("README.txt");
			
			while (fileSource->tell() < fileSource->size())
			{
				printf("%c", fileSource->read_int8());
			}
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;
		}

		// Display console close message and wait for a key
		console.display_close_message();

		return 0;
	}
} app;
