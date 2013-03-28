
#include "precomp.h"
#include "character_controller.h"

using namespace clan;

CharacterController::CharacterController(Physics3DWorld &world, float radius, float height, float step_height, float gravity)
	: height_offset(-height), step_height(step_height), gravity(gravity), flying(true), sweep_test(world), allowed_ccd_penetration(0.0001f), margin(0.1f)
{
	shape = Physics3DShape::capsule(radius, height);
}

Vec3f CharacterController::get_position() const
{
	return position + Vec3f(0.0f, height_offset, 0.0f);
}

void CharacterController::set_position(const clan::Vec3f &new_position)
{
	position = new_position - Vec3f(0.0f, height_offset, 0.0f);
}

void CharacterController::move(const Vec3f &velocity)
{
	if (flying)
	{
		// Apply gravity (disable for water / space)
		fly_velocity -= Vec3f(0.0f, gravity, 0.0f);

		// Move in air
		fly_velocity += velocity * 0.005f;

		if (move_vertical(position + fly_velocity))
		{
			if (fly_velocity.y > 0.0f)
			{
				fly_velocity.y = 0.0f;
			}
			else
			{
				flying = false;
				fly_velocity = Vec3f();
			}
		}
	}
	else if (velocity != Vec3f())
	{
		// Move position upwards to help walk on stairs:
		move_vertical(position + Vec3f(0.0f, step_height, 0.0f));

		// Move position forwards:
		move_forward(position + velocity);

		// Move position downwards to help walk on stairs:
		if (!move_vertical(position - Vec3f(0.0f, 2.0f * step_height, 0.0f)))
		{
			flying = true;
			fly_velocity = velocity * 0.5f;
		}
	}
}

void CharacterController::jump(const clan::Vec3f &jump_velocity)
{
	if (!flying)
	{
		flying = true;
		fly_velocity = jump_velocity;
	}
}

void CharacterController::move_forward(clan::Vec3f target)
{
	const int max_iterations = 10;
	float move_length = (target - position).length();
	for (int iterations = 0; iterations < max_iterations; iterations++)
	{
		Vec3f direction = Vec3f::normalize(target - position);
		if (sweep_test.test_first_hit(shape, position, Quaternionf(), target + direction * margin, Quaternionf(), allowed_ccd_penetration))
		{
			if (iterations + 1 == max_iterations)
			{
				Vec3f delta = sweep_test.get_hit_position(0) - position;
				float length = std::max(delta.length() - margin, 0.0f);

				position = position + direction * length;
			}
			else
			{
				Vec3f hit_normal = sweep_test.get_hit_normal(0);
				Vec3f reflect_dir = reflect(direction, hit_normal);
				Vec3f perpendicular_dir = perpendicular_component(reflect_dir, hit_normal);
				target = position + perpendicular_dir * move_length;
			}
		}
		else
		{
			position = target;
			break;
		}
	}
}

bool CharacterController::move_vertical(clan::Vec3f target)
{
	const int max_iterations = 10;
	float move_length = (target - position).length();
	for (int iterations = 0; iterations < max_iterations; iterations++)
	{
		Vec3f direction = Vec3f::normalize(target - position);
		if (sweep_test.test_first_hit(shape, position, Quaternionf(), target + direction * margin, Quaternionf(), allowed_ccd_penetration))
		{
			Vec3f hit_normal = sweep_test.get_hit_normal(0);
			float angle = direction.y > 0.0f ? Vec3f(0.0f, 1.0f, 0.0f).dot(hit_normal) : Vec3f(0.0f, -1.0f, 0.0f).dot(hit_normal);
			bool is_to_steep_to_stand_on = (angle < -0.5f);

			if (iterations + 1 == max_iterations || is_to_steep_to_stand_on)
			{
				Vec3f delta = sweep_test.get_hit_position(0) - position;
				float length = std::max(delta.length() - margin, 0.0f);

				position = position + direction * length;
				return true;
			}
			else
			{
				Vec3f reflect_dir = reflect(direction, hit_normal);
				Vec3f perpendicular_dir = perpendicular_component(reflect_dir, hit_normal);
				target = position + perpendicular_dir * move_length;
			}
		}
		else
		{
			position = target;
		}
	}

	return false;
}

Vec3f CharacterController::reflect(const Vec3f &direction, const Vec3f &normal)
{
	return direction - normal * (2.0f * direction.dot(normal));
}

Vec3f CharacterController::parallel_component(const Vec3f &direction, const Vec3f &normal)
{
	float magnitude = direction.dot(normal);
	return normal * magnitude;
}

Vec3f CharacterController::perpendicular_component(const Vec3f &direction, const Vec3f &normal)
{
	return direction - parallel_component(direction, normal);
}
