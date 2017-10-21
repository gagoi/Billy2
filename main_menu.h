#ifndef MAIN_MENU_H_INCLUDED
#define MAIN_MENU_H_INCLUDED

void loadBackground(SDL_Renderer *pRenderer, SDL_Surface *pSprite, SDL_Texture *pTexture);
void loadTitle(SDL_Renderer *pRenderer, SDL_Surface *pSprite, SDL_Texture *pTexture);
void loadPlayButton(SDL_Renderer *pRenderer, SDL_Surface *pSprite, SDL_Texture *pTexture);
void loadQuitButton(SDL_Renderer *pRenderer, SDL_Surface *pSprite, SDL_Texture *pTexture);
void printAvailableResolutions(int screenIndex = 0);

#endif // main_menu.h is included