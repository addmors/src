#include "../Precompiled.h"


CollisionCallback Disp[Type::typeCount][Type::typeCount] =
{
  {CircletoCircle, CircletoPolygon},
  {PolygontoCircle, PolygontoPolygon}
};


void FindIncidentFace(glm::vec2 *v, Polygon *RefPoly, Polygon *IncPoly, int referenceIndex) {
	glm::vec2 referenceNormal = RefPoly->normals[referenceIndex];

	// Calculate normal in incident's frame of reference
	referenceNormal = RefPoly->u * referenceNormal; // To world space
	referenceNormal = glm::transpose(IncPoly->u) * referenceNormal; // To incident's model space

	// Find most anti-normal face on incident polygon
	unsigned int incidentFace = 1;
	float minDot = FLT_MAX;
	for (unsigned int i = 0; i < IncPoly->count_vertex; ++i)
	{
		float dot = glm::dot(referenceNormal, IncPoly->normals[i]);
		if (dot < minDot)
		{
			minDot = dot;
			incidentFace = i;
		}
	}

	// Assign face vertices for incidentFace
	v[0] = IncPoly->u * IncPoly->vertices[incidentFace].Pos + IncPoly->body->position;
	incidentFace = incidentFace + 1 >= IncPoly->count_vertex ? 0 : incidentFace + 1;
	v[1] = IncPoly->u * IncPoly->vertices[incidentFace].Pos + IncPoly->body->position;
}




float FindAxisLeastPenetration(int *faceIndex, Polygon *A, Polygon *B)
{
	float bestDistance = -FLT_MAX;
	int bestIndex;

	for (unsigned int i = 0; i < A->count_vertex; ++i)
	{
		//Получаю нормаль
		glm::vec2 n = A->normals[i];
		//Умножаю нормаль на угол поворота скина
		glm::vec2 nw = A->u * n;
		// Вращаю для модели относительно А;
		glm::mat2 buT = glm::transpose(B->u);
		n = buT * nw;

		// Retrieve support point from B along -n
		n = -n;
		glm::vec2 s = B->GetSupport(n);
		n = -n;
		// Retrieve vertex on face from A, transform into
		// B's model space
		glm::vec2 v = A->vertices[i].Pos;
		v = A->u * v + A->body->position;
		v -= B->body->position;
		v = buT * v;

		// Compute penetration distance (in B's model space)
		float d = glm::dot(n, s-v);

		// Store greatest distance
		if (d > bestDistance)
		{
			bestDistance = d;
			bestIndex = i;
		}
	}

	*faceIndex = bestIndex;
	return bestDistance;
};



int Clip(glm::vec2 n, float c, glm::vec2  *face)
{
	int sp = 0;
	glm::vec2 out[2] = {
	  face[0],
	  face[1]};

	// Retrieve distances from each endpoint to the line
	// d = ax + by - c
	float d1 = glm::dot(n, face[0]) - c;
	float d2 = glm::dot(n, face[1]) - c;

	// If negative (behind plane) clip
	if (d1 <= 0.0f) out[sp++] = face[0];
	if (d2 <= 0.0f) out[sp++] = face[1];

	// If the points are on different sides of the plane
	if (d1 * d2 < 0.0f) // less than to ignore -0.0f
	{
		// Push interesection point
		float alpha = d1 / (d1 - d2);
		out[sp] = face[0] + alpha * (face[1] - face[0]);
		++sp;
	}

	// Assign our new converted values
	face[0] = out[0];
	face[1] = out[1];

	assert(sp != 3);

	return sp;
};

void CircletoCircle(Manifold *m, Shape *a, Shape *b)
{
	Circle *A = dynamic_cast<Circle *>(a);
	Circle *B = dynamic_cast<Circle *>(b);
	glm::vec2 normal = a->body->position - b->body->position;
	double dist_sqr = normal.x*normal.x + normal.y*normal.y;
	float radius = A->radius + B->radius;

	if (dist_sqr >= radius * radius)
	{
		m->contact_count = 0;
		return;
	}
	float distance = (float)std::sqrt(dist_sqr);

	m->contact_count = 1;

	if (distance == 0.0f)
	{
		m->penetration = A->radius;
		m->normal = glm::vec2(1, 0);
		m->contacts[0] = a->body->position;
	} else 
	{
		m->penetration = radius - distance;
		m->normal = -normal / distance; // Faster than using Normalized since we already performed sqrt
		m->contacts[0] = m->normal * A->radius + a->body->position;
	}

};

