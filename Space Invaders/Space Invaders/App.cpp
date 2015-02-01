#include "App.h"

using namespace std;

GLuint LoadTexture(const char* image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);

	return textureID;
}

bool checkCollision(Entity ball, Entity paddle){
	float ballRight = ball.x + ball.width / 2;
	float ballLeft = ball.x - ball.width / 2;
	float ballTop = ball.y + ball.height / 2;
	float ballBot = ball.y - ball.height / 2;
	float paddleRight = paddle.x + paddle.width / 2;
	float paddleLeft = paddle.x - paddle.width / 2;
	float paddleTop = paddle.y + paddle.height / 2;
	float paddleBot = paddle.y - paddle.height / 2;

	if (!(ballRight < paddleLeft || ballLeft > paddleRight || ballTop < paddleBot || ballBot > paddleTop)){
		return true;
	}
	else
		return false;
}

App::App(){
	Init();
};

App::~App(){

};

void App::Init(){
	SDL_Init(SDL_INIT_VIDEO);//Initializes SDL
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?
	glViewport(0, 0, 800, 600);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions

	elapsed = 0.0f;
	lastFrameTicks = 0.0f;
	
	playerTexture = LoadTexture("SpaceInvader1.png");
	player.textureID = playerTexture;

	player.index = 0;
	player.spriteCountX = 2;
	player.spriteCountY = 1;
	player.width = .05f;
	player.height = .05f;
	player.y = -.85f;
}

bool App::ProcessEvents(){
	while (SDL_PollEvent(&EVENT)) {
		if (EVENT.type == SDL_QUIT || EVENT.type == SDL_WINDOWEVENT_CLOSE) {//If the Window is closed or the user quits the program, end the loop.
			return(true);
		}
	}
	return false;
}

void App::Update(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	keys = SDL_GetKeyboardState(NULL);

	
};

void App::Render(){
	//glClear(GL_COLOR_BUFFER_BIT);//Makes background default color
	player.Render();

	SDL_GL_SwapWindow(displayWindow);
};
