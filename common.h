#pragma once

#include <Windows.h>
#include <string>

#define SAFE_RELEASE(p)			{ if(p) { (p)->Release();   (p)=NULL; } }
#define SAFE_DELETE(p)			{ if(p) { delete (p);       (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);     (p)=NULL; } }
#define SAFE_CLOSE_HANDLE(p)	{ if(p) { ::CloseHandle(p); (p)=NULL; } }

using QWORD = unsigned long long;

namespace DebugLog
{
	inline FILE* fpLog = NULL;
	inline void SetDebugLog(std::wstring sLogPath)
	{
		if (fpLog) {
			// �ʂ̃C���X�^���X�ɂ��J����Ă���
			return;
		}

		fpLog = _wfsopen(sLogPath.c_str(), L"a+", _SH_DENYNO);

		return;
	}

	inline void CloseDebugLog(void)
	{
		if (fpLog) {
			fclose(fpLog);
			fpLog = NULL;
		}

		return;
	}

	inline void OutputDebug(LPCWSTR format, ...)
	{
		WCHAR buffer[2048];
		va_list ap;
		va_start(ap, format);
		::vswprintf_s(buffer, sizeof(buffer) / sizeof(buffer[0]), format, ap);
		va_end(ap);
		::OutputDebugStringW(buffer);

		if (fpLog) {
			fwprintf(fpLog, buffer);
			fflush(fpLog);
		}
	}
}

using namespace DebugLog;

namespace common
{
	std::string StringPrintf(LPCSTR format, ...);
	std::wstring WStringPrintf(LPCWSTR format, ...);
	std::basic_string<TCHAR> TStringPrintf(LPCTSTR format, ...);
	std::string WStringToString(std::wstring Src);
	std::wstring StringToWString(std::string Src);
	std::basic_string<TCHAR> WStringToTString(std::wstring Src);
	std::wstring WStringToUpperCase(std::wstring Src);
	std::wstring WStringToLowerCase(std::wstring Src);
	int WStringToLong(std::wstring Src);
	int WStringDecimalToLong(std::wstring Src);
	double WstringToDouble(std::wstring Src);
	std::wstring GUIDToWString(GUID guid);
	std::wstring GetModuleName(HMODULE hModule);
	std::wstring::size_type WStringSplit(std::wstring * Src, WCHAR Separator, std::wstring * Out);
}
