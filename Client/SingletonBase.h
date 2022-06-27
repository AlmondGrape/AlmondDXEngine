#pragma once
#include <cassert> // c++ 판 assert

#define DECLARE_SINGLETON_CLASS(TClass) \
	public:	\
		static TClass SingletonInstance; \
	private: \
		TClass() = default; \
		~TClass() = default; \
		TClass(const TClass& instance) = default; \
		TClass& operator=(const TClass& instance) = delete;

#define IMPLEMENT_SINGLETON_CLASS(TClass) \
	TClass TClass::SingletonInstance;

/// <summary>
/// 2022/04/04 Nut
/// </summary>
namespace ProjectN
{
	// 템플릿 클래스 구조..
	// template<typename 타입이름1, typename 타입이름2, ...>	
	// class 클래스명 {	
	// 		클래스 멤버;	
	//	};
	template <class TClass>
	class SingletonInstanceBase
	{
	public:
		SingletonInstanceBase();
		virtual ~SingletonInstanceBase();

		static TClass* GetInstance();

	protected:
		static TClass* SingletonInstance;
	};

	template <class TClass>
	SingletonInstanceBase<TClass>::SingletonInstanceBase()
	{
		if (SingletonInstance != nullptr)
		{
			assert(true);
		}
		SingletonInstance = static_cast<TClass*>(this);
	}

	template <class TClass>
	SingletonInstanceBase<TClass>::~SingletonInstanceBase()
	{
		// 싱글턴 인스턴스를 지워준다. (delete 안해도 되나?..)
		delete SingletonInstance;
		SingletonInstance = nullptr;
	}

	template <class TClass>
	TClass* SingletonInstanceBase<TClass>::GetInstance()
	{
		return SingletonInstance;
	}

	template <class TClass>
	TClass* SingletonInstanceBase<TClass>::SingletonInstance = nullptr;

}
