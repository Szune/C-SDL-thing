#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"
#include "option.h"
#include "result.h"
#include "list.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SPRITE_SIZE 32
#define SMALL_SPRITE_SIZE 16

/* sprites */
// 16x16 sprite
const bool g_mouse_sprite[256] = {
1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
0,0,1,1,1,1,1,1,1,1,1,0,0,1,0,0,
0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,
0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,
0,0,0,0,0,1,1,1,1,0,1,0,1,0,0,0,
0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
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

SDL_Texture* CreateSmallTexture(SDL_Renderer* renderer, const bool sprite[256]) {

	// TODO: get size from sprite info?
	SDL_Surface* temp = SDL_CreateRGBSurface(0, SMALL_SPRITE_SIZE, SMALL_SPRITE_SIZE, 32, g_rmask, g_gmask, g_bmask, g_amask);


	int len = temp->h * temp->pitch;
	int i;
	int c;
	for (i = 0, c = 0; i < len; i+=4, ++c) {
		if (sprite[c]) {
			*((Uint8*)temp->pixels + i) = 255;
			*((Uint8*)temp->pixels + i + 1) = 255;
			*((Uint8*)temp->pixels + i + 2) = 255;
			*((Uint8*)temp->pixels + i + 3) = 255;
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

/*=============================Main================================*/

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Very Exciting",
										  SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED,
										  SCREEN_WIDTH,
										  SCREEN_HEIGHT,
										  SDL_WINDOW_SHOWN);

	// hide cursor
	SDL_ShowCursor(SDL_DISABLE);

	// setup palette
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture* tex_mouse = CreateSmallTexture(renderer, g_mouse_sprite);
	SDL_Texture* texture = CreateTexture(renderer);

	SDL_Surface* screen = SDL_GetWindowSurface(window);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x33, 0x33, 0x33)); // set background to #333
	// ^ gets overridden by the renderer, need to redraw it
	SDL_UpdateWindowSurface(window);
	SDL_Event event;
	bool gameover = false;

	SDL_Rect tex_draw_r = { 
		.x = SCREEN_WIDTH / 2,
		.y = SCREEN_HEIGHT / 2,
		.w = SPRITE_SIZE * 2,
		.h = SPRITE_SIZE * 2 };

	SDL_Rect tex_mouse_draw_r = { 
		.x = (SCREEN_WIDTH / 2) + 16 + (SPRITE_SIZE * 2),
		.y = (SCREEN_HEIGHT / 2) + 16+ (SPRITE_SIZE * 2),
		.w = SMALL_SPRITE_SIZE,
		.h = SMALL_SPRITE_SIZE};

	while(!gameover) {
		if(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					gameover = true;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
						//case SDLK_q:
							gameover = true;
							break;
					}
					break;
				case SDL_MOUSEMOTION:
					tex_mouse_draw_r.x = event.motion.x;
					tex_mouse_draw_r.y = event.motion.y;
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
		SDL_RenderCopy(renderer, texture, NULL, &tex_draw_r);
		SDL_RenderCopy(renderer, tex_mouse, NULL, &tex_mouse_draw_r);
		// ---

		// update the screen
		SDL_RenderPresent(renderer);

		SDL_Delay(1); // TODO: eventually change to SDL_Delay(0);
	}

	// destroy all textures
	SDL_DestroyTexture(tex_mouse);
	SDL_DestroyTexture(texture);

	// destroy renderer
	SDL_DestroyRenderer(renderer);

	// destroy all windows
	SDL_DestroyWindow(window);
	
	// quit SDL
	SDL_Quit();
	return 0;
}

/*
	printf("===Args===\n");
	for(int i = 0; i < argc; ++i) {
		printf("Arg: %s\n", argv[i]);
	}
	printf("==========\n");
	List_u8* test = list_u8_create(3);
	list_u8_add(test, 10);
	list_u8_add(test, 15);
	for(u64 i = 0; i < test->length; ++i) {
		printf("Value in 'generic' list: %i\n", list_u8_get(test, i));
	}
	list_u8_free(test);

	Option_u8 scared = square_em(15);
	if(scared.is_some) {
		printf("Scared was some: %i\n", scared.value);
	} else {
		printf("Scared was none :(\n");
	}

	Result_u8 resulto = add_them(5,6);

	if(resulto.is_success) {
		printf("Resulto was success: %i\n", resulto.value_or_error.value);
	} else {
		printf("Resulto was failure '%s' :(\n", resulto.value_or_error.error);
	}
*/

/*
u8 val = 158;
printf("Value: %i\n", val);
List* list = list_create(5, sizeof(u8));
list_add(list, (void*)10);
list_add(list, (void*)15);
list_add(list, (void*)20);
for(u64 i = 0; i < list->length; ++i) {
	printf("Value in list: %i\n", (u8)list->items[i]);
}
list_free(list);
*/


//Result res = {.is_success = true,.value_or_error.value = &val};
/*
Result res_ok = OK(&val);
printf("Result: success = %i, value = %u\n", res_ok.is_success, *(u8*)res_ok.value_or_error.value);
Result res_err = ERR("Huge error");
printf("Result: success = %i, value = %s\n", res_err.is_success, res_err.value_or_error.error);
u8 x = 1 + 1;
printf("Value: %i\n", x);
*/

/*
// Handling Result types with void pointers: 
Result addem = add_them(11, 12);
if (addem.is_success && addem.value_or_error.value) {
	printf("Result: success = %i, value = %i\n", addem.is_success, *(u8*) addem.value_or_error.value);
} else {
	if (addem.value_or_error.error) {
		printf("Result: success = %i, error = %s\n", addem.is_success, addem.value_or_error.error);
	} else {
		printf("Result: success = %i, no error\n", addem.is_success);
	}
}
*/

/* 
// Handling Option types with void pointers: 
Option squared = square_em(15);
if (squared.is_some && squared.value) {
	printf("Option: is_some = %i, value = %i\n", squared.is_some, *(u8*) squared.value);
} else {
	printf("Option: is_some = %i\n", squared.is_some);
}
*/



/*
Result_u8 add_them(u8 a, u8 b) {
	if(a < 5 || b < 5) {
		Result_u8 res = ERR("Huge error from add_them");
		return res;
	} else {
		Result_u8 res = OK(a + b);
		//Result res = OK(NULL);
		return res;
	}
}

Option_u8 square_em(u8 a) {
	if(a > 12) {
		Option_u8 res = NONE();
		return res;
	} else {
		Option_u8 res = SOME(a * a);
		return res;
	}
}
*/
