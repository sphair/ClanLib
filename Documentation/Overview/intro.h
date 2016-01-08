
/*! \page Intro Getting started

<p>ClanLib is a cross platform toolkit library with a primary focus on game creation. The
library is Open Source and free for commercial use, under the <a href="license.html">ClanLib License</a>.</p>
		
<p>The library is split into sub-libraries, each providing different classes
of functionality. The library is built so that the modules do not put more external
dependencies than they absolutely have to. With the exception of clanCore, you can pretty much
cherry pick which libraries and classes you find useful.</p>

<h2>Getting started with the build environment</h2>

<p>The first step is to ensure that your build environment is set up and ready. We have
the following guides to help you with this:</p>

<ul>
	<li><a href="/build-environment-windows-msvc.html">Visual C++ on Windows</a></li>
	<li><a href="/build-environment-linux-gcc.html">GNU C++ on Linux systems</a></li>
</ul>
		
<p>After you have set up your build environment and successfully built ClanLib, you are ready
to start creating your applications.</p>
		
<h2>Creating your applications</h2>
		
<p>ClanLib supports that you use the normal platform specific main functions, such as main or
WinMain, but a more common approach is to use the cross platform application main provided by
the clanApplication library. We have a few examples here on how simple "Hello World" ClanLib
applications may look like. We advise looking at Examples/Display/Basic2D found in the SDK package</p>
		
<h2>Getting help</h2>
		
<p>If you are having any problems with ClanLib or find some documentation to be limited about
a certain topic you can always ask in our forums. These documents might help you as well:</p>
		
<ul>
	<li><a href="/object-types.html">ClanLib class object types overview</a></li>
	<li><a href="/debugging-msvc.html">Debugging with MSVC</a></li>
</ul>

*/
