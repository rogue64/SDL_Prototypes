#include "VehicleMovComp.h"
#include "Math.h"
#include "Actor.h"
#include "SDL\SDL_scancode.h"
#include "Game.h"
#include "HeightMap.h"

VehicleMovComp::VehicleMovComp(Actor* owner) : Component(owner, 50) { }


VehicleMovComp::~VehicleMovComp() { }

void VehicleMovComp::Update(float deltaTime) {
	float accl = 0.f;
	if (pedal_held) {
		// increase acceleration timer
		accl_time += SEC * deltaTime;
		accl_time = Math::Clamp<float>(accl_time, 0, plyr_c::ACCL_TIME_LIM);
		// lerp acceleration due to accel-time
		accl = Math::Lerp(plyr_c::MIN_ACCL, plyr_c::MAX_ACCL, accl_time / plyr_c::ACCL_TIME_LIM);
		// interperlate velocity due to accl
		my_vel += mOwner->GetForward() * accl * deltaTime;
	}
	// else reset accl-timer
	else
		accl_time = 0;
	// interpolate position
	Vector3 my_pos = mOwner->GetPosition();
	my_pos += my_vel * deltaTime;
	//mOwner->SetPosition(mOwner->GetPosition() + (my_vel * deltaTime));

	// change height
	//my_pos.z = mOwner->GetGame()->getHeightMap()->getHeight(my_pos.x, my_pos.y);
	my_pos.z = Math::Lerp(my_pos.z, mOwner->GetGame()->getHeightMap()->getHeight(my_pos.x, my_pos.y), 0.1);
	mOwner->SetPosition(my_pos);


	// interpolate angle-vel based on player input
	my_ang_vel = plyr_c::ANGL_ACCL * turn_input * deltaTime;

	mOwner->SetRotation(mOwner->GetRotation() + (my_ang_vel * deltaTime));

	// apply drag
	if (pedal_held)
		my_vel *= plyr_c::ACCLING_DRAG;
	else
		my_vel *= plyr_c::COAST_DRAG;
	
	my_ang_vel *= plyr_c::ANGL_DRAG;
}
