#include <iostream>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };

struct Point
{
	int x, y;
}  a[4], b[4];

int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) 
			return 0;
		else if (field[a[i].y][a[i].x]) 
			return 0;
	return 1;
}

enum class GameScreen
{
	Welcome,
	Game,
	GameOver
};

int main() 
{	
	bool keyLeftPressed = false;
	bool keyRightPressed = false;
	bool keyUpPressed = false;

	srand(time(0));
	RenderWindow window(VideoMode(320, 480), "Daniyar's Tetris Game!");

	window.setFramerateLimit(60);
	
	Texture t1, t2, t3;
	t1.loadFromFile("images/tiles.png");
	t2.loadFromFile("images/background.png");
	t3.loadFromFile("images/frame.png");

	Sprite s(t1), background(t2), frame(t3);
	s.setTextureRect(IntRect(0,0,18,18));

	int dx = 0;
	bool rotate = 0;
	int colorNum = 1;
	float timer = 0, delay = 0.3;

	Clock clock;

	Font font;
	if (!font.loadFromFile("fonts/arial.ttf"))
	{
		std::cout << "Error Loading Font." << std::endl;
	}

	Text welcomeText("Welcome to Tetris!", font, 24);
	welcomeText.setPosition(60, 200);
	welcomeText.setFillColor(Color::White);
	welcomeText.setOutlineColor(Color::Black);
	welcomeText.setOutlineThickness(2.0f);

	Text startText("Press ENTER to start!", font, 24);
	startText.setPosition(40, 240);
	startText.setFillColor(Color::White);
	startText.setOutlineColor(Color::Black);
	startText.setOutlineThickness(2.0f);

	Text gameOverText("GAME OVER", font, 24);
	gameOverText.setPosition(80, 200);
	gameOverText.setFillColor(Color::White); // Set text color to white
	gameOverText.setOutlineColor(Color::Black); // Add these lines to set the outline color
	gameOverText.setOutlineThickness(2.0f);

	Text restartText("Press ENTER to restart!", font, 24);
	restartText.setPosition(30, 240);
	restartText.setFillColor(Color::White); // Set text color to white
	restartText.setOutlineColor(Color::Black); // Add these lines to set the outline color
	restartText.setOutlineThickness(2.0f); // Add these lines to set the outline thickness

	Text timeText("", font, 24);
	timeText.setPosition(240, 105);
	timeText.setFillColor(Color::White); // Set text color to white
	timeText.setOutlineColor(Color::Black); // Add these lines to set the outline color
	timeText.setOutlineThickness(2.0f); // Add these lines to set the outline thickness

	Text scoreText("", font, 24);
	scoreText.setPosition(240, 5);
	scoreText.setFillColor(Color::White); // Set text color to white
	scoreText.setOutlineColor(Color::Black); // Add these lines to set the outline color
	scoreText.setOutlineThickness(2.0f); // Add these lines to set the outline thickness

	bool gameStarted = false;
	bool gameEnded = false;

	int gameScore = 0;
	Clock gameClock;
	float playTime = 0;

	GameScreen currentScreen = GameScreen::Welcome;

	bool init = true;


	while (window.isOpen()) 
	{	
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		//Input handling

		if (Keyboard::isKeyPressed(Keyboard::Up) && !keyUpPressed)
		{
			rotate = true;
			keyUpPressed = true;
		}
		else if (!Keyboard::isKeyPressed(Keyboard::Up)) 
		{
			keyUpPressed = false;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Left) && !keyLeftPressed) {
			dx = -1;
			keyLeftPressed = true;
		}
		else if (!Keyboard::isKeyPressed(Keyboard::Left)) {
			keyLeftPressed = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Right) && !keyRightPressed) {
			dx = 1;
			keyRightPressed = true;
		}
		else if (!Keyboard::isKeyPressed(Keyboard::Right)) {
			keyRightPressed = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			delay = 0.05;
		}
		

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
			if (currentScreen == GameScreen::Welcome)
			{
				if (Keyboard::isKeyPressed(Keyboard::Enter))
				{
					currentScreen = GameScreen::Game;
					gameClock.restart();
					init = true;
				}
			}
		}

		if (currentScreen == GameScreen::Game)

		{
			//GAme Logic and drawing //
			//Check if game is over //

			if (init)
			{
				//create the first block

				int n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2 + N / 2 - 1; // Start in the middle
					a[i].y = figures[n][i] / 2 - 1;
				}
				init = false;
			}

			

			//// <- Move -> ////
			for (int i = 0; i < 4; i++) {
				b[i] = a[i]; 
				a[i].x += dx;
			}
			if (!check()) 
				for (int i = 0; i < 4; i++) 
					a[i] = b[i];

			////Rotate////
			if (rotate)
			{
				Point p = a[1]; // center of rotation
				for (int i = 0; i < 4; i++)
				{
					int x = a[i].y - p.y;
					int y = a[i].x - p.x;
					a[i].x = p.x - static_cast<int>(x);
					a[i].y = p.y + static_cast<int>(y);

				}
				}
				if (!check()) 
					for (int i = 0; i < 4; i++) 
						a[i] = b[i];
			
			
			// Tick //

			if (timer > delay)
			{
				for (int i = 0; i < 4; i++) 
				{
					b[i] = a[i]; 
					a[i].y += 1;
				}

				if (!check())
				{
					for (int i = 0; i < 4; i++) 
						field[b[i].y][b[i].x] = colorNum;

					// check game over //

					bool gameOver = false;
					for (int j = 0; j < N; j++)
					{
						if (field[0][j])
						{
							gameOver = true;
							break;
						}
					}


					if (gameOver)
					{
						currentScreen = GameScreen::GameOver;
					}

					else 
					{
						colorNum = 1 + rand() % 7;
						int n = rand() % 7;
						for (int i = 0; i < 4; i++)
						{
							a[i].x = figures[n][i] % 2 + N/2 - 1;
							a[i].y = figures[n][i] / 2 - 1;
						}
					}
				}
				timer = 0;

				playTime = gameClock.getElapsedTime().asSeconds();
				std::stringstream playTimeStream;
				playTimeStream << "Time: \n" << static_cast<int>(playTime);
				timeText.setString(playTimeStream.str());

				std::stringstream scoreStream;
				scoreStream << "Score: \n" << gameScore;
				scoreText.setString(scoreStream.str());
			}

			//check lines//
			int k = M - 1;
			for (int i = M - 1; i > 0; i--) 
			{
				int count = 0;
				for (int j = 0; j < N; j++) 
				{
					if (field[i][j]) 
						count++;
						field[k][j] = field[i][j];
				}
				if (count < N)
				{
					k--;
				}
				else 
				{
					gameScore += 100;
				}
			}

			
		}

		if (currentScreen == GameScreen::GameOver)
				{
					if (Keyboard::isKeyPressed(Keyboard::Enter))
					{
						// reset game state and switch back to the welcome screen
						currentScreen = GameScreen::Welcome;
						gameScore = 0;
						for (int i = 0; i < M; i++)
							for (int j = 0; j < N; j++)
								field[i][j] = 0;
					}
				}
			
		dx = 0; 
		rotate = 0; 
		delay = 0.3;

		//draw//

		window.clear(Color::White);
		window.draw(background);

		for (int i=0; i< M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				s.setPosition(j * 18, i * 18);
				s.move(28, 31); //offset
				window.draw(s);
			}

		if (currentScreen == GameScreen::Game)
		{ 
		for (int i = 0; i < 4; i++)
			{	
				s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
				s.setPosition(a[i].x * 18, a[i].y * 18);
				s.move(28, 31); //offset
				window.draw(s);
			}
		}

		if (currentScreen == GameScreen::Game)
		{
			window.draw(timeText);
			window.draw(scoreText);
		}
		else if (currentScreen == GameScreen::Welcome)
		{
			window.draw(welcomeText);
			window.draw(startText);
		}
		else
		{
			window.draw(gameOverText);
			window.draw(restartText);
		}
		
		window.display();
	}

	

	return 0;
}
