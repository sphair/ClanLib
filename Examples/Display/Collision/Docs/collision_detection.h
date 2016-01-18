
/*! \page CollisionDetection Collision Detection

The collision system works by checking for intersections between line segments in line-loops surrounding the objects to collide. The advantage of this method over traditional methods, such as pixel based collision detection, is that the amount of data that needs to be worked with when checking for collisions and when doing transformations is very low. This enables besides fast collision testing also fast rotation and scaling of outline geometries. 

<h2>Collision Outlines</h2>

Outlines can be generated from RGBA images by following the edge between transparent and opaque pixels. 

To generate an outline from a RGBA image stored as image.png the following can be used: 

\ref clan::CollisionOutline outline("image.png");

Generating outlines from bitmaps can be quite expensive causing long load times. To battle that \ref clan::CollisionOutline has a save function and constructors which can load saved outlines: 

\ref clan::CollisionOutline generated("image.png");
generated.save("image.out");
 
\ref clan::CollisionOutline loaded("image.out");

Initially the contour following algorithm adds every pixel along the edge to the outline. This results in a lot of redundant information being added to the outline, but that can be optimized away without reducing the accuracy of the outline too much. The constructor which creates outlines from RGBA images takes a \ref clan::OutlineAccuracy parameter which specifies how much to optimize the outline. 

\ref clan::CollisionOutline generated("image.png", accuracy_high);
The values for \ref clan::CollisionAccuracy are:

<ul>
<li>accuracy_raw</li>
<li>accuracy_high</li>
<li>accuracy_medium</li>
<li>accuracy_low</li>
<li>accuracy_poor</li>
</ul> 

and their affects on generated outlines: 

<img src="../../img/OutlineAccuracy.png"/>

The optimization causes errors mostly in the round parts. 

<h2>Testing for collisions</h2>

Once the collision outlines have been positioned using the transformation function (translate, rotate and scale) checking for a collision is simply a matter of calling the collide function. In a game one might have code similar to this: 

\code 
if( outline.collide(outline2) )
{
	foo();
}
\endcode

It's also possible to test if a point is inside an outline: 

\code 
if( outline.point_inside( clan::Mouse::get_x(), clan::Mouse::get_y() )
{
	bar();
}
\endcode

When checking for a collision a bounding circle test is always performed first. The way the collision testing is done can be adjusted by enabling/disabling completely inside test, and by enable/disable the object bounding box test. 

When using the inside test, outlines completely inside another outline (or completely surrounding) will report a collision. If either of the objects being tested has inside_test set to true, the inside test will be done for both objects. 

Object bounding box (obb) test uses a rotated tightly computed rectangle around the outline, and the obb is tested against the other obb first before any further (more detailed) collision detection tests are performed. For long narrow outlines an obb will give a lot more tighter bounds than a bounding circle, effectively eliminating further redundant checks. 

 
outline.set_inside_test(true);
outline.set_obb_test(false);

<h2>Internal operation</h2>

The structure of the collision data structures is as follows: 

\code
clan::CollisionOutline
	clan::Contour(s)
		std::vector<clan::Pointf> points
		std::vector<clan::CollisionCircle> subcircles
\endcode
Collisions are checked for by checking each line-segment forming the outline, against the line-segments of the other collision outline. If the number of line segments is big this will be somewhat slow. To eliminate checks that are sure to fail, the line-segments have been grouped into circles which hold a start and end index in the point array. 

These sub-circles are collided against each other before any line-line intersection tests take place. Line-segments encapsulated in subcircles are only checked when two subcircles collides with each other. If the subcircles don't collide there is no chance that the line-segments inside them will collide either. 

<img src="../../img/OutlineSubcircles.png"/>

If an outline is created manually by adding a contour to the outline and points into the contour, it's necessary to calculate the subcircles and the radius before collision tests can be performed. It should also be noted that the points of the contour is assumed "closed" that is, you do not need to specify the same point as both first and last. And the points must be added to the contour in counter-clockwice order. 


\code 
	clan::CollisionOutline outline;
 
	clan::Contour contour;
	contour.get_points().push_back( clan::Pointf(0.0f,0.0f) );
	contour.get_points().push_back( clan::Pointf(0.0f,100.0f) );
	outline.get_contours().push_back(contour);
	outline.calculate_radius();
	outline.calculate_sub_circles();
\endcode
If your contours are rather small, you can optimise them to only have one (sub)circle. This circle is calculated to be as small as possible, and still contain all the points. It can be done with the following code. 


 \code
	[... create the outline in some way ...]
	// Use this in-stead of "calculate_sub_circles()";
	outline.calculate_smallest_enclosing_discs();
\endcode

<h2>Collision Info</h2>

If you have enabled any kind of collision info (with enable_collision_info(points,normals,metadata)) then you can retrieve this information with the method get_collision_info. What will be returned is a vector with \ref clan::CollidingContour structures. Each representing the collision of two contours. 

These structures contain the information you asked the outlines to collect. They always contain two pointers to the colliding contours. These are called: contour1 and contour2. If you told it to collect collision-points, then these can be found as the "point" property of all the \ref clan::CollisionPoint stored in the vector points. If you told it to collect normals then they can be found as the "normal" property of all the \ref clan::CollisionPoint stored in the vector points. 

The following code demonstrates how you can access the information. 


 \code
	outline.enable_collision_info(true, true, false);
	if( outline.collide(outline2) )
	{
		const std::vector<clan::CollidingContours> &colpointinfo = outline.get_collision_info();
		// Loop through all pairs of colliding contours
		for(size_t c = 0; c < colpointinfo.size(); c++)
		{
			const clan::CollidingContours &cc = colpointinfo[c];
			for(size_t p = 0; p < cc.points.size(); p++)
			{
				std::cout << "Collision: Point(" << cc.points[p].point.x << "," << cc.points[p].point.y << ")\n";
				std::cout << "Collision: Normal(" << cc.points[p].normal.x << "," << cc.points[p].normal.y << ")\n";
			}
		}
	}
\endcode

<h2>Collision Metadata</h2>

The metadata-part of a collision takes a bit more explaination. It is used to keep track of where the intersections between two contours occured. That is, they keep track of what linesegments generated the collision-points. They also keep track of whether the collision point is an entry or an exit point (Note this works because all contours are described as a list of points in counter-clockwice order). The metadata is mainly intended to calculate the depth of a penetration. Since contours only store the points, and not the linesegments, we have to store the two indexes of the two points that represent the linesegment. And we have to do this for both linesegments. 

The following piece of code shows how to access the metadata. 
 
 \code
	outline.enable_collision_info(true, true, true);
	if( outline.collide(outline2) )
	{
		const std::vector<clan::CollidingContours> &colpointinfo = outline.get_collision_info();
		// Loop through all pairs of colliding contours
		for(size_t c = 0; c < colpointinfo.size(); c++)
		{
			const clan::CollidingContours &cc = colpointinfo[c];
			for(size_t p = 0; p < cc.points.size(); p++)
			{
				const clan::CollisionPoint &colp = cc.points[p];
				std::cout << "LineSegment1:"
					<< "(" << cc.contour1->get_points()[colp.contour1_line_start].x
					<< "," << cc.contour1->get_points()[colp.contour1_line_start].y << ") - "
					<< "(" << cc.contour1->get_points()[colp.contour1_line_end].x
					<< "," << cc.contour1->get_points()[colp.contour1_line_end].y << ")\n";
				std::cout << "LineSegment2:"
					<< "(" << cc.contour2->get_points()[colp.contour2_line_start].x
					<< "," << cc.contour2->get_points()[colp.contour2_line_start].y << ") - "
					<< "(" << cc.contour2->get_points()[colp.contour2_line_end].x
					<< "," << cc.contour2->get_points()[colp.contour2_line_end].y << ")\n";
			}
		}
	}
\endcode

*/
