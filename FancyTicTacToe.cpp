#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

extern "C" {
    extern unsigned char FancyTicTacToe_Background[];
    extern unsigned int FancyTicTacToe_Background_len;
}

using namespace sf;

const int CELL_SIZE = 105;
const int GRID_SIZE = 3;

int main()
{
    RenderWindow app(VideoMode({600, 600}), "Fancy Tic Tac Toe");
    app.setFramerateLimit(60);

    // Load background (board + pieces) from embedded memory
    Texture bgTexture;
    if (!bgTexture.loadFromMemory(
        FancyTicTacToe_Background,
        FancyTicTacToe_Background_len)) {
        std::cerr << "Failed to load background image from memory\n";
        return 1;
    }

    // Board background
    Sprite board(bgTexture);

    // X and O piece sprites (cropped from bottom left)
    Sprite xSprite(bgTexture, IntRect({0, 522}, {75, 75}));
    Sprite oSprite(bgTexture, IntRect({80, 522}, {75, 75}));

    // Load font for message
    Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arialbd.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    // Game state: 0 = empty, 1 = X, 2 = O
    int grid[GRID_SIZE][GRID_SIZE] = {0};
    int turn = 1; // 1 = X, 2 = O
    int winner = 0;
    bool draw = false;
    bool showMessage = false;
    std::string message;

    while (app.isOpen())
    {
        while (auto optEvent = app.pollEvent())
        {
            const sf::Event& e = *optEvent;

            // Window closed
            if (e.is<sf::Event::Closed>())
                app.close();

            // Get current window size and scaling factors
            Vector2u winSize = app.getSize();
            float scaleX = winSize.x / 600.0f;
            float scaleY = winSize.y / 600.0f;

            // Mouse button pressed
            if (!winner && !draw && e.is<sf::Event::MouseButtonPressed>()) {
                if (const auto* mouse = e.getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        Vector2i pos = Mouse::getPosition(app);

                        // Map window mouse position to 600x600 virtual board
                        float virtualX = pos.x / scaleX;
                        float virtualY = pos.y / scaleY;

                        int gridOffsetX = (600 - (CELL_SIZE * GRID_SIZE)) / 2;
                        int gridOffsetY = (600 - (CELL_SIZE * GRID_SIZE)) / 2;
                        int col = (virtualX - gridOffsetX) / CELL_SIZE;
                        int row = (virtualY - gridOffsetY) / CELL_SIZE;

                        if (col >= 0 && col < GRID_SIZE && row >= 0 && row < GRID_SIZE)
                        {
                            if (grid[row][col] == 0)
                            {
                                grid[row][col] = turn;

                                // Check rows, columns, and diagonals for a win
                                for (int i = 0; i < GRID_SIZE; ++i) {
                                    // Row
                                    if (grid[i][0] == turn && grid[i][1] == turn && grid[i][2] == turn)
                                        winner = turn;
                                    // Column
                                    if (grid[0][i] == turn && grid[1][i] == turn && grid[2][i] == turn)
                                        winner = turn;
                                }
                                // Diagonals
                                if (grid[0][0] == turn && grid[1][1] == turn && grid[2][2] == turn)
                                    winner = turn;
                                if (grid[0][2] == turn && grid[1][1] == turn && grid[2][0] == turn)
                                    winner = turn;

                                // Check for draw (no empty cells and no winner)
                                draw = true;
                                for (int r = 0; r < GRID_SIZE; ++r)
                                    for (int c = 0; c < GRID_SIZE; ++c)
                                        if (grid[r][c] == 0)
                                            draw = false;

                                if (winner) {
                                    message = (winner == 1 ? "X wins!" : "O wins!");
                                    showMessage = true;
                                } else if (draw) {
                                    message = "It's a draw!";
                                    showMessage = true;
                                }

                                turn = 3 - turn; // Switch turns
                            }
                        }
                    }
                }
            }

            // Key pressed (restart)
            if (showMessage && e.is<sf::Event::KeyPressed>()) {
                // Reset game
                for (int r = 0; r < GRID_SIZE; ++r)
                    for (int c = 0; c < GRID_SIZE; ++c)
                        grid[r][c] = 0;
                winner = 0;
                draw = false;
                showMessage = false;
                turn = 1;
            }
        }

        app.clear(Color::White);
        app.draw(board);

        // Draw pieces (unchanged)
        int gridOffsetX = (600 - (CELL_SIZE * GRID_SIZE)) / 2;
        int gridOffsetY = (600 - (CELL_SIZE * GRID_SIZE)) / 2;
        for (int row = 0; row < GRID_SIZE; ++row)
        {
            for (int col = 0; col < GRID_SIZE; ++col)
            {
                if (grid[row][col] == 1 || grid[row][col] == 2)
                {
                    Sprite piece = (grid[row][col] == 1) ? xSprite : oSprite;
                    // No scaling applied
                    // Center in cell, offset by grid position
                    float px = gridOffsetX + col * CELL_SIZE + (CELL_SIZE - 75) / 2.0f;
                    float py = gridOffsetY + row * CELL_SIZE + (CELL_SIZE - 75) / 2.0f;
                    piece.setPosition({px, py});
                    app.draw(piece);
                }
            }
        }

        // Draw message window if game over
        if (showMessage)
        {
            // Place message box at the bottom of the window
            float boxWidth = 400;
            float boxHeight = 70;
            float boxX = (600 - boxWidth) / 2.0f;
            float boxY = 600 - boxHeight - 30; // 30px margin from bottom

            RectangleShape msgBox(Vector2f(boxWidth, boxHeight));
            msgBox.setFillColor(Color(255, 255, 255, 230));
            msgBox.setOutlineColor(Color::Black);
            msgBox.setOutlineThickness(3);
            msgBox.setPosition({boxX, boxY});

            Text msgText(font, message + "\nPress any key to restart");
            msgText.setCharacterSize(20); // Smaller font
            msgText.setFillColor(Color::Black);
            msgText.setStyle(Text::Bold);
            msgText.setPosition({boxX + 20, boxY + 10});

            app.draw(msgBox);
            app.draw(msgText);
        }

        app.display();
    }

    return 0;
}
