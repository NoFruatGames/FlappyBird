#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <algorithm>
using namespace sf;
struct Point
{
    int x;
    int y;
    Point()
    {
        x = 800;
        y = 0;
    }
    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};
struct Tube
{
    Point coord;
    int height;
    Tube()
    {
        coord;
        height = rand() % 40;
    }
};
void drawTube(const Point& drawCoord, int height, RenderWindow& window)
{
    RectangleShape tube1(Vector2f(50, height * 10));
    tube1.setPosition(drawCoord.x, drawCoord.y);
    tube1.setFillColor(Color::Red);

    RectangleShape tube2(Vector2f(50, 450-(height * 10)));
    tube2.setPosition(drawCoord.x, drawCoord.y + (height * 10) + 150);
    tube2.setFillColor(Color::Red);
    window.draw(tube2);
    window.draw(tube1);
}
bool checkColision(const Point& player_coord, Tube& t)
{
    if ((player_coord.y + 25 >= t.coord.y && player_coord.y + 25 <= t.coord.y + (t.height * 10)) && (player_coord.x + 50 >= t.coord.x && player_coord.x + 50<= t.coord.x + 50))
        return true;
    else if ((player_coord.x + 50 >= t.coord.x && player_coord.x + 50 <= t.coord.x + 50) && (player_coord.y + 25 >= t.coord.y + (t.height * 10) + 150 ))
        return true;
    else if (player_coord.y == 545)
        return true;
    else
        return false;
}
int main()
{
    srand(time(NULL));
    rand();
    RenderWindow window(VideoMode(800, 600), "Flappy Bird");
    Texture standart_bird;
    standart_bird.loadFromFile("social_twitter_bird_2554.png");
    Texture fly_bird;
    fly_bird.loadFromFile("social_twitter_bird_2552.png");
    Sprite bird(standart_bird);
    bird.scale(2, 2);
    Clock gravity;
    float fall_speed = 0.005;
    bool jump = false;
    Tube tubes[4];
    Clock tubes_clock;
    float tubes_speed = 0.005;
    Font font;
    font.loadFromFile("calibri.ttf");
    Text text("Time: ", font);
    text.setFillColor(Color::Black);
    text.setPosition(0, 550);
    Clock time;
    int minutes=0;
    int seconds=0;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Space)
                {
                    if (bird.getPosition().y - 50 >= 0)
                    {
                        bird.setPosition(bird.getPosition().x, bird.getPosition().y - 50);
                    }
                    gravity.restart();
                }
            }
        }
        if (gravity.getElapsedTime().asSeconds() >= fall_speed && !jump)
        {
            if (bird.getPosition().y + 56 <= 600)
                bird.setPosition(bird.getPosition().x, bird.getPosition().y + 1);
            gravity.restart();
        }

        window.clear(Color::White);
        if (jump)
            bird.setTexture(fly_bird);
        else
            bird.setTexture(standart_bird);
        
        if (tubes_clock.getElapsedTime().asSeconds() >= tubes_speed)
        {
            for (int i = 0; i < 4; ++i)
            {
                if (i == 0 && tubes[0].coord.x - 1 >= -50)
                {
                    tubes[0].coord.x -= 1;
                }
                else
                {
                    if (tubes[i].coord.x - 1 >= -50 && tubes[i - 1].coord.x - 1 <= 200)
                    {
                        tubes[i].coord.x -= 1;
                    }
                }
            }
            for (int i = 0; i < 4; ++i)
            {
                if (tubes[i].coord.x == -50)
                {
                    Tube newTube;
                    for (int j = 0; j < 3; ++j)
                    {
                        std::swap(tubes[j], tubes[j + 1]);
                    }
                    tubes[3] = newTube;
                }
            }
            tubes_clock.restart();
        }
        for (int i = 0; i < 4; ++i)
        {
            if (checkColision(Point(bird.getPosition().x, bird.getPosition().y), tubes[i]))
            {
                window.close();
            }
        }
        for (int i = 0; i < 4; ++i)
        {
            drawTube(tubes[i].coord, tubes[i].height, window);
        }

        seconds = time.getElapsedTime().asSeconds();
        if (seconds == 60)
        {
            seconds -= 60;
            minutes += 1;
            time.restart();
        }
        text.setString("Time: " + std::to_string(minutes) + ":" + std::to_string(seconds));
        window.draw(text);
        window.draw(bird);
        window.display();
    }
}