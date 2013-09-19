
/*! \page ResourceManagement Resource Management

MainDocs:Resource Management

From ClanLib Game SDK
 

Resource management interfaces in ClanLib: 
• Resource manager 
• Resource object 

Resource Manager

The resource manager in ClanLib allows you to use a central source for all your resources in your application. The system defines a simple XML format describing resources used by ClanLib, and since it is XML it also allows you to use it for your own custom resources. 

A resource XML file may look like this: 


<resources>
	<section name="section1">
		<texture name="texture1" file="image.png" />
	</section>
	<section name="section2">
		<section name="section3">
			<my-type name="my-resource1">
				<text>Hello World!</text>
			</my-type>
		</section>
	</section>
	<sample name="my-sample" file="test.wav" />
</resources>

The class CL_ResourceManager is used to load, manage and save resource files. The syntax for loading resources is fairly straightforward: 


CL_ResourceManager resources("resources.xml");
CL_Texture texture(gc, "section1/texture1", &resources);
CL_SoundBuffer sample("my-sample", &resources);

Resource Objects

Although the above syntax is the common way to load resources, in reality all objects defined in a resource XML are all represented as CL_Resource objects. To illustrate how this object is used, the above CL_Texture constructor used to load a texture may have been implemented like this: 


CL_ResourceManager resources("resources.xml");
CL_Resource resource = resources.get_resource("section1/texture1");
CL_String filename = resource.get_element().get_attribute("file");
CL_PixelBuffer image = CL_ImageProviderFactory::load(filename, resources.get_directory());
CL_Texture texture(gc, image);

In the same way we might create our own custom resource loader: 


CL_String load_my_type(const CL_String &res_id, CL_ResourceManager &resources)
{
	CL_Resource resource = resources.get_resource(res_id);
	if (resource.get_type() != "my-type")
		throw CL_Exception("Resource not of expected type!");
	CL_DomElement text_element = resource.get_element().named_item("text").to_element();
	return text_element.get_text();
}
 
CL_ResourceManager resources("resources.xml");
CL_Console::write_line(load_my_type("section2/section3/my-resource1", resources);

*/
