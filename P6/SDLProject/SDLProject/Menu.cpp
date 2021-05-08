#include "Menu.h"

void Menu::Initialize() {
    fontTextureID = Util::LoadTexture("font.png");
    state.nextScene = -1;
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
}
void Menu::Update(float deltaTime) {
    if (state.player->startGame == true){
        state.nextScene = 1;
    }
}

void Menu::Render(ShaderProgram *program) {
    Util::DrawText(program, fontTextureID, "ROOMS", 1, -0.20, glm::vec3(-1.5, 1.5, 0));
    Util::DrawText(program, fontTextureID, "Press Enter", 0.7, -0.20, glm::vec3(-2.5, -0.5, 0));
    Util::DrawText(program, fontTextureID, "to", 0.7, -0.20, glm::vec3(-0.25, -1, 0));
    Util::DrawText(program, fontTextureID, "Start", 0.7, -0.20, glm::vec3(-1, -1.5, 0));
}
