#pragma once

#include <windows.h>

#include "vec.h"
#include "projection.h"

class Character {

    public:

        int8bytes address;

        Vec3* position;
        Vec3* facing;

        Vec3* projected;

        // Constructors
        Character();
        Character(int8bytes address, BYTE* positionBytes, BYTE* facingBytes);
        Character(int8bytes address, Vec3* position, Vec3* facing);
        Character(int8bytes address, float posx, float posy, float posz, float facingx, float facingy, float facingz);
        
        // Destructor
        ~Character();

        // Instance Functions
        void project(Character* camera);

};

class CharacterList {

    public:

        /*   Instance Variables   */

        Character** list;
        const int size = 100;

        // Stores a reference to the players Character object, this is flagged as the facing direction not being (0, 0, 0)
        Character* camera;


        // Constructor
        CharacterList();

        // Destructor
        ~CharacterList();

        // Instance Functions
        Character* getCharacter(int index);

        void setCharacter(Character* character, int index);

        // Links the player to a given address. This populates this->player
        void setPlayer(int8bytes address);

        // Fills the projected coordinates of all the characters in the list
        void projectAll();

};
