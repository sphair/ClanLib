
/*! \page ResourceManagement Resource Management

*** THIS PAGE REQUIRES UPDATING FOR CLANLIB 3.0 ***

<h2>Resource Manager</h2>

The resource manager in ClanLib allows you to use a central source for all your resources in your application. The system defines a simple XML format describing resources used by ClanLib, and since it is XML it also allows you to use it for your own custom resources. 

A resource XML file may look like this: 

\code
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
\endcode

The class \ref clan::ResourceManager is used to load, manage and save resource files. The syntax for loading resources is fairly straightforward: 

\code
clan::ResourceManager resources("resources.xml");
clan::Texture texture(gc, "section1/texture1", &resources);
clan::SoundBuffer sample("my-sample", &resources);
\endcode

<h2>Resource Objects</h2>

Although the above syntax is the common way to load resources, in reality all objects defined in a resource XML are all represented as \ref clan::Resource objects. To illustrate how this object is used, the above \ref clan::Texture constructor used to load a texture may have been implemented like this: 

\code
clan::ResourceManager resources("resources.xml");
clan::Resource resource = resources.get_resource("section1/texture1");
clan::String filename = resource.get_element().get_attribute("file");
clan::PixelBuffer image = clan::ImageProviderFactory::load(filename, resources.get_directory());
clan::Texture texture(gc, image);
\endcode

In the same way we might create our own custom resource loader: 

\code
clan::String load_my_type(const clan::String &res_id, clan::ResourceManager &resources)
{
	clan::Resource resource = resources.get_resource(res_id);
	if (resource.get_type() != "my-type")
		throw clan::Exception("Resource not of expected type!");
	clan::DomElement text_element = resource.get_element().named_item("text").to_element();
	return text_element.get_text();
}
 
clan::ResourceManager resources("resources.xml");
clan::Console::write_line(load_my_type("section2/section3/my-resource1", resources);
\endcode
*/
