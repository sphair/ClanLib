
/*! \page Display Display Module Reference

    <h2>Window and Input</h2>

    <ul>
        <li>clan::DisplayWindow - Top-level window class</li>
        <li>clan::DisplayWindowDescription - Allows you to setup a more advanced description when creating a display window</li>
        <li>clan::Cursor - Mouse cursor class</li>
        <li>clan::CursorDescription - Allows you to setup a more advanced description when creating a mouse cursor</li>
        <li>clan::InputDevice - Interface for keyboards, mice and game controllers</li>
        <li>clan::InputEvent - Describes an input event, such as key down, key up, mouse movement, etc</li>
        <li>clan::InputCode - This enumeration contains all the input IDs for well-known buttons</li>
    </ul>

    <h2>2D Graphics</h2>

    <ul>
        <li>clan::Canvas - Main 2D drawing class</li>
        <li>clan::Image - 2D image</li>
        <li>clan::Sprite - More advanced version of a 2D image with multiple frames for animation</li>
        <li>clan::Path - Describes a 2D path</li>
        <li>clan::Color, clan::Colorf - Specifies a color using red, green, blue and alpha components</li>
        <li>clan::ColorHSLi, clan::ColorHSLf - Specifies a color using hue, saturation, lightness and alpha components</li>
        <li>clan::ColorHSVi, clan::ColorHSVf - Specifies a color using hue, saturation, value and alpha components</li>
        <li>clan::Gradient - Describes a linear gradient</li>
        <li>clan::SpanLayout - Draw rich text with word wrapping, images, fonts and colors</li>
        <li>clan::TextureGroup, clan::Subtexture - Calibrates which images end up in which atlas textures</li>
    </ul>

    <h2>Fonts</h2>

    <ul>
        <li>clan::Font - Font class for System / TrueType / OpenType fonts and fonts using sprite image frames as glyphs</li>
        <li>clan::FontDescription - Allows you to setup a more advanced description when creating a font</li>
        <li>clan::FontMetrics - The metrics of a font</li>
    </ul>

    <h2>Image Pixels</h2>

    <ul>
        <li>clan::PixelBuffer - Buffer containing image pixels (in CPU or GPU memory)</li>
        <li>clan::PixelBufferSet - Set of images that combined form a complete texture</li>
        <li>clan::PixelConverter - Pixel format converter class</li>
        <li>clan::TextureFormat - Enumeration for all the pixel formats supported</li>
        <li>clan::IconSet - Set of images for rendering an icon at different sizes</li>
        <li>clan::PerlinNoise - Perlin Noise Generator</li>
    </ul>

    <h2>3D Graphics</h2>

    <ul>
        <li>clan::GraphicContext - Main immediate drawing class</li>
        <li>clan::Texture - Texture object base class</li>
        <li>clan::Texture1D, clan::Texture1DArray - One dimensional textures</li>
        <li>clan::Texture2D, clan::Texture2DArray - Two dimensional textures</li>
        <li>clan::Texture3D - Three dimensional textures</li>
        <li>clan::TextureCube, clan::TextureCubeArray - Cube textures</li>
        <li>clan::UniformBuffer, clan::UniformVector&lt;T&gt; - GPU buffer for uniform variables</li>
        <li>clan::VertexArrayBuffer, clan::VertexArrayVector&lt;T&gt; - GPU buffer for vertex attributes</li>
        <li>clan::ElementArrayBuffer, clan::ElementArrayVector&lt;T&gt; - GPU buffer for element indices</li>
        <li>clan::StorageBuffer, clan::StorageVector&lt;T&gt; - GPU buffer for shader storage (aka. Unordered Access View)</li>
        <li>clan::TransferBuffer, clan::TransferTexture, clan::TransferVector - CPU/GPU memory transfer buffers</li>
        <li>clan::BlendState, clan::BlendStateDescription - Blending state setup</li>
        <li>clan::DepthStencilState, clan::DepthStencilStateDescription - Depth and stencil state setup</li>
        <li>clan::RasterizerState, clan::RasterizerStateDescription - Rasterizer state setup</li>
        <li>clan::FrameBuffer, clan::RenderBuffer - Frame buffer setup</li>
        <li>clan::ProgramObject, clan::ShaderObject - Shader programs</li>
        <li>clan::PrimitivesArray - Vertex buffer objects</li>
        <li>clan::OcclusionQuery - Occlusion queries</li>
        <li>clan::RenderBatcher - Interface for participating in render batching</li>
    </ul>

    <h2>Shader Effects</h2>

    <ul>
        <li>clan::ShaderEffect - High level class for running shaders</li>
        <li>clan::ShaderEffectDescription - Allows you to setup a more advanced description when creating a shader effect</li>
    </ul>

    <h2>Resources</h2>

    <ul>
        <li>clan::DisplayCache - Resource cache interface for clanDisplay resources</li>
    </ul>

	<h2>Thread run-loop</h2>

	<ul>
	<li>clan::RunLoop - Application loop helper</li>
	<li>clan::Timer - Timer class that invokes a callback on a specified interval</li>
	</ul>

*/
