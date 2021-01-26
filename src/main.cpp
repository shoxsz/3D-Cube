#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>

#include "window.hpp"
#include <gl/GL.h>

#include "cube.hpp"
#include "fps_counter.hpp"

HWND g_window;
HDC g_hdc;
HGLRC g_glrc;

float speed = 200;

float anglexz = 0;
float angleyz = 0;
float anglexy = 0;

void glPerspective(GLfloat* matrix, float ratio, float fovy, float nearp, float farp) {
	float v = std::tanf(fovy * 3.14f / 360.0f) * nearp;
	
	float top = v;
	float bottom = -top;

	float right = v * ratio;
	float left = -right;

	float temp, temp2, temp3, temp4;
	temp = 2 * nearp;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = farp - nearp;

	matrix[0] = temp / temp2;
	matrix[1] = 0.0;
	matrix[2] = 0.0;
	matrix[3] = 0.0;
	matrix[4] = 0.0;
	matrix[5] = temp / temp3;
	matrix[6] = 0.0;
	matrix[7] = 0.0;
	matrix[8] = (right + left) / temp2;
	matrix[9] = (top + bottom) / temp3;
	matrix[10] = (-farp - nearp) / temp4;
	matrix[11] = -1.0;
	matrix[12] = 0.0;
	matrix[13] = 0.0;
	matrix[14] = (-temp * farp) / temp4;
	matrix[15] = 0.0;
}

void onCreate(HWND hwnd, HDC hdc, HGLRC glrc) {
	g_window = hwnd;
	g_hdc = hdc;
	g_glrc = glrc;
}

void onKeyDown(int vk){
}

void onKeyUp(int vk){
}

int main(){
    MSG msg;

	Cube cube;
	FPSCounter fps;
	GLfloat matrix[16];
	std::thread logFPS;
	bool alive;

    createWindow("Magic Cube", 800, 800);

	cube.getPosition().set(0, 0, 0);
	glPerspective(matrix, 1, 90, 0.1f, 100.0f);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.5f, 1.0f, 1);
	fps.setFPS(120);
	alive = true;

	logFPS = std::thread([&fps, &alive]() -> void {
		while (alive) {
			float delta = fps.getFrameDelta();
			float actualFPS = fps.getActualFPS();
			std::cout << "FPS: " << actualFPS << " Last Frame:  " << delta << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds((int)delta * 1000));
		}
	});

    while (alive) {
		fps.delay();

		float delta = fps.getFrameDelta();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

			if (msg.message == WM_QUIT) {
				alive = false;
				logFPS.join();
				return msg.wParam;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (isKeyPressed('A')) {
			anglexz -= speed * delta;
		}
		
		if (isKeyPressed('D')) {
			anglexz += speed * delta;
		}

		if (isKeyPressed('W')) {
			angleyz -= speed * delta;
		}

		if (isKeyPressed('S')) {
			angleyz += speed * delta;
		}

		if (isKeyPressed('Q')) {
			anglexy -= speed * delta;
		}

		if (isKeyPressed('E')) {
			anglexy += speed * delta;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(matrix);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, 0, -5);

		cube.getTransform().setRotation(angleyz, anglexz, anglexy);

		cube.draw();

		wglSwapLayerBuffers(g_hdc, WGL_SWAP_MAIN_PLANE);
	}
}