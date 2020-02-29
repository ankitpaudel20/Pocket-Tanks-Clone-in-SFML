#include"Tank.h"




void Tank::create_chasis(float x, float y,bool flip) {
	
	chasis_joint = nullptr;
	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position.Set(x / SCALE, y / SCALE);
	bodydef.angle = 0;

	sfml_tank = new sftank;

	chasis_contact = false;
	//chasis
	b2PolygonShape chasisShape;
	//std::vector <b2Vec2> p;
	points.reserve(7);
	points.push_back(b2Vec2((c_width / 4) / SCALE, -(c_height / 2) / SCALE));
	points.push_back(b2Vec2(-(c_width / 2) / SCALE, -(c_height / 4) / SCALE));
	points.push_back(b2Vec2(-(c_width / 2) / SCALE, 0 / SCALE));
	points.push_back(b2Vec2(-(c_width / 2) / SCALE, (c_height / 2) / SCALE));
	points.push_back(b2Vec2((c_width / 4) / SCALE, (c_height / 2) / SCALE));
	points.push_back(b2Vec2((c_width / 2) / SCALE, -(c_height / 5) / SCALE));

	std::vector<b2Vec2> temp;
	temp = points;


	if (flip) {		
		for (int i = 0; i < points.size(); i++) {
			temp[i].x = -points[i].x;
		}
	}


	chasisShape.Set(&temp[0], temp.size());

	chasis = c_world->CreateBody(&bodydef);
	chasis->SetUserData(this->chasis);
	m_chasisdef.shape = &chasisShape;
	chasis->CreateFixture(&m_chasisdef);



	//setup chasis in sfml world
	sfml_tank->chasis.setPointCount(temp.size());
	for (int i = 0; i < temp.size(); i++)
		sfml_tank->chasis.setPoint(i, sf::Vector2f(temp[i].x * SCALE, temp[i].y * SCALE));
	//sfml_tank->chasis.setFillColor(sf::Color(138, 179, 69, 255));
	sfml_tank->chasis.setTexture(&body_texture);
	if(flip)
		std::cout << "made a new sfml Tank for p2" <<  std::endl;
	else
		std::cout << "made a new sfml Tank for p1" << std::endl;


	sfml_tank->barrel.setSize(sf::Vector2f(30, 2));
	sfml_tank->barrel.setOrigin(sf::Vector2f(0, sfml_tank->barrel.getSize().y * 0.5));
	sfml_tank->barrel.setPosition(sfml_tank->chasis.getPosition());
	sfml_tank->barrel.setFillColor(sf::Color::Blue);

	//Top shape
	//float t_width = c_width * 0.25;
	//float t_height = c_height;

	//sfml_tank->top.setFillColor(sf::Color::Magenta);
	//sfml_tank->top.setPointCount(4);
	//sfml_tank->top.setPoint(0, sf::Vector2f(-t_width * 0.5 * 0.5, -t_height * 0.5));
	//sfml_tank->top.setPoint(0, sf::Vector2f( t_width * 0.5 * 0.5, -t_height * 0.5));
	//sfml_tank->top.setPoint(0, sf::Vector2f( t_width * 0.5,		t_height * 0.5));
	//sfml_tank->top.setPoint(0, sf::Vector2f(-t_width * 0.5,		t_height * 0.5));
 // /*sfml_tank->top.setPoint(0, sf::Vector2f(-t_width , -t_height ));
	//sfml_tank->top.setPoint(1, sf::Vector2f( t_width , -t_height ));
	//sfml_tank->top.setPoint(2, sf::Vector2f( t_width , t_height ));
	//sfml_tank->top.setPoint(3, sf::Vector2f(-t_width , t_height ));*/
	//sfml_tank->top.setPosition(sf::Vector2f(c_width*0.25, -c_height));
	//sfml_tank->top.setOrigin(t_width,0);

	//b2PolygonShape topshape;
	//std::vector <b2Vec2> po;
	//
	//po.reserve(4);
	//po.push_back(b2Vec2(-t_width / SCALE, -t_height / SCALE));
	//po.push_back(b2Vec2(t_width / SCALE, -t_height / SCALE));
	//po.push_back(b2Vec2(t_width / SCALE, t_height / SCALE));
	//po.push_back(b2Vec2(-t_width / SCALE, t_height / SCALE));
	//
	//
	//
	//topshape.Set(&p[0], 4);
	//m_topdef.shape = &topshape;	
	//bodydef.position = b2Vec2(x / SCALE, (y + c_height) / SCALE);
	//top = c_world->CreateBody(&bodydef);
	//
	//chasis->CreateFixture(&m_topdef);
	//

	//b2DistanceJointDef jointdef;
	//
	//jointdef.bodyA = chasis;
	//jointdef.bodyB = top;
	//jointdef.localAnchorA = b2Vec2(0, c_height/SCALE);
	//jointdef.localAnchorB = b2Vec2(0, -c_height/SCALE);
	//jointdef.length = abs(-c_height / SCALE) + abs(-c_height / SCALE);
	//jointdef.frequencyHz = 0;
	//jointdef.dampingRatio = 1;
	//jointdef.collideConnected = true;
	//c_world->CreateJoint(&jointdef);

}

