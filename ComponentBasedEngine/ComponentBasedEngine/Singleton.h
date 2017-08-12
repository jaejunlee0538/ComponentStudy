#pragma once
template <typename T>
class Singleton
{
private:
	//복사, 대입 연산 방지
	Singleton(const Singleton<T>&){}
	Singleton& operator=(const Singleton<T>&){}
public:
	Singleton();
	~Singleton();

	static T& GetSingleton();
protected:
	static T* m_pSingleton;
};

template <typename T>
T* Singleton<T>::m_pSingleton = nullptr;

template<typename T>
Singleton<T>::Singleton()
{
	assert(m_pSingleton == nullptr && "싱글턴 객체는 하나만 생성될 수 있습니다");
	m_pSingleton = static_cast<T*>(this);
}

template<typename T>
Singleton<T>::~Singleton()
{
	m_pSingleton = nullptr;
}

template<typename T>
T & Singleton<T>::GetSingleton()
{
	assert(m_pSingleton != nullptr);
	return *m_pSingleton;
}
