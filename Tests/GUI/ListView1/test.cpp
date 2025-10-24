#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h> 
#include <ClanLib/gl.h> 
    
class App
{
public:
	int main(const std::vector<CL_String> &args)
	{
		CL_ConsoleWindow console("Console");
 
		try
		{
			CL_ResourceManager resources;
			resources.load("../../../Resources/GUIThemeLuna/resources.xml");

			CL_GUIManager gui;

			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(wm); 

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document("../../../Resources/GUIThemeLuna/theme.css");
 
			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("ListView test app.");
			win_desc.set_position(CL_Rect(200, 200, 840, 680), false);
			CL_Window window(&gui, win_desc);			
			window.func_close().set(this, &App::on_close, &window); 
     	  
			CL_GraphicContext gc = window.get_gc();

			sprite = CL_Sprite(gc, "../../../Resources/GUIThemeLuna/Images/icon_mycomputer.png");
			sprite2 = CL_Sprite(gc, "../../../Resources/GUIThemeLuna/Images/icon_folder_32x32.png");
			sprite_olay = CL_Sprite(gc, "../../../Resources/GUIThemeLuna/Images/overlay_96x96.png");

//			CL_PixelBuffer pbuf = CL_PNGProvider::load("../Images/icon_folder.png");
			//			CL_PixelBuffer pbuf2 = CL_PNGProvider::load("../Images/icon_folder_32x32.png");

			CL_ListView lv1(&window);
			lv1.set_geometry(CL_Rect(10, 10, 630, 210));
			lv1.set_display_mode(listview_mode_details);
			populate_listview(lv1);

			print_tree(lv1.get_document_item());
			CL_Console::write_line("num items: %1", lv1.get_document_item().get_child_count(true,false));

			CL_ListView lv2(&window);
			lv2.set_geometry(CL_Rect(10, 220, 630, 470));
			lv2.set_display_mode(listview_mode_thumbnails);
			populate_listview(lv2);

			gc.clear();

			gui.exec();
		} 
		catch (CL_Exception e)
		{
 			CL_Console::write_line(e.message);
			console.display_close_message();
		} 
 
		return 0;
	} 

	void populate_listview(CL_ListView &lv)
	{
		CL_ListViewItem doc_item = lv.get_document_item(); 

		CL_ListViewHeader *lv_header = lv.get_header();  
		lv_header->append(lv_header->create_column("col1_id", "col1")).set_width(130);
		lv_header->append(lv_header->create_column("col2_id", "col2")).set_width(80);
		lv_header->append(lv_header->create_column("col3_id", "col3")).set_width(80);

		CL_ListViewIconList ico_list = lv.get_icon_list();

//		CL_Colorf sel_color("#316ac5");
//		ico_list.set_selected_color(CL_Colorf::cornflowerblue);

		CL_ListViewIcon icon;
		icon.set_sprite(sprite, listview_mode_details);
//		icon.set_pixel_buffer(pbuf2, listview_mode_icons);
		icon.set_sprite(sprite2, listview_mode_icons);
		icon.set_sprite(sprite2, listview_mode_thumbnails);

		CL_ListViewIcon olay;
		olay.set_sprite(sprite_olay, listview_mode_thumbnails);
		olay.set_offset(CL_Point(0, 96-sprite_olay.get_height()), listview_mode_thumbnails);
		olay.set_scalable(false);
		
		ico_list.set_icon(1, icon);
		ico_list.set_icon(1001, olay);

		CL_ListViewItem i1 = lv.create_item();
		{
			i1.set_column_text("col1_id", "i1");
			i1.set_column_text("col2_id", "col2 text!");
			i1.set_editable(true);
			i1.set_icon(1);
			i1.add_overlay_icon(1001);
			i1.set_open(false);
			doc_item.append_child(i1);

			CL_ListViewItem i1c1 = lv.create_item();  
			{ 
				i1c1.set_column_text("col1_id", "i1c1");
				i1c1.set_column_text("col2_id", "BOOYAA!");
				i1c1.set_icon(1);
				i1.append_child(i1c1);
			}
		}

		CL_ListViewItem i2 = lv.create_item();
		{
			i2.set_column_text("col1_id", "i2");
			i2.set_column_text("col2_id", "bar2");
			i2.set_column_text("col3_id", "foobar!");
			i2.set_icon(1);
			i2.set_open(true);

			CL_ListViewItem i2c1 = lv.create_item();  
			{
				i2c1.set_column_text("col1_id", "i2c1"); 
				i2c1.set_column_text("col2_id", "BOOYAA!");
				i2c1.set_icon(1);
			}
			i2.append_child(i2c1); 

			CL_ListViewItem i2c2 = lv.create_item();
			{
 				i2c2.set_column_text("col1_id", "i2c2");
				i2c2.set_column_text("col2_id", "BOOYAA!");
				i2c2.set_icon(1);
				i2c2.set_open(true); 

				CL_ListViewItem i2c2c1 = lv.create_item();
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
			CL_ListViewItem i = lv.create_item();
			i.set_editable(true);
			i.set_column_text("col1_id", cl_format("item %1", j)); 
			i.set_icon(1);       
			if ((j%4) == 0)
			{ 
				i.set_column_text("col3_id", cl_format("item %1, col3", j));
			}
			if (( j%2)==0)
			{
				i.set_column_text("col2_id", cl_format("item %1, col2", j));
			} 
			doc_item.append_child(i);
		}
	}

	bool on_close(CL_Window *win)
	{
		win->exit_with_code(0);
		return true;
	}  

	void print_tree(CL_ListViewItem item)
	{
		CL_ListViewItem i = item.get_first_child();

		while (i.is_item())
		{
			int pc = i.get_parent_count();
			CL_String node;
			for (int j=0;j<pc;j++)
			{
				node += "\t";
			}
			node += i.get_column("col1_id").get_text();
			CL_Console::write_line(node);

			if (item.has_children())
			{
				print_tree(i);
			}
			i = i.get_next_sibling();
		}
	}

	CL_Sprite sprite, sprite2, sprite_olay;
};

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

