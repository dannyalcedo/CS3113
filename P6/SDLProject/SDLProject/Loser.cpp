#include "Loser.h"

void Loser::Initialize() {
    fontTextureID = Util::LoadTexture("font.png");
    state.nextScene = -1;
    
}
void Loser::Update(float deltaTime) {
}

void Loser::Render(ShaderProgram *program) {
    Util::DrawText(program, fontTextureID, "You", 1, -0.20, glm::vec3(-0.7, 0.5, 0));
    Util::DrawText(program, fontTextureID, "Lost", 1, -0.20, glm::vec3(-1, -0.5, 0));
}
