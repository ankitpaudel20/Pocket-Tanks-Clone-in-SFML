#include "Game.h"

Game::Game(contactlistener& listner_instance) :world(b2Vec2(0, 9.8)), window(sf::VideoMode(width, height), "main")
{

    //p1->tank.body.loadFromFile("body.png");

    p1 = new Player(world, inti_pos_player1.x, inti_pos_player1.y,"assets/body.png","assets/wheel.png",false);
    p2 = new Player(world, inti_pos_player2.x, inti_pos_player2.y, "assets/body_flipped.png", "assets/wheel.png",true);
    game_timing = nullptr;

    multiplier = 25;
    max_fps = w_max_fps;
    window.setFramerateLimit(max_fps);
    std::vector	<unsigned int>	values;
    values = get_values(400, width, 200, 100);
    t = new terrain(values, window);
    t->CreateGround(world);

    listner_instance.setup(p1->tank, p2->tank, *t);
    world.SetContactListener(&listner_instance);
    p1->tank.test = 1;
    p2->tank.test = 2;

    p1->armory.push_back(Weapon::total_destroy);
    p1->selected_weapon = &p1->armory[0];

    p2->armory.push_back(Weapon::penta_shot);
    p2->selected_weapon = &p2->armory[0];

    selected_player = p1;


    /* if (!bg.loadFromFile("space.jpg")) {
         std::cout << "error in loading space from file" << std::endl;
     }
     bg_s.setTexture(bg);
     bg_s.setOrigin(width / 2, height / 2);
     bg_s.setPosition(width / 2, height / 2);*/

   


    


    //p1->tank.sfml_tank->chasis.setTextureRect(sf::IntRect(0,));

    //test_s.setTexture(tank_texture);





    world.SetDebugDraw(&debugdraw);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    //flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    //flags += b2Draw::e_centerOfMassBit;
    debugdraw.SetFlags(flags);

}

std::vector<unsigned int> Game::get_values(unsigned int gnd_y, unsigned int end_x, unsigned int period_pixels, unsigned int amplitude)
{
    std::vector	<unsigned int> values;
    float increment = (PI / period_pixels);
    for (int i = 0; i < end_x; i++) {
        values.push_back(gnd_y + amplitude * sin(i * increment));
    }
    return values;
}

