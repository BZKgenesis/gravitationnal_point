#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

const int H = 720;
const int W = 1280;
int G = 2500;

class Particle {
public:
    bool visible = true;
    sf::Vector2f pos;
    sf::Vector2f vel;
    float size;
    sf::Vector2f acc = sf::Vector2f(0,0);
    sf::Color color = sf::Color::White;
    Particle(sf::Vector2f position, sf::Vector2f velocity, float size) : pos(position), vel(velocity), size(size) {};


    void draw(sf::RenderWindow& win) {
        sf::CircleShape circle = sf::CircleShape(1);
        circle.setPosition(pos);
        circle.setFillColor(color);
        circle.setOutlineColor(color);
        win.draw(circle);
    }

    void update(float dt) {
        vel += acc*dt;
        pos += vel*dt;
    }
};

sf::Vector2f forceGravitationel(Particle p1, Particle p2) {
    sf::Vector2f d = p1.pos - p2.pos;
    float dist = d.x * d.x + d.y * d.y;
    
    float force = G / std::max( dist,500.f);
    

    sf::Vector2f a = force * d * p1.size* p2.size / sqrt(dist);
    return a;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(W, H), "interraction gravitationnel");
    sf::Clock deltaClock;
    window.setFramerateLimit(600);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    Particle pCentre = Particle(sf::Vector2f(W / 2, H / 2), sf::Vector2f(0, 0),20);
    int nbParticle = 150;
    std::vector<Particle> particules;
    for (int i = 0; i < nbParticle; i++) {
        sf::Vector2f p = sf::Vector2f(rand() % W+1, rand() % H+1);
        sf::Vector2f v = sf::Vector2f(-(p.y - pCentre.pos.y), (p.x - pCentre.pos.x));
        particules.push_back(Particle(p, v*0.1f,10));
    }

    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.scancode == sf::Keyboard::Scan::Escape)
                    window.close();
            }
        }
        sf::Time dt = deltaClock.restart();

        window.clear();
        float deltaT = dt.asSeconds();
        sf::Vector2f accel;
        for (int i = 0; i < nbParticle; i++) {
            if (particules[i].visible) {
                if (particules[i].pos.x < 0 && particules[i].vel.x < 0) {
                    particules[i].vel.x *= -1;
                }
                else if (particules[i].pos.x > W && particules[i].vel.x > 0) {
                    particules[i].vel.x *= -1;
                }
                if (particules[i].pos.y < 0 && particules[i].vel.y < 0) {
                    particules[i].vel.y *= -1;
                }
                else if (particules[i].pos.y > H && particules[i].vel.y > 0) {
                    particules[i].vel.y *= -1;
                }
                accel = sf::Vector2f(0, 0);
                for (int j = 0; j < nbParticle; j++) {
                    if (i != j && particules[j].visible)
                        accel += forceGravitationel(particules[j], particules[i]);
                }
                //accel += forceGravitationel(pCentre, particules[i]);
                //float col = sqrt(abs( accel.x * accel.x + accel.y * accel.y))*1.5;
                //particules[i].color = sf::Color(255, col, col);
                particules[i].acc = accel;
            }
        }
        for (int i = 0; i < nbParticle; i++) {
            if (particules[i].visible) {
                particules[i].update(deltaT);
                particules[i].draw(window);
            }
        }
        //pCentre.draw(window);

        window.display();
    }

    return 0;
}