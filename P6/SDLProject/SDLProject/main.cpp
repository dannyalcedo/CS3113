/*
 Project: Student's Choice
 ROOMS
 By Danny Alcedo
 
 ROOMS is a minimalistic maze game where the player must traverse a set of rooms looking for keys
 that will open portals to subsequent rooms.
 Not only must the player find the keys and the resulting spawn-point of the portal, but also be
 weary of the red circles of death which deminish the player's lives if they touch one. The player can see
 how many lives they have by how many circles are in their sprite.
 
 All art created by Danny on Canva
 ambience.wav by Danny Alcedo
 key.wav from Nox_Sound on freesound.org
 teleport.wav from Breviceps on freesound.org
 font.png from the dwarffortresswiki.org tileset repository posted by Bisasam
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
#include "Menu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "Winner.h"
#include "Loser.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

int playerLives = 3;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene *currentScene;
Scene *sceneList[8];

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

GLuint fontTextureID;

Mix_Music *music;
Mix_Chunk *key;
Mix_Chunk *teleport;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("ROOMS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("ambiance.wav");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    
    key = Mix_LoadWAV("key.wav");
    teleport = Mix_LoadWAV("teleport.wav");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
    //Initialize Text
    fontTextureID = Util::LoadTexture("font.png");
    
    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    sceneList[4] = new Level4();
    sceneList[5] = new Level5();
    sceneList[6] = new Winner();
    sceneList[7] = new Loser();
    SwitchToScene(sceneList[0]);
}

void ProcessInput() {
    if(currentScene != sceneList[7] && currentScene != sceneList[6]){
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
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if(currentScene != sceneList[6] && currentScene != sceneList[7]){
        if (keys[SDL_SCANCODE_LEFT]) {
            currentScene->state.player->movement.x = -1.0f;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            currentScene->state.player->movement.x = 1.0f;
        }
        if (keys[SDL_SCANCODE_UP]) {
            currentScene->state.player->movement.y = 1.0f;
        }
        else if (keys[SDL_SCANCODE_DOWN]) {
            currentScene->state.player->movement.y = -1.0f;
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
    
    if(currentScene != sceneList[6] && currentScene != sceneList[7] && currentScene != sceneList[0]){
        viewMatrix = glm::mat4(1.0f);
        if (currentScene->state.player->position.x > 4.5 && currentScene->state.player->position.x < 10.5 && currentScene->state.player->position.y < -3.5 && currentScene->state.player->position.y > -11.5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, -currentScene->state.player->position.y, 0));
        }
        
        if(currentScene->state.player->position.x <= 4.5 && currentScene->state.player->position.y <= -11.5){ //bottom left
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-4.5, 11.5, 0));
        }
        else if(currentScene->state.player->position.x <= 4.5 && currentScene->state.player->position.y >= -3.5){ //top left
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-4.5, 3.5, 0));
        }
        else if(currentScene->state.player->position.x >= 10.5 && currentScene->state.player->position.y <= -11.5){ //bottom right
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-10.5, 11.5, 0));
        }
        else if(currentScene->state.player->position.x >= 10.5 && currentScene->state.player->position.y >= -3.5){ //top right
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-10.5, 3.5, 0));
        }
        else if(currentScene->state.player->position.x <= 4.5){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-4.5, -currentScene->state.player->position.y, 0));
        }
        else if(currentScene->state.player->position.x >= 10.5){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-10.5, -currentScene->state.player->position.y, 0));
        }
        else if(currentScene->state.player->position.y <= -11.5){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 11.5, 0));
        }
        else if(currentScene->state.player->position.y >= -3.5){
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.5, 0));
        }
        
        if(currentScene->state.player->passLives){
            currentScene->state.player->passLives = false;
            playerLives = currentScene->state.player->lives;
        }
        
        if(currentScene->state.player->touchedKey){
            currentScene->state.player->touchedKey = false;
            Mix_PlayChannel(-1, key, 0);
        }
        if(currentScene->state.player->touchedPortal){
            currentScene->state.player->touchedPortal = false;
            Mix_PlayChannel(-1, teleport, 0);
        }
    }
    else{
        viewMatrix = glm::mat4(1.0f);
    }
    
}
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
    
    if(currentScene != sceneList[6] && currentScene != sceneList[7]){
        if(currentScene->state.player->failure){
            SwitchToScene(sceneList[7]);
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
            if(currentScene != sceneList[6] && currentScene != sceneList[7]){
                currentScene->state.player->lives = playerLives;
            }
        }
        
        Render();
    }
    
    Shutdown();
    return 0;
}
