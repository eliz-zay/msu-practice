#pragma once

#include "Object.hpp"

void Object::setMaterial(Material* material) {
    this->material = material;
}

Material* Object::getMaterial() {
    return this->material;
}