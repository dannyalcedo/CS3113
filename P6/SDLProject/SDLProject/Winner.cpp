#include "Winner.h"

void Winner::Initialize() {
    fontTextureID = Util::LoadTexture("font.png");
    state.nextScene = -1;
    
}
void Winner::Update(float deltaTime) {
}

void Winner::Render(ShaderProgram *program) {
    Util::DrawText(program, fontTextureID, "Good Job!", 1, -0.20, glm::vec3(-3.10, 2, 0));
    Util::DrawText(program, fontTextureID, "You've", 1, -0.20, glm::vec3(-2.10, 0, 0));
    Util::DrawText(program, fontTextureID, "completed", 1, -0.20, glm::vec3(-3.25, -1, 0));
    Util::DrawText(program, fontTextureID, "the quest", 1, -0.20, glm::vec3(-3.25, -2, 0));
}
