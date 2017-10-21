#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "main_menu.h"

int main()
{
	bool terminate = false;

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());

		return EXIT_FAILURE;
	}

	SDL_Window* window = NULL;
	window = SDL_CreateWindow("BILLY 2 : Shpïone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1600, 900, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_SHOWN);

	if (window)
	{
	    while(!terminate)
	    {
			SDL_Renderer *pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // Création d'un SDL_Renderer utilisant l'accélération matérielle
			if (pRenderer)
			{
				SDL_Surface* pSprite = SDL_LoadBMP("./Pictures/main_menu_wallpaper.bmp");
				if (pSprite)
				{
					SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pSprite); // Préparation du sprite
					if (pTexture)
					{
						SDL_Rect dest = {0, 0, pSprite->w, pSprite->h};
						SDL_RenderCopy(pRenderer, pTexture, NULL, &dest); // Copie du sprite grâce au SDL_Renderer
						  
						SDL_RenderPresent(pRenderer); // Affichage
						  
						SDL_DestroyTexture(pTexture); // Libération de la mémoire associée à la texture
					}
					else
					{
						fprintf(stderr,"Échec de création de la texture (%s)\n",SDL_GetError());
					}
					   SDL_FreeSurface(pSprite); // Libération de la ressource occupée par le sprite
					}
					else
					{
					   fprintf(stderr,"Échec de chargement du sprite (%s)\n",SDL_GetError());
					}
					SDL_DestroyRenderer(pRenderer); // Libération de la mémoire du SDL_Renderer
				}
				else
				{
					fprintf(stderr,"Échec de création du renderer (%s)\n",SDL_GetError());
				}

					SDL_UpdateWindowSurface(window);
					SDL_Delay(1000); /* Attendre trois secondes, que l'utilisateur voie la fenêtre */

					SDL_DestroyWindow(window);
			}
		}
	else
	{
		fprintf(stderr,"Erreur de création de la fenêtre: %s\n", SDL_GetError());
	}

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
