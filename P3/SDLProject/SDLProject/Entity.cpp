#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other){
    float xdist = fabs(position.x - other->position.x) - ((width + other->width)/2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height)/2.0f);
    
    if (xdist < 0 && ydist <0) return true;
    
    return false;
}

void Entity::Update(float deltaTime, Entity *platforms, Entity *walls, int platformCount, int wallCount)
{
    velocity += acceleration * deltaTime;
    position.x += velocity.x * deltaTime; // Move on X
    position.y += velocity.y * deltaTime; // Move on Y
    
    for (int i = 0; i<wallCount; i++){
        Entity *wall = &walls[i];
        if(CheckCollision(wall)){
            failure = true;
            velocity = glm::vec3(0);
            acceleration = glm::vec3(0);
        }
    }
    for (int i = 0; i<platformCount; i++){
        Entity *platform = &platforms[i];
        if(CheckCollision(platform)){
            success = true;
            velocity = glm::vec3(0);
            acceleration = glm::vec3(0);
        }
    }
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::Render(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
