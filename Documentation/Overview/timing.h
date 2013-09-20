
/*! \page Timing Timing

** Note this page should also include the new clan::GameTime class

<h2>Refresh Rate </h2>

The refresh rate (sometimes known as vertical sync) is the number of times in a second that display hardware draws the data. 
<br/>
Historicially in Americia the refresh rate for TV's was 60hz. In Europe it was 50hz. This made is easy for game to judge timing. 
<br/>
However, PC monitors and modern televisions have a varied refresh rate. Usually between 60hz and 80hz, but refresh rates of 120hz are now available. 
<br/>

\ref clan::Display::flip() 

You only want to draw to the screen after you have finished drawing, else a partial drawn image is shown (known as screen tearing) 
<br/>
The display hardware contains 2 screens in memory. One that is being written to and one that is being displayed. For reference, this is known by OpenGL as the GL_DRAW_FRAMEBUFFER and clan::READ_FRAMEBUFFER. 
<br/>
To swap these screens you need to call flip() 
<br/>
This is known as "double buffering" or "page switching" 
<br/>
It is a waste to draw, 2 screens during 1 display refresh. Ideally you should wait after the first screen swap until the next display refresh. 
<br/>
Fortunately, OpenGL (and DirectX) helps you do this. (Many years ago, you had to wait for the vertical blanking interrupt instead) 
<br/>
There is an integer parameter used in flip() 
<br/>
"-1" (Default). The screen is drawn using the display card default setting, this may be "do not wait for the next display refresh" or "wait for the next display refresh". 
<br/>
"0" .The screen is drawn without waiting for the next display refresh 
<br/>
"1" . The screen is drawn after the next display refresh 
<br/>
"n" . The screen is drawn after the nth display refresh 
<br/>
You must not assume that the application will wait for the next display refresh even if you asked for it. Some display drivers contain an option for the user to override the application setting. With NVidia it is the "Vertical Sync" option. 
<br/>
If you want to test your application speed, use flip(0). For normal use, use flip(1) 
<br/>
Do not use flip() to control the game cycle time 
<br/>

\ref clan::DisplayWindowDescription 
<br/>
This contains: set_refresh_rate() 
<br/>
It is not used by the OpenGL target. It used to in the past, but it had to be removed because it caused a number of problems. 
<br/>
It is used by the SWRender target, to make it easier for the programmer to switch between ClanLib display targets. It calls \ref clan::System::sleep() to give the application the appearance that it is being synchronized to the display refresh. If it is not defined, it defaults to 60hz. 
<br/>
To recap, it is only used on clanSWRender when used with flip(1) 
<br/>
Also this class contains: set_swap_interval() 
<br/>
This controls the initial flip() swap interval setting, changing the default state of flip() 
<br/>
I.E. It is only used when you call flip(-1) or flip(). 
<br/>
This function is useful for controlling the display refresh when using clanGUI 
<br/>
\ref clan::System::sleep() 
<br/>
This sleeps for a number of milliseconds, reducing cpu usage of your application. 
<br/>
Internally, this function causes a thread to relinquish the remainder of its time slice. The system clock "ticks" at a constant rate. If the time is less than the resolution of the system clock, the thread may sleep for less than the specified length of time. If time is greater than one tick but less than two, the wait can be anywhere between one and two ticks, and so on 
<br/>
If you specify 0 milliseconds, the thread will relinquish the remainder of its time slice, thus it will not return immediately. 
<br/>
Note, on the windows platform, you can adjust the accuracy of sleep() using timeBeginPeriod - http://msdn.microsoft.com/en-us/libr...24(VS.85).aspx 
<br/>
ClanLib currently does not do this. 
<br/>
\ref clan::System::pause() 
<br/>
This is like \ref clan::System::sleep(), except it may perform a spinlock to pause the application. This was required because sometimes on the windows platform sleep() may not return at exact intervals. 
<br/>
Avoid using this, unless you require more exact timing. Other applications on the computer may run slower. 
<br/>
\ref clan::KeepAlive::process 
<br/>
This processes the operating system messages (keyboard, mouse etc) 
<br/>
The parameter to this function is the time out (in milliseconds) 
<br/>
For games, you always want to keep this at 0. 
<br/>
If you set it to -1 (Wait unless message is found), your game main loop will only operate when the mouse is moved (an interesting but useless effect). 
<br/>
<h2>Main Loop Timing </h2>

The ideal code will be: 

Code: ( See Examples/Display/Timing ) 

\code

	clan::GameTime game_time(60, 60);
	
	// Run until someone presses escape
	while (!quit)
	{
		game_time.update();

		canvas.clear(clan::Colorf(0.0f,0.0f,0.0f));

		draw_graphics(canvas, game_time.get_time_elapsed());

		window.flip();

		clan::KeepAlive::process(0);
	}

\endcode


*/
