
#include "GL/precomp.h"
#include "cocoa_mouse_input_device_provider.h"
#include "API/Display/Window/keys.h"

CL_CocoaMouseInputDeviceProvider::CL_CocoaMouseInputDeviceProvider()
: sig_provider_event(0), down(false)
{
}

CL_CocoaMouseInputDeviceProvider::~CL_CocoaMouseInputDeviceProvider()
{
}

bool CL_CocoaMouseInputDeviceProvider::get_keycode(int keycode) const
{
    if (keycode == GL_MOUSE_LEFT)
        return down;
    return false;
}

int CL_CocoaMouseInputDeviceProvider::get_x() const
{
    if (touches.empty())
        return 0;
    else
        return (int)touches[0].x;
}

int CL_CocoaMouseInputDeviceProvider::get_y() const
{
    if (touches.empty())
        return 0;
    else
        return (int)touches[0].y;
}
