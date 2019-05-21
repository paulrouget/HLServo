#include "pch.h"
#include "MainPage.xaml.h"

using namespace ServoApp;

void Log(const wchar_t* msg) {
  wchar_t msg2[256];
  swprintf(msg2, 255, L"%s\r\n", msg);
  OutputDebugString(msg2);
}

void LogWithError(const wchar_t* msg) {
  wchar_t buf[256];
  FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
  Log(msg);
  Log(buf);
}

HMODULE LoadDLL(const wchar_t* name) {
  HMODULE m = LoadPackagedLibrary(name, 0);
  if (m == NULL) {
    wchar_t error[256];
    swprintf(error, 255, L"FAILED: %s", name);
    LogWithError(error);
  } else {
	  wchar_t msg[256];
	  swprintf(msg, 255, L"SUCCESS: %s", name);
	  Log(msg);
  }
  return m;
}

MainPage::MainPage()
{
  InitializeComponent();

  HMODULE ret = LoadDLL(L"simpleservo");

  FARPROC r = GetProcAddress(ret, "servo_version");
  if (r == NULL) {
    LogWithError(L"GetProcAddress failed");
    return;
  }

  char * v = (char *)r();

  std::string s_str = std::string(v);
  std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
  const wchar_t* w_char = wid_str.c_str();

  Log(w_char);

}

