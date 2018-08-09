#pragma once


#include <Windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include "DefineData.h"
#include "resource.h"

#define String std::string




LRESULT CALLBACK mainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void OnCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void OnBtnClick(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void OnTrayMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void OnRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DelTrayIcon();
void CloseApp(HWND hWnd);
void StayWindowSide(HWND hWnd);