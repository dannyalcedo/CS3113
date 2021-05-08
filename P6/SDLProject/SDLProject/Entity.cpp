#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other){
  
    if (other == this) return false;
    
    if(isActive == false || other->isActive == false) return false;
    float xdist = fabs(position.x - other->position.x)-((width + other->width)/2.0f);
    float ydist = fabs(position.y - other->position.y)-((height + other->height)/2.0f);
    
    if (xdist < 0 && ydist < 0) return true;
    
    return false;
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount)
{
   for (int i = 0; i < objectCount; i++)
   {
       Entity *object = &objects[i];
       if (CheckCollision(object))
       {
           float ydist = fabs(position.y - object->position.y);
           float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
           if (velocity.y > 0) {
               position.y -= penetrationY;
               velocity.y = 0;
               if (object->entityType == ENEMY) collideEnem[0] = true; // collided with enemy 1 top
           }
           else if (velocity.y < 0) {
               position.y += penetrationY;
               velocity.y = 0;
               if (object->entityType == ENEMY) collideEnem[1] = true; // collided with enemy 1 bottom
           }
       }
   }
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount)
{
   for (int i = 0; i < objectCount; i++)
   {
       Entity *object = &objects[i];
       if (CheckCollision(object))
       {
           float xdist = fabs(position.x - object->position.x);
           float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
           if (velocity.x > 0) {
               position.x -= penetrationX;
               velocity.x = 0;
               if (object->entityType == ENEMY) collideEnem[3] = true;
           }
           else if (velocity.x < 0) {
               position.x += penetrationX;
               velocity.x = 0;
               if (object->entityType == ENEMY) collideEnem[2] = true;
           }
       }
   }
}

void Entity::CheckCollisionsY(Map *map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
    }
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
    }
}

void Entity::CheckCollisionsX(Map *map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);
    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        std::cout<<"Here3"<<std::endl;
        position.x += penetration_x;
        velocity.x = 0;
        //collidePlat[2] = true;
    }
    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        std::cout<<"Here4"<<std::endl;
        position.x -= penetration_x;
        velocity.x = 0;
        //collidePlat[3] = true;
    }
}

void Entity::AIPatroller(float start, float end, bool axis){ // if axis == true, x-axis, if false y-axis
    if (axis){
        if (position.x > start){
            position.x = start;
            flip = -1;
        }
        else if(position.x < end){
            position.x = end;
            flip = 1;
        }
        movement = glm::vec3(1*flip, 0, 0);
    }
    else{
        if (position.y > start){
            position.y = start;
            flip = -1;
        }
        else if(position.y < end){
            position.y = end;
            flip = 1;
        }
        movement = glm::vec3(0, 1*flip, 0);
    }
}

void Entity::AI(Entity *player, float deltaTime){
    AIPatroller(start, end, axis);
}

void Entity::Update(float deltaTime, Entity *player, Entity *enemies, int enemyCount, Map *map, Entity *key, Entity *portal)
{
    if(isActive == false) return;
    
    //std::fill(collidePlat.begin(), collidePlat.end(), false);
    std::fill(collideEnem.begin(), collideEnem.end(), false);
    
    velocity.x = movement.x * speed;
    velocity.y = movement.y * speed;
    
    position.y += velocity.y * deltaTime;
    CheckCollisionsY(map);
    CheckCollisionsY(enemies, enemyCount);
    
    position.x += velocity.x * deltaTime;
    CheckCollisionsX(map);
    CheckCollisionsX(enemies, enemyCount);
    
    if (entityType == ENEMY){
        AI(player, deltaTime);
    }
    
    if (entityType == PLAYER){
        if(collideEnem[0] || collideEnem[2] ||collideEnem[3]){
            lives -= 1;
            passLives = true;
            restartLevel = true;
        }
        
        if(failure){
            for (int i = 0; i < enemyCount; i++){
                enemies[i].velocity = glm::vec3(0);
                enemies[i].speed = 0;
            }
        }
        
        if(lives == 0){
            failure = true;
            isActive = false;
        }
        
        if(CheckCollision(key)){
            touchedKey = true;
            portalOpen = true;
        }
        
        if(CheckCollision(portal)){
            touchedPortal = true;
        }
        
    }
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::Render(ShaderProgram *program) {
    
    if(isActive == false) return;
    
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
