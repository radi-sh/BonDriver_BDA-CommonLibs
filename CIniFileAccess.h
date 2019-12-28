#pragma once

#include "common.h"

#include <Windows.h>
#include <string>
#include <map>

class CIniFileAccess
{
public:
	using Map = const std::map<const std::wstring, const int, std::less<>>;
public:
	CIniFileAccess();
	CIniFileAccess(const WCHAR* IniFilePath);
	~CIniFileAccess();
	void SetIniFilePath(const WCHAR* IniFilePath);
	void SetSectionName(const WCHAR* SectionName);
	int ReadKeyI(const WCHAR* KeyName, int default);
	int ReadKeyI(const WCHAR* SectionName, const WCHAR* KeyName, int default);
	BOOL ReadKeyB(const WCHAR* KeyName, BOOL default);
	BOOL ReadKeyB(const WCHAR* SectionName, const WCHAR* KeyName, BOOL default);
	int ReadKeyIValueMap(const WCHAR* KeyName, int default, const Map* ValueMap);
	int ReadKeyIValueMap(const WCHAR* SectionName, const WCHAR* KeyName, int default, const Map* ValueMap);
	double ReadKeyF(const WCHAR* KeyName, double default);
	double ReadKeyF(const WCHAR* SectionName, const WCHAR* KeyName, double default);
	std::wstring ReadKeyS(const WCHAR* KeyName, const WCHAR* default);
	std::wstring ReadKeyS(const WCHAR* SectionName, const WCHAR* KeyName, const WCHAR* default);
	size_t ReadSection(const WCHAR* SectionName);
	size_t CreateSectionData(void);
	void DeleteSectionData(void);
	void Reset(void);
	bool ReadSectionData(std::wstring* Key, std::wstring* Data);
	int ReadKeyISectionData(const WCHAR* KeyName, int default);
	BOOL ReadKeyBSectionData(const WCHAR* KeyName, BOOL default);
	int ReadKeyIValueMapSectionData(const WCHAR* KeyName, int default, const Map* ValueMap);
	double ReadKeyFSectionData(const WCHAR* KeyName, double default);
	std::wstring ReadKeySSectionData(const WCHAR* KeyName, const WCHAR* default);

private:
	std::wstring m_IniFilePath;
	std::wstring m_SectionName;
	WCHAR* m_pKeysBuffer = NULL;
	size_t m_KeysSize = 0;
	std::wstring m_KeysSectionName;
	std::multimap<std::wstring, std::wstring, std::less<>> m_SectionData;
	std::multimap<std::wstring, std::wstring, std::less<>>::iterator m_SectionDataIterator;
};

