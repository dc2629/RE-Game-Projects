#include "App.h"

using namespace std;

void drawText(GLint fontTexture, string text, float size, float spacing, float r, float g, float b, float a, float x, float y) {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size, ((size + spacing) * i) +
			(-0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), 0.5f
			* size });
		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);//gotta disable
	glPopMatrix();
}

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

	SDL_Init(SDL_INIT_VIDEO);//Initializes SDL
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//Creates the window with OpenGL and the dimensions of the window.
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);//Make the window appear?
	glViewport(0, 0, 800, 600);//The start of using OpenGL with the arguments as the resolution.
	glMatrixMode(GL_PROJECTION);//Usually ran once and thats it.
	glOrtho(-1.33, 1.33, -1, 1, -1, 1);//The ratio of resolutions

	Init();

};

App::~App(){

};

void App::Init(){
	
	//Timer Initalizations
	elapsed = 0.0f;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	actualElapsed = 0.0f;
	delay = 0.0f;

	//Save Images
	playerTexture = LoadTexture("SpaceInvader1.png");
	Alien1texture = LoadTexture("SpaceInvaderAlien1.png");
	Alien2texture = LoadTexture("SpaceInvaderAlien2.png");
	Alien3texture = LoadTexture("SpaceInvaderAlien3.png");
	font = LoadTexture("font.png");

	//Initializing Player and Back Alien Entities
	player.textureID = playerTexture;
	player.index = 0;
	player.spriteCountX = 2;
	player.spriteCountY = 1;
	player.width = .08f;
	player.height = .05f;
	player.y = -.85f;
	player.velocity_x = -0.02f;

	backAlien.textureID = playerTexture;
	backAlien.index = 1;
	backAlien.spriteCountX = 2;
	backAlien.spriteCountY = 1;
	backAlien.width = .08f;
	backAlien.height = .05f;
	backAlien.y = .90f;
	Entities.push_back(&backAlien);

	//Initialize all other Alien Entities
	for (int i = 0; i < 11; i++){
		Alien1[i].textureID = Alien1texture;
		Alien1[i].index = 0;
		Alien1[i].spriteCountX = 2;
		Alien1[i].spriteCountY = 1;
		Alien1[i].width = .05f;
		Alien1[i].height = .05f;
		Alien1[i].x = -.95f + .175*i;
		Alien1[i].y = .8f;
		Entities.push_back(&Alien1[i]);
	}

	for (int i = 0; i < 11; i++){
		Alien2r1[i].textureID = Alien2texture;
		Alien2r1[i].index = 0;
		Alien2r1[i].spriteCountX = 2;
		Alien2r1[i].spriteCountY = 1;
		Alien2r1[i].width = .05f;
		Alien2r1[i].height = .05f;
		Alien2r1[i].x = -.95f + .175*i;
		Alien2r1[i].y = .68f;
		Entities.push_back(&Alien2r1[i]);
	}

	for (int i = 0; i < 11; i++){
		Alien2r2[i].textureID = Alien2texture;
		Alien2r2[i].index = 0;
		Alien2r2[i].spriteCountX = 2;
		Alien2r2[i].spriteCountY = 1;
		Alien2r2[i].width = .05f;
		Alien2r2[i].height = .05f;
		Alien2r2[i].x = -.95f + .175*i;
		Alien2r2[i].y = .56f;
		Entities.push_back(&Alien2r2[i]);
	}

	for (int i = 0; i < 11; i++){
		Alien3r1[i].textureID = Alien3texture;
		Alien3r1[i].index = 0;
		Alien3r1[i].spriteCountX = 2;
		Alien3r1[i].spriteCountY = 1;
		Alien3r1[i].width = .05f;
		Alien3r1[i].height = .05f;
		Alien3r1[i].x = -.95f + .175*i;
		Alien3r1[i].y = .44f;
		Entities.push_back(&Alien3r1[i]);
	}

	for (int i = 0; i < 11; i++){
		Alien3r2[i].textureID = Alien3texture;
		Alien3r2[i].index = 0;
		Alien3r2[i].spriteCountX = 2;
		Alien3r2[i].spriteCountY = 1;
		Alien3r2[i].width = .05f;
		Alien3r2[i].height = .05f;
		Alien3r2[i].x = -.95f + .175*i;
		Alien3r2[i].y = .32f;
		Entities.push_back(&Alien3r2[i]);
	}

	//Background
	background.textureID = LoadTexture("spbackground.png");
	background.index = 0;
	background.spriteCountX = 1;
	background.spriteCountY = 1;
	background.width = 1.33f;
	background.height = 1.0f;

	//Alien Movement
	AlienMovement = false;
	AlienMvtSpeedModifier = 0.06f;

	//Player Lives
	for (int i = 0; i < 3; i++){
		playerlives[i].textureID = playerTexture;
		playerlives[i].index = 0;
		playerlives[i].spriteCountX = 2;
		playerlives[i].spriteCountY = 1;
		playerlives[i].width = .05333f;
		playerlives[i].height = .03333f;
		playerlives[i].y = -.95f;
		playerlives[i].x = 0.9f + 0.13f*i;
	}
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
	//Elapsed Timer Frame and Animation timers
	actualElapsed = elapsed - delay;
	//Alien Animation Timer
	AlienAnimationTimer += actualElapsed;

	//Alien Animation
	if (AlienAnimationTimer > (.45f)){
		for (int i = 1; i < Entities.size(); i++){
			Entities[i]->index++;
			AlienAnimationTimer = 0.0f;
			if (Entities[i]->index > 1){
				Entities[i]->index = 0;
			}
		}
	}

	//Update delay
	delay = elapsed;

	//Alien Movement
	if (!AlienMovement){
		for (int i = 1; i < Entities.size(); i++){
			Entities[i]->x += AlienMvtSpeedModifier*actualElapsed;
		}
	}
	else{
		for (int i = 1; i < Entities.size(); i++){
			Entities[i]->x -= AlienMvtSpeedModifier*actualElapsed;
		}
	}
	if (Entities[11]->x > 1.25){
		for (int i = 1; i < Entities.size(); i++){
			Entities[i]->y -= 0.05f;
		}
		if (!AlienMovement){
			AlienMvtSpeedModifier += 0.06f;
		}
		AlienMovement = true;
	}
	else if (Entities[1]->x < -1.25){
		for (int i = 1; i < Entities.size(); i++){
			Entities[i]->y -= 0.05f;
		}
		if (AlienMovement){
			AlienMvtSpeedModifier += 0.06f;
		}
		AlienMovement = false;
	}


};

void App::FixedUpdate(){
	keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_RIGHT]){
		player.x -= player.velocity_x*FIXED_TIMESTEP;
	}
	if (keys[SDL_SCANCODE_LEFT]){
		player.x += player.velocity_x*FIXED_TIMESTEP;
	}
}

void App::Render(){
	glClear(GL_COLOR_BUFFER_BIT);//Makes background default color

	background.Render();

	player.Render();

	for (int i = 0; i < Entities.size(); i++){
		Entities[i]->Render();
	}
	stringstream ScorenValue;
	ScorenValue << "Score: " << Score;
	string text = ScorenValue.str().c_str();
	drawText(font, text, 0.1f, -0.05f, 1.0f, 0.0f, 0.0f, 1.0f, -1.15f, -.95f);

	text = "Lives: ";

	drawText(font, text, 0.1f, -0.05f, 0.0f, 1.0f, 0.0f, 1.0f, .55f, -.95f);
	for (int i = 0; i < 3; i++){
		playerlives[i].Render();
	}


	SDL_GL_SwapWindow(displayWindow);
};

void App::UpdateandRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEP) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEP;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}

	timeLeftOver = fixedElapsed;

	Update();

	Render();

	player.resetCollisions();
}
