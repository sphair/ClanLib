
#pragma once

class CharacterController
{
public:
	CharacterController(clan::Physics3DWorld &world, float radius, float height, float step_height, float gravity);

	clan::Vec3f get_position() const;
	void set_position(const clan::Vec3f &position);

	void move(const clan::Vec3f &velocity);
	void jump(const clan::Vec3f &jump_velocity);

private:
	bool try_move_to(const clan::Vec3f &target);
	void move_forward(clan::Vec3f target);
	bool move_vertical(clan::Vec3f target);

	static clan::Vec3f reflect(const clan::Vec3f &direction, const clan::Vec3f &normal);
	static clan::Vec3f parallel_component(const clan::Vec3f &direction, const clan::Vec3f &normal);
	static clan::Vec3f perpendicular_component(const clan::Vec3f &direction, const clan::Vec3f &normal);

	clan::Physics3DShape shape;
	float height_offset;
	clan::Vec3f position;
	float step_height;
	float gravity;
	clan::Vec3f fly_velocity;
	bool flying;

	float margin;

	clan::Physics3DSweepTest sweep_test;

	float allowed_ccd_penetration;
};
