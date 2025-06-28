#ifndef ENTITY_H
#define ENTITY_H

#include "OpenGL/renderer.h"

//STORES THE DATA managed by a single entity on a currently running program
class Entity{
public:
    glm::vec3 position, size, rotation, color;
    bool active;

    Entity();
    Entity(glm::vec3 pos, glm::vec3 size, glm::vec3 color);

    const glm::mat4 GetModelMatrix() const;

    virtual ~Entity() = default;

};

#endif