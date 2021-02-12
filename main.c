#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"
#include "option.h"
#include "result.h"
#include "list.h"
#include "map.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SPRITE_SIZE 32
#define SMALL_SPRITE_SIZE 16
#define TINY_SPRITE_SIZE 8
#define TINY_SPRITE_SCALE 4

typedef struct {
	u8 r;
	u8 g;
	u8 b;
	u8 a;
} RgbaColor;

#define RGB(rv,gv,bv) {.r = (rv), .g = (gv), .b = (bv), .a = 255 }


/* sprites */
// 16x16 sprites
const u8 g_mouse_sprite_1byte[256] = {
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,255,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,255,255,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,255,255,255,255,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  1,255,255,255,255,  1,  1,  1,  1,  0,  0,  1,  0,  0,
  0,  0,  1,255,255,255,255,255,255,255,255,  1,  1,  0,  0,  0,
  0,  0,  0,  1,255,255,255,255,255,255,255,255,  1,  0,  0,  0,
  0,  0,  0,  0,  1,255,255,255,255,255,255,255,  1,  0,  0,  0,
  0,  0,  0,  0,  1,255,255,255,255,255,255,255,  1,  0,  0,  0,
  0,  0,  0,  0,  1,255,255,255,255,255,255,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  1,255,255,255,255,255,255,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  1,255,255,255,  1,  1,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  1,  1,255,  1,  0,  0,
  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  1,255,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,255,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1
};

const u8 g_creature_sprite_1byte[256] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,255,255,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,255,255,255,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,255,255,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  1,255,255,255,255,  1,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  1,255,255,255,255,255,255,255,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  1,255,255,255,255,255,255,255,  1,  0,  0,  0,  0,
  0,  0,  0,  1,255,255,255,255,255,255,255,255,  1,  0,  0,  0,
  0,  0,  1,  1,  1,255,  1,  1,  1,255,255,255,255,  1,  1,  0,
  0,  1,255,  1,  1,  1,  1,  0,  1,255,  1,255,255,255,255,  1,
  0,  1,  1,  0,  0,  0,  0,  0,  1,255,  1,  1,  1,255,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,255,  1,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,255,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 8x8 sprites
const RgbaColor g_grass_tile_rgba[64] = {
RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),
RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),
RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),
RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),
RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),
RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),
RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),
RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),RGB(120,255,100),
};

const RgbaColor g_water_tile_rgba[64] = {
RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),
RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),
RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),
RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),
RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),
RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),
RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),
RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),RGB(120,100,255),
};

/* sprites */

// \/ from https://wiki.libsdl.org/SDL_CreateRGBSurface

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
const Uint32 g_rmask = 0xff000000;
const Uint32 g_gmask = 0x00ff0000;
const Uint32 g_bmask = 0x0000ff00;
const Uint32 g_amask = 0x000000ff;
#else
const Uint32 g_rmask = 0x000000ff;
const Uint32 g_gmask = 0x0000ff00;
const Uint32 g_bmask = 0x00ff0000;
const Uint32 g_amask = 0xff000000;
#endif
// /\ from https://wiki.libsdl.org/SDL_CreateRGBSurface

SDL_Texture* CreateTinyTexture4Byte(SDL_Renderer* renderer, const RgbaColor sprite[64]) {

	// TODO: get size from sprite info?
	SDL_Surface* temp = SDL_CreateRGBSurface(0, TINY_SPRITE_SIZE, TINY_SPRITE_SIZE, 32, g_rmask, g_gmask, g_bmask, g_amask);


	int len = temp->h * temp->pitch;
	int i;
	int c;
	for (i = 0, c = 0; i < len; i+=4, ++c) {
		*((Uint8*)temp->pixels + i) = sprite[c].r; // R
		*((Uint8*)temp->pixels + i + 1) = sprite[c].g; // G
		*((Uint8*)temp->pixels + i + 2) = sprite[c].b; // B
		*((Uint8*)temp->pixels + i + 3) = sprite[c].a; // A
	}
	
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	return tex;
}

