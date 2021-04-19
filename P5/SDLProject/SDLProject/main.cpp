/*
 Project 5: Platformer
 By Danny Alcedo for CS3113, Intro to Game Programming
 Code snippets taken from Professor Guida's lectures
 Platform and george graphics taken from the assets folder of the class.
 Edited enemy eye graphic from: https://www.pngjoy.com/preview/m5m1v9b9k0b6e5_terraria-icon-terraria-moon-lord-eye-sprite-transparent/
 
 The goal of this game is to complete all three levels. But beware, you might run into dangerous jumping A(EYE)s.
 You can kill them by jumping on them but if you touch them anywhere else but the top, you will lose a life.
 You have three lives. Killing the A(EYE) is not required for winning.
 Be careful of endless pits of doom!
 
 And yes - you CAN double jump!
 */

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

int playerLives = 3;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene *currentScene;
Scene *sceneList[5];

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

GLuint fontTextureID;

Mix_Music *music;
Mix_Chunk *bounce;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("The A(EYE) Strike Back!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    
    bounce = Mix_LoadWAV("bounce.wav");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
    //Initialize Text
    fontTextureID = Util::LoadTexture("font.png");
    
    sceneList[0] = new Level0();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    sceneList[4] = new Level4();
    SwitchToScene(sceneList[0]);
}

void ProcessInput() {
    if(currentScene != sceneList[0] && currentScene != sceneList[4]){
        currentScene->state.player->movement = glm::vec3(0);
    }
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_RETURN:
                        if(currentScene == sceneList[0]){
                            currentScene->state.player->startGame =true;
                        }
                        break;
                    case SDLK_SPACE:
                        // Some sort of action
                        if(currentScene != sceneList[0] && currentScene != sceneList[4]){
                            if (currentScene->state.player->collidePlat[1] || currentScene->state.player->doubleJump){
                                currentScene->state.player->jump = true;
                                currentScene->state.player->jumpNum += 1;
                                currentScene->state.player->doubleJump =true;
                                Mix_PlayChannel(-1, bounce, 0);
                            }
                            if(currentScene->state.player->jumpNum == 2){
                                currentScene->state.player->doubleJump = false;
                                currentScene->state.player->jumpNum = 0;
                            }
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if(currentScene != sceneList[0] && currentScene != sceneList[4]){
        if (keys[SDL_SCANCODE_LEFT]) {
            currentScene->state.player->movement.x = -1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animLeft;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            currentScene->state.player->movement.x = 1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animRight;
        }
        

        if (glm::length(currentScene->state.player->movement) > 1.0f) {
            currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
        }
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    deltaTime += accumulator;
   
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
   }
   
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);
        
        deltaTime -= FIXED_TIMESTEP;
    }
   
    accumulator = deltaTime;
    
    if(currentScene != sceneList[0] && currentScene != sceneList[4]){
        viewMatrix = glm::mat4(1.0f);
        if (currentScene->state.player->position.x > 5 && currentScene->state.player->position.x < 7.5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
        }
        else if(currentScene->state.player->position.x <= 5){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        }
        else if (currentScene->state.player->position.x >= 7){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-7.5, 3.75, 0));
        }
    }
    
    if(currentScene != sceneList[4] && currentScene->state.player->passLives){
        currentScene->state.player->passLives = false;
        playerLives = currentScene->state.player->lives;
    }
}
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
    
    if(currentScene != sceneList[4] && currentScene->state.player->failure){
        Util::DrawText(&program, fontTextureID, "Mission Failure", 1, -0.5f, glm::vec3(currentScene->state.player->position.x-3.25, currentScene->state.player->position.y+2, 0));
    }
    
    if(currentScene != sceneList[0] && currentScene != sceneList[4]){
        std::string livesText = "Number of Lives: " + std::to_string(playerLives);
        if (currentScene->state.player->position.x > 5 && currentScene->state.player->position.x < 7.5) {
            Util::DrawText(&program, fontTextureID, livesText, 1, -0.5f, glm::vec3(currentScene->state.player->position.x-4.25, -0.5, 0));
        }
        else if(currentScene->state.player->position.x <= 5){
            Util::DrawText(&program, fontTextureID, livesText, 1, -0.5f, glm::vec3(0.75, -0.5, 0));
        }
        else if (currentScene->state.player->position.x >= 7){
            Util::DrawText(&program, fontTextureID, livesText, 1, -0.5f, glm::vec3(3.25, -0.5, 0));
        }
    }
    
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
        
        if (currentScene->state.nextScene >= 0){
            SwitchToScene(sceneList[currentScene->state.nextScene]);
            if(currentScene != sceneList[4]){
                currentScene->state.player->lives = playerLives;
            }
        }
        
        Render();
    }
    
    Shutdown();
    return 0;
}
