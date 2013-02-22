
#include "precomp.h"
#include "character_controller.h"

using namespace clan;

CharacterController::CharacterController(Physics3DWorld &world, float radius, float height, float step_height, float gravity)
	: height_offset(-height), step_height(step_height), gravity(gravity), flying(true), sweep_test(world), allowed_ccd_penetration(0.0001f)
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
		fly_velocity -= Vec3f(0.0f, gravity, 0.0f);

		if (try_move_to(position + fly_velocity))
		{
			flying = false;
			fly_velocity = Vec3f();
		}
	}
	else if (velocity != Vec3f())
	{
		// Move position upwards to help walk on stairs and slopes:
		try_move_to(position + Vec3f(0.0f, step_height, 0.0f));

		// Move position forwards:
		try_move_to(position + velocity);

		// Move position downwards to help walk on stairs and slopes:
		if (!try_move_to(position - Vec3f(0.0f, 2.0f * step_height, 0.0f)))
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

bool CharacterController::try_move_to(const Vec3f &target)
{
	float margin = 0.1f;

	Vec3f direction = Vec3f::normalize(target - position);

	if (sweep_test.test_first_hit(shape, position, Quaternionf(), target + direction * margin, Quaternionf(), allowed_ccd_penetration))
	{
		Vec3f delta = sweep_test.get_hit_position(0) - position;
		float length = std::max(delta.length() - margin, 0.0f);

		position = position + direction * length;

		return true;
	}
	else
	{
		position = target;

		return false;
	}
}
