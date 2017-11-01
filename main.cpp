#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include "Shader.h"
#include "Player/Player.h"
#include "Map/map.h"



#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

bool msaa = true;
int fps = 60;
unsigned int lastTime, startTime, timePassed;
SDL_Window *screen;
SDL_GLContext glcontext;

double angleX = 0, angleY = 0, altitude = 0;

void draw(SDL_Window *);
void SDLGLSetup();
void prepareVBO();
void CheckVBO();
GLenum err;

Shader * occlusionShader;
Shader * lightrayShader;
Shader * drawShader;

Player * player;

GLuint vboID; // the 3 rectangles in the scene

GLuint occlusionTexture;
GLuint occlusionFramebuffer;

GLuint lightrayTexture;
GLuint lightrayFramebuffer;
glm::vec2 shadowRes = glm::vec2((float)1000, (float)2000);

const char *layout = "AAAAAAAMAMMMAAAMMMAAAAAAAMAMMMAAAMMMAAAAAAAAAAAAAAAAAAMMMMMMMMMAMMMMMMMMMMMMMMMMMAAAAMMMMMMMMMMMMMMAMMAMMMMMMMMMMMMMMAAAAMMMMMMMMMMMMMMMMMMMMMMM";
int sizeX = 18;
int sizeY = 8;

float scl = 100;
Map * map;

float bufferInfos[32 * 3] =  {
                        0,0,
                        0,0,
                        0,0,
                        0,0,
                        
                        0,0,
                        0,0,
                        0,0,
                        0,0,
                        
                        0,0,
                        0,0,
                        0,0,
                        0,0,
                        
                        0, 0,
                        shadowRes.x, 0,
                        shadowRes.x, WINDOW_HEIGHT,
                        0, WINDOW_HEIGHT,

                        0, 0,
                        WINDOW_WIDTH, 0,
                        WINDOW_WIDTH, WINDOW_HEIGHT,
                        0, WINDOW_HEIGHT,

                        0, 0,
                        WINDOW_WIDTH, 0,
                        WINDOW_WIDTH, WINDOW_HEIGHT,
                        0, WINDOW_HEIGHT,

                        0, 0,
                        1, 0,
                        1, 1,
                        0, 1,
                        0, 0,
                        1, 0,
                        1, 1,
                        0, 1,
                        0, 0,
                        1, 0,
                        1, 1,
                        0, 1,
                        
                        0, 0,
                        1, 0,
                        1, 1,
                        0, 1,
                        0, 0,
                        1, 0,
                        1, 1,
                        0, 1,
                        0, 0,
                        1, 0,
                        1, 1,
                        0, 1};

int main(int argc, char *argv[])
{
    SDLGLSetup();
    glewInit();

    prepareVBO();

    map = new Map(sizeX, sizeY, layout, scl);

    occlusionShader = new Shader(std::string("Shaders/shader2D.vert"), std::string("Shaders/occlusion2D.frag"));
    lightrayShader = new Shader(std::string("Shaders/shader2d.vert"), std::string("Shaders/lightray2D.frag"));
    drawShader = new Shader(std::string("Shaders/shader2D.vert"), std::string("Shaders/shader2D.frag"));

    occlusionShader->charger();
    lightrayShader->charger();
    drawShader->charger();

    player = new Player();
    player->tp(WINDOW_WIDTH/2., WINDOW_HEIGHT/2.);

    SDL_Event event;
    bool terminate = false;

    lastTime = SDL_GetTicks();

    while(!terminate){

        lastTime = startTime;
        startTime = SDL_GetTicks();
        timePassed = startTime - lastTime;

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
            angleY -= timePassed;
        if (state[SDL_SCANCODE_S])
            angleY += timePassed;
        if (state[SDL_SCANCODE_A])
            angleX += timePassed;
        if (state[SDL_SCANCODE_D])
            angleX -= timePassed;
        if (state[SDL_SCANCODE_SPACE])
            altitude += 1;
        if (state[SDL_SCANCODE_C])
            altitude -= 1;
        draw(screen);

        int sleep = 1000./fps - (SDL_GetTicks() - startTime);
        if(sleep > 0)
            SDL_Delay(sleep);
    }


    return 0;
}



