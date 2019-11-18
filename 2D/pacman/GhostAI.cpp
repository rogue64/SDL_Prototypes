#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"
#include <vector>
#include <utility>

GhostAI::GhostAI(class Actor* owner)
:Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
}

void GhostAI::Update(float deltaTime)
{
	// TODO: Implement
	Vector2 dir = mNextNode->GetPosition() - mPrevNode->GetPosition();
	dir = Vector2::Normalize(dir);
	
	// set animations depending on dir and states
	if (dir.x > 0) {
		mOwner->GetComponent<AnimatedSprite>()->SetAnimation("right");
		if (mState == State::Dead)
			mOwner->GetComponent<AnimatedSprite>()->SetAnimation("deadright");
	}
	else if (dir.x < 0) {
		mOwner->GetComponent<AnimatedSprite>()->SetAnimation("left");
		if (mState == State::Dead)
			mOwner->GetComponent<AnimatedSprite>()->SetAnimation("deadleft");
	}
	else if (dir.y > 0) {
		mOwner->GetComponent<AnimatedSprite>()->SetAnimation("down");
		if (mState == State::Dead)
			mOwner->GetComponent<AnimatedSprite>()->SetAnimation("deaddown");
	}
	else if (dir.y < 0) {
		mOwner->GetComponent<AnimatedSprite>()->SetAnimation("up");
		if (mState == State::Dead)
			mOwner->GetComponent<AnimatedSprite>()->SetAnimation("deadup");
	}

	// special checks if state is frightened
	if (mState == State::Frightened) {
		// change animations
		if (frighten_timer < ghost_const::FRIGHTEN_LIM)
			mOwner->GetComponent<AnimatedSprite>()->SetAnimation("scared0");
		else if (frighten_timer < ghost_const::FRIGHTEN_END)
			mOwner->GetComponent<AnimatedSprite>()->SetAnimation("scared1");
		// change state and speed
		else {
			mState = State::Scatter;
			my_spd = ghost_const::SCATTER_SPD;
		}
		frighten_timer += SEC * deltaTime;
	}

	mOwner->SetPosition(mOwner->GetPosition() + (dir * my_spd * deltaTime));



	if (mOwner->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>())) {
		mOwner->SetPosition(mNextNode->GetPosition());

		// teleport to other end of tunnel if collided with tunnel
		if (mNextNode == mOwner->GetGame()->mTunnelLeft) {
			mOwner->SetPosition(mOwner->GetGame()->mTunnelRight->GetPosition());
			mPrevNode = mOwner->GetGame()->mTunnelLeft;
			// "jump" to next set of nodes if there are any in mPath
			if (!mPath.empty()) {
				mNextNode = mPath[mPath.size() - 1];
				mPath.pop_back();
			}
			// else, set the next to be the new tunnel node
			else {
				mNextNode = mOwner->GetGame()->mTunnelRight;
			}
		}
		else if (mNextNode == mOwner->GetGame()->mTunnelRight) {
			mOwner->SetPosition(mOwner->GetGame()->mTunnelLeft->GetPosition());
			mPrevNode = mOwner->GetGame()->mTunnelRight;
			// "jump" to next set of nodes if there are any in mPath
			if (!mPath.empty()) {
				mNextNode = mPath[mPath.size() - 1];
				mPath.pop_back();
			}
			// else, set the next to be the new tunnel node
			else {
				mNextNode = mOwner->GetGame()->mTunnelLeft;
			}
		}

		// if path is empty, then make a new one
		if (mPath.empty()) {

			// if scattering try to set path to scatter node (no reversing); if you can't, path to the closest node
			if (mState == State::Scatter && !getPath(mNextNode, ((Ghost*)mOwner)->GetScatterNode())) {
				int min = -1;
				PathNode* newTarget = nullptr;
				for (auto x : mNextNode->mAdjacent) {
					// can't turn back
					if (x == mPrevNode || x == ((Ghost*)mOwner)->GetScatterNode())
						continue;
					int curr_dis = getDist( ((Ghost*)mOwner)->GetScatterNode() , x);
					if (min == -1 || curr_dis < min) {
						newTarget = x;
						min = curr_dis;
					}
				}
				getPath(mNextNode, newTarget);
			}
			
			// if frightened, make path to random adjacent node, no reversing
			if (mState == State::Frightened) {
				std::vector<PathNode*> options;
				for (auto x : mNextNode->mAdjacent)
					// no reversing or going into ghost-pen
					if (x != mPrevNode && x->GetType() != PathNode::Type::Ghost)
						options.push_back(x);
				int choice = Random::GetIntRange(0, options.size() - 1);
				getPath(mNextNode, options[choice]);
			}

			if (mState == State::Dead) {
				Start(mNextNode);
			}
		}
		// else set prev and next nodes to follow path
		else {
			mPrevNode = mNextNode;
			mNextNode = mPath[mPath.size() - 1];
			mPath.pop_back();
		}
	}
}

