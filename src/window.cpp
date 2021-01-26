#include "window.hpp"

#include <string>
#include <iostream>

#include "opengl.hpp"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg) {
	case WM_CREATE: {
		HDC hdc;
		HGLRC glrc;
		if (initOpenGL(hwnd, &hdc, &glrc)) {
			onCreate(hwnd, hdc, glrc);
		}
		else {
			std::cerr << "Falha ao criar opengl!" << std::endl;
			PostQuitMessage(-1);
		}
	}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		onKeyDown((int)wParam);
		break;
	case WM_KEYUP:
		onKeyDown((int)wParam);
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

WCHAR* WideStrFromUTF8Str(const char* source) {
	int count = MultiByteToWideChar(CP_UTF8, 0, source, -1, NULL, 0);
	if (!count) {
		return nullptr;
	}

	WCHAR* target = (WCHAR*)calloc(count, sizeof(WCHAR));

	if (!MultiByteToWideChar(CP_UTF8, 0, source, -1, target, count)) {
		free(target);
		return nullptr;
	}

	return target;
}

char* UTF8StrFromWideStr(const WCHAR* source) {
	int size = WideCharToMultiByte(CP_UTF8, 0, source, -1, NULL, 0, NULL, NULL);
	if (!size) {
		return nullptr;
	}

	char* target = (char*)calloc(size, 1);

	if (!WideCharToMultiByte(CP_UTF8, 0, source, -1, target, size, NULL, NULL)) {
		free(target);
		return nullptr;
	}

	return target;
}

HWND createWindow(const char* c_title, unsigned int width, unsigned int height){
    HWND window;
	WNDCLASSEXW wc;

    std::wstring windowClassName = L"MCUBE";

	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = GetModuleHandleW(NULL);
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wc.lpszClassName = windowClassName.c_str();

	if (!RegisterClassExW(&wc)) {
		return nullptr;
	}

	WCHAR* title = WideStrFromUTF8Str(c_title);

	window = CreateWindowW(
		windowClassName.c_str(),
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL,
		NULL,
		GetModuleHandleW(NULL),
		NULL
	);

	free(title);

	if (window == NULL) {
        return nullptr;
	}

	ShowWindow(window, SW_SHOW);

    return window;
}

bool isKeyPressed(int vk) {
	return (GetAsyncKeyState(vk) & (sizeof(SHORT) - 1)) == 1;
}