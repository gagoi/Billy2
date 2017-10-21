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
#include "Shader.h"



#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

int msaa = 0;
unsigned int lastTime, currentTime;
SDL_Window *screen;
SDL_GLContext glcontext;

float angleX = 0, angleY = 0, altitude = 0;

void draw(SDL_Window *);
void SDLGLSetup();
void prepareVBO();

Shader * occlusionShader;
Shader * drawShader;

GLuint vboID;
GLuint occlusionTexture;
GLuint occlusionBuffer;

float bufferInfos[16] = {-1, -1,
                        1, -1,
                        1, 1,
                        -1, 1,
                        0, 0,
                        1, 0,
                        1, 1,
                        0, 1};

int main(int argc, char *argv[])
{
    SDLGLSetup();
    glewInit();
    prepareVBO();

    occlusionShader = new Shader(std::string("Shaders/shader2D.vert"), std::string("Shaders/occlusion2D.frag"));
    drawShader = new Shader(std::string("Shaders/shader2D.vert"), std::string("Shaders/shader2D.frag"));

    occlusionShader->charger();
    drawShader->charger();

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

    glm::ortho(-10.f, 10.f, -10.f, 10.f);
    glm::mat4 projection = glm::ortho(-10.f, 10.f, -10.f, 10.f);
    glm::mat4 modelview = glm::mat4(1.0);
    

    glUseProgram(occlusionShader->getProgramID());
        glGenFramebuffers(1, &occlusionBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, occlusionBuffer);

            glUniformMatrix4fv(glGetUniformLocation(occlusionShader->getProgramID(), "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(occlusionShader->getProgramID(), "modelview"), 1, GL_FALSE, &modelview[0][0]);
                
            glBindBuffer(GL_ARRAY_BUFFER, vboID);

                glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            //draw scene with only walls

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);

    glUseProgram(drawShader->getProgramID());

        glBindBuffer(GL_ARRAY_BUFFER, vboID);
    
            glUniformMatrix4fv(glGetUniformLocation(occlusionShader->getProgramID(), "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(occlusionShader->getProgramID(), "modelview"), 1, GL_FALSE, &modelview[0][0]);
            
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(8));
            glEnableVertexAttribArray(1);

            glBindTexture(GL_TEXTURE_2D, occlusionTexture);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
            glBindTexture(GL_TEXTURE_2D, 0);
                    
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(0);



    //occlusion is now the occlusion map texture

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
                                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_GL_CreateContext(screen);

    if(msaa)
        glEnable(GLUT_MULTISAMPLE);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void prepareVBO(){

    glGenFramebuffers(1, &occlusionBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, occlusionBuffer);
    
        glGenTextures(1, &occlusionTexture);
        glBindTexture(GL_TEXTURE_2D, occlusionTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB,
            GL_UNSIGNED_BYTE, 0);
            
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, occlusionTexture, 0);
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);

        glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), &bufferInfos, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER , 0);
}