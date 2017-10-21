#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

int msaa = 0;
unsigned int lastTime, currentTime;
SDL_Window *screen;
SDL_GLContext glcontext;

float angleX = 0, angleY = 0, altitude = 0;

void draw(SDL_Window *);
void SDLGLSetup();


int main(int argc, char *argv[])
{
    glewInit();
    SDLGLSetup();

    SDL_Event event;
    bool terminate = false;
    while(!terminate){
        SDL_PollEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                terminate = true;
        }
        
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        
        if (state[SDL_SCANCODE_ESCAPE])
            terminate = true;;
        if (state[SDL_SCANCODE_W])
            angleY += 0.02;
        if (state[SDL_SCANCODE_S])
            angleY -= 0.02;
        if (state[SDL_SCANCODE_A])
            angleX += 0.02;
        if (state[SDL_SCANCODE_D])
            angleX -= 0.02;
        if (state[SDL_SCANCODE_SPACE])
            altitude += 1;
        if (state[SDL_SCANCODE_C])
            altitude -= 1;
        draw(screen);

        SDL_Delay(10);
    }


    return 0;
}

void draw(SDL_Window * screen){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glm::ortho(-10.f, 10.f, -10.f, 10.f);//
    glm::mat4 projection = glm::perspective(100., (double) WINDOW_WIDTH/WINDOW_HEIGHT, 1., 5000.);
    glm::mat4 modelview = glm::mat4(1.0);
    /*
    modelview = glm::rotate(modelview, angleY, glm::vec3(1.,0.,0.));
    modelview = glm::rotate(modelview, angleX, glm::vec3(0.,0.,1.));
    */
    //modelview = glm::scale(glm::vec3(altitude, altitude, altitude));
    modelview = glm::translate(modelview, glm::vec3(0, 0, altitude));
    modelview = glm::rotate(modelview, angleY, glm::vec3(0,0,1));
    modelview = glm::rotate(modelview, angleX, glm::vec3(0,1,0));
    
    //g_Model->Render(projection, modelview);
    
    glFlush();
    SDL_GL_SwapWindow(screen);
}

void SDLGLSetup(){
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);

    if(msaa){
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msaa);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    screen = SDL_CreateWindow("SDL GL Application",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                WINDOW_WIDTH, WINDOW_HEIGHT,
                                SDL_WINDOW_OPENGL);

    SDL_GL_CreateContext(screen);

    if(msaa)
        glEnable(GLUT_MULTISAMPLE);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}