void GhostAI::Frighten()
{
	// TODO: Implement
	// set state
	mState = State::Frightened;
	frighten_timer = 0;
	my_spd = ghost_const::FRIGHTEN_SPD;
	// switch directions and create new frightened path
	PathNode* temp = mNextNode;
	mNextNode = mPrevNode;
	mPrevNode = temp;
	getPath(mPrevNode, mNextNode);
}

void GhostAI::Start(PathNode* startNode)
{
	// TODO: Implement

	// set position to given start-node
	mOwner->SetPosition(startNode->GetPosition());

	// start state machine at scatter state
	mState = State::Scatter;
	scatter_timer = 0;
	chase_timer = 0;

	// clear next, prev, and target node
	mNextNode = nullptr;
	mPrevNode = nullptr;
	mTargetNode = nullptr;

	// get path from startNode to scatter node
	getPath(startNode, ((Ghost*)mOwner)->GetScatterNode());
}

void GhostAI::Die()
{
	// TODO: Implement
	mState = State::Dead;
	//mOwner->SetPosition(mOwner->GetGame()->mPlayer->GetPosition());
	mOwner->SetPosition(mNextNode->GetPosition());
	my_spd = ghost_const::DEAD_SPD;
	//PathNode* tmp = mPrevNode;
	getPath(mNextNode, mOwner->GetGame()->mGhostPen);
	//mPrevNode = tmp;
	//getPath(mPrevNode, mOwner->GetGame()->mGhostPen);
}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mOwner->GetPosition().x),
			static_cast<int>(mOwner->GetPosition().y),
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y));
	}

	// Exit if no path
	if (mPath.empty())
	{
		return;
	}

	// Line from next node to subsequent on path
	SDL_RenderDrawLine(render,
		static_cast<int>(mNextNode->GetPosition().x),
		static_cast<int>(mNextNode->GetPosition().y),
		static_cast<int>(mPath.back()->GetPosition().x),
		static_cast<int>(mPath.back()->GetPosition().y));

	// Lines for rest of path
	for (size_t i = 0; i < mPath.size() - 1; i++)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mPath[i]->GetPosition().x),
			static_cast<int>(mPath[i]->GetPosition().y),
			static_cast<int>(mPath[i + 1]->GetPosition().x),
			static_cast<int>(mPath[i + 1]->GetPosition().y));
	}
}

