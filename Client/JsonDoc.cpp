#include "pch.h"
#include "JsonDoc.h"
#include "./rapidjson/prettywriter.h"		// for stringify JSON
#include "./rapidjson/filereadstream.h"	// fopen������� ����..
#include "./rapidjson/filewritestream.h"
#include "./rapidjson/error/error.h"		// �����Ǵ��� ���ؼ�
#include "./rapidjson/error/en.h"
#include "./rapidjson/encodedstream.h"    // AutoUTFInputStream Json���� ���ڵ� �ؼ��� ���ؼ�..
#include "./rapidjson/pointer.h"

using namespace ProjectN;

const int kReadBufferSize = 15000;

bool JsonDoc::Read(std::string const& file)
{
	errno_t err = fopen_s(&fp_, file.c_str(), "rb");
	if (err) {
		return false;
	}
	char readBuffer[kReadBufferSize];

	rapidjson::FileReadStream readStream{ fp_, readBuffer, sizeof(readBuffer) };

	// ���� ���� json������ ������ rapid �⺻�� �ٸ��� ����� BOM�� ���� ������, ó�� �ʿ�
	rapidjson::AutoUTFInputStream<unsigned,
		rapidjson::FileReadStream> eis(readStream);  // wraps bis into eis

	rapidjson::ParseResult result = m_document.ParseStream<0, rapidjson::AutoUTF<unsigned> >(eis);

	if (!result)
	{
		printf("JSON parse error: %s (%u)\n", GetParseError_En(result.Code()), (UINT)result.Offset());
		exit(1);
		return false;
	}

	// ���� document�� array���... ���������Ʈ�ϱ�..
	if (m_document.IsArray())
	{
		int _id;
		// ������� ������Ʈ�� �����ͼ�..
		for (rapidjson::SizeType i = 0; i < m_document.Size(); i++)
		{
			// ID�� ���� ������Ʈ�� json�����͸� ������ �����̳ʿ� �־��ش�.
			_id = m_document[i]["ID"].GetInt();
			// RapidJson�� Pointer�� /Ű��, Ȥ��index ������ ���ڿ��̴�.
			std::string _pointerToken = "/" + std::to_string(i);
			rapidjson::Value* pObj = rapidjson::Pointer(_pointerToken.c_str()).Get(m_document);
			m_pDocumentObjects.insert({ _id, pObj });
		}
	}

	fclose(fp_);

	return true;
}

