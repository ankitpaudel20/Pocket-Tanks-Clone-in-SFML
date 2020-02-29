#include "Player.h"

Player::Player(b2World& world, float x, float y, std::string body_text,  std::string wheel_text, bool flip)
{
    
    score = 0;
    selected_weapon = new Weapon;
    *selected_weapon = Weapon::none;
    state = Player_state::idle;
    power = 50.f;

    b2FixtureDef Tankfixture, wheelfixure;
    Tankfixture.density = 6;
    Tankfixture.friction = .4;
    Tankfixture.restitution = 0.f;


    wheelfixure.density = 3;
    wheelfixure.friction = 10;
    wheelfixure.restitution = 0.1f;
    tank.setup(world, Tankfixture, wheelfixure, x, y, body_text, wheel_text,flip);
   
    tank.sfml_tank->barrel_angle = -PI / 4;


}

void Player::make(float dt, float mul, terrain& t, Tank& tank2) 
{
    

    if (state==Player_state::fired) {

        if (m_shot != nullptr) {

            if (m_shot->is_initialized()) {
                m_shot->make(dt, mul, t, tank, tank2);

                if ((m_shot->is_done())) {
                    delete m_shot;
                    t.need_update = true;
                    m_shot = nullptr;
                    state = Player_state::away;
                }
            }
        }

    }

}

void Player::fire()
{
    if (*selected_weapon == Weapon::none)
        return;

    if (m_shot == nullptr)
        m_shot = new shot;
    else
        return;


    switch (*selected_weapon) {

    case Weapon::single_shot:


        m_shot->init(tank.sfml_tank->muzzle, power * sf::Vector2f(cos(tank.sfml_tank->barrel_angle), sin(tank.sfml_tank->barrel_angle)), sf::Vector2f(0, 5), 1, 50);

        break;

    case Weapon::triple_shot:

        m_shot->init(tank.sfml_tank->muzzle, power * sf::Vector2f(cos(tank.sfml_tank->barrel_angle), sin(tank.sfml_tank->barrel_angle)), sf::Vector2f(0, 5), 3, 50);

        break;

    case Weapon::penta_shot:

        m_shot->init(tank.sfml_tank->muzzle, power * sf::Vector2f(cos(tank.sfml_tank->barrel_angle), sin(tank.sfml_tank->barrel_angle)), sf::Vector2f(0, 5), 5, 50);

        break;

    case Weapon::total_destroy:

        m_shot->init(tank.sfml_tank->muzzle, power * sf::Vector2f(cos(tank.sfml_tank->barrel_angle), sin(tank.sfml_tank->barrel_angle)), sf::Vector2f(0, 5), 2 , 20);
        dynamic_cast<shot*>(m_shot)->is_powerful = true;       
        break;

    }

    state = Player_state::fired;
}

void Player::draw(sf::RenderWindow& window)
{
    if (state==Player_state::fired) {
        if (m_shot != nullptr) {

            if (m_shot->is_initialized()) {
                m_shot->draw(window);
            }
        }
    }
}

Player::~Player() {
    //delete selected_weapon;
}