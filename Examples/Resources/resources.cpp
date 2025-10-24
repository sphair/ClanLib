
#include <ClanLib/core.h>
#include <ClanLib/application.h>

class ResourceApp : public CL_ClanApplication
{
public:
	int main(int, char **)
	{
		// Boot:
		CL_SetupCore setup_core;

		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Resource Example",80,1000); // 1000 allows a y-scrollbar to be present
		console.redirect_stdio();

		try
		{
			// Construct resource manager. This will cause sig_resource_added() to be signalled for each
			// resource object located in the resource file. This means ResourceApp::on_resource_added will
			// get called for each resource.
			CL_ResourceManager resources("resources.xml");

			// Lets try to access some of the clanlib objects in the resource file:
			std::string config_name = CL_String::load("Configuration/name", &resources);
			int config_width = CL_Integer("Configuration/width", &resources);
			int config_height = CL_Integer("Configuration/height", &resources);

			std::cout << "name = " << config_name.c_str() << std::endl;
			std::cout << "width = " << config_width << std::endl;
			std::cout << "height = " << config_height << std::endl;

			// Get a list over all classes
			std::vector<std::string> v = resources.get_resources_of_type("uclass", "Classes");
			std::vector<std::string>::iterator it;
			for(it = v.begin(); it != v.end(); ++it)
			{
				// Get one of our custom resources:
				CL_Resource resource = resources.get_resource(*it);

				// Before a resource is to be used, load() should generally be called on it. This causes
				// sig_load() signal to be called (if it isnt loaded already), allowing the attached data
				// objects to load any additional data. Eg. a surface resource would load the image data itself.
				resource.load();

				// Its possible to access the dom element object in the resource:
				CL_DomElement element = resource.get_element();
				int strength = CL_String::to_int(element.get_attribute("strength"));
				int magic = CL_String::to_int(element.get_attribute("magic"));

				std::cout << std::endl << "name = " << resource.get_name() << std::endl;
				std::cout << "strength = " << strength << std::endl;
				std::cout << "magic = " << magic << std::endl;

				// Unload resource after usage:
				resource.unload();
			}
		}
		catch (CL_Error e)
		{
			std::cout << "Unhandled exception: " << e << std::endl;
		}

		// Wait for user to press a key in console:
		console.display_close_message();

		return 0;
	}
};

// The one and only global application object. Required for any clanApp project.
ResourceApp app;
