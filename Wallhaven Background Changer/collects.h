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
// Namespace for Collections
namespace collects {

	// Used namespaces
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	// Class for collections
	public ref class collects : public System::Windows::Forms::Form
	{
#pragma region variables
		//////////////////////////////////////////////////////////////////////////////
		//																			//
		// VARIABLES																//
		//																			//
		//////////////////////////////////////////////////////////////////////////////

		// Your api key.
		private: System::String^ apikey;

		// Collection ids from specified search.
		private: array<String^>^ collectionIDs;

		// Collection names from specified search.
		private: array<Object^>^ collectionNames;

		// Session variable for getting current session state.
		private: bool session = false;

		// Selected index from collection list box.
		private: int selectedCollectionListBoxIndex = 0;

		// Page index for sending page.
		private: int page = 0;

		// Callbacks and delegates for sending information to parent
		public: delegate System::Void collectionFirstCallback();
		public: delegate System::Void collectionSecondCallback();
		public: delegate System::Void collectionThirdCallback(System::String^ _url, int _page);
		public: delegate System::Void collectionErrorCallback();
		private: collectionFirstCallback^ collFCb;
		private: collectionSecondCallback^ collSCb;
		private: collectionThirdCallback^ collTCb;
		private: collectionErrorCallback^ collECb;
#pragma endregion

		// Base constructor for collections
		public: collects(System::String^ _apikey, collectionFirstCallback^ FCb, collectionSecondCallback^ SCb, collectionThirdCallback^ TCb, collectionErrorCallback^ ECb, bool saveStatus)
		{
			// Load App Icon Before Initialize Components
			this->Icon = gcnew System::Drawing::Icon(resourceAssembly->GetManifestResourceStream("favicon.ico"));

			// Initialize components.
			InitializeComponent();

			//This line disables maximize button.
			SetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWL_STYLE, GetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWL_STYLE) & ~WS_MAXIMIZEBOX);

			// Get variables from parent
			apikey = _apikey;
			collFCb = FCb;
			collSCb = SCb;
			collTCb = TCb;
			collECb = ECb;

			// Create json instance for loading json
			nlohmann::json j;

