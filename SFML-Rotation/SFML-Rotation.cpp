
Eduardo Dominguez Ramirez <coolanimeguy666@gmail.com>
12:06 PM (0 minutes ago)
to me

#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

bool keys[] = { false, false, false, false, false ,false }; //array to hold keyboard input\
//-g++ SFML-Rotation.cpp -o a.out -lsfml-graphics -lsfml-window -lsfml-system && ./a.out
enum DIRECTIONS { LEFT, RIGHT, UP, DOWN , ROT_LEFT, ROT_RIGHT }; //left is 0, right is 1, up is 2, down is 3


int vx = 0, vy = 0;


static const double pi = 3.14159265359;
float angle = 0;
sf::Vector2f player_pos = {400,400};

double distance(sf::Vector2f pos) {
    return sqrtf(powf(( player_pos.x - pos.x), 2) + powf(( player_pos.y- pos.y ), 2));
}


void Rotate(sf::Vector2f& real, sf::VertexArray& Projected, size_t index) {
    auto dist = distance(real);
    (angle > 2 * pi) ? angle = 0 : (angle < 0) ? angle = (2 * pi) -0.01f:(float)NULL;// doing angle = (2*pi) lets you turn left when angle = 0
    //rotate 
    Projected[index].position.x = player_pos.x + (cosf(angle + atan2f(real.x - player_pos.x, real.y - player_pos.y)) * dist);
    Projected[index].position.y = player_pos.y + (sinf(angle + atan2f(real.x - player_pos.x, real.y - player_pos.y)) * dist);
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
                it.x += (cosf(angle + temp)) * 2;
                it.y += (sinf(angle + temp)) * 2;
            }
            
        }

       /* player_pos.x = player_pos.x + (std::cosf(angle+temp)) * 5;
        player_pos.y = player_pos.y + (std::sinf(angle+temp)) * 5;*/
    }


}

sf::CircleShape intersect(10);
int main()
{
    intersect.setFillColor(sf::Color::Black);
    sf::CircleShape bullet(15);

    sf::RenderWindow window({800,800}, "Rotation");
    window.setFramerateLimit(120);
    sf::Event event;
    sf::VertexArray lines(sf::LinesStrip, 5);
    //square 


    std::vector<sf::Vector2f> points({ {100,100}, {200, 100}, {200,200 }, {100,200} , {100,100} });//last index is so that lines can wrap back to start 
    std::vector<std::vector<sf::Vector2f>> Level_Data({ points });

    std::vector<sf::Vector2f> pixels({});
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
        for (auto& shapes : Level_Data) {//applies rotation formula to every object 
            int x = 0;
            for (auto& shape_data : shapes) {
                Rotate(shape_data, lines, x);
                x++;
            }
        }

            //pic1.setRotation(angle*180/pi);//goofy azz function uses angles and not radians :P 
        int camera_plane = player_pos.y / 2;//calculate the camera plane
        pic1.setRotation((angle - pi / 2)/(180/pi));
        window.clear(sf::Color::White);

        for (float it = 0; it < pi * 2; it+=0.1) {
            sf::VertexArray temp(sf::Lines, 2);
            temp[0].position.x = player_pos.x + cos(-it) * 300;
            temp[0].position.y = player_pos.y + sin(-it) * 300;
            temp[1].position = player_pos;
            
            float under_ray = (player_pos.y-(sinf(-it)+player_pos.y));//demoninator
            float top_ray = (player_pos.x - (cosf(-it)+player_pos.x));//slope of ray - seperating the two in case of under being undefined 

            float y_inter = player_pos.y;
            for(auto shape: Level_Data){//iterates through shapes
                //struct stuff std::vector<sf::Vector2f>::iterator iter = shape.begin(), Vector2f first= {0,0}, second = {0,0}; 
                //goofy goblin code :P 
                for(struct{std::vector<sf::Vector2f>::iterator iter ; sf::Vector2f first, second;} values = {shape.begin(),*shape.begin(), *(shape.begin()+1)}; 
                values.iter !=  (shape.end()-1); values.iter ++, values.first = *(values.iter), values.second = *(values.iter+1)
                )
                {
                    float top_ray_temp = values.first.y - values.second.y;
                    std::cout<<values.second.y<<std::endl;

                    float under_ray_temp = values.first.x - values.second.x;
                    std::cout<<values.second.x<<std::endl;
                    
                    // if(under_ray == 0 || under_ray_temp ==0){
                    //     puts("fix this...\n");
                    //     exit(-1);
                    // }
                    float y_inter_temp = values.first.y -((top_ray_temp/under_ray_temp)*values.first.x);//y intercept using slope formula
                    //draw a circle at inter section

                    float inter_x  = (((-(top_ray/under_ray))*player_pos.x)+player_pos.y - y_inter_temp)/((top_ray_temp/under_ray_temp)-(top_ray/under_ray));
                    if(inter_x < values.second.x && inter_x > values.first.x){
                    intersect.setPosition(inter_x, ((top_ray_temp/under_ray_temp)*inter_x)+y_inter_temp);
                    window.draw(intersect);
                    }
                }

            }
            temp[0].color = sf::Color::Black;
            temp[1].color = sf::Color::Black;

            window.draw(temp);
        }


        window.draw(pic1);
        window.draw(lines);
        window.display();


    }
}
