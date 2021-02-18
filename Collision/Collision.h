#pragma once
#include "../Shape/Shape.h"
#include "../Manifold/Manifold.h"

#include "../Math.h"

struct Manifold;
class Shape;
typedef void(*CollisionCallback)(Manifold *m, Shape *a, Shape *b);
extern CollisionCallback Disp[Type::typeCount][Type::typeCount];
void PolygontoPolygon(Manifold *m, Shape *a, Shape *b);
void CircletoCircle(Manifold *m, Shape *a, Shape *b);
void CircletoPolygon(Manifold *m, Shape *a, Shape *b);
void PolygontoCircle(Manifold *m, Shape *a, Shape *b);



