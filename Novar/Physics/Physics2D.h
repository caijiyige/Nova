#pragma once
#include "Novar/Scene/Components.h"

#include "box2d/id.h"

namespace NV
{
	
	class Timestep;
	class Physics2D
	{
	public:
		Physics2D();
		~Physics2D();
		void CreateBody(Rigidbody2DComponent& rigidBody2D, const TransformComponent& transform);
		void CreatePolygonShape(BoxCollider2DComponent& boxCollider2D, const TransformComponent& transform);
		

		void OnUpdate(const Timestep& timeStep);
		void UpdateSystem(Rigidbody2DComponent& rigidBody2D, TransformComponent& transform);
	private:
		b2WorldId m_spPhysicsWorldId;
		b2BodyId* m_pBody;//实体
		b2BodyId boxid;
		
		b2ShapeId m_pShape;//形状
	};

}


