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
#pragma once
using namespace System;

public ref struct JsonUtils
{
	// Create Callback delegates for callback handling. I explained Callbacks in MyForm1.h and I don't explain here again.
	public: delegate System::Void requestCallback(String^ j, int i);
	public: delegate System::Void jsProcessCallback(Object^ i);

	// Specified url builded with queryBuilder
	public: String^ _url;

	// Not parsed json
	public: String^ json;

	// Callbacks
	public: requestCallback^ reqCb;
	public: jsProcessCallback^ jsProcCb;

	// Page number for checking pageNumber is bigger or smaller than realpageNumber or not
	public: int page;

	// Default constructor for jsonRequest
	public: JsonUtils(String^ url, requestCallback^ cback) {
		_url = url;
		reqCb = cback;
	}

	// Default Constructor for processing Json
	public: JsonUtils(String^ j, int pg, jsProcessCallback^ jsProc) {
		json = j;
		page = pg;
		jsProcCb = jsProc;
	}

	//
	// Function: jsonRequest: Sends Json request to wallhaven.cc for getting image results.
	// 
	// returns void.
	//
	public: System::Void jsonRequest()
	{
		// Create web request with _url(Specified in default constructor)
		System::Net::WebRequest^ request = System::Net::WebRequest::Create(_url);

		// Telling server to we are not a robot.
		request->Credentials = System::Net::CredentialCache::DefaultCredentials;

		// Get Response
		System::Net::HttpWebResponse^ response = dynamic_cast<System::Net::HttpWebResponse^>(request->GetResponse());

		// If HttpStatusCode is ok
		if (response->StatusCode == System::Net::HttpStatusCode(200)) {
			// Create Stream for not parsed json
			System::IO::Stream^ dataStream = response->GetResponseStream();

			// Read Stream for not parsed json
			System::IO::StreamReader^ reader = gcnew System::IO::StreamReader(dataStream);

			// Get not parsed json
			System::String^ responseFromServer = reader->ReadToEnd();

			// Send not parsed json and httpcode in callback then return
			reqCb(responseFromServer, 200);
			return;
		}
		// If HttpStatusCode isn't ok
		else {
			// There is no json, there is error.
			System::String^ responseFromServer = gcnew String("Error");

			// Send error with 500 code then return;
			reqCb(responseFromServer, 500);
			return;
		}
	}

	//
	// Function: processJson: process json for get variables.
	// 
	// returns void.
	//
	public: System::Void processJson() 
	{
		// Context for type casting.
		msclr::interop::marshal_context context;

		// Create new Json variable and parse json(Specified in default constructor)
		nlohmann::json j = nlohmann::json::parse(context.marshal_as<std::string>(json));

		// Get total result number in json.
		int total = j["meta"]["total"].get<int>();

		// Get total page number in json.
		int pageNumber = j["meta"]["last_page"].get<int>();

		// Get Current page number in json.
		int currentPageNumber = j["meta"]["current_page"].get<int>();

		// Create per page instance
		int perPage;
		try {
			// Try get per page in string than parse it with integer
			perPage = std::stoi(j["meta"]["per_page"].get<std::string>());
		}
		catch (Exception^) {
			// If error get per page in integer
			perPage = j["meta"]["per_page"].get<int>();
		}

		// Get resultnumber
		int resultNumber;
		// If we are in last page then get last page's result count
		if (currentPageNumber == pageNumber)
			resultNumber = total % perPage;
		// If we are in not last page then get resultNumber is equal to perPage
		else
			resultNumber = perPage;

		// Create string arrays for processing.
		array<String^>^ paths = gcnew array<String^>(resultNumber);
		array<String^>^ shortUrls = gcnew array<String^>(resultNumber);
		array<String^>^ IDs = gcnew array<String^>(resultNumber);
		array<String^>^ fileTypes = gcnew array<String^>(resultNumber);
		array<String^>^ purity = gcnew array<String^>(resultNumber);

		// If we have results(total result number is bigger than 0) and pageNumber is bigger than or equals to page
		if (total > 0 && pageNumber >= page) {
			// Loop for results.
			for (int i = 0; i < resultNumber; i += 1) {
				// Get Paths.
				paths[i] = gcnew System::String((j[nlohmann::json::json_pointer((std::string)"/data/" + std::to_string(i))]["path"].get<std::string>()).c_str());

				// Get shortUrls.
				shortUrls[i] = gcnew System::String((j[nlohmann::json::json_pointer((std::string)"/data/" + std::to_string(i))]["short_url"].get<std::string>()).c_str());

				// Get IDs.
				IDs[i] = gcnew System::String((j[nlohmann::json::json_pointer((std::string)"/data/" + std::to_string(i))]["id"].get<std::string>()).c_str());

				// Process filetype
				std::string fileType = j[nlohmann::json::json_pointer((std::string)"/data/" + std::to_string(i))]["file_type"].get<std::string>();

				// and assign it to fileTypes.
				fileTypes[i] = fileType.substr(fileType.size() - 3) == (std::string)"peg" ? gcnew String("jpg") : gcnew String("png");

				// Get purity.
				purity[i] = gcnew System::String((j[nlohmann::json::json_pointer((std::string)"/data/" + std::to_string(i))]["purity"].get<std::string>()).c_str());
			}

			// Call callback and return.
			jsProcCb(Tuple::Create(pageNumber, resultNumber, paths, shortUrls, IDs, fileTypes, purity));
			return;
		}
		// No results. Return error.
		else
		{
			// Call callback and return.
			jsProcCb(System::Tuple::Create(27272727, resultNumber, paths, shortUrls, IDs, fileTypes, purity));
			return;
		}
	}
};
