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
void CheckVBO();

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
    glClearColor(SDL_GetTicks()%11355/11355., SDL_GetTicks()%3345/3345., SDL_GetTicks()%83153/83153., 1);

    glm::mat4 projection = glm::ortho(-2.f, 1.f, -2.f, 2.f);
    glm::mat4 modelview = glm::mat4(1.0);

    modelview = translate(modelview, glm::vec3(angleX, angleY, 0));
    

    glUseProgram(occlusionShader->getProgramID());

        glBindFramebuffer(GL_FRAMEBUFFER, occlusionBuffer);

            glBindBuffer(GL_ARRAY_BUFFER, vboID);

                glUniformMatrix4fv(glGetUniformLocation(occlusionShader->getProgramID(), "projection"), 1, GL_FALSE, &projection[0][0]);
                glUniformMatrix4fv(glGetUniformLocation(occlusionShader->getProgramID(), "modelview"), 1, GL_FALSE, &modelview[0][0]);

                glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            //draw scene with only walls

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(0);
    
    //occlusionTexture is now the occlusion map texture

    glUseProgram(drawShader->getProgramID());

        glBindBuffer(GL_ARRAY_BUFFER, vboID);
    
            glUniformMatrix4fv(glGetUniformLocation(occlusionShader->getProgramID(), "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(occlusionShader->getProgramID(), "modelview"), 1, GL_FALSE, &modelview[0][0]);

            glBindTexture(GL_TEXTURE_2D, occlusionTexture);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
            glBindTexture(GL_TEXTURE_2D, 0);
                    
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(0);




    glFlush();
    SDL_GL_SwapWindow(screen);
}

void SDLGLSetup(){
    SDL_Init(SDL_INIT_VIDEO);
    GLenum err;

    atexit(SDL_Quit);

    if(msaa){
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msaa);
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

    std::cout << "Errors setup\n";
    while((err = glGetError()) != GL_NO_ERROR){
        std::cout <<std::hex<<"0x"<< err << std::endl;
    }

    if(msaa)
        glEnable(GLUT_MULTISAMPLE);

    glEnable(GL_DEPTH_TEST);
    std::cout << "Errors depth\n";
    while((err = glGetError()) != GL_NO_ERROR){
        std::cout <<std::hex<<"0x"<< err << std::endl;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "Errors setup\n";
    while((err = glGetError()) != GL_NO_ERROR){
        std::cout <<std::hex<<"0x"<< err << std::endl;
    }
}

void prepareVBO(){
    GLenum err;

    std::cout << "Errors\n";
    while((err = glGetError()) != GL_NO_ERROR){
        std::cout <<std::hex<<"0x"<< err << std::endl;
    }

    glGenFramebuffers(1, &occlusionBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, occlusionBuffer);
    
    std::cout << "Errors\n";
    while((err = glGetError()) != GL_NO_ERROR){
        std::cout <<std::hex<<"0x"<< err << std::endl;
    }

        glGenTextures(1, &occlusionTexture);
        glBindTexture(GL_TEXTURE_2D, occlusionTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB,
            GL_UNSIGNED_BYTE, 0);
            
    std::cout << "Errors\n";
    while((err = glGetError()) != GL_NO_ERROR){
        std::cout <<std::hex<<"0x"<< err << std::endl;
    }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        std::cout << "Errors\n";
        while((err = glGetError()) != GL_NO_ERROR){
            std::cout <<std::hex<<"0x"<< err << std::endl;
        }
    
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, occlusionTexture, 0);
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers);

        std::cout << "Errors\n";
        while((err = glGetError()) != GL_NO_ERROR){
            std::cout <<std::hex<<"0x"<< err << std::endl;
        }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);

    std::cout << "Errors\n";
    while((err = glGetError()) != GL_NO_ERROR){
        std::cout <<std::hex<<"0x"<< err << std::endl;
    }

        glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), bufferInfos, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        
        std::cout << "Errors attrib 0\n";
        while((err = glGetError()) != GL_NO_ERROR){
            std::cout <<std::hex<<"0x"<< err << std::endl;
        }
    
        glEnableVertexAttribArray(0);

        std::cout << "Errors enable 0\n";
        while((err = glGetError()) != GL_NO_ERROR){
            std::cout <<std::hex<<"0x"<< err << std::endl;
        }
    
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(8));
        glEnableVertexAttribArray(1);


        std::cout << "Errors attrib 1\n";
        while((err = glGetError()) != GL_NO_ERROR){
            std::cout <<std::hex<<"0x"<< err << std::endl;
        }
    
    glBindBuffer(GL_ARRAY_BUFFER , 0);

    std::cout << "Errors\n";
    while((err = glGetError()) != GL_NO_ERROR){
        std::cout <<std::hex<<"0x"<< err << std::endl;
    }

    CheckVBO();
}

void CheckVBO(){
    int size = 16 * sizeof(float);
    float * data = (float*) calloc(size, sizeof(float));

    GLenum err;
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

    for(int i = 0; i < 16; i += 2){
        std::cout << data[i] <<" "<<data[i+1]<<"\n";
    }
    
    std::cout << "-------------- VBO Content --------------\n\n";
}