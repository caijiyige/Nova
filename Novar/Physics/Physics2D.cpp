#include "Novar/Core/Base.h"
#include "Novar/Core/TimeStep.h"
#include "Novar/Physics/Physics2D.h"

#include "box2d/base.h"
#include "box2d/box2d.h"
#include "box2d/collision.h"
#include "box2d/id.h"
#include "box2d/types.h"
#include "box2d/math_functions.h"

namespace NV
{
	
	b2BodyType RigidBodyTypeToBox2D(Rigidbody2DComponent::BodyType bodyTyep)
	{
		switch (bodyTyep)
		{
			case Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
			default: 
			{
				NV_CORE_ASSERT(false, "Unknown bodyType");
				return b2_staticBody;
			}
		}
	};


	Physics2D::Physics2D()
	:m_pBody(nullptr)
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = (b2Vec2){0.0f, -9.8f};
		
		m_spPhysicsWorldId = b2CreateWorld(&worldDef);

	}

	Physics2D::~Physics2D()
	{
		b2DestroyWorld(m_spPhysicsWorldId);
	}

	void Physics2D::CreateBody(Rigidbody2DComponent& rigidBody2D, const TransformComponent& transform)
	{
		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = RigidBodyTypeToBox2D(rigidBody2D.Type);
		bodyDef.position= (b2Vec2){transform.Translation.x, transform.Translation.y};
		auto bian = sqrt(transform.Rotation.x*transform.Rotation.x+transform.Rotation.y*transform.Rotation.y);
		bodyDef.rotation =(b2Rot){cos(transform.Rotation.x/bian),sin(transform.Rotation.y/bian)};

		boxid = b2CreateBody(m_spPhysicsWorldId, &bodyDef);

	}

	void Physics2D::CreatePolygonShape(BoxCollider2DComponent& boxCollider2D, const TransformComponent& transform)
	{
		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = boxCollider2D.Density;

		shapeDef.material.friction = boxCollider2D.Friction;
		shapeDef.material.restitution = boxCollider2D.Restitution;

		b2Polygon box = b2MakeBox(boxCollider2D.Size.x, boxCollider2D.Size.y);
		b2CreatePolygonShape(boxid, &shapeDef, &box);
	}

	void Physics2D::OnUpdate(const Timestep& timeStep)
	{
		b2World_Step(m_spPhysicsWorldId, (float)timeStep, 1.0);
		
	}

	void Physics2D::UpdateSystem(Rigidbody2DComponent& rigidBody2D, TransformComponent& transform)
	{
		auto Body = (b2BodyDef*)rigidBody2D.RuntimeBody;
		transform.Translation.x = Body->position.x;
		transform.Translation.y = Body->position.y;
		
	}
}
