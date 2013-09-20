
/*! \page Core Core Module Overview

    <p>The clanCore library is the base library used by all other ClanLib libraries. It provides a basic framework with
    common functionality needed by almost all types of applications.</p>
		
   <p>Core Overview Pages: \ref ResourceManagement , \ref Threading , \ref TemplateCallbacks </p>
 		
    <h2>Platform abstraction</h2>
		
    <p>Perhaps the most important features of clanCore are its platform abstraction classes. While the C++ Standard Library
    also includes a limited platform abstraction, the clanCore classes extend, complement, or completely replace
    them in some cases.</p>
		
    <ul>
	    <li>clan::System - General system helper functions</li>
	    <li>clan::Thread, clan::Runnable - Threading support</li>
	    <li>clan::Mutex, clan::MutexSection - Simple mutex support</li>
	    <li>clan::InterlockedVariable - Atomic integer operations</li>
	    <li>clan::Event - OS level waitable event</li>
	    <li>clan::RegistryKey - C++ class for accessing the Windows registry</li>
	    <li>clan::ComPtr - Smart pointer template for Component Object Model (COM) objects</li>
    </ul>
		
    <h2>General utility classes</h2>
		
    <p>In addition to the above classes there are also a few general purpose classes providing infrastructure for other
    parts of the library.</p>
		
    <ul>
	    <li>clan::Exception - Base class for all exceptions thrown by ClanLib</li>
	    <li>clan::DataBuffer - General purpose memory buffer class</li>
	    <li>clan::DisposableObject - Helper for objects that may outlive their disposable resources</li>
	    <li>clan::UserDataOwner - Allows storing any std::shared_ptr as user data on an object</li>
	    <li>clan::DateTime - Date/time handling class</li>
    </ul>
		
    <h2>Thread run-loop</h2>
		
    <p>Most operating systems uses a model where a thread receives messages from the host system by participating in a
    message run loop.</p>
		
    <p>In ClanLib, the run-loop is known as the keep-alive application loop and the following classes provides this
    infrastructure:</p>
		
    <ul>
	    <li>clan::KeepAlive - Application loop helper</li>
	    <li>clan::KeepAliveObject - Interface for objects participating in the thread keep-alive processing loop</li>
	    <li>clan::Timer - Timer class that invokes a callback on a specified interval</li>
    </ul>
		
    <h2>Signals and callbacks</h2>
		
    <p>Templates for creating type-safe callbacks are provided by clanCore. They come in two flavors: signals and callbacks.</p>
		
    <p>Signals are one-to-many callback templates. When the invoke function is called on a signal, all
        connected callback functions are called. clan::Slot is the representation of a connected callback
        function and if the slot is destroyed the connection to the signal is broken. clan::SlotContainer is
        provided for keeping multiple slots connected.</p>
		
    <p>Callback templates are just pointing at a single callback function.  The clan::invoke can only be
    called safely if a function has been set.  The main advantage callbacks have over signals is that the callback
    can return a value to the invoking function.  A signal would have to pass such a return value by reference.</p>
		
    <h2>Math</h2>
		
    <p>ClanCore sports a lot of classes for doing basic graphics related math. The most important classes are:</p>
		
    <ul>
	    <li>clan::VecXx (clan::Vec4i, clan::Vec3f, clan::Vec2ub, etc) - Vector templates for 2D, 3D and 4D</li>
	    <li>clan::HalfFloat, clan::VecXhf - Half-float converters</li>
	    <li>clan::MatXx (clan::Mat4f, clan::Mat3f, etc) - Matrix templates</li>
	    <li>clan::Quaternionf, clan::Quaterniond - Quaternion rotation/orientation classes</li>
	    <li>clan::Rect, clan::Rectf, clan::Rectd - 2D (left,top,right,bottom) rectangle structure</li>
	    <li>clan::Point, clan::Pointf, clan::Pointd - 2D (x,y) point structure</li>
	    <li>clan::Size, clan::Sizef, clan::Sized - 2D (width,height) size structure</li>
    </ul>
		
    <h2>Text</h2>
		
    <p>All strings are assumed to be UTF-8, unless otherwise specified.  std::string is used as the main string class.</p>
		
    <ul>
	    <li>clan::StringHelp - Misc string helper functions</li>
	    <li>clan::UTF8_Reader - Converts UTF-8 to 32-bit unicode characters</li>
	    <li>clan::StringFormat, clan::string_format - String formatting class and functions</li>
	    <li>clan::Console - Read from/write to console windows</li>
    </ul>
		
    <h2>XML</h2>
		
    <p>Working with XML can be done by using either the XML tokenizer classes, the Document Object Model (DOM) classes, or the XPath
    evaluator.</p>
		
    <ul>
	    <li>clan::XMLTokenizer - Breaks a XML file into tokens</li>
	    <li>clan::XMLWriter - Writes a XML file based on tokens</li>
	    <li>clan::DomDocument - DOM Document class</li>
	    <li>clan::DomNode, clan::DomElement, clan::DomText - Main node types</li>
	    <li>clan::XPathEvaluator - Evaluate the result of xpath expressions</li>
    </ul>

    <h2>JSON</h2>

    <p>The clan::JsonValue class allows you to parse or generate JSON strings.</p>
		
    <h2>I/O devices</h2>
		
    <p>The clan::IODevice class implements the classic input/output stream. It is derived by clan::File
        and others to support various different devices. The full set of classes working with I/O devices are as follows:
    </p>

    <ul>
        <li>clan::IODevice - Input/Output stream abstraction</li>
        <li>clan::File - Class for opening files</li>
        <li>clan::IODevice_Memory - IODevice for memory buffers</li>
        <li>clan::PipeConnection - IODevice for pipes</li>
        <li>clan::PipeListen - Accepts incoming pipe connections</li>
    </ul>
		
    <h2>File systems</h2>

    <p>There are several ways to deal with file systems. The clan::File class mentioned above can be used or files can
        be opened via clan::FileSystem. The file system class supports virtual file systems, such as opening files in a
        zip file. In addition to this, there are several classes for working with files and paths:
    </p>

    <ul>
        <li>clan::FileSystem - Virtual file system support</li>
        <li>clan::DirectoryListing - List files in a virtual file system</li>
        <li>clan::DirectoryScanner - List files outside of clan::FileSystem</li>
        <li>clan::PathHelp - Helper class for working with path strings</li>
        <li>clan::Directory - Perform basic directory based file operations</li>
        <li>clan::HTMLUrl - Parsing HTML URLs and breaking them down into their basic components</li>
    </ul>

    <h2>Zip files</h2>

    <p>Zip files can be managed using either the clan::FileSystem class mentioned in the file systems section, or
        they can be read or written using the following lower level classes:
    </p>

    <ul>
        <li>clan::ZipArchive - Create, open or modify a zip file</li>
        <li>clan::ZipReader - Low level class for reading zip files sequentially</li>
        <li>clan::ZipWriter - Write zip files sequentially</li>
        <li>clan::ZLibCompression - Deflate algorithm compressor / decompressor</li>
    </ul>
		
    <h2>Resource management</h2>
		
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

    <p>Resource infrastructure classes:</p>

    <ul>
        <li>clan::ResourceManager - Class holding all resource caches, which in turn holds all resources</li>
        <li>clan::ResourceObject - Class that can hold resources of any type, with casting support to specific types</li>
        <li>clan::Resource&lt;Type&gt; - Proxy to a resource of a specific type</li>
        <li>clan::ResourceContainer - Helper class for resource caches. Helps storing resources</li>
        <li>clan::XMLResourceManager - ResourceManager with standard resource caches using XML resources</li>
        <li>clan::XMLResourceDocument - Document class for the ClanLib XML resource format</li>
        <li>clan::XMLResourceNode - Resource node in a ClanLib XML resource file</li>
    </ul>

    <h2>Crypto</h2>

    <p>Cryptographic classes for dealing with SHA hashing, AES encryption and TLS are provided:</p>

    <ul>
        <li>clan::SHA1, clan::SHA224, clan::SHA256, clan::SHA384,
            clan::SHA512, clan::SHA512_224, clan::SHA512_256 - SHA Hashing functions
        </li>
        <li>clan::MD5 - MD5 Hashing function</li>
        <li>clan::RSA - RSA encryption/decryption</li>
        <li>clan::Random - Uses the operating system cryptographically secure pseudorandom number generator</li>
        <li>clan::AES128_Encrypt, clan::AES128_Decrypt - AES-128 encryption/decryption running in Cipher Block Chaining mode</li>
        <li>clan::AES192_Encrypt, clan::AES192_Decrypt - AES-192 encryption/decryption running in Cipher Block Chaining mode</li>
        <li>clan::AES256_Encrypt, clan::AES256_Decrypt - AES-256 encryption/decryption running in Cipher Block Chaining mode</li>
        <li>clan::Secret - Key class</li>
        <li>clan::TLSClient - TLS protocol handling for a TLS client</li>
    </ul>

    <h2>Error reporting</h2>

    <p>To deal with errors, clanCore offers the following functionality:</p>

    <ul>
        <li>clan::CrashReporter - Generates crash reports when the application crashes, or when requested</li>
        <li>clan::DetectHang - Calls the crash reporter if the constructing thread hangs for more than 30 seconds</li>
        <li>clan::ExceptionDialog - Displays a modal dialog with exception details</li>
    </ul>

*/
