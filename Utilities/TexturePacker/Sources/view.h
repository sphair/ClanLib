#pragma once

class MainWindow;

class View : public CL_GUIComponent
{
public:
	View(CL_GUIComponent *parent, MainWindow *mainwindow, const CL_String &title);
	virtual ~View();

	MainWindow *get_mainwindow() const { return mainwindow; }

	const CL_String &get_title() const { return title; }
	void set_title(const CL_String &title) { this->title = title; }

	bool is_closable() const { return closable; }
	void set_closable(bool closable) { this->closable = closable; }

private:
	MainWindow *mainwindow;
	CL_String title;

	bool closable;
};
