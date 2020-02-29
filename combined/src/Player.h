#pragma once
#include<iostream>

#include"SFML/Graphics.hpp"
#include"box2d/box2d.h"
#include"Tank.h"
#include"all_commons.h"
#include"weaponsfile.h"


enum class Player_state{idle,fired,move_right,move_left,away};


class Player
{
private:
	projectile* m_shot;
	float power;
	int score;

public:
	Tank tank;
	std::vector<Weapon> armory;
	Weapon* selected_weapon;
	Player_state state;


	Player(b2World& world, float x, float y, const std::string body_path, const std::string top_path, bool flip);

	void set_power(float p) { power = p; }
	float get_power() { return power; }
	void make(float dt, float mul, terrain& t, Tank& tank2);
	void fire();
	void draw(sf::RenderWindow& window);
	~Player();
};

