#include "Level5.h"
#include "Util.h"
#define LEVEL5_WIDTH 16
#define LEVEL5_HEIGHT 16
#define LEVEL5_ENEMY_COUNT 4
unsigned int level5_data[] =
{
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3,
    3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3,
    3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3,
    3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

void Level5::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL5_WIDTH, LEVEL5_HEIGHT, level5_data, mapTextureID, 1.0f, 4, 1);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->lives = 3;
    state.player->position = glm::vec3(1.5f, -13.5f, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 2.0f;
    if(state.player->lives == 3){
        state.player->textureID = Util::LoadTexture("3lives.png");
    }
    else if (state.player->lives == 2){
        state.player->textureID = Util::LoadTexture("2lives.png");
    }
    else if(state.player->lives == 1){
        state.player->textureID = Util::LoadTexture("1life.png");
    }
    state.player->height = 1.0f;
    state.player->width = 1.0f;
    
    // Initialize Key
    state.key = new Entity();
    state.key->entityType = KEY;
    state.key->position = glm::vec3(13.5f, -13.5f, 0);
    state.key->textureID = Util::LoadTexture("key.png");
    
    //Initialize portal
    state.portal = new Entity();
    state.portal->entityType = PORTAL;
    state.portal->position = glm::vec3(1.5f, -1.5f, 0);
    state.portal->textureID = Util::LoadTexture("portal.png");
    state.portal->isActive = false;
    
    //Initialize Enemies
    state.enemies = new Entity[LEVEL5_ENEMY_COUNT];
    GLuint enemytextureID = Util::LoadTexture("enemy.png");
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemytextureID;
    state.enemies[0].position = glm::vec3(13.5, -5.5, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = PATROLLER;
    state.enemies[0].axis = true;
    state.enemies[0].start = 13.5;
    state.enemies[0].end = 1.5;
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemytextureID;
    state.enemies[1].position = glm::vec3(3, -9.5, 0);
    state.enemies[1].speed = 1;
    state.enemies[1].aiType = PATROLLER;
    state.enemies[1].axis = true;
    state.enemies[1].start = 13.5;
    state.enemies[1].end = 1.5;
    
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = enemytextureID;
    state.enemies[2].position = glm::vec3(9.5, -1.5, 0);
    state.enemies[2].speed = 1;
    state.enemies[2].aiType = PATROLLER;
    state.enemies[2].axis = false;
    state.enemies[2].start = -1.5;
    state.enemies[2].end = -13.5;
    
    state.enemies[3].entityType = ENEMY;
    state.enemies[3].textureID = enemytextureID;
    state.enemies[3].position = glm::vec3(5.5, -11.5, 0);
    state.enemies[3].speed = 1;
    state.enemies[3].aiType = PATROLLER;
    state.enemies[3].axis = false;
    state.enemies[3].start = -1.5;
    state.enemies[3].end = -13.5;
}
void Level5::Update(float deltaTime) {
    
    if(state.player->lives == 3){
        state.player->textureID = Util::LoadTexture("3lives.png");
    }
    else if (state.player->lives == 2){
        state.player->textureID = Util::LoadTexture("2lives.png");
    }
    else if(state.player->lives == 1){
        state.player->textureID = Util::LoadTexture("1life.png");
    }
    
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL5_ENEMY_COUNT, state.map, state.key, state.portal);
    state.key->Update(deltaTime, state.player, state.enemies, LEVEL5_ENEMY_COUNT, state.map, state.key, state.portal);
    state.portal->Update(deltaTime, state.player, state.enemies, LEVEL5_ENEMY_COUNT, state.map, state.key, state.portal);
    for (int i = 0; i < LEVEL5_ENEMY_COUNT; i++){
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL5_ENEMY_COUNT, state.map, state.key, state.portal);
    }
    
    if(state.player->restartLevel){
        state.player->restartLevel = false;
        state.player->position = glm::vec3(2.0f, -13.5f, 0);
    }
    
    if(state.player->touchedKey){
        state.key->isActive = false;
    }
    
    if(state.player->portalOpen){
        state.portal->isActive = true;
    }
    
    if (state.player->touchedPortal){
        state.nextScene = 6;
    }
}
void Level5::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    state.key->Render(program);
    state.portal->Render(program);
    for (int i = 0; i < LEVEL5_ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
}
