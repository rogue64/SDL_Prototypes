#pragma once
#include "Actor.h"
class SecurityCam : public Actor {
public:
	SecurityCam(Game* owner);
	~SecurityCam();
protected:
	class MeshComponent* my_meshc = nullptr;
};

