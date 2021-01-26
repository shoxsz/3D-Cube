#ifndef _MMWINDOW_HPP_
#define _MMWINDOW_HPP_

#include <windows.h>

WCHAR* WideStrFromUTF8Str(const char* source);
char* UTF8StrFromWideStr(const WCHAR* source);

HWND createWindow(const char* c_title, unsigned int width, unsigned int height);

bool isKeyPressed(int vk);

void onCreate(HWND hwnd, HDC hdc, HGLRC glrc);
void onKeyDown(int vk);
void onKeyUp(int vk);

#endif