#pragma once

#include <iostream>
#include <array>

#include "vec.h"



class Matrix4 {

    public:

        /*   Instance Variables   */

        // Holds the matrix values as a 4x4 array
        float** m;


        /*   Constructors   */

        // Default constructor. Initializes as an identity matrix
        Matrix4();

        // Constructor to initialize with a custom 4x4 matrix
        Matrix4(float values[4][4]);

        // Destructor
        ~Matrix4();


        /*   Instance Functions   */

        // Matrix multiplication. Multiplies this matrix with another Matrix4 and returns the result
        Matrix4* mul(Matrix4* other) const;

        // Matrix-vector multiplication. Multiplies this matrix with a Vec4 and returns the result
        Vec4* mul(Vec4* vec) const;

        // Helper method to print the matrix for debugging
        void print() const;


        /*   Class Functions   */

        // Set up a translation matrix (translation by dx, dy, dz)
        static Matrix4* translation(float dx, float dy, float dz);

        // Set up a rotation matrix (rotation around the Z axis by angle)
        static Matrix4* rotation(float angle);

        // Set up a perspective projection matrix (field of view, aspect ratio, near and far planes)
        static Matrix4* perspective(float fov, float aspect, float nearClip, float farClip);

        // Set up a view matrix (look-at matrix)
        static Matrix4* lookAt(Vec3* position, Vec3* target, Vec3* up);

};



// Function to convert from world space to camera space
Vec3* worldToCameraSpace(Vec3* worldPos, Vec3* cameraPos, Vec3* cameraDir);

// Handles the projection. Turns camera space coordinates into screen coordinates. return->z is distance from cam
Vec3* cameraToScreenSpace(Vec3* vec);
