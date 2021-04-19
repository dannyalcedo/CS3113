#pragma once
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
#include<vector>
#include<variant>
#include "Map.h"

enum EntityType {PLAYER, PLATFORM, ENEMY};

enum AIType {WAITANDGO, JUMPER, WALKER};
enum AIState {IDLE, WALKING, ATTACKING};

class Entity {
public:
    
    bool startGame = false;
    bool restartLevel = false;
    bool failure = false;
    
    bool passLives = false;
    int lives;
    
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    int flip = 1;
    
    float width = 1;
    float height = 1;
    
    bool jump = false;
    float jumpPower = 0;
    float doubleJump = false;
    int jumpNum = 0;
    
    float speed;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUp = NULL;
    int *animDown = NULL;

    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    
    bool isActive = true;
    
    std::vector <bool> collidePlat {false, false, false, false};
    std::vector <bool> collideEnem {false, false, false, false};
        
    Entity();
    
    bool CheckCollision(Entity *other);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsX(Map *map);
    void CheckCollisionsY(Map *map);
    
    void Update(float deltaTime, Entity *player, Entity *enemies, int enemyCount, Map *map);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    
    void AI(Entity *player);
    void AIWalker();
    void AIWaitAndGo(Entity *player);
    void AIJumper();
};