void Game::make(sf::Time& dt)
{
    if (selected_player->state == Player_state::away) {

        selected_player->state = Player_state::idle;

        if (selected_player == p1) {
            selected_player = p2;
            //selected_player->state = Player_state::idle;
            std::cout << "selected player is p2" << std::endl;
        }
        else if (selected_player == p2) {
            selected_player = p1;
            //selected_player->state == Player_state::idle;
            std::cout << "selected player is p1" << std::endl;
        }

    }

    //code to maintain the movement time of tanks
    if (game_timing != nullptr) {
        if (selected_player->state == Player_state::move_left) {
            if (game_timing->getElapsedTime().asSeconds() < player_move_time) {
                selected_player->tank.moveleft(*t);
            }
            else
            {
                selected_player->tank.stop(*t);
                selected_player->state = Player_state::idle;
                delete game_timing;
                game_timing = nullptr;
            }
        }
        else if (selected_player->state == Player_state::move_right) {
            if (game_timing->getElapsedTime().asSeconds() < player_move_time) {
                selected_player->tank.moveright(*t);
            }
            else
            {
                selected_player->tank.stop(*t);
                selected_player->state = Player_state::idle;
                delete game_timing;
                game_timing = nullptr;
            }
        }
    }
    //
    //

    //code to check if a joint is needed to make or destroy depending on the position of wheels and ground
    if (p1->state == Player_state::idle || p1->state == Player_state::away) {
        if (p1->tank.left_wheel_contact && p1->tank.right_wheel_contact) {

            if (p1->tank.chasis_joint == nullptr) {

                p1->tank.makejoint(*t);
                //std::cout << "joint made becauuse they are touuching of p1" << std::endl<< std::endl;

            }
        }
        else {
            if (p1->tank.chasis_joint != nullptr) {
                world.DestroyJoint(p1->tank.chasis_joint);
                //std::cout << "joint destroyed becaues in the air of p1" << std::endl<< std::endl;
                p1->tank.chasis_joint = nullptr;
            }
        }
    }

    if (p2->state == Player_state::idle || p2->state == Player_state::away) {

        if (p2->tank.left_wheel_contact && p2->tank.right_wheel_contact) {

            if (p2->tank.chasis_joint == nullptr) {

                p2->tank.makejoint(*t);
                //std::cout << "joint made becauuse they are touuching of p2" << std::endl;

            }
        }
        else {
            if (p2->tank.chasis_joint != nullptr) {
                world.DestroyJoint(p2->tank.chasis_joint);
                //std::cout << "joint destroyed becaues in the air of p2" << std::endl;
                p2->tank.chasis_joint = nullptr;
            }
        }

    }
    //
    //

    //Code to make the projectile of player
    p1->make(1 / float(max_fps), multiplier, *t,p2->tank);
    p2->make(1 / float(max_fps), multiplier, *t,p1->tank);
    //
    //

    //Code to simulate the world of box2d
    world.Step(dt.asSeconds(), 5, 8);
    //
    //

    //code to find right time to recompute chainshape of box2d world
    if (t->need_update) {
        if (!t->need_update_b2d)
            t->need_update_b2d = true;
    }

    if (t->need_update_b2d) {
        if (!t->need_update) {
            t->recomputechain(world);

            //check if fall is needed and update values of contact variables
            p1->tank.fall_if_needed(*t);
            p2->tank.fall_if_needed(*t);

        }
    }
    //
    //

    //Code to make terrain in sfml world depending on its case 
    t->make();
}

