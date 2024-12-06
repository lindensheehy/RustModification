#include "../include/projection.h"


/* ------------------- */
/* ---   Matrix4   --- */
/* ------------------- */

// Constructors
Matrix4::Matrix4() {
    
    // Build matrix on the heap
    m = new float*[4];
    for (int i = 0; i < 4; ++i)
        m[i] = new float[4] {0.0f};

    // Set diagonal elements to 1 (identity matrix)
    for (int i = 0; i < 4; ++i)
        m[i][i] = 1.0f;

}

Matrix4::Matrix4(float values[4][4]) {

    m = new float*[4];

    for (int i = 0; i < 4; ++i) {

        m[i] = new float[4];

        for (int j = 0; j < 4; ++j)
            m[i][j] = values[i][j];

    }

}

// Destructor
Matrix4::~Matrix4() {

    for (int i = 0; i < 4; ++i)
        delete[] m[i];

    delete[] m;

}


// Instance Functions
Matrix4* Matrix4::mul(Matrix4* other) const {

    Matrix4* result = new Matrix4();

    for (int i = 0; i < 4; ++i) {

        for (int j = 0; j < 4; ++j) {

            result->m[i][j] = 0.0f;

            for (int k = 0; k < 4; ++k)
                result->m[i][j] += m[i][k] * other->m[k][j];

        }

    }

    return result;

}

Vec4* Matrix4::mul(Vec4* vec) const {

    // Perform the matrix-vector multiplication
    float x = (m[0][0] * vec->x) + (m[0][1] * vec->y) + (m[0][2] * vec->z) + (m[0][3] * vec->w);
    float y = (m[1][0] * vec->x) + (m[1][1] * vec->y) + (m[1][2] * vec->z) + (m[1][3] * vec->w);
    float z = (m[2][0] * vec->x) + (m[2][1] * vec->y) + (m[2][2] * vec->z) + (m[2][3] * vec->w);
    float w = (m[3][0] * vec->x) + (m[3][1] * vec->y) + (m[3][2] * vec->z) + (m[3][3] * vec->w);

    // Return the result as a new Vec4
    return new Vec4(x, y, z, w);

}

void Matrix4::print() const {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// Class Functions
Matrix4* Matrix4::translation(float dx, float dy, float dz) {
    
    // New identity matrix
    Matrix4* m = new Matrix4();

    m->m[0][3] = dx;
    m->m[1][3] = dy;
    m->m[2][3] = dz;

    return m;

}

Matrix4* Matrix4::rotation(float angle) {

    Matrix4* m = new Matrix4();

    float cosA = cos(angle);
    float sinA = sin(angle);

    m->m[0][0] = cosA;
    m->m[0][1] = -sinA;
    m->m[1][0] = sinA;
    m->m[1][1] = cosA;

    return m;

}

Matrix4* Matrix4::perspective(float fov, float aspect, float nearClip, float farClip) {

    if (nearClip == farClip) return nullptr;

    Matrix4* m = new Matrix4();

    float tanHalfFov = tan(fov / 2.0f);

    m->m[0][0] = 1.0f / (aspect * tanHalfFov);
    m->m[1][1] = 1.0f / tanHalfFov;
    m->m[2][2] = -(farClip + nearClip) / (farClip - nearClip);
    m->m[2][3] = -(farClip * nearClip) / (farClip - nearClip);
    m->m[3][2] = -1.0f;

    return m;

}

Matrix4* Matrix4::lookAt(Vec3* position, Vec3* target, Vec3* up) {

    Vec3* zaxis = position->copy()->sub(target)->normalise();       // Camera's forward axis
    Vec3* xaxis = up->crossProduct(zaxis)->normalise();             // Camera's right axis
    Vec3* yaxis = zaxis->crossProduct(xaxis);                       // Camera's up axis


    Matrix4* m = new Matrix4();

    m->m[0][0] = xaxis->x;
    m->m[1][0] = xaxis->y;
    m->m[2][0] = xaxis->z;

    m->m[0][1] = yaxis->x;
    m->m[1][1] = yaxis->y;
    m->m[2][1] = yaxis->z;

    m->m[0][2] = -zaxis->x;
    m->m[1][2] = -zaxis->y;
    m->m[2][2] = -zaxis->z;

    m->m[0][3] = -xaxis->dotProduct(position);
    m->m[1][3] = -yaxis->dotProduct(position);
    m->m[2][3] = zaxis->dotProduct(position);

    return m;

}



/* -------------------------------- */
/* ---   Projection Functions   --- */
/* -------------------------------- */

// Function to convert from world space to camera space
Vec3* worldToCameraSpace(Vec3* worldPos, Vec3* cameraPos, Vec3* cameraDir) {

    // Translation, cameraPos is subtracted from worldPos
    Vec3* relativePos3 = worldPos->copy()->sub(cameraPos);

    // Rotation: Align the camera facing direction to world axes
    // Assuming cameraDir is a normalized vector for the camera's facing direction
    Vec3* forward = cameraDir->copy();  // Camera forward direction
    Vec3* temp = new Vec3(0, 1, 0);
    Vec3* right = forward->crossProduct(temp)->normalise(); // Right vector
    
    Vec3* up = right->crossProduct(forward)->normalise();  // Up vector

    // Construct the camera space matrix (rotation and translation)
    Matrix4* cameraMatrix = new Matrix4();

    cameraMatrix->m[0][0] = right->x;
    cameraMatrix->m[0][1] = right->y;
    cameraMatrix->m[0][2] = right->z;
    cameraMatrix->m[0][3] = 0.0f;

    cameraMatrix->m[1][0] = up->x;
    cameraMatrix->m[1][1] = up->y;
    cameraMatrix->m[1][2] = up->z;
    cameraMatrix->m[1][3] = 0.0f;

    cameraMatrix->m[2][0] = -forward->x;
    cameraMatrix->m[2][1] = -forward->y;
    cameraMatrix->m[2][2] = -forward->z;
    cameraMatrix->m[2][3] = 0.0f;

    cameraMatrix->m[3][0] = -right->dotProduct(relativePos3);
    cameraMatrix->m[3][1] = -up->dotProduct(relativePos3);
    cameraMatrix->m[3][2] = forward->dotProduct(relativePos3);
    cameraMatrix->m[3][3] = 1.0f;

    // Create a Vec4 for relative position (set w to 1)
    Vec4* relativePos4 = new Vec4(relativePos3->x, relativePos3->y, relativePos3->z, 1.0f);

    // Transform the position using the matrix
    Vec4* result = cameraMatrix->mul(relativePos4); // Use Matrix4::mul for multiplication
    Vec3* ret = new Vec3(result->x, result->y, result->z);

    // Clean up
    delete relativePos3;
    delete relativePos4;
    delete forward;
    delete temp;
    delete right;
    delete up;
    delete cameraMatrix;
    delete result;

    return ret;

}

// Handles the projection. Turns camera space coordinates into screen coordinates. return->z is distance from cam
Vec3* cameraToScreenSpace(Vec3* vec) {

    Vec3* vecCopy = vec->copy();

    vecCopy->project();

    const double screenWidth = 1800;
    const double screenHeight = 900;

    double drawPosx = vecCopy->x * (double) screenWidth;
    double drawPosy = screenHeight - (vecCopy->y * (double) screenHeight); // minus because y=0 is at the top
    double depth = vec->magnitude();

    delete vecCopy;

    return new Vec3(drawPosx, drawPosy, depth);

}
