#include "HUD.h"
#include <SDL/SDL_ttf.h>
#include "Font.h"
#include "constants.h"
#include <string>
#include <sstream>
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include <cstdio>
#include "constants.h"
#include "SecurityCam.h"

HUD::HUD(Actor* owner) : UIComponent(owner) {
	// load font and timer texture
	my_font = new Font();
	my_font->Load("Assets/Inconsolata-Regular.ttf");
	my_timer_text = my_font->RenderText("00:00.00");
	// load radar textures
	radar_back_text = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Radar.png");
	radar_arrow_text = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/RadarArrow.png");
	radar_blip_text = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Blip.png");
	// set coin counter to whatever the game has set
	setCoins(mOwner->GetGame()->getCoins());
	// set message for game hud if there is one
	setCPMessage(mOwner->GetGame()->getCPString());
}

HUD::~HUD() {
	// store cp-message string in game
	mOwner->GetGame()->setCPString(cp_string);
}

void HUD::Update(float deltaTime) {
	float my_time = mOwner->GetGame()->getTime();
	int ff = ((int)(my_time * 100)) % 100;
	int ss = ((int)my_time) % 60;
	int mm = ((int)my_time) / 60;
	if (my_timer_text) {
		delete my_timer_text;
		char tmp[8 + 1]; // 8 characters plus the '\0' character
		sprintf_s(tmp, 8 + 1, "%02d:%02d.%02d", mm, ss, ff);
		my_timer_text = my_font->RenderText(tmp);
	}
	// increase message timer
	if (cp_message_timer < CP_MESSAGE_LIMIT)
		cp_message_timer += deltaTime;
	// delete message when timer runs out;
	else if (my_cp_message_text) {
		delete my_cp_message_text;
		my_cp_message_text = nullptr;
		cp_string = "";
	}
}

void HUD::Draw(Shader* shader) {
	// draws timer-text texture to the bottom left of screen
	DrawTexture(shader, my_timer_text, Vector2(-420.0f, -325.0f));
	// draws the checkpoint message at center of screen
	if (my_cp_message_text)
		DrawTexture(shader, my_cp_message_text, Vector2::Zero);
	// draw radar at bottom right of screen
	DrawTexture(shader, radar_back_text, Vector2(400.0f, -275.0f));
	DrawTexture(shader, radar_arrow_text, Vector2(400.0f, -275.0f));
	// draw all the security cams on the radar
	std::vector<SecurityCam*> game_cams = mOwner->GetGame()->getCams();
	for (int i = 0; i < game_cams.size(); i++) {
		if (inRadarRange(game_cams[i]))
			DrawTexture(shader, radar_blip_text, Vector2(400.0f, -275.0f) + radarOffset(game_cams[i]));
	}
	// draw coin counter above timer
	if (my_coin_text)
		DrawTexture(shader, my_coin_text, Vector2(-420.0f, -300.0f));
}

void HUD::setCPMessage(const std::string& in) {
	cp_string = in;
	if (my_cp_message_text)
		delete my_cp_message_text;
	if (in == "") {
		my_cp_message_text = nullptr;
		return;
	}
	// sets the current texture to be applied to HUD
	my_cp_message_text = my_font->RenderText(in);
	// reset timer for the message to be displayed
	cp_message_timer = 0;
}

void HUD::setCoins(int coins) {
	// turn counter into a formatted string
	char tmp[5 + 1];
	sprintf_s(tmp, 5 + 1, "%02d/%02d", coins, MAX_COINS);
	// create a texture from string
	if (my_coin_text)
		delete my_coin_text;
	my_coin_text = my_font->RenderText(tmp);
}

bool HUD::inRadarRange(class SecurityCam* in) {
	Vector2 player_pos = Vector2(mOwner->GetPosition().x, mOwner->GetPosition().y);
	Vector2 cam_pos = Vector2(in->GetPosition().x, in->GetPosition().y);
	return (player_pos - cam_pos).LengthSq() < (RADAR_RANGE * RADAR_RANGE);
}

Vector2 HUD::radarOffset(class SecurityCam* in) {
	Vector2 player_pos = Vector2(mOwner->GetPosition().x, mOwner->GetPosition().y);
	Vector2 cam_pos = Vector2(in->GetPosition().x, in->GetPosition().y);
	Vector2 p_to_c = cam_pos - player_pos;
	Vector2 p_forward = Vector2(mOwner->GetForward().x, mOwner->GetForward().y);
	Matrix3 rotation = Matrix3::CreateRotation(Math::Atan2(-p_forward.y, p_forward.x));
	p_to_c.x /= RADAR_RANGE;
	p_to_c.y /= RADAR_RANGE;
	p_to_c.x *= 92;
	p_to_c.y *= 92;
	return Vector2::Transform(p_to_c, rotation);
}