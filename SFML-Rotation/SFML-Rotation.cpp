#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

bool keys[] = { false, false, false, false, false ,false }; //array to hold keyboard input\

enum DIRECTIONS { LEFT, RIGHT, UP, DOWN , ROT_LEFT, ROT_RIGHT }; //left is 0, right is 1, up is 2, down is 3


int vx = 0, vy = 0;


static const double pi = 3.14159265359;
float angle = 0;
sf::Vector2f player_pos = {400,400};

double distance(sf::Vector2f pos) {
    return sqrtf(powf(( player_pos.x - pos.x), 2) + powf(( player_pos.y- pos.y ), 2));
}


void Rotate(sf::Vector2f real, sf::VertexArray& Projected, size_t index) {
    auto dist = distance(real);
    (angle > 2 * pi) ? angle = 0 : (angle < 0) ? angle = (2 * pi) -0.01f:NULL;// doing angle = (2*pi) lets you turn left when angle = 0
    //rotate 
    Projected[index].position.x = player_pos.x + (std::cosf(angle + atan2f(real.x - player_pos.x, real.y - player_pos.y)) * dist);
    Projected[index].position.y = player_pos.y + (std::sinf(angle + atan2f(real.x - player_pos.x, real.y - player_pos.y)) * dist);
    Projected[index].color = sf::Color::Black;
}

void HandleKeys(bool keys[6], std::vector<std::vector<sf::Vector2f>>*  world_Data) {
    float temp = 0;//change to the orientation of angle 
    if (keys[LEFT] || keys[RIGHT] || keys[UP] || keys[DOWN]) {
        if (keys[LEFT]) {
            temp = pi/2 ;
        }
        if (keys[RIGHT]) {
            temp = (3*pi)/2;
        }
        if (keys[UP]) {
            temp = 0;
        }
        if (keys[DOWN]) {
            temp = pi;
        
        }
        for (auto& iter : *world_Data) {
            for (auto& it : iter) {
                it.x += (std::cosf(angle + temp)) * 2;
                it.y += (std::sinf(angle + temp)) * 2;
            }
            
        }

       /* player_pos.x = player_pos.x + (std::cosf(angle+temp)) * 5;
        player_pos.y = player_pos.y + (std::sinf(angle+temp)) * 5;*/
    }


}


int main()
{
    sf::CircleShape bullet(15);

    sf::RenderWindow window({800,800}, "Rotation");
    window.setFramerateLimit(120);
    sf::Event event;
    sf::VertexArray lines(sf::LinesStrip, 5);
    //square 


    std::vector<sf::Vector2f> points({ {100,100}, {200, 100}, {200,200 }, {100,200} , {100,100} });//last index is so that lines can wrap back to start 
    std::vector<std::vector<sf::Vector2f>> Level_Data({ points });

    std::vector<sf::Vector2f> pixels({});
    std::vector<float> pixel_angles({});
    sf::Texture img1;
    if (!img1.loadFromFile("Arrow.png")) return 0; //this line loads the image AND kills your program if it doesn't load
    sf::Sprite pic1;
    pic1.setTexture(img1);
    pic1.setPosition(player_pos.x - 16, player_pos.y - 16);//offset by half of pixel dimensions   

    int camera_plane = player_pos.y / 2;
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                pixel_angles.emplace_back( -pi/2);
            }
        }

        {        //input handleing here 

            HandleKeys(keys, &Level_Data);
            if (keys[ROT_LEFT]) {
                angle -= 0.05;
            }
            if (keys[ROT_RIGHT]) {
                angle += 0.05;
            }
        }
        for (auto shapes : Level_Data) {//applies rotation formula to every object 
            int x = 0;
            for (auto shape_data : shapes) {
                Rotate(shape_data, lines, x);
                x++;
            }
        }

            //pic1.setRotation(angle*180/pi);//goofy azz function uses angles and not radians :P 
        int camera_plane = player_pos.y / 2;//calculate the camera plane
        pic1.setRotation((angle - pi / 2)/(180/pi));
        
        

        sf::VertexArray temp(sf::Lines, 2);

        int x = 0;
        for (auto it : pixel_angles) {
            temp[0].position.x = player_pos.x;
            temp[0].position.y = player_pos.y;

            temp[1].position.x = player_pos.x+cosf(it) * 300;
            temp[1].position.y = player_pos.y+sin(it) * 300;

            temp[0].color = sf::Color::Black;
            temp[1].color = sf::Color::Black;

        }
        //pixel_angles.clear();
        window.clear(sf::Color::White);

        window.draw(temp);

        
        window.draw(pic1);
        window.draw(lines);
        window.display();


    }
}
