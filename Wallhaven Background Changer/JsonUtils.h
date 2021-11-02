#pragma once
using namespace System;

// TODO png support.

public ref struct JsonUtils
{
public:
	delegate System::Void requestCallback(String^ j, int i);
	delegate System::Void jsProcessCallback(Object^ i);

	String^ _url;
	requestCallback^ reqCb;
	JsonUtils(String^ url, requestCallback^ cback) {
		_url = url;
		reqCb = cback;
	}

	System::Void jsonRequest()
	{
		System::Net::WebRequest^ request = System::Net::WebRequest::Create(_url);
		request->Credentials = System::Net::CredentialCache::DefaultCredentials;
		System::Net::HttpWebResponse^ response = dynamic_cast<System::Net::HttpWebResponse^>(request->GetResponse());
		if (response->StatusCode == System::Net::HttpStatusCode(200)) {
			System::IO::Stream^ dataStream = response->GetResponseStream();
			System::IO::StreamReader^ reader = gcnew System::IO::StreamReader(dataStream);
			System::String^ responseFromServer = reader->ReadToEnd();

			reqCb(responseFromServer, 200);
		}
		else {
			System::String^ responseFromServer = gcnew String("Error");
			reqCb(responseFromServer, 500);
		}
	}
public:
	String^ json;
	int page;
	jsProcessCallback^ jsProcCb;
	JsonUtils(String^ j, int pg, jsProcessCallback^ jsProc) {
		json = j;
		page = pg;
		jsProcCb = jsProc;
	}
	System::Void processJson() {
		msclr::interop::marshal_context context;
		nlohmann::json j = nlohmann::json::parse(context.marshal_as<std::string>(json));
		int total = j["meta"]["total"].get<int>();
		int pageNumber = j["meta"]["last_page"].get<int>();
		int currentPageNumber = j["meta"]["current_page"].get<int>();
		int perPage;
		try {
			perPage = std::stoi(j["meta"]["per_page"].get<std::string>());
		}
		catch (Exception^) {
			perPage = j["meta"]["per_page"].get<int>();
		}

		int resultNumber;
		if (currentPageNumber == pageNumber)
			resultNumber = total % perPage;
		else
			resultNumber = perPage;
		array<String^>^ paths = gcnew array<String^>(resultNumber);
		array<String^>^ shortUrls = gcnew array<String^>(resultNumber);
		array<String^>^ IDs = gcnew array<String^>(resultNumber);
		array<String^>^ fileTypes = gcnew array<String^>(resultNumber);
		array<String^>^ purity = gcnew array<String^>(resultNumber);


		if ( total > 0 && pageNumber >= page) {
			for (int i = 0; i < resultNumber; i += 1) {
				paths[i] = gcnew System::String((j[nlohmann::json::json_pointer((std::string)"/data/" + std::to_string(i))]["path"].get<std::string>()).c_str());
				shortUrls[i] = gcnew System::String((j[nlohmann::json::json_pointer((std::string)"/data/" + std::to_string(i))]["short_url"].get<std::string>()).c_str());
				IDs[i] = gcnew System::String((j[nlohmann::json::json_pointer((std::string)"/data/" + std::to_string(i))]["id"].get<std::string>()).c_str());
				std::string fileType = j[nlohmann::json::json_pointer((std::string)"/data/" + std::to_string(i))]["file_type"].get<std::string>();
				fileTypes[i] = fileType.substr(fileType.size() - 3) == (std::string)"peg" ? gcnew String("jpg") : gcnew String("png");
				purity[i] = gcnew System::String((j[nlohmann::json::json_pointer((std::string)"/data/" + std::to_string(i))]["purity"].get<std::string>()).c_str());


			}
			jsProcCb(Tuple::Create(pageNumber, resultNumber, paths, shortUrls, IDs, fileTypes, purity));
		}
		else
		{
			jsProcCb(System::Tuple::Create(27272727, resultNumber, paths, shortUrls, IDs, fileTypes, purity));
		}
	}
};
