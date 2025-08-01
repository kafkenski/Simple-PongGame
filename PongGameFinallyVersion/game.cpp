#include "game.h"

Game::Game()
{
}
Game::~Game()
{
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, Uint32 flags)
{
	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		std::cout << "SDL initialized successfully.\n";

		if (TTF_Init() == 0)
		{
			std::cout << "SDL_TTF initialized successfully.\n";
			font = TTF_OpenFont("arial.ttf", 12);
		}

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window Created\n";
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			leftPaddle = { 20, 250, 20, 100 };
			rightPaddle = { 760, 250, 20, 100 };
			ball = { 395, 295, 12, 12 };

			std::cout << "Renderer Create\n";
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}
}

void Game::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_w: leftPaddleVelocity = -paddleSpeed; break;
			case SDLK_s:leftPaddleVelocity = paddleSpeed; break;
			case SDLK_UP:rightPaddleVelocity = -paddleSpeed; break;
			case SDLK_DOWN:rightPaddleVelocity = paddleSpeed; break;
			} break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_w:leftPaddleVelocity = 0; break;
			case SDLK_s:leftPaddleVelocity = 0; break;
			case SDLK_UP:rightPaddleVelocity = 0; break;
			case SDLK_DOWN:rightPaddleVelocity = 0; break;
			case SDLK_SPACE:
				if (player1Score >= 3 or player2Score >= 3)
				{
					resetGame = true;
				}
				break;
			} break;
		case SDL_QUIT:
			resetGame = true;
			isRunning = false;
			break;
		}
	}
}

void Game::resetBall()
{
	ball.x = 395;
	ball.y = 295;

	int randomNumber = rand() % 4; // 0, 1, 2, 3
	switch (randomNumber)
	{
	case 0:
		ballVelX *= 1;
		ballVelY *= 1;
		break;
	case 1:
		ballVelX *= 1;
		ballVelY *= -1;
		break;
	case 2:
		ballVelX *= -1;
		ballVelY *= 1;
		break;
	case 3:
		ballVelX *= -1;
		ballVelY *= -1;
		break;
	}
}

void Game::moveObjects()
{
	leftPaddle.y += leftPaddleVelocity;
	rightPaddle.y += rightPaddleVelocity;
	ball.x += ballVelX;
	ball.y += ballVelY;
}

void Game::paddleLimit(int& paddleY, int& paddleH)
{
	if (paddleY < 0) { paddleY = 0; }
	if (paddleY > windowHeight - paddleH) { paddleY = windowHeight - paddleH; }
}


void Game::updateScoreTextures()
{
	if (textTexture1) SDL_DestroyTexture(textTexture1);
	if (textTexture2) SDL_DestroyTexture(textTexture2);

	SDL_Color colorBlack = { 0, 0, 0, 255 };
	std::string p1ScoreString = std::to_string(player1Score);
	std::string p2ScoreString = std::to_string(player2Score);

	textSurface1 = TTF_RenderText_Solid(font, p1ScoreString.c_str(), colorBlack);
	textSurface2 = TTF_RenderText_Solid(font, p2ScoreString.c_str(), colorBlack);

	if (textSurface1 && textSurface2)
	{
		score1Rect = { centerX - offset - counterWidth, 0, counterWidth, counterHeight };
		score2Rect = { centerX + offset, 0, counterWidth, counterHeight };

		textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
		textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
	}

	if (textSurface1) SDL_FreeSurface(textSurface1);
	if (textSurface2) SDL_FreeSurface(textSurface2);
}

void Game::checkBallPaddleCollision() {
	if (SDL_HasIntersection(&ball, &rightPaddle) || SDL_HasIntersection(&ball, &leftPaddle))
	{
		if (ballVelX == 5 || ballVelY == 3 || ballVelX == -5 && ballVelY == -3)
		{
			ballVelX *= 2;
			ballVelY *= 2;
		}
		ballVelX *= -1;
	}
}

void Game::checkScore()
{
	if (player1Score >= 3 || player2Score >= 3)
	{
		updateScoreTextures();
		render();
		std::cout << "Enter space to reset the game.\n";
		while (!resetGame)
		{
			handleEvents();
		}
		resetGame = false;
		(ballVelX > 0) ? 5 : -5;
		(ballVelY > 0) ? 3 : -3;
		player1Score = 0;
		player2Score = 0;
	}
}

void Game::checkBallWallsCollision()
{
	if (ball.y <= 0 || ball.y >= windowHeight - ball.h)
	{
		ballVelY *= -1;
	}

	if (ball.x <= 0 || ball.x >= windowWidth - ball.w)
	{
		if (ballVelX == 10 || ballVelY == 6 || ballVelX == -10 || ballVelY == -6)
		{
			ballVelX /= 2;
			ballVelY /= 2;
		}
		if (ball.x <= 0) { player2Score += 1; }
		else { player1Score += 1; }
		resetBall();
	}
}

void Game::update()
{
	moveObjects();
	paddleLimit(leftPaddle.y, leftPaddle.h);
	paddleLimit(rightPaddle.y, rightPaddle.h);
	checkBallWallsCollision();
	checkBallPaddleCollision();
	checkScore();
	updateScoreTextures();
}

void Game::render()
{
	SDL_RenderClear(renderer);
	// Black objects
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &leftPaddle);
	SDL_RenderFillRect(renderer, &rightPaddle);
	SDL_RenderFillRect(renderer, &ball);
	SDL_RenderCopy(renderer, textTexture1, nullptr, &score1Rect);
	SDL_RenderCopy(renderer, textTexture2, nullptr, &score2Rect);
	//Middle line
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	for (int y = 0; y < windowHeight; y += 20) {
		SDL_RenderDrawLine(renderer, centerX, y, centerX, y + 10);
	}

	// back with white screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyTexture(textTexture1);
	SDL_DestroyTexture(textTexture2);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}