void CircletoPolygon(Manifold *m, Shape *a, Shape *b)
{
	Circle *A = dynamic_cast<Circle *>(a);
	Polygon *B = dynamic_cast<Polygon *>(b);

	m->contact_count = 0;

	// Transform circle center to Polygon model space
	glm::vec2 center = a->body->position;
	center = glm::transpose(B->u)*(center - b->body->position);

	// Find edge with minimum penetration
	// Exact concept as using support points in Polygon vs Polygon
	float separation = -FLT_MAX;
	unsigned int faceNormal = 0;
	for (unsigned int i = 0; i < B->count_vertex; ++i)
	{
		float s = glm::dot(B->normals[i], center - b->vertices[i].Pos);

		if (s > A->radius)
			return;

		if (s > separation)
		{
			separation = s;
			faceNormal = i;
		}
	}

	// Grab face's vertices
	glm::vec2  v1 = B->vertices[faceNormal].Pos;
	int i2 = faceNormal + 1 < B->count_vertex ? faceNormal + 1 : 0;
	glm::vec2 v2 = B->vertices[i2].Pos;

	// Check to see if center is within polygon
	if (separation < EPSILON)
	{
		m->contact_count = 1;
		m->normal = -(B->u * B->normals[faceNormal]);
		m->contacts[0] = m->normal * A->radius + a->body->position;
		m->penetration = A->radius;
		return;
	}

	// Determine which voronoi region of the edge center of circle lies within
	float dot1 = glm::dot(center - v1, v2 - v1);
	float dot2 = glm::dot(center - v2, v1 - v2);
	m->penetration = A->radius - separation;

	// Closest to v1
	if (dot1 <= 0.0f)
	{
		if (DistSqr(center, v1) > A->radius * A->radius)
			return;

		m->contact_count = 1;
		glm::vec2 n = v1 - center;
		n = B->u * n;
		n = glm::normalize(n);
		m->normal = n;
		v1 = B->u * v1 + b->body->position;
		m->contacts[0] = v1;
	}

	// Closest to v2
	else if (dot2 <= 0.0f)
	{
		if (DistSqr(center, v2) > A->radius * A->radius)
			return;

		m->contact_count = 1;
		glm::vec2 n = v2 - center;
		v2 = B->u * v2 + b->body->position;
		m->contacts[0] = v2;
		n = B->u * n;
		n = glm::normalize(n);
		m->normal = n;
	}

	// Closest to face
	else
	{
		glm::vec2 n = B->normals[faceNormal];
		if (glm::dot(center - v1, n) > A->radius)
			return;

		n = B->u * n;
		m->normal = -n;
		m->contacts[0] = m->normal * A->radius + a->body->position;
		m->contact_count = 1;
	}
}

void PolygontoCircle(Manifold *m, Shape *a, Shape *b)
{
	CircletoPolygon(m, b, a);
	m->normal = -m->normal;
}

void PolygontoPolygon(Manifold * m, Shape * a, Shape * b)
{
	Polygon *A = dynamic_cast<Polygon *>(a);
	Polygon *B = dynamic_cast<Polygon *>(b);
	m->contact_count = 0;

	int faceA;
	float penetrationA = FindAxisLeastPenetration(&faceA, A, B);
	if (penetrationA >= 0.0f)
		return;

	int faceB;
	float penetrationB = FindAxisLeastPenetration(&faceB, B, A);
	if (penetrationB >= 0.0f)
		return;


	int referenceIndex;
	bool flip; // Always point from a to b

	Polygon *RefPoly; // Reference
	Polygon *IncPoly; // Incident

	if (BiasGreaterThan(penetrationA, penetrationB))
	{
		RefPoly = A;
		IncPoly = B;
		referenceIndex = faceA;
		flip = false;
	}

	else
	{
		RefPoly = B;
		IncPoly = A;
		referenceIndex = faceB;
		flip = true;
	}

	glm::vec2 incidentFace[2];
	FindIncidentFace(incidentFace, RefPoly, IncPoly, referenceIndex);


	// Setup reference face vertices
	glm::vec2 v1 = RefPoly->vertices[referenceIndex].Pos;
	referenceIndex = referenceIndex + 1 == RefPoly->count_vertex ? 0 : referenceIndex + 1;
	glm::vec2 v2 = RefPoly->vertices[referenceIndex].Pos;

	// Transform vertices to world space
	v1 = RefPoly->u * v1 + RefPoly->body->position;
	v2 = RefPoly->u * v2 + RefPoly->body->position;

	glm::vec2 sidePlaneNormal = glm::normalize(v2 - v1);

	// Orthogonalize
	glm::vec2 refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

	// ax + by = c
	// c is distance from origin
	float refC = glm::dot(refFaceNormal, v1);
	float negSide = -glm::dot(sidePlaneNormal, v1);
	float posSide = glm::dot(sidePlaneNormal, v2);

	// Clip incident face to reference face side planes
	if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
		return; // Due to floating point error, possible to not have required points

	if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
		return; // Due to floating point error, possible to not have required points

	m->normal = flip ? -refFaceNormal : refFaceNormal;


	int cp = 0; // clipped points behind reference face
	float separation = glm::dot(refFaceNormal, incidentFace[0]) - refC;
	if (separation <= 0.0f)
	{
		m->contacts[cp] = incidentFace[0];
		m->penetration = -separation;
		++cp;
	}
	else m->penetration = 0;

	separation = glm::dot(refFaceNormal, incidentFace[1]) - refC;
	if (separation <= 0.0f)
	{
		m->contacts[cp] = incidentFace[1];

		m->penetration += -separation;
		++cp;

		// Average penetration
		m->penetration /= (float)cp;
	}

	m->contact_count = cp;
}
