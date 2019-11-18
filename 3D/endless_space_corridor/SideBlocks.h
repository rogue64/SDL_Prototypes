#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "Renderer.h"
class SideBlocks : public Actor {
public:
	SideBlocks(Game* owner, float x, float y, float z, int meshVersion);
	~SideBlocks();
protected:
	MeshComponent* my_meshc = nullptr;
	int my_mesh_index;
	void OnUpdate(float deltatime) override;
};

