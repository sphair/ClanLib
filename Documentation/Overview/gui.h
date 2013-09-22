
/*! \page GUI User Interface Framework Overview

	<p>Introduction to the GUI module: \ref GUIFramework </p>

	<h2>Base Framework</h2>

	<ul>
		<li>clan::GUIManager - Main class running the user interface</li>
		<li>clan::GUIComponent - View covering an area of the user interface</li>
		<li>clan::GUITopLevelDescription - Typedef for DisplayWindowDescription. Describes the frame of the top-level component</li>
		<li>clan::GUIThemePart - Theme part represents a pseudo element in CSS</li>
		<li>clan::AcceleratorTable, clan::AcceleratorKey - Keyboard shortcuts</li>
	</ul>

	<h2>UI Messages</h2>

	<ul>
		<li>clan::GUIMessage - Base class for UI messages being sent</li>
		<li>clan::GUIMessage_ActivationChange - Window activation change notification</li>
		<li>clan::GUIMessage_Close - Window close message</li>
		<li>clan::GUIMessage_FocusChange - Focus change notification</li>
		<li>clan::GUIMessage_Input - Input event</li>
		<li>clan::GUIMessage_Pointer - Mouse pointer enter/exit change notification</li>
		<li>clan::GUIMessage_Resize - Window resize notification</li>
	</ul>

	<h2>Window Managers</h2>

	<ul>
		<li>clan::GUIWindowManager - Base class for window managers</li>
		<li>clan::GUIWindowManagerSystem - System (OS) window manager. Each top-level component becomes its own window</li>
		<li>clan::GUIWindowManagerDirect - Draws the entire UI to a single 2D canvas</li>
		<li>clan::GUIWindowManagerTexture - Draws each top-level component to its own texture</li>
		<li>clan::GUIWindowManagerProvider - Interface for implementing custom UI window managers</li>
	</ul>

	<h2>Deprecated UI Layout</h2>

	<ul>
		<li>clan::GUILayout, clan::GUILayoutProvider - Interface for creating layout managers</li>
		<li>clan::GUILayoutCorners - Layout components using top, left, right, bottom anchoring</li>
	</ul>

	<h2>Standard Components</h2>

	<ul>
		<li>clan::CheckBox - Check box button</li>
		<li>clan::ComboBox - Menu drop down</li>
		<li>clan::DragBox - Move component by dragging</li>
		<li>clan::BrowseFolderDialog - Displays a select folder dialog</li>
		<li>clan::Frame - Frame decoration</li>
		<li>clan::GroupBox - Group box</li>
		<li>clan::ImageView - Shows an image</li>
		<li>clan::Label - Text label</li>
		<li>clan::LineEdit - One line text input</li>
		<li>clan::ListView - List view component</li>
		<li>clan::MenuBar - Menu bar</li>
		<li>clan::OpenFileDialog - Displays an open file dialog</li>
		<li>clan::PopupMenu - Pop-up menu</li>
		<li>clan::ProgressBar - Progress indicator bar</li>
		<li>clan::PushButton - Button</li>
		<li>clan::RadioButton - Radio button</li>
		<li>clan::Ribbon - Ribbon</li>
		<li>clan::SaveFileDialog - Displays a save file dialog</li>
		<li>clan::ScrollBar - Scrolling bar</li>
		<li>clan::Slider - Track bar</li>
		<li>clan::Spin - Spinner</li>
		<li>clan::StatusBar - Status bar</li>
		<li>clan::Tab - Tab bar with pages</li>
		<li>clan::TextEdit - Multi line text editor</li>
		<li>clan::ToolBar - Tool bar</li>
		<li>clan::ToolTip - Shows a tool tip pop-up</li>
		<li>clan::Window - Top-level component with a standard window look</li>
	</ul>

*/
