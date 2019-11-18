#pragma once
#include "MeshComponent.h"
#include "SegmentCast.h"
class LaserComponent : public MeshComponent {
public:
	LaserComponent(Actor* owner);
	~LaserComponent();
	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
protected:
	LineSegment my_ls;
	Matrix4 getWorldTransform();
};

