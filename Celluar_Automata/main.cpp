#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

#define WHITE sf::Color::White
#define BLACK sf::Color::Black
#define GRAY sf::Color(153, 153, 153)

int wrapValue(int v, int vMax)
{
    // To generalize this, use modulo
    if (v == -1) return vMax - 1;
    if (v == vMax) return 0;
    return v;
}

int main()
{
    const int CELL_SIZE = 1;
    const int GRID_WIDTH = 1300;
    const int GRID_HEIGHT = 700;
    const int N_CELLS = GRID_WIDTH * GRID_HEIGHT;

    std::vector<int> grid(N_CELLS);
    std::vector<int> gridNext(N_CELLS);
    sf::VertexArray gridDraw(sf::Points, N_CELLS);

    srand(time(NULL));
    for (int i = 0; i < N_CELLS; i++)
        grid[i] = (double(rand()) / RAND_MAX < 0.1) ? 1 : 0;

    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            gridDraw[x + y * GRID_WIDTH].position = sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE);
        }
    }
 
    const int DELAY_INC = 50;
    int delay = 10;
    bool isPlaying = true;

    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * GRID_WIDTH, CELL_SIZE * GRID_HEIGHT + 50), "Game of Life");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
          
            case sf::Event::MouseButtonPressed:
                if (!isPlaying && event.mouseButton.button == sf::Mouse::Left)
                {
                    int x = double(event.mouseButton.x) / CELL_SIZE;
                    int y = double(event.mouseButton.y) / CELL_SIZE;
                    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT)
                        grid[x + y * GRID_WIDTH] = !grid[x + y * GRID_WIDTH];
                }
                break;
            }
        }

        // display grid and prepare gridNext
        window.clear();
        // prepare gridNext
        if (isPlaying)
        {
            for (int x = 0; x < GRID_WIDTH; x++)
            {
                for (int y = 0; y < GRID_HEIGHT; y++)
                {
                    int neighborSum = 0;
                    for (int i = -1; i < 2; i++)
                        for (int j = -1; j < 2; j++)
                        {
                            int xi = wrapValue(x + i, GRID_WIDTH);
                            int yj = wrapValue(y + j, GRID_HEIGHT);
                            neighborSum += grid[xi + yj * GRID_WIDTH];
                        }

                    int current = x + y * GRID_WIDTH;
                    neighborSum -= grid[current];
                    gridNext[current] = grid[current];

                    if (grid[current] == 1 && (neighborSum < 2 || neighborSum > 3))
                        gridNext[current] = 0;
                    else if (neighborSum == 3)
                        gridNext[current] = 1;

                }
            }
        }

        for (int i = 0; i < gridDraw.getVertexCount(); i++) {
            if (grid[i] == 0 and gridDraw[i].color != sf::Color(0, 0, 0))
                gridDraw[i].color = { 0, 0, 0 };
            else if (grid[i] == 1 and gridDraw[i].color != sf::Color(255, 255, 255))
                gridDraw[i].color = { 255, 255, 255 };
        }

        window.draw(gridDraw);

        // move gridNext to grid
        if (isPlaying)
            for (int i = 0; i < N_CELLS; i++)
                grid[i] = gridNext[i];


        window.display();
    }
}