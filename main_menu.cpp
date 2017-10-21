#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "main_menu.h"

int main()
{
	bool terminate = false;
	SDL_Window* window = NULL;
	SDL_Event event;
	SDL_Renderer *pRenderer;
	SDL_Surface* pSprite;
	SDL_Texture* pTexture;
	SDL_Rect dest;

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());

		return EXIT_FAILURE;
	}

	window = SDL_CreateWindow("BILLY 2 : Shpïone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1600, 900, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_SHOWN);

	if (window)
	{
		pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // Création d'un SDL_Renderer utilisant l'accélération matérielle
		if (pRenderer)
		{
			pSprite = SDL_LoadBMP("./Pictures/main_menu_wallpaper.bmp");
			if (pSprite)
			{
				pTexture = SDL_CreateTextureFromSurface(pRenderer, pSprite); // Préparation du sprite
				if (pTexture)
				{
					dest = {0, 0, pSprite->w, pSprite->h};
					SDL_RenderCopy(pRenderer, pTexture, NULL, &dest); // Copie du sprite grâce au SDL_Renderer
					  
					SDL_RenderPresent(pRenderer); // Affichage
				}
				else
				{
					fprintf(stderr,"Échec de création de la texture (%s)\n",SDL_GetError());
				}
			}
			else
			{
			   fprintf(stderr,"Échec de chargement du sprite (%s)\n",SDL_GetError());
			}
		}
		else
		{
			fprintf(stderr,"Échec de création du renderer (%s)\n",SDL_GetError());
		}

	    while(!terminate)
	    {
	    	if (SDL_PollEvent(&event))
	    	{
	        	switch(event.type)
	        	{
	            	case SDL_KEYDOWN:
	            		if (event.key.keysym.sym == SDLK_q)
	            		{
	                		terminate = true;
	            		}
	                	break;
	        	}
	    	}
	    	SDL_UpdateWindowSurface(window);
		}
		SDL_DestroyTexture(pTexture);
		SDL_FreeSurface(pSprite);
		SDL_DestroyRenderer(pRenderer);
	}

	else
	{
		fprintf(stderr,"Erreur de création de la fenêtre: %s\n", SDL_GetError());
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

void printResolutions(int screenIndex)
{
	int modeNumber = 0;
	int i = 0;
	int error = 0;
	SDL_DisplayMode displayMode;

	modeNumber = SDL_GetNumDisplayModes(screenIndex);

	if (modeNumber < 0)
	{
		fprintf(stderr,"Échec lors de la récupération du nombre de modes (%s)\n", SDL_GetError());
	}
	fprintf(stdout,"Vous avez %d mode(s) d'affichage\n",modeNumber);

	// Affichage des différents modes
	for (i = 0 ; i < modeNumber ; i++)
	{
		error = SDL_GetDisplayMode(0,i,&displayMode);
		if (error < 0)
		{
			fprintf(stderr,"Échec lors de la récupération du mode d'affichage (%s)\n", SDL_GetError());
		}
		
		fprintf(stdout,"Mode %d : %dx%d@%dfps\n", i, displayMode.w, displayMode.h, displayMode.refresh_rate);
	}
}
