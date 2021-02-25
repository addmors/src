#pragma once
#include "../Shape/Shape.h"


struct Manifold;

typedef void(*CollisionCallback)(Manifold *m, Shape *a, Shape *b);
extern CollisionCallback Disp[Type::typeCount][Type::typeCount];
void PolygontoPolygon(Manifold *m, Shape *a, Shape *b);
void CircletoCircle(Manifold *m, Shape *a, Shape *b);
void CircletoPolygon(Manifold *m, Shape *a, Shape *b);
void PolygontoCircle(Manifold *m, Shape *a, Shape *b);