			// If Json file exist then get to options if not do nothing.
			if (System::IO::File::Exists("whaven-wallpaper-config.json"))
			{
				// Create filestream for reading whaven-wallpaper-config.json
				std::ifstream i("whaven-wallpaper-config.json");

				// parsing json
				i >> j;

				// If save session state true and collection is not null:
				if (j["options"]["savesession"] && !j["collections"].is_null()) {
					// Get nickname and write it in search input.
					this->usernameInput->Text = gcnew String(j["collections"]["nickname"].get<std::string>().c_str());

					// We have a session, so our session state is true.
					session = true;


					// Getting selected collection as index number.
					if (j["collections"]["selectedSearch"].is_null())
						selectedCollectionListBoxIndex = 0;
					else
						selectedCollectionListBoxIndex = j["collections"]["selectedSearch"];

					// If parent saying us to status saved before:
					if (saveStatus) {
						// Getting page index
						if (j["session"]["pageIndex"].is_null())
							page = 0;
						else
							page = j["session"]["pageIndex"];
						// Run search button event for initializing app.
						searchButton_Click("", nullptr);
					}
				}

			}
		}

		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~collects()
			{
				// Create json instance for loading json
				nlohmann::json j;

				// Create filestream for reading whaven-wallpaper-config.json
				std::ifstream i("whaven-wallpaper-config.json");

				// parsing json
				i >> j;

				if (j["options"]["savesession"]) {
					// Create context for type casting.
					msclr::interop::marshal_context context;

					// Set nickname for getting from constructor
					j["collections"]["nickname"] = context.marshal_as<const char*>(this->usernameInput->Text);

					// Set selected collection index for getting from constructor
					j["collections"]["selectedSearch"] = this->collectionsListBox->SelectedIndex;

					// Create write stream for json.
					std::ofstream o("whaven-wallpaper-config.json");

					// Write json to file.
					o << std::setw(4) << j << std::endl;
				}

				if (components)
				{
					delete components;
				}
			}
		private: System::Reflection::Assembly^ resourceAssembly = Reflection::Assembly::GetExecutingAssembly();
		private: System::Windows::Forms::Label^ usernameLabel;
		private: System::Windows::Forms::TextBox^ usernameInput;
		private: System::Windows::Forms::Button^ searchButton;
		private: System::Windows::Forms::Label^ collectionsLabel;
		private: System::Windows::Forms::Button^ collectionOkButton;
		private: System::Windows::Forms::TextBox^ collectionNumberTextBox;
		private: System::Windows::Forms::Label^ collectionNumberLabel;
		private: System::Windows::Forms::Label^ oeacnLabel;
		private: System::Windows::Forms::ListBox^ collectionsListBox;
		private: System::Windows::Forms::Label^ requiredLabel;


		private:
			/// <summary>
			/// Required designer variable.
			/// </summary>
			System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->usernameLabel = (gcnew System::Windows::Forms::Label());
			this->usernameInput = (gcnew System::Windows::Forms::TextBox());
			this->searchButton = (gcnew System::Windows::Forms::Button());
			this->collectionsLabel = (gcnew System::Windows::Forms::Label());
			this->collectionOkButton = (gcnew System::Windows::Forms::Button());
			this->collectionNumberTextBox = (gcnew System::Windows::Forms::TextBox());
			this->collectionNumberLabel = (gcnew System::Windows::Forms::Label());
			this->oeacnLabel = (gcnew System::Windows::Forms::Label());
			this->collectionsListBox = (gcnew System::Windows::Forms::ListBox());
			this->requiredLabel = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// usernameLabel
			// 
			this->usernameLabel->AutoSize = true;
			this->usernameLabel->Location = System::Drawing::Point(13, 45);
			this->usernameLabel->Name = L"usernameLabel";
			this->usernameLabel->Size = System::Drawing::Size(58, 13);
			this->usernameLabel->TabIndex = 0;
			this->usernameLabel->Text = L"Username:";
			// 
			// usernameInput
			// 
			this->usernameInput->Location = System::Drawing::Point(77, 42);
			this->usernameInput->Name = L"usernameInput";
			this->usernameInput->Size = System::Drawing::Size(100, 20);
			this->usernameInput->TabIndex = 1;
			// 
			// searchButton
			// 
			this->searchButton->Location = System::Drawing::Point(77, 68);
			this->searchButton->Name = L"searchButton";
			this->searchButton->Size = System::Drawing::Size(100, 23);
			this->searchButton->TabIndex = 3;
			this->searchButton->Text = L"Search";
			this->searchButton->UseVisualStyleBackColor = true;
			this->searchButton->Click += gcnew System::EventHandler(this, &collects::searchButton_Click);
			// 
			// collectionsLabel
			// 
			this->collectionsLabel->AutoSize = true;
			this->collectionsLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold));
			this->collectionsLabel->Location = System::Drawing::Point(12, 9);
			this->collectionsLabel->Name = L"collectionsLabel";
			this->collectionsLabel->Size = System::Drawing::Size(185, 24);
			this->collectionsLabel->TabIndex = 26;
			this->collectionsLabel->Text = L"Collections  ————";
			// 
			// collectionOkButton
			// 
			this->collectionOkButton->Location = System::Drawing::Point(142, 223);
			this->collectionOkButton->Name = L"collectionOkButton";
			this->collectionOkButton->Size = System::Drawing::Size(43, 23);
			this->collectionOkButton->TabIndex = 50;
			this->collectionOkButton->Text = L"Ok";
			this->collectionOkButton->UseVisualStyleBackColor = true;
			// 
			// collectionNumberTextBox
			// 
			this->collectionNumberTextBox->Location = System::Drawing::Point(16, 225);
			this->collectionNumberTextBox->Name = L"collectionNumberTextBox";
			this->collectionNumberTextBox->Size = System::Drawing::Size(120, 20);
			this->collectionNumberTextBox->TabIndex = 49;
			// 
			// collectionNumberLabel
			// 
			this->collectionNumberLabel->AutoSize = true;
			this->collectionNumberLabel->Location = System::Drawing::Point(13, 208);
			this->collectionNumberLabel->Name = L"collectionNumberLabel";
			this->collectionNumberLabel->Size = System::Drawing::Size(67, 13);
			this->collectionNumberLabel->TabIndex = 48;
			this->collectionNumberLabel->Text = L"(0 collection)";
			// 
			// oeacnLabel
			// 
			this->oeacnLabel->AutoSize = true;
			this->oeacnLabel->Location = System::Drawing::Point(13, 195);
			this->oeacnLabel->Name = L"oeacnLabel";
			this->oeacnLabel->Size = System::Drawing::Size(145, 13);
			this->oeacnLabel->TabIndex = 47;
			this->oeacnLabel->Text = L"or Enter a Collection Number:";
			// 
			// collectionsListBox
			// 
			this->collectionsListBox->FormattingEnabled = true;
			this->collectionsListBox->Location = System::Drawing::Point(16, 97);
			this->collectionsListBox->Name = L"collectionsListBox";
			this->collectionsListBox->Size = System::Drawing::Size(120, 95);
			this->collectionsListBox->TabIndex = 46;
			this->collectionsListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &collects::collectionsListBox_SelectedIndexChanged);
			// 
			// requiredLabel
			// 
			this->requiredLabel->AutoSize = true;
			this->requiredLabel->Location = System::Drawing::Point(16, 62);
			this->requiredLabel->Name = L"requiredLabel";
			this->requiredLabel->Size = System::Drawing::Size(51, 13);
			this->requiredLabel->TabIndex = 51;
			this->requiredLabel->Text = L"(required)";
			// 
			// collects
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(201, 261);
			this->Controls->Add(this->requiredLabel);
			this->Controls->Add(this->collectionOkButton);
			this->Controls->Add(this->collectionNumberTextBox);
			this->Controls->Add(this->collectionNumberLabel);
			this->Controls->Add(this->oeacnLabel);
			this->Controls->Add(this->collectionsListBox);
			this->Controls->Add(this->collectionsLabel);
			this->Controls->Add(this->searchButton);
			this->Controls->Add(this->usernameInput);
			this->Controls->Add(this->usernameLabel);
			this->Name = L"collects";
			this->Text = L"Collections";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

