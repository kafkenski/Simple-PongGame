#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <cstdlib>

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, Uint32 flags);

	void handleEvents();
	void update();
	void render();
	void clean();

	void moveObjects();
	void paddleLimit(int& paddleY, int& paddleH);
	void checkBallPaddleCollision();
	void checkBallWallsCollision();
	void resetBall();

	void checkScore();
	void updateScoreTextures();

	bool running() { return isRunning; }
private:
	const int windowWidth = 800;
	const int windowHeight = 600;
	const int centerX = windowWidth / 2;
	const int offset = 100;
	const int counterWidth = 30;
	const int counterHeight = 50;

	int paddleSpeed = 10;
	int leftPaddleVelocity = 0;
	int rightPaddleVelocity = 0;
	int player1Score = 0;
	int player2Score = 0;

	int ballVelX = 5;
	int ballVelY = 3;

	bool isRunning;
	bool resetGame = false;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	TTF_Font* font = nullptr;
	SDL_Rect leftPaddle;
	SDL_Rect rightPaddle;
	SDL_Rect ball;
	SDL_Rect score1Rect;
	SDL_Rect score2Rect;
	SDL_Surface* textSurface1 = nullptr;
	SDL_Surface* textSurface2 = nullptr;
	SDL_Texture* textTexture1 = nullptr;
	SDL_Texture* textTexture2 = nullptr;
};