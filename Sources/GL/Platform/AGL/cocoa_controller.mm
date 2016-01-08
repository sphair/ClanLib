/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "GL/precomp.h"
#include "cocoa_controller.h"
#include "cocoa_view.h"
#include "cocoa_window.h"
#include "API/Core/Math/vec2.h"
#import <QuartzCore/QuartzCore.h>

@interface CocoaController ()
@property (nonatomic, assign) CADisplayLink *displayLink;
@end

@implementation CocoaController

@synthesize displayLink;

- (id)init
{
    self = [super init];
    if (self)
    {
        self.displayLink = nil;
        self->clanlib_window = 0;
    }
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

-(void) setClanLibWindow:(clan::CocoaWindow *)window
{
    self->clanlib_window = window;
}

- (void)loadView
{
    self.view = [[CocoaView alloc] init];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
//    return YES;
    return NO;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    // Recreate framebuffer here?
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    std::vector<Vec2f> vec_touches;
    NSSet *allTouches = [event allTouches];
    NSEnumerator *enumerator = [allTouches objectEnumerator];
    for (UITouch *touch in enumerator)
    {
        CGPoint point = [touch locationInView:self.view];
        vec_touches.push_back(Vec2f(point.x, point.y));
    }
    if (clanlib_window)
        clanlib_window->on_touches_began(vec_touches);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    std::vector<Vec2f> vec_touches;
    NSSet *allTouches = [event allTouches];
    NSEnumerator *enumerator = [allTouches objectEnumerator];
    for (UITouch *touch in enumerator)
    {
        CGPoint point = [touch locationInView:self.view];
        vec_touches.push_back(Vec2f(point.x, point.y));
    }
    if (clanlib_window)
        clanlib_window->on_touches_moved(vec_touches);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    std::vector<Vec2f> vec_touches;
    NSSet *allTouches = [event allTouches];
    NSEnumerator *enumerator = [allTouches objectEnumerator];
    for (UITouch *touch in enumerator)
    {
        CGPoint point = [touch locationInView:self.view];
        vec_touches.push_back(Vec2f(point.x, point.y));
    }
    if (clanlib_window)
        clanlib_window->on_touches_ended(vec_touches);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    std::vector<Vec2f> vec_touches;
    NSSet *allTouches = [event allTouches];
    NSEnumerator *enumerator = [allTouches objectEnumerator];
    for (UITouch *touch in enumerator)
    {
        CGPoint point = [touch locationInView:self.view];
        vec_touches.push_back(Vec2f(point.x, point.y));
    }
    if (clanlib_window)
        clanlib_window->on_touches_cancelled(vec_touches);
}

- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event
{

}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event
{

}

- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event
{

}

@end

