#pragma once
#include "Component.h"
#include "Math.h"
class CameraComponent : public Component {
public:
	CameraComponent(Actor* owner);
	~CameraComponent();
	void Update(float deltaTime) override;
protected:
	Vector3 cam_pos = Vector3::Zero;
	Vector3 cam_vel = Vector3::Zero;
	//  calculates and returns the ideal position of the camera
	Vector3 idealPosition();
};