bool GhostAI::getPath(PathNode* start_node, PathNode* target_node) {
	// clear current path no matter what happens
	mPath.clear();

	// if the start and target are same, there is no path; also checks if target is prev mode.. because then it will start in closed set
	if (start_node == target_node || mPrevNode == target_node)
		return false;

	// hash map to store opened nodes and their heuristic scores
	std::unordered_map<PathNode*, NodeData> opened;

	// place PrevNode in closed set (if there is one)
	if (mPrevNode) {
		NodeData prev_data;
		prev_data.closed = true;
		opened.insert(std::make_pair(mPrevNode, prev_data));
	}

	// place the start node and its heuristics into the open set
	NodeData start_data;
	start_data.h = getEuDist(start_node, target_node);
	start_data.f = start_data.h;
	// get the mPrevNode as parent if there is one set
	//start_data.parent = mPrevNode;
	opened.insert(std::make_pair(start_node, start_data));

	// while there are open nodes, evaluate them
	while (true) {
		// the min node is the open node with the smallest f(x)
		std::pair<PathNode*, NodeData> min = std::make_pair(nullptr, start_data);
		for (auto x : opened) {
			if ((!min.first || x.second.f < min.second.f) && !x.second.closed)
				min = x;
		}
		// if there is no node in the opened set, break
		if (!min.first)
			break;

		// place min in closed set
		min.second.closed = true;
		opened[min.first] = min.second;

		// now look at all the non-closed adjacent nodes
		for (size_t i = 0; i < min.first->mAdjacent.size(); i++) {
			std::unordered_map<PathNode*, NodeData>::iterator curr = opened.find(min.first->mAdjacent[i]);
			// if curr is not in the open set, calc hueristics and add to open set
			if (curr == opened.end()) {
				NodeData curr_data;
				// calc h(x)
				curr_data.h = getEuDist(min.first->mAdjacent[i], target_node);
				// cacl g(x)
				curr_data.g = min.second.g + getDist(min.first->mAdjacent[i], min.first);
				// calc f(x)
				curr_data.f = curr_data.g + curr_data.h;
				// set parent
				curr_data.parent = min.first;
				opened.insert(std::make_pair(min.first->mAdjacent[i], curr_data));
			}
			// else if the node is in the open set already (and not closed), check to see if curr_node is a better parent
			else if (!curr->second.closed) {
				// get new possible g(x)
				int new_g = min.second.g + getDist(curr->first, min.first);
				// check for adoption
				if (new_g < curr->second.g) {
					curr->second.g = new_g;
					curr->second.f = curr->second.g + curr->second.h;
					curr->second.parent = min.first;
				}
			}
			// else skip node because it is in closed set
			else
				continue;
		}
	}

	// if the target node was never opened, then no path to it
	if (opened.find(target_node) == opened.end() /*|| opened.find(target_node)->second.closed*/)
		return false; 

	mPrevNode = start_node;
	mTargetNode = target_node;
	PathNode* curr = mTargetNode;
	while (start_node != opened.find(curr)->second.parent) {
		mPath.push_back(curr);
		curr = opened.find(curr)->second.parent;
	}
	mNextNode = curr;
	return true;
}

int GhostAI::getEuDist(PathNode* a, PathNode* b) { 
	// distance to self and distance from tunnel to tunnel is 0
	if ((a == b) || (a->GetType() == PathNode::Type::Tunnel && b->GetType() == PathNode::Type::Tunnel))
		return 0;
	// return the smallest distance between a and b, taking tunnel-warps into account
	int min = (a->GetPosition() - b->GetPosition()).LengthSq();
	int tmp = (mOwner->GetGame()->mTunnelLeft->GetPosition() - a->GetPosition()).LengthSq()
			+ (b->GetPosition() - mOwner->GetGame()->mTunnelRight->GetPosition()).LengthSq();
	if (tmp < min)
		min = tmp;
	tmp = (mOwner->GetGame()->mTunnelRight->GetPosition() - a->GetPosition()).LengthSq()
		+ (b->GetPosition() - mOwner->GetGame()->mTunnelLeft->GetPosition()).LengthSq();
	if (tmp < min)
		min = tmp;
	//return min;
	return Math::Sqrt(min);
}

int GhostAI::getDist(PathNode* a, PathNode* b) {
	if ((a == b) || (a->GetType() == PathNode::Type::Tunnel && b->GetType() == PathNode::Type::Tunnel))
		return 0;
	//return (a->GetPosition() - b->GetPosition()).LengthSq();
	return (a->GetPosition() - b->GetPosition()).Length();
}