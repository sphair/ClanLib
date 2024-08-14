
/*! \page Core Core Module Reference

 <p>The clanCore library is the base library used by all other ClanLib libraries. It provides a basic framework with
 common functionality needed by almost all types of applications.</p>
  
 <h2>Platform abstraction</h2>

 <ul>
  <li>clan::System - General system helper functions</li>
  <li>clan::RegistryKey - C++ class for accessing the Windows registry</li>
  <li>clan::ComPtr - Smart pointer template for Component Object Model (COM) objects</li>
 </ul>

 <h2>General utility classes</h2>
  
 <ul>
  <li>clan::Exception - Base class for all exceptions thrown by ClanLib</li>
  <li>clan::DataBuffer - General purpose memory buffer class</li>
  <li>clan::DisposableObject - Helper for objects that may outlive their disposable resources</li>
  <li>clan::UserDataOwner - Allows storing any std::shared_ptr as user data on an object</li>
  <li>clan::DateTime - Date/time handling class</li>
 </ul>
  
 <h2>Signals</h2>

 <ul>
  <li>clan::Signal</li>
  <li>clan::SlotContainer</li>
 </ul>

 <h2>Math</h2>
  
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
  
 <ul>
  <li>clan::IODevice - Input/Output stream abstraction</li>
  <li>clan::File - Class for opening files</li>
  <li>clan::MemoryDevice - IODevice for memory buffers</li>
 </ul>
  
 <h2>File systems</h2>

 <ul>
  <li>clan::FileSystem - Virtual file system support</li>
  <li>clan::DirectoryListing - List files in a virtual file system</li>
  <li>clan::DirectoryScanner - List files outside of clan::FileSystem</li>
  <li>clan::PathHelp - Helper class for working with path strings</li>
  <li>clan::Directory - Perform basic directory based file operations</li>
  <li>clan::HTMLUrl - Parsing HTML URLs and breaking them down into their basic components</li>
 </ul>

 <h2>Zip files</h2>

 <ul>
  <li>clan::ZipArchive - Create, open or modify a zip file</li>
  <li>clan::ZipReader - Low level class for reading zip files sequentially</li>
  <li>clan::ZipWriter - Write zip files sequentially</li>
  <li>clan::ZLibCompression - Deflate algorithm compressor / decompressor</li>
 </ul>
  
 <h2>Resource management</h2>
  
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

 <ul>
  <li>clan::SHA1, clan::SHA224, clan::SHA256, clan::SHA384, clan::SHA512, clan::SHA512_224, clan::SHA512_256 - SHA Hashing functions</li>
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

 <ul>
  <li>clan::CrashReporter - Generates crash reports when the application crashes, or when requested</li>
  <li>clan::DetectHang - Calls the crash reporter if the constructing thread hangs for more than 30 seconds</li>
  <li>clan::ExceptionDialog - Displays a modal dialog with exception details</li>
 </ul>

*/