void Game::draw()
{
    window.clear();
    window.draw(bg_s);
    for (b2Body* BodyIterator = world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
    {

        if (BodyIterator->GetType() != b2_dynamicBody)
        {
            window.draw(*t);
            window.draw(t->get_surface());

        }
        else
        {
            float X = SCALE * BodyIterator->GetPosition().x;
            float Y = SCALE * BodyIterator->GetPosition().y;

            b2Body* body = static_cast<b2Body*> (BodyIterator->GetUserData());

            if (body == p1->tank.get_chasis())
            {
                /*sf::VertexArray gravity;
                gravity.setPrimitiveType(sf::LineStrip);
                gravity.append(sf::Vertex(sf::Vector2f(X, Y)));
                gravity.append(sf::Vertex(sf::Vector2f(X + world.GetGravity().x * SCALE/5, Y + world.GetGravity().y * SCALE/5)));
                window.draw(gravity);*/


                /*sf::VertexArray normal;
                normal.setPrimitiveType(sf::LineStrip);
                normal.append(sf::Vertex(sf::Vector2f(X, Y)));
                normal.append(sf::Vertex(sf::Vector2f(X + world.GetGravity().Skew().x * SCALE/5, Y + world.GetGravity().Skew().y * SCALE/5)));
                window.draw(normal);*/

                



                p1->tank.sfml_tank->chasis.setPosition(X, Y);
                p1->tank.sfml_tank->chasis.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);

                /* float t_width = 55 * 0.25;
                 float t_height = 15;
                 sf::ConvexShape test;
                 test.setFillColor(sf::Color::Magenta);
                 test.setPointCount(4);
                 test.setPoint(0, sf::Vector2f(-t_width * 0.5 * 0.5, -t_height * 0.5));
                 test.setPoint(1, sf::Vector2f(t_width * 0.5 * 0.5, -t_height * 0.5));
                 test.setPoint(2, sf::Vector2f(t_width * 0.5, t_height * 0.5));
                 test.setPoint(3, sf::Vector2f(-t_width * 0.5, t_height * 0.5));
                 test.setPosition(sf::Vector2f(sf::Vector2f(c_width * 0.25, -c_height)));
                 test.setOrigin(sf::Vector2f(t_width , 0));*/


                sf::Transform trans = p1->tank.sfml_tank->chasis.getTransform();
                sf::Vertex point(sf::Vector2f(0, p1->tank.sfml_tank->chasis.getPoint(0).y));
                sf::Vector2f p = trans.transformPoint(point.position);


                //window.draw(test_s,trans);
                window.draw(p1->tank.sfml_tank->chasis);

                sf::CircleShape test(3);
                test.setOrigin(test.getRadius(), test.getRadius());
                b2PolygonShape* temp = static_cast<b2PolygonShape*>(BodyIterator->GetFixtureList()->GetShape());

                for (int i = 0; i < temp->m_count; i++) {
                    b2Vec2 pos = temp->m_vertices[i];
                    test.setPosition(pos.x * SCALE, pos.y * SCALE);
                    window.draw(test, trans);
                }




                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                p1->tank.sfml_tank->barrel.setPosition(p.x, p.y);
                p1->tank.sfml_tank->barrel.setPosition(p.x, p.y);
                selected_player->tank.sfml_tank->point_to(static_cast<sf::Vector2f>(localPosition));
                window.draw(p1->tank.sfml_tank->barrel);

            }
            else if (body == p1->tank.get_leftwheel()) {
                p1->tank.sfml_tank->left_wheel.setPosition(X, Y);
                p1->tank.sfml_tank->left_wheel.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
                window.draw(p1->tank.sfml_tank->left_wheel);

                /*sf::CircleShape test(6);
                test.setOrigin(test.getRadius() * 0.5, test.getRadius() * 0.5);
                test.setPosition(p1->tank.sfml_tank->left_wheel.getPosition()- p1->tank.sfml_tank->left_wheel.getOrigin()/4.f);
                window.draw(test);*/

            }
            else if (body == p1->tank.get_rightwheel()) {
                p1->tank.sfml_tank->right_wheel.setPosition(X, Y);
                p1->tank.sfml_tank->right_wheel.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
                window.draw(p1->tank.sfml_tank->right_wheel);


            }

            if (body == p2->tank.get_chasis())
            {
                /*sf::VertexArray gravity;
                gravity.setPrimitiveType(sf::LineStrip);
                gravity.append(sf::Vertex(sf::Vector2f(X, Y)));
                gravity.append(sf::Vertex(sf::Vector2f(X + world.GetGravity().x * SCALE/5, Y + world.GetGravity().y * SCALE/5)));
                window.draw(gravity);


                sf::VertexArray normal;
                normal.setPrimitiveType(sf::LineStrip);
                normal.append(sf::Vertex(sf::Vector2f(X, Y)));
                normal.append(sf::Vertex(sf::Vector2f(X + world.GetGravity().Skew().x * SCALE/5, Y + world.GetGravity().Skew().y * SCALE/5)));
                window.draw(normal);*/


                /* sf::CircleShape test(10);
                 test.setPosition(c.sfml_tank->muzzle);
                 test.setOrigin(test.getRadius() * 0.5, test.getRadius() * 0.5);
                 window.draw(test);*/
                p2->tank.sfml_tank->chasis.setOrigin(body->GetLocalCenter().x, body->GetLocalCenter().y);
                p2->tank.sfml_tank->chasis.setPosition(X, Y);
                p2->tank.sfml_tank->chasis.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
                window.draw(p2->tank.sfml_tank->chasis);


                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                p2->tank.sfml_tank->barrel.setPosition(X, Y);
                selected_player->tank.sfml_tank->point_to(static_cast<sf::Vector2f>(localPosition));
                window.draw(p2->tank.sfml_tank->barrel);

            }
            else if (body == p2->tank.get_leftwheel()) {
                p2->tank.sfml_tank->left_wheel.setPosition(X, Y);
                p2->tank.sfml_tank->left_wheel.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
                window.draw(p2->tank.sfml_tank->left_wheel);
            }
            else if (body == p2->tank.get_rightwheel()) {
                p2->tank.sfml_tank->right_wheel.setPosition(X, Y);
                p2->tank.sfml_tank->right_wheel.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
                window.draw(p2->tank.sfml_tank->right_wheel);
            }


        }
    }

    /*sf::RectangleShape aabb;
    aabb.setFillColor(sf::Color::Transparent);
    aabb.setOutlineColor(sf::Color::Blue);
    aabb.setOutlineThickness(-2);
    aabb.setSize(sf::Vector2f(p1->tank.getaabb().width, p1->tank.getaabb().height));
    aabb.setPosition(sf::Vector2f(p1->tank.getaabb().left, p1->tank.getaabb().top));
    window.draw(aabb);
    aabb.setSize(sf::Vector2f(p2->tank.getaabb().width, p2->tank.getaabb().height));
    aabb.setPosition(sf::Vector2f(p2->tank.getaabb().left, p2->tank.getaabb().top));
    window.draw(aabb);*/


    world.DrawDebugData();
    p1->draw(window);
    p2->draw(window);

    window.display();
}

