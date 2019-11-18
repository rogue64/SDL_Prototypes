#pragma once
#include "Component.h"
#include "Math.h"
class CameraComponent : public Component {
public:
	CameraComponent(Actor* owner);
	~CameraComponent();
	void Update(float deltaTime) override;
	const float& getPitchSpeed() const { return my_pitch_spd; }
	void setPitchSpeed(const float speed) { my_pitch_spd = speed; }
	inline void setUpTarg(const Vector3& up) { my_up_targ = up; }
protected:
	Vector3 cam_pos = Vector3::Zero;
	Vector3 my_up_targ = Vector3::UnitZ;
	Vector3 my_up = Vector3::UnitZ;
	float my_pitch_angle = 0.0f;
	float my_pitch_spd = 0.00f;
};