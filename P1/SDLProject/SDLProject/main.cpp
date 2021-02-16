// Project 1 for Intro to Game Programming CS3113
// By Danny Alcedo with code templates created by Professor Carmine Guida

// zen.png sourced from the Overwatch Wiki at https://overwatch.gamepedia.com/File:Spray_Zenyatta_Adorable.png
// thanks to wiki editor TheModster-GPUSER

// discord.png sourced from devianart.com at https://www.deviantart.com/cfowler7-sfm/art/Overwatch-Discord-Orb-645417804
// thanks to the artist cfowler7-SFM

// harmony.png is the same image as discord.png but with edited values to make it more yellow/white

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, playerMatrix, harmonyMatrix, discordMatrix, projectionMatrix;

float player_y = 0;
float harmony_rotate = 0;
float discord_rotate = 0;
bool flip = true;

GLuint playerTextureID;
GLuint harmonyTextureID;
GLuint discordTextureID;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project One!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    playerMatrix = glm::mat4(1.0f);
    harmonyMatrix = glm::mat4(1.0f);
    discordMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    playerTextureID = LoadTexture("zen.png");
    harmonyTextureID = LoadTexture("harmony.png");
    discordTextureID = LoadTexture("discord.png");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

float lastTicks = 0.0f;

void Update() {
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    if(player_y > 0.3 & player_y < 2){
        flip = false;
    }
    if(player_y > -2 & player_y < -0.3){
        flip = true;
    }
    
    if(flip){
        player_y += 1.0f * deltaTime;
    }
    else{
        player_y -= 1.0f * deltaTime;
    }
    
    harmony_rotate += 90.0f * deltaTime;
    discord_rotate -= 90.0f * deltaTime;
    
    playerMatrix = glm::mat4(1.0f);
    harmonyMatrix = glm::mat4(1.0f);
    discordMatrix = glm::mat4(1.0f);
    
    playerMatrix = glm::scale(playerMatrix, glm::vec3(3.0f, 3.0f, 1.0f));
    playerMatrix = glm::translate(playerMatrix, glm::vec3(0.0f, player_y, 0.0f));
    
    harmonyMatrix = glm::scale(harmonyMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
    harmonyMatrix = glm::translate(harmonyMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
    harmonyMatrix = glm::rotate(harmonyMatrix, glm::radians(harmony_rotate),glm::vec3(0.0f, 0.0f, 1.0f));
    
    discordMatrix = glm::scale(discordMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
    discordMatrix = glm::translate(discordMatrix, glm::vec3(-1.0f, 0.0f, 0.0f));
    discordMatrix = glm::rotate(discordMatrix, glm::radians(discord_rotate),glm::vec3(0.0f, 0.0f, 1.0f));
}

void Render() {
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    program.SetModelMatrix(playerMatrix);
    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    program.SetModelMatrix(harmonyMatrix);
    glBindTexture(GL_TEXTURE_2D, harmonyTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    program.SetModelMatrix(discordMatrix);
    glBindTexture(GL_TEXTURE_2D, discordTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
