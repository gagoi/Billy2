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



#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

int msaa = 8;
unsigned int lastTime, currentTime;
SDL_Window *screen;
SDL_GLContext glcontext;

double angleX = 0, angleY = 0, altitude = 0;

void draw(SDL_Window *);
void SDLGLSetup();
void prepareVBO();
void CheckVBO();

Shader * occlusionShader;
Shader * lightrayShader;
Shader * drawShader;

Player * player;

GLuint vboID; // the 3 rectangles in the scene

GLuint occlusionTexture;
GLuint occlusionFramebuffer;

GLuint lightrayTexture;
GLuint lightrayFramebuffer;
glm::vec2 shadowRes = glm::vec2((float)500, (float)500); // 0.5 degrees with 1/100 unit dist. resolution

float bufferInfos[32 * 3] =  {
                        702, 575,
                        702, 758,
                        965, 758,
                        965, 575,
                        1000, 800,
                        1200, 800,
                        1200, 660,
                        1000, 660,
                        400, 300,
                        500, 300,
                        500, 400,
                        400, 400,
                        
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
    while(!terminate){
        SDL_PollEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                terminate = true;
        }
        
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        
        float timePassed = 20./1000;

        if (state[SDL_SCANCODE_ESCAPE])
            terminate = true;;
        if (state[SDL_SCANCODE_W])
            angleY += 1000 * timePassed;
        if (state[SDL_SCANCODE_S])
            angleY -= 1000 * timePassed;
        if (state[SDL_SCANCODE_A])
            angleX -= 1000 * timePassed;
        if (state[SDL_SCANCODE_D])
            angleX += 1000 * timePassed;
        if (state[SDL_SCANCODE_SPACE])
            altitude += 1;
        if (state[SDL_SCANCODE_C])
            altitude -= 1;
        draw(screen);

        SDL_Delay(20);
    }


    return 0;
}



void draw(SDL_Window * screen){
    glClearColor(0,0,0,0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClearColor(SDL_GetTicks()%11355/11355., SDL_GetTicks()%33245/33245., SDL_GetTicks()%83153/83153., 1);
    glm::mat4 projection = glm::ortho(0., (double) WINDOW_WIDTH, 0., (double) WINDOW_HEIGHT);
    glm::mat4 modelview = glm::mat4(1.);

    modelview = translate(modelview, glm::vec3(angleX, angleY, 0.));
    
    //Generate occlusion map in occlusionTexture
    glUseProgram(occlusionShader->getProgramID());

        glBindFramebuffer(GL_FRAMEBUFFER, occlusionFramebuffer);

            
            glClearColor(0,0,0,0);
            glClear( GL_COLOR_BUFFER_BIT );

            glBindBuffer(GL_ARRAY_BUFFER, vboID);

                glUniformMatrix4fv(glGetUniformLocation(occlusionShader->getProgramID(), "projection"), 1, GL_FALSE, &projection[0][0]);
                glUniformMatrix4fv(glGetUniformLocation(occlusionShader->getProgramID(), "modelview"), 1, GL_FALSE, &modelview[0][0]);

                for( int i = 0; i < 4*3; i+= 4)
                    glDrawArrays(GL_TRIANGLE_FAN, i, 4);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            //draw scene with only walls

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(0);
    //occlusionTexture is now the occlusion map texture

    modelview = glm::mat4(1.0f);

    projection = glm::ortho(0.,(double) WINDOW_WIDTH, 0., (double) WINDOW_HEIGHT);
    //projection = glm::ortho(0.,1.,0.,1.);

    // OSEF projection, all is done in frag shader depending on occlusion texture and pixel width
    // but using same projection to have correct x values on occlusion map (which is WINDOW_WIDTH)
    // could do projection = glm::ortho(0, x, 0, 1) if drawing a (0, x) on x axis rectangle
    // optimisation would be x beeing shadowRes.x
    glUseProgram(lightrayShader->getProgramID());

        glBindFramebuffer(GL_FRAMEBUFFER, lightrayFramebuffer);

            glClearColor(0,0,0,0);
            glClear(GL_COLOR_BUFFER_BIT);

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

            GLenum err;
            while((err = glGetError()) != GL_NO_ERROR)
                std::cout << "Error drawing: " << err << std::endl;

                    
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(0);

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


    if(msaa)
        glEnable(GLUT_MULTISAMPLE);

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void prepareVBO(){
    //occlusion texture/framebuffer generation
    glGenFramebuffers(1, &occlusionFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, occlusionFramebuffer);
    
        glGenTextures(1, &occlusionTexture);
        glBindTexture(GL_TEXTURE_2D, occlusionTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB,
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, shadowRes.x, 900, 0, GL_RGB,
            GL_UNSIGNED_BYTE, 0);                                 
            
        // /!\ interpolated when drawed, need to test differents settings with poor resolution result: no effect :(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, lightrayTexture, 0);
        DrawBuffers[0] = GL_COLOR_ATTACHMENT0;
        glDrawBuffers(1, DrawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // test VBO for rendering 3 rects. generation
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);

        glBufferData(GL_ARRAY_BUFFER, 32 * 3 * sizeof(float), bufferInfos, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(16*3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER , 0);

    CheckVBO();
}

void CheckVBO(){
    int size = 32*3 * sizeof(float);
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

    for(int i = 0; i < 32*3; i += 2){
        std::cout << data[i] <<" "<<data[i+1]<<"\n";
    }
    
    std::cout << "-------------- VBO Content --------------\n\n";
}