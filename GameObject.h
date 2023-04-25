#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <directxmath.h>
#include <d3d11_4.h>
#include <string>
#include "Vector3.h"

#include "Structures.h"
#include "Transform.h"
#include "Appearance.h"
#include "RigidBodyModel.h"

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject(string type, Appearance* appearance, Transform* transform, PhysicsModel* physicsModel);
	~GameObject();

	Appearance* GetAppearance() const { return m_appearance; }
	Transform* GetTransform() const { return m_transform; }
	PhysicsModel* GetPhysicsModel() const { return m_physicsModel; }

	string GetType() const { return m_type; }

	void Update(float t);
	void Draw(ID3D11DeviceContext * pImmediateContext);

private:
	Transform* m_transform;
	Appearance* m_appearance;
	PhysicsModel* m_physicsModel;
	
	string m_type;
};

#endif // !GAME_OBJECT_H