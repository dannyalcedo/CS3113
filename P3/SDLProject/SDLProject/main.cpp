/*
 Project #3 for CS3113 - Intro to Game Programming
 By: Danny Alcedo with code snippets and templates by Professor Carmine Guida
 All visuals taken from the Class Repo at https://github.com/carmineguida/CS3113
 
 This is a minimal version of the classic computer game, Lunar Lander.
 The goal is to land the ship on the green platform without hitting any of the gray walls.
 The lander must be within the bounds of the green platofrm to succeed, even the slightest touch to a wall will result in mission failure.
 Note that moving left and right changes the acceleration of the ship, not the velocity, so be wary of overshooting your movement.
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
#include <vector>

#define PLATFORM_COUNT 2
#define WALL_COUNT 27

struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *walls;
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
    displayWindow = SDL_CreateWindow("Lunar Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
   
    // Initialize Game Objects
    // Initialize Text
    fontTextureID = LoadTexture("font.png");
    
    // Initialize Player
    state.player = new Entity();
    state.player->position = glm::vec3(-2.0f, 3.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -0.1f, 0);
    state.player->speed = 0.0f;
    state.player->textureID = LoadTexture("lander.png");
    
    // Initialize Platform
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID = LoadTexture("platform.png");
    
    state.platforms[0].textureID = platformTextureID;
    state.platforms[0].position = glm::vec3(-2,-3.5f, 0);
    state.platforms[1].textureID = platformTextureID;
    state.platforms[1].position = glm::vec3(-1,-3.5f, 0);
    
    // Initialize Walls
    state.walls = new Entity[WALL_COUNT];
    GLuint wallTextureID = LoadTexture("wall.png");
    state.walls[0].textureID = wallTextureID;
    state.walls[0].position = glm::vec3(-5,-3.5f, 0);
    state.walls[1].textureID = wallTextureID;
    state.walls[1].position = glm::vec3(-4,-3.5f, 0);
    state.walls[2].textureID = wallTextureID;
    state.walls[2].position = glm::vec3(-3,-3.5f, 0);
    state.walls[3].textureID = wallTextureID;
    state.walls[3].position = glm::vec3(0,-3.5f, 0);
    state.walls[4].textureID = wallTextureID;
    state.walls[4].position = glm::vec3(1,-3.5f, 0);
    state.walls[5].textureID = wallTextureID;
    state.walls[5].position = glm::vec3(2,-3.5f, 0);
    state.walls[6].textureID = wallTextureID;
    state.walls[6].position = glm::vec3(3,-3.5f, 0);
    state.walls[7].textureID = wallTextureID;
    state.walls[7].position = glm::vec3(4,-3.5f, 0);
    state.walls[8].textureID = wallTextureID;
    state.walls[8].position = glm::vec3(5,-3.5f, 0);
    
    state.walls[9].textureID = wallTextureID;
    state.walls[9].position = glm::vec3(-5.0f,-2.5f, 0);
    state.walls[10].textureID = wallTextureID;
    state.walls[10].position = glm::vec3(-5.0f,-1.5f, 0);
    state.walls[11].textureID = wallTextureID;
    state.walls[11].position = glm::vec3(-5.0f,-0.5f, 0);
    state.walls[12].textureID = wallTextureID;
    state.walls[12].position = glm::vec3(-5.0f,0.5f, 0);
    state.walls[13].textureID = wallTextureID;
    state.walls[13].position = glm::vec3(-5.0f,1.5f, 0);
    state.walls[14].textureID = wallTextureID;
    state.walls[14].position = glm::vec3(-5.0f,2.5f, 0);
    state.walls[15].textureID = wallTextureID;
    state.walls[15].position = glm::vec3(-5.0f,3.5f, 0);
    
    state.walls[16].textureID = wallTextureID;
    state.walls[16].position = glm::vec3(5.0f,-2.5f, 0);
    state.walls[17].textureID = wallTextureID;
    state.walls[17].position = glm::vec3(5.0f,-1.5f, 0);
    state.walls[18].textureID = wallTextureID;
    state.walls[18].position = glm::vec3(5.0f,-0.5f, 0);
    state.walls[19].textureID = wallTextureID;
    state.walls[19].position = glm::vec3(5.0f,0.5f, 0);
    state.walls[20].textureID = wallTextureID;
    state.walls[20].position = glm::vec3(5.0f,1.5f, 0);
    state.walls[21].textureID = wallTextureID;
    state.walls[21].position = glm::vec3(5.0f,2.5f, 0);
    state.walls[22].textureID = wallTextureID;
    state.walls[22].position = glm::vec3(5.0f,3.5f, 0);
    
    state.walls[23].textureID = wallTextureID;
    state.walls[23].position = glm::vec3(-4.0f,0.5f, 0);
    state.walls[24].textureID = wallTextureID;
    state.walls[24].position = glm::vec3(-3.0f,0.5f, 0);
    state.walls[25].textureID = wallTextureID;
    state.walls[25].position = glm::vec3(-2.0f,0.5f, 0);
    state.walls[26].textureID = wallTextureID;
    state.walls[26].position = glm::vec3(-1.0f,0.5f, 0);

    for (int i = 0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Update(0, NULL, NULL, 0, 0);
    }
    
    for (int i = 0; i < WALL_COUNT; i++){
        state.walls[i].Update(0, NULL, NULL, 0, 0);
    }
    
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
                
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        state.player->acceleration.x = 0;
                        break;
                        
                    case SDLK_RIGHT:
                        state.player->acceleration.x = 0;
                        break;
                }
                break;
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->acceleration.x += -0.1;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->acceleration.x += 0.1;
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
        state.player->Update(FIXED_TIMESTEP, state.platforms, state.walls, PLATFORM_COUNT, WALL_COUNT);
        
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

    for (int i = 0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Render(&program);
    }
    for (int i = 0; i < WALL_COUNT; i++){
        state.walls[i].Render(&program);
    }
    
    state.player->Render(&program);
    
    if(state.player->failure){
        DrawText(&program, fontTextureID, "Mission Failure", 1, -0.5f, glm::vec3(-3.6, 0.5, 0));
    }
    else if(state.player->success){
        DrawText(&program, fontTextureID, "Mission Successful", 1, -0.5f, glm::vec3(-4.2, 0.5, 0));
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