void draw(SDL_Window * screen){
    glm::mat4 projection = glm::ortho(0., (double) WINDOW_WIDTH, 0., (double) WINDOW_HEIGHT);
    glm::mat4 modelview = glm::mat4(1.);

    modelview = translate(modelview, glm::vec3(angleX, angleY, 0.));
    
    //Generate occlusion map in occlusionTexture
    glUseProgram(occlusionShader->getProgramID());

        glBindFramebuffer(GL_FRAMEBUFFER, occlusionFramebuffer);

            glClearColor(0,0,0,1);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glUniformMatrix4fv(glGetUniformLocation(occlusionShader->getProgramID(), "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(occlusionShader->getProgramID(), "modelview"), 1, GL_FALSE, &modelview[0][0]);

            for( int i = 0; i < sizeY; i++){
                for( int j = 0; j < sizeX; j++){
                    if(map->getType(j*scl, i*scl) != 0)
                        map->getWall(j*scl, i*scl)->draw();
                }
            }

            //draw scene with only walls

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(0);
    //occlusionTexture is now the occlusion map texture

    modelview = glm::mat4(1.0f);
    projection = glm::ortho(0.,(double) WINDOW_WIDTH, 0., (double) WINDOW_HEIGHT);

    glUseProgram(lightrayShader->getProgramID());

        glBindFramebuffer(GL_FRAMEBUFFER, lightrayFramebuffer);

            glClearColor(0,0,0,0);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glBindBuffer(GL_ARRAY_BUFFER, vboID);
        
                glUniformMatrix4fv(glGetUniformLocation(lightrayShader->getProgramID(), "projection"), 1, GL_FALSE, &projection[0][0]);
                glUniformMatrix4fv(glGetUniformLocation(lightrayShader->getProgramID(), "modelview"), 1, GL_FALSE, &modelview[0][0]);
                
                glUniform2f(glGetUniformLocation(lightrayShader->getProgramID(), "resolution"), shadowRes.x, shadowRes.y);

                glBindTexture(GL_TEXTURE_2D, occlusionTexture);
                    glDrawArrays(GL_TRIANGLE_FAN, 4*3, 4);
                glBindTexture(GL_TEXTURE_2D, 0);
                        
            glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(0);

    projection = glm::ortho(0.,(double) WINDOW_WIDTH, 0., (double) WINDOW_HEIGHT);
    
    glUseProgram(drawShader->getProgramID());

        glClearColor(0,0,0,0);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
    
            glUniformMatrix4fv(glGetUniformLocation(drawShader->getProgramID(), "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(drawShader->getProgramID(), "modelview"), 1, GL_FALSE, &modelview[0][0]);
            glUniform2f(glGetUniformLocation(drawShader->getProgramID(), "resolution"), shadowRes.x, shadowRes.y);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, occlusionTexture);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, lightrayTexture);

                    glDrawArrays(GL_TRIANGLE_FAN, 4*4, 4);
                glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);

            
            while((err = glGetError()) != GL_NO_ERROR)
                std::cout << "Error drawing: " << err << std::endl;

                    
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(0);

    glBegin(GL_QUADS);
        glColor3f(0, 0, 1);
        glVertex2f(-0.01, -0.01*16/9);
        glVertex2f(0.01, -0.01*16/9);
        glVertex2f(0.01, 0.01*16/9);
        glVertex2f(-0.01, 0.01*16/9);
    glEnd();

    SDL_GL_SwapWindow(screen);
}

void SDLGLSetup(){
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);

    GLint * samples;
    glGetIntegerv(GL_MAX_SAMPLES, samples);

    if(msaa){
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, *samples);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    screen = SDL_CreateWindow("SDL GL Application",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                WINDOW_WIDTH, WINDOW_HEIGHT,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    SDL_GL_CreateContext(screen);

    if(msaa)
        glEnable(GL_MULTISAMPLE);

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void prepareVBO(){
    //occlusion texture/framebuffer generation
    glGenFramebuffers(1, &occlusionFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, occlusionFramebuffer);
    
        glGenTextures(1, &occlusionTexture);
        glBindTexture(GL_TEXTURE_2D, occlusionTexture);
        

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB,
            GL_UNSIGNED_BYTE, 0);
            
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, occlusionTexture, 0);
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // lightray texture/framebuffer generation
    glGenFramebuffers(1, &lightrayFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, lightrayFramebuffer);
    
        glGenTextures(1, &lightrayTexture);
        glBindTexture(GL_TEXTURE_2D, lightrayTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, shadowRes.x, 1, 0, GL_RGB,
            GL_UNSIGNED_BYTE, 0);                                 
            
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, lightrayTexture, 0);
        DrawBuffers[0] = GL_COLOR_ATTACHMENT0;
        glDrawBuffers(1, DrawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);

        glBufferData(GL_ARRAY_BUFFER, 32 * 3 * sizeof(float), bufferInfos, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(16*3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER , 0);

    std::cout << "Rendering errors\n";
    while((err = glGetError()) != GL_NO_ERROR){
        std::cout <<std::hex<<"0x"<< err << std::endl;
    }

    //CheckVBO();
}

void CheckVBO(){
    int size = 32*3 * sizeof(float);
    float * data = (float*) calloc(size, sizeof(float));

    
    std::cout << "Errors\n";
    while((err = glGetError()) != GL_NO_ERROR){
        std::cout <<std::hex<<"0x"<< err << std::endl;
    }

    std::cout << "-------------- VBO Content --------------\n\n";

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    while((err = glGetError()) != GL_NO_ERROR){
        std::cout << "Errors retrieving vbo: " <<std::hex<<"0x"<< err << std::endl;
    }

    for(int i = 0; i < 32*3; i += 2){
        std::cout << data[i] <<" "<<data[i+1]<<"\n";
    }
    
    std::cout << "-------------- VBO Content --------------\n\n";
}