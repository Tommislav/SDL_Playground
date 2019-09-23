#include <SDL2/SDL.h>
#include <stdio.h>

SDL_Rect GetRect(int x, int y, int w, int h) {
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}


SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
SDL_Texture* texture; 
Uint64 lastTime = 0;

int InitSDL(const char* title, int w, int h) {
	// init sdl
	if (SDL_Init(SDL_INIT_VIDEO) != 0) { return 1; }

	// create window
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	if (window == nullptr) { return 2; }

	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) { return 3; }

	// load bitmap image
	surface = SDL_LoadBMP("spritesheet.bmp");
	if (surface == nullptr) { return 4; }

	// upload image to the renderer
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (texture == nullptr) { return 5; }

	return 0;
}

void QuitSDL() {
	if (surface != nullptr) { SDL_FreeSurface(surface); }
	if (texture != nullptr) { SDL_DestroyTexture(texture); }
	if (renderer != nullptr) { SDL_DestroyRenderer(renderer); }
	if (window != nullptr) { SDL_DestroyWindow(window); }
	SDL_Quit();
}

double GetDeltaTime() {
	Uint64 now = SDL_GetPerformanceCounter();
	double deltaTime = (double)((now - lastTime) * 1000 / (double)SDL_GetPerformanceFrequency());
	lastTime = now;
	return deltaTime;
}

int main(int argc, char* argv[]) {
	
	int status = InitSDL("Goodbye World", 100, 100);
	if (status > 0) {
		QuitSDL();
	}

	// draw the texture
	SDL_Rect src = GetRect(16,16,16,16); 
	SDL_Rect dst = GetRect(10,10,16,16);

	SDL_Event e;
	bool quit = false;
	bool leftDown = false;
	bool rightDown = false;
	int mov = 0;
	int cnt = 0;
	double frameCountDown = 0;

	while(!quit) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT || e.type == SDL_MOUSEBUTTONDOWN) {
				quit = true;
				break;
			}

			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
					break;
				}

				if (e.key.keysym.sym == SDLK_RIGHT) { rightDown = true; }
				else if (e.key.keysym.sym == SDLK_LEFT) { leftDown = true; }
			}

			if (e.type == SDL_KEYUP) {
				if (e.key.keysym.sym == SDLK_RIGHT) { rightDown = false; }
				else if (e.key.keysym.sym == SDLK_LEFT) { leftDown = false; }
			}

		} // end of event polling


		// frame timer!
		frameCountDown -= GetDeltaTime();
		if (frameCountDown <= 0) {
			frameCountDown = 100;
		}
		else {
			continue;
		}


		if (leftDown) { mov = -1; }
		else if (rightDown) { mov = 1; }
		else { mov = 0; }

		dst.x += mov;
		cnt += mov == 0 ? 0 : 1;
		if (cnt > 8) { cnt = 0; }

		src.x = cnt * 17;		

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, &src, &dst);
		SDL_RenderPresent(renderer);
	}

	QuitSDL();
	return 0;
}

