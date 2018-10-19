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
#include <windows.h>
#include <stdio.h>
#include "Shader.h"
#include "Player.h"
#include "Map.h"
#include "mapLoader.h"

#ifndef BUFFER_OFFSET
    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))
#endif

typedef BOOL (WINAPI *EDDType)(LPCSTR,DWORD,PDISPLAY_DEVICEA,DWORD);

template<class T>
const T& constrain(const T& x, const T& a, const T& b) {
    if(x < a) {
        return a;
    }
    else if(b < x) {
        return b;
    }
    else
        return x;
}