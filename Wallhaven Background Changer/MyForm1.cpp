/// <summary>
/// Whaven Wallpaper
/// WHaven Wallpaper is a desktop wallpaper change program for change your wallpapers directly from [wallhaven](https://wallhaven.cc/).
///	Copyright(C) 2021 EndChapter
///
///	This program is free software : you can redistribute itand /or modify
///	it under the terms of the GNU General Public License as published by
///	the Free Software Foundation, either version 3 of the License, or
///	(at your option) any later version.
///
///	This program is distributed in the hope that it will be useful,
///	but WITHOUT ANY WARRANTY; without even the implied warranty of
///	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
/// GNU General Public License for more details.
///
///	You should have received a copy of the GNU General Public License
///	along with this program.If not, see < https://www.gnu.org/licenses/>.
/// </summary>

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