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
glm::mat4 viewMatrix, leftMatrix, rightMatrix, ballMatrix, projectionMatrix;

//position and 'velocity' vectors
glm::vec3 left_position = glm::vec3(-4.5, 0, 0);
glm::vec3 left_movement = glm::vec3(0, 0, 0);
glm::vec3 right_position = glm::vec3(4.5, 0, 0);
glm::vec3 right_movement = glm::vec3(0, 0, 0);
glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement = glm::vec3(0, 0, 0);

float left_width = 1.0;
float right_width = 1.0;
float ball_width = 0.4;
float left_height = 2.0;
float right_height = 2.0;
float ball_height = 0.4;

float player_speed = 3.0f;
float ball_speed = 3.0f;

bool game_end = true;
float tick_remove = 0;
float lastTicks = 0.0f;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Ping-Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    
    leftMatrix = glm::mat4(1.0f);
    rightMatrix = glm::mat4(1.0f);
    leftMatrix = glm::translate(leftMatrix, glm::vec3(-4.5f, 0.0f, 0.0f));
    rightMatrix = glm::translate(rightMatrix, glm::vec3(4.5f, 0.0f, 0.0f));
    
    ballMatrix = glm::mat4(1.0f);
    ballMatrix = glm::translate(ballMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ProcessInput() {
    
    left_movement = glm::vec3(0, 0, 0);
    right_movement = glm::vec3(0, 0, 0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type){
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
                        game_end = false;
                        tick_remove = (float)SDL_GetTicks() / 1000.0f;
                        ball_movement = glm::vec3(0.7, 0.3, 0);
                        lastTicks = 0.0f;
                        std::cout<<tick_remove;
                        std::cout<<"game started \n";
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_W]) {
        left_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        left_movement.y = -1.0f;
    }
    
    if (keys[SDL_SCANCODE_UP]) {
        right_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        right_movement.y = -1.0f;
    }
}

void Update() {
    
    if(!game_end){
        
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float deltaTime = ticks - tick_remove - lastTicks;
        lastTicks = ticks - tick_remove;
        
        float xLeftBallDist = fabs(left_position.x-ball_position.x) - ((left_width+ball_width)/2.0f);
        float xRightBallDist = fabs(ball_position.x-right_position.x) - ((right_width+ball_width)/2.0f);
        float yLeftBallDist = fabs(left_position.y-ball_position.y) - ((left_height+ball_height)/2.0f);
        float yRightBallDist = fabs(ball_position.y-right_position.y) - ((right_height+ball_height)/2.0f);
        
        if((left_position.y >= 2.78) && ((left_movement * player_speed * deltaTime).y > 0)){
            // can't move
        }
        else if((left_position.y <= -2.78) && ((left_movement * player_speed * deltaTime).y < 0)){
            // can't move
        }
        else{
            leftMatrix = glm::mat4(1.0f);
            left_position += left_movement * player_speed * deltaTime;
            leftMatrix = glm::translate(leftMatrix, left_position);
        }
        
        if((right_position.y >= 2.78) && ((right_movement * player_speed * deltaTime).y > 0)){
            // can't move
        }
        else if((right_position.y <= -2.78) && ((right_movement * player_speed * deltaTime).y < 0)){
            // can't move
        }
        else{
            rightMatrix = glm::mat4(1.0f);
            right_position += right_movement * player_speed * deltaTime;
            rightMatrix = glm::translate(rightMatrix, right_position);
        }
        
//        std::cout << "ballposX: " << ball_position.x << std::endl;
//        std::cout << "ballposY: " <<ball_position.y << std::endl;
//
//        std::cout << "Rx: " << xRightBallDist << std::endl;
//        std::cout << "Ry: " << yRightBallDist << std::endl;
//        std::cout << "Lx: " << xLeftBallDist << std::endl;
//        std::cout << "Ly: " << yLeftBallDist << std::endl;
        
        if(xLeftBallDist < 0 && yLeftBallDist < 0){
            //std::cout<<"collision with left player flip to right \n";
            ball_movement.x = 0.7;//ball_movement.x * -1;
        }
        else if(xRightBallDist < 0 && yRightBallDist < 0){
            //std::cout<<"collision with right player flip to left \n";
            ball_movement.x = -0.7;//ball_movement.x * -1;
        }
        else if(ball_position.y <= -3.55){
            //std::cout<<"collision with bottom flip to up \n";
            ball_movement.y = 0.3;//ball_movement.y * -1;
        }
        else if(ball_position.y >= 3.55){
            //std::cout<<"collision with top flip to down \n";
            ball_movement.y = -0.3;//ball_movement.y * -1;
        }
        else if ((ball_position.x <= -4.8) || (ball_position.x >= 4.8)){
            std::cout<<"game_end";
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            ball_movement = glm::vec3(0, 0, 0);
            game_end = true;
        }
        
        ballMatrix = glm::mat4(1.0f);
        ball_position += ball_movement * ball_speed * deltaTime;
        ballMatrix = glm::translate(ballMatrix, ball_position);
    }
}
 

void Render() {
    float playerVertices[] = { -0.5, -1.0, 0.5, -1.0, 0.5, 1.0, -0.5, -1.0, 0.5, 1.0, -0.5, 1.0};
    float ballVertices[] = { -0.20, -0.20, 0.20, -0.20, 0.20, 0.20, -0.20, -0.20, 0.20, 0.20, -0.20, 0.20};
    glClear(GL_COLOR_BUFFER_BIT);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, playerVertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    program.SetModelMatrix(leftMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    program.SetModelMatrix(rightMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ballVertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    program.SetModelMatrix(ballMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
        
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
