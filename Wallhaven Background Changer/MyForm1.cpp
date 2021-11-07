// Define WIN32_LEAN_AND_MEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Include libraries
#include <iostream>
#include <fstream>
#include <iomanip>
#include <regex>
#include <windows.h>
#include <msclr/marshal_atl.h>
#include <msclr/marshal_cppstd.h>
#include <nlohmann/json.hpp>
#include "resource.h"
#include "JsonUtils.h"
#include "JpegUtils.h"
#include "options.h"
#include "MyForm1.h"

// Include ws2_32.lib
#pragma comment(lib,"ws2_32.lib")

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;

// Change assembly version
using namespace System::Reflection;
[assembly:AssemblyVersionAttribute("1.0.0.1")] ;
// [assembly:AssemblyKeyFileAttribute("keyfile.snk")] ;


// Main function
[STAThread]
void Main(array<String^>^ args)
{
	// Enable visual styles and disable GDI+ text rendering.
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Run Form
	Application::Run(gcnew WhavenWallpaper::MyForm);
}