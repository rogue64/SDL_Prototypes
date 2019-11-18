#include "LaserComponent.h"
#include "Renderer.h"
#include "Game.h"
#include "Math.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "VertexArray.h"

LaserComponent::LaserComponent(Actor* owner) : MeshComponent(owner, true) {
	SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}

LaserComponent::~LaserComponent() {
	
}

void LaserComponent::Update(float deltaTime) {
	my_ls.mStart = mOwner->GetPosition();
	my_ls.mEnd = my_ls.mStart + (mOwner->GetForward() * 500);
}

Matrix4 LaserComponent::getWorldTransform() {
	Matrix4 scale = Matrix4::CreateScale(my_ls.Length(), 1, 1);
	//Quaternion rot_quat = Quaternion(Vector3::UnitX, mOwner->GetRotation());
	//Matrix4 rotation = Matrix4::CreateFromQuaternion(rot_quat);
	Matrix4 rotation = Matrix4::CreateRotationZ(mOwner->GetRotation());
	Matrix4 translation = Matrix4::CreateTranslation(my_ls.mStart + (mOwner->GetForward() * 250));
	// multiply to make tranform: SenioR T.
	Matrix4 out = scale * rotation;
	out *= translation;
	return out;
}

void LaserComponent::Draw(class Shader* shader) {
	if (mMesh)
	{
		// Set the world transform
		shader->SetMatrixUniform("uWorldTransform",
			this->getWorldTransform());
		// Set the active texture
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// Set the mesh's vertex array as active
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}