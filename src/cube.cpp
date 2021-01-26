#include "cube.hpp"

#include <cmath>
#include <iostream>

#include <windows.h>
#include <GL/GL.h>

#define PI (3.14159f)
#define TO_RADIANS(degrees) (degrees * PI / 180.0f)

#define RED 1, 0, 0
#define GREEN 0, 1, 0
#define BLUE 0, 0, 1

Transform::Transform() {
	setIdentity();
}

void Transform::setIdentity() {
	for (int i = 0; i < 16; i++) {
		mat[i] = 0;
	}

	mat[0] = 1;
	mat[5] = 1;
	mat[10] = 1;
	mat[15] = 1;
}

void Transform::setTranslation(float x, float y, float z) {
	mat[12] = x;
	mat[13] = y;
	mat[14] = z;
}

void Transform::translate(float x, float y, float z) {
	mat[12] += x;
	mat[13] += y;
	mat[14] += z;
}

void setRot(float mat[16], float yaw, float pitch, float roll) {
	float cosYaw = std::cosf(TO_RADIANS(yaw));
	float sinYaw = std::sinf(TO_RADIANS(yaw));

	float cosPitch = std::cosf(TO_RADIANS(pitch));
	float sinPitch = std::sinf(TO_RADIANS(pitch));

	float cosRoll = std::cosf(TO_RADIANS(roll));
	float sinRoll = std::sinf(TO_RADIANS(roll));

	for (int i = 0; i < 16; i++) {
		mat[i] = 0;
	}

	mat[0] = cosYaw * cosPitch;
	mat[1] = (cosYaw * sinPitch * sinRoll) - (sinYaw * cosRoll);
	mat[2] = (cosYaw * sinPitch * cosRoll) + (sinYaw * sinRoll);
	mat[3] = 0;
	mat[4] = sinYaw * cosPitch;
	mat[5] = (sinYaw * sinPitch * sinRoll) + (cosYaw * cosRoll);
	mat[6] = (sinYaw * sinPitch * cosRoll) - (cosYaw * sinRoll);
	mat[7] = 0;
	mat[8] = -sinPitch;
	mat[9] = cosPitch * sinRoll;
	mat[10] = cosRoll * cosPitch;
	mat[15] = 1;
}

void Transform::setRotation(float yaw, float pitch, float roll) {
	setRot(mat, yaw, pitch, roll);
}

void Transform::rotate(float yaw, float pitch, float roll) {
	float mat2[16];

	for (int i = 0; i < 16; i++)
		mat2[i] = 0;

	mat2[0] = 1;
	mat2[5] = 1;
	mat2[10] = 1;
	mat2[15] = 1;

	setRot(mat2, yaw, pitch, roll);

	mat[0] = mat[0] * mat2[0] + mat[1] * mat2[4] + mat[2] * mat2[8];
	mat[1] = mat[0] * mat2[1] + mat[1] * mat2[5] + mat[2] * mat2[9];
	mat[2] = mat[0] * mat2[2] + mat[1] * mat2[6] + mat[2] * mat2[10];


	mat[4] = mat[4] * mat2[0] + mat[5] * mat2[4] + mat[6] * mat2[8];
	mat[5] = mat[4] * mat2[1] + mat[5] * mat2[5] + mat[6] * mat2[9];
	mat[6] = mat[4] * mat2[2] + mat[5] * mat2[6] + mat[6] * mat2[10];

	mat[8] = mat[8] * mat2[0] + mat[9] * mat2[4] + mat[10] * mat2[8];
	mat[9] = mat[8] * mat2[1] + mat[9] * mat2[5] + mat[10] * mat2[9];
	mat[10] = mat[8] * mat2[2] + mat[9] * mat2[6] + mat[10] * mat2[10];
}

void Transform::print() {
	for (int i = 0; i < 4; i++) {
		std::cout << "[ ";
        for(int j = 0; j < 4; j++){
            std::cout << mat[i * 4 + j] << " ";
        }
        std::cout << "]\n";
	}
}

void calcVertex(Coord3D& position, float x, float y, float z, float r, float g, float b) {
	Coord3D result;

	result.x = position.x + x * 0.5f;
	result.y = position.y + y * 0.5f; 
	result.z = position.z + z * 0.5f;
	glColor3f(r, g, b);
	glVertex3f(result.x, result.y, result.z);
}

void Cube::draw() {
	Coord3D pos;

	glPushMatrix();

	glMultMatrixf(transform.getMatrix());

	glBegin(GL_QUADS);

	//frente e tras
	calcVertex(position, -1, -1, 1, RED);
	calcVertex(position, 1, -1, 1, GREEN);
	calcVertex(position, 1, 1, 1, BLUE);
	calcVertex(position, -1, 1, 1, RED);

	calcVertex(position, -1, -1, -1, RED);
	calcVertex(position, 1, -1, -1, GREEN);
	calcVertex(position, 1, 1, -1, BLUE);
	calcVertex(position, -1, 1, -1, RED);

	//laterais: direita&esquerda
	calcVertex(position, 1, -1, -1, RED);
	calcVertex(position, 1, -1, 1, GREEN);
	calcVertex(position, 1, 1, 1, BLUE);
	calcVertex(position, 1, 1, -1, RED);

	calcVertex(position, -1, -1, -1, RED);
	calcVertex(position, -1, -1, 1, GREEN);
	calcVertex(position, -1, 1, 1, BLUE);
	calcVertex(position, -1, 1, -1, RED);

	//tampa cima&baixo
	calcVertex(position, -1, 1, 1, RED);
	calcVertex(position, 1, 1, 1, GREEN);
	calcVertex(position, 1, 1, -1, BLUE);
	calcVertex(position, -1, 1, -1, RED);

	calcVertex(position, -1, -1, 1, RED);
	calcVertex(position, 1, -1, 1, GREEN);
	calcVertex(position, 1, -1, -1, BLUE);
	calcVertex(position, -1, -1, -1, RED);

	glEnd();

	glPopMatrix();
}