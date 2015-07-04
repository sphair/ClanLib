
#pragma once

#include "Views/EditConnection/edit_connection_view.h"

class EditConnectionController : public clan::ViewController
{
public:
	EditConnectionController();

private:
	std::shared_ptr<EditConnectionView> edit_view;
};
