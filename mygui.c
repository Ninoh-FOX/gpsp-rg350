#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "font.h"

extern SDL_Surface *screen;
extern SDL_Surface *display;
SDL_Surface* background_tmp;

extern char gamepak_filename[512];
static int32_t choice_menu = 0;

#define COLOR_BG           	SDL_MapRGB(screen->format,5,3,2)
#define COLOR_OK			SDL_MapRGB(screen->format,0,0,255)
#define COLOR_KO			SDL_MapRGB(screen->format,255,0,0)
#define COLOR_INFO			SDL_MapRGB(screen->format,0,255,0)
#define COLOR_LIGHT			SDL_MapRGB(screen->format,255,255,0)
#define COLOR_ACTIVE_ITEM   SDL_MapRGB(screen->format,255, 255, 255)
#define COLOR_INACTIVE_ITEM SDL_MapRGB(screen->format,255,255,255)

/* Shows menu items and pointing arrow	*/
#define SPRX (16)
#define OFF_X 0

/* Re-adujusting Menu Y position */
#define OFF_Y (-6)

static uint16_t GBA_screen[(240*160)*2];

static int32_t savestate_slot = 0;
extern int load_state(const char *savestate_filename);
extern int save_state(const char *savestate_filename, uint16_t *screen_capture);

extern int screen_scale;

static void screen_showchar(SDL_Surface *s, int32_t x, int32_t y, uint8_t a, const int32_t fg_color, const int32_t bg_color) 
{
	uint16_t *dst;
	uint16_t w, h;
	SDL_LockSurface(s);
	for(h = 8; h; h--) 
	{
		dst = (uint16_t *)s->pixels + ((y+8-h)*s->w + x);
		for(w = 8; w; w--) 
		{
			uint16_t color = *dst; /* background */
			if((fontdata8x8[a*8 + (8-h)] >> w) & 1) color = fg_color;
			*dst++ = color;
		}
	}
	SDL_UnlockSurface(s);
}

static void print_string_menu(const int8_t *s, const  uint16_t fg_color, const uint16_t bg_color, int32_t x, int32_t y) 
{
	int32_t i, j = strlen(s);
	for(i = 0; i < j; i++, x += 6)
	{
		screen_showchar(screen, x, y, s[i], fg_color, bg_color);
	}
}

uint32_t MyGUI()
{
	char current_savestate_filename[256];
	SDL_Event event;
	uint8_t done = 0;
	
	savestate_slot = 0;
	choice_menu = 0;
	
	while(!done)
	{
		SDL_BlitSurface(background_tmp, NULL, screen, NULL);
		print_string_menu("GPSP for RS-97, Arcade Mini", COLOR_ACTIVE_ITEM, 0, 16, 16); 
		
		print_string_menu("Go back to Game", COLOR_ACTIVE_ITEM, 0, 32, 48); 
		
		print_string_menu("Load State", COLOR_ACTIVE_ITEM, 0, 32, 64); 
		
		print_string_menu("Save State", COLOR_ACTIVE_ITEM, 0, 32, 80); 
		
		switch(screen_scale)
		{
			case 0:
				print_string_menu("Scaling : None", COLOR_ACTIVE_ITEM, 0, 32, 96); 
			break;
			case 1:
				print_string_menu("Scaling : Aspect", COLOR_ACTIVE_ITEM, 0, 32, 96); 
			break;
			case 2:
				print_string_menu("Scaling : Fullscreen", COLOR_ACTIVE_ITEM, 0, 32, 96); 
			break;
		}
		
		
		print_string_menu("Exit GPSP", COLOR_ACTIVE_ITEM, 0, 32, 112); 
		
		print_string_menu("=>", COLOR_LIGHT, 0, 16, 48 + (choice_menu * 16)); 
		
		SDL_SoftStretch(screen, NULL, display, NULL);
		SDL_Flip(display);
		while( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
				case SDL_KEYDOWN:
				switch( event.key.keysym.sym )
                {
                    case SDLK_LEFT:
                        switch(choice_menu)
                        {
							case 3:
								screen_scale -= 1;
								if (screen_scale < 0) screen_scale = 2;
							break;
						}
					break;
                    case SDLK_RIGHT:
                        switch(choice_menu)
                        {
							case 3:
								screen_scale += 1;
								if (screen_scale > 2) screen_scale = 0;
							break;
						}
					break;
                    case SDLK_UP:
                        choice_menu -= 1;
                        if (choice_menu < 0) choice_menu = 0;
					break;
                    case SDLK_DOWN:
                        choice_menu += 1;
                        if (choice_menu > 4) choice_menu = 4;
					break;
                    case SDLK_LCTRL:
                        switch(choice_menu)
                        {
							case 0:
								done = 1;
							break;
							/* Load state */
							case 1:
								//get_savestate_filename_noshot(savestate_slot, current_savestate_filename);
								//
								//snprintf(current_savestate_filename, sizeof(current_savestate_filename), "%s/.gpsp/%s.svs", getenv("HOME"), gamepak_filename);
								//snprintf(tmp_fname, sizeof(tmp_fname), "%s/.gpsp/%s.svs", getenv("HOME"), gamepak_filename);
								//strcpy(current_savestate_filename, tmp_fname);
								
								strcpy(current_savestate_filename, gamepak_filename);
								strcat(current_savestate_filename, ".svs");
								
								load_state((const char*)current_savestate_filename);
								done = 1;
							break;
							/* Save State */
							case 2:
								memset(GBA_screen, 0, sizeof(GBA_screen));
								//current_screen = copy_screen();
								//get_savestate_filename_noshot(savestate_slot, current_savestate_filename);
								//snprintf(tmp_fname, sizeof(tmp_fname), "%s/.gpsp/%s.svs", getenv("HOME"), gamepak_filename);
								
								strcpy(current_savestate_filename, gamepak_filename);
								strcat(current_savestate_filename, ".svs");
								
								save_state((const char*)current_savestate_filename, GBA_screen);
								//free(current_screen);
							break;
							case 3:
								screen_scale += 1;
								if (screen_scale > 2) screen_scale = 0;
							break;
							case 4:
								done = 2;
								//save_config_file();
								//quit();
								
							break;
						}
					break;
					default:
					break;
                }
				break;
				default:
				break;
			}
		}
	}
	SDL_FillRect(screen, NULL, 0);
	SDL_FillRect(display, NULL, 0);
	SDL_Flip(display);
	
	if (done == 2) return 1;
	else return 0;
}

