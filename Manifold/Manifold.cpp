#include "Manifold.h"


void Manifold::Solve() {

	Disp[A->GetType()][B->GetType()](this, A, B);
}

void Manifold::Initialize(void) {
	for (int i = 0; i < contact_count; i++) {
		//Проникновение для A shape;
		//glm::vec2 ra = contacts[i] - A->position;
		//Проникновение для B shape;
		//glm::vec2 rb = contacts[i] - B->position;
		//Относительная скорость
		glm::vec2 rv = B->velocity + CrossMy(B->angularVelocity, contacts[i] - B->position) -
			A->velocity - CrossMy(A->angularVelocity, contacts[i] - A->position);
		if (glm::length(rv) < glm::length(dt * gravity) + 0.0001f) {
			e = 0.0f; 
			return;
		}
	}
};

void Manifold::ApplyImpulse(){

	if (Equal(A->invesMass + B->invesMass, 0))
	{   InfiniteMassCorrection();
		return;
	}
	for (int i = 0; i < contact_count; ++i)
	{
		// Calculate radii from COM to contact
		glm::vec2 ra = contacts[i] - A->position;
		//Проникновение для B shape;
		glm::vec2 rb = contacts[i] - B->position;

		//Относительная скорость
		glm::vec2 rv = B->velocity + CrossMy(B->angularVelocity, rb) - A->velocity - CrossMy(A->angularVelocity, ra);
		//Скорость по нормали
		float contactVel = glm::dot(rv, normal);
		//Если скорость по нормали больше 0 то она ращделяет обьекты

		if (contactVel >= 0) return;

		//Действующая масса
			float raCrossN = CrossMy(ra, normal);
		float rbCrossN = CrossMy(rb, normal);

		float invMassSum = A->invesMass + B->invesMass + raCrossN*raCrossN * A->inversInertial + rbCrossN * rbCrossN * B->inversInertial;
		//Импульс
		
		float j = -(1.0f + e) * contactVel / (invMassSum*contact_count);
		//Приминение импульса 
		glm::vec2 impulse = normal * j;
		A->ApplyImpulse(-impulse, ra);
		B->ApplyImpulse(impulse, rb);
		//Импульсы трения
		rv = B->velocity + CrossMy(B->angularVelocity, rb) -
			A->velocity - CrossMy(A->angularVelocity, ra);
		glm::vec2 t = glm::normalize(rv - (normal * glm::dot(rv, normal)));
		
		if (t!=t) return;
		float jt = -glm::dot(rv, t) / (invMassSum * contact_count);
		//Если импульс малл 
		if (Equal(jt, 0.0f))
			return;
		
		//Закон Кулона
		glm::vec2 tangentImpulse;
		if (std::abs(jt) < j * sf) tangentImpulse = t * jt;
		else tangentImpulse = t * -j * df;
		A->ApplyImpulse(-tangentImpulse, ra);
		B->ApplyImpulse(tangentImpulse, rb);
	}
}

void Manifold::PositionalCorrection()
{
	glm::vec2 correction = (std::max(penetration - K_SLOP, 0.0f) / (A->invesMass + B->invesMass)) * normal * PERCENT;
	A->position -= correction * A->invesMass;
	B->position += correction * B->invesMass;
}

void Manifold::InfiniteMassCorrection()
{
	A->velocity = { 0, 0 };
	B->velocity = { 0, 0 };
}