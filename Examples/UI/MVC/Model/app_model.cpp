
#include "precomp.h"
#include "app_model.h"

AppModel::AppModel()
{
	instance_ptr = this;
}

AppModel::~AppModel()
{
	instance_ptr = nullptr;
}

AppModel *AppModel::instance()
{
	return instance_ptr;
}

AppModel *AppModel::instance_ptr = nullptr;
