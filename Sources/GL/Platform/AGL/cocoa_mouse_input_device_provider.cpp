
#include "GL/precomp.h"
#include "cocoa_mouse_input_device_provider.h"
#include "API/Display/Window/keys.h"

namespace clan
{

CocoaMouseInputDeviceProvider::CocoaMouseInputDeviceProvider()
: sig_provider_event(nullptr), down(false)
{
}

CocoaMouseInputDeviceProvider::~CocoaMouseInputDeviceProvider()
{
}

bool CocoaMouseInputDeviceProvider::get_keycode(int keycode) const
{
    if (keycode == mouse_left)
        return down;
    return false;
}

float CocoaMouseInputDeviceProvider::get_x() const
{
    if (touches.empty())
        return 0;
    else
        return touches[0].x;
}

float CocoaMouseInputDeviceProvider::get_y() const
{
    if (touches.empty())
        return 0;
    else
        return touches[0].y;
}

}
