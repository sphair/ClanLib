
#ifndef header_property_list
#define header_property_list

class PropertyList : public CL_Component
{
//! Construction:
public:
	PropertyList(const CL_Rect &pos, CL_Component *parent);

	~PropertyList();

//! Attributes:
public:
	CL_Font *font;

	typedef std::pair<std::string, std::string> Property;

	std::vector<Property> properties;

//! Operations:
public:

//! Implementation:
private:
	void on_paint();

	CL_SlotContainer slots;
};

#endif
