#pragma once

// TODO random seeding
// TODO startup shortcut
// TODO json in appdata

namespace WhavenWallpaper {
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Diagnostics;
	using namespace nlohmann;

	public ref class MyForm : public System::Windows::Forms::Form
	{

	public:
		MyForm(void)
		{
			// Load App Icon Before Initialize Components
			this->Icon = gcnew System::Drawing::Icon(resourceAssembly->GetManifestResourceStream("favicon.ico"));

			// Initialize components
			InitializeComponent();

			// Add notify icon.
			this->notifyIcon1->Icon = gcnew System::Drawing::Icon(resourceAssembly->GetManifestResourceStream("favicon.ico"));
			this->notifyIcon1->Visible = false;
			
			//
			//TODO: Add the constructor code here
			//
			this->orderDropdown->SelectedIndex = 0;
			this->sortingDropdown->SelectedIndex = 0;
			this->counterDropDown->SelectedIndex = 1;
			disableResultComponents();

			//This line disables maximize button.
			SetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWL_STYLE, GetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWL_STYLE) & ~WS_MAXIMIZEBOX);

			// Create json instance for loading json file.
			nlohmann::json j;

			// Create bool instance for first running session we are checking this in session condution
			bool saveSessionInit = false;

			// If Json file exist.
			if (System::IO::File::Exists("whaven-wallpaper-config.json"))
			{
				// Then create filestream for json.
				std::ifstream i("whaven-wallpaper-config.json");

				// Get filestrem to json.
				i >> j;

				// Get apikey
				apikey = gcnew System::String((j["options"]["apikey"].get<std::string>()).c_str());

				// Get download images option.
				download = j["options"]["download"];

				// Get save when quit option.
				savesession = j["options"]["savesession"];
				

			}
			// If Json file not exist
			else
			{
				// Create write stream for json
				std::ofstream o("whaven-wallpaper-config.json");

				// Set defaults to json
				j["options"]["apikey"] = "";
				j["options"]["download"] = true;
				j["options"]["savesession"] = true;

				// This is app's first time so don't enter session even session is true
				saveSessionInit = true;

				// Write json
				o << std::setw(4) << j << std::endl;
			}

			// If there is no api key then dont show nsfw checkbox
			if (apikey != gcnew String("") && apikey != nullptr) {
				this->nsfwCheckBox->Visible = true;
			}

