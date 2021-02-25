#pragma once


#ifdef WIN32
#include <Windows.h>
#endif

#undef min
#undef max

#include <cstring> // strlen, memcpy, etc.
#include <cstdlib> // exit
#include <cfloat>  // FLT_MAX
#include <vector>

#include "Math.h"
#include "Time.h"
#include "../Body/Body.h"
#include "../Shape/Shape.h"
#include "../Collision/Collision.h"
#include "../Manifold/Manifold.h"
#include "../SceenPhis/Sceen.h"
#include "../Window/Window.h"

