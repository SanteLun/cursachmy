// stdafx.h: ���������� ���� ��� ����������� ��������� ���������� ������
// ��� ���������� ������ ��� ����������� �������, ������� ����� ������������, ��
// �� ����� ����������
#pragma once

// стандартные файлы для включения необходимых системных функций
#include <stdio.h>
#include <tchar.h>

// TODO:      ,   
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

// поддержка локализации и кодировок
#include <locale>
#include <codecvt>
#include <fcntl.h>
#include <io.h>

// настройка поддержки русского языка
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif

// Используем сборку DLL
#using "..\..\cursachDLL\Debug\cursachDLL.dll"
using namespace cursachDLL;
