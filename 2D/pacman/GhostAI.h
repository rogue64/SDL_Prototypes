#pragma once
#include "constants.h"
#include "Component.h"
#include "Math.h"
#include <vector>

class PathNode;

class GhostAI : public Component
{
public:
	// Used to track the four different GhostAI states
	enum State
	{
		Scatter,
		Chase,
		Frightened,
		Dead
	};

	struct NodeData {
		int f = 0;
		int h = 0;
		int g = 0;
		bool closed = false;
		PathNode* parent = nullptr;
	};
	
	GhostAI(class Actor* owner);

	void Update(float deltaTime) override;
	
	// Called when the Ghost starts at the beginning
	// (or when the ghosts should respawn)
	void Start(class PathNode* startNode);
	
	// Get the current state
	State GetState() const { return mState; }
	
	// Called when the ghost should switch to the "Frightened" state
	void Frighten();
	
	// Called when the ghost should switch to the "Dead" state
	void Die();

	//  Helper function to draw GhostAI's current path
	void DebugDrawPath(struct SDL_Renderer* render);
private:
	// Member data for pathfinding

	// TargetNode is our current goal node
	class PathNode* mTargetNode = nullptr;
	// PrevNode is the last node we intersected
	// with prior to the current position
	class PathNode* mPrevNode = nullptr;
	// NextNode is the next node we're trying
	// to get to
	class PathNode* mNextNode = nullptr;

	// This vector always contains the path
	// from "next node" to "target node"
	// (if there is still such a path)
	std::vector<class PathNode*> mPath;

	// Current state of the Ghost AI
	State mState = Scatter;

	// Save the owning actor (cast to a Ghost*)
	class Ghost* mGhost;

	// TODO: Add any member data/helper functions here!

	float frighten_timer = 0;

	float my_spd = ghost_const::SCATTER_SPD;

	float chase_timer = 0;

	float scatter_timer = 0;
	
	// returns vector of PathNodes to mTargetNode using A*
	bool getPath(PathNode* start_node, PathNode* target_node);

	// returns Euclidean Distance (squared) from node a to node b
	int getEuDist(PathNode* a, PathNode* b);

	// returns regular distance (squared) from node a to b
	int getDist(PathNode* a, PathNode* b);
};
