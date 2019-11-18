#include "CheckPointArrow.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"

class Mesh;

CheckPointArrow::CheckPointArrow(Game* owner) : Actor(owner) {
	my_meshc = new MeshComponent(this);
	my_meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Arrow.gpmesh"));
	mScale = 0.15f;
	
}

CheckPointArrow::~CheckPointArrow() {
}

void CheckPointArrow::OnUpdate(float deltaTime) {
	//if (!mGame->getPlayer())
		//return;

	// get the rotation, and it's axis, of previous direction to the current checkpoint (using quaternions)
	CheckPoint* curr_cp = mGame->getActiveCheckPoint();
	float theta = 0;
	// set to the identity quaternion if there are no more checkpoints
	if (!curr_cp) {
		my_quat = Quaternion::Identity;
	}
	else {
		Vector3 p_to_cp = curr_cp->GetPosition() - mGame->getPlayer()->GetPosition();
		float tmp = Vector3::Dot(Vector3::UnitX, p_to_cp);
		// if the dot product is 1, set to  identity since the angle will stay the same because we are still pointing in same direction
		if (tmp == 1) {
			my_quat = Quaternion::Identity;
		}
		// if the dot product is -1, then the axis is the same but we are looking behind, meaning we turn a full radian
		else if (tmp == -1) {
			theta = Math::Pi;
		}
		// else get the angle in radians and find out on what axis it is; put both into a quaternion
		else {
			//theta = Math::Acos(tmp);
			//my_quat = Quaternion(Vector3::Cross(Vector3::UnitX, p_to_cp), theta);
			my_quat = Quaternion::Identity;
		}
	}

	mPosition = mGame->GetRenderer()->Unproject(Vector3(0.f, 250.f, 0.1f));
}