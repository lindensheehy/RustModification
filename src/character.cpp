#include "../include/character.h"


/* ------------------------- */
/* ------- Character ------- */
/* ------------------------- */

// Constructors
Character::Character() {

    this->address = 0;
    this->position = new Vec3();
    this->facing = new Vec3();
    this->projected = new Vec3();

}

Character::Character(int8bytes address, BYTE* positionBytes, BYTE* facingBytes) {
    
    this->address = address;

    float* floatPositionBytes = reinterpret_cast<float*>(positionBytes);
    float* floatFacingBytes = reinterpret_cast<float*>(facingBytes);

    this->position = new Vec3(floatPositionBytes[0], floatPositionBytes[1], floatPositionBytes[2]);

    // For facing direction, components are as follows (y, z, x)
    this->facing = new Vec3(floatFacingBytes[2], floatFacingBytes[0], floatFacingBytes[1]);

    this->projected = new Vec3();

}

Character::Character(int8bytes address, Vec3* position, Vec3* facing) {
    
    this->address = address;

    this->position = position;
    this->facing = facing;

    this->projected = new Vec3();

}

Character::Character(int8bytes address, float posx, float posy, float posz, float facingx, float facingy, float facingz) {
    
    this->address = address;

    this->position = new Vec3(posx, posy, posz);
    this->facing = new Vec3(facingx, facingy, facingz);

    this->projected = new Vec3();

}

// Destructor
Character::~Character() {
    delete this->position;
    delete this->facing;
    delete this->projected;
}

// Instance Functions
void Character::project(Character* camera) {

    // Apply world to camera space transformation
    Vec3* cameraSpacePos = worldToCameraSpace(this->position, camera->position, camera->facing);
    if (cameraSpacePos == nullptr) return;

    // Transform camera space into screen space, z component holds the depth
    Vec3* projectedPos = cameraToScreenSpace(cameraSpacePos);
    if (projectedPos == nullptr) return;

    this->projected->set(projectedPos);

    // Clean up dynamically allocated memory for projection positions
    delete cameraSpacePos;
    delete projectedPos;  // No need to handle the other positions since `project` function combines them

}



/* ----------------------------- */
/* ------- CharacterList ------- */
/* ----------------------------- */

// Constructor
CharacterList::CharacterList() {
    this->list = new Character*[this->size];
    for (int i = 0; i < this->size; i++) this->list[i] = nullptr;
}

// Destructor
CharacterList::~CharacterList() {

    for (int i = 0; i < this->size; i++) {
        if (this->list[i] != nullptr) {
            delete this->list[i];
        }
    }

    delete[] this->list;

}

// Instance Functions
Character* CharacterList::getCharacter(int index) {

    if (index < 0 || index >= this->size) return nullptr;

    return this->list[index];
    
}

void CharacterList::setCharacter(Character* character, int index) {

    if (index < 0 || index >= this->size) return;

    if (this->list[index] != nullptr)
        delete this->list[index];

    this->list[index] = character;

}

void CharacterList::setPlayer(int8bytes address) {

    Character* current;

    for (int i = 0; i < this->size; i++) {

        current = this->getCharacter(i);

        if (current == nullptr) break;

        if (current->address == address) {
            this->camera = current;
            this->camera->position->y += 1.5;
            return;
        }

    }

}

void CharacterList::projectAll() {

    if (this->camera == nullptr) {
        logWrite("Cannot project before defining camera!", true);
        return;
    }

    Character* current;

    for (int i = 0; i < this->size; i++) {

        current = this->getCharacter(i);

        if (current == nullptr) break;

        current->project(this->camera);

    }

} 
