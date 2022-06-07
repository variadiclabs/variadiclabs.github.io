#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "../glm/glm/gtc/matrix_transform.hpp"
#include "../glm/glm/gtc/type_ptr.hpp"
#include "../glm/glm/gtx/string_cast.hpp"
#include "../glm/glm/ext.hpp"
#include "shader.hpp"
#include "sfc.hpp"

#include <iostream>

#include <stdlib.h>
static GLboolean autoRotate=GL_TRUE;

#include <stdio.h>
#include <SDL/SDL.h>
SDL_Window *window;

#ifdef __EMSCRIPTEN__
#include<emscripten.h>
#endif

#define XLEN 1
#define YLEN 1
using namespace glm;

static float reqAngleX=0.0;
static float reqAngleY=0.0;
static float reqAngleZ=0.0;

static float currentAngleX=45.0;
static float currentAngleY=45.0;
static float currentAngleZ=45.0;

static float autodelay=1.0;

extern "C" {
  void setAngleX(const double ucx) {
    reqAngleX = ucx;
  }
  void setAngleY(const double ucy) {
    reqAngleY = ucy;
  }
  void setAngleZ(const double ucz) {
    reqAngleZ = ucz;
  }

  void setRotationSpeed(const float delay) {
    autodelay = delay;
  }

  void checkbox() {
    autoRotate = !autoRotate;
  }

  void resizeWindow(int width, int height) {
    glViewport(0, 0, width, height);
  }

  void handleKey(const char* keyStr) {
    if (strcmp(keyStr, "Escape") == 0) {
      SDL_Quit( );
    }
    if (strcmp(keyStr, " ") == 0) {
      autoRotate = !autoRotate;
    }
    if (strcmp(keyStr, "ArrowUp") == 0) {
      reqAngleZ++;
    }
    if (strcmp(keyStr, "ArrowDown") == 0) {
      reqAngleZ--;
    }
    if (strcmp(keyStr, "ArrowRight") == 0) {
      reqAngleY++;
    }
    if (strcmp(keyStr, "ArrowLeft") == 0) {
      reqAngleY--;
    }
  }
}

static const GLfloat vertexData[]={
  -1, -1, -1, 1,
  +1, -1, -1, 1,
  +1, +1, -1, 1,
  -1, +1, -1, 1,
  -1, -1, +1, 1,
  -1, +1, +1, 1,
  +1, +1, +1, 1,
  +1, -1, +1, 1,
};

static GLuint elements[]={
  0, 1, 2,
  2, 3, 0,
  0, 3, 4,
  4, 3, 5,
  5, 6, 4,
  6, 7, 4,
  4, 7, 0,
  0, 7, 1,
  1, 7, 6,
  6, 2, 1,
  3, 2, 6,
  6, 5, 3,
};

static const GLfloat colorData[]={
  1, 0, 0, 1,
  0, 1, 0, 1,
  0, 0, 1, 1,
  0, 1, 1, 1,
  1, 1, 0, 1,
  1, 0, 1, 1,
  1, 0, 0, 1,
  0, 1, 1, 1,
};

GLuint elementsBuf;
GLuint vertexBuf;
GLuint colorBuffer;
GLuint program;
int width=XLEN;
int height=YLEN;
glm::mat4 viewPortMat;
glm::mat4 Projection;
glm::mat4 View;
mat4 identityMat;
mat4 cameraRotMat;
vec3 cameraPos;
GLuint viewPortLoc;
GLuint cameraRotLoc;

void initScene() {
  //FOV, aspect ratio, znear, zfar
  Projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 1.0f, 1024.0f);
  //ortho camera:
  //glm::mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
  identityMat = mat4(1.0f);
  cameraRotMat = identityMat;
  //cameraPos = vec3(4, 3, 3); 
  cameraPos = vec3(4, 4, 0); 
   
  // Camera matrix
  View = glm::lookAt(
    cameraPos,				//camera location
    glm::vec3(0, 0, 0), //location of origin
    glm::vec3(0, 0, 1)  //up vector
  );
    
  viewPortMat = Projection * View;
}
void updateScene() {

  if(reqAngleX != currentAngleX) {
    cameraRotMat = rotate(cameraRotMat, radians(reqAngleX - currentAngleX), vec3(0, 1, 0));
    currentAngleX = reqAngleX;
  }
  if(reqAngleY != currentAngleY) {
    cameraRotMat = rotate(cameraRotMat, radians(reqAngleY - currentAngleY), vec3(0, 1, 0));
    currentAngleY = reqAngleY;
  }
  if(reqAngleZ != currentAngleZ) {
    cameraRotMat = rotate(cameraRotMat, radians(reqAngleZ - currentAngleZ), vec3(0, 0, 1));
    currentAngleZ = reqAngleZ;
  }
  if(autoRotate) {
    cameraRotMat = rotate(cameraRotMat, radians(autodelay), vec3(1, 0, 0));

  }
  glUniformMatrix4fv(cameraRotLoc, 1, GL_FALSE, value_ptr(cameraRotMat));

  glUniformMatrix4fv(viewPortLoc, 1, GL_FALSE, value_ptr(viewPortMat));
}