void Tank::create_wheels(float x, float y)
{
	left_joint = nullptr;
	right_joint = nullptr;

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position.Set(x / SCALE, y / SCALE);
	bodydef.angle = 0;
	bodydef.angularDamping = 10;

	b2CircleShape wheelshape;
	wheelshape.m_radius = (20 / 2) / SCALE;

	m_wheeldef.shape = &wheelshape;

	//left Wheel
	left_wheel_contact = false;
	left_wheel_contact_point[0] = b2Vec2(0, 0);
	left_wheel_contact_point[1] = b2Vec2(0, 0);

	leftwheel = c_world->CreateBody(&bodydef);
	leftwheel->SetUserData(this->leftwheel);
	leftwheel->CreateFixture(&m_wheeldef);


	//right Wheel
	right_wheel_contact = false;
	right_wheel_contact_point[0] = b2Vec2(0, 0);
	right_wheel_contact_point[1] = b2Vec2(0, 0);

	rightwheel = c_world->CreateBody(&bodydef);
	rightwheel->SetUserData(this->rightwheel);
	rightwheel->CreateFixture(&m_wheeldef);


	sfml_tank->left_wheel.setRadius(wheelshape.m_radius * SCALE);
	//sfml_tank->left_wheel.setFillColor(sf::Color::Red);
	sfml_tank->left_wheel.setOrigin(wheelshape.m_radius * SCALE, wheelshape.m_radius * SCALE);
	sfml_tank->left_wheel.setTexture(&wheels_texture);


	sfml_tank->right_wheel.setRadius(wheelshape.m_radius * SCALE);
	//sfml_tank->right_wheel.setFillColor(sf::Color::Blue);
	sfml_tank->right_wheel.setOrigin(wheelshape.m_radius * SCALE, wheelshape.m_radius * SCALE);
	sfml_tank->right_wheel.setTexture(&wheels_texture);


	b2WheelJointDef axisdef;
	axisdef.bodyA = chasis;
	axisdef.bodyB = leftwheel;
	axisdef.localAnchorA.Set(-c_width * 0.5 * 0.6 / SCALE, c_height * 1.5 * 0.45 / SCALE);
	axisdef.frequencyHz = 10;
	axisdef.localAxisA.Set(0, 1);

	axisdef.maxMotorTorque = (m_chasisdef.density + m_wheeldef.density) * 50;
	leftaxis = (b2WheelJoint*)c_world->CreateJoint(&axisdef);


	axisdef.bodyB = rightwheel;
	axisdef.localAnchorA.x *= -1;
	axisdef.localAxisA.Set(0, 1);

	rightaxis = (b2WheelJoint*)c_world->CreateJoint(&axisdef);
}

Tank::Tank() {}

Tank::Tank(b2World& world, b2FixtureDef& chasisdef, b2FixtureDef& wheeldef, float x, float y,  std::string body_text,  std::string wheel_text,bool flip)
{
	setup(world, chasisdef, wheeldef, x, y,body_text,wheel_text, flip);

}

void Tank::setup(b2World& world, b2FixtureDef& chasisdef, b2FixtureDef& wheeldef, float x, float y,  std::string body_text,  std::string wheel_text,bool flip) {
	
	body_texture.loadFromFile(body_text);
	wheels_texture.loadFromFile(wheel_text);


	right_wheel_contact = false;
	left_wheel_contact = false;
	stop_delayed = false;
	c_world = &world;
	m_chasisdef = chasisdef;
	m_wheeldef = wheeldef;
	
	m_topdef = chasisdef;
	frames_in_air = 1;

	

	right_wheel_contact_point[0] = b2Vec2(0, 0);
	right_wheel_contact_point[1] = b2Vec2(0, 0);
	left_wheel_contact_point[0] = b2Vec2(0, 0);
	left_wheel_contact_point[1] = b2Vec2(0, 0);

	create_chasis(x, y,flip);
	create_wheels(x, y);
	max_normal_force = world.GetGravity().Normalize() * (chasis->GetMass() + leftwheel->GetMass() + rightwheel->GetMass());

}


void Tank::moveright(terrain& t)
{
	//std::cout << "moveright from p" << test << std::endl;

	if (chasis_joint != nullptr) {
		c_world->DestroyJoint(chasis_joint);
		std::cout << "leftjoint destroyed of p" << test <<"while miving right"<< std::endl << std::endl;

		chasis_joint = nullptr;
	}

	applynormalforce(t);

	leftaxis->EnableMotor(true);
	rightaxis->EnableMotor(true);
	leftaxis->SetMotorSpeed(+motorspeed);
	rightaxis->SetMotorSpeed(+motorspeed);
	if (!left_wheel_contact && !right_wheel_contact) {
		if (frames_in_air > fall_limit)
			c_world->SetGravity(9.8 * b2Vec2(0, 1));
		return;
	}
}

