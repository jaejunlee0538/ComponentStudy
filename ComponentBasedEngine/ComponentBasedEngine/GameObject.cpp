#include "stdafx.h"
#include "GameObject.h"
#include <algorithm>

GameObject::GameObject()
{
	SetScale(1, 1, 1);
	SetPosition(0, 0, 0);
	SetRotation(0, 0, 0);
}

GameObject::~GameObject()
{
	ClearChilds();
}

void GameObject::Render() const
{
	//모든 자식을 그린다.
	for each(auto pChild in _childs) {
		pChild->Render();
	}
}

void GameObject::Update(bool isDirty)
{
	isDirty |= _isDirty;
	if (isDirty) {
		CalculateWorldMatrix();
		_isDirty = false;
	}

	//모든 자식을 업데이트한다.
	for each(auto pChild in _childs) {
		pChild->Update(isDirty);
	}
}

void GameObject::SetTag(const char * tag)
{
	_tag = tag;
}

const std::string & GameObject::GetTag() const
{
	return _tag;
}

bool GameObject::CompareTag(const char * tag) const
{
	return _tag.compare(tag) == 0;
}

void GameObject::SetScale(float sx, float sy, float sz)
{
	D3DXMatrixScaling(&_matScale, sx, sy, sz);
}

void GameObject::SetScale(const D3DXVECTOR3 & scale)
{
	SetScale(scale.x, scale.y, scale.z);
}

void GameObject::CalculateWorldMatrix()
{
	D3DXMATRIX matRot;
	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation(&matTranslation, _position.x, _position.y, _position.z);
	D3DXMatrixRotationQuaternion(&matRot, &_quaternion);
	_matWorld = matRot *  matTranslation;

	if (_parent) {
		//부모가 있는 경우 부모의 world행렬을 곱한다.
		_matWorld = _matWorld * _parent->GetWorldMatrix();
	}
	else {
		_matWorld = _matScale * _matWorld;
	}
}

size_t GameObject::GetNObjects() const
{
	size_t count = 1;
	for each(auto pChild in _childs) {
		count += pChild->GetNObjects();
	}
	return count;
}

bool GameObject::HasParent() const
{
	return _parent != nullptr;
}

bool GameObject::HasChild() const
{
	return _childs.empty() == false;
}

void GameObject::InsertChild(GameObject * child)
{
	assert(child != nullptr);
	//head에 새로운 자식을 삽입한다.
	_childs.push_front(child);
	child->_isDirty = true;
	child->_parent = this;
}

void GameObject::InsertChild(const char * parentTag, GameObject * child)
{
	GameObject* parent = FindObject(parentTag);
	if (parent) {
		parent->InsertChild(child);
	}
	else {
		delete child;
	}
}

GameObject * GameObject::FindObject(const char * tag)
{
	if (_tag.compare(tag) == 0) {
		return this;
	}
	for each(auto pChild in _childs) {
		GameObject* obj = pChild->FindObject(tag);
		if (obj) return obj;
	}
	return nullptr;
}

GameObject * GameObject::RemoveObject(const char * tag)
{
	auto it = std::find_if(_childs.begin(), _childs.end(), [&tag](GameObject* obj) {return obj->CompareTag(tag); });
	if (it != _childs.end()) {
		_childs.erase(it);
		return *it;
	}
	for each(auto pChild in _childs) {
		GameObject* obj = pChild->RemoveObject(tag);
		if (obj) return obj;
	}
	return nullptr;
}

bool GameObject::DeleteObject(const char * tag)
{
	auto it = std::find_if(_childs.begin(), _childs.end(), [&tag](GameObject* obj) {return obj->CompareTag(tag); });
	if (it != _childs.end()) {
		_childs.erase(it);
		delete *it;
		return true;
	}
	for each(auto pChild in _childs) {
		if (pChild->DeleteObject(tag)) return true;
	}
	return false;
}

void GameObject::ClearChilds()
{
	for each(auto pChild in _childs) {
		pChild->ClearChilds();
		delete pChild;
	}
	_childs.clear();
}

void GameObject::SendMessageLocal(Message message)
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it) {
		it->second->HandleMessage(message);
	}
}

void GameObject::SendMessageGlobal(Message message)
{
	assert(false);//TODO : 
}

void GameObject::SetPosition(float x, float y, float z)
{
	_position = D3DXVECTOR3(x, y, z);
	_isDirty = true;
}

void GameObject::SetPosition(const D3DXVECTOR3 & pos)
{
	SetPosition(pos.x, pos.y, pos.z);
}

void GameObject::SetRotation(float angleX, float angleY, float angleZ)
{
	D3DXQuaternionRotationYawPitchRoll(&_quaternion, angleY, angleX, angleZ);
	_isDirty = true;
}

void GameObject::SetRotation(const D3DXVECTOR3 & axis, float angle)
{
	D3DXQuaternionRotationAxis(&_quaternion, &axis, angle);
	_isDirty = true;
}

void GameObject::SetRotation(const D3DXQUATERNION & quat)
{
	_quaternion = quat;
	_isDirty = true;
}

void GameObject::SetTransform(const D3DXMATRIX * transform)
{
	_position.x = (*transform)(3, 0);
	_position.y = (*transform)(3, 1);
	_position.z = (*transform)(3, 2);
	D3DXQuaternionRotationMatrix(&_quaternion, transform);
	_isDirty = true;
}

D3DXVECTOR3 GameObject::GetWorldPosition() const
{
	return D3DXVECTOR3(_matWorld(3, 0), _matWorld(3, 1), _matWorld(3, 2));
}

D3DXQUATERNION GameObject::GetWorldRotation() const
{
	D3DXQUATERNION quat;
	D3DXQuaternionRotationMatrix(&quat, &_matWorld);
	return quat;
}

void GameObject::Translate(float dx, float dy, float dz)
{
	_position = _position + D3DXVECTOR3(dx, dy, dz);
	_isDirty = true;
}

void GameObject::TranslateLocal(float dx, float dy, float dz)
{
	D3DXVECTOR3 displace(dx, dy, dz);
	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &_quaternion);
	D3DXVec3TransformNormal(&displace, &displace, &rot);
	_position = _position + displace;
	_isDirty = true;
}

void GameObject::Rotate(float rotAngleX, float rotAngleY, float rotAngleZ)
{
	D3DXMATRIX rot;
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, rotAngleY, rotAngleX, rotAngleZ);
	D3DXMatrixRotationQuaternion(&rot, &quatRot);
	D3DXVec3TransformNormal(&_position, &_position, &rot);
	_quaternion = _quaternion * quatRot;
	_isDirty = true;
}

void GameObject::RotateLocal(float rotAngleX, float rotAngleY, float rotAngleZ)
{
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, rotAngleY, rotAngleX, rotAngleZ);
	_quaternion = _quaternion * quatRot;

	_isDirty = true;
}