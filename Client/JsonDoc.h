#pragma once
#include "./rapidjson/document.h"			// rapidjson's DOM-style API 

// Todo : 파싱함수 템플릿 일반화 발전.

namespace ProjectN
{
	class JsonDoc
	{
	private:
		FILE* fp_ = nullptr;
		rapidjson::Document m_document;
		std::map<int, rapidjson::Value*> m_pDocumentObjects;

	public:
		JsonDoc() {};
		virtual ~JsonDoc() {};
		JsonDoc(const JsonDoc& other) = default;
		JsonDoc& operator =(const JsonDoc& other) = default;
		JsonDoc(JsonDoc&& other) = default;

		// Json 파일을 읽는 함수
		bool Read(std::string const& file);
		// Json 파일을 쓰는 함수
		bool Write(std::string const& file);

		// Json document 객체 반환함수	
		rapidjson::Document& GetDocument();
		int GetDocumentSheetSize();

		// Json document 스프레트시트형의 객체 반환함수 (예외없으니, 꼭 ID포함된 스프레드시트형 문서에만 사용할 것)
		rapidjson::Value& GetDocumentObject(int objectID);

		/// Json 스프레트 시트에서 ID와 키값을 통해 값을 리턴받는 함수
		// const char*의 문자열을 유니코드의 wstring으로 리턴함. 인코딩 비용있으므로 update에서 사용하지 말것.
		std::wstring GetUnicodeStrByID(int objectID, std::string const& keystring);
		std::wstring GetArrayObjUnicodeStrByID(int objectID, std::string const& keystring, int index);

		// 이건 index와 키값으로 입력 받는 것..
		std::wstring GetUnicodeStrByIndex(int index, std::string const& keystring);
		std::wstring GetArrayObjUnicodeStrByIndex(int index, std::string const& keystring, int secondindex);

		// 오브젝트 안에 키값이 있는가 체크하는 함수
		bool IsKeyExistByID(int objectID, std::string const& keystring);
		bool IsKeyExistByIndex(int index, std::string const& keystring);

		// 그대로 const char* 문자열을 반환. 영문자 및 숫자 스트링은 처리없이 빠르게 이걸 사용할 것
		const char* GetMultibyteStrByID(int objectID, std::string const& keystring);
		const char* GetArrayObjMultibyteStrByID(int objectID, std::string const& keystring, int index);

		const char* GetMultibyteStrByIndex(int index, std::string const& keystring);
		const char* GetArrayObjMultibyteStrByIndex(int index, std::string const& keystring, int keyindex);

		int GetArrayObjSizeByID(int objectID, std::string const& keystring);
		int GetArrayObjSizeByIndex(int index, std::string const& keystring);

		// 단순한 수치 리턴은 템플릿형 함수. 이건 헤더에 모두 기입	
		template <typename T>
		T GetNumNBoolByID(int objectID, std::string const& keystring)
		{
			if (m_document.IsArray() == true)
			{
				// 스프레트시트형 json으로 인식, map에서 ID에 해당하는 값을 리턴함
				rapidjson::Value::ConstMemberIterator itr = m_pDocumentObjects[objectID]->FindMember(keystring.c_str());
				if (itr != m_pDocumentObjects[objectID]->MemberEnd())
				{
					if (std::is_same<int, T>::value)
						return (T)itr->value.GetInt();
					else if (std::is_same<float, T>::value)
						return (T)itr->value.GetFloat();
					else if (std::is_same<bool, T>::value)
						return (T)itr->value.GetBool();
					else if (std::is_same<double, T>::value)
						return (T)itr->value.GetDouble();
				}
				else
					return NULL;
			}
			else
			{
				// 정상적인 DOM구조 json. 다큐먼트에서 바로 값을 찾아 리턴
				if (std::is_same<int, T>::value)
					return (T)m_document[keystring.c_str()].GetInt();
				else if (std::is_same<float, T>::value)
					return (T)m_document[keystring.c_str()].GetFloat();
				else if (std::is_same<bool, T>::value)
					return (T)m_document[keystring.c_str()].GetBool();
				else if (std::is_same<double, T>::value)
					return (T)m_document[keystring.c_str()].GetDouble();
				else
					return NULL;
			}
		}

