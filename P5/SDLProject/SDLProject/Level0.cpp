#include "Level0.h"

void Level0::Initialize() {
    fontTextureID = Util::LoadTexture("font.png");
    state.nextScene = -1;
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    
}
void Level0::Update(float deltaTime) {
    if (state.player->startGame == true){
        state.nextScene = 1;
    }
}
void Level0::Render(ShaderProgram *program) {
    Util::DrawText(program, fontTextureID, "The A(EYE) Strike", 1, -0.5f, glm::vec3(-4.0, 2.0, 0));
    Util::DrawText(program, fontTextureID, "Back!", 1, -0.5f, glm::vec3(-1.0, 1.0, 0));
    Util::DrawText(program, fontTextureID, "Press Enter", 1, -0.5f, glm::vec3(-2.5, -1, 0));
    Util::DrawText(program, fontTextureID, "to Begin", 1, -0.5f, glm::vec3(-2.0, -2.0, 0));
}
