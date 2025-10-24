
#include <ClanLib/core.h>
#include <ClanLib/application.h>

class UClassResourceData : public CL_ResourceData
{
public:
	UClassResourceData(CL_Resource &resource) : CL_ResourceData(resource) {}

	virtual void on_load()
	{
		CL_DomElement element = get_resource().get_element();
		strength = CL_String::to_int(element.get_attribute("strength"));
		magic = CL_String::to_int(element.get_attribute("magic"));
	}

	int strength;
	int magic;
};

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
			// Hook up callback that gets called when a resource is added to a resource manager:
			slots.connect(CL_ResourceManager::sig_resource_added(), this, &ResourceApp::on_resource_added);

			// Construct resource manager. This will cause sig_resource_added() to be signalled for each
			// resource object located in the resource file. This means ResourceApp::on_resource_added will
			// get called for each resource.
			CL_ResourceManager resources("resources.xml");

			// Get one of our custom uclass resources:
			CL_Resource resource_mage = resources.get_resource("Classes/Mage");

			// Locate the custom data object we attached to it:
			UClassResourceData *custom_data = (UClassResourceData *) resource_mage.get_data("uclass_data");

			// Before a resource is to be used, load() should generally be called on it. This causes
			// sig_load() signal to be called (if it isnt loaded already), allowing the attached data
			// objects to load any additional data. Eg. a surface resource would load the image data itself.
			resource_mage.load();
			
			std::cout << "mage strength = " << custom_data->strength << std::endl;
			std::cout << "mage magic = " << custom_data->magic << std::endl;

			// Unload resource after usage:
			resource_mage.unload();
		}
		catch (CL_Error e)
		{
			std::cout << "Unhandled exception: " << e.message.c_str() << std::endl;
		}

		// Wait for user to press a key in console:
		console.display_close_message();

		return 0;
	}

	void on_resource_added(CL_Resource &resource)
	{
		// Here we check if the resource type is interesting for us.
		// We can attach any number of CL_ResourceData objects to a resource,
		// including those that clanlib also handles. They can then later
		// be retrieved with a call to CL_Resource::get_data(name_they_were_attached_with).

		// In this example we will connect a custom object for the 'uclass' resource type:
		if (resource.get_type() == "uclass")
		{
			resource.attach_data("uclass_data", new UClassResourceData(resource));
		}
	}

	// Container object for signal->slot connections.
	CL_SlotContainer slots;
};

// The one and only global application object. Required for any clanApp project.
ResourceApp app;