void Game::pollevents_states(sf::Event& event)
{


    while (window.pollEvent(event)) {

        switch (event.type)
        {

        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::A:
                if (selected_player->state == Player_state::idle) {
                    selected_player->state = Player_state::move_left;
                    if (game_timing == nullptr)
                        game_timing = new sf::Clock;
                }
                break;
            case sf::Keyboard::D:
                if (selected_player->state == Player_state::idle) {
                    selected_player->state = Player_state::move_right;
                    if (game_timing == nullptr)
                        game_timing = new sf::Clock;
                }
                break;
            case sf::Keyboard::R:
                if (selected_player->state == Player_state::idle)
                    selected_player == p1 ? selected_player->tank.respawn_Tank(false) : selected_player->tank.respawn_Tank(true);
                //selected_player->state = Player_state::idle;

                break;
            case sf::Keyboard::F:
                if (selected_player->state == Player_state::idle) {
                    selected_player->fire();
                    selected_player->state = Player_state::fired;
                }
                break;
            case sf::Keyboard::Q:
                if (selected_player == p1) {
                    selected_player = p2;
                    std::cout << "selected player is p2" << std::endl;
                }
                else if (selected_player == p2) {
                    selected_player = p1;
                    std::cout << "selected player is p1" << std::endl;
                }
                break;
            default:
                break;
            }

            break;



        case sf::Event::MouseButtonPressed:

            if (event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
                //std::cout << "mouse postion" << pos.x << "    " << pos.y << std::endl;
                t->destroy(pos, 50);
            }
            else {
                t->need_update = true;
            }

            break;

        case sf::Event::MouseWheelScrolled:
            //std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
            if (event.mouseWheelScroll.delta == 1)
                selected_player->set_power(selected_player->get_power() + 3);

            else
                selected_player->set_power(selected_player->get_power() - 3);
            std::cout << "the power is: " << selected_player->get_power() << std::endl;
            break;


        default:
            break;
        }


    }

    /*if (game_timing != nullptr)
        std::cout << "timer :" << game_timing->getElapsedTime().asMicroseconds() << std::endl;*/

    sf::Vector2i localPosition = sf::Mouse::getPosition(window);

    selected_player->tank.sfml_tank->point_to((sf::Vector2f) localPosition);

}

void Game::run()
{
    sf::Event event;
    sf::Clock frame_clk;

    while (window.isOpen()) {
        sf::Time dt = frame_clk.restart();
        pollevents_states(event);

        //std::cout << "running game loop" << std::endl;
        make(dt);
        draw();
    }
}

Game::~Game() {
    delete t;
    delete p1;
    delete p2;
}