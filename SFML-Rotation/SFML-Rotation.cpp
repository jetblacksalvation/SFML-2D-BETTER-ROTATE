#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>


bool keys[] = { false, false, false, false, false ,false }; //array to hold keyboard input\

enum DIRECTIONS { LEFT, RIGHT, UP, DOWN , ROT_LEFT, ROT_RIGHT }; //left is 0, right is 1, up is 2, down is 3




static const double pi = 3.14159265359;
double angle = 0;
sf::Vector2f player_pos = {400,400};

double distance(sf::Vector2f pos) {
    return sqrtf(powf((pos.x - player_pos.x), 2) + powf((pos.y - player_pos.y), 2));
}


void Rotate(sf::Vector2f real, sf::VertexArray& Projected, size_t index) {
    
    (angle > 2 * pi) ? angle = 0 : (angle < 0) ? angle = (2 * pi) - 0.01f :NULL;// doing angle = (2*pi)- 0.01f lets you turn left when angle = 0
    //rotate 
    Projected[index].position.x = player_pos.x + (std::cosf(angle + atan2f(real.x - player_pos.x, real.y - player_pos.y)) * distance(real));
    Projected[index].position.y = player_pos.y + (std::sinf(angle + atan2f(real.x - player_pos.x, real.y - player_pos.y)) * distance(real));

}



int main()
{
    sf::RenderWindow window({800,800}, "Rotation");
    window.setFramerateLimit(120);
    sf::Event event;

    sf::CircleShape player(10);

    player.setPosition(player_pos);
    sf::VertexArray lines(sf::LinesStrip, 5);
    //i could use an array for this, but im too lazy :P
    sf::Vector2f point1(100,100);
    sf::Vector2f point2(200, 100);
    sf::Vector2f point3(200, 200);
    sf::Vector2f point4(100, 200);
    sf::Vector2f point10(100, 100);
    

    std::cout << "Hello Rotation!\n";
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == event.Closed) {
                window.close();
            }




            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                keys[LEFT] = true;
            }
            else keys[LEFT] = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                keys[RIGHT] = true;
            }
            else keys[RIGHT] = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                keys[UP] = true;
            }
            else keys[UP] = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                keys[DOWN] = true;
            }
            else keys[DOWN] = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                keys[ROT_LEFT] = true;
            }
            else keys[ROT_LEFT] = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                keys[ROT_RIGHT] = true;
            }
            else keys[ROT_RIGHT] = false;
        }
        //input handleing here 
        {
            if (keys[LEFT]) {
                player_pos.x -= 1;
            }
            if (keys[RIGHT]) {
                player_pos.x += 1;
            }
            if (keys[UP]) {
                player_pos.y -= 1;
            }
            if (keys[DOWN]) {
                player_pos.x += 1;
            }
            if (keys[ROT_LEFT]) {
                angle -= 0.01;
            }
            if (keys[ROT_RIGHT]) {
                angle += 0.01;
            }
        }
        Rotate(point1, lines, 0);
        Rotate(point2, lines, 1);
        Rotate(point3, lines, 2);
        Rotate(point4, lines, 3);
        Rotate(point10, lines, 4);

        window.clear(sf::Color::Black);
        window.draw(lines);
        window.display();

    }
}
