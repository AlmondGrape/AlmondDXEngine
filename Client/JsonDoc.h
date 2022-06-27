#pragma once
#include "./rapidjson/document.h"			// rapidjson's DOM-style API 

// Todo : �Ľ��Լ� ���ø� �Ϲ�ȭ ����.

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

		// Json ������ �д� �Լ�
		bool Read(std::string const& file);
		// Json ������ ���� �Լ�
		bool Write(std::string const& file);

		// Json document ��ü ��ȯ�Լ�	
		rapidjson::Document& GetDocument();
		int GetDocumentSheetSize();

		// Json document ������Ʈ��Ʈ���� ��ü ��ȯ�Լ� (���ܾ�����, �� ID���Ե� ���������Ʈ�� �������� ����� ��)
		rapidjson::Value& GetDocumentObject(int objectID);

		/// Json ������Ʈ ��Ʈ���� ID�� Ű���� ���� ���� ���Ϲ޴� �Լ�
		// const char*�� ���ڿ��� �����ڵ��� wstring���� ������. ���ڵ� ��������Ƿ� update���� ������� ����.
		std::wstring GetUnicodeStrByID(int objectID, std::string const& keystring);
		std::wstring GetArrayObjUnicodeStrByID(int objectID, std::string const& keystring, int index);

		// �̰� index�� Ű������ �Է� �޴� ��..
		std::wstring GetUnicodeStrByIndex(int index, std::string const& keystring);
		std::wstring GetArrayObjUnicodeStrByIndex(int index, std::string const& keystring, int secondindex);

		// ������Ʈ �ȿ� Ű���� �ִ°� üũ�ϴ� �Լ�
		bool IsKeyExistByID(int objectID, std::string const& keystring);
		bool IsKeyExistByIndex(int index, std::string const& keystring);

		// �״�� const char* ���ڿ��� ��ȯ. ������ �� ���� ��Ʈ���� ó������ ������ �̰� ����� ��
		const char* GetMultibyteStrByID(int objectID, std::string const& keystring);
		const char* GetArrayObjMultibyteStrByID(int objectID, std::string const& keystring, int index);

		const char* GetMultibyteStrByIndex(int index, std::string const& keystring);
		const char* GetArrayObjMultibyteStrByIndex(int index, std::string const& keystring, int keyindex);

		int GetArrayObjSizeByID(int objectID, std::string const& keystring);
		int GetArrayObjSizeByIndex(int index, std::string const& keystring);

		// �ܼ��� ��ġ ������ ���ø��� �Լ�. �̰� ����� ��� ����	
		template <typename T>
		T GetNumNBoolByID(int objectID, std::string const& keystring)
		{
			if (m_document.IsArray() == true)
			{
				// ������Ʈ��Ʈ�� json���� �ν�, map���� ID�� �ش��ϴ� ���� ������
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
				// �������� DOM���� json. ��ť��Ʈ���� �ٷ� ���� ã�� ����
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

		// �̰� key�� value�� array������ �� index���� ���ڷ� �־�, ���� ��ȯ��
		template <typename T>
		T GetArrayObjNumNBoolByID(int objectID, std::string const& keystring, int index)
		{
			if (m_document.IsArray() == true)
			{
				// ������Ʈ��Ʈ�� json���� �ν�, map���� ID�� �ش��ϴ� ���� ������
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
				// �������� DOM���� json. ��ť��Ʈ���� �ٷ� ���� ã�� ����
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
				// ��ť��Ʈ�� array���..
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
				// ������Ʈ��Ʈ�� json���� �ν�, map���� ID�� �ش��ϴ� ���� ������
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
				// �������� DOM���� json. ��ť��Ʈ���� �ٷ� ���� ã�� ����
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