void initGL() {
  program = glCreateProgram();

  glGenBuffers(1, &elementsBuf);
  glGenBuffers(1, &vertexBuf);
  glGenBuffers(1, &colorBuffer);

  initScene();
  loadShaders(program);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBuf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  glEnableVertexAttribArray(glGetAttribLocation(program, "vertexBuf"));

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  glVertexAttribPointer(
    glGetAttribLocation(program, "vertexBuf"),
    4,						//coordinates per vertex
    GL_FLOAT,			//type
    GL_FALSE,			//normalized?
    0,						//stride
    0							//buffer offset
  );

  glEnableVertexAttribArray(glGetAttribLocation(program, "colorBuffer"));

  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
  glVertexAttribPointer(
    glGetAttribLocation(program,"colorBuffer"),
    4,
    GL_FLOAT,
    GL_FALSE,
    0,
    0
  );

  viewPortLoc = glGetUniformLocation(program, "viewPort");
  cameraRotLoc = glGetUniformLocation(program, "cameraRot");
//	glDisableVertexAttribArray(0);
}

void drawScreen() {

  // glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(program);
  updateScene();
  //glDrawArrays(GL_TRIANGLES, 0, 3);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  //SDL_GL_SwapWindow(window);
  SDL_GL_SwapBuffers();
}

static void handle_key_down(SDL_keysym* keysym) {
  // do nothing, replaced above
}

static void processEvents() {
    SDL_Event event;

    /* Grab all the events off the queue. */
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_KEYDOWN:
            /* Handle key presses. */
            handle_key_down(&event.key.keysym);
            break;
        case SDL_QUIT:
            /* Handle quit requests (like Ctrl-c). */
            SDL_Quit();
            break;
        }
    }
}

void main_loop() {
  processEvents();
  drawScreen();
}

static void setup_opengl(int width, int height) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glClearColor(((float)(rand() % 256)) / 255.0f, ((float)(rand() % 256)) / 255.0f, ((float)(rand() % 256)) / 255.0f, 0);
    glViewport(0, 0, width, height);
}

static void setup_sdl_window(int width,int height) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    auto err = SDL_GetError();
    EM_ASM_({
      console.log('Video initialization failed: ' + $0);
    });
  }

  //SDL2 initialization.  requireds -s USE_SDL=2 in makefile.
  window = SDL_CreateWindow("OpenGL",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    XLEN, YLEN,
    SDL_WINDOW_OPENGL);
  //	SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  SDL_GL_CreateContext(window);

  const SDL_VideoInfo* info;
  int bpp = 0;	/* Color depth in bits of our window. */
  int flags = 0;
  info = SDL_GetVideoInfo( );

  if (!info) {
    auto err = SDL_GetError();
    EM_ASM_({
      console.log('Video query failed: ' + $0);
    });
  }
  bpp = info->vfmt->BitsPerPixel;

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  //flags = SDL_OPENGL | SDL_FULLSCREEN;
  flags = SDL_OPENGL;
  if (SDL_SetVideoMode(width, height, bpp, flags) == 0) {
    // DISPLAY not set, the specified resolution not available?
    auto err = SDL_GetError();
    EM_ASM_({
      console.log('Video mode set failed: ' + $0);
    }, err);
  }
  /*
  GLEW init.  only required for GLEW extensions.
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
      printf("Error: glewInit: %s\n",glewGetErrorString(glew_status));
    }
  */
}
 
int main() {
  EM_ASM({
    specialHTMLTargets["!canvas"] = Module.canvas;
  });

  int width;
  int height;

  width = atoi(std::getenv("WIDTH"));
  height = atoi(std::getenv("HEIGHT"));
  char* charHash = std::getenv("TOKEN_HASH");

  std::string tokenHash = charHash;


  EM_ASM_({
    console.log('[Demo passing string as a pointer] Passed hash ' + Module.UTF8ToString($0) + ' from external');
  }, tokenHash.c_str());
  printf("Passed hash %s from external\n", tokenHash.c_str());
  

  // super simple example of generating a numeric seed for srand from a token hash
  int i;
  int num = 0;
  for (i = 0; i < 66; i += 2) {
    num += (tokenHash[i] - '0') * 16 + (tokenHash[i + 1] - '0');
  }
  EM_ASM_({
    console.log('num: ' + $0);
  }, num);
  // printf("num: %d\n", num);

  srand(num);

  // printf("Some random outputs from this seed: %d %d %d\n", rand(), rand(), rand());
  EM_ASM_({
    console.log('Some random outputs from this seed: ' + $0 + ' ' + $1 + ' ' + $2);
  }, rand(), rand(), rand());

  setup_sdl_window(width,height);
  setup_opengl(width,height);
  initGL();
  emscripten_set_main_loop(main_loop,0,0);
  EM_ASM({
    console.log('Rotating 3D cube written in C++ using SDL and OpenGL.  Converted to WebASM through Emscripten.\n');
    console.log('Press spacebar to pause rotation.');
    console.log('Press up/down to rotate up/down.');
    console.log('Press left/right to rotate left/right.');
  });
  // printf("Rotating 3D cube written in C++ using SDL and OpenGL.  Converted to WebASM through Emscripten.\n");
  // printf("Press spacebar to pause rotation.\n");
  // printf("Press up/down to rotate up/down.\n");
  // printf("Press left/right to rotate left/right.\n");
  return 0;
}