#pragma region Functions
		//////////////////////////////////////////////////////////////////////////////
		//																			//
		// FUNCTIONS																//
		//																			//
		//////////////////////////////////////////////////////////////////////////////

		//
		// Function queryBuilder(bool status, int _page): Build an url for specified user selects. 
		// 
		// bool status: send true if collection selected.
		// 
		// int page: send -1 if not page specified send index number if page changed.
		// 
		// returns Url String.(System::String)
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::String^ _collectionQueryBuilder(bool status, int _page);
		public: System::String^ collectionQueryBuilder(bool status, int _page) {
			// Context for string operations.
			msclr::interop::marshal_context context;

			// Create New Url String
			std::string url = "https://wallhaven.cc/api/v1/collections/";

			// Get search input
			url += context.marshal_as<std::string>(this->usernameInput->Text);

			// If collection selected, add that in url
			if (status) {
				url += "/" + context.marshal_as<std::string>(collectionIDs[this->collectionsListBox->SelectedIndex]);
			}


			// If api key specified then add url to api key
			if (apikey != gcnew String("") && apikey != nullptr) {
				// If there is a page which means not -1
				if (_page != -1) {
					// Build api key with page
					url += "?page=" + std::to_string((_page + 1));
					url += "&apikey=" + context.marshal_as<std::string>(apikey);
				}
				else {
					// Build api key without page
					url += "?apikey=" + context.marshal_as<std::string>(apikey);
				}
			}
			else {
				// If there is a page which means not -1
				if (_page != -1)
					url += "?page=" + std::to_string((_page + 1));
			}


			// Get url in System::String
			String^ _url = gcnew String(url.c_str());

			// Return _url
			return _url;
		}

		//
		// Function updateResultsLabel(String^ collectionLabelText): Updates collectionNumberLabel used in when user clicks search.
		// 
		// String^ collectionLabelText: String for collectionNumberLabel's text
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void _updateResultsLabel(String^ collectionLabelText);
		private: System::Void updateResultsLabel(String^ collectionLabelText)
		{
			// Set result label text.
			this->collectionNumberLabel->Text = collectionLabelText;
		}

		//
		// Function: disableButtons(Object^ exceptedComponentArray): this function disables all components except Object^ exceptedComponentArray(array<String^>).
		// 
		// Object^ exceptedComponentArray: array<Windows::Forms::Control^>^ array for not disabled components
		// 
		// returns void.
		//
		private: System::Void disableButtons(Object^ exceptedComponentArray) {
			// Get not disabled components.
			array<Windows::Forms::Control^>^ dComponents = dynamic_cast<array< Windows::Forms::Control^>^>(exceptedComponentArray);

			// Get all components.
			array<Windows::Forms::Control^>^ allComponents = gcnew array<Windows::Forms::Control^>(this->Controls->Count);
			this->Controls->CopyTo(allComponents, 0);

			// Delegate getComponentName and disableComponent functions
			getCName^ getName = gcnew getCName(this, &collects::getComponentName);
			dComponent^ disComponent = gcnew dComponent(this, &collects::disableComponent);

			// If disabled components excepted then this variable will be true.
			bool matched = false;

			// For loop for all components
			for (int i = 0; i < allComponents->Length; i += 1)
			{
				// For loop for disabled components
				for (int x = 0; x < dComponents->Length; x += 1)
				{
					// get selected allComponent name.
					String^ acName = dynamic_cast<String^>(Control::Invoke(getName, allComponents[i]));

					// get not disabled component name
					String^ dName = dynamic_cast<String^>(Control::Invoke(getName, dComponents[x]));

					// if not disabled component equals all component name then this is matched so don't disable this component.
					if (acName == dName)
						matched = true;
				}
				// If matched then don't disable(Look 3 line above)
				if (matched)
				{
					matched = false;
					continue;
				}
				// If not matched then disable
				else {
					Control::Invoke(disComponent, allComponents[i]);
				}
			}
		}

		//
		// Function: enableButtons(Object^ exceptedComponentArray): this function enables all components except Object^ exceptedComponentArray(array<String^>).
		// 
		// Object^ exceptedComponentArray: array<Windows::Forms::Control^>^ array for not enabled components
		// 
		// returns void.
		//
		public: System::Void enableButtons(Object^ exceptedComponentArray) {
			// This function exactly same with disableButtons. Its just enabling components instead of disabling, You can check disableButtons(look above) for this functions explainment I'll delete this duplicate later.
			array<Windows::Forms::Control^>^ eComponents = dynamic_cast<array< Windows::Forms::Control^>^>(exceptedComponentArray);
			array<Windows::Forms::Control^>^ allComponents = gcnew array<Windows::Forms::Control^>(this->Controls->Count);
			this->Controls->CopyTo(allComponents, 0);
			getCName^ getName = gcnew getCName(this, &collects::getComponentName);
			bool matched = false;
			for (int i = 0; i < allComponents->Length; i += 1)
			{
				for (int x = 0; x < eComponents->Length; x += 1)
				{
					if (allComponents[i]->Name == eComponents[x]->Name)
						matched = true;
				}
				if (matched)
				{
					matched = false;
					continue;
				}
				else
				{

				}
				{
					eComponent^ chComponent = gcnew eComponent(this, &collects::enableComponent);
					Control::Invoke(chComponent, allComponents[i]);
				}
			}
		}

		//
		// Function: getComponentName(Windows::Forms::Control^ sendedComponent): Simply gets form' component name using in threads for Invoke.
		// 
		// Windows::Forms::Control^ sendedComponent: Sended Component.
		// 
		// returns Component Name. (System::String)
		//
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::String^ getCName(Windows::Forms::Control^ sendedComponent);
		private: System::String^ getComponentName(Windows::Forms::Control^ sendedComponent) {
			// Get Component name.
			return sendedComponent->Name;
		}

		//
		// Function: enableComponent(Windows::Forms::Control^ sendedComponent): Simply enables selected Component.
		// 
		// Windows::Forms::Control^ sendedComponent: Sended Component.
		// 
		// returns void.
		//
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void eComponent(Windows::Forms::Control^ sendedComponent);
		private: System::Void enableComponent(Windows::Forms::Control^ sendedComponent) {
			// sendedComponent->Enabled = !(sendedComponent->Enabled) is not works.
			sendedComponent->Enabled = true;
		}

		//
		// Function: disableComponent(Windows::Forms::Control^ sendedComponent): Simply disables selected Component.
		// 
		// Windows::Forms::Control^ sendedComponent: Sended Component.
		// 
		// returns void.
		//
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void dComponent(Windows::Forms::Control^ sendedComponent);
		private: System::Void disableComponent(Windows::Forms::Control^ sendedComponent) {
			// sendedComponent->Enabled = !(sendedComponent->Enabled) is not works.
			sendedComponent->Enabled = false;
		}

		//
		// Function: setCollectionRange(array<Object^>^ pageArray): Clears and adds new items to collectionsListBox.
		// 
		// array<Object^>^ collectionArray: collections String array.
		// 
		// returns void.
		//
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void setCRange(array<Object^>^ collectionArray);
		private: System::Void setCollectionRange(array<Object^>^ collectionArray) {
			// Clear pagesListBox items.
			this->collectionsListBox->Items->Clear();

			// Add new items.
			this->collectionsListBox->Items->AddRange(collectionArray);
		}

		//
		// Function selectC(int selectedResult): Selects collection for invoking in threads.
		// 
		// int selectedCollection: Selected Collection Index.
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void selectC(int selectedCollection);
		private: System::Void selectCollection(int selectedCollection) {
			// True for select specified item(don't ask me why. SetSelected written by Microsoft I just use it.).
			this->collectionsListBox->SetSelected(selectedCollection, true);
		}
