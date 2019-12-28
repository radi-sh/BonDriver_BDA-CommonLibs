#include "common.h"

#include "CIniFileAccess.h"

namespace {
	const CIniFileAccess::Map mapBool = {
		{ L"NO",    0 },
		{ L"YES",   1 },
		{ L"FALSE", 0 },
		{ L"TRUE",  1 },
		{ L"OFF",   0 },
		{ L"ON",    1 },
	};
}

CIniFileAccess::CIniFileAccess()
{
	m_SectionDataIterator = m_SectionData.begin();
}

CIniFileAccess::CIniFileAccess(const WCHAR* IniFilePath)
{
	SetIniFilePath(IniFilePath);
	CIniFileAccess();
}

CIniFileAccess::~CIniFileAccess()
{
	m_SectionData.clear();
	SAFE_DELETE_ARRAY(m_pKeysBuffer);
}

void CIniFileAccess::SetIniFilePath(const WCHAR* IniFilePath)
{
	if (IniFilePath) {
		m_IniFilePath = *IniFilePath;
	}
}

void CIniFileAccess::SetSectionName(const WCHAR* SectionName)
{
	if (SectionName) {
		m_SectionName = *SectionName;
	}
}

int CIniFileAccess::ReadKeyI(const WCHAR* KeyName, int default)
{
	return ReadKeyI(m_SectionName.c_str(), KeyName, default);
}

int CIniFileAccess::ReadKeyI(const WCHAR* SectionName, const WCHAR* KeyName, int default)
{
	return ::GetPrivateProfileIntW(SectionName, KeyName, default, m_IniFilePath.c_str());
}

BOOL CIniFileAccess::ReadKeyB(const WCHAR* KeyName, BOOL default)
{
	return ReadKeyB(m_SectionName.c_str(), KeyName, default);
}

BOOL CIniFileAccess::ReadKeyB(const WCHAR* SectionName, const WCHAR* KeyName, BOOL default)
{
	return ReadKeyIValueMap(SectionName, KeyName, default, &mapBool);
}

int CIniFileAccess::ReadKeyIValueMap(const WCHAR* KeyName, int default, const Map* ValueMap)
{
	return ReadKeyIValueMap(m_SectionName.c_str(), KeyName, default, ValueMap);
}

int CIniFileAccess::ReadKeyIValueMap(const WCHAR* SectionName, const WCHAR* KeyName, int default, const Map* ValueMap)
{
	WCHAR buf[_MAX_PATH + 1];
	::GetPrivateProfileStringW(SectionName, KeyName, L"", buf, sizeof(buf) / sizeof(buf[0]), m_IniFilePath.c_str());
	auto it = ValueMap->find(common::WStringToUpperCase(buf));
	if (it != ValueMap->end())
		return it->second;

	return ::GetPrivateProfileIntW(SectionName, KeyName, default, m_IniFilePath.c_str());
}

double CIniFileAccess::ReadKeyF(const WCHAR* KeyName, double default)
{
	return ReadKeyF(m_SectionName.c_str(), KeyName, default);
}

double CIniFileAccess::ReadKeyF(const WCHAR* SectionName, const WCHAR* KeyName, double default)
{
	WCHAR buf[_MAX_PATH + 1];
	::GetPrivateProfileStringW(SectionName, KeyName, std::to_wstring(default).c_str(), buf, sizeof(buf) / sizeof(buf[0]), m_IniFilePath.c_str());
	return common::WstringToDouble(buf);
}

std::wstring CIniFileAccess::ReadKeyS(const WCHAR* KeyName, const WCHAR* default)
{
	return ReadKeyS(m_SectionName.c_str(), KeyName, default);
}

std::wstring CIniFileAccess::ReadKeyS(const WCHAR* SectionName, const WCHAR* KeyName, const WCHAR* default)
{
	WCHAR buf[_MAX_PATH + 1];
	::GetPrivateProfileStringW(SectionName, KeyName, default, buf, sizeof(buf) / sizeof(buf[0]), m_IniFilePath.c_str());
	return std::wstring(buf);
}

size_t CIniFileAccess::ReadSection(const WCHAR* SectionName)
{
	if (!m_pKeysBuffer) {
		m_pKeysBuffer = new WCHAR[(_MAX_PATH + 1) * 100];
	}

	if (SectionName) {
		m_KeysSectionName = SectionName;
	}
	m_KeysSize = (size_t)::GetPrivateProfileSectionW(m_KeysSectionName.c_str(), m_pKeysBuffer, (_MAX_PATH + 1) * 100, m_IniFilePath.c_str());
	return m_KeysSize;
}

