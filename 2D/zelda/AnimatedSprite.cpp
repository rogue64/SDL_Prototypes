#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder) {
}

void AnimatedSprite::Update(float deltaTime) {
	// if no animation name, return
	if (mAnimName.empty())
		return;
	// if animation not paused, increase timer
	if (!mIsPaused)
		mAnimTimer += FPS * deltaTime;
	// get the current frame based on fps
	int frames = mAnims.at(mAnimName).size();
	if (mAnimTimer > mAnimFPS * frames)
		mAnimTimer -= mAnimFPS * frames;
	int curr_frame = (int) (mAnimTimer / mAnimFPS);
	curr_frame = curr_frame % frames;
	// set texture to current frame
	SetTexture(mAnims.at(mAnimName).at(curr_frame));
}

void AnimatedSprite::SetAnimation(const std::string& name, bool resetTimer) {
	if (mAnimName != name) {
		mAnimName = name;
	}
	
	if (resetTimer) {
		mAnimTimer = 0.0f;
	}
}

void AnimatedSprite::AddAnimation(const std::string& name,
								  const std::vector<SDL_Texture*>& images) {
	mAnims.emplace(name, images);
}
