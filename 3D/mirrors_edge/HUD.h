#pragma once
#include "UIComponent.h"
#include <string>
#include "constants.h"

class HUD : public UIComponent {
public:
	HUD(Actor* owner);
	~HUD();
	// sets message to be displayed at center screen
	void setCPMessage(const std::string& in);
	// set the coin count displayed on HUD
	void setCoins(int coins);
protected:
	//const float CP_MESSAGE_LIMIT = 5.f;
	float cp_message_timer = CP_MESSAGE_LIMIT;
	float my_time = 0;
	std::string cp_string = "";
	class Font* my_font = nullptr;
	class Texture* my_cp_message_text = nullptr;
	class Texture* my_timer_text = nullptr;
	class Texture* my_coin_text = nullptr;
	class Texture* radar_back_text = nullptr;
	class Texture* radar_arrow_text = nullptr;
	class Texture* radar_blip_text = nullptr;
	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
	bool inRadarRange(class SecurityCam* in);
	Vector2 radarOffset(class SecurityCam* in);
};