SDL_Texture* CreateSmallTexture1Byte(SDL_Renderer* renderer, const u8 sprite[256]) {

	// TODO: get size from sprite info?
	SDL_Surface* temp = SDL_CreateRGBSurface(0, SMALL_SPRITE_SIZE, SMALL_SPRITE_SIZE, 32, g_rmask, g_gmask, g_bmask, g_amask);


	int len = temp->h * temp->pitch;
	int i;
	int c;
	for (i = 0, c = 0; i < len; i+=4, ++c) {
		if (sprite[c]) { // if sprite[c] > 0
			*((Uint8*)temp->pixels + i) = sprite[c]; // R
			*((Uint8*)temp->pixels + i + 1) = sprite[c]; // G
			*((Uint8*)temp->pixels + i + 2) = sprite[c]; // B
			*((Uint8*)temp->pixels + i + 3) = 255; // A
		}
	}
	
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	return tex;
}

SDL_Texture* CreateTexture(SDL_Renderer* renderer) {

	// TODO: get size from sprite info?
	SDL_Surface* temp = SDL_CreateRGBSurface(0, SPRITE_SIZE, SPRITE_SIZE, 32, g_rmask, g_gmask, g_bmask, g_amask);


	int len = temp->h * temp->pitch;
	int i;
	for (i = 0; i < len; i+=4) {
		*((Uint8*)temp->pixels + i) = 255;
		*((Uint8*)temp->pixels + i + 1) = (i % 250);
		*((Uint8*)temp->pixels + i + 2) = 0;
		*((Uint8*)temp->pixels + i + 3) = 255;
	}
	
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	return tex;
}

/*=============================Timer================================*/

// callback codes
#define CODE_NONE 0
#define CODE_PROCESS_CREATURES 1

// callback
Uint32 timer_callback(Uint32 interval, void *param) {
	*(u8*)param = ((*(u8*)param) % 40) + 1;

	SDL_Event event;
	SDL_UserEvent user_event;

	user_event.type = SDL_USEREVENT;
	if (*(u8*)param % 20 == 0) {
		user_event.code = CODE_PROCESS_CREATURES;
	} else {
		user_event.code = CODE_NONE;
	}
	user_event.data1 = NULL;
	user_event.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = user_event;

	SDL_PushEvent(&event);
	return interval;
}

/*=============================Main================================*/