// �ø��������� ���� �� ��.  �ʿ��� �� �������
bool JsonDoc::Write(std::string const& file)
{
	errno_t err = fopen_s(&fp_, file.c_str(), "wb");
	if (err) {
		return false;
	}

	char writeBuffer[kReadBufferSize];
	rapidjson::FileWriteStream writeStream(fp_, writeBuffer, _countof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(writeStream);
	m_document.Accept(writer);

	fclose(fp_);

	return true;
}

// ���� ��ü�� ������. ���⼭���� ã�ư��� ����.
rapidjson::Document& JsonDoc::GetDocument()
{
	return m_document;
}

int JsonDoc::GetDocumentSheetSize()
{
	return static_cast<int>(m_document.Size());
}

// id�� �ش� ID�� ������Ʈ�� ������
// m_pTest->GetDocumentObject(10101002)["KeyName"].GetInt(); �̷������� ���� Ÿ�� ���� ��.
// �̰� ���ܻ�Ȳ ó�� ����������, json ���� �� ���� �� ��. (DOM���� json���� �̰� ���� ����)
rapidjson::Value& JsonDoc::GetDocumentObject(int objectID)
{
	return m_pDocumentObjects[objectID]->GetObj();
}

std::wstring JsonDoc::GetUnicodeStrByID(int objectID, std::string const& keystring)
{
	char strUTF8[500] = {};
	wchar_t strUnicode[252] = {};
	if (m_document.IsArray() == true)
	{
		rapidjson::Value::ConstMemberIterator itr = m_pDocumentObjects[objectID]->FindMember(keystring.c_str());
		if (itr != m_pDocumentObjects[objectID]->MemberEnd())
		{
			strcpy_s(strUTF8, 500, itr->value.GetString());
		}
	}
	else
	{
		strcpy_s(strUTF8, 500, m_document[keystring.c_str()].GetString());
	}
	// �Էµ� utf8 ��Ƽ����Ʈ ���ڸ� �����ڵ�� ��ȯ.
	int nLen = MultiByteToWideChar(
		CP_UTF8,
		0,
		strUTF8,
		(int)strlen(strUTF8),
		NULL,
		NULL
	);
	MultiByteToWideChar(
		CP_UTF8,
		0,
		strUTF8,
		(int)strlen(strUTF8),
		strUnicode,
		nLen
	);
	// ��Ʈ�� �ν��Ͻ��� ���� �־ �����Ѵ�.
	std::wstring _returnStr(strUnicode);
	return _returnStr;
}

std::wstring JsonDoc::GetArrayObjUnicodeStrByID(int objectID, std::string const& keystring, int index)
{
	char strUTF8[500] = {};
	wchar_t strUnicode[252] = {};
	if (m_document.IsArray() == true)
	{
		rapidjson::Value::ConstMemberIterator itr = m_pDocumentObjects[objectID]->FindMember(keystring.c_str());
		if (itr != m_pDocumentObjects[objectID]->MemberEnd())
		{
			strcpy_s(strUTF8, 500, itr->value[index].GetString());
		}
	}
	else
	{
		strcpy_s(strUTF8, 500, m_document[keystring.c_str()][index].GetString());
	}
	// �Էµ� utf8 ��Ƽ����Ʈ ���ڸ� �����ڵ�� ��ȯ.
	int nLen = MultiByteToWideChar(
		CP_UTF8,
		0,
		strUTF8,
		(int)strlen(strUTF8),
		NULL,
		NULL
	);
	MultiByteToWideChar(
		CP_UTF8,
		0,
		strUTF8,
		(int)strlen(strUTF8),
		strUnicode,
		nLen
	);
	// ��Ʈ�� �ν��Ͻ��� ���� �־ �����Ѵ�.
	std::wstring _returnStr(strUnicode);
	return _returnStr;
}

std::wstring JsonDoc::GetUnicodeStrByIndex(int index, std::string const& keystring)
{
	char strUTF8[500] = {};
	wchar_t strUnicode[252] = {};
	if (m_document.IsArray() == true)
	{
		strcpy_s(strUTF8, 500, m_document[index][keystring.c_str()].GetString());
	}
	else
	{
		strcpy_s(strUTF8, 500, m_document[keystring.c_str()].GetString());
	}
	// �Էµ� utf8 ��Ƽ����Ʈ ���ڸ� �����ڵ�� ��ȯ.
	int nLen = MultiByteToWideChar(
		CP_UTF8,
		0,
		strUTF8,
		(int)strlen(strUTF8),
		NULL,
		NULL
	);
	MultiByteToWideChar(
		CP_UTF8,
		0,
		strUTF8,
		(int)strlen(strUTF8),
		strUnicode,
		nLen
	);
	// ��Ʈ�� �ν��Ͻ��� ���� �־ �����Ѵ�.
	std::wstring _returnStr(strUnicode);
	return _returnStr;
}

std::wstring JsonDoc::GetArrayObjUnicodeStrByIndex(int index, std::string const& keystring, int keyindex)
{
	char strUTF8[500] = {};
	wchar_t strUnicode[252] = {};
	if (m_document.IsArray() == true)
	{
		strcpy_s(strUTF8, 500, m_document[index][keystring.c_str()][keyindex].GetString());
	}
	else
	{
		strcpy_s(strUTF8, 500, m_document[keystring.c_str()][keyindex].GetString());
	}
	// �Էµ� utf8 ��Ƽ����Ʈ ���ڸ� �����ڵ�� ��ȯ.
	int nLen = MultiByteToWideChar(
		CP_UTF8,
		0,
		strUTF8,
		(int)strlen(strUTF8),
		NULL,
		NULL
	);
	MultiByteToWideChar(
		CP_UTF8,
		0,
		strUTF8,
		(int)strlen(strUTF8),
		strUnicode,
		nLen
	);
	// ��Ʈ�� �ν��Ͻ��� ���� �־ �����Ѵ�.
	std::wstring _returnStr(strUnicode);
	return _returnStr;
}

bool JsonDoc::IsKeyExistByID(int objectID, std::string const& keystring)
{
	if (m_document.IsArray() == true)
	{
		rapidjson::Value::ConstMemberIterator itr = m_pDocumentObjects[objectID]->FindMember(keystring.c_str());
		if (itr != m_pDocumentObjects[objectID]->MemberEnd())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value::ConstMemberIterator itr = m_document.FindMember(keystring.c_str());
		if (itr != m_document.MemberEnd())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool JsonDoc::IsKeyExistByIndex(int index, std::string const& keystring)
{
	if (m_document.IsArray() == true)
	{
		rapidjson::Value::ConstMemberIterator itr = m_document[index].FindMember(keystring.c_str());
		if (itr != m_document[index].MemberEnd())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		rapidjson::Value::ConstMemberIterator itr = m_document.FindMember(keystring.c_str());
		if (itr != m_document.MemberEnd())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}

const char* JsonDoc::GetMultibyteStrByID(int objectID, std::string const& keystring)
{
	if (m_document.IsArray() == true)
	{
		rapidjson::Value::ConstMemberIterator itr = m_pDocumentObjects[objectID]->FindMember(keystring.c_str());
		if (itr != m_pDocumentObjects[objectID]->MemberEnd())
		{
			return itr->value.GetString();
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return m_document[keystring.c_str()].GetString();
	}
}

const char* JsonDoc::GetArrayObjMultibyteStrByID(int objectID, std::string const& keystring, int index)
{
	if (m_document.IsArray() == true)
	{
		rapidjson::Value::ConstMemberIterator itr = m_pDocumentObjects[objectID]->FindMember(keystring.c_str());
		if (itr != m_pDocumentObjects[objectID]->MemberEnd())
		{
			return itr->value[index].GetString();
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return m_document[keystring.c_str()][index].GetString();
	}

}

const char* JsonDoc::GetMultibyteStrByIndex(int index, std::string const& keystring)
{
	if (m_document.IsArray() == true)
	{

		return m_document[index][keystring.c_str()].GetString();
	}
	else
	{
		return m_document[keystring.c_str()].GetString();
	}

}

const char* JsonDoc::GetArrayObjMultibyteStrByIndex(int index, std::string const& keystring, int keyindex)
{
	if (m_document.IsArray() == true)
	{

		return m_document[index][keystring.c_str()][keyindex].GetString();
	}
	else
	{
		return m_document[keystring.c_str()].GetString();
	}

}

int JsonDoc::GetArrayObjSizeByID(int objectID, std::string const& keystring)
{
	if (m_document.IsArray() == true)
	{
		rapidjson::Value::ConstMemberIterator itr = m_pDocumentObjects[objectID]->FindMember(keystring.c_str());
		if (itr != m_pDocumentObjects[objectID]->MemberEnd())
		{
			return static_cast<int>(itr->value.Size());
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return static_cast<int>(m_document[keystring.c_str()].Size());
	}
}

int JsonDoc::GetArrayObjSizeByIndex(int index, std::string const& keystring)
{
	if (m_document.IsArray() == true)
	{

		return static_cast<int>(m_document[index][keystring.c_str()].Size());
	}
	else
	{
		return static_cast<int>(m_document[keystring.c_str()].Size());
	}

}