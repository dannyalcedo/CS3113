#include "Level2.h"
#include "Util.h"
#define LEVEL2_WIDTH 16
#define LEVEL2_HEIGHT 16
#define LEVEL2_ENEMY_COUNT 2
unsigned int level2_data[] =
{
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3,
    3, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3,
    3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3,
    3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3,
    3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

void Level2::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
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
    state.key->position = glm::vec3(2.0f, -2.0f, 0);
    state.key->textureID = Util::LoadTexture("key.png");
    
    //Initialize portal
    state.portal = new Entity();
    state.portal->entityType = PORTAL;
    state.portal->position = glm::vec3(13.5f, -12.5f, 0);
    state.portal->textureID = Util::LoadTexture("portal.png");
    state.portal->isActive = false;
    
    //Initialize Enemies
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemytextureID = Util::LoadTexture("enemy.png");
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemytextureID;
    state.enemies[0].position = glm::vec3(11.5, -1.5, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = PATROLLER;
    state.enemies[0].axis = false;
    state.enemies[0].start = -1.5;
    state.enemies[0].end = -13.5;
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemytextureID;
    state.enemies[1].position = glm::vec3(4.5, -6.5, 0);
    state.enemies[1].speed = 1;
    state.enemies[1].aiType = PATROLLER;
    state.enemies[1].axis = false;
    state.enemies[1].start = -6.5;
    state.enemies[1].end = -13.5;
}
void Level2::Update(float deltaTime) {
    
    if(state.player->lives == 3){
        state.player->textureID = Util::LoadTexture("3lives.png");
    }
    else if (state.player->lives == 2){
        state.player->textureID = Util::LoadTexture("2lives.png");
    }
    else if(state.player->lives == 1){
        state.player->textureID = Util::LoadTexture("1life.png");
    }
    
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map, state.key, state.portal);
    state.key->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map, state.key, state.portal);
    state.portal->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map, state.key, state.portal);
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++){
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map, state.key, state.portal);
    }
    
    if(state.player->restartLevel){
        state.player->restartLevel = false;
        state.player->position = glm::vec3(1.5f, -13.5f, 0);
    }
    
    if(state.player->touchedKey){
        state.key->isActive = false;
    }
    
    if(state.player->portalOpen){
        state.portal->isActive = true;
    }
    
    if (state.player->touchedPortal){
        state.nextScene = 3;
    }
}
void Level2::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    state.key->Render(program);
    state.portal->Render(program);
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
}
