#include "CameraComponent.h"
#include "Actor.h"
#include "Math.h"
#include "constants.h"
#include "Game.h"
#include "Renderer.h"

CameraComponent::CameraComponent(Actor* owner) : Component(owner) {
	cam_pos = idealPosition();
}

CameraComponent::~CameraComponent() {
}

void CameraComponent::Update(float deltaTime) {
	Vector3 ideal = idealPosition();
	Vector3 displacement = cam_pos - ideal;
	Vector3 spring_accl = (-CAM_SPRING * displacement) - (CAM_DAMP * cam_vel);
	cam_vel += spring_accl * deltaTime;
	cam_pos += cam_vel * deltaTime;
	Vector3 targ_pos = cam_pos;
	targ_pos += mOwner->GetForward() * TARG_D;
	Matrix4 camera_view = Matrix4::CreateLookAt(cam_pos, targ_pos, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(camera_view);
}

Vector3 CameraComponent::idealPosition() {
	Vector3 eye_pos = mOwner->GetPosition();
	eye_pos += mOwner->GetForward() * -HORZ_D;
	eye_pos.z = DEFAULT_HEIGHT;
	return eye_pos;
}
