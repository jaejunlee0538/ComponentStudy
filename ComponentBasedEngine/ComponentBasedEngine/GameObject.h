#pragma once
#include "Component.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	virtual void Render() const;
	virtual void Update(bool isDirty);

	void SetTag(const char* tag);
	const std::string& GetTag() const;
	bool CompareTag(const char* tag) const;

	void SetScale(float sx, float sy, float sz);
	void SetScale(const D3DXVECTOR3& scale);
	void SetPosition(float x, float y, float z);
	void SetPosition(const D3DXVECTOR3& pos);
	void SetRotation(float angleX, float angleY, float angleZ);
	void SetRotation(const D3DXVECTOR3& axis, float angle);
	void SetRotation(const D3DXQUATERNION& quat);

	void SetSpeed(float vx, float vy, float vz);

	void SetTransform(const D3DXMATRIX* transform);

	const D3DXVECTOR3& GetLocalPosition() const { return _position; }
	const D3DXQUATERNION& GetLocalRotation() const { return _quaternion; }

	D3DXVECTOR3 GetWorldPosition() const;
	D3DXQUATERNION GetWorldRotation() const;

	//�θ� ��ǥ�踦 �������� ������Ʈ�� �̵���Ų��.
	void Translate(float dx, float dy, float dz);
	//Local ��ǥ�踦 �������� ������Ʈ�� �̵���Ų��.
	void TranslateLocal(float dx, float dy, float dz);
	//�θ� ��ǥ���� ���� �������� ȸ���Ѵ�.
	void Rotate(float rotAngleX, float rotAngleY, float rotAngleZ);
	//Local ��ǥ���� ���� �������� ȸ���Ѵ�.
	void RotateLocal(float rotAngleX, float rotAngleY, float rotAngleZ);

	void CalculateWorldMatrix();
	const D3DXMATRIX& GetWorldMatrix() const { return _matWorld; }

	size_t GetNObjects() const;

	GameObject* GetParent() { return _parent; }
	bool HasParent() const;
	bool HasChild() const;
	void InsertChild(GameObject* child);

	void InsertChild(const char* parentTag, GameObject* child);
	GameObject* FindObject(const char* tag);
	GameObject* RemoveObject(const char* tag);
	bool DeleteObject(const char* tag);
	void ClearChilds();

	template <typename ComponentT>
	ComponentT* GetComponentByName(std::string componentName);

	void SendMessageLocal(Message message);
	void SendMessageGlobal(Message message);
private:
	GameObject* _parent = nullptr;
	std::list<GameObject*> _childs;

	std::string _tag;

	//_parent�� null�� �ƴ� ��쿡�� �θ� ���� ��� ��ġ
	//_parent�� null�� ��쿡�� world�󿡼��� ��ġ
	D3DXVECTOR3 _position;
	D3DXQUATERNION _quaternion;

	std::map<std::string, Component*> m_components;

	//world���
	bool _isDirty = true;
	D3DXMATRIX _matWorld;
	D3DXMATRIX _matScale;
};

template<typename ComponentT>
ComponentT * GameObject::GetComponentByName(std::string componentName)
{
	auto it = m_components.find(componentName);
	if (it != m_components.end()) {
		assert(dynamic_cast<ComponentT*>(it->second) != nullptr);
		return static_cast<ComponentT*>(it->second);
	}
	return nullptr;
}
