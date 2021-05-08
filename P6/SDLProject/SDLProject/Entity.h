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

enum EntityType {PLAYER, ENEMY, DOOR, KEY, PORTAL};

enum AIType {PATROLLER};

class Entity{
public:
    bool startGame = false; //used for opening scene
    bool restartLevel = false; //used when player loses life
    bool failure = false; //used when player loses all three lives
    
    bool passLives = false; //used to pass # of lives between scenes
    int lives;
    
    EntityType entityType;
    AIType aiType;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 velocity;
    
    //used for collision checking
    float width = 1;
    float height = 1;
    
    float speed;
    
    GLuint textureID;
    glm::mat4 modelMatrix;
    
    bool isActive = true; //used to update and render an entity
    float start; //for patroller
    float end; //for patroller
    bool axis; //for patroller
    int flip = 1;//used to flip patroller direction
    
    std::vector <bool> collideEnem {false, false, false, false};
    
    bool touchedKey = false;
    bool portalOpen = false;
    bool touchedPortal = false;
    
    Entity();
    
    bool CheckCollision(Entity *other);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsX(Map *map);
    void CheckCollisionsY(Map *map);
    
    void Update(float deltaTime, Entity *player, Entity *enemies, int enemyCount, Map *map, Entity *key, Entity *portal);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    
    // everything here and below is bound to change
    void AI(Entity *player, float deltaTime);
    void AIPatroller(float start, float end, bool axis);
    void AIZombie(Entity *player);
    void AIDoor(float deltaTime);
    
};
