#include "World/entity.h"

Entity::Entity()
    :   position(glm::vec3(0.0f)), size(glm::vec3(1.0f)), color(glm::vec3(0.0f)),
        rotation(0.0f), active(true){ }

Entity::Entity(glm::vec3 pos, glm::vec3 size, glm::vec3 color)
    :   position(pos), size(size), color(color),
        rotation(0.0f), active(true){ }

const glm::mat4 Entity::GetModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->position);

    if (this->rotation != glm::vec3(0.0f)){
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0)); //pitch
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0)); //yaw
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1)); //roll
    }

    model = glm::scale(model, this->size);
    
    return model;
}
