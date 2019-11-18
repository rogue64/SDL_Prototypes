#include "SideBlocks.h"
#include "Game.h"
#include "Player.h"

class Mesh;

SideBlocks::SideBlocks(Game* owner, float x, float y, float z, int meshVersion) : Actor(owner) {
	mPosition = Vector3(x, y, z);
	mScale = 500;
	my_meshc = new MeshComponent(this);
	my_meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	my_mesh_index = meshVersion;
	if (my_mesh_index > 5 || my_mesh_index < 1)
		my_mesh_index = 1;
	my_meshc->SetTextureIndex(my_mesh_index);
}


SideBlocks::~SideBlocks()
{
	//if (mGame->isRunning())
		//new SideBlocks(mGame, mPosition.x + 3000, mPosition.y, mPosition.z, my_mesh_index + 1);
}

void SideBlocks::OnUpdate(float deltatime) {
	// destroy self when player passes
	/*if (mGame->getPlayer()->GetPosition().x > mPosition.x + 500)
		mState = ActorState::Destroy;*/
	if (mGame->getPlayer()->GetPosition().x > mPosition.x + 500) {
		my_mesh_index++;
		if (my_mesh_index > 5 || my_mesh_index < 1)
			my_mesh_index = 1;
		mPosition.x += 3500;
		my_meshc->SetTextureIndex(my_mesh_index);
	}
}
