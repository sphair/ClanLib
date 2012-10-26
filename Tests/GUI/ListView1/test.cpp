#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h> 
#include <ClanLib/gl.h> 
using namespace clan;

class App
{
public:
	int main(const std::vector<std::string> &args)
	{
		ConsoleWindow console("Console");
 
		try
		{
			ResourceManager resources;
			resources.load("../../../Resources/GUIThemeLuna/resources.xml");

			GUIManager gui;

			GUIWindowManagerSystem wm;
			gui.set_window_manager(wm); 

			GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document("../../../Resources/GUIThemeLuna/theme.css");
 
			DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("ListView test app.");
			win_desc.set_position(Rect(200, 200, 840, 680), false);
			Window window(&gui, win_desc);			
			window.func_close().set(this, &App::on_close, &window); 
     	  
			GraphicContext gc = window.get_gc();

			sprite = Sprite(gc, "../../../Resources/GUIThemeLuna/Images/icon_mycomputer.png");
			sprite2 = Sprite(gc, "../../../Resources/GUIThemeLuna/Images/icon_folder_32x32.png");
			sprite_olay = Sprite(gc, "../../../Resources/GUIThemeLuna/Images/overlay_96x96.png");

//			PixelBuffer pbuf = PNGProvider::load("../Images/icon_folder.png");
			//			PixelBuffer pbuf2 = PNGProvider::load("../Images/icon_folder_32x32.png");

			ListView lv1(&window);
			lv1.set_geometry(Rect(10, 10, 630, 210));
			lv1.set_display_mode(listview_mode_details);
			populate_listview(lv1);

			print_tree(lv1.get_document_item());
			Console::write_line("num items: %1", lv1.get_document_item().get_child_count(true,false));

			ListView lv2(&window);
			lv2.set_geometry(Rect(10, 220, 630, 470));
			lv2.set_display_mode(listview_mode_thumbnails);
			populate_listview(lv2);

			gc.clear();

			gui.exec();
		} 
		catch (Exception e)
		{
 			Console::write_line(e.message);
			console.display_close_message();
		} 
 
		return 0;
	} 

	void populate_listview(ListView &lv)
	{
		ListViewItem doc_item = lv.get_document_item(); 

		ListViewHeader *lv_header = lv.get_header();  
		lv_header->append(lv_header->create_column("col1_id", "col1")).set_width(130);
		lv_header->append(lv_header->create_column("col2_id", "col2")).set_width(80);
		lv_header->append(lv_header->create_column("col3_id", "col3")).set_width(80);

		ListViewIconList ico_list = lv.get_icon_list();

//		Colorf sel_color("#316ac5");
//		ico_list.set_selected_color(Colorf::cornflowerblue);

		ListViewIcon icon;
		icon.set_sprite(sprite, listview_mode_details);
//		icon.set_pixel_buffer(pbuf2, listview_mode_icons);
		icon.set_sprite(sprite2, listview_mode_icons);
		icon.set_sprite(sprite2, listview_mode_thumbnails);

		ListViewIcon olay;
		olay.set_sprite(sprite_olay, listview_mode_thumbnails);
		olay.set_offset(Point(0, 96-sprite_olay.get_height()), listview_mode_thumbnails);
		olay.set_scalable(false);
		
		ico_list.set_icon(1, icon);
		ico_list.set_icon(1001, olay);

		ListViewItem i1 = lv.create_item();
		{
			i1.set_column_text("col1_id", "i1");
			i1.set_column_text("col2_id", "col2 text!");
			i1.set_editable(true);
			i1.set_icon(1);
			i1.add_overlay_icon(1001);
			i1.set_open(false);
			doc_item.append_child(i1);

			ListViewItem i1c1 = lv.create_item();  
			{ 
				i1c1.set_column_text("col1_id", "i1c1");
				i1c1.set_column_text("col2_id", "BOOYAA!");
				i1c1.set_icon(1);
				i1.append_child(i1c1);
			}
		}

		ListViewItem i2 = lv.create_item();
		{
			i2.set_column_text("col1_id", "i2");
			i2.set_column_text("col2_id", "bar2");
			i2.set_column_text("col3_id", "foobar!");
			i2.set_icon(1);
			i2.set_open(true);

			ListViewItem i2c1 = lv.create_item();  
			{
				i2c1.set_column_text("col1_id", "i2c1"); 
				i2c1.set_column_text("col2_id", "BOOYAA!");
				i2c1.set_icon(1);
			}
			i2.append_child(i2c1); 

			ListViewItem i2c2 = lv.create_item();
			{
 				i2c2.set_column_text("col1_id", "i2c2");
				i2c2.set_column_text("col2_id", "BOOYAA!");
				i2c2.set_icon(1);
				i2c2.set_open(true); 

				ListViewItem i2c2c1 = lv.create_item();
				{
					i2c2c1.set_column_text("col1_id", "i2c2c1"); 
					i2c2c1.set_column_text("col2_id", "BOOYAA!");
					i2c2c1.set_icon(1);
				}
				i2c2.append_child(i2c2c1); 
			}
			i2.append_child(i2c2);
		}
		doc_item.append_child(i2);


		for (int j = 0;j < 15; j++)
		{  
			ListViewItem i = lv.create_item();
			i.set_editable(true);
			i.set_column_text("col1_id", string_format("item %1", j)); 
			i.set_icon(1);       
			if ((j%4) == 0)
			{ 
				i.set_column_text("col3_id", string_format("item %1, col3", j));
			}
			if (( j%2)==0)
			{
				i.set_column_text("col2_id", string_format("item %1, col2", j));
			} 
			doc_item.append_child(i);
		}
	}

	bool on_close(Window *win)
	{
		win->exit_with_code(0);
		return true;
	}  

	void print_tree(ListViewItem item)
	{
		ListViewItem i = item.get_first_child();

		while (i.is_item())
		{
			int pc = i.get_parent_count();
			std::string node;
			for (int j=0;j<pc;j++)
			{
				node += "\t";
			}
			node += i.get_column("col1_id").get_text();
			Console::write_line(node);

			if (item.has_children())
			{
				print_tree(i);
			}
			i = i.get_next_sibling();
		}
	}

	Sprite sprite, sprite2, sprite_olay;
};

// This is the Program class that is called by Application
class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupGL setup_gl;

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);