size_t CIniFileAccess::CreateSectionData(void)
{
	m_SectionData.clear();

	WCHAR *pkey = m_pKeysBuffer;
	while (pkey < m_pKeysBuffer + m_KeysSize) {
		if (pkey[0] == L'\0') {
			// キー名の一覧終わり
			break;
		}
		std::wstring line = pkey;
		// 行先頭が";"ならコメント
		if (line.find(L";") == 0) {
			line = L"";
		}
		// "="の前後で分割
		size_t pos = line.find(L"=");
		if (pos != std::wstring::npos) {
			std::wstring key = line.substr(0, pos);
			std::wstring data = line.substr(pos + 1);
			size_t l, r;
			// キーの前後空白を除去
			l = key.find_first_not_of(L" \t\v\r\n");
			if (l == std::wstring::npos)
				// キーが空
				continue;
			r = key.find_last_not_of(L" \t\v\r\n");
			key = key.substr(l, r - l + 1);
			// 値の前後空白を除去
			l = data.find_first_not_of(L" \t\v\r\n");
			if (l == std::wstring::npos)
				data = L"";
			else {
				r = data.find_last_not_of(L" \t\v\r\n");
				data = data.substr(l, r - l + 1);
			}
			// 値がダブルクォーテーションで囲まれている場合は除去
			l = data.find(L"\"");
			if (l == 0) {
				r = data.rfind(L"\"");
				if (r != std::wstring::npos) {
					data = data.substr(1, r - 1);
				}
			} else {
				// 値がシングルクォーテーションで囲まれている場合は除去
				l = data.find(L"'");
				if (l == 0) {
					r = data.rfind(L"'");
					if (r != std::wstring::npos) {
						data = data.substr(1, r - 1);
					}
				}
			}
			m_SectionData.emplace(common::WStringToUpperCase(key), data);
		}
		pkey += ::wcslen(pkey) + 1;
	}
	m_SectionDataIterator = m_SectionData.begin();

	return m_SectionData.size();
}

void CIniFileAccess::DeleteSectionData(void)
{
	m_SectionData.clear();
	m_SectionDataIterator = m_SectionData.begin();
}

void CIniFileAccess::Reset(void)
{
	m_SectionDataIterator = m_SectionData.begin();
}

bool CIniFileAccess::ReadSectionData(std::wstring* Key, std::wstring* Data)
{
	if (m_SectionDataIterator != m_SectionData.end()) {
		if (Key) {
			*Key = m_SectionDataIterator->first;
		}
		if (Data) {
			*Data = m_SectionDataIterator->second;
		}
		m_SectionDataIterator++;
		return true;
	}

	return false;
}

int CIniFileAccess::ReadKeyISectionData(const WCHAR* KeyName, int default)
{
	std::wstring s = ReadKeySSectionData(KeyName, L"");
	if (s.length() == 0)
		return default;
	return common::WStringToLong(s);
}

BOOL CIniFileAccess::ReadKeyBSectionData(const WCHAR* KeyName, BOOL default)
{
	return ReadKeyIValueMapSectionData(KeyName, default, &mapBool);
}

int CIniFileAccess::ReadKeyIValueMapSectionData(const WCHAR* KeyName, int default, const Map* ValueMap)
{
	std::wstring s = ReadKeySSectionData(KeyName, L"");
	if (s.length() == 0)
		return default;

	auto it = ValueMap->find(common::WStringToUpperCase(s));
	if (it != ValueMap->end())
		return it->second;

	return common::WStringToLong(s);
}

double CIniFileAccess::ReadKeyFSectionData(const WCHAR* KeyName, double default)
{
	std::wstring s = ReadKeySSectionData(KeyName, L"");
	if (s.length() == 0)
		return default;
	return common::WstringToDouble(s);
}

std::wstring CIniFileAccess::ReadKeySSectionData(const WCHAR* KeyName, const WCHAR* default)
{
	auto it = m_SectionData.find(common::WStringToUpperCase(KeyName));
	if (it == m_SectionData.end()) {
		return std::wstring(default);
	}
	return it->second;
}
