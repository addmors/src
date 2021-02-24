#include"Sceen.h"


void IntegrateForces(Shape *s, float dt)
{
	if (s->invesMass == 0.0f) return;

	s->velocity += (s->force * s->invesMass + gravity) * (dt / 2.0f);
	s->angularVelocity += s->torque * s->inversInertial * (dt / 2.0f);

}

void IntegrateVelocity(Shape * s, float dt)
{
	if (s->invesMass == 0.0f) return;

	s->position += s->velocity * dt;
	s->orient += s->angularVelocity * dt;
	s->SetOrient(s->orient);
	IntegrateForces(s, dt);
}

void Scene::Step(void)
{
	contacts.clear();
	for (int i = 0; i < shapes.size(); ++i){
		Shape *A = shapes[i];
		for (int j = i + 1; j < shapes.size(); ++j)
		{
			Shape *B = shapes[j];
			if (A->joint == B) continue;
			if (A->invesMass == 0 && B->invesMass == 0) continue;
			if ((A->length + B->length+3) < glm::length(A->position - B->position)) continue;
			Manifold m(A, B);
			m.Solve();
			if (m.contact_count) contacts.push_back(m);
		}
	}

	for (int i = 0; i < shapes.size(); ++i)
		IntegrateForces(shapes[i], m_dt);

	 for(int i = 0; i < contacts.size( ); ++i)
			contacts[i].Initialize( );

	 for (int j = 0; j < m_iterations; ++j) {
		 for (int i = 0; i < contacts.size(); ++i)
			 contacts[i].ApplyImpulse();
		 for (auto joint : joints)
			 joint.ApplyImpulse(); 
		 for (int i = 0; i < contacts.size(); ++i)
			 contacts[i].ApplyImpulse();
	 }

	for (int i = 0; i < shapes.size(); ++i)
		 IntegrateVelocity(shapes[i], m_dt);

	for (auto joint : joints)
		joint.PositionalCorrection();

	 for (int i = 0; i < contacts.size(); ++i)
		 contacts[i].PositionalCorrection();

	 for (int i = 0; i < shapes.size(); ++i)
	 {
		 shapes[i]->force = {0, 0};
		 shapes[i]->torque =0;
	 }
}

void Scene::Add(Shape * shape) {shapes.push_back(shape);}

void Scene::AddJoint(Joint& joint) {joints.push_back(joint);}
