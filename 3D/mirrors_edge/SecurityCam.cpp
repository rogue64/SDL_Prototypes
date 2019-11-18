#include "SecurityCam.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"

class Mesh;

SecurityCam::SecurityCam(Game* owner) : Actor(owner) {
	my_meshc = new MeshComponent(this);
	my_meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Camera.gpmesh"));
	// add self to game cams
	mGame->addCam(this);
}

SecurityCam::~SecurityCam() {
	mGame->removeCam(this);
}
