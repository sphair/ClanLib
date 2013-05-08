/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Arkadiusz Kalinowski
*/


#include "Physics2D/precomp.h"
#include "API/display.h"
#include "physics_debug_draw_impl.h"
#include "physics_world_impl.h"
#include "API/Physics2D/World/physics_debug_draw.h"
#include "API/Physics2D/World/physics_world.h"


namespace clan
{

//																											_______________________																											
//																											C O N S T R U C T O R S
PhysicsDebugDraw_Impl::PhysicsDebugDraw_Impl(PhysicsWorld_Impl &pw_impl)
: owner(&pw_impl),
  physic_scale(owner->physic_scale)
{
	owner->world.SetDebugDraw(this);
}

//																											___________________																											
//																											A T T R I B U T E S


//																											_____________																							
//																											S I G N A L S



//																											___________________																											
//																											O P E R A T I O N S

void PhysicsDebugDraw_Impl::DrawPolygon(const b2Vec2* vertices, int32 k_segments, const b2Color& color)
{
	Colorf colorr(color.r,color.g,color.b,1.0f);

	int i;
	for(i=0;i<k_segments-1;i++)
	{
		used_canvas->draw_line(vertices[i].x*physic_scale,vertices[i].y*physic_scale,
							vertices[i+1].x*physic_scale,vertices[i+1].y*physic_scale,
							colorr);
	}
		used_canvas->draw_line(vertices[0].x*physic_scale,vertices[0].y*physic_scale,
							vertices[k_segments-1].x*physic_scale,vertices[k_segments-1].y*physic_scale,
							colorr);
}

void PhysicsDebugDraw_Impl::DrawSolidPolygon(const b2Vec2* vertices, int32 k_segments, const b2Color& color)
{
	Colorf color1(0.5f*color.r,0.5f*color.g,0.5f*color.b,0.5f);
	Colorf color2(color.r,color.g,color.b,1.0f);

	int i;
	for(i=0;i<k_segments-3;i++)
	{
		used_canvas->fill_triangle(Pointf(vertices[i].x*physic_scale,vertices[i].y*physic_scale),
								Pointf(vertices[i+1].x*physic_scale, vertices[i+1].y*physic_scale),
								Pointf(vertices[i+2].x*physic_scale, vertices[i+2].y*physic_scale),
								color1);
	}
		used_canvas->fill_triangle(Pointf(vertices[0].x*physic_scale,vertices[0].y*physic_scale),
								Pointf(vertices[k_segments-2].x*physic_scale, vertices[k_segments-2].y*physic_scale),
								Pointf(vertices[k_segments-1].x*physic_scale, vertices[k_segments-1].y*physic_scale),
								color1);

	for(i=0;i<k_segments-1;i++)
	{
		used_canvas->draw_line(vertices[i].x*physic_scale,vertices[i].y*physic_scale,
							vertices[i+1].x*physic_scale,vertices[i+1].y*physic_scale,
							color2);
	}
		used_canvas->draw_line(vertices[0].x*physic_scale,vertices[0].y*physic_scale,
							vertices[k_segments-1].x*physic_scale,vertices[k_segments-1].y*physic_scale,
							color2);
}

void PhysicsDebugDraw_Impl::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	used_canvas->fill_circle(Pointf(center.x*physic_scale, center.y*physic_scale), radius*physic_scale, Colorf(color.r,color.g,color.b,0.5f));
}

void PhysicsDebugDraw_Impl::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const int k_segments = 16;
	const float32 k_increment = 2.0f * b2_pi / (float)k_segments;
	float32 radiuss = radius * physic_scale;
	float32 theta = 0.0f;

	GraphicContext gc = used_canvas->get_gc();

	Vec2f CLaxis(axis.x,axis.y);
	Vec2f CLcenter(center.x*physic_scale,center.y*physic_scale);
 	Vec2f *positions= new Vec2f[k_segments]; //delete id 3
	Colorf color1(0.5f*color.r,0.5f*color.g,0.5f*color.b,0.5f);
	Colorf color2(color.r,color.g,color.b,1.0f);

	used_canvas->fill_circle(Pointf(CLcenter.x, CLcenter.y), radiuss, Colorf(color.r,color.g,color.b,0.5f));

	int i;
	for(i=0;i<k_segments;i++)
	{
		positions[i]= (Vec2f(cosf(theta),sinf(theta)) * radiuss) + CLcenter;

		theta+= k_increment;
	}

	for(i=0;i<k_segments;i++)
	{
		used_canvas->draw_line(positions[i].x*physic_scale,positions[i].y*physic_scale,
							positions[i+1].x*physic_scale,positions[i+1].y*physic_scale,
							color2);
	}
		used_canvas->draw_line(positions[0].x*physic_scale,positions[0].y*physic_scale,
							positions[k_segments-1].x*physic_scale,positions[k_segments-1].y*physic_scale,
							color2);

	delete positions; //delete id 3

}

void PhysicsDebugDraw_Impl::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	used_canvas->draw_line(p1.x*physic_scale,
					 p1.y*physic_scale,
					 p2.x*physic_scale,
					 p2.y*physic_scale,
					 Colorf(color.r,color.g,color.b));
}

void PhysicsDebugDraw_Impl::DrawTransform(const b2Transform& xf)
{
	Vec2f p1(xf.p.x,xf.p.y);
	Vec2f p2;
	const float32 k_axisScale = 0.4f;
	
	p2 = p1 + Vec2f(xf.q.GetXAxis().x,xf.q.GetXAxis().y) * k_axisScale;
	used_canvas->draw_line(p1.x*physic_scale,
					 p1.y*physic_scale,
					 p2.x*physic_scale,
					 p2.y*physic_scale,
					 Colorf(1.0f,0.0f,0.0f));

	p2 = p1 + Vec2f(xf.q.GetYAxis().x,xf.q.GetYAxis().y) * k_axisScale;
	used_canvas->draw_line(p1.x*physic_scale,
					 p1.y*physic_scale,
					 p2.x*physic_scale,
					 p2.y*physic_scale,
					 Colorf(0.0f,1.0f,0.0f));
	
}

void PhysicsDebugDraw_Impl::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	used_canvas->fill_rect(p.x-size,
					 p.y-size,
					 p.x+size,
					 p.y+size,
					 Colorf(color.r,color.g,color.b));
}

void PhysicsDebugDraw_Impl::DrawString(int x, int y, const char *string, ...)
{
	/*
	char buffer[128];

	va_list arg;
	va_start(arg, string);
	vsprintf_s(buffer, string, arg);
	va_end(arg);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.9f, 0.6f, 0.6f);
	glRasterPos2i(x, y);
	int32 length = (int32)strlen(buffer);
	for (int32 i = 0; i < length; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	*/
}

void PhysicsDebugDraw_Impl::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	used_canvas->draw_box(aabb->lowerBound.x*physic_scale,
					aabb->lowerBound.y*physic_scale,
					aabb->upperBound.x*physic_scale,
					aabb->upperBound.y*physic_scale,
					Colorf(c.r,c.g,c.b));
	/*
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_LINE_LOOP);
	
	glVertex2f(calcDrawPos(aabb->lowerBound.x),calcDrawPos(aabb->lowerBound.y));
	glVertex2f(calcDrawPos(aabb->upperBound.x),calcDrawPos(aabb->lowerBound.y));
	glVertex2f(calcDrawPos(aabb->upperBound.x),calcDrawPos(aabb->upperBound.y));
	glVertex2f(calcDrawPos(aabb->lowerBound.x),calcDrawPos(aabb->upperBound.y));

	glEnd();
	*/
}

}