		// 이건 key의 value가 array형태일 때 index값도 인자로 넣어, 값을 반환함
		template <typename T>
		T GetArrayObjNumNBoolByID(int objectID, std::string const& keystring, int index)
		{
			if (m_document.IsArray() == true)
			{
				// 스프레트시트형 json으로 인식, map에서 ID에 해당하는 값을 리턴함
				rapidjson::Value::ConstMemberIterator itr = m_pDocumentObjects[objectID]->FindMember(keystring.c_str());
				if (itr != m_pDocumentObjects[objectID]->MemberEnd())
				{
					if (std::is_same<int, T>::value)
						return (T)itr->value[index].GetInt();
					else if (std::is_same<float, T>::value)
						return (T)itr->value[index].GetFloat();
					else if (std::is_same<bool, T>::value)
						return (T)itr->value[index].GetBool();
					else if (std::is_same<double, T>::value)
						return (T)itr->value[index].GetDouble();
				}
				else
					return NULL;
			}
			else
			{
				// 정상적인 DOM구조 json. 다큐먼트에서 바로 값을 찾아 리턴
				if (std::is_same<int, T>::value)
					return (T)m_document[keystring.c_str()][index].GetInt();
				else if (std::is_same<float, T>::value)
					return (T)m_document[keystring.c_str()][index].GetFloat();
				else if (std::is_same<bool, T>::value)
					return (T)m_document[keystring.c_str()][index].GetBool();
				else if (std::is_same<double, T>::value)
					return (T)m_document[keystring.c_str()][index].GetDouble();
				else
					return NULL;
			}

		}

		template <typename T>
		T GetNumNBoolByIndex(int index, std::string const& keystring)
		{
			if (m_document.IsArray() == true)
			{
				// 다큐먼트가 array라면..
				if (std::is_same<int, T>::value)
					return (T)m_document[index][keystring.c_str()].GetInt();
				else if (std::is_same<float, T>::value)
					return (T)m_document[index][keystring.c_str()].GetFloat();
				else if (std::is_same<bool, T>::value)
					return (T)m_document[index][keystring.c_str()].GetBool();
				else if (std::is_same<double, T>::value)
					return (T)m_document[index][keystring.c_str()].GetDouble();
				else
					return NULL;

			}
			else
			{
				if (std::is_same<int, T>::value)
					return (T)m_document[keystring.c_str()].GetInt();
				else if (std::is_same<float, T>::value)
					return (T)m_document[keystring.c_str()].GetFloat();
				else if (std::is_same<bool, T>::value)
					return (T)m_document[keystring.c_str()].GetBool();
				else if (std::is_same<double, T>::value)
					return (T)m_document[keystring.c_str()].GetDouble();
				else
					return NULL;
			}
		}

		template <typename T>
		T GetArrayObjNumNBoolByIndex(int index, std::string const& keystring, int keyindex)
		{
			if (m_document.IsArray() == true)
			{
				if (std::is_same<int, T>::value)
					return (T)m_document[index][keystring.c_str()][keyindex].GetInt();
				else if (std::is_same<float, T>::value)
					return (T)m_document[index][keystring.c_str()][keyindex].GetFloat();
				else if (std::is_same<bool, T>::value)
					return (T)m_document[index][keystring.c_str()][keyindex].GetBool();
				else if (std::is_same<double, T>::value)
					return (T)m_document[index][keystring.c_str()][keyindex].GetDouble();
				else
					return NULL;
			}
			else
			{
				if (std::is_same<int, T>::value)
					return (T)m_document[keystring.c_str()][keyindex].GetInt();
				else if (std::is_same<float, T>::value)
					return (T)m_document[keystring.c_str()][keyindex].GetFloat();
				else if (std::is_same<bool, T>::value)
					return (T)m_document[keystring.c_str()][keyindex].GetBool();
				else if (std::is_same<double, T>::value)
					return (T)m_document[keystring.c_str()][keyindex].GetDouble();
				else
					return NULL;
			}
		}

		template <typename T>
		T GetDoubleArrayObjNumNBoolByID(int objectID, std::string const& keystring, int index, int secondIndex)
		{
			if (m_document.IsArray() == true)
			{
				// 스프레트시트형 json으로 인식, map에서 ID에 해당하는 값을 리턴함
				rapidjson::Value::ConstMemberIterator itr = m_pDocumentObjects[objectID]->FindMember(keystring.c_str());
				if (itr != m_pDocumentObjects[objectID]->MemberEnd())
				{
					if (std::is_same<int, T>::value)
						return (T)itr->value[index][secondIndex].GetInt();
					else if (std::is_same<float, T>::value)
						return (T)itr->value[index][secondIndex].GetFloat();
					else if (std::is_same<bool, T>::value)
						return (T)itr->value[index][secondIndex].GetBool();
					else if (std::is_same<double, T>::value)
						return (T)itr->value[index][secondIndex].GetDouble();
					else
						return NULL;
				}
			}
			else
			{
				// 정상적인 DOM구조 json. 다큐먼트에서 바로 값을 찾아 리턴
				if (std::is_same<int, T>::value)
					return (T)m_document[keystring.c_str()][index][secondIndex].GetInt();
				else if (std::is_same<float, T>::value)
					return (T)m_document[keystring.c_str()][index][secondIndex].GetFloat();
				else if (std::is_same<bool, T>::value)
					return (T)m_document[keystring.c_str()][index][secondIndex].GetBool();
				else if (std::is_same<double, T>::value)
					return (T)m_document[keystring.c_str()][index][secondIndex].GetDouble();
				else
					return NULL;
			}

		}

	};
}
