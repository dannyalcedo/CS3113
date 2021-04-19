#include "Level4.h"

void Level4::Initialize() {
    fontTextureID = Util::LoadTexture("font.png");
    state.nextScene = -1;
    
}
void Level4::Update(float deltaTime) {
}

void Level4::Render(ShaderProgram *program) {
    Util::DrawText(program, fontTextureID, "Good Job!", 1, -0.5f, glm::vec3(5.5, -2.0, 0));
    Util::DrawText(program, fontTextureID, "You've completed", 1, -0.5f, glm::vec3(3.75, -4, 0));
    Util::DrawText(program, fontTextureID, "the quest", 1, -0.5f, glm::vec3(5.5, -5.0, 0));
}
