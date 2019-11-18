#pragma once
#include "Component.h"
#include "Math.h"
#include "constants.h"
class VehicleMovComp : public Component {
public:
	VehicleMovComp(Actor* owner);
	~VehicleMovComp();
	void Update(float deltaTime) override;
	//void ProcessInput(const Uint8* keyState) override;
	void setPedalHeld(bool _pedal_held) { pedal_held = _pedal_held; }
	void setDirInput(int in) { if (in > 1) turn_input = 1; else if (in < -1) turn_input = -1; else turn_input = in; }
protected:
	Vector3 my_vel = Vector3::Zero;
	float my_ang_vel = 0.f;
	float accl_time = 0.f;

	// input vars
	int turn_input = 0;
	bool pedal_held = false;
};

