#pragma once

namespace Options {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	

	/// <summary>
	/// Summary for options
	/// </summary>
	public ref class options : public System::Windows::Forms::Form
	{
	public:
		options(void)
		{
			// Load App Icon Before Initialize Components
			this->Icon = gcnew System::Drawing::Icon(resourceAssembly->GetManifestResourceStream("favicon.ico"));
			InitializeComponent();

			//This line disables maximize button.
			SetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWL_STYLE, GetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWL_STYLE) & ~WS_MAXIMIZEBOX);

			// Create json instance for loading json
			nlohmann::json j;

			// If Json file exist then get to options if not do nothing.
			if (System::IO::File::Exists("whaven-wallpaper-config.json"))
			{
				// Create filestream for reading whaven-wallpaper-config.json
				std::ifstream i("whaven-wallpaper-config.json");

				// parsing json
				i >> j;

				// Initialize checked states for options
				this->saveSession->Checked = j["options"]["savesession"];
				this->saveImages->Checked = j["options"]["download"];
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~options()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Reflection::Assembly^ resourceAssembly = Reflection::Assembly::GetExecutingAssembly();


	private: System::Windows::Forms::CheckBox^ saveImages;
	private: System::Windows::Forms::Label^ warnLabel;
	private: System::Windows::Forms::Button^ saveButton;
	private: System::Windows::Forms::CheckBox^ saveSession;

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
			this->saveImages = (gcnew System::Windows::Forms::CheckBox());
			this->warnLabel = (gcnew System::Windows::Forms::Label());
			this->saveButton = (gcnew System::Windows::Forms::Button());
			this->saveSession = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// saveImages
			// 
			this->saveImages->AutoSize = true;
			this->saveImages->Checked = true;
			this->saveImages->CheckState = System::Windows::Forms::CheckState::Checked;
			this->saveImages->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->saveImages->Location = System::Drawing::Point(12, 10);
			this->saveImages->Name = L"saveImages";
			this->saveImages->Size = System::Drawing::Size(100, 21);
			this->saveImages->TabIndex = 2;
			this->saveImages->Text = L"Save Images";
			this->saveImages->UseVisualStyleBackColor = true;
			// 
			// warnLabel
			// 
			this->warnLabel->AutoSize = true;
			this->warnLabel->Location = System::Drawing::Point(9, 92);
			this->warnLabel->Name = L"warnLabel";
			this->warnLabel->Size = System::Drawing::Size(253, 39);
			this->warnLabel->TabIndex = 4;
			this->warnLabel->Text = L"This options stored in whaven-wallpaper-config.json \nIf you change to program dir"
				L"ectory move that file in \nprogram directory too";
			// 
			// saveButton
			// 
			this->saveButton->Location = System::Drawing::Point(12, 66);
			this->saveButton->Name = L"saveButton";
			this->saveButton->Size = System::Drawing::Size(75, 23);
			this->saveButton->TabIndex = 5;
			this->saveButton->Text = L"Save";
			this->saveButton->UseVisualStyleBackColor = true;
			this->saveButton->Click += gcnew System::EventHandler(this, &options::saveButton_Click);
			// 
			// saveSession
			// 
			this->saveSession->AutoSize = true;
			this->saveSession->Checked = true;
			this->saveSession->CheckState = System::Windows::Forms::CheckState::Checked;
			this->saveSession->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->saveSession->Location = System::Drawing::Point(12, 38);
			this->saveSession->Name = L"saveSession";
			this->saveSession->Size = System::Drawing::Size(204, 21);
			this->saveSession->TabIndex = 3;
			this->saveSession->Text = L"Save My Search When Quitting";
			this->saveSession->UseVisualStyleBackColor = true;
			// 
			// options
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(282, 137);
			this->Controls->Add(this->saveButton);
			this->Controls->Add(this->warnLabel);
			this->Controls->Add(this->saveSession);
			this->Controls->Add(this->saveImages);
			this->Name = L"options";
			this->Text = L"Options";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

#pragma region Variables
		//////////////////////////////////////////////////////////////////////////////
		//																			//
		// VARIABLES																//
		//																			//
		//////////////////////////////////////////////////////////////////////////////


#pragma endregion

#pragma region Functions
		//////////////////////////////////////////////////////////////////////////////
		//																			//
		// FUNCTIONS																//
		//																			//
		//////////////////////////////////////////////////////////////////////////////

#pragma endregion

		//////////////////////////////////////////////////////////////////////////////
		//																			//
		// EVENTS																	//
		//																			//
		//////////////////////////////////////////////////////////////////////////////

		//
		// Disclaimer: I don't explain how events works(In function explanation). You can check that in Microsoft Docs.
		//

		//
		// Event: saveButton_Click: Event for when save button clicked.
		//
		private: System::Void saveButton_Click(System::Object^ sender, System::EventArgs^ e) {
			// Marshal context for type conversations
			msclr::interop::marshal_context context;

			// Json instance for loading json
			nlohmann::json j;

			// Create filestream instance for loading whaven-wallpaper-config.json
			std::ifstream i("whaven-wallpaper-config.json");

			// Parse json from file
			i >> j;

			// Save options in json instance
			j["options"]["savesession"] = this->saveSession->Checked;
			j["options"]["download"] = this->saveImages->Checked;

			// Create file stream for saving options
			std::ofstream o("whaven-wallpaper-config.json");

			// Save options in file.
			o << std::setw(4) << j << std::endl;

			// Change options for saying app options changed
			Application::Restart();
		}
};
}
