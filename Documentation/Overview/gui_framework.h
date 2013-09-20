
/*! \page GUIFramework GUI Framework

ClanLib features a complete graphical user interface (GUI) toolkit. A GUI toolkit normally consists of a general GUI component framework, and then some standard components generally needed by most applications. In clanGUI, the different user interface elements are split into components, handled by clan::GUIComponent, where each component covers a specific part of the screen and performs a specific role. The components are arranged in a tree-like structure, which for a simple application may look like this: 

\code
clan::Window window("Hello GUI", &gui_manager) 
clan::ListView listview(&window) 
clan::Scrollbar vertical_scroll(&listview) 
clan::Scrollbar horizontal_scroll(&listview) 

clan::Button ok_button("Ok", &window) 
clan::Button cancel_button("Cancel", &window) 
\endcode

Besides \ref clan::GUIComponent, the other base components of the GUI framework are: 
<br/>
\ref clan::GUIManager, the class running the GUIframework. 
<br/>
\ref clan::GUIDefaultTheme, the default theme engine which uses CSS properties to describe theme parts. 
<br/>
\ref clan::GUIWindowManager, the interface which bridges the components to the system hosting the GUI. 
<br/>
\ref clan::GUIWindowManagerSystem, window manager that places the top-level components in operating system windows. 
<br/>
\ref clan::GUIWindowManagerTexture, window manager that places top-level components in textures. 
<br/>
\ref clan::GUIMessage, base message class for all messages being sent in the GUI. 
<br/>


<h2>The GUI Manager</h2>

\ref clan::GUIManager manages the environment in which GUI components run. The functions of the GUI manager can generally be split into these groups: 
<br/>

<ul><li>Handling of the GUI message queue. </li></ul>
<ul><li>Root of all top-level components. </li></ul>
<ul><li>Container for the basic GUI subsystems: window manager, resource manager, CSS document and so on. </li></ul>
<ul><li>Provides a default message loop. </li></ul>

<br/>

At the heart of the GUI system is the message queue and the message pump. All communication between the window manager and the GUI is done by queueing messages onto the message queue, which then are dispatched to the relevant GUI component. For example, if the user hits a key, this generates a clan::GUIMessage_Input message, which is sent to the currently focused component. 
<br/>

There are many situations where the application will want to filter messages, change where they are going, or simply perform other operations when there are no message to be dealt with. To maximize control over what happens between messages, the application can read and dispatch the messages itself, or it can call the default message loop via  clan::GUIManager::exec(). 
<br/>

But enough talking, here's how you create a GUI and pump the message queue: 
<br/>


\code 
// Create GUI environment:
clan::ResourceManager resources(..);
clan::CSSDocument css_document(..);
clan::GUIWindowManagerSystem window_manager;
clan::GUIManager gui_manager;
gui_manager.set_resources(resources);
gui_manager.set_css_document(css_document);
gui_manager.set_window_manager(&window_manager);
 
// Create components:
clan::GUITopLevelDescription desc;
desc.set_title("ClanLib GUI");
clan::Window window(clan::Rect(0,0, 640, 480), &gui_manager, desc);
clan::PushButton button1(&window);
button1.set_geometry(clan::Rect(100, 100, 200, 120));
button1.set_text("Okay!");
button1.func_clicked().set(&on_button1_clicked, &button1);
 
// Pump the message queue:
clan::AcceleratorTable accelerator_table;
while (!gui_manager.get_exit_flag())
{
	clan::GUIMessage message = gui_manager.get_message();
	gui_manager.dispatch_message(message);
	if (!message.is_consumed())
		accelerator_table.process_message(message);
}
 
void on_button1_clicked(clan::PushButton *button)
{
	button->exit_with_code(0);
}
\endcode

The message pump above does exactly the same as \ref clan::GUIManager::exec, so if you do not need any additional processing, you can just call that one. 

<h2>Components</h2>

As mentioned in the introduction, the screen is split into components, which are rectangular areas where each component draws itself and processes input. Components can be split into two types: top-level components and child components. 
<br/>

The top-level components are constructed using a \ref clan::GUITopLevelDescription class, which is passed on to the window manager for further interpretation. The system window manager creates a \ref clan::DisplayWindow for each top-level component, where the information in the description class is used to give that window a title, icon, border and other styling properties. The texture window manager creates a new texture for the window and ignores most of the other properties of the description. 
<br/>

Child components are constructed by only passing a parent component to the constructor of \ref lan::GUIComponent. The window manager is not aware of child components and does not require any special extra data to construct them. A child component is initially created at (0,0) with a (0,0) size, which means that you will have to explicitly set the geometry of the component after you created it. 
<br/>

<h2>Component Messages</h2>

The GUI system communicates with the component using GUI messages. They are dispatched to the component, which means that \ref clan::GUIManager::dispatch_message() invokes the callback given by \ref clan::GUIComponent::func_process_message(). It can receive messages of the following types: 

<ul><li>\ref clan::GUIMessage_Close, sent by window manager to top-level components when the user clicks the Close button on the window.  </li></ul>
<ul><li>\ref clan::GUIMessage_FocusChange, sent by the GUI manager to the component losing keyboard focus and the component gaining keyboard focus. </li></ul> 
<ul><li>\ref clan::GUIMessage_Input, sent by to the component receiving some user input. Input can be mouse movements, mouse clicking, keyboard key presses, etc.  </li></ul>
<ul><li>\ref clan::GUIMessage_Resize, sent when a component's geometry changes. </li></ul> 
<ul><li>\ref clan::GUIMessage_Pointer, sent when the mouse enters or leaves the area covered by the component.  </li></ul>

<br/>

When the GUI system needs the component to paint itself, it will invoke \ref clan::GUIComponent::func_render(). Likewise, \ref clan::GUIComponent has other callbacks you can hook into to react to certain events. 
<br/>

Example of a component that changes color if mouse is hovering above it: 
<br/>


\code 
class MyComponent : public clan::GUIComponent {
public:
	MyComponent(clan::GUIComponent *parent) : clan::GUIComponent(parent), above(false)
	{
		set_type_name("MyComponent");
		func_render().set(this, &MyComponent::on_render);
		func_process_message().set(this, &MyComponent::on_process_message);
	}
 
private:
	void on_render(clan::GraphicContext &gc, const clan::Rect &clip_rect)
	{
		clan::Rect client(clan::Point(0,0), get_geometry().get_size());
		clan::Draw::fill(
			gc,
			client,
			this->above ? clan::Colorf::blue : clan::Colorf::red);
	}
 
	void on_process_message(clan::GUIMessage &message)
	{
		if (message.is_type(clan::GUIMessage_Pointer::get_type_name()))
		{
			clan::GUIMessage_Pointer msg_pointer = message;
			switch (msg_pointer.get_pointer_type())
			{
			case clan::GUIMessage_Pointer::pointer_enter:
				above = true;
				break;
			case clan::GUIMessage_Pointer::pointer_leave:
				above = false;
				break;
			}
			// Cause repaint of component:
			request_repaint();
 
		}
	}
	bool above;
};
\endcode

<h2>Using clanGUI Compared to clanDisplay Rendering</h2>

One of the things that confuse people the most when it comes to the GUI is how the screen is updated. In particular, if you are attempting to port an application using clanDisplay to clanGUI, your application typically does something along these lines: 


\code 
clan::GraphicContext gc = displaywindow.get_gc();
while (!exit)
{
	update_game_logic();
	render_game(gc);
	displaywindow.flip();
	clan::KeepAlive::process();
}
\endcode

A common mistake is to assume that if you want to render the GUI, you can simply add a call to gui_manager.exec() just before the displaywindow.flip() line. This will not work. The gui_manager.exec() function will not exit until something in the GUI calls clan::GUIComponent::exit_with_code(), causing only the GUI to be rendered and your game will disappear and hang. 
<br/>
The best way to fix this problem is to get rid of your own while(!exit) loop and create your game screen as a \ref clan::GUIComponent. Since your game probably wants to constantly repaint itself, you can use the \ref clan::GUIComponent::set_constant_repaint() function to achieve this. Also, depending on how often your game logic is to be run, you can either place this in the render callback function or use a \ref clan::Timer to call it at desired intervals. 

Here's a simple example how how a game component might look like: 

\code
class GameWindow : public clan::Window
{
public:
	GameWindow(clan::GUIManager *manager, const clan::DisplayWindowDescription &desc)
	: clan::Window(manager, desc)
	{
		func_render().set(this, &GameWindow::on_render);
		set_constant_repaint(true);
 
		button = new clan::PushButton(this);
		button->set_geometry(clan::Rect(10, 10, 150, 25));
		button->set_text("A button on top of my game!");
	}
 
private:	
	void on_render(clan::GraphicContext &gc, const clan::Rect &clip_rect)
	{
		update_game_logic();
		render_game(gc);
	}
 
	clan::PushButton *button;
\endcode

This document describes the behavior of various parts of clanGUI into further detail. It is mostly intended for developers working directly on clanGUI, acting as a specification for how things are meant to work (as opposed to how things might actually work due to bugs). 
<br/>


<h2>Input messages are processed as follows</h2>

<ul><li>Input message is delivered to the window manager from the OS </li></ul>
<ul><li>Window manager attributes the message to the active window. For the system WM this is always the window the WM_INPUT message was designated, but for the texture WM it does its internal tracking of which toplevel window is the currently active window </li></ul>
<ul><li>The GUI manager is notified by the WM about a new message attributed to a specific toplevel window. Each toplevel window has a single focused component that gets the first chance to process this message. The message is delivered to this component</li></ul> 
<ul><li>If the focus component does not consume the message, the message is routed to the parent window. This continues until a component consumes the message or it reaches the toplevel component</li></ul> 
<ul><li>If the message is still not consumed after the toplevel component, the accelerator table gets a chance to process the message</li></ul> 
<ul><li>If the accelerator table does not consume the message, the dialog input message handler processes the message. This is where things like tabbing between controls and the default action of enter is handled </li></ul>
<br/>

<h2>Focus switching details</h2>
<ul><li>Tab order is controlled by the component tree and the FocusPolicy of the individual components. The focus moving order is depth-first iteration of the component tree.</li></ul> 
<ul><li>If a child component has focus and is about to be made hidden, \ref clan::GUIComponent::focus_next() is called before making it invisible</li></ul> 
<ul><li>If the child component being made hidden is still the focused component after focus_next(), the focus component is set to null</li></ul> 
<ul><li>If the current focus component is null, and a component that accepts focus is made visible, the shown component will get the focus. Otherwise the focus will remain null </li></ul>
<ul><li>If \<tab\>, \<alt-tab\> or arrow keys reach the dialog input handler, the messages are translated into focus_next() or focus_previous() calls on the currently focused component.</li></ul> 
<ul><li>If FocusPolicy of the currently focused component is focus_group, all components with the same group-name -attribute will be skipped over</li></ul> 
<ul><li>If FocusPolicy of the component to be focused is focus_group, the focus is set on the selected component in the group. If no component in the group is set as selected, normal focus changing rules apply. </li></ul>
<ul><li>Focus indication (dashed line surrounding text) is drawn on the focused component only after the keyboard has been used to navigate the dialog. Otherwise it is not drawn.</li></ul> 
<ul><li>Focusable components receive focus on mouse clicks after keyboard navigation has been used.</li></ul> 


<h2>Dialog input handler details</h2>
<ul><li>A toplevel window has one single default action component. If \<enter\> or \<space\> reaches the dialog input handler, the messages are sent to the default action component </li></ul> 
<ul><li>The defaulted component is painted with the default state when the following conditions are met: </li></ul> 
<ul><li>The top level window has activation. </li></ul> 
<ul><li>The focused component does not consume the enter key. </li></ul> 
<ul><li>Any component that consumes the enter key may paint itself defaulted when focused. </li></ul> 


<h2>Geometry details</h2>
<ul><li>The GUI consists of two types of components: top-level and child components</li></ul>  
<ul><li>The geometry rectangle of a top-level component is the client area of the windowing system window, but where the origin is the upper left corner of the primary screen (this differs from GetClientRect() where the origin is the client area itself) </li></ul> 
<ul><li>The geometry rectangle of a child component is its position relative to its parent component </li></ul> 
<ul><li>Top-level components also have a window frame geometry, which include the caption and border frames of the window. This geometry is retrieved and set using get_window_geometry() and set_window_geometry(). The origin of the rect is also the upper left corner of the primary screen. </li></ul> 
<ul><li>In Windows, the workspace is the area defined by GetWindowPlacement() and SetWindowPlacement(). This is the screen area that excludes the task bar and is the only 'safe' geometry to be stored and restored in Windows between sessions. When we add support for the workspace area, those functions should be named get_window_placement() and set_window_placement() and should not replace any of the other geometry functions </li></ul> 

*/
