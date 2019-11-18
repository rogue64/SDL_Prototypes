#pragma once
#include "Actor.h"
class CheckPoint : public Actor { 
public:
	CheckPoint(class Game* owner);
	~CheckPoint();
	void setActive(bool b);
	void setText(const std::string& in) { my_text = in; }
	const std::string& getText() const { return my_text; }
	void setLevelString(std::string level) { level_string = level; }
protected:
	class MeshComponent* my_meshc = nullptr;
	class CollisionComponent* my_cc = nullptr;
	bool active = false;
	std::string my_text = "";
	std::string level_string = "";
};

