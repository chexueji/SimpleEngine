#pragma once

#include "resource.h"
#include "vec.h"
#include "mat.h"
#include "camera.h"
#include "triangle.h"
#include "scene.h"
#include "light.h"
#include "material.h"

#define GetR(c)	((BYTE)((c) & 0x0000FF))
#define GetG(c)	((BYTE)(((c) & 0x00FF00) >> 8))	
#define GetB(c)	((BYTE)(((c) & 0xFF0000) >> 16))	
	

