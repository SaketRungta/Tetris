#include <SFML/Graphics.hpp>
#include <time.h>

const int m = 20;
const int n = 10;

int field[m][n] = { 0 };

struct point
{
    int x;
    int y;
} a[4], b[4];

int figures[7][4] =
{
    1,3,5,7, // i
    2,4,5,7, // z
    3,5,4,6, // s
    3,5,4,7, // t
    2,3,5,7, // l
    3,5,7,6, // j
    2,3,4,5  // 0
};

bool check()
{
    for (int i = 0; i < 4; i++)
    {
        if (a[i].x < 0 || a[i].x >= n || a[i].y >= m)return 0;
        else if (field[a[i].y][a[i].x])return 0;
    }

    return 1;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(320, 480), "Tetris");

    sf::Texture tile;
    tile.loadFromFile("../Assets/tiles.png");

    sf::Sprite s(tile);
    s.setTextureRect(sf::IntRect(0, 0, 18, 18));

    sf::Texture backGround;
    backGround.loadFromFile("../Assets/background.png");

    sf::Sprite bg(backGround);
    bg.setTextureRect(sf::IntRect(0, 0, 320, 480));

    int dx = 0; 
    bool rotate = 0; 
    int colorNum = 1;

    float timer = 0.0f;
    float delay = 0.3f;
    
    sf::Clock clock;

    while (window.isOpen())
    {
        for (int i = 0; i < n; i++)
        {
            if (field[2][i] == 1)
                window.close();
        }

        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::KeyPressed)
            {
                if (e.key.code == sf::Keyboard::W) rotate = true;
                else if (e.key.code == sf::Keyboard::A) dx = -1;
                else if (e.key.code == sf::Keyboard::D) dx = 1;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) delay = 0.05f;

        /* Move */
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].x += dx;
        }

        if (!check()) for (int i = 0; i < 4; i++)a[i] = b[i];

        /* Rotate */
        if (rotate)
        {
            point p = a[1];
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check()) for (int i = 0; i < 4; i++)a[i] = b[i];
        }

        /* Tick */
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++)
            {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!check())
            {
                for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;

                colorNum = 1 + rand() % 7;
                int n = rand() % 7;
                for (int i = 0; i < 4; i++)
                {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            }

            timer = 0.0f;
        } 

        /* Check Lines */

        int k = m - 1;
        for (int i = m - 1; i > 0; i--)
        {
            int count = 0;
            for (int j = 0; j < n; j++)
            {
                if (field[i][j])
                    count++;
                field[k][j] = field[i][j];
            }
            if (count < n)
                k--;
        }

        dx = 0; rotate = 0; delay = 0.3f;

        /* Draw */
        window.clear(sf::Color::White);
        window.draw(bg);

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (field[i][j] == 0)continue;
                s.setTextureRect(sf::IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition((float)j * 18, (float)i * 18);
                s.move(28, 31);
                window.draw(s);
            }
        }

        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(sf::IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition((float)a[i].x * 18, (float)a[i].y * 18);
            s.move(28, 31);
            window.draw(s);
        }
        window.display();
    }

    return 0;
}