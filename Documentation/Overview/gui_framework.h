
/*! \page GUIFramework GUI Framework

MainDocs:GUI Framework

From ClanLib Game SDK
 

ClanLib features a complete graphical user interface (GUI) toolkit. A GUI toolkit normally consists of a general GUI component framework, and then some standard components generally needed by most applications. In clanGUI, the different user interface elements are split into components, handled by CL_GUIComponent, where each component covers a specific part of the screen and performs a specific role. The components are arranged in a tree-like structure, which for a simple application may look like this: 



•CL_Window window("Hello GUI", &gui_manager) ?CL_ListView listview(&window) ?CL_Scrollbar vertical_scroll(&listview) 
?CL_Scrollbar horizontal_scroll(&listview) 

?CL_Button ok_button("Ok", &window) 
?CL_Button cancel_button("Cancel", &window) 






 Besides CL_GUIComponent, the other base components of the GUI framework are: 



•CL_GUIManager, the class running the GUIframework. 
•CL_GUITheme, theme engine interface used by the theme parts. 
•CL_GUIThemePart, the theming interface used by components to apply the current theme. 
•CL_GUIDefaultTheme, the default theme engine which uses CSS properties to describe theme parts. 
•CL_GUIWindowManager, the interface which bridges the components to the system hosting the GUI. 
•CL_GUIWindowManagerSystem, window manager that places the top-level components in operating system windows. 
•CL_GUIWindowManagerTexture, window manager that places top-level components in textures. 
•CL_GUIMessage, base message class for all messages being sent in the GUI. 






Contents
•1 The GUI Manager
•2 Components
•3 Component Messages
•4 Using clanGUI Compared to clanDisplay Rendering
 

The GUI Manager

CL_GUIManager manages the environment in which GUI components run. The functions of the GUI manager can generally be split into these groups: 



• Handling of the GUI message queue. 
• Root of all top-level components. 
• Container for the basic GUI subsystems: window manager, resource manager, CSS document and so on. 
• Provides a default message loop. 


 At the heart of the GUI system is the message queue and the message pump. All communication between the window manager and the GUI is done by queueing messages onto the message queue, which then are dispatched to the relevant GUI component. For example, if the user hits a key, this generates a CL_GUIMessage_Input message, which is sent to the currently focused component. 

There are many situations where the application will want to filter messages, change where they are going, or simply perform other operations when there are no message to be dealt with. To maximize control over what happens between messages, the application can read and dispatch the messages itself, or it can call the default message loop via  CL_GUIManager::exec(). 

But enough talking, here's how you create a GUI and pump the message queue: 


 
// Create GUI environment:
CL_ResourceManager resources(..);
CL_GUIDefaultTheme theme;
CL_CSSDocument css_document(..);
CL_GUIWindowManagerSystem window_manager;
CL_GUIManager gui_manager;
gui_manager.set_resources(resources);
gui_manager.set_css_document(css_document);
gui_manager.set_window_manager(&window_manager);
 
// Create components:
CL_GUITopLevelDescription desc;
desc.set_title("ClanLib GUI");
CL_Window window(CL_Rect(0,0, 640, 480), &gui_manager, desc);
CL_PushButton button1(&window);
button1.set_geometry(CL_Rect(100, 100, 200, 120));
button1.set_text("Okay!");
button1.func_clicked().set(&on_button1_clicked, &button1);
 
// Pump the message queue:
CL_AcceleratorTable accelerator_table;
while (!gui_manager.get_exit_flag())
{
	CL_GUIMessage message = gui_manager.get_message();
	gui_manager.dispatch_message(message);
	if (!message.is_consumed())
		accelerator_table.process_message(message);
}
 
void on_button1_clicked(CL_PushButton *button)
{
	button->exit_with_code(0);
}

The message pump above does exactly the same as CL_GUIManager::exec, so if you do not need any additional processing, you can just call that one. 

Components

As mentioned in the introduction, the screen is split into components, which are rectangular areas where each component draws itself and processes input. Components can be split into two types: top-level components and child components. 

The top-level components are constructed using a CL_GUITopLevelDescription class, which is passed on to the window manager for further interpretation. The system window manager creates a CL_DisplayWindow for each top-level component, where the information in the description class is used to give that window a title, icon, border and other styling properties. The texture window manager creates a new texture for the window and ignores most of the other properties of the description. 

Child components are constructed by only passing a parent component to the constructor of CL_GUIComponent. The window manager is not aware of child components and does not require any special extra data to construct them. A child component is initially created at (0,0) with a (0,0) size, which means that you will have to explicitly set the geometry of the component after you created it. 

Component Messages

The GUI system communicates with the component using GUI messages. They are dispatched to the component, which means that CL_GUIManager::dispatch_message() invokes the callback given by CL_GUIComponent::func_process_message(). It can receive messages of the following types: 



•CL_GUIMessage_Close, sent by window manager to top-level components when the user clicks the Close button on the window. 
•CL_GUIMessage_FocusChange, sent by the GUI manager to the component losing keyboard focus and the component gaining keyboard focus. 
•CL_GUIMessage_Input, sent by to the component receiving some user input. Input can be mouse movements, mouse clicking, keyboard key presses, etc. 
•CL_GUIMessage_Resize, sent when a component's geometry changes. 
•CL_GUIMessage_Pointer, sent when the mouse enters or leaves the area covered by the component. 




When the GUI system needs the component to paint itself, it will invoke CL_GUIComponent::func_render(). Likewise, CL_GUIComponent has other callbacks you can hook into to react to certain events. 

Example of a component that changes color if mouse is hovering above it: 


 
class MyComponent : public CL_GUIComponent {
public:
	MyComponent(CL_GUIComponent *parent) : CL_GUIComponent(parent), above(false)
	{
		set_type_name("MyComponent");
		func_render().set(this, &MyComponent::on_render);
		func_process_message().set(this, &MyComponent::on_process_message);
	}
 
private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
	{
		CL_Rect client(CL_Point(0,0), get_geometry().get_size());
		CL_Draw::fill(
			gc,
			client,
			this->above ? CL_Colorf::blue : CL_Colorf::red);
	}
 
	void on_process_message(CL_GUIMessage &message)
	{
		if (message.is_type(CL_GUIMessage_Pointer::get_type_name()))
		{
			CL_GUIMessage_Pointer msg_pointer = message;
			switch (msg_pointer.get_pointer_type())
			{
			case CL_GUIMessage_Pointer::pointer_enter:
				above = true;
				break;
			case CL_GUIMessage_Pointer::pointer_leave:
				above = false;
				break;
			}
			// Cause repaint of component:
			request_repaint();
 
		}
	}
	bool above;
};

Using clanGUI Compared to clanDisplay Rendering

One of the things that confuse people the most when it comes to the GUI is how the screen is updated. In particular, if you are attempting to port an application using clanDisplay to clanGUI, your application typically does something along these lines: 


 
CL_GraphicContext gc = displaywindow.get_gc();
while (!exit)
{
	update_game_logic();
	render_game(gc);
	displaywindow.flip();
	CL_KeepAlive::process();
}

A common mistake is to assume that if you want to render the GUI, you can simply add a call to gui_manager.exec() just before the displaywindow.flip() line. This will not work. The gui_manager.exec() function will not exit until something in the GUI calls CL_GUIComponent::exit_with_code(), causing only the GUI to be rendered and your game will disappear and hang. 

The best way to fix this problem is to get rid of your own while(!exit) loop and create your game screen as a CL_GUIComponent. Since your game probably wants to constantly repaint itself, you can use the CL_GUIComponent::set_constant_repaint() function to achieve this. Also, depending on how often your game logic is to be run, you can either place this in the render callback function or use a CL_Timer to call it at desired intervals. 

Here's a simple example how how a game component might look like: 


 
class GameWindow : public CL_Window
{
public:
	GameWindow(CL_GUIManager *manager, const CL_DisplayWindowDescription &desc)
	: CL_Window(manager, desc)
	{
		func_render().set(this, &GameWindow::on_render);
		set_constant_repaint(true);
 
		button = new CL_PushButton(this);
		button->set_geometry(CL_Rect(10, 10, 150, 25));
		button->set_text("A button on top of my game!");
	}
 
private:	
	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
	{
		update_game_logic();
		render_game(gc);
	}
 
	CL_PushButton *button;


This document describes the behavior of various parts of clanGUI into further detail. It is mostly intended for developers working directly on clanGUI, acting as a specification for how things are meant to work (as opposed to how things might actually work due to bugs). 

If you see anything in clanGUI not working as described here, it is a bug in the framework. 



Contents
•1 Input messages are processed as follows
•2 Focus switching details
•3 Dialog input handler details
•4 Geometry details
 

Input messages are processed as follows
• Input message is delivered to the window manager from the OS 
• Window manager attributes the message to the active window. For the system WM this is always the window the WM_INPUT message was designated, but for the texture WM it does its internal tracking of which toplevel window is the currently active window 
• The GUI manager is notified by the WM about a new message attributed to a specific toplevel window. Each toplevel window has a single focused component that gets the first chance to process this message. The message is delivered to this component 
• If the focus component does not consume the message, the message is routed to the parent window. This continues until a component consumes the message or it reaches the toplevel component 
• If the message is still not consumed after the toplevel component, the accelerator table gets a chance to process the message 
• If the accelerator table does not consume the message, the dialog input message handler processes the message. This is where things like tabbing between controls and the default action of enter is handled 




Focus switching details
• Tab order is controlled by the component tree and the FocusPolicy of the individual components. The focus moving order is depth-first iteration of the component tree. 
• If a child component has focus and is about to be made hidden, CL_GUIComponent::focus_next() is called before making it invisible 
• If the child component being made hidden is still the focused component after focus_next(), the focus component is set to null 
• If the current focus component is null, and a component that accepts focus is made visible, the shown component will get the focus. Otherwise the focus will remain null 
• If <tab>, <alt-tab> or arrow keys reach the dialog input handler, the messages are translated into focus_next() or focus_previous() calls on the currently focused component. 
• If FocusPolicy of the currently focused component is focus_group, all components with the same group-name -attribute will be skipped over 
• If FocusPolicy of the component to be focused is focus_group, the focus is set on the selected component in the group. If no component in the group is set as selected, normal focus changing rules apply. 
• Focus indication (dashed line surrounding text) is drawn on the focused component only after the keyboard has been used to navigate the dialog. Otherwise it is not drawn. 
• Focusable components receive focus on mouse clicks after keyboard navigation has been used. 




Dialog input handler details
• A toplevel window has one single default action component. If <enter> or <space> reaches the dialog input handler, the messages are sent to the default action component 
• The defaulted component is painted with the default state when the following conditions are met: 
• The top level window has activation. 
• The focused component does not consume the enter key. 



• Any component that consumes the enter key may paint itself defaulted when focused. 




Geometry details
• The GUI consists of two types of components: top-level and child components 
• The geometry rectangle of a top-level component is the client area of the windowing system window, but where the origin is the upper left corner of the primary screen (this differs from GetClientRect() where the origin is the client area itself) 
• The geometry rectangle of a child component is its position relative to its parent component 
• Top-level components also have a window frame geometry, which include the caption and border frames of the window. This geometry is retrieved and set using get_window_geometry() and set_window_geometry(). The origin of the rect is also the upper left corner of the primary screen. 
• In Windows, the workspace is the area defined by GetWindowPlacement() and SetWindowPlacement(). This is the screen area that excludes the task bar and is the only 'safe' geometry to be stored and restored in Windows between sessions. When we add support for the workspace area, those functions should be named get_window_placement() and set_window_placement() and should not replace any of the other geometry functions 

*/
