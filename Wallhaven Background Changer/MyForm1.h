#pragma once

// TODO random seeding maybe?

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
			this->Icon = gcnew System::Drawing::Icon(resourceAssembly->GetManifestResourceStream("favicon.ico"));
			InitializeComponent();
			this->notifyIcon1->Icon = gcnew System::Drawing::Icon(resourceAssembly->GetManifestResourceStream("favicon.ico"));
			this->notifyIcon1->Visible = false;
			
			//
			//TODO: Add the constructor code here
			//
			this->orderDropdown->SelectedIndex = 0;
			this->sortingDropdown->SelectedIndex = 0;
			this->counterDropDown->SelectedIndex = 1;
			disableResultComponents();
			//This code disables maximize button.
			SetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWL_STYLE,
				GetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWL_STYLE) & ~WS_MAXIMIZEBOX);

			INIReader reader("./whaven-wallpaper-config.ini");
			if (reader.ParseError() < 0) {
				String^ fileName = "whaven-wallpaper-config.ini";

				System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(fileName);
				sw->WriteLine("[options]");
				sw->WriteLine("apikey=");
				sw->WriteLine("download=true");
				sw->Close();
			}
			else {
				apikey = gcnew String(reader.Get("options", "apikey", "").c_str());
				download = reader.GetBoolean("options", "download", true);
			}
			if (apikey != gcnew String("") && apikey != nullptr) {
				this->nsfwCheckBox->Visible = true;
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			repeat = false;
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
			this->statusStrip1->SuspendLayout();
			this->statusStrip2->SuspendLayout();
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(25, 44);
			this->textBox1->Margin = System::Windows::Forms::Padding(4);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(471, 22);
			this->textBox1->TabIndex = 3;
			this->textBox1->Tag = L"Data";
			this->textBox1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::textBox1_KeyDown);
			// 
			// categoriesLabel
			// 
			this->categoriesLabel->AutoSize = true;
			this->categoriesLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold));
			this->categoriesLabel->Location = System::Drawing::Point(16, 108);
			this->categoriesLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->categoriesLabel->Name = L"categoriesLabel";
			this->categoriesLabel->Size = System::Drawing::Size(395, 29);
			this->categoriesLabel->TabIndex = 4;
			this->categoriesLabel->Text = L"Categories  ————————————————";
			// 
			// purityLabel
			// 
			this->purityLabel->AutoSize = true;
			this->purityLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold));
			this->purityLabel->Location = System::Drawing::Point(20, 167);
			this->purityLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->purityLabel->Name = L"purityLabel";
			this->purityLabel->Size = System::Drawing::Size(378, 29);
			this->purityLabel->TabIndex = 5;
			this->purityLabel->Text = L"Purity  ———————————————————";
			// 
			// sortingLabel
			// 
			this->sortingLabel->AutoSize = true;
			this->sortingLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold));
			this->sortingLabel->Location = System::Drawing::Point(21, 231);
			this->sortingLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->sortingLabel->Name = L"sortingLabel";
			this->sortingLabel->Size = System::Drawing::Size(381, 29);
			this->sortingLabel->TabIndex = 6;
			this->sortingLabel->Text = L"Sorting  ——————————————————";
			// 
			// miniSortingLabel
			// 
			this->miniSortingLabel->AutoSize = true;
			this->miniSortingLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->miniSortingLabel->Location = System::Drawing::Point(17, 266);
			this->miniSortingLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->miniSortingLabel->Name = L"miniSortingLabel";
			this->miniSortingLabel->Size = System::Drawing::Size(67, 20);
			this->miniSortingLabel->TabIndex = 7;
			this->miniSortingLabel->Text = L"Sorting:";
			// 
			// miniOrderLabel
			// 
			this->miniOrderLabel->AutoSize = true;
			this->miniOrderLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->miniOrderLabel->Location = System::Drawing::Point(284, 266);
			this->miniOrderLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->miniOrderLabel->Name = L"miniOrderLabel";
			this->miniOrderLabel->Size = System::Drawing::Size(57, 20);
			this->miniOrderLabel->TabIndex = 8;
			this->miniOrderLabel->Text = L"Order:";
			// 
			// generalCheckBox
			// 
			this->generalCheckBox->AutoSize = true;
			this->generalCheckBox->Checked = true;
			this->generalCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->generalCheckBox->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->generalCheckBox->Location = System::Drawing::Point(21, 143);
			this->generalCheckBox->Margin = System::Windows::Forms::Padding(4);
			this->generalCheckBox->Name = L"generalCheckBox";
			this->generalCheckBox->Size = System::Drawing::Size(86, 25);
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
			this->animeCheckBox->Location = System::Drawing::Point(216, 143);
			this->animeCheckBox->Margin = System::Windows::Forms::Padding(4);
			this->animeCheckBox->Name = L"animeCheckBox";
			this->animeCheckBox->Size = System::Drawing::Size(78, 25);
			this->animeCheckBox->TabIndex = 11;
			this->animeCheckBox->Text = L"Anime";
			this->animeCheckBox->UseVisualStyleBackColor = true;
			// 
			// peopleCheckBox
			// 
			this->peopleCheckBox->AutoSize = true;
			this->peopleCheckBox->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->peopleCheckBox->Location = System::Drawing::Point(411, 143);
			this->peopleCheckBox->Margin = System::Windows::Forms::Padding(4);
			this->peopleCheckBox->Name = L"peopleCheckBox";
			this->peopleCheckBox->Size = System::Drawing::Size(79, 25);
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
			this->sfwCheckBox->Location = System::Drawing::Point(21, 207);
			this->sfwCheckBox->Margin = System::Windows::Forms::Padding(4);
			this->sfwCheckBox->Name = L"sfwCheckBox";
			this->sfwCheckBox->Size = System::Drawing::Size(56, 25);
			this->sfwCheckBox->TabIndex = 13;
			this->sfwCheckBox->Text = L"sfw";
			this->sfwCheckBox->UseVisualStyleBackColor = true;
			// 
			// sketchyCheckBox
			// 
			this->sketchyCheckBox->AutoSize = true;
			this->sketchyCheckBox->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->sketchyCheckBox->Location = System::Drawing::Point(216, 207);
			this->sketchyCheckBox->Margin = System::Windows::Forms::Padding(4);
			this->sketchyCheckBox->Name = L"sketchyCheckBox";
			this->sketchyCheckBox->Size = System::Drawing::Size(85, 25);
			this->sketchyCheckBox->TabIndex = 14;
			this->sketchyCheckBox->Text = L"sketchy";
			this->sketchyCheckBox->UseVisualStyleBackColor = true;
			// 
			// nsfwCheckBox
			// 
			this->nsfwCheckBox->AutoSize = true;
			this->nsfwCheckBox->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->nsfwCheckBox->Location = System::Drawing::Point(411, 207);
			this->nsfwCheckBox->Margin = System::Windows::Forms::Padding(4);
			this->nsfwCheckBox->Name = L"nsfwCheckBox";
			this->nsfwCheckBox->Size = System::Drawing::Size(65, 25);
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
			this->sortingDropdown->Location = System::Drawing::Point(21, 290);
			this->sortingDropdown->Margin = System::Windows::Forms::Padding(4);
			this->sortingDropdown->Name = L"sortingDropdown";
			this->sortingDropdown->Size = System::Drawing::Size(207, 24);
			this->sortingDropdown->TabIndex = 16;
			// 
			// orderDropdown
			// 
			this->orderDropdown->AccessibleRole = System::Windows::Forms::AccessibleRole::ScrollBar;
			this->orderDropdown->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->orderDropdown->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Descending", L"Ascending" });
			this->orderDropdown->Location = System::Drawing::Point(288, 290);
			this->orderDropdown->Margin = System::Windows::Forms::Padding(4);
			this->orderDropdown->Name = L"orderDropdown";
			this->orderDropdown->Size = System::Drawing::Size(193, 24);
			this->orderDropdown->TabIndex = 17;
			// 
			// searchButton
			// 
			this->searchButton->Location = System::Drawing::Point(25, 76);
			this->searchButton->Margin = System::Windows::Forms::Padding(4);
			this->searchButton->Name = L"searchButton";
			this->searchButton->Size = System::Drawing::Size(472, 28);
			this->searchButton->TabIndex = 20;
			this->searchButton->Text = L"Search";
			this->searchButton->UseVisualStyleBackColor = true;
			this->searchButton->Click += gcnew System::EventHandler(this, &MyForm::searchButton_Click);
			// 
			// pathLinkLabel
			// 
			this->pathLinkLabel->AutoSize = true;
			this->pathLinkLabel->Location = System::Drawing::Point(112, 688);
			this->pathLinkLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->pathLinkLabel->Name = L"pathLinkLabel";
			this->pathLinkLabel->Size = System::Drawing::Size(135, 17);
			this->pathLinkLabel->TabIndex = 21;
			this->pathLinkLabel->TabStop = true;
			this->pathLinkLabel->Text = L"https:://wallhaven.cc";
			this->pathLinkLabel->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &MyForm::pathLinkLabelClicked);
			// 
			// autoCheckBox
			// 
			this->autoCheckBox->AutoSize = true;
			this->autoCheckBox->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->autoCheckBox->Location = System::Drawing::Point(21, 646);
			this->autoCheckBox->Margin = System::Windows::Forms::Padding(4);
			this->autoCheckBox->Name = L"autoCheckBox";
			this->autoCheckBox->Size = System::Drawing::Size(277, 25);
			this->autoCheckBox->TabIndex = 22;
			this->autoCheckBox->Text = L"Select Next Image Automatically in:";
			this->autoCheckBox->UseVisualStyleBackColor = true;
			this->autoCheckBox->CheckedChanged += gcnew System::EventHandler(this, &MyForm::repeatCheckBox_CheckedChanged);
			// 
			// statusStrip1
			// 
			this->statusStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripStatusLabel1 });
			this->statusStrip1->Location = System::Drawing::Point(0, 720);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Padding = System::Windows::Forms::Padding(1, 0, 19, 0);
			this->statusStrip1->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->statusStrip1->Size = System::Drawing::Size(516, 25);
			this->statusStrip1->TabIndex = 23;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8));
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(121, 19);
			this->toolStripStatusLabel1->Text = L"Application Ready.";
			// 
			// statusStrip2
			// 
			this->statusStrip2->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->statusStrip2->AutoSize = false;
			this->statusStrip2->Dock = System::Windows::Forms::DockStyle::None;
			this->statusStrip2->ImageScalingSize = System::Drawing::Size(20, 20);
			this->statusStrip2->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripProgressBar1 });
			this->statusStrip2->Location = System::Drawing::Point(356, 718);
			this->statusStrip2->Name = L"statusStrip2";
			this->statusStrip2->Padding = System::Windows::Forms::Padding(1, 0, 19, 0);
			this->statusStrip2->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->statusStrip2->Size = System::Drawing::Size(157, 27);
			this->statusStrip2->TabIndex = 24;
			this->statusStrip2->Text = L"statusStrip2";
			// 
			// toolStripProgressBar1
			// 
			this->toolStripProgressBar1->Name = L"toolStripProgressBar1";
			this->toolStripProgressBar1->Size = System::Drawing::Size(133, 19);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold));
			this->label1->Location = System::Drawing::Point(20, 16);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(379, 29);
			this->label1->TabIndex = 25;
			this->label1->Text = L"Search  ——————————————————";
			// 
			// pagesListBox
			// 
			this->pagesListBox->FormattingEnabled = true;
			this->pagesListBox->ItemHeight = 16;
			this->pagesListBox->Location = System::Drawing::Point(21, 457);
			this->pagesListBox->Margin = System::Windows::Forms::Padding(4);
			this->pagesListBox->Name = L"pagesListBox";
			this->pagesListBox->Size = System::Drawing::Size(159, 116);
			this->pagesListBox->TabIndex = 27;
			this->pagesListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::pagesListBox_SelectedIndexChanged);
			// 
			// pagesLabel
			// 
			this->pagesLabel->AutoSize = true;
			this->pagesLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->pagesLabel->Location = System::Drawing::Point(17, 432);
			this->pagesLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->pagesLabel->Name = L"pagesLabel";
			this->pagesLabel->Size = System::Drawing::Size(61, 20);
			this->pagesLabel->TabIndex = 28;
			this->pagesLabel->Text = L"Pages:";
			// 
			// resultsBigLabel
			// 
			this->resultsBigLabel->AutoSize = true;
			this->resultsBigLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold));
			this->resultsBigLabel->Location = System::Drawing::Point(16, 402);
			this->resultsBigLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->resultsBigLabel->Name = L"resultsBigLabel";
			this->resultsBigLabel->Size = System::Drawing::Size(384, 29);
			this->resultsBigLabel->TabIndex = 29;
			this->resultsBigLabel->Text = L"Results  ——————————————————";
			// 
			// oeapnLabel
			// 
			this->oeapnLabel->AutoSize = true;
			this->oeapnLabel->Location = System::Drawing::Point(17, 577);
			this->oeapnLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->oeapnLabel->Name = L"oeapnLabel";
			this->oeapnLabel->Size = System::Drawing::Size(165, 17);
			this->oeapnLabel->TabIndex = 30;
			this->oeapnLabel->Text = L"or Enter a page Number:";
			// 
			// pageNumberLabel
			// 
			this->pageNumberLabel->AutoSize = true;
			this->pageNumberLabel->Location = System::Drawing::Point(17, 593);
			this->pageNumberLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->pageNumberLabel->Name = L"pageNumberLabel";
			this->pageNumberLabel->Size = System::Drawing::Size(62, 17);
			this->pageNumberLabel->TabIndex = 31;
			this->pageNumberLabel->Text = L"(0 page)";
			// 
			// pageNumberTextBox
			// 
			this->pageNumberTextBox->Location = System::Drawing::Point(21, 614);
			this->pageNumberTextBox->Margin = System::Windows::Forms::Padding(4);
			this->pageNumberTextBox->Name = L"pageNumberTextBox";
			this->pageNumberTextBox->Size = System::Drawing::Size(159, 22);
			this->pageNumberTextBox->TabIndex = 32;
			this->pageNumberTextBox->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::pageNumberTextBox_KeyDown);
			this->pageNumberTextBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::pageNumberTextBox_KeyPress);
			// 
			// resultsListBox
			// 
			this->resultsListBox->FormattingEnabled = true;
			this->resultsListBox->ItemHeight = 16;
			this->resultsListBox->Location = System::Drawing::Point(288, 457);
			this->resultsListBox->Margin = System::Windows::Forms::Padding(4);
			this->resultsListBox->Name = L"resultsListBox";
			this->resultsListBox->Size = System::Drawing::Size(159, 116);
			this->resultsListBox->TabIndex = 33;
			this->resultsListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::resultsListBox_SelectedIndexChanged);
			// 
			// resultsLabel
			// 
			this->resultsLabel->AutoSize = true;
			this->resultsLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->resultsLabel->Location = System::Drawing::Point(284, 432);
			this->resultsLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->resultsLabel->Name = L"resultsLabel";
			this->resultsLabel->Size = System::Drawing::Size(71, 20);
			this->resultsLabel->TabIndex = 34;
			this->resultsLabel->Text = L"Results:";
			// 
			// resultNumberLabel
			// 
			this->resultNumberLabel->AutoSize = true;
			this->resultNumberLabel->Location = System::Drawing::Point(284, 593);
			this->resultNumberLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->resultNumberLabel->Name = L"resultNumberLabel";
			this->resultNumberLabel->Size = System::Drawing::Size(65, 17);
			this->resultNumberLabel->TabIndex = 36;
			this->resultNumberLabel->Text = L"(0 result)";
			// 
			// oearnLabel
			// 
			this->oearnLabel->AutoSize = true;
			this->oearnLabel->Location = System::Drawing::Point(284, 577);
			this->oearnLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->oearnLabel->Name = L"oearnLabel";
			this->oearnLabel->Size = System::Drawing::Size(168, 17);
			this->oearnLabel->TabIndex = 35;
			this->oearnLabel->Text = L"or Enter a result Number:";
			// 
			// resolutionXInput
			// 
			this->resolutionXInput->Enabled = false;
			this->resolutionXInput->Location = System::Drawing::Point(21, 361);
			this->resolutionXInput->Margin = System::Windows::Forms::Padding(4);
			this->resolutionXInput->Name = L"resolutionXInput";
			this->resolutionXInput->Size = System::Drawing::Size(136, 22);
			this->resolutionXInput->TabIndex = 37;
			this->resolutionXInput->Text = L"1920";
			this->resolutionXInput->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::resolutionXInput_KeyPress);
			// 
			// resolutionYInput
			// 
			this->resolutionYInput->Enabled = false;
			this->resolutionYInput->Location = System::Drawing::Point(193, 361);
			this->resolutionYInput->Margin = System::Windows::Forms::Padding(4);
			this->resolutionYInput->Name = L"resolutionYInput";
			this->resolutionYInput->Size = System::Drawing::Size(136, 22);
			this->resolutionYInput->TabIndex = 38;
			this->resolutionYInput->Text = L"1080";
			this->resolutionYInput->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::resolutionYInput_KeyPress);
			// 
			// resolutionSeperatorLabel
			// 
			this->resolutionSeperatorLabel->AutoSize = true;
			this->resolutionSeperatorLabel->Enabled = false;
			this->resolutionSeperatorLabel->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->resolutionSeperatorLabel->Location = System::Drawing::Point(167, 363);
			this->resolutionSeperatorLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->resolutionSeperatorLabel->Name = L"resolutionSeperatorLabel";
			this->resolutionSeperatorLabel->Size = System::Drawing::Size(18, 21);
			this->resolutionSeperatorLabel->TabIndex = 39;
			this->resolutionSeperatorLabel->Text = L"x";
			// 
			// atLeastRadioButton
			// 
			this->atLeastRadioButton->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->atLeastRadioButton->Location = System::Drawing::Point(21, 330);
			this->atLeastRadioButton->Margin = System::Windows::Forms::Padding(4);
			this->atLeastRadioButton->Name = L"atLeastRadioButton";
			this->atLeastRadioButton->Size = System::Drawing::Size(112, 21);
			this->atLeastRadioButton->TabIndex = 0;
			this->atLeastRadioButton->Text = L"At Least:";
			this->atLeastRadioButton->UseVisualStyleBackColor = true;
			this->atLeastRadioButton->CheckedChanged += gcnew System::EventHandler(this, &MyForm::atLeastRadioButton_CheckedChanged);
			// 
			// exactlyRadioButton
			// 
			this->exactlyRadioButton->AutoSize = true;
			this->exactlyRadioButton->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->exactlyRadioButton->Location = System::Drawing::Point(181, 327);
			this->exactlyRadioButton->Margin = System::Windows::Forms::Padding(4);
			this->exactlyRadioButton->Name = L"exactlyRadioButton";
			this->exactlyRadioButton->Size = System::Drawing::Size(81, 25);
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
			this->pixelsLabel->Location = System::Drawing::Point(339, 363);
			this->pixelsLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->pixelsLabel->Name = L"pixelsLabel";
			this->pixelsLabel->Size = System::Drawing::Size(50, 21);
			this->pixelsLabel->TabIndex = 41;
			this->pixelsLabel->Text = L"pixels";
			// 
			// noSelectRadioButton
			// 
			this->noSelectRadioButton->AutoSize = true;
			this->noSelectRadioButton->Checked = true;
			this->noSelectRadioButton->Font = (gcnew System::Drawing::Font(L"Corbel", 10));
			this->noSelectRadioButton->Location = System::Drawing::Point(296, 327);
			this->noSelectRadioButton->Margin = System::Windows::Forms::Padding(4);
			this->noSelectRadioButton->Name = L"noSelectRadioButton";
			this->noSelectRadioButton->Size = System::Drawing::Size(185, 25);
			this->noSelectRadioButton->TabIndex = 42;
			this->noSelectRadioButton->TabStop = true;
			this->noSelectRadioButton->Text = L"No Resolution Sorting";
			this->noSelectRadioButton->UseVisualStyleBackColor = true;
			this->noSelectRadioButton->CheckedChanged += gcnew System::EventHandler(this, &MyForm::noSelectRadioButton_CheckedChanged);
			// 
			// resultNumberTextbox
			// 
			this->resultNumberTextbox->Location = System::Drawing::Point(288, 614);
			this->resultNumberTextbox->Margin = System::Windows::Forms::Padding(4);
			this->resultNumberTextbox->Name = L"resultNumberTextbox";
			this->resultNumberTextbox->Size = System::Drawing::Size(159, 22);
			this->resultNumberTextbox->TabIndex = 43;
			this->resultNumberTextbox->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::resultNumberTextbox_KeyDown);
			this->resultNumberTextbox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::resultNumberTextbox_KeyPress);
			// 
			// pageOkButton
			// 
			this->pageOkButton->Location = System::Drawing::Point(189, 612);
			this->pageOkButton->Margin = System::Windows::Forms::Padding(4);
			this->pageOkButton->Name = L"pageOkButton";
			this->pageOkButton->Size = System::Drawing::Size(57, 28);
			this->pageOkButton->TabIndex = 45;
			this->pageOkButton->Text = L"Ok";
			this->pageOkButton->UseVisualStyleBackColor = true;
			this->pageOkButton->Click += gcnew System::EventHandler(this, &MyForm::pageOkButton_Click);
			// 
			// resultOkButton
			// 
			this->resultOkButton->Location = System::Drawing::Point(456, 612);
			this->resultOkButton->Margin = System::Windows::Forms::Padding(4);
			this->resultOkButton->Name = L"resultOkButton";
			this->resultOkButton->Size = System::Drawing::Size(57, 28);
			this->resultOkButton->TabIndex = 46;
			this->resultOkButton->Text = L"Ok";
			this->resultOkButton->UseVisualStyleBackColor = true;
			this->resultOkButton->Click += gcnew System::EventHandler(this, &MyForm::resultOkButton_Click);
			// 
			// imageLinkLabel
			// 
			this->imageLinkLabel->AutoSize = true;
			this->imageLinkLabel->Location = System::Drawing::Point(21, 688);
			this->imageLinkLabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->imageLinkLabel->Name = L"imageLinkLabel";
			this->imageLinkLabel->Size = System::Drawing::Size(80, 17);
			this->imageLinkLabel->TabIndex = 47;
			this->imageLinkLabel->Text = L"Image Link:";
			// 
			// autoCounterInput
			// 
			this->autoCounterInput->Enabled = false;
			this->autoCounterInput->Location = System::Drawing::Point(327, 646);
			this->autoCounterInput->Margin = System::Windows::Forms::Padding(4);
			this->autoCounterInput->Name = L"autoCounterInput";
			this->autoCounterInput->Size = System::Drawing::Size(81, 22);
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
			this->counterDropDown->Location = System::Drawing::Point(417, 645);
			this->counterDropDown->Margin = System::Windows::Forms::Padding(4);
			this->counterDropDown->Name = L"counterDropDown";
			this->counterDropDown->Size = System::Drawing::Size(64, 24);
			this->counterDropDown->TabIndex = 49;
			// 
			// notifyIcon1
			// 
			this->notifyIcon1->Text = L"Whaven Wallpaper";
			this->notifyIcon1->Visible = true;
			this->notifyIcon1->Click += gcnew System::EventHandler(this, &MyForm::notifyIcon1_Click);
			// 
			// MyForm
			// 
			this->AcceptButton = this->searchButton;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(516, 745);
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
			this->Margin = System::Windows::Forms::Padding(4);
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
		private: String^ apikey;
		private: Thread^ counterThread;
		private: array<String^>^ paths;
		private: array<String^>^ shortUrls;
		private: array<String^>^ IDs;
		private: array<String^>^ fileTypes;
		private: array<String^>^ purity;
		private: static bool repeat = true;
		private: bool pageInit = false;
		private: bool resultInit = false;
		private: bool repeatInit = false;
		private: int selectedResultIndex = 0;
		private: int page = 0;
		private: int counter;
		private: bool download;
		private: delegate System::Void _updateStrip(String^ toolStripText, int progress, int statusCode);
		private: System::Void updateStrip(String^ toolStripText, int progress, int statusCode)
		{
			if (statusCode != 0) {
				this->toolStripProgressBar1->BackColor = System::Drawing::Color::DarkRed;
			}
			this->toolStripStatusLabel1->Text = toolStripText;
			this->toolStripProgressBar1->Value = progress;
		}
		private: delegate System::Void uPathLink(String^ pathLink);
		private: System::Void updatePathLink(String^ pathLink) 
		{
			this->pathLinkLabel->Text = pathLink;
		}
		// TODO: Logger
		private: delegate System::Void _download_jpeg(char* i);
		//
		//	function searchButton_Click
		//

		private: System::Void searchButton_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			qBuilder^ qBuild = gcnew qBuilder(this, &MyForm::queryBuilder);
			String^ _url = qBuild(0);

			array<Windows::Forms::Control^>^ exceptedComponents = gcnew array<Windows::Forms::Control^>(4);
			exceptedComponents[0] = this->imageLinkLabel;
			exceptedComponents[1] = this->pathLinkLabel;
			exceptedComponents[2] = this->statusStrip1;
			exceptedComponents[3] = this->statusStrip2;

			Thread^ disableComponents = gcnew Thread(gcnew ParameterizedThreadStart(this, &MyForm::disableButtons));
			disableComponents->Start(exceptedComponents);
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);
			Control::Invoke(us, gcnew String("Url build completed, sending json request..."), 12, 0);
			JsonUtils^ jd = gcnew JsonUtils(_url, gcnew JsonUtils::requestCallback(this, &MyForm::jsonRequestCallbackHandler));
			Thread^ backgroundThread = gcnew Thread(gcnew ThreadStart(jd, &JsonUtils::jsonRequest));
			backgroundThread->Start();
			/* Thread^ backgroundThread = gcnew Thread(gcnew ThreadStart(this, &MyForm::queryBuilder));
			backgroundThread->Start();*/
		}
		private: delegate String^ qBuilder(int statusCode);
		private: String^ queryBuilder(int statusCode) {
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);
			Control::Invoke(us, gcnew String("Building url..."), 5, 0);
			delete us;
			if (statusCode != 0) {
				page = this->pagesListBox->SelectedIndex + 1;
			}
			msclr::interop::marshal_context context;
			std::string url = "https://wallhaven.cc/api/v1/search?";
			if (apikey != gcnew String(""))
				url += "apikey=" + context.marshal_as<std::string>(apikey);
			url += (std::string)"&q=" + context.marshal_as<std::string>(this->textBox1->Text);

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

			// I used switch case first. But program bugged. :/ And i trie
			int sortItem = this->sortingDropdown->SelectedIndex;
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

			int orderItem = orderDropdown->SelectedIndex;

			if (orderItem == 0)
				url += (std::string)"&order=desc";
			else if (orderItem == 1)
				url += (std::string)"&order=asc";
			if(page == 0)
				url += (std::string)"&page=1";
			else
				url += ("&page=" + std::to_string(page));

			if (this->atLeastRadioButton->Checked)
				url += "&atleast=" + context.marshal_as<std::string>(this->resolutionXInput->Text) + "x" + context.marshal_as<std::string>(this->resolutionYInput->Text);
			if (this->exactlyRadioButton->Checked)
				url += "&resolutions=" + context.marshal_as<std::string>(this->resolutionXInput->Text) + "x" + context.marshal_as<std::string>(this->resolutionYInput->Text);

			String^ _url = gcnew String(url.c_str());
			return _url;
		}
		//
		//	function disableButtons(Object^ exceptedComponentArray): this function disables all components except Object^ exceptedComponentArray(array<String^>).
		//
		private: System::Void disableButtons(Object^ exceptedComponentArray) {
			array<Windows::Forms::Control^>^ dComponents = dynamic_cast<array< Windows::Forms::Control^>^>(exceptedComponentArray);
			array<Windows::Forms::Control^>^ allComponents = gcnew array<Windows::Forms::Control^>(this->Controls->Count);
			this->Controls->CopyTo(allComponents, 0);
			getCName^ getName = gcnew getCName(this, &MyForm::getComponentName);
			dComponent^ disComponent = gcnew dComponent(this, &MyForm::disableComponent);
			bool matched = true;
			for (int i = 0; i < allComponents->Length; i += 1) 
			{
				for (int x = 0; x < dComponents->Length; x += 1) 
				{
					String^ acName = dynamic_cast<String^>(Control::Invoke(getName, allComponents[i]));
					String^ dName = dynamic_cast<String^>(Control::Invoke(getName, dComponents[x]));
					if (acName == dName)
						matched = true;
				}
				if (matched)
				{
					matched = false;
					continue;
				}
				else {
					Control::Invoke(disComponent, allComponents[i]);
				}
 			}
		}
		//
		//	function enableButtons(Object^ exceptedComponentArray): this function enables all components except Object^ exceptedComponentArray(array<String^>).
		//
		private: System::Void enableButtons(Object^ exceptedComponentArray) {
			array<Windows::Forms::Control^>^ eComponents = dynamic_cast<array< Windows::Forms::Control^>^>(exceptedComponentArray);
			array<Windows::Forms::Control^>^ allComponents = gcnew array<Windows::Forms::Control^>(this->Controls->Count);
			this->Controls->CopyTo(allComponents, 0);
			getCName^ getName = gcnew getCName(this, &MyForm::getComponentName);
			bool matched = true;
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
			setR^ sRes = gcnew setR(this, &MyForm::setRestComponents);
			Control::Invoke(sRes);
			
		}
		private: delegate System::Void setR();
		private: System::Void setRestComponents() {
			if (this->noSelectRadioButton->Checked) {
				this->resolutionXInput->Enabled = false;
				this->resolutionYInput->Enabled = false;
				this->resolutionSeperatorLabel->Enabled = false;
				this->pixelsLabel->Enabled = false;
			}
			this->counterDropDown->Enabled = true;
		}
		private: delegate System::String^ getCName(Windows::Forms::Control^ i);
		private: System::String^ getComponentName(Windows::Forms::Control^ i) {
			return i->Name;
		}
		private: delegate System::Void eComponent(Windows::Forms::Control^ i);
		private: System::Void enableComponent(Windows::Forms::Control^ i) {
			i->Enabled = true;
		}
		private: delegate System::Void dComponent(Windows::Forms::Control^ i);
		private: System::Void disableComponent(Windows::Forms::Control^ i) {
			i->Enabled = false;
		}
		private: System::Void jsonRequestCallbackHandler(String^ j, int i) 
		{
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);

			if (i == 200) 
			{
				Control::Invoke(us, gcnew String("Json Retrieved and Processing..."), 25, 0);
				JsonUtils^ prJson = gcnew JsonUtils(j, 1, gcnew JsonUtils::jsProcessCallback(this, &MyForm::jsonProcessCallBack));
				Thread^ backgroundThread = gcnew Thread(gcnew ThreadStart(prJson, &JsonUtils::processJson));
				backgroundThread->Start();
			}
			else 
			{
				Control::Invoke(us, gcnew String("Error While Getting Json Http code:" + i), 25, i);
			}
		}
		private: delegate System::Void setPRange(array<Object^>^ pageArray);
		private: System::Void setPageRange(array<Object^>^ pageArray) {
			this->pagesListBox->Items->Clear();
			this->pagesListBox->Items->AddRange(pageArray);
		}
		private: delegate System::Void setRRange(array<Object^>^ resultsArray);
		private: System::Void setResultsRange(array<Object^>^ resultsArray) {
			this->resultsListBox->Items->Clear();
			this->resultsListBox->Items->AddRange(resultsArray);
		}
		private: delegate System::Void selectP(int selectedPage);
		private: System::Void selectPage(int selectedPage) {
			this->pagesListBox->SetSelected(selectedPage, true);
		}
		private: delegate System::Void selectR(int i);
		private: System::Void selectResult(int selectedResult) {
			this->resultsListBox->SetSelected(selectedResult, true);
		}
		private: System::Void jsonProcessCallBack(Object^ tupleContent) {
			auto args = dynamic_cast<Tuple<int, int, array<String^>^, array<String^>^, array<String^>^, array<String^>^, array<String^>^>^>(tupleContent);
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);
			if (args->Item1 != 27272727) {
				Control::Invoke(us, gcnew String("Json process Completed Downloading jpeg..."), 50, 0);

				int pageNumber = args->Item1;
				int resultNumber = args->Item2;
				paths = args->Item3;
				shortUrls = args->Item4;
				IDs = args->Item5;
				fileTypes = args->Item6;
				purity = args->Item7;
				selectedResultIndex = 0;

				jpegDownloadCallback^ jCB = gcnew jpegDownloadCallback(this, &MyForm::jpegDownloadCback);
				JpegUtils^ jUtils = gcnew JpegUtils(paths[0], IDs[0], fileTypes[0], purity[0], download, jCB);
				Thread^ downloadJpeg = gcnew Thread(gcnew ThreadStart(jUtils, &JpegUtils::download_jpeg));
				downloadJpeg->Start();

				array<System::Object^>^ pageItemObject = gcnew array<System::Object^>(pageNumber);

				for (int i = 0; i < pageNumber; i += 1) {
					pageItemObject[i] = String::Concat("Page ", (i + 1).ToString());
				}
				setPRange^ setPgRange = gcnew setPRange(this, &MyForm::setPageRange);
				Control::Invoke(setPgRange, gcnew array<Object^>{pageItemObject});

				array<System::Object^>^ resultsItemObject = gcnew array<System::Object^>(resultNumber);

				for (int i = 0; i < resultNumber; i += 1) {
					resultsItemObject[i] = String::Concat("Result ", (i + 1).ToString());
				}

				setRRange^ setRsRange = gcnew setRRange(this, &MyForm::setResultsRange);
				Control::Invoke(setRsRange, gcnew array<Object^>{resultsItemObject});

				if (pageNumber >= 1) {
					pageInit = true;
					resultInit = true;
					selectP^ setPg = gcnew selectP(this, &MyForm::selectPage);
					Control::Invoke(setPg, page);
					selectR^ setRs = gcnew selectR(this, &MyForm::selectResult);
					Control::Invoke(setRs, 0);
					_updatePageLabel^ uPageLabel = gcnew _updatePageLabel(this, &MyForm::updatePageLabel);
					Control::Invoke(uPageLabel, (gcnew String("(") + pageNumber + gcnew String(" page)")));
					_updateResultsLabel^ uResultsLabel = gcnew _updateResultsLabel(this, &MyForm::updateResultsLabel);
					Control::Invoke(uResultsLabel, (gcnew String("(") + resultNumber + gcnew String(" result)")));
					uPathLink^ uPath = gcnew uPathLink(this, &MyForm::updatePathLink);
					Control::Invoke(uPath, shortUrls[0]);
				}

				delete setPgRange;
				delete pageItemObject;
				delete setRsRange;
				delete resultsItemObject;
			}
			else
			{
				Control::Invoke(us, gcnew String("Error while processing json"), 50, 272727);
			}
		}
		private: delegate System::Void _updatePageLabel(String^ pageLabelText);
		private: System::Void updatePageLabel(String^ pageLabelText)
		{
			this->pageNumberLabel->Text = pageLabelText;
		}
		private: delegate System::Void _updateResultsLabel(String^ resultLabelText);
		private: System::Void updateResultsLabel(String^ resultLabelText)
		{
			this->resultNumberLabel->Text = resultLabelText;
		}
		private: System::Void jpegDownloadCback(int statusCode) 
		{
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);

			if (statusCode == 0) 
			{
				Control::Invoke(us, gcnew String("Jpeg Download Completed. Setting Jpeg As Wallpaper..."), 85, statusCode);
				setWallpaperCallback^ _setWallpaperCallback = gcnew setWallpaperCallback(this, &MyForm::sWallpaperCallback);
				JpegUtils^ jUtil = gcnew JpegUtils(IDs[selectedResultIndex], fileTypes[selectedResultIndex], purity[selectedResultIndex], download, _setWallpaperCallback);

				Thread^ setWallpaper = gcnew Thread(gcnew ThreadStart(jUtil, &JpegUtils::setWallpaper));
				setWallpaper->Start();
			}
			else
			{
				Control::Invoke(us, gcnew String("Jpeg Download Failed"), 85, statusCode);
			}

		}
		private: System::Void sWallpaperCallback(int statusCode) {
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);
			array<Windows::Forms::Control^>^ enableAllComponents = gcnew array<Windows::Forms::Control^>(0);
			Thread^ enableComponents = gcnew Thread(gcnew ParameterizedThreadStart(this, &MyForm::enableButtons));
			enableComponents->Start(enableAllComponents);
			if (statusCode == 0)
			{
				Control::Invoke(us, gcnew String("Jpeg setted as wallpaper"), 100, statusCode);
				Control::Invoke(us, gcnew String("Application Ready"), 0, statusCode);
			}
			else
			{
				Control::Invoke(us, gcnew String("Setting Jpeg as wallpaper failed"), 100, statusCode);
			}

		}
		private: delegate System::Void cResultComponents();
		private: System::Void disableResultComponents() {
			dComponent^ disComponent = gcnew dComponent(this, &MyForm::disableComponent);
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
		private: System::Void pathLinkLabelClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e) {
			System::Diagnostics::Process::Start(this->pathLinkLabel->Text);
		}
		//
		// function repeatCheckBox_CheckedChanged
		//
		private: System::Void repeatCheckBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
			if (counterThread != nullptr) {
				counterThread->Abort();
				counterThread = nullptr;
			}
			repeat = this->autoCheckBox->Checked;
			repeatInit = true;
			counterThread = gcnew Thread(gcnew ThreadStart(this, &MyForm::Repeat));
			counterThread->Start();
		}
		private: System::Void MyForm::Repeat()
		{
			_getCounterInput^ gCounterInput;
			_getCounterIndex^ gCounterIndex;
			int selectedCounterIndex;
			// auto args = dynamic_cast<Tuple<String^, String^, int>^>(i);
			while (repeat) {
				gCounterInput = gcnew _getCounterInput(this, &MyForm::getCounterInput);
				gCounterIndex = gcnew _getCounterIndex(this, &MyForm::getCounterIndex);
				selectedCounterIndex = static_cast<int>(Control::Invoke(gCounterIndex));
				try {
					counter = static_cast<int>(Control::Invoke(gCounterInput));
				}
				catch (Exception^) {
					counter = 1800000;
				}

				if (selectedCounterIndex == 0)
				{
					if (counter <= 30) {
						counter = 30;
					}
					counter *= 1000;
				}
				else if (selectedCounterIndex == 1)
				{
					if (counter <= 1) {
						counter = 1;
					}
					counter *= 60000;
				}
				else if (selectedCounterIndex == 2)
				{
					if (counter <= 1) {
						counter = 1;
					}
					counter *= 3600000;
				}
				if (repeatInit) {
					repeatInit = false;
					Thread::Sleep(counter);
				}
				selectedResultIndex += 1;
				if (selectedResultIndex >= paths->Length) {
					page += 1;
					selectP^ setPg = gcnew selectP(this, &MyForm::selectPage);
					Control::Invoke(setPg, page);
					Thread::Sleep(counter);
				}
				else {
					selectR^ setRs = gcnew selectR(this, &MyForm::selectResult);
					Control::Invoke(setRs, selectedResultIndex);
					Thread::Sleep(counter);
				}
			}
		}
		private: delegate int _getCounterInput();
		private: int getCounterInput() {
			return int::Parse(this->autoCounterInput->Text);
		}
		private: delegate int _getCounterIndex();
		private: int getCounterIndex() {
			return this->counterDropDown->SelectedIndex;
		}
		private: System::Void resultsListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
			if (resultInit)
			{
				resultInit = false;
				return;
			}
			array<Windows::Forms::Control^>^ exceptedComponents = gcnew array<Windows::Forms::Control^>(4);
			exceptedComponents[0] = this->imageLinkLabel;
			exceptedComponents[1] = this->pathLinkLabel;
			exceptedComponents[2] = this->statusStrip1;
			exceptedComponents[3] = this->statusStrip2;
			Thread^ disableComponents = gcnew Thread(gcnew ParameterizedThreadStart(this, &MyForm::disableButtons));
			disableComponents->Start(exceptedComponents);
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);
			Control::Invoke(us, gcnew String("Downloading jpeg..."), 50, 0);
			selectedResultIndex = this->resultsListBox->SelectedIndex;
			uPathLink^ uPath = gcnew uPathLink(this, &MyForm::updatePathLink);
			Control::Invoke(uPath, shortUrls[selectedResultIndex]);
			jpegDownloadCallback^ jCB = gcnew jpegDownloadCallback(this, &MyForm::jpegDownloadCback);
			JpegUtils^ jUtils = gcnew JpegUtils(paths[selectedResultIndex], IDs[selectedResultIndex], fileTypes[selectedResultIndex], purity[selectedResultIndex], download, jCB);
			Thread^ downloadJpeg = gcnew Thread(gcnew ThreadStart(jUtils, &JpegUtils::download_jpeg));
			downloadJpeg->Start();
		}
		private: System::Void pagesListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
			if (pageInit)
			{
				pageInit = false;
				return;
			}
			qBuilder^ qBuild = gcnew qBuilder(this, &MyForm::queryBuilder);
			String^ _url = qBuild(1);
			array<Windows::Forms::Control^>^ exceptedComponents = gcnew array<Windows::Forms::Control^>(4);
			exceptedComponents[0] = this->imageLinkLabel;
			exceptedComponents[1] = this->pathLinkLabel;
			exceptedComponents[2] = this->statusStrip1;
			exceptedComponents[3] = this->statusStrip2;
			Thread^ disableComponents = gcnew Thread(gcnew ParameterizedThreadStart(this, &MyForm::disableButtons));
			disableComponents->Start(exceptedComponents);
			_updateStrip^ us = gcnew _updateStrip(this, &MyForm::updateStrip);
			Control::Invoke(us, gcnew String("Url build completed, sending json request..."), 12, 0);
			JsonUtils^ jd = gcnew JsonUtils(_url, gcnew JsonUtils::requestCallback(this, &MyForm::jsonRequestCallbackHandler));
			Thread^ backgroundThread = gcnew Thread(gcnew ThreadStart(jd, &JsonUtils::jsonRequest));
			backgroundThread->Start();
		}
		private: System::Void textBox1_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			if (e->KeyData == System::Windows::Forms::Keys::Enter)
				searchButton_Click("", nullptr);
		}
		private: System::Void autoCounterInput_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
			if (e->KeyChar == '.') {
				if (this->autoCounterInput->Text->Contains(".") && !this->autoCounterInput->SelectedText->Contains("."))
					e->Handled = true;
			}
			// Accept only digits ".", "-" and the Backspace character
			else if (!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
				e->Handled = true;
			}	
		}
		private: System::Void resolutionXInput_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
			if (e->KeyChar == '.') {
				if (this->resolutionXInput->Text->Contains(".") && !this->resolutionXInput->SelectedText->Contains("."))
					e->Handled = true;
			}
			// Accept only digits ".", "-" and the Backspace character
			else if (!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
				e->Handled = true;
			}
		}
		private: System::Void resolutionYInput_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
			if (e->KeyChar == '.') {
				if (this->resolutionYInput->Text->Contains(".") && !this->resolutionYInput->SelectedText->Contains("."))
					e->Handled = true;
			}
			// Accept only digits ".", "-" and the Backspace character
			else if (!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
				e->Handled = true;
			}
		}
		private: System::Void pageNumberTextBox_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
			if (e->KeyChar == '.') {
				if (this->pageNumberTextBox->Text->Contains(".") && !this->pageNumberTextBox->SelectedText->Contains("."))
					e->Handled = true;
			}
			// Accept only digits ".", "-" and the Backspace character
			else if (!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
				e->Handled = true;
			}
		}
		private: System::Void resultNumberTextbox_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
			if (e->KeyChar == '.') {
				if (this->resultNumberTextbox->Text->Contains(".") && !this->resultNumberTextbox->SelectedText->Contains("."))
					e->Handled = true;
			}
			// Accept only digits ".", "-" and the Backspace character
			else if (!Char::IsDigit(e->KeyChar) && e->KeyChar != 0x08) {
				e->Handled = true;
			}
		}
		private: System::Void autoCounterInput_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			if (this->autoCounterInput->Text == gcnew String("") && this->counterDropDown->SelectedIndex == 0)
				this->autoCounterInput->Text = "10";
			else if ((this->autoCounterInput->Text == gcnew String("") && this->counterDropDown->SelectedIndex == 1) && (this->autoCounterInput->Text == gcnew String("") && this->counterDropDown->SelectedIndex == 2))
				this->autoCounterInput->Text = "1";
			counter = int::Parse(this->autoCounterInput->Text);
		}
		private: System::Void noSelectRadioButton_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
			this->resolutionXInput->Enabled = false;
			this->resolutionYInput->Enabled = false;
			this->resolutionSeperatorLabel->Enabled = false;
			this->pixelsLabel->Enabled = false;
		}
		private: System::Void exactlyRadioButton_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
			this->resolutionXInput->Enabled = true;
			this->resolutionYInput->Enabled = true;
			this->resolutionSeperatorLabel->Enabled = true;
			this->pixelsLabel->Enabled = true;
		}
		private: System::Void atLeastRadioButton_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
			this->resolutionXInput->Enabled = true;
			this->resolutionYInput->Enabled = true;
			this->resolutionSeperatorLabel->Enabled = true;
			this->pixelsLabel->Enabled = true;
		}
		private: System::Void pageOkButton_Click(System::Object^ sender, System::EventArgs^ e) {
			try 
			{
				page = int::Parse(this->pageNumberTextBox->Text);
			}
			catch (Exception^)
			{
				page = this->pagesListBox->SelectedIndex;
			}
			selectP^ setPg = gcnew selectP(this, &MyForm::selectPage);
			Control::Invoke(setPg, page);
		}
		private: System::Void resultOkButton_Click(System::Object^ sender, System::EventArgs^ e) {
			try
			{
				selectedResultIndex = int::Parse(this->resultNumberTextbox->Text);

			}

			catch (Exception^)
			{
				selectedResultIndex = this->resultsListBox->SelectedIndex;
			}
			selectR^ setRs = gcnew selectR(this, &MyForm::selectResult);
			Control::Invoke(setRs, selectedResultIndex);
		}
		private: System::Void pageNumberTextBox_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			if (e->KeyData == System::Windows::Forms::Keys::Enter)
				pageOkButton_Click("", nullptr);
		}
		private: System::Void resultNumberTextbox_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
			if (e->KeyData == System::Windows::Forms::Keys::Enter)
				resultOkButton_Click("", nullptr);
		}
	private: System::Void notifyIcon1_Click(System::Object^ sender, System::EventArgs^ e) {
		this->Visible = true;
		this->notifyIcon1->Visible = false;
		this->WindowState = System::Windows::Forms::FormWindowState::Normal;
	}
private: System::Void MyForm_Resize(System::Object^ sender, System::EventArgs^ e) {
	if (this->WindowState == System::Windows::Forms::FormWindowState::Minimized) {
		this->notifyIcon1->Visible = true;
		this->Visible = false;
	}
}
};
}



	




