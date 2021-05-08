#include "Scene.h"
class Winner : public Scene {
public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
    GLuint fontTextureID;
};
