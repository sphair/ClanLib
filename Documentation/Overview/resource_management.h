
/*! \page ResourceManagement Resource Management

<p>The resource management system in ClanLib is based on the principle that for each resource type there is a resource cache object responsible
for fetching resources of that type.</p>
                    
<p>Since resources can be virtually anything (images, textures, 3D meshes, shared GPU objects, etc), the
system relies on casting the generic clan::ResourceObject class to a specific clan::Resource&lt;Type&gt;
object. The clan::Resource&lt;Type&gt; template can return a reference the actual shared resource object, as well as track if the
resource has been modified.
</p>

<p>Just like resources can be virtually anything, the method for fetching a clan::ResourceObject also varies from type to
type. To get a resource, the resource cache object managing it must first be retrieved. clan::ResourceManager has a
clan::get_cache&lt;Type&gt;(name) function with returns a specific cache. For clanDisplay based resources, the cache
interface is the clan::DisplayCache class, and it is stored in the clan::ResourceManager with the
name "clan.display".
</p>

<p>clan::ResourceManager is in other words just a map of cache objects for different resource types. It is possible to create
custom resource handling by constructing clan::ResourceManager manually, then attach your own implementation of
clan::DisplayCache and so on.</p>
                        
<p>Most applications rely on the default implementation which is provided by constructing clan::XMLResourceManager instead. It
loads resources described in an XML file. For the standard objects offered by ClanLib, such as the clan::Sprite
class, the above manual fetching of resource is also not needed. The convenience function clan::Sprite::resource(gc, id, resources)
takes care of the common case.
</p>

*** THIS PAGE REQUIRES UPDATING FOR CLANLIB 3.0

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
