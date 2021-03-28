#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
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
               if(object->entityType == PLATFORM) collidePlat[0] = true; // collided with platform top
               if (object->entityType == ENEMY && object->aiType == WAITANDGO) collideEnem1[0] = true; // collided with enemy 1 top
               else if (object->entityType == ENEMY && object->aiType == JUMPER) collideEnem2[0] = true;
               else if (object->entityType == ENEMY && object->aiType == WALKER) collideEnem3[0] = true;
           }
           else if (velocity.y < 0) {
               position.y += penetrationY;
               velocity.y = 0;
               if(object->entityType == PLATFORM) collidePlat[1] = true; // collided with platform bottom
               if (object->entityType == ENEMY && object->aiType == WAITANDGO) collideEnem1[1] = true; // collided with enemy 1 bottom
               else if (object->entityType == ENEMY && object->aiType == JUMPER) collideEnem2[1] = true;
               else if (object->entityType == ENEMY && object->aiType == WALKER) collideEnem3[1] = true;
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
               if(object->entityType == PLATFORM) collidePlat[3] = true;
               if (object->entityType == ENEMY && object->aiType == WAITANDGO) collideEnem1[3] = true;
               else if (object->entityType == ENEMY && object->aiType == JUMPER) collideEnem2[3] = true;
               else if (object->entityType == ENEMY && object->aiType == WALKER) collideEnem3[3] = true;
           }
           else if (velocity.x < 0) {
               position.x += penetrationX;
               velocity.x = 0;
               if(object->entityType == PLATFORM) collidePlat[2] = true;
               if (object->entityType == ENEMY && object->aiType == WAITANDGO) collideEnem1[2] = true;
               else if (object->entityType == ENEMY && object->aiType == JUMPER) collideEnem2[2] = true;
               else if (object->entityType == ENEMY && object->aiType == WALKER) collideEnem3[2] = true;
           }
       }
   }
}

void Entity::AIWaitAndGo(Entity *player){
    
    switch (aiState){
        case IDLE:
            if (glm::distance(position, player->position) < 3.0f){
                aiState = WALKING;
            }
            break;
        case WALKING:
            if (player->position.x < position.x){
                movement = glm::vec3(-1, 0, 0);
            }
            else{
                movement = glm::vec3(1, 0, 0);
            }
            break;
        case ATTACKING:
            break;
    }
}

void Entity::AIJumper(){
    if (collidePlat[1]){
        jump = true;
    }
}

void Entity::AIWalker(){
    if (position.x > 0){
        position.x = 0;
        flip = -1;
    }
    else if(position.x < -4.5){
        position.x = -4.5;
        flip = 1;
    }
    
    movement = glm::vec3(1*flip, 0, 0);
    
}

void Entity::AI(Entity *player){
    switch(aiType){
        case WAITANDGO:
            AIWaitAndGo(player);
            break;
        case JUMPER:
            AIJumper();
            break;
        case WALKER:
            AIWalker();
            break;
    }
}

void Entity::Update(float deltaTime, Entity *player, Entity *platforms, Entity *enemies, int platformCount, int enemyCount)
{
    if(isActive == false) return;
    
    std::fill(collidePlat.begin(), collidePlat.end(), false);
    std::fill(collideEnem1.begin(), collideEnem1.end(), false);
    std::fill(collideEnem2.begin(), collideEnem2.end(), false);
    std::fill(collideEnem3.begin(), collideEnem3.end(), false);
    
    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        } else {
            animIndex = 0;
        }
    }
    
    if(jump){
        jump = false;
        velocity.y += jumpPower;
    }
    
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    
    position.y += velocity.y * deltaTime; // Move on Y
    CheckCollisionsY(platforms, platformCount);// Fix if needed
    CheckCollisionsY(enemies, enemyCount);
    
    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(platforms, platformCount);// Fix if needed
    CheckCollisionsX(enemies, enemyCount);
    
    if (entityType == ENEMY){
        AI(player);
    }
    
    if (entityType == PLAYER){
        if(collideEnem1[0] || collideEnem1[2] ||collideEnem1[3]){
            failure = true;
            isActive = false;
        }
        else if(collideEnem2[0] || collideEnem2[2] ||collideEnem2[3]){
            failure = true;
            isActive = false;
        }
        else if(collideEnem3[0] || collideEnem3[2] ||collideEnem3[3]){
            failure = true;
            isActive = false;
        }
        
        if(failure){
            for (int i = 0; i < enemyCount; i++){
                enemies[i].velocity = glm::vec3(0);
                enemies[i].acceleration = glm::vec3(0);
                enemies[i].speed = 0;
            }
        }
        
        if (collideEnem1[1]){
            numberBeaten += 1;
            enemies[0].isActive = false;
        }
        else if (collideEnem2[1]){
            numberBeaten += 1;
            enemies[1].isActive = false;
        }
        else if (collideEnem3[1]){
            numberBeaten += 1;
            enemies[2].isActive = false;
        }
        
        if(numberBeaten == 3){
            success = true;
            velocity = glm::vec3(0);
            acceleration = glm::vec3(0);
        }
        
    }
    
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    
    if(isActive == false) return;
    
    program->SetModelMatrix(modelMatrix);
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }
    
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
