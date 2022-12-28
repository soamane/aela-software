/*coded by https://github.com/soamane*/

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _HAS_AUTO_PTR_ETC 1

#define RELEASE_WEAPON_PARAMS(offset, function, status) offset->SetValue(function,status)

extern "C" int _fltused = 0;

#include <windows.h>
#include <iostream>
#include <assert.h>
#include <DXGI.h>
#include <tchar.h>
using namespace std;

#include "binding.hpp"
#include "math.h"
Vec3_tpl<float> screen = typeZero::ZERO;
int bones[] = { 35,36,40,63, 28,42,65,26, 25, 24, 1, 6, 5, 17, 10, 11, 18 };

char name_health[32] = " HP"; char health[32];
char name_armor[32] = " ARM"; char armor[32];
char name_distance[32] = " M"; char distance_[32];
char name_enemy[32]; char enemy[32];
const char* class_name;


