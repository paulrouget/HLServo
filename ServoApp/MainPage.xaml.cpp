#include "pch.h"
#include "MainPage.xaml.h"

using namespace ServoApp;

MainPage::MainPage()
{
	InitializeComponent();
	
	HMODULE ret = LoadPackagedLibrary(L"hlservo.dll", 0);
	FARPROC r = GetProcAddress(ret, "foobar");
	INT_PTR ptr = r();

	int number = ptr;

}
