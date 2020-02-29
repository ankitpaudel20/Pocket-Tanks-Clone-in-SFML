#pragma once
#include"src/terrain.h"
#include"src/b2GLDraw.h"
#include"src/contact_listener.h"
#include"src/Player.h"
#define SCALE 72.f


static const float player_move_time = 2.0f;


static const b2Vec2 inti_pos_player1(400, 200);
static const b2Vec2 inti_pos_player2(width - 400, 200);

class Game
{
private:
	b2World world;
	sf::RenderWindow window;
	terrain* t;
	b2GLDraw debugdraw;
	Player* p1;
	Player* p2;
	Player* selected_player;
	float multiplier;
	sf::Clock* game_timing;
	unsigned max_fps;

	//sf::Sprite test_s;
	sf::Texture bg;
	sf::Sprite  bg_s;

	void pollevents_states(sf::Event& event);
	void make(sf::Time& dt);
	void draw();
public:
	Game(contactlistener& listner_instance);

	std::vector<unsigned int> get_values(unsigned int gnd_y, unsigned int end_x, unsigned int period_pixels, unsigned int amplitude);
	void run();

	~Game();
};

