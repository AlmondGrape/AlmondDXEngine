#pragma once
#include <cassert> // c++ �� assert

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
	// ���ø� Ŭ���� ����..
	// template<typename Ÿ���̸�1, typename Ÿ���̸�2, ...>	
	// class Ŭ������ {	
	// 		Ŭ���� ���;	
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
		// �̱��� �ν��Ͻ��� �����ش�. (delete ���ص� �ǳ�?..)
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