void Tank::moveleft(terrain& t)
{
	//std::cout << "moveleft from p" << test << std::endl;
	if (chasis_joint != nullptr) {
		c_world->DestroyJoint(chasis_joint);
		std::cout << "leftjoint destroyed of p" << test <<" while moving left"<< std::endl << std::endl;
		
		chasis_joint = nullptr;
		
	}
	applynormalforce(t);

	leftaxis->EnableMotor(true);
	rightaxis->EnableMotor(true);
	leftaxis->SetMotorSpeed(-motorspeed);
	rightaxis->SetMotorSpeed(-motorspeed);
	if (!left_wheel_contact && !right_wheel_contact) {
		if (frames_in_air > fall_limit)
			c_world->SetGravity(9.8 * b2Vec2(0, 1));
		return;
	}
}


void Tank::makejoint(terrain& t) {
	b2WeldJointDef jointdef;

	jointdef.Initialize(chasis, t.getchainbody(), chasis->GetPosition());
	
	chasis_joint = c_world->CreateJoint(&jointdef);
	//std::cout << "leftjoint created of p" << test << std::endl;

	
}

void Tank::stop(terrain &t) {
	if (leftaxis->GetMotorSpeed() != 0) {
		leftaxis->SetMotorSpeed(0);
		leftaxis->EnableMotor(false);
	}

	if (rightaxis->GetMotorSpeed() != 0) {
		rightaxis->SetMotorSpeed(0);
		rightaxis->EnableMotor(false);
	}

	if (chasis->GetLinearVelocity() != b2Vec2(0, 0)) {
		b2Vec2 imp_dir = c_world->GetGravity().Skew();
		imp_dir.Normalize();
		imp_dir *= chasis->GetLinearVelocity().Length();
		chasis->ApplyLinearImpulseToCenter(imp_dir, true);
	}

	if (c_world->GetGravity() != b2Vec2(0, 9.8)) {
		c_world->SetGravity(9.8 * b2Vec2(0, 1));
	}

	/*if (left_wheel_contact || right_wheel_contact) 
		makejoint(t);*/
	
	//frames_in_air = 0;

	

	
}

void Tank::applynormalforce(terrain& t)
{
	if ((left_wheel_contact == true) || (right_wheel_contact == true)) {
		b2Vec2 move_dir = rightwheel->GetPosition() - leftwheel->GetPosition();
		move_dir.Normalize();
		b2Vec2 fake_gravity1 = t.get_chain_point(chasis->GetPosition()).Skew();
		b2Vec2 fake_gravity2 = move_dir.Skew();
		fake_gravity1.Normalize();
		
		b2Vec2 fake_gravity = fake_gravity1 + fake_gravity2;
		fake_gravity.Normalize();
		c_world->SetGravity(9.8 * fake_gravity);;

		frames_in_air = 0;

		b2Vec2 velocity = chasis->GetLinearVelocity();
		velocity.Normalize();
		chasis->ApplyForceToCenter(fake_gravity.Skew() * 2, true);
	}
	else if (left_wheel_contact == false && right_wheel_contact == false) {

		frames_in_air++;
		if (frames_in_air > fall_limit) {
			c_world->SetGravity(9.8 * b2Vec2(0, 1));
			frames_in_air = 0;
		}
	}


}


void Tank::respawn_Tank(bool flip)
{
	b2Vec2 current_Tank_pos;

	current_Tank_pos = get_chasis()->GetPosition();
	c_world->DestroyBody(get_chasis());
	c_world->DestroyBody(get_rightwheel());
	c_world->DestroyBody(get_leftwheel());
	delete sfml_tank;
	create_chasis(current_Tank_pos.x * SCALE, 200,flip);
	create_wheels(current_Tank_pos.x * SCALE, 200);
	frames_in_air = 1;
}

void Tank::fall_if_needed(terrain &t)
{
	left_wheel_contact = false;
	right_wheel_contact = false;
	auto surface = t.get_surface();
	float radius = sfml_tank->left_wheel.getRadius();
	sf::Vector2f origin = sfml_tank->left_wheel.getOrigin();

	sf::Vector2f center = sfml_tank->left_wheel.getPosition() - origin / 4.f;
	for (int i = center.x - radius; i < center.x + radius; i++) {
		float distance = t.distance(surface[i].position, center);
		if (distance <= radius) {
			left_wheel_contact = true;
		}
	}

	center = sfml_tank->right_wheel.getPosition() - origin / 4.f;
	for (int i = center.x - radius; i < center.x + radius; i++) {
		float distance = t.distance(surface[i].position, center);
		if (distance <= radius) {
			right_wheel_contact = true;
		}
	}
}

