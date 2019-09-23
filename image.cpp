#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	
	// init sdl
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init error: %s", SDL_GetError());
		return 1;
	}

	// create window
	SDL_Window *win = SDL_CreateWindow("Goodbye world", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		printf("SDL_CreateWindow error: %s", SDL_GetError());
		SDL_Quit();
		return 2;
	}

	// create renderer
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 3;
	}

	// load bitmap image
	SDL_Surface *bmp = SDL_LoadBMP("bitmap.bmp");
	if (bmp == nullptr) {
		printf("Error loading bitmap: %s", SDL_GetError());
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 4;
	}

	// upload image to the renderer
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if (tex == nullptr) {
		printf("Error creating texture: %s", SDL_GetError());
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 5;
	}

	// draw the texture
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, tex, NULL, NULL);
	SDL_RenderPresent(ren);
	SDL_Delay(3000);


	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

