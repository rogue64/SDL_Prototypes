#include "CameraComponent.h"
#include "Actor.h"
#include "Math.h"
#include "constants.h"
#include "Game.h"
#include "Renderer.h"

CameraComponent::CameraComponent(Actor* owner) : Component(owner) {

}

CameraComponent::~CameraComponent() {
}

void CameraComponent::Update(float deltaTime) {
	cam_pos = mOwner->GetPosition();
	// get yaw view angle (z-axis rotation)
	Matrix4 yaw_view = Matrix4::CreateFromQuaternion(Quaternion(Vector3(0, 0, 1), mOwner->GetRotation()));
	// get pitch view angle (x-axis rotation)
	// set pitch based on pitch speed
	my_pitch_angle += my_pitch_spd * deltaTime;
	my_pitch_angle = Math::Clamp(my_pitch_angle, -Math::Pi / 4, Math::Pi / 4);
	Matrix4 pitch_view = Matrix4::CreateFromQuaternion(Quaternion(Vector3(0, 1, 0), my_pitch_angle));
	// create combined rotation view and get forward vector from it
	Vector3 forward_vec = Vector3::Transform(Vector3(1, 0, 0), pitch_view * yaw_view);
	// lerp up-dir towards up-dir target
	my_up.x = Math::Lerp(my_up.x, my_up_targ.x, deltaTime);
	my_up.y = Math::Lerp(my_up.y, my_up_targ.y, deltaTime);
	my_up.z = Math::Lerp(my_up.z, my_up_targ.z, deltaTime);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(Matrix4::CreateLookAt(cam_pos, cam_pos + forward_vec, my_up));
}