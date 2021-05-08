#include "Level1.h"
#include "Util.h"
#define LEVEL1_WIDTH 16
#define LEVEL1_HEIGHT 16
#define LEVEL1_ENEMY_COUNT 0
unsigned int level1_data[] =
{
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

void Level1::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->lives = 3;
    state.player->position = glm::vec3(1.5f, -7.5f, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("3lives.png");
    state.player->height = 1.0f;
    state.player->width = 1.0f;
    
    // Initialize Key
    state.key = new Entity();
    state.key->entityType = KEY;
    state.key->position = glm::vec3(13.5f, -7.5f, 0);
    state.key->textureID = Util::LoadTexture("key.png");
    
    //Initialize portal
    state.portal = new Entity();
    state.portal->entityType = PORTAL;
    state.portal->position = glm::vec3(7.0f, -7.5f, 0);
    state.portal->textureID = Util::LoadTexture("portal.png");
    state.portal->isActive = false;
}
void Level1::Update(float deltaTime) {
    
    if(state.player->lives == 3){
        state.player->textureID = Util::LoadTexture("3lives.png");
    }
    else if (state.player->lives == 2){
        state.player->textureID = Util::LoadTexture("2lives.png");
    }
    else if(state.player->lives == 1){
        state.player->textureID = Util::LoadTexture("1life.png");
    }
    
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map, state.key, state.portal);
    state.key->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map, state.key, state.portal);
    state.portal->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map, state.key, state.portal);
    
    if(state.player->restartLevel){
        state.player->restartLevel = false;
        state.player->position = glm::vec3(1.5f, -7.5f, 0);
    }
    
    if(state.player->touchedKey){
        state.key->isActive = false;
    }
    
    if(state.player->portalOpen){
        state.portal->isActive = true;
    }
    
    if (state.player->touchedPortal){
        state.nextScene = 2;
    }
}
void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    state.key->Render(program);
    state.portal->Render(program);
}
