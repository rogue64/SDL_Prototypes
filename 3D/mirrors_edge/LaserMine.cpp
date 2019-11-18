#include "LaserMine.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "LaserComponent.h"

class Mesh;

LaserMine::LaserMine(Game* owner) : Actor(owner) {
	my_meshc = new MeshComponent(this);
	my_meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/LaserMine.gpmesh"));
	my_laser = new LaserComponent(this);
}


LaserMine::~LaserMine() {

}
