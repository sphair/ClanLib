
#pragma once

class Client;

// Class that holds highlighted and/or selected map areas.
// The result is a list of color codes that can be applied to a GameTerrain.
class MapSelection
{
// Construction:
public:
	MapSelection(Client *client);

// Attributes:
public:
	// Returns true is highlights or selections has been modified since last get_result_colors() call
	bool is_modified() const;

	// Returns true is the selection is complete; it has both a source and a destination selected
	bool is_complete() const;

	// Returns the area id of selected source (attacking) area
	int get_selected_source_area_id() const;

	// Returns the area id of selected destination (attacked) area
	int get_selected_destination_area_id() const;

// Operations:
public:
	// Sets colors on areas based on player ownership.
	void update_areas_with_player_ownership();

	// Sets the current highlighted area (there can be only one at a time)
	void set_highlighted_area(int area_id);

	// Selects the currently highlighted area
	void select_highlighted_area();

	// Clear all selected areas
	void clear_selection();

	// Returns the list of map area colors, based on highlights and selections
	// Note: This also clears modified flag!
	const std::vector<unsigned int> &get_result_colors();

// Implementation:
private:
	bool has_highlighted_area() const;

	int get_highlighted_area_id() const;

	bool is_area_id_valid(int area_id) const;

	void toggle_selected_area(int area_id);

	void clear_selected_source_area();

	void set_selected_area(int area_id);

	std::vector<unsigned int> original_area_colors;
	std::vector<unsigned int> current_area_colors;

	int highlighted_area_id;
	int selected_source_area_id;
	int selected_destination_area_id;

	Color color_highlighted;
	Color color_selected_source;
	Color color_selected_destination;

	bool is_dirty;

	bool is_initialized;

	Client *client;
};