#pragma endregion

#pragma region Callbacks
		//////////////////////////////////////////////////////////////////////////////
		//																			//
		// CALLBACKS																//
		//																			//
		//////////////////////////////////////////////////////////////////////////////

		//
		// Callback: jsonRequestCallbackHandler(String^ notParsedJson, int httpStatusCode): handles json request callback. I simply sent this callback in JsonUtils and get back in there
		// 
		// String^ notParsedJson: It is not parsed json.
		// int httpStatusCode: And it is httpStatusCode.
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: System::Void jsonRequestCallbackHandler(String^ notParsedJson, int httpStatusCode)
		{
			// If httpStatusCode is ok.
			if (httpStatusCode == 200)
			{
				// Create new instance of JsonUtils(I send jsonProcessCallBack in exactly here.).
				JsonUtils^ prJson = gcnew JsonUtils(notParsedJson, gcnew JsonUtils::jsProcessCallback(this, &collects::jsonProcessCallBack));

				// Create new Thread of processJson.
				System::Threading::Thread^ backgroundThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(prJson, &JsonUtils::processCollectionJson));

				// Start Thread.
				backgroundThread->Start();
			}

			// If httpStatusCode is not ok.
			else
			{
				//Create empty array for not enabled components
				array<Windows::Forms::Control^>^ exceptedComponents = gcnew array<Windows::Forms::Control^>(0);

				// Create Thread for disable all components.
				System::Threading::Thread^ enableComponents = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &collects::enableButtons));

				// Start thread and send excepted components.
				enableComponents->Start(exceptedComponents);

				// Error retrieved call error callback
				collECb();
			}
		}

		//
		// Callback: jsonProcessCallBack(Object^ tupleContent): running when json processed You can check this callback in JsonUtils.
		// 
		// Object^ tupleContent: System Tuple with 7 args you can cast this tuple with dynamic_cast<Tuple<int, int, array<String^>^, array<String^>^, array<String^>^, array<String^>^, array<String^>^>^>(tupleContent);
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: System::Void jsonProcessCallBack(Object^ tupleContent) {
			// Getting contents from tuple
			Tuple<array<String^>^, array<Object^>^>^ args = dynamic_cast<Tuple<array<String^>^, array<Object^>^>^>(tupleContent);
			collectionIDs = args->Item1;
			collectionNames = args->Item2;

			// Set collection range from collectionNames string array
			setCRange^ collectionRange = gcnew setCRange(this, &collects::setCollectionRange);
			Control::Invoke(collectionRange, gcnew array<Object^>{collectionNames});

			// Select collection
			selectC^ sC = gcnew selectC(this, &collects::selectCollection);

			// If there is not a session
			if (!session) {
				selectedCollectionListBoxIndex = 0;
				session = false;
			}
			// Set collection index to List box with invoke 
			Control::Invoke(sC, selectedCollectionListBoxIndex);
		}
