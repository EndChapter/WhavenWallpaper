#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>
#include <fstream>
#include <regex>
#include <windows.h>
#include <msclr/marshal_atl.h>
#include <msclr/marshal_cppstd.h>
#include <nlohmann/json.hpp>
#include <INIReader.h>
#include <ini.h>
#include "resource.h"
#include "JsonUtils.h"
#include "JpegUtils.h"
#include "MyForm1.h"

#pragma comment(lib,"ws2_32.lib")

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;

using namespace System::Reflection;
[assembly:AssemblyVersionAttribute("0.1.0.0")] ;
// [assembly:AssemblyKeyFileAttribute("keyfile.snk")] ;

[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	WhavenWallpaper::MyForm form;
	Application::Run(% form);
}