int main(int argc, char *argv[]) {
	// initialize SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	// initialize application specific values
	u8 timer_count = 0;
	Uint32 timer_delay = 50;
	SDL_TimerID process_timer = SDL_AddTimer(timer_delay, timer_callback, &timer_count);

	// create window
	SDL_Window* window = SDL_CreateWindow("Very Exciting",
										  SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED,
										  SCREEN_WIDTH,
										  SCREEN_HEIGHT,
										  SDL_WINDOW_SHOWN);

	// hide cursor
	SDL_ShowCursor(SDL_DISABLE);

	// setup renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	// setup textures
	SDL_Texture* tex_mouse = CreateSmallTexture1Byte(renderer, g_mouse_sprite_1byte);
	SDL_Texture* texture = CreateTexture(renderer);
	SDL_Texture* tex_grass_tile = CreateTinyTexture4Byte(renderer, g_grass_tile_rgba);
	SDL_Texture* tex_water_tile = CreateTinyTexture4Byte(renderer, g_water_tile_rgba);
	SDL_Texture* tex_creature = CreateSmallTexture1Byte(renderer, g_creature_sprite_1byte);

	// setup basic rectangles
	SDL_Rect tex_map_tile_r = { 
		.x = 0,
		.y = 0,
		.w = TINY_SPRITE_SIZE * TINY_SPRITE_SCALE,
		.h = TINY_SPRITE_SIZE * TINY_SPRITE_SCALE };

	SDL_Rect tex_mouse_draw_r = { 
		.x = (SCREEN_WIDTH / 2) + 16 + (SPRITE_SIZE * 2),
		.y = (SCREEN_HEIGHT / 2) + 16+ (SPRITE_SIZE * 2),
		.w = SMALL_SPRITE_SIZE,
		.h = SMALL_SPRITE_SIZE};

	SDL_Rect tex_creature_draw_r = { 
		.x = (SCREEN_WIDTH / 2) - (SPRITE_SIZE * 2),
		.y = (SCREEN_HEIGHT / 2) - (SPRITE_SIZE * 2),
		.w = SMALL_SPRITE_SIZE,
		.h = SMALL_SPRITE_SIZE};

	// might as well do this with a macro, but w/e
	u16 max_x = SCREEN_WIDTH / (TINY_SPRITE_SIZE * TINY_SPRITE_SCALE);
	u16 max_y = SCREEN_HEIGHT / (TINY_SPRITE_SIZE * TINY_SPRITE_SCALE);

	u8 creature_state = 0;


	// general design idea for the threads/timers
	// a SDL timer spawns a new thread,
	// so keep rendering (and input?) code on main thread,
	// process timed events (creature movements etc) on the timer thread
	// it's a place to start at least
	// if the "AI" takes too long to process, perhaps move that to a separate thread entirely, we'll see, another solution might be to modularize the AI and do it in parts, or performing half of the "AI" objects at a time

	SDL_Event event;
	bool gameover = false;

	while(!gameover) {
		if(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					gameover = true;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
						case SDLK_q:
							gameover = true;
							break;
					}
					break;
				case SDL_MOUSEMOTION:
					tex_mouse_draw_r.x = event.motion.x;
					tex_mouse_draw_r.y = event.motion.y;
					break;
				case SDL_USEREVENT:
					switch(event.user.code) {
						case CODE_PROCESS_CREATURES:
							creature_state = (creature_state + 1) % 4;
							switch(creature_state) {
								case 0:
									tex_creature_draw_r.x += SMALL_SPRITE_SIZE;
									break;
								case 1:
									tex_creature_draw_r.y += SMALL_SPRITE_SIZE;
									break;
								case 2:
									tex_creature_draw_r.y -= SMALL_SPRITE_SIZE;
									break;
								case 3:
									tex_creature_draw_r.x -= SMALL_SPRITE_SIZE;
									break;
							}
							break;
					}
					break;
			}
		}
		
		// clear screen
		SDL_RenderClear(renderer);

		// draw to renderer---

		// API is (_, _, src rect, dst rect)
		// [rects are of type SDL_Rect*]
		// src rect == NULL means drawing the entire texture
		// dst rect == NULL means drawing the texture over the entire rendering target

		// draw map (defined in map.h)
		for(u16 y = 0; y < max_y; ++y) {
			for(u16 x = 0; x < max_x; ++x) {
				tex_map_tile_r.x = TINY_SPRITE_SIZE * TINY_SPRITE_SCALE * x;
				tex_map_tile_r.y = TINY_SPRITE_SIZE * TINY_SPRITE_SCALE * y;
				switch(g_game_map[(y * max_x) + x]) {
					case 1:
						SDL_RenderCopy(renderer, tex_grass_tile, NULL, &tex_map_tile_r);
						break;
					case 2:
						SDL_RenderCopy(renderer, tex_water_tile, NULL, &tex_map_tile_r);
						break;
				}
			}
		}

		// draw creature
		SDL_RenderCopy(renderer, tex_creature, NULL, &tex_creature_draw_r);


		// draw mouse over everything else
		SDL_RenderCopy(renderer, tex_mouse, NULL, &tex_mouse_draw_r);
		// ---

		// update the screen
		SDL_RenderPresent(renderer);

		SDL_Delay(1); // TODO: eventually change to SDL_Delay(0);
	}

	// destroy all textures
	SDL_DestroyTexture(tex_mouse);
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(tex_grass_tile);
	SDL_DestroyTexture(tex_water_tile);
	SDL_DestroyTexture(tex_creature);

	// destroy renderer
	SDL_DestroyRenderer(renderer);

	// destroy all windows
	SDL_DestroyWindow(window);

	// destroy all timers
	SDL_RemoveTimer(process_timer);
	
	// quit SDL
	SDL_Quit();
	return 0;
}

