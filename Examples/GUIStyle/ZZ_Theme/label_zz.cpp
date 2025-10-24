#include <ClanLib/core.h> // for CL_DomElement
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "label_zz.h"
#include "stylemanager_zz.h"

CL_Label_ZZ::CL_Label_ZZ(
	CL_Label * label,
	CL_StyleManager_ZZ * style,
    LabelType label_type)
:
    CL_ComponentStyle(label),
    label_(label),
    label_type_(label_type),
    resources_(style->get_resources())
{
    if (label_type_ == type1)
    {
        font_ = new CL_Font("Label/font", resources_);
    }
    else
    {
        font_ = new CL_Font("Button/font", resources_);
    }
    label_->set_height(font_->get_height("P"));
    label_->set_width(font_->get_width(label_->get_text()));
	
    slots_.connect(label_->sig_paint(), this, &CL_Label_ZZ::on_paint);
    slots_.connect(label_->sig_get_preferred_size(), this, &CL_Label_ZZ::on_get_preferred_size);
    slots_.connect(label_->sig_set_options(), this, &CL_Label_ZZ::on_set_options);
}

CL_Label_ZZ::~CL_Label_ZZ()
{
	delete font_;
}

void CL_Label_ZZ::on_get_preferred_size(CL_Size &size)
{
    label_->set_height(font_->get_height("P"));
    label_->set_width(font_->get_width(label_->get_text()));
	size.width = font_->get_width(label_->get_text());
	size.height = font_->get_height(label_->get_text());
}

void CL_Label_ZZ::on_paint()
{
    std::string const & text = label_->get_text();

	CL_Point pos = calc_origin(label_->get_alignment(), CL_Size(label_->get_width() / 2, label_->get_height()));
	
	font_->draw(label_->get_screen_x() + pos.x, label_->get_screen_y() + pos.y, text);
}

void CL_Label_ZZ::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("type"))
	{
		if (options.get_attribute("type") == "type1")
			label_type_ = type1;
		else
			label_type_ = type2;
	}
    
	delete font_;

    if (label_type_ == type1)
    {
        font_ = new CL_Font("Label/font", resources_);
    }
    else
    {
        font_ = new CL_Font("Button/font", resources_);
    }
}
