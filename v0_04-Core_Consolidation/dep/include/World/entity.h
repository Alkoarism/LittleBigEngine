#ifndef ENTITY_H
#define ENTITY_H

#include "OpenGL/renderer.h"

#include "World/model.h"

//STORES THE DATA managed by a single entity on a currently running program
class Entity{
public:
    glm::vec3 position, size, color;
    float rotation;
    bool active;

    Entity();
    Entity(glm::vec3 pos, glm::vec3 size, glm::vec3 color);

    virtual ~Entity() = default;

    virtual void Draw (const Model&);
};

#endif