#pragma endregion
		//////////////////////////////////////////////////////////////////////////////
		//																			//
		// EVENTS																	//
		//																			//
		//////////////////////////////////////////////////////////////////////////////

		//
		// Event: searchButton_Click: Event for when search button clicked.
		//
		private: System::Void searchButton_Click(System::Object^ sender, System::EventArgs^ e) {
			_collectionQueryBuilder^ queryBuilder = gcnew _collectionQueryBuilder(this, &collects::collectionQueryBuilder);
			System::String^ _url = queryBuilder(false, -1);

			// Create Callback for json Request.
			JsonUtils::requestCallback^ jRequest = gcnew JsonUtils::requestCallback(this, &collects::jsonRequestCallbackHandler);

			// Create JsonUtils instance, send builded url and callback.
			JsonUtils^ jd = gcnew JsonUtils(_url, jRequest);

			//Create empty array for not disabled components
			array<Windows::Forms::Control^>^ exceptedComponents = gcnew array<Windows::Forms::Control^>(0);

			// Create Thread for disable all components.
			System::Threading::Thread^ disableComponents = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &collects::disableButtons));

			// Start thread and send excepted components.
			disableComponents->Start(exceptedComponents);

			//Calling first callback for initializing app
			collFCb();

			// Create jsonRequest Thread for request.
			System::Threading::Thread^ backgroundThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(jd, &JsonUtils::jsonRequest));

			// Start Thread.
			backgroundThread->Start();
		}

		//
		// Event: collectionsListBox_SelectedIndexChanged: Event for when collection list(known as collectionsListBox) index changed.
		//
		private: System::Void collectionsListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {

			// Create query builder delegate for building url
			_collectionQueryBuilder^ queryBuilder = gcnew _collectionQueryBuilder(this, &collects::collectionQueryBuilder);

			// Build and get url
			System::String^ _url = queryBuilder(true, page);

			// Call third callback(Which means collectionSelectedIndexChangedCallback in MyForm1.h) with url
			collTCb(_url, page);

			//Create empty array for not disabled components
			array<Windows::Forms::Control^>^ exceptedComponents = gcnew array<Windows::Forms::Control^>(0);

			// Create Thread for disable all components.
			System::Threading::Thread^ disableComponents = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &collects::disableButtons));

			// Start thread and send excepted components.
			disableComponents->Start(exceptedComponents);

			//Calling first callback for initializing app(Which means disable components.)
			collFCb();
		}

	};
}
