#pragma once

// a file to store all constants, enums, and forward class-declarations used for the game

// forward declarations for classes (good idea? well it fixed my annoying bugs)
class Game;
class Actor;
class Player;
class Collider;
class Secret;
class Door;
class Spawner;
class Component;
class MoveComponent;
class PlayerMovComp;
class CollisionComponent;
class SpriteComponent;
class AnimatedSprite;
class TiledBGComp;

// main constants
static const int WINDOW_W = 600;
static const int WINDOW_H = 448;
static const int SEC = 1000; // a second in ms
static const int FPS = 60;
static const int DELTA_MIN = SEC / FPS;
static const float DELTA_CAP = 0.033f;
static const float GRAV = 2000; // gravity, pixels per second per second
static const float WORLD_BOTTOM = 448; // y-pos of bottom of world so that objects stop falling
static const int TILE_SIZE = 32;

// constants for player
namespace player_const {
	static const int SIZE = 20;
	static const float M_SPEED = 130.f;
};

// enum class for actor class
enum class my_actorstate {
	Active,
	Paused,
	Destroy
};

// enum class for collision component
enum class CollSide {
	None,
	Top,
	Bottom,
	Left,
	Right
};

// enum classes for door class
enum class Direction {
	Left,
	Right,
	Up,
	Down
};

enum class DoorState {
	Open,
	Closed,
	Locked
};