			// If save option checked and this is not apps first time and if there is a session section in json file.
			if (savesession && !saveSessionInit && j["session"].is_object())
			{
				// Getting resolution sort
				String^ resShort = gcnew String((j["session"]["resShort"].get<std::string>()).c_str());

				//Getting search query
				this->textBox1->Text = gcnew String((j["session"]["search"].get<std::string>()).c_str());

				// Getting categories
				this->generalCheckBox->Checked = j["session"]["general"];
				this->animeCheckBox->Checked = j["session"]["anime"];
				this->peopleCheckBox->Checked = j["session"]["people"];

				// Getting purity
				this->sfwCheckBox->Checked = j["session"]["sfw"];
				this->sketchyCheckBox->Checked = j["session"]["sketchy"];
				this->nsfwCheckBox->Checked = j["session"]["nsfw"];

				// Getting sorting index
				if (j["session"]["sortingIndex"].is_null())
					this->sortingDropdown->SelectedIndex = 0;
				else
					this->sortingDropdown->SelectedIndex = j["session"]["sortingIndex"];

				// Getting order index
				if (j["session"]["orderIndex"].is_null())
					this->orderDropdown->SelectedIndex = 0;
				else 
					this->orderDropdown->SelectedIndex = j["session"]["orderIndex"];

				// Setting resolution sorting
				if (resShort == gcnew String("atleast"))
				{
					this->atLeastRadioButton->Checked = true;
					this->exactlyRadioButton->Checked = false;
					this->noSelectRadioButton->Checked = false;
				}
				else if (resShort == gcnew String("exactly"))
				{
					this->atLeastRadioButton->Checked = false;
					this->exactlyRadioButton->Checked = true;
					this->noSelectRadioButton->Checked = false;
				}
				else if (resShort = gcnew String("noResShort"))
				{
					this->atLeastRadioButton->Checked = false;
					this->exactlyRadioButton->Checked = false;
					this->noSelectRadioButton->Checked = true;
				}
				// Setting resolution inputs
				this->resolutionXInput->Text = gcnew String((j["session"]["resXInput"].get<std::string>()).c_str());
				this->resolutionYInput->Text = gcnew String((j["session"]["resYInput"].get<std::string>()).c_str());
				this->autoCounterInput->Text = gcnew String((j["session"]["autoInput"].get<std::string>()).c_str());

				// Setting app init true for setting app and result
				appSessionInit = true;
				// Getting page number in index number
				if (j["session"]["pageIndex"].is_null())
					page = 0;
				else
					page = j["session"]["pageIndex"];

				// Getting result number in index number
				if (j["session"]["resultIndex"].is_null())
					result = 0;
				else
					result = j["session"]["resultIndex"];

				// Setting page this automatically sets results too
				this->pagesListBox_SelectedIndexChanged("", nullptr);

				// Getting  auto select next image  counter dropdown index
				if (j["session"]["autoDropdownIndex"].is_null())
					this->counterDropDown->SelectedIndex = 0;
				else
					this->counterDropDown->SelectedIndex = j["session"]["autoDropdownIndex"];

				// Getting auto select next image check state
				this->autoCheckBox->Checked = j["session"]["autoCheckBox"];

			} 
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{

			// If user selects to save current session and if app changed.
			if (savesession && changed) {
				// Create json instance for saving app state.
				nlohmann::json j;

				// Create context for type casting.
				msclr::interop::marshal_context context;

				// Create resolution short string. We are using this in constructor and setting this in down there.
				String^ resShort = gcnew String("");

				// Create virtual page index for getting page index.
				int _page = page;

				// Create virtual result index for getting result index.
				int _result = result;

				// Create file stream for getting current json.
				std::ifstream i("whaven-wallpaper-config.json");
				
				// Write file to json.
				i >> j;

				// Setting search state to json.
				j["session"]["search"] = context.marshal_as<const char*>(this->textBox1->Text);

				// Setting categories to json.
				j["session"]["general"] = this->generalCheckBox->Checked;
				j["session"]["anime"] = this->animeCheckBox->Checked;
				j["session"]["people"] = this->peopleCheckBox->Checked;

				// Setting purity to json.
				j["session"]["sfw"] = this->sfwCheckBox->Checked;
				j["session"]["sketchy"] = this->sketchyCheckBox->Checked;
				j["session"]["nsfw"] = this->nsfwCheckBox->Checked;

				// Setting sorting indexes to json.
				j["session"]["sortingIndex"] = this->sortingDropdown->SelectedIndex;
				j["session"]["orderIndex"] = this->orderDropdown->SelectedIndex;

				// Setting sorting state to json.
				if (this->atLeastRadioButton->Checked)
					resShort = gcnew String("atleast");
				else if (this->exactlyRadioButton->Checked)
					resShort = gcnew String("exactly");
				else if (this->noSelectRadioButton->Checked)
					resShort = gcnew String("noResShort");
				j["session"]["resShort"] = context.marshal_as<const char*>(resShort);

				// Setting resolution inputs to json.
				j["session"]["resXInput"] = this->resolutionXInput->Text == gcnew String("") ? "1920" : context.marshal_as<const char*>(this->resolutionXInput->Text);
				j["session"]["resYInput"] = this->resolutionYInput->Text == gcnew String("") ? "1080" : context.marshal_as<const char*>(this->resolutionYInput->Text);

				// Setting page and result index to json.
				j["session"]["pageIndex"] = _page;
				j["session"]["resultIndex"] = _result;

				// Setting automatically select next option to json for saving.
				j["session"]["autoCheckBox"] = this->autoCheckBox->Checked;
				j["session"]["autoInput"] = this->autoCounterInput->Text == gcnew String("") ? "30" : context.marshal_as<const char*>(this->autoCounterInput->Text);
				j["session"]["autoDropdownIndex"] = this->counterDropDown->SelectedIndex;

				// Create write stream for json.
				std::ofstream o("whaven-wallpaper-config.json");

				// Write json to file.
				o << std::setw(4) << j << std::endl;
			}

			// If there is a thread for select automatically next image abort that.
			if(counterThread != nullptr)
				counterThread->Abort();

			// Delete components.
			if (components) {
				delete components;
			}
		}
	private: System::Reflection::Assembly^ resourceAssembly = Reflection::Assembly::GetExecutingAssembly();
	private: System::Windows::Forms::Button^ searchButton;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Label^ categoriesLabel;
	private: System::Windows::Forms::Label^ purityLabel;
	private: System::Windows::Forms::Label^ sortingLabel;
	private: System::Windows::Forms::Label^ miniSortingLabel;
	private: System::Windows::Forms::Label^ miniOrderLabel;
	private: System::Windows::Forms::CheckBox^ generalCheckBox;
	private: System::Windows::Forms::CheckBox^ animeCheckBox;
	private: System::Windows::Forms::CheckBox^ peopleCheckBox;
	private: System::Windows::Forms::CheckBox^ sfwCheckBox;
	private: System::Windows::Forms::CheckBox^ sketchyCheckBox;
	private: System::Windows::Forms::CheckBox^ nsfwCheckBox;
	private: System::Windows::Forms::ComboBox^ sortingDropdown;
	private: System::Windows::Forms::ComboBox^ orderDropdown;
	private: System::Windows::Forms::LinkLabel^ pathLinkLabel;
	private: System::Windows::Forms::CheckBox^ autoCheckBox;
	private: System::Windows::Forms::StatusStrip^ statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^ toolStripStatusLabel1;
	private: System::Windows::Forms::StatusStrip^ statusStrip2;
	private: System::Windows::Forms::ToolStripProgressBar^ toolStripProgressBar1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ pagesLabel;
	private: System::Windows::Forms::ListBox^ pagesListBox;
	private: System::Windows::Forms::Label^ pageNumberLabel;
	private: System::Windows::Forms::Label^ oeapnLabel;
	private: System::Windows::Forms::Label^ resultsBigLabel;
	private: System::Windows::Forms::Label^ resolutionSeperatorLabel;
	private: System::Windows::Forms::TextBox^ resolutionYInput;
	private: System::Windows::Forms::TextBox^ resolutionXInput;
	private: System::Windows::Forms::Label^ resultNumberLabel;
	private: System::Windows::Forms::Label^ oearnLabel;
	private: System::Windows::Forms::Label^ resultsLabel;
	private: System::Windows::Forms::ListBox^ resultsListBox;
	private: System::Windows::Forms::TextBox^ pageNumberTextBox;
	private: System::Windows::Forms::RadioButton^ exactlyRadioButton;
	private: System::Windows::Forms::RadioButton^ atLeastRadioButton;
	private: System::Windows::Forms::TextBox^ resultNumberTextbox;
	private: System::Windows::Forms::RadioButton^ noSelectRadioButton;
	private: System::Windows::Forms::Label^ pixelsLabel;
	private: System::Windows::Forms::Label^ imageLinkLabel;
	private: System::Windows::Forms::Button^ resultOkButton;
	private: System::Windows::Forms::Button^ pageOkButton;
	private: System::Windows::Forms::TextBox^ autoCounterInput;
	private: System::Windows::Forms::ComboBox^ counterDropDown;
	private: System::Windows::Forms::NotifyIcon^ notifyIcon1;
	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::Button^ optionsButton;
	private: System::ComponentModel::IContainer^ components;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->categoriesLabel = (gcnew System::Windows::Forms::Label());
			this->purityLabel = (gcnew System::Windows::Forms::Label());
			this->sortingLabel = (gcnew System::Windows::Forms::Label());
			this->miniSortingLabel = (gcnew System::Windows::Forms::Label());
			this->miniOrderLabel = (gcnew System::Windows::Forms::Label());
			this->generalCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->animeCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->peopleCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->sfwCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->sketchyCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->nsfwCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->sortingDropdown = (gcnew System::Windows::Forms::ComboBox());
			this->orderDropdown = (gcnew System::Windows::Forms::ComboBox());
			this->searchButton = (gcnew System::Windows::Forms::Button());
			this->pathLinkLabel = (gcnew System::Windows::Forms::LinkLabel());
			this->autoCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->statusStrip2 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripProgressBar1 = (gcnew System::Windows::Forms::ToolStripProgressBar());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->pagesListBox = (gcnew System::Windows::Forms::ListBox());
			this->pagesLabel = (gcnew System::Windows::Forms::Label());
			this->resultsBigLabel = (gcnew System::Windows::Forms::Label());
			this->oeapnLabel = (gcnew System::Windows::Forms::Label());
			this->pageNumberLabel = (gcnew System::Windows::Forms::Label());
			this->pageNumberTextBox = (gcnew System::Windows::Forms::TextBox());
			this->resultsListBox = (gcnew System::Windows::Forms::ListBox());
			this->resultsLabel = (gcnew System::Windows::Forms::Label());
			this->resultNumberLabel = (gcnew System::Windows::Forms::Label());
			this->oearnLabel = (gcnew System::Windows::Forms::Label());
			this->resolutionXInput = (gcnew System::Windows::Forms::TextBox());
			this->resolutionYInput = (gcnew System::Windows::Forms::TextBox());
			this->resolutionSeperatorLabel = (gcnew System::Windows::Forms::Label());
			this->atLeastRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->exactlyRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->pixelsLabel = (gcnew System::Windows::Forms::Label());
			this->noSelectRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->resultNumberTextbox = (gcnew System::Windows::Forms::TextBox());
			this->pageOkButton = (gcnew System::Windows::Forms::Button());
			this->resultOkButton = (gcnew System::Windows::Forms::Button());
			this->imageLinkLabel = (gcnew System::Windows::Forms::Label());
			this->autoCounterInput = (gcnew System::Windows::Forms::TextBox());
			this->counterDropDown = (gcnew System::Windows::Forms::ComboBox());
			this->notifyIcon1 = (gcnew System::Windows::Forms::NotifyIcon(this->components));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->optionsButton = (gcnew System::Windows::Forms::Button());
			this->statusStrip1->SuspendLayout();
			this->statusStrip2->SuspendLayout();
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(19, 36);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(354, 20);
			this->textBox1->TabIndex = 3;
			this->textBox1->Tag = L"Data";
			this->textBox1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::textBox1_KeyDown);
			// 
			// categoriesLabel
			// 
			this->categoriesLabel->AutoSize = true;
			this->categoriesLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold));
			this->categoriesLabel->Location = System::Drawing::Point(12, 88);
			this->categoriesLabel->Name = L"categoriesLabel";
			this->categoriesLabel->Size = System::Drawing::Size(362, 24);
			this->categoriesLabel->TabIndex = 4;
			this->categoriesLabel->Text = L"Categories  ————————————————";
			// 
			// purityLabel
			// 
			this->purityLabel->AutoSize = true;
			this->purityLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold));
			this->purityLabel->Location = System::Drawing::Point(15, 136);
			this->purityLabel->Name = L"purityLabel";
			this->purityLabel->Size = System::Drawing::Size(359, 24);
			this->purityLabel->TabIndex = 5;
			this->purityLabel->Text = L"Purity  ———————————————————";
			// 
			// sortingLabel
			// 
			this->sortingLabel->AutoSize = true;
			this->sortingLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold));
			this->sortingLabel->Location = System::Drawing::Point(16, 188);
			this->sortingLabel->Name = L"sortingLabel";
			this->sortingLabel->Size = System::Drawing::Size(358, 24);
			this->sortingLabel->TabIndex = 6;
			this->sortingLabel->Text = L"Sorting  ——————————————————";
			// 
			// miniSortingLabel
			// 
			this->miniSortingLabel->AutoSize = true;
			this->miniSortingLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->miniSortingLabel->Location = System::Drawing::Point(13, 216);
			this->miniSortingLabel->Name = L"miniSortingLabel";
			this->miniSortingLabel->Size = System::Drawing::Size(57, 17);
			this->miniSortingLabel->TabIndex = 7;
			this->miniSortingLabel->Text = L"Sorting:";
			// 
			// miniOrderLabel
			// 
			this->miniOrderLabel->AutoSize = true;
			this->miniOrderLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->miniOrderLabel->Location = System::Drawing::Point(213, 216);
			this->miniOrderLabel->Name = L"miniOrderLabel";
			this->miniOrderLabel->Size = System::Drawing::Size(49, 17);
			this->miniOrderLabel->TabIndex = 8;
			this->miniOrderLabel->Text = L"Order:";
			// 
			// generalCheckBox
			// 
			this->generalCheckBox->AutoSize = true;
			this->generalCheckBox->Checked = true;
			this->generalCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->generalCheckBox->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->generalCheckBox->Location = System::Drawing::Point(16, 116);
			this->generalCheckBox->Name = L"generalCheckBox";
			this->generalCheckBox->Size = System::Drawing::Size(72, 21);
			this->generalCheckBox->TabIndex = 10;
			this->generalCheckBox->Text = L"General";
			this->generalCheckBox->UseVisualStyleBackColor = true;
			// 
			// animeCheckBox
			// 
			this->animeCheckBox->AutoSize = true;
			this->animeCheckBox->Checked = true;
			this->animeCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->animeCheckBox->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->animeCheckBox->Location = System::Drawing::Point(162, 116);
			this->animeCheckBox->Name = L"animeCheckBox";
			this->animeCheckBox->Size = System::Drawing::Size(65, 21);
			this->animeCheckBox->TabIndex = 11;
			this->animeCheckBox->Text = L"Anime";
			this->animeCheckBox->UseVisualStyleBackColor = true;
			// 
			// peopleCheckBox
			// 
			this->peopleCheckBox->AutoSize = true;
			this->peopleCheckBox->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->peopleCheckBox->Location = System::Drawing::Point(308, 116);
			this->peopleCheckBox->Name = L"peopleCheckBox";
			this->peopleCheckBox->Size = System::Drawing::Size(65, 21);
			this->peopleCheckBox->TabIndex = 12;
			this->peopleCheckBox->Text = L"People";
			this->peopleCheckBox->UseVisualStyleBackColor = true;
			// 
			// sfwCheckBox
			// 
			this->sfwCheckBox->AutoSize = true;
			this->sfwCheckBox->Checked = true;
			this->sfwCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->sfwCheckBox->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->sfwCheckBox->Location = System::Drawing::Point(16, 168);
			this->sfwCheckBox->Name = L"sfwCheckBox";
			this->sfwCheckBox->Size = System::Drawing::Size(47, 21);
			this->sfwCheckBox->TabIndex = 13;
			this->sfwCheckBox->Text = L"sfw";
			this->sfwCheckBox->UseVisualStyleBackColor = true;
			// 
			// sketchyCheckBox
			// 
			this->sketchyCheckBox->AutoSize = true;
			this->sketchyCheckBox->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->sketchyCheckBox->Location = System::Drawing::Point(162, 168);
			this->sketchyCheckBox->Name = L"sketchyCheckBox";
			this->sketchyCheckBox->Size = System::Drawing::Size(72, 21);
			this->sketchyCheckBox->TabIndex = 14;
			this->sketchyCheckBox->Text = L"sketchy";
			this->sketchyCheckBox->UseVisualStyleBackColor = true;
			// 
			// nsfwCheckBox
			// 
			this->nsfwCheckBox->AutoSize = true;
			this->nsfwCheckBox->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->nsfwCheckBox->Location = System::Drawing::Point(308, 168);
			this->nsfwCheckBox->Name = L"nsfwCheckBox";
			this->nsfwCheckBox->Size = System::Drawing::Size(54, 21);
			this->nsfwCheckBox->TabIndex = 15;
			this->nsfwCheckBox->Text = L"nsfw";
			this->nsfwCheckBox->UseVisualStyleBackColor = true;
			this->nsfwCheckBox->Visible = false;
			// 
			// sortingDropdown
			// 
			this->sortingDropdown->AccessibleRole = System::Windows::Forms::AccessibleRole::ScrollBar;
			this->sortingDropdown->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->sortingDropdown->Items->AddRange(gcnew cli::array< System::Object^  >(6) {
				L"Date Added", L"Relevance", L"Random", L"Views",
					L"Favorites", L"Toplist"
			});
			this->sortingDropdown->Location = System::Drawing::Point(16, 236);
			this->sortingDropdown->Name = L"sortingDropdown";
			this->sortingDropdown->Size = System::Drawing::Size(156, 21);
			this->sortingDropdown->TabIndex = 16;
			// 
			// orderDropdown
			// 
			this->orderDropdown->AccessibleRole = System::Windows::Forms::AccessibleRole::ScrollBar;
			this->orderDropdown->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->orderDropdown->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Descending", L"Ascending" });
			this->orderDropdown->Location = System::Drawing::Point(216, 236);
			this->orderDropdown->Name = L"orderDropdown";
			this->orderDropdown->Size = System::Drawing::Size(146, 21);
			this->orderDropdown->TabIndex = 17;
			// 
			// searchButton
			// 
			this->searchButton->Location = System::Drawing::Point(19, 62);
			this->searchButton->Name = L"searchButton";
			this->searchButton->Size = System::Drawing::Size(354, 23);
			this->searchButton->TabIndex = 20;
			this->searchButton->Text = L"Search";
			this->searchButton->UseVisualStyleBackColor = true;
			this->searchButton->Click += gcnew System::EventHandler(this, &MyForm::searchButton_Click);
			// 
			// pathLinkLabel
			// 
			this->pathLinkLabel->AutoSize = true;
			this->pathLinkLabel->Location = System::Drawing::Point(84, 559);
			this->pathLinkLabel->Name = L"pathLinkLabel";
			this->pathLinkLabel->Size = System::Drawing::Size(109, 13);
			this->pathLinkLabel->TabIndex = 21;
			this->pathLinkLabel->TabStop = true;
			this->pathLinkLabel->Text = L"https:://wallhaven.cc";
			this->pathLinkLabel->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &MyForm::pathLinkLabelClicked);
			// 
			// autoCheckBox
			// 
			this->autoCheckBox->AutoSize = true;
			this->autoCheckBox->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->autoCheckBox->Location = System::Drawing::Point(16, 525);
			this->autoCheckBox->Name = L"autoCheckBox";
			this->autoCheckBox->Size = System::Drawing::Size(233, 21);
			this->autoCheckBox->TabIndex = 22;
			this->autoCheckBox->Text = L"Select Next Image Automatically in:";
			this->autoCheckBox->UseVisualStyleBackColor = true;
			this->autoCheckBox->CheckedChanged += gcnew System::EventHandler(this, &MyForm::repeatCheckBox_CheckedChanged);
			// 
			// statusStrip1
			// 
			this->statusStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripStatusLabel1 });
			this->statusStrip1->Location = System::Drawing::Point(0, 583);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->statusStrip1->Size = System::Drawing::Size(387, 22);
			this->statusStrip1->TabIndex = 23;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8));
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(103, 17);
			this->toolStripStatusLabel1->Text = L"Application Ready.";
			// 
			// statusStrip2
			// 
			this->statusStrip2->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->statusStrip2->AutoSize = false;
			this->statusStrip2->Dock = System::Windows::Forms::DockStyle::None;
			this->statusStrip2->ImageScalingSize = System::Drawing::Size(20, 20);
			this->statusStrip2->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripProgressBar1 });
			this->statusStrip2->Location = System::Drawing::Point(267, 583);
			this->statusStrip2->Name = L"statusStrip2";
			this->statusStrip2->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->statusStrip2->Size = System::Drawing::Size(118, 22);
			this->statusStrip2->TabIndex = 24;
			this->statusStrip2->Text = L"statusStrip2";
			// 
			// toolStripProgressBar1
			// 
			this->toolStripProgressBar1->Name = L"toolStripProgressBar1";
			this->toolStripProgressBar1->Size = System::Drawing::Size(100, 16);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold));
			this->label1->Location = System::Drawing::Point(15, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(358, 24);
			this->label1->TabIndex = 25;
			this->label1->Text = L"Search  ——————————————————";
			// 
			// pagesListBox
			// 
			this->pagesListBox->FormattingEnabled = true;
			this->pagesListBox->Location = System::Drawing::Point(16, 371);
			this->pagesListBox->Name = L"pagesListBox";
			this->pagesListBox->Size = System::Drawing::Size(120, 95);
			this->pagesListBox->TabIndex = 27;
			this->pagesListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::pagesListBox_SelectedIndexChanged);
			// 
			// pagesLabel
			// 
			this->pagesLabel->AutoSize = true;
			this->pagesLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->pagesLabel->Location = System::Drawing::Point(13, 351);
			this->pagesLabel->Name = L"pagesLabel";
			this->pagesLabel->Size = System::Drawing::Size(52, 17);
			this->pagesLabel->TabIndex = 28;
			this->pagesLabel->Text = L"Pages:";
			// 
			// resultsBigLabel
			// 
			this->resultsBigLabel->AutoSize = true;
			this->resultsBigLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold));
			this->resultsBigLabel->Location = System::Drawing::Point(12, 327);
			this->resultsBigLabel->Name = L"resultsBigLabel";
			this->resultsBigLabel->Size = System::Drawing::Size(360, 24);
			this->resultsBigLabel->TabIndex = 29;
			this->resultsBigLabel->Text = L"Results  ——————————————————";
			// 
			// oeapnLabel
			// 
			this->oeapnLabel->AutoSize = true;
			this->oeapnLabel->Location = System::Drawing::Point(13, 469);
			this->oeapnLabel->Name = L"oeapnLabel";
			this->oeapnLabel->Size = System::Drawing::Size(123, 13);
			this->oeapnLabel->TabIndex = 30;
			this->oeapnLabel->Text = L"or Enter a page Number:";
			// 
			// pageNumberLabel
			// 
			this->pageNumberLabel->AutoSize = true;
			this->pageNumberLabel->Location = System::Drawing::Point(13, 482);
			this->pageNumberLabel->Name = L"pageNumberLabel";
			this->pageNumberLabel->Size = System::Drawing::Size(46, 13);
			this->pageNumberLabel->TabIndex = 31;
			this->pageNumberLabel->Text = L"(0 page)";
			// 
			// pageNumberTextBox
			// 
			this->pageNumberTextBox->Location = System::Drawing::Point(16, 499);
			this->pageNumberTextBox->Name = L"pageNumberTextBox";
			this->pageNumberTextBox->Size = System::Drawing::Size(120, 20);
			this->pageNumberTextBox->TabIndex = 32;
			this->pageNumberTextBox->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::pageNumberTextBox_KeyDown);
			this->pageNumberTextBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::pageNumberTextBox_KeyPress);
			// 
			// resultsListBox
			// 
			this->resultsListBox->FormattingEnabled = true;
			this->resultsListBox->Location = System::Drawing::Point(216, 371);
			this->resultsListBox->Name = L"resultsListBox";
			this->resultsListBox->Size = System::Drawing::Size(120, 95);
			this->resultsListBox->TabIndex = 33;
			this->resultsListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::resultsListBox_SelectedIndexChanged);
			// 
			// resultsLabel
			// 
			this->resultsLabel->AutoSize = true;
			this->resultsLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->resultsLabel->Location = System::Drawing::Point(213, 351);
			this->resultsLabel->Name = L"resultsLabel";
			this->resultsLabel->Size = System::Drawing::Size(59, 17);
			this->resultsLabel->TabIndex = 34;
			this->resultsLabel->Text = L"Results:";
			// 
			// resultNumberLabel
			// 
			this->resultNumberLabel->AutoSize = true;
			this->resultNumberLabel->Location = System::Drawing::Point(213, 482);
			this->resultNumberLabel->Name = L"resultNumberLabel";
			this->resultNumberLabel->Size = System::Drawing::Size(47, 13);
			this->resultNumberLabel->TabIndex = 36;
			this->resultNumberLabel->Text = L"(0 result)";
			// 
			// oearnLabel
			// 
			this->oearnLabel->AutoSize = true;
			this->oearnLabel->Location = System::Drawing::Point(213, 469);
			this->oearnLabel->Name = L"oearnLabel";
			this->oearnLabel->Size = System::Drawing::Size(124, 13);
			this->oearnLabel->TabIndex = 35;
			this->oearnLabel->Text = L"or Enter a result Number:";
			// 
			// resolutionXInput
			// 
			this->resolutionXInput->Enabled = false;
			this->resolutionXInput->Location = System::Drawing::Point(16, 293);
			this->resolutionXInput->Name = L"resolutionXInput";
			this->resolutionXInput->Size = System::Drawing::Size(103, 20);
			this->resolutionXInput->TabIndex = 37;
			this->resolutionXInput->Text = L"1920";
			this->resolutionXInput->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::resolutionXInput_KeyPress);
			// 
			// resolutionYInput
			// 
			this->resolutionYInput->Enabled = false;
			this->resolutionYInput->Location = System::Drawing::Point(145, 293);
			this->resolutionYInput->Name = L"resolutionYInput";
			this->resolutionYInput->Size = System::Drawing::Size(103, 20);
			this->resolutionYInput->TabIndex = 38;
			this->resolutionYInput->Text = L"1080";
			this->resolutionYInput->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::resolutionYInput_KeyPress);
			// 
			// resolutionSeperatorLabel
			// 
			this->resolutionSeperatorLabel->AutoSize = true;
			this->resolutionSeperatorLabel->Enabled = false;
			this->resolutionSeperatorLabel->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->resolutionSeperatorLabel->Location = System::Drawing::Point(125, 295);
			this->resolutionSeperatorLabel->Name = L"resolutionSeperatorLabel";
			this->resolutionSeperatorLabel->Size = System::Drawing::Size(14, 17);
			this->resolutionSeperatorLabel->TabIndex = 39;
			this->resolutionSeperatorLabel->Text = L"x";
			// 
			// atLeastRadioButton
			// 
			this->atLeastRadioButton->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->atLeastRadioButton->Location = System::Drawing::Point(16, 268);
			this->atLeastRadioButton->Name = L"atLeastRadioButton";
			this->atLeastRadioButton->Size = System::Drawing::Size(84, 17);
			this->atLeastRadioButton->TabIndex = 0;
			this->atLeastRadioButton->Text = L"At Least:";
			this->atLeastRadioButton->UseVisualStyleBackColor = true;
			this->atLeastRadioButton->CheckedChanged += gcnew System::EventHandler(this, &MyForm::atLeastRadioButton_CheckedChanged);
			// 
			// exactlyRadioButton
			// 
			this->exactlyRadioButton->AutoSize = true;
			this->exactlyRadioButton->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->exactlyRadioButton->Location = System::Drawing::Point(136, 266);
			this->exactlyRadioButton->Name = L"exactlyRadioButton";
			this->exactlyRadioButton->Size = System::Drawing::Size(68, 21);
			this->exactlyRadioButton->TabIndex = 40;
			this->exactlyRadioButton->Text = L"Exactly";
			this->exactlyRadioButton->UseVisualStyleBackColor = true;
			this->exactlyRadioButton->CheckedChanged += gcnew System::EventHandler(this, &MyForm::exactlyRadioButton_CheckedChanged);
			// 
			// pixelsLabel
			// 
			this->pixelsLabel->AutoSize = true;
			this->pixelsLabel->Enabled = false;
			this->pixelsLabel->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->pixelsLabel->Location = System::Drawing::Point(254, 295);
			this->pixelsLabel->Name = L"pixelsLabel";
			this->pixelsLabel->Size = System::Drawing::Size(40, 17);
			this->pixelsLabel->TabIndex = 41;
			this->pixelsLabel->Text = L"pixels";
			// 
			// noSelectRadioButton
			// 
			this->noSelectRadioButton->AutoSize = true;
			this->noSelectRadioButton->Checked = true;
			this->noSelectRadioButton->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->noSelectRadioButton->Location = System::Drawing::Point(222, 266);
			this->noSelectRadioButton->Name = L"noSelectRadioButton";
			this->noSelectRadioButton->Size = System::Drawing::Size(151, 21);
			this->noSelectRadioButton->TabIndex = 42;
			this->noSelectRadioButton->TabStop = true;
			this->noSelectRadioButton->Text = L"No Resolution Sorting";
			this->noSelectRadioButton->UseVisualStyleBackColor = true;
			this->noSelectRadioButton->CheckedChanged += gcnew System::EventHandler(this, &MyForm::noSelectRadioButton_CheckedChanged);
			// 
			// resultNumberTextbox
			// 
			this->resultNumberTextbox->Location = System::Drawing::Point(216, 499);
			this->resultNumberTextbox->Name = L"resultNumberTextbox";
			this->resultNumberTextbox->Size = System::Drawing::Size(120, 20);
			this->resultNumberTextbox->TabIndex = 43;
			this->resultNumberTextbox->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::resultNumberTextbox_KeyDown);
			this->resultNumberTextbox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::resultNumberTextbox_KeyPress);
			// 
			// pageOkButton
			// 
			this->pageOkButton->Location = System::Drawing::Point(142, 497);
			this->pageOkButton->Name = L"pageOkButton";
			this->pageOkButton->Size = System::Drawing::Size(43, 23);
			this->pageOkButton->TabIndex = 45;
			this->pageOkButton->Text = L"Ok";
			this->pageOkButton->UseVisualStyleBackColor = true;
			this->pageOkButton->Click += gcnew System::EventHandler(this, &MyForm::pageOkButton_Click);
			// 
			// resultOkButton
			// 
			this->resultOkButton->Location = System::Drawing::Point(342, 497);
			this->resultOkButton->Name = L"resultOkButton";
			this->resultOkButton->Size = System::Drawing::Size(43, 23);
			this->resultOkButton->TabIndex = 46;
			this->resultOkButton->Text = L"Ok";
			this->resultOkButton->UseVisualStyleBackColor = true;
			this->resultOkButton->Click += gcnew System::EventHandler(this, &MyForm::resultOkButton_Click);
			// 
			// imageLinkLabel
			// 
			this->imageLinkLabel->AutoSize = true;
			this->imageLinkLabel->Location = System::Drawing::Point(16, 559);
			this->imageLinkLabel->Name = L"imageLinkLabel";
			this->imageLinkLabel->Size = System::Drawing::Size(62, 13);
			this->imageLinkLabel->TabIndex = 47;
			this->imageLinkLabel->Text = L"Image Link:";
			// 
			// autoCounterInput
			// 
			this->autoCounterInput->Enabled = false;
			this->autoCounterInput->Location = System::Drawing::Point(245, 525);
			this->autoCounterInput->Name = L"autoCounterInput";
			this->autoCounterInput->Size = System::Drawing::Size(62, 20);
			this->autoCounterInput->TabIndex = 48;
			this->autoCounterInput->Text = L"30";
			this->autoCounterInput->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::autoCounterInput_KeyDown);
			this->autoCounterInput->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::autoCounterInput_KeyPress);
			// 
			// counterDropDown
			// 
			this->counterDropDown->AccessibleRole = System::Windows::Forms::AccessibleRole::ScrollBar;
			this->counterDropDown->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->counterDropDown->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"sec", L"min", L"hr" });
			this->counterDropDown->Location = System::Drawing::Point(313, 524);
			this->counterDropDown->Name = L"counterDropDown";
			this->counterDropDown->Size = System::Drawing::Size(49, 21);
			this->counterDropDown->TabIndex = 49;
			// 
			// notifyIcon1
			// 
			this->notifyIcon1->Text = L"Whaven Wallpaper";
			this->notifyIcon1->Visible = true;
			this->notifyIcon1->Click += gcnew System::EventHandler(this, &MyForm::notifyIcon1_Click);
			// 
			// optionsButton
			// 
			this->optionsButton->Location = System::Drawing::Point(245, 554);
			this->optionsButton->Name = L"optionsButton";
			this->optionsButton->Size = System::Drawing::Size(75, 23);
			this->optionsButton->TabIndex = 50;
			this->optionsButton->Text = L"Options";
			this->optionsButton->UseVisualStyleBackColor = true;
			this->optionsButton->Click += gcnew System::EventHandler(this, &MyForm::optionsButton_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(387, 605);
			this->Controls->Add(this->optionsButton);
			this->Controls->Add(this->counterDropDown);
			this->Controls->Add(this->autoCounterInput);
			this->Controls->Add(this->imageLinkLabel);
			this->Controls->Add(this->resultOkButton);
			this->Controls->Add(this->pageOkButton);
			this->Controls->Add(this->resultNumberTextbox);
			this->Controls->Add(this->noSelectRadioButton);
			this->Controls->Add(this->pixelsLabel);
			this->Controls->Add(this->exactlyRadioButton);
			this->Controls->Add(this->atLeastRadioButton);
			this->Controls->Add(this->resolutionSeperatorLabel);
			this->Controls->Add(this->resolutionYInput);
			this->Controls->Add(this->resolutionXInput);
			this->Controls->Add(this->resultNumberLabel);
			this->Controls->Add(this->oearnLabel);
			this->Controls->Add(this->resultsLabel);
			this->Controls->Add(this->resultsListBox);
			this->Controls->Add(this->pageNumberTextBox);
			this->Controls->Add(this->pageNumberLabel);
			this->Controls->Add(this->oeapnLabel);
			this->Controls->Add(this->resultsBigLabel);
			this->Controls->Add(this->pagesLabel);
			this->Controls->Add(this->pagesListBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->statusStrip2);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->autoCheckBox);
			this->Controls->Add(this->pathLinkLabel);
			this->Controls->Add(this->searchButton);
			this->Controls->Add(this->orderDropdown);
			this->Controls->Add(this->sortingDropdown);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->categoriesLabel);
			this->Controls->Add(this->purityLabel);
			this->Controls->Add(this->sortingLabel);
			this->Controls->Add(this->miniSortingLabel);
			this->Controls->Add(this->miniOrderLabel);
			this->Controls->Add(this->peopleCheckBox);
			this->Controls->Add(this->animeCheckBox);
			this->Controls->Add(this->generalCheckBox);
			this->Controls->Add(this->sfwCheckBox);
			this->Controls->Add(this->sketchyCheckBox);
			this->Controls->Add(this->nsfwCheckBox);
			this->ForeColor = System::Drawing::SystemColors::ControlText;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"MyForm";
			this->Text = L"WHaven Wallpaper";
			this->Resize += gcnew System::EventHandler(this, &MyForm::MyForm_Resize);
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->statusStrip2->ResumeLayout(false);
			this->statusStrip2->PerformLayout();
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
		
		// Your api key
		private: String^ apikey;

		// Global repeat thread. Why Global? because I will stop this thread when automatically repeat checkbox unchecked or program stopped
		private: Thread^ counterThread;

		// Options class instance for showing options.
		Options::options^ opt = gcnew Options::options();


		// Global paths, shorturls, IDs, fileTypes, purity variables. I use these variables in many functions.
		private: array<String^>^ paths;
		private: array<String^>^ shortUrls;
		private: array<String^>^ IDs;
		private: array<String^>^ fileTypes;
		private: array<String^>^ purity;

		// Global repeat variable: I use this when automatically repeat checkbox unchecked or checked.
		private: static bool repeat = true;

		// Inits: I use these inits when a user sends a request.
		private: bool pageInit = false;
		private: bool resultInit = false;
		private: bool repeatInit = false;

		// Savesession: I use this variable when user wants to save when exit.
		private: bool savesession = true;

		// Selected Result Index: I use this variable for checking selected result.
		private: int result = 0;

		// Page: I use this variable for checkind selected page index.
		private: int page = 0;

		// Counter: I use this variable for checking counter in miliseconds.
		private: int counter;

		// Download: I use this variable for user wants to download or not. 
		private: bool download = true;

		// Page Number: I use this variable for checking page number in functions.
		private: int pageNumber;

		// Page Number: I use this variable for checking result number in functions.
		private: int resultNumber;

		// Page Number: I use this variable for checking user when changed app for saving state of app.
		private: bool changed;

		// appSessionInit: I check this variable for when app initialized(Look at constructor.)
		private: bool appSessionInit = false;
#pragma endregion

#pragma region Functions
		//////////////////////////////////////////////////////////////////////////////
		//																			//
		// FUNCTIONS																//
		//																			//
		//////////////////////////////////////////////////////////////////////////////

		//
		// Function: updateStrip(String^ toolStripText, int progress, int statusCode): Updates strip(statusbar) text and progressbar if status code is not 0, then progressbar will be red.(for errors)
		// 
		// String^ toolStripText: StatusBar text.
		// int progress: progressBar value.
		// int statusCode: send not 0 if there is a error.
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void _updateStrip(String^ toolStripText, int progress, int statusCode);
		private: System::Void updateStrip(String^ toolStripText, int progress, int statusCode)
		{
			if (statusCode != 0) {
				// If status code isn't zero, then change progressBar color to red.
				this->toolStripProgressBar1->BackColor = System::Drawing::Color::DarkRed;
			}

			// Change Striptext and Progressbar Value.
			this->toolStripStatusLabel1->Text = toolStripText;
			this->toolStripProgressBar1->Value = progress;
		}

		//
		// Function: updatePathLink(String^ pathLink): Simply updates blue image link 
		// 
		// String^ pathLink: String of the link. 
		//  
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void uPathLink(String^ pathLink);
		private: System::Void updatePathLink(String^ pathLink)
		{
			// Update path link text with pathLink variable.
			this->pathLinkLabel->Text = pathLink;
		}

		//
		// Function queryBuilder(int statusCode): Build an url for specified user selects. 
		// 
		// int statusCode: send not 0 if page changed.
		// 
		// returns Url String.(System::String)
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate String^ qBuilder(int statusCode);
		private: String^ queryBuilder(int statusCode) 
		{
			// If app not initialized and page changed
			if (statusCode != 0 && !appSessionInit) 
			{
				// if page changed then change page index.
				page = this->pagesListBox->SelectedIndex;
			}
			// If new query sended and app not initialized.
			else if(statusCode == 0 && !appSessionInit)
			{
				// If user sends to new query then select page with 1
				page = 0;
			}
			// Context for string operations.
			msclr::interop::marshal_context context;
			
			// Create New String
			std::string url = "https://wallhaven.cc/api/v1/search?";

			// If api key specified then add url to api key
			if (apikey != gcnew String("") && apikey!=nullptr)
				url += "apikey=" + context.marshal_as<std::string>(apikey);

			// Add search Query(q) to url.
			url += (std::string)"&q=" + context.marshal_as<std::string>(this->textBox1->Text);

			// If checked add 1 if not 0
			if (this->generalCheckBox->Checked)
				url += (std::string)"&categories=1";
			else
				url += (std::string)"&categories=0";
			if (this->animeCheckBox->Checked)
				url += (std::string)"1"; // .&categories=11
			else
				url += (std::string)"0";
			if (this->peopleCheckBox->Checked)
				url += (std::string)"1"; // .&categories=111
			else
				url += (std::string)"0";
			if (this->sfwCheckBox->Checked)
				url += (std::string)"&purity=1";
			else
				url += (std::string)"&purity=0";
			if (this->sketchyCheckBox->Checked)
				url += (std::string)"1";
			else
				url += (std::string)"0";
			if (this->nsfwCheckBox->Checked)
				url += (std::string)"1";
			else
				url += (std::string)"0";

			// Sorting.
			// Get shorting dropdown Index
			int sortItem = this->sortingDropdown->SelectedIndex;
			// I used switch case first. But program bugged. :/(It's my fault actually but I heard to if functions more faster than switch case.)
			// Loop for sorting item.
			if (sortItem == 0)
				url += (std::string)"&sorting=date_added";
			else if (sortItem == 1)
				url += (std::string)"&sorting=relevance";
			else if (sortItem == 2)
				url += (std::string)"&sorting=random";
			else if (sortItem == 3)
				url += (std::string)"&sorting=views";
			else if (sortItem == 4)
				url += (std::string)"&sorting=favorites";
			else if (sortItem == 5)
				url += (std::string)"&sorting=toplist";

			// Get orderDropdow index
			int orderItem = orderDropdown->SelectedIndex;
			// Loop for orderItem
			if (orderItem == 0)
				url += (std::string)"&order=desc";
			else if (orderItem == 1)
				url += (std::string)"&order=asc";

			// if pageIndex is 0 then page = 1 and if pageIndex is not 0 then page is pageIndex + 1;
			if(page == 0)
				url += (std::string)"&page=1";
			else
				url += ("&page=" + std::to_string((page +1)));

			// Atleast and Exactly resolution checker.
			if (this->atLeastRadioButton->Checked)
				url += "&atleast=" + context.marshal_as<std::string>(this->resolutionXInput->Text) + "x" + context.marshal_as<std::string>(this->resolutionYInput->Text);
			if (this->exactlyRadioButton->Checked)
				url += "&resolutions=" + context.marshal_as<std::string>(this->resolutionXInput->Text) + "x" + context.marshal_as<std::string>(this->resolutionYInput->Text);

			// Get url in System::String
			String^ _url = gcnew String(url.c_str());

			// Return _url
			return _url;
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
			getCName^ getName = gcnew getCName(this, &MyForm::getComponentName);
			dComponent^ disComponent = gcnew dComponent(this, &MyForm::disableComponent);

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
		private: System::Void enableButtons(Object^ exceptedComponentArray) {
			// This function exactly same with disableButtons. Its just enabling components instead of disabling, You can check disableButtons(look above) for this functions explainment I'll delete this duplicate later.
			array<Windows::Forms::Control^>^ eComponents = dynamic_cast<array< Windows::Forms::Control^>^>(exceptedComponentArray);
			array<Windows::Forms::Control^>^ allComponents = gcnew array<Windows::Forms::Control^>(this->Controls->Count);
			this->Controls->CopyTo(allComponents, 0);
			getCName^ getName = gcnew getCName(this, &MyForm::getComponentName);
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
					eComponent^ chComponent = gcnew eComponent(this, &MyForm::enableComponent);
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
		// Function: setPRange(array<Object^>^ pageArray): Clears and adds new items to pagesListBox.
		// 
		// array<Object^>^ pageArray: pages String array.
		// 
		// returns void.
		//
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void setPRange(array<Object^>^ pageArray);
		private: System::Void setPageRange(array<Object^>^ pageArray) {
			// Clear pagesListBox items.
			this->pagesListBox->Items->Clear();

			// Add new items.
			this->pagesListBox->Items->AddRange(pageArray);
		}

		//
		// Function: setRRange(array<Object^>^ resultsArray): Clears and adds new items to resultsListBox.
		// 
		// array<Object^>^ resultsArray: results String array.
		// 
		// returns void.
		//
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void setRRange(array<Object^>^ resultsArray);
		private: System::Void setResultsRange(array<Object^>^ resultsArray) {

			// Clear resultsListBox items.
			this->resultsListBox->Items->Clear();

			// Add new items.
			this->resultsListBox->Items->AddRange(resultsArray);
		}

		//
		// Function selectP(int selectedPage): Selects page used in autorepeat and when user clicks search (jsonProcessCallback).
		// 
		// int selectedPage: Selected Page Index.
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void selectP(int selectedPage);
		private: System::Void selectPage(int selectedPage) {
			// True for select specified item(don't ask me why. SetSelected written by Microsoft I just use it.).
			this->pagesListBox->SetSelected(selectedPage, true);
		}

		//
		// Function selectR(int selectedResult): Selects result used in autorepeat and when user clicks search (jsonProcessCallback).
		// 
		// int selectedResult: Selected Result Index.
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void selectR(int selectedResult);
		private: System::Void selectResult(int selectedResult) {
			// True for select specified item(don't ask me why. SetSelected written by Microsoft I just use it.).
			this->resultsListBox->SetSelected(selectedResult, true);
		}

		//
		// Function updatePageLabel(String^ pageLabelText): Updates pageNumberLabel used in when user clicks search.
		// 
		// String^ pageLabelText: String for pageNumberLevel's text
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void _updatePageLabel(String^ pageLabelText);
		private: System::Void updatePageLabel(String^ pageLabelText)
		{
			// Set page label text.
			this->pageNumberLabel->Text = pageLabelText;
		}

		//
		// Function updateResultsLabel(String^ resultLabelText): Updates resultNumberLabel used in when user clicks search.
		// 
		// String^ resultLabelText: String for resultNumberLabel's text
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void _updateResultsLabel(String^ resultLabelText);
		private: System::Void updateResultsLabel(String^ resultLabelText)
		{
			// Set result label text.
			this->resultNumberLabel->Text = resultLabelText;
		}

		//
		// Function disableResultComponents(): Disables result components used in when program starts.
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate System::Void cResultComponents();
		private: System::Void disableResultComponents() {
			// Create disable component delegate for disabling components.
			dComponent^ disComponent = gcnew dComponent(this, &MyForm::disableComponent);

			// Disable Results components.
			disComponent(this->resultsBigLabel);
			disComponent(this->pagesLabel);
			disComponent(this->resultsLabel);
			disComponent(this->pagesListBox);
			disComponent(this->resultsListBox);
			disComponent(this->oeapnLabel);
			disComponent(this->oearnLabel);
			disComponent(this->pageNumberLabel);
			disComponent(this->pageNumberTextBox);
			disComponent(this->pageOkButton);
			disComponent(this->resultNumberLabel);
			disComponent(this->resultNumberTextbox);
			disComponent(this->resultOkButton);
			disComponent(this->autoCheckBox);
			disComponent(this->autoCounterInput);
			disComponent(this->counterDropDown);
		}
		
		//
		// Function Repeat(): function for when auto counter enabled its select next result or page automatically used in auto select next feature.
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: System::Void MyForm::Repeat()
		{
			// delegates for get Counter Input and Get Counter Index i called in here because i dont want to create new instance in every while call
			_getCounterInput^ gCounterInput;
			_getCounterIndex^ gCounterIndex;

			// Sec, Min, Hr dropdown index.
			int selectedCounterIndex;

			while (repeat) {
				// Check if inputs or indexs changed.
				gCounterInput = gcnew _getCounterInput(this, &MyForm::getCounterInput);
				gCounterIndex = gcnew _getCounterIndex(this, &MyForm::getCounterIndex);
				selectedCounterIndex = static_cast<int>(Control::Invoke(gCounterIndex));

				// If user write 0 or Null in CounterInput then change Counter with 1
				try {
					counter = static_cast<int>(Control::Invoke(gCounterInput));
				}
				catch (Exception^) {
					counter = 1;
				}

				// If Selected counter is seconds:
				if (selectedCounterIndex == 0)
				{
					// if Counter is < than 30 seconds change counter with 30.
					if (counter <= 30) {
						counter = 30;
					}

					// Multiple with 1000 because I use miliseconds.
					counter *= 1000;
				}

				// If selected Counter is Minutes.
				else if (selectedCounterIndex == 1)
				{
					// if Counter is < than 1 mins change counter with 1.
					if (counter <= 1) {
						counter = 1;
					}

					// Multiple with 60000 because I use miliseconds and 1 Minutes = 60000 miliseconds.
					counter *= 60000;
				}

				// If selected Counter is Hours.
				else if (selectedCounterIndex == 2)
				{

					// if Counter is < than 1 hours change counter with 1.
					if (counter <= 1) {
						counter = 1;
					}

					// Multiple with 3600000 because I use miliseconds and 1 hours = 3600000 miliseconds.
					counter *= 3600000;
				}

				// If repeat is called first time, then wait first and change then.
				if (repeatInit) {
					repeatInit = false;
					// Sleep for counter(sec/min/hr)
					Thread::Sleep(counter);
				}

				// Increase Result by 1.
				result += 1;

				// If Result > than resultNumber, than select next page.
				if (result >= paths->Length) {
					// Increase page number.
					page += 1;

					// Create select page delegate.
					selectP^ setPg = gcnew selectP(this, &MyForm::selectPage);

					// Select next page.
					Control::Invoke(setPg, page);

					// Sleep for counter(sec/min/hr).
					Thread::Sleep(counter);
				}

				// Else Select next Result.
				else {

					// Create Select result delegate.
					selectR^ setRs = gcnew selectR(this, &MyForm::selectResult);

					// Select next result.
					Control::Invoke(setRs, result);

					// Sleep for counter(sec/min/hr).
					Thread::Sleep(counter);
				}
			}
		}

		//
		// Function getCounterInput(): gets Counter Inputs.
		// 
		// returns Counter integer.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate int _getCounterInput();
		private: int getCounterInput() {
			// Get counter input.
			return int::Parse(this->autoCounterInput->Text);
		}

		//
		// Function getCounterIndex(): gets Counter Index.
		// 
		// returns Counter index integer.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: delegate int _getCounterIndex();
		private: int getCounterIndex() {
			// Get counter dropdown index.
			return this->counterDropDown->SelectedIndex;
		}
#pragma endregion

#pragma region CallBacks
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
			// Create updateStrip function delegate. You can check this function in functions section.
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);

			// If httpStatusCode is ok.
			if (httpStatusCode == 200)
			{
				// Update strip(StatusBar).
				Control::Invoke(us, gcnew String("Json Retrieved and Processing..."), 25, 0);

				// Create new instance of JsonUtils(I send jsonProcessCallBack in exactly here.).
				JsonUtils^ prJson = gcnew JsonUtils(notParsedJson, 1, gcnew JsonUtils::jsProcessCallback(this, &MyForm::jsonProcessCallBack));

				// Create new Thread of processJson.
				Thread^ backgroundThread = gcnew Thread(gcnew ThreadStart(prJson, &JsonUtils::processJson));

				// Start Thread.
				backgroundThread->Start();
			}

			// If httpStatusCode is not ok.
			else 
			{
				// Update strip(StatusBar) and do nothing(means dont continue).
				Control::Invoke(us, gcnew String("Error While Getting Json Http code:" + httpStatusCode), 25, httpStatusCode);
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
			// Cast Tuple like I said in callback explanation.
			Tuple<int, int, array<String^>^, array<String^>^, array<String^>^, array<String^>^, array<String^>^>^ args = dynamic_cast<Tuple<int, int, array<String^>^, array<String^>^, array<String^>^, array<String^>^, array<String^>^>^>(tupleContent);

			// Create updateStrip function delegate. You can check this function in functions section.
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);

			// If no error occured
			if (args->Item1 != 27272727) {

				// Get variables frop tuple.
				pageNumber = args->Item1;
				resultNumber = args->Item2;
				paths = args->Item3;
				shortUrls = args->Item4;
				IDs = args->Item5;
				fileTypes = args->Item6;
				purity = args->Item7;



				// Update strip(StatusBar).
				Control::Invoke(us, gcnew String("Json process Completed Downloading jpeg..."), 50, 0);

				// Create jpegDownloadCallback instance.
				jpegDownloadCallback^ jCB = gcnew jpegDownloadCallback(this, &MyForm::jpegDownloadCback);

				// Create JpegUtils instance and send callback with it.
				JpegUtils^ jUtils = gcnew JpegUtils(paths[0], IDs[0], fileTypes[0], purity[0], download, jCB);

				// Create Thread for created JpegUtils instance.
				Thread^ downloadJpeg = gcnew Thread(gcnew ThreadStart(jUtils, &JpegUtils::download_jpeg));

				// Start thread.
				downloadJpeg->Start();

				// Create page Numbers Object for change page list.
				array<System::Object^>^ pageItemObject = gcnew array<System::Object^>(pageNumber);

				// Add page strings to Object.
				for (int i = 0; i < pageNumber; i += 1) {
					pageItemObject[i] = String::Concat("Page ", (i + 1).ToString());
				}

				// Create setPageRange delegate.
				setPRange^ setPgRange = gcnew setPRange(this, &MyForm::setPageRange);

				// Send page strings to setPageRange.
				Control::Invoke(setPgRange, gcnew array<Object^>{pageItemObject});


				// Create results Numbers Object for change page list.
				array<System::Object^>^ resultsItemObject = gcnew array<System::Object^>(resultNumber);

				// Add page strings to Object.
				for (int i = 0; i < resultNumber; i += 1) {
					resultsItemObject[i] = String::Concat("Result ", (i + 1).ToString());
				}

				// Create setResultsRange delegate.
				setRRange^ setRsRange = gcnew setRRange(this, &MyForm::setResultsRange);

				// Send result strings to setResultsRange
				Control::Invoke(setRsRange, gcnew array<Object^>{resultsItemObject});

				if (pageNumber >= 1) {
					// Set pageInit true for dont trigger selectedPageChanged event.
					pageInit = true;

					// Create setPage delegate.
					selectP^ setPg = gcnew selectP(this, &MyForm::selectPage);

					// Set page index in selected page index.
					Control::Invoke(setPg, page);

					// Create selectResult Instance.
					selectR^ setRs = gcnew selectR(this, &MyForm::selectResult);

					// If app not inititialized reset result.
					if (!appSessionInit)
					{
						// Set resultInit true for dont trigger selectedResultChanged event.
						resultInit = true;

						// Reset Result Index
						result = 0;

						// Set result index in 0 because results reloaded.
						Control::Invoke(setRs, result);
					}
					// If app inititialized select result
					else
					{
						// If user change result number from json control that
						if (result > IDs->Length)
							result = IDs->Length - 1;
						// Set result index in result because app initialized.
						Control::Invoke(setRs, result);

						// Set appSessionInit false because we are finished initializing.
						appSessionInit = false;
					}

					// Create updatePageLabel delegate.
					_updatePageLabel^ uPageLabel = gcnew _updatePageLabel(this, &MyForm::updatePageLabel);

					// Update pageLabel with pageNumber.
					Control::Invoke(uPageLabel, (gcnew String("(") + pageNumber + gcnew String(" page)")));

					// Create updateResultsLabel delegate.
					_updateResultsLabel^ uResultsLabel = gcnew _updateResultsLabel(this, &MyForm::updateResultsLabel);
					
					// Update resultLabel with resultNumber.
					Control::Invoke(uResultsLabel, (gcnew String("(") + resultNumber + gcnew String(" result)")));

					// Create updatePathLink delegate.
					uPathLink^ uPath = gcnew uPathLink(this, &MyForm::updatePathLink);

					// Update Link Label with url.
					Control::Invoke(uPath, shortUrls[0]);
				}

				// Delete Created Items.
				delete setPgRange;
				delete pageItemObject;
				delete setRsRange;
				delete resultsItemObject;
			}

			// If error occured.
			else
			{
				// Update strip(StatusBar) and do nothing(means dont continue).
				Control::Invoke(us, gcnew String("Error while processing json"), 50, 272727);
			}
		}

		//
		// Callback: jpegDownloadCback(int statusCode): running when jpeg downloaded.
		// 
		// int statusCode: Status code for if error occured.
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: System::Void jpegDownloadCback(int statusCode) 
		{
			// Create updateStrip(StatusBar Update) delegate
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);

			// If statuscode is ok.
			if (statusCode == 0) 
			{
				// Update Status Bar
				Control::Invoke(us, gcnew String("Jpeg Download Completed. Setting Jpeg As Wallpaper..."), 85, statusCode);

				// Create Set Wallpaper Callback for sending JpegUtils.
				setWallpaperCallback^ _setWallpaperCallback = gcnew setWallpaperCallback(this, &MyForm::sWallpaperCallback);

				// Create JpegUtils instance for set Wallpaper.
				JpegUtils^ jUtil = gcnew JpegUtils(IDs[result], fileTypes[result], purity[result], download, _setWallpaperCallback);

				// Create Thread for created JpegUtils instance.
				Thread^ setWallpaper = gcnew Thread(gcnew ThreadStart(jUtil, &JpegUtils::setWallpaper));

				// Start Thread.
				setWallpaper->Start();
			}
			else
			{
				// Error occured dont continue.
				Control::Invoke(us, gcnew String("Jpeg Download Failed"), 85, statusCode);
			}

		}

		//
		// Callback: sWallpaperCallback(int statusCode): running when setting wallpaper and process completed.
		// 
		// int statusCode: Status code for if error occured.
		// 
		// returns void.
		// 
		// Warning: You will call with delegate if you want to use this function in events or threads.
		//
		private: System::Void sWallpaperCallback(int statusCode) {

			// Create updateStrip(StatusBar Update) delegate
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);

			// Create index integer for not enabled components.
			int disabledComponentIndex = 0;

			// If No Select Resolution button checked than not enable that components.
			if (this->noSelectRadioButton->Checked)
				disabledComponentIndex += 4;
			array<Windows::Forms::Control^>^ disabledComponents = gcnew array<Windows::Forms::Control^>(disabledComponentIndex);
			if (disabledComponentIndex > 0) {
				disabledComponents[0] = this->resolutionXInput;
				disabledComponents[1] = this->resolutionYInput;
				disabledComponents[2] = this->resolutionSeperatorLabel;
				disabledComponents[3] = this->pixelsLabel;
			}

			// Create enableButtons Thread which enables all components.
			Thread^ enableComponents = gcnew Thread(gcnew ParameterizedThreadStart(this, &MyForm::enableButtons));

			// Start thread and dont enable Disabled Components.
			enableComponents->Start(disabledComponents);

			// If Wallpaper setting is completed.
			if (statusCode == 0)
			{
				// Update Status Bar and finished all process.
				Control::Invoke(us, gcnew String("Jpeg setted as wallpaper"), 100, statusCode);
				Control::Invoke(us, gcnew String("Application Ready"), 0, statusCode);
			}
			else
			{
				// Error setting wallpaper.
				Control::Invoke(us, gcnew String("Setting Jpeg as wallpaper failed"), 100, statusCode);
			}

		}
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
		// Event: searchButton_Click: Event for when search button clicked.
		//
		private: System::Void searchButton_Click(System::Object^ sender, System::EventArgs^ e)
		{
			// App changed so I can save. I check this variable in destructor
			if (!changed)
				changed = true;
			// Updating strip(Statusbar).
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);
			Control::Invoke(us, gcnew String("Building url..."), 5, 0);

			// Delegating function for queryBuilder function.
			qBuilder^ qBuild = gcnew qBuilder(this, &MyForm::queryBuilder);

			// Building and Retrieving Url.
			String^ _url = qBuild(0);

			// Disabling Buttons.
			array<Windows::Forms::Control^>^ exceptedComponents = gcnew array<Windows::Forms::Control^>(4);
			exceptedComponents[0] = this->imageLinkLabel;
			exceptedComponents[1] = this->pathLinkLabel;
			exceptedComponents[2] = this->statusStrip1;
			exceptedComponents[3] = this->statusStrip2;

			// Create Thread for disable all components.
			Thread^ disableComponents = gcnew Thread(gcnew ParameterizedThreadStart(this, &MyForm::disableButtons));

			// Start thread and send excepted components.
			disableComponents->Start(exceptedComponents);

			// Update strip(Statusbar).
			Control::Invoke(us, gcnew String("Url build completed, sending json request..."), 12, 0);

			// Create Callback for json Request.
			JsonUtils::requestCallback^ jRequest = gcnew JsonUtils::requestCallback(this, &MyForm::jsonRequestCallbackHandler);

			// Create JsonUtils instance, send builded url and callback.
			JsonUtils^ jd = gcnew JsonUtils(_url, jRequest);

			// Create jsonRequest Thread for request.
			Thread^ backgroundThread = gcnew Thread(gcnew ThreadStart(jd, &JsonUtils::jsonRequest));

			// Start Thread.
			backgroundThread->Start();

			// Delete created items.
			delete us;
		}

		//
		// Event: pathLinkLabelClicked: Event for when shorted link(known as pathLinkLabel) clicked.
		//
		private: System::Void pathLinkLabelClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e) {
			// Open url when shorted link clicked
			System::Diagnostics::Process::Start(this->pathLinkLabel->Text);
		}

		//
		// Event: repeatCheckBox_CheckedChanged: Event for when automate function(known as repeatCheckBox) checked.
		//
		private: System::Void repeatCheckBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
			// If there is a counter counterThread then abort that thread(counterThread is globalVariable)
			if (counterThread != nullptr) {
				counterThread->Abort();
				counterThread = nullptr;
			}
			// Initialize repeat
			repeat = this->autoCheckBox->Checked;

			// RepeatInit true so you will wait for first call
			repeatInit = true;

			// Create Thread for repeat function(You can check this function in functions section).
			counterThread = gcnew Thread(gcnew ThreadStart(this, &MyForm::Repeat));

			// Start thread.
			counterThread->Start();
		}

		//
		// Event: resultsListBox_SelectedIndexChanged: Event for when result list(known as resultsListBox) index changed.
		//
		private: System::Void resultsListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
			// App changed so I can save. I check this variable in destructor
			if (!changed)
				changed = true;

			// If result don't changed by user, then quit the function because I actually handle that in other functions.
			if (resultInit)
			{
				resultInit = false;
				return;
			}

			// Define excepted Component array.
			array<Windows::Forms::Control^>^ exceptedComponents = gcnew array<Windows::Forms::Control^>(4);
			exceptedComponents[0] = this->imageLinkLabel;
			exceptedComponents[1] = this->pathLinkLabel;
			exceptedComponents[2] = this->statusStrip1;
			exceptedComponents[3] = this->statusStrip2;

			// Create thread for disable all components.
			Thread^ disableComponents = gcnew Thread(gcnew ParameterizedThreadStart(this, &MyForm::disableButtons));

			// disable all components except exceptedComponents array.
			disableComponents->Start(exceptedComponents);

			// Change Selected Result Index.
			result = this->resultsListBox->SelectedIndex;

			// Create Delegate for update shortlink.
			uPathLink^ uPath = gcnew uPathLink(this, &MyForm::updatePathLink);

			// Update shortlink with shorturl.
			Control::Invoke(uPath, shortUrls[result]);

			// Create callback for downloading jpeg.
			jpegDownloadCallback^ jCB = gcnew jpegDownloadCallback(this, &MyForm::jpegDownloadCback);

			// Create JpegUtils instance with jpegDownloadCallback.
			JpegUtils^ jUtils = gcnew JpegUtils(paths[result], IDs[result], fileTypes[result], purity[result], download, jCB);

			// Create Thread for created JpegUtils instance.
			Thread^ downloadJpeg = gcnew Thread(gcnew ThreadStart(jUtils, &JpegUtils::download_jpeg));

			// Update status bar.
			downloadJpeg->Start();


			// Create delegate for updateStrip(StatusBar) function.
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);

			// Update Status bar when downloading jpeg
			Control::Invoke(us, gcnew String("Downloading jpeg..."), 50, 0);

			// Delete created items.
			delete us;
		}

		//
		// Event: resultsListBox_SelectedIndexChanged: Event for when result list(known as resultsListBox) index changed.
		//
		private: System::Void pagesListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
			// App changed so I can save. I check this variable in destructor
			if (!changed)
				changed = true;

			// If page don't changed by user, then quit the function because I actually handle that in other functions.
			if (pageInit)
			{
				pageInit = false;
				return;
			}

			// Create delegate for updateStrip(StatusBar) function.
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);

			// Update Status bar when building url.
			Control::Invoke(us, gcnew String("Building url..."), 5, 0);

			// Create queryBuilder delegate
			qBuilder^ qBuild = gcnew qBuilder(this, &MyForm::queryBuilder);

			// Call queryBuilder delegate with 1 status code which change page query.
			String^ _url = qBuild(1);

			// Creating array for not disabling components before call disableButtons which disables all components.
			array<Windows::Forms::Control^>^ exceptedComponents = gcnew array<Windows::Forms::Control^>(4);
			exceptedComponents[0] = this->imageLinkLabel;
			exceptedComponents[1] = this->pathLinkLabel;
			exceptedComponents[2] = this->statusStrip1;
			exceptedComponents[3] = this->statusStrip2;

			// Create Thread for disabling all components
			Thread^ disableComponents = gcnew Thread(gcnew ParameterizedThreadStart(this, &MyForm::disableButtons));

			// Start thread and send exceptedComponents with that.
			disableComponents->Start(exceptedComponents);

			// Update Status Bar when url building finished and sending json requests.
			Control::Invoke(us, gcnew String("Url build completed, sending json request..."), 12, 0);

			// Create callback delegate for getting Jsonrequest result
			JsonUtils::requestCallback^ _requestCallback = gcnew JsonUtils::requestCallback(this, &MyForm::jsonRequestCallbackHandler);

			// Create JsonUtils instance for getting json request
			JsonUtils^ jd = gcnew JsonUtils(_url, _requestCallback);

			// Create Thread for JsonUtils instance
			Thread^ backgroundThread = gcnew Thread(gcnew ThreadStart(jd, &JsonUtils::jsonRequest));

			// Start Thread
			backgroundThread->Start();

			// Delete created items
			delete us;
		}

		//
		// Event: textBox1_KeyDown: Event for when user keydown enter when searching.
		//
		private: System::Void textBox1_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			// If user clicks enter key
			if (e->KeyData == System::Windows::Forms::Keys::Enter)
				// Then run searchButton_Click event.
				this->searchButton_Click("", nullptr);
		}

		//
		// Event: autoCounterInput_KeyPress: Event for disables non-digit characters for autoCounterInput.
		//
		private: System::Void autoCounterInput_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
			// Accept only digits ".", "-" and the Backspace(0x08) character
			if (!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
				e->Handled = true;
			}	
		}

		//
		// Event: resolutionXInput_KeyPress: Event for disables non-digit characters for resolutionXInput.
		//
		private: System::Void resolutionXInput_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
			// Accept only digits ".", "-" and the Backspace(0x08) character
			if (!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
				e->Handled = true;
			}
		}

		//
		// Event: resolutionYInput_KeyPress: Event for disables non-digit characters for resolutionYInput.
		//
		private: System::Void resolutionYInput_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
			// Accept only digits ".", "-" and the Backspace(0x08) character
			if (!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
				e->Handled = true;
			}
		}

		//
		// Event: pageNumberTextBox_KeyPress: Event for disables non-digit characters for pageNumberTextBox.
		//
		private: System::Void pageNumberTextBox_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
			// Accept only digits ".", "-" and the Backspace(0x08) character
			if (!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
				e->Handled = true;
			}
		}

		//
		// Event: resultNumberTextbox_KeyPress: Event for disables non-digit characters for resultNumberTextbox.
		//
		private: System::Void resultNumberTextbox_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
			// Accept only digits ".", "-" and the Backspace(0x08) character
			if (!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
				e->Handled = true;
			}
		}

		//
		// Event: autoCounterInput_KeyDown: This event is supposed to change when a value less than 10 is entered, but it doesn't work as I want.(Comment from future: It works now I fixed)
		// And changes counter every input changed.
		//
		private: System::Void autoCounterInput_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			// If seconds selected and string is emtpy or < than 10 then change input with 10
			if ((this->autoCounterInput->Text == gcnew String("") || int::Parse(this->autoCounterInput->Text) < 10) && this->counterDropDown->SelectedIndex == 0)
				this->autoCounterInput->Text = "10";
			// If minutes or houts selected and string is emtpy or < than 1 then change input with 1
			else if (((this->autoCounterInput->Text == gcnew String("") || int::Parse(this->autoCounterInput->Text) < 10) && this->counterDropDown->SelectedIndex == 1) && (this->autoCounterInput->Text == gcnew String("") && this->counterDropDown->SelectedIndex == 2))
				this->autoCounterInput->Text = "1";

			// Change counter every input changed
			counter = int::Parse(this->autoCounterInput->Text);
		}
		
		//
		// Event: noSelectRadioButton_CheckedChanged: disables resolution components when noResulutionSelectSorting checked.
		//
		private: System::Void noSelectRadioButton_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
			this->resolutionXInput->Enabled = false;
			this->resolutionYInput->Enabled = false;
			this->resolutionSeperatorLabel->Enabled = false;
			this->pixelsLabel->Enabled = false;
		}

		//
		// Event: exactlyRadioButton_CheckedChanged: enables resolution components when exactlyResolutionSorting checked.
		//
		private: System::Void exactlyRadioButton_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
			this->resolutionXInput->Enabled = true;
			this->resolutionYInput->Enabled = true;
			this->resolutionSeperatorLabel->Enabled = true;
			this->pixelsLabel->Enabled = true;
		}

		//
		// Event: exactlyRadioButton_CheckedChanged: enables resolution components when atLeastResolutionSorting checked.
		//
		private: System::Void atLeastRadioButton_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
			this->resolutionXInput->Enabled = true;
			this->resolutionYInput->Enabled = true;
			this->resolutionSeperatorLabel->Enabled = true;
			this->pixelsLabel->Enabled = true;
		}

		//
		// Event: pageOkButton_Click: Handles when user clicks Ok button in pages.
		//
		private: System::Void pageOkButton_Click(System::Object^ sender, System::EventArgs^ e) {

			// Create virtual instance for trying parse. If can't parse then our page index not changed.
			int _page;
			// Try to parse int, if input text is not integer then do nothing.
			try 
			{
				// Parsing int and indexing(-1)
				_page = int::Parse(this->pageNumberTextBox->Text) - 1;
			}
			catch (Exception^) 
			{
				// If can't parse, then do nothing.
				return;
			}
			// If not error occured then change page index.
			page = _page;

			// If user somehow typed page number to smaller than 0, then equal that to 0.
			if (page < 0)
				page = 0;

			// If user typed page to bigger than last page number, then select last page.
			if (page > pageNumber)
				page = pageNumber - 1;


			// Create delegate instance for selectPage and when selected page changed it triggers pagesListBox_SelectedIndexChanged event
			selectP^ setPg = gcnew selectP(this, &MyForm::selectPage);

			// Run created delegate. Why invoke? Because Invoke safe with when thread is running
			Control::Invoke(setPg, page);
		}

		//
		// Event: resultOkButton_Click: Handles when user clicks Ok button in results.
		//
		private: System::Void resultOkButton_Click(System::Object^ sender, System::EventArgs^ e) {

			// Create virtual instance for trying parse. If can't parse then our page index not changed.
			int _result;
			// Try to parse int, if input text is not integer then do nothing.
			try
			{
				_result = int::Parse(this->resultNumberTextbox->Text) - 1;

			}
			catch (Exception^)
			{
				return;
			}
			// If not error occured then change result index.
			result = _result;

			// If user somehow typed result number to smaller than 0, then equal that to 0.
			if (result < 0)
				result = 0;

			// If user typed result to bigger than last result number, then select last result.
			if (result > resultNumber)
				result = resultNumber - 1;

			// Create delegate instance for selectResult and when selected result changed it triggers resultsListBox_SelectedIndexChanged event
			selectR^ setRs = gcnew selectR(this, &MyForm::selectResult);

			// Run created delegate. Why invoke? Because Invoke safe with when thread is running
			Control::Invoke(setRs, result);
		}

		//
		// Event: pageNumberTextBox_KeyDown: Event for when user keydown enter changing page.
		//
		private: System::Void pageNumberTextBox_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			// If user clicks enter key
			if (e->KeyData == System::Windows::Forms::Keys::Enter)
				// Then run pageOkButton_Click event.
				this->pageOkButton_Click("", nullptr);
		}

		//
		// Event: resultNumberTextbox_KeyDown: Event for when user keydown enter changing result.
		//
		private: System::Void resultNumberTextbox_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			// If user clicks enter key
			if (e->KeyData == System::Windows::Forms::Keys::Enter)
				// Then run resultOkButton_Click event.
				this->resultOkButton_Click("", nullptr);
		}

		//
		// Event: notifyIcon1_Click: Event for when user clicks taskbar icon.
		//
		private: System::Void notifyIcon1_Click(System::Object^ sender, System::EventArgs^ e) {
			// Change form state with visible
			this->Visible = true;

			// Disable notifyIcon
			this->notifyIcon1->Visible = false;

			// Load window
			this->WindowState = System::Windows::Forms::FormWindowState::Normal;
		}

		//
		// Event: MyForm_Resize: Event for when user minimize the program.
		//
		private: System::Void MyForm_Resize(System::Object^ sender, System::EventArgs^ e) {
			// If window state minimized
			if (this->WindowState == System::Windows::Forms::FormWindowState::Minimized) {
				// Show notify icon
				this->notifyIcon1->Visible = true;

				// Hide program
				this->Visible = false;
			}
		}
		//
		// Event: MyForm_Resize: Event for when options button clicked.
		//
		private: System::Void optionsButton_Click(System::Object^ sender, System::EventArgs^ e) {
			if (opt->IsDisposed)
				opt = gcnew Options::options();
			// Change options visible state to true.
			
			opt->Visible = !(opt->Visible);

			// Change windows state for focusing options.
			opt->WindowState = System::Windows::Forms::FormWindowState::Normal;
		}
	};
}