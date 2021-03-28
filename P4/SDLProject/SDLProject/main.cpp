/*
 Project 4: Rise of the AI
 By Danny Alcedo for CS3113, Intro to Game Programming
 Code snippets taken from Professor Guida's lectures
 Platform and george graphics taken from the assets folder of the class.
 Edited enemy eye graphic from: https://www.pngjoy.com/preview/m5m1v9b9k0b6e5_terraria-icon-terraria-moon-lord-eye-sprite-transparent/
 
 The goal of this game is to defeat all of the A(EYE)s one at a time. To do so, you must jump on them.
 If you touch them anywhere else but the top, you will lose. You win when you've defeated them all.
 
 The A(EYE) on the bottom platform is a Wait and Go enemy. They are idle until you get close enough to them.
 Then they will follow you until you defeat it.
 
 The A(EYE) on the middle platform is a Jumper enemy. All they do is jump in place.
 
 The A(EYE) in the top platform is a patroller enemy. Al they do is move back and forth on the platform until you defeat it.
 
 And yes - you CAN double jump!
 */

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

#include "Entity.h"

#define PLATFORM_COUNT 24
#define ENEMY_COUNT 3

struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *enemies;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint fontTextureID;

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
    displayWindow = SDL_CreateWindow("Rise of the A(EYE)!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Initialize Game Objects
    
    //Initialize Text
    fontTextureID = LoadTexture("font.png");
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-4, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = LoadTexture("george.png");
    
    state.player->animRight = new int[4] {3, 7, 11, 15};
    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    state.player->height = 0.8f;
    state.player->width = 0.65f;
    
    state.player->jumpPower = 4.5f;
    
    // Initialize Platforms
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID = LoadTexture("platform.png");
    
    for(int i=0; i < 12; i++){
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(-5+i,-3.5f, 0);
    }
    
    for(int i=0; i < 6; i++){
        state.platforms[12+i].entityType = PLATFORM;
        state.platforms[12+i].textureID = platformTextureID;
        state.platforms[12+i].position = glm::vec3(5-i,-1.0f, 0);
    }
    
    for(int i=0; i < 6; i++){
        state.platforms[18+i].entityType = PLATFORM;
        state.platforms[18+i].textureID = platformTextureID;
        state.platforms[18+i].position = glm::vec3(-5+i,1.5f, 0);
    }
    
    for (int i = 0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Update(0, NULL, NULL, NULL, 0, 0);
    }
    
    // Initialize Enemies
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemytextureID = LoadTexture("eye.png");
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemytextureID;
    state.enemies[0].position = glm::vec3(4, -2.5, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemytextureID;
    state.enemies[1].position = glm::vec3(3, 0, 0);
    state.enemies[1].speed = 1;
    state.enemies[1].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies[1].jumpPower = 4.0f;
    state.enemies[1].aiType = JUMPER;
    state.enemies[1].aiState = IDLE;
    
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = enemytextureID;
    state.enemies[2].position = glm::vec3(-3, 2.5, 0);
    state.enemies[2].speed = 1;
    state.enemies[2].aiType = WALKER;
    state.enemies[2].aiState = IDLE;
}

void ProcessInput() {    
    state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                        
                    case SDLK_SPACE:
                        // Some sort of action
                        if (state.player->collidePlat[1] || state.player->doubleJump){
                            state.player->jump = true;
                            state.player->jumpNum += 1;
                            state.player->doubleJump =true;
                        }
                        if(state.player->jumpNum == 2){
                            state.player->doubleJump = false;
                            state.player->jumpNum = 0;
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
    }
    

    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
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
        state.player->Update(FIXED_TIMESTEP, state.player, state.platforms, state.enemies, PLATFORM_COUNT, ENEMY_COUNT);
        
        for (int i = 0; i < ENEMY_COUNT; i++){
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, state.enemies, PLATFORM_COUNT, ENEMY_COUNT);
        }
        
        deltaTime -= FIXED_TIMESTEP;
    }
   
    accumulator = deltaTime;
}

void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text,
                                 float size, float spacing, glm::vec3 position){
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;
    
    std::vector<float> vertices;
    std::vector<float> texCoords;
    
    for(int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        vertices.insert(vertices.end(), {
                offset + (-0.5f * size), 0.5f * size,
                offset + (-0.5f * size), -0.5f * size,
                offset + (0.5f * size), 0.5f * size,
                offset + (0.5f * size), -0.5f * size,
                offset + (0.5f * size), 0.5f * size,
                offset + (-0.5f * size), -0.5f * size,
        });
        
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
        });
    }
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);
    
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for(int i = 0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Render(&program);
    }
    
    for(int i = 0; i < ENEMY_COUNT; i++){
        state.enemies[i].Render(&program);
    }
    
    state.player->Render(&program);
    
    if(state.player->failure){
        DrawText(&program, fontTextureID, "Mission Failure", 1, -0.5f, glm::vec3(-3.6, 0.25, 0));
    }
    else if(state.player->success){
        DrawText(&program, fontTextureID, "Mission Successful", 1, -0.5f, glm::vec3(-4.2, 0.25, 0));
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
        Render();
    }
    
    Shutdown();
    return 0;
}
