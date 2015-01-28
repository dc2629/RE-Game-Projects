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
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?
	glViewport(0, 0, 800, 600);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions

	elapsed = 0.0f;
	lastFrameTicks = 0.0f;

	background.textureID = LoadTexture("background.png");
	background.height = 2;
	background.width = 2.66;
	//Initialize Ball
	ball.textureID = LoadTexture("Ball.png");
	ball.height = .2;
	ball.width = .2;
	ball.velocity_x = 1.4* cos(60);
	ball.velocity_y = 1.4* sin(60);
	ball.CollidedVertically = false;
	Entities.push_back(&ball);
	//Initialize Paddle 1
	paddle1.textureID = LoadTexture("Paddle.png");
	paddle1.x = -1.25;
	paddle1.velocity_y = 1.2f;
	paddle1.height = .3;
	paddle1.scale_x = .2;
	Entities.push_back(&paddle1);
	//Initialize Paddle 2
	paddle2.textureID = LoadTexture("Paddle.png");
	paddle2.x = 1.25;
	paddle2.velocity_y = 1.2f;
	paddle2.height = .3;
	paddle2.scale_x = .2;
	Entities.push_back(&paddle2);


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

	//Entity X-Movement

	//Ball motion and update
	ball.x += ball.velocity_x*elapsed;
	//Collision Check + Collision Response

	if (checkCollision(ball, paddle1)){
		if (!ball.CollidedVertically){
			ball.x += .01;
			ball.velocity_x = -ball.velocity_x;
		}
	}
	else if (checkCollision(ball, paddle2)){
		if (!ball.CollidedVertically){
			ball.x -= .01;
			ball.velocity_x = -ball.velocity_x;
		}
	}

	//Entity Y-Movement

	ball.y += ball.velocity_y*elapsed;
	//Paddle 1 motion
	if (keys[SDL_SCANCODE_W]){
		paddle1.y += paddle1.velocity_y*elapsed;
	}
	if (keys[SDL_SCANCODE_S]){
		paddle1.y -= paddle1.velocity_y*elapsed;
	}
	//Paddle 2 motion
	if (keys[SDL_SCANCODE_UP]){
		paddle2.y += paddle2.velocity_y*elapsed;
	}
	if (keys[SDL_SCANCODE_DOWN]){
		paddle2.y -= paddle2.velocity_y*elapsed;
	}




	if (checkCollision(ball, paddle1)){
		if (!ball.CollidedVertically){
			ball.velocity_y = -ball.velocity_y;
			ball.CollidedVertically = true;
		}
	}
	else if (checkCollision(ball, paddle2)){
		if (!ball.CollidedVertically){
			ball.velocity_y = -ball.velocity_y;
			ball.CollidedVertically = true;
		}
	}




	if (ball.y > .91){
		ball.y = .905;
		ball.velocity_y = -ball.velocity_y;
	}
	else if (ball.y < -.91){
		ball.y = -.905;
		ball.velocity_y = -ball.velocity_y;
	}

	if (paddle1.y > .91){
		paddle1.y = .905;
	}
	else if (paddle1.y < -.91){
		paddle1.y = -.905;
	}

	if (paddle2.y > .91){
		paddle2.y = .905;
	}
	else if (paddle2.y < -.91){
		paddle2.y = -.905;
	}

	if (ball.x > 1.4 || ball.x < -1.4){
		ball.x = 0;
		ball.y = 0;
		float randomAngle = 360 * RANDOM_NUMBER - 180;
		ball.velocity_x = 1.4*cos(randomAngle);
		ball.velocity_y = 1.4*sin(randomAngle);
		ball.CollidedVertically = false;
	}
	//Ball cool animation
	if (ball.velocity_x < 0){
		ball.scale_x = -1;
	}
	else{
		ball.scale_x = 1;
	}

};

void App::Render(){
	//glClear(GL_COLOR_BUFFER_BIT);//Makes background default color

	background.Draw();

	for (int i = 0; i < Entities.size(); i++){
		Entities[i]->Draw();
	}

	SDL_GL_SwapWindow(displayWindow);
};
