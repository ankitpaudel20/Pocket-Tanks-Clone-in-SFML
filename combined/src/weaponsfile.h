#pragma once
#include"SFML/Graphics.hpp"
#include"projectile.h"
#include"terrain.h"
#include"collision.h"


class shot : public projectile {
	//friend class player;
protected:
	//bool done = false;
	std::vector<particle> m_particle;
	float blast_radius;
public:
	bool is_powerful;	

	std::vector<sf::Vector2f> getpos() {
		std::vector<sf::Vector2f> pos;
		for (int i = 0; i < m_particle.size(); i++) {
			pos.push_back(m_particle[i].shape.getPosition());
		}
		return pos;
	}

	virtual void init(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f accln, int no, float blast_r) {
		blast_radius = blast_r;
		m_particle.resize(no);
		int a = no/2;
		float j = 1 - 0.1 * a;
	

		for (int i = 0; i < no; i++) {
			m_particle[i].m_vel = vel * j;
			m_particle[i].m_accln = accln;
			m_particle[i].shape.setRadius(2);
			m_particle[i].shape.setOrigin(5, 5);
			m_particle[i].shape.setPosition(pos);
			m_particle[i].in_bound = true;
			m_particle[i].draw = true;
			j = j + 0.1;
		}
		initialized = true;
		in_bound = true;
	}


	virtual void draw(sf::RenderWindow& window)
	{
		//window.draw(*m_particle[0].shape);
		for (int i = 0; i < m_particle.size(); i++) {
			if (m_particle[i].draw) {
			
				//std::cout << "proj drawn" << std::endl;
				window.draw(m_particle[i].shape);
				
			}
		}
	}

	virtual void make(float dt, float multiplier, terrain& t, Tank& tank, Tank& tank1)
	{
		float time = dt * multiplier;
		//std::cout << "make is being called" << std::endl;
		for (int i = 0; i < m_particle.size(); i++) {
			if (m_particle[i].draw) {
				m_particle[i].m_vel = m_particle[i].m_vel + m_particle[i].m_accln * time;
				m_particle[i].shape.move(m_particle[i].m_vel.x * time, m_particle[i].m_vel.y * time);
				//std::cout << "position of projectile is " << m_particle[i].shape.getPosition().x << "," << m_particle[i].shape.getPosition().y << std::endl;
				//std::cout << "make called" << std::endl;

				if (t.circle_collides(m_particle[i].shape)) {

					t.destroy(m_particle[i].shape.getPosition(), blast_radius);
					
					if(!is_powerful)
						m_particle[i].draw=false;
				}
				b2Vec2 projectile_pos(m_particle[i].shape.getPosition().x / SCALE, m_particle[i].shape.getPosition().y / SCALE);

				sf::Transform trans = tank.sfml_tank->chasis.getTransform();
				sf::Transform trans1 = tank1.sfml_tank->chasis.getTransform();

				std::vector<sf::Vector2f> vertices,vertices1;
				vertices.reserve(tank.sfml_tank->chasis.getPointCount());
				vertices1.reserve(tank1.sfml_tank->chasis.getPointCount());

				for (int i = 0; i < tank.sfml_tank->chasis.getPointCount(); i++) {
					vertices.push_back(tank.sfml_tank->chasis.getPoint(i));
					vertices[i] = trans.transformPoint(vertices[i]);
					vertices1.push_back(tank1.sfml_tank->chasis.getPoint(i));
					vertices1[i] = trans1.transformPoint(vertices1[i]);
				}

				if (polygonPoint(vertices, projectile_pos.x, projectile_pos.y)) {
					std::cout << "hit oneself" << std::endl;
					t.destroy(m_particle[i].shape.getPosition(), blast_radius);

					if (!is_powerful)
						m_particle[i].draw = false;
				}

				if (polygonPoint(vertices1, projectile_pos.x, projectile_pos.y)) {
					std::cout << "hit other tank" << std::endl;
					t.destroy(m_particle[i].shape.getPosition(), blast_radius);

					if (!is_powerful)
						m_particle[i].draw = false;
				}
				



				/*if (tank.get_chasis()->GetFixtureList()->TestPoint(projectile_pos)) {
					std::cout << "hit oneself" << std::endl;
					t.destroy(m_particle[i].shape.getPosition(), blast_radius);

					if (!is_powerful)
						m_particle[i].draw = false;
				}

				if (tank1.get_chasis()->GetFixtureList()->TestPoint(projectile_pos)) {
					std::cout << "hit other tank" << std::endl;
					t.destroy(m_particle[i].shape.getPosition(), blast_radius);

					if (!is_powerful)
						m_particle[i].draw = false;
				}*/

				


				if (m_particle[i].draw) {
					if (!(sf::IntRect(0, -1000, width, 1768).contains((sf::Vector2i)m_particle[i].shape.getPosition()))) {
						m_particle[i].draw = false;
						//std::cout << "projectile" << i << "is out of bound" << std::endl;
					}
				}
			}
			
				//std::cout << "projectile" << i << "not made" << std::endl;
		}

		done = true;
		
		for (int i = 0; i < m_particle.size(); i++)
			if (m_particle[i].draw)
				done = false;
		

	}

	~shot() {
		initialized = false;
		
	}
	
};


