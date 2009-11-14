#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

extern "C" {
	void Capture3D(HWND hWnd, LPCSTR szFileName);
};

namespace genkeytool {

	/// <summary>
	/// Summary for frmMain
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmMain : public System::Windows::Forms::Form
	{
	private:
		HWND		m_hWnd;

	private: System::Windows::Forms::Button^  btnRun;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Label^  lblDragDrop;

			 PWINDOWINFO m_pWindowInfo;
	public:
		frmMain(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmMain()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txtWindowClass;
	protected: 


	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  txtHandle;
	private: System::Windows::Forms::TextBox^  txtRectLeft;


	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  txControlID;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  txtRectWidth;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::TextBox^  txtRectHeight;
	private: System::Windows::Forms::TextBox^  txtSampleLenght;


	private: System::Windows::Forms::TextBox^  txtSampleOriginY;

	private: System::Windows::Forms::TextBox^  txtSampleOriginX;




	private: System::Windows::Forms::TextBox^  txtRectTop;

	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::TextBox^  txtKey;

	private: System::Windows::Forms::TextBox^  txtWindowTitle;

	private: System::Windows::Forms::Label^  label14;
	private: System::ComponentModel::BackgroundWorker^  bgwMouseReader;
	private: System::ComponentModel::IContainer^  components;

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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtWindowClass = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->txtHandle = (gcnew System::Windows::Forms::TextBox());
			this->txtRectLeft = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->txControlID = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->txtRectWidth = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->lblDragDrop = (gcnew System::Windows::Forms::Label());
			this->btnRun = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->txtRectHeight = (gcnew System::Windows::Forms::TextBox());
			this->txtKey = (gcnew System::Windows::Forms::TextBox());
			this->txtSampleLenght = (gcnew System::Windows::Forms::TextBox());
			this->txtSampleOriginY = (gcnew System::Windows::Forms::TextBox());
			this->txtSampleOriginX = (gcnew System::Windows::Forms::TextBox());
			this->txtRectTop = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->txtWindowTitle = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->bgwMouseReader = (gcnew System::ComponentModel::BackgroundWorker());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 47);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(76, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Window class:";
			// 
			// txtWindowClass
			// 
			this->txtWindowClass->Enabled = false;
			this->txtWindowClass->Location = System::Drawing::Point(88, 44);
			this->txtWindowClass->Name = L"txtWindowClass";
			this->txtWindowClass->Size = System::Drawing::Size(348, 19);
			this->txtWindowClass->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(14, 41);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(27, 12);
			this->label2->TabIndex = 0;
			this->label2->Text = L"Left:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(7, 73);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(42, 12);
			this->label3->TabIndex = 0;
			this->label3->Text = L"Handle:";
			// 
			// txtHandle
			// 
			this->txtHandle->Enabled = false;
			this->txtHandle->Location = System::Drawing::Point(88, 70);
			this->txtHandle->Name = L"txtHandle";
			this->txtHandle->Size = System::Drawing::Size(130, 19);
			this->txtHandle->TabIndex = 1;
			// 
			// txtRectLeft
			// 
			this->txtRectLeft->AcceptsReturn = true;
			this->txtRectLeft->Location = System::Drawing::Point(55, 38);
			this->txtRectLeft->Name = L"txtRectLeft";
			this->txtRectLeft->Size = System::Drawing::Size(65, 19);
			this->txtRectLeft->TabIndex = 1;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(241, 73);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(59, 12);
			this->label4->TabIndex = 0;
			this->label4->Text = L"Control ID:";
			// 
			// txControlID
			// 
			this->txControlID->AcceptsReturn = true;
			this->txControlID->Enabled = false;
			this->txControlID->Location = System::Drawing::Point(306, 70);
			this->txControlID->Name = L"txControlID";
			this->txControlID->Size = System::Drawing::Size(130, 19);
			this->txControlID->TabIndex = 1;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(14, 68);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(26, 12);
			this->label5->TabIndex = 0;
			this->label5->Text = L"Top:";
			// 
			// txtRectWidth
			// 
			this->txtRectWidth->Location = System::Drawing::Point(55, 90);
			this->txtRectWidth->Name = L"txtRectWidth";
			this->txtRectWidth->Size = System::Drawing::Size(65, 19);
			this->txtRectWidth->TabIndex = 1;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->lblDragDrop);
			this->groupBox1->Controls->Add(this->btnRun);
			this->groupBox1->Controls->Add(this->pictureBox1);
			this->groupBox1->Controls->Add(this->label15);
			this->groupBox1->Controls->Add(this->label10);
			this->groupBox1->Controls->Add(this->label9);
			this->groupBox1->Controls->Add(this->label8);
			this->groupBox1->Controls->Add(this->label13);
			this->groupBox1->Controls->Add(this->label7);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->label12);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->txtRectHeight);
			this->groupBox1->Controls->Add(this->txtRectWidth);
			this->groupBox1->Controls->Add(this->txtKey);
			this->groupBox1->Controls->Add(this->txtSampleLenght);
			this->groupBox1->Controls->Add(this->txtSampleOriginY);
			this->groupBox1->Controls->Add(this->txtSampleOriginX);
			this->groupBox1->Controls->Add(this->txtRectTop);
			this->groupBox1->Controls->Add(this->label11);
			this->groupBox1->Controls->Add(this->txtRectLeft);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Location = System::Drawing::Point(12, 114);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(446, 187);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Rectangle && Sample Information";
			// 
			// lblDragDrop
			// 
			this->lblDragDrop->AllowDrop = true;
			this->lblDragDrop->AutoSize = true;
			this->lblDragDrop->Location = System::Drawing::Point(18, 155);
			this->lblDragDrop->Name = L"lblDragDrop";
			this->lblDragDrop->Size = System::Drawing::Size(19, 12);
			this->lblDragDrop->TabIndex = 4;
			this->lblDragDrop->Text = L"[+]";
			// 
			// btnRun
			// 
			this->btnRun->Location = System::Drawing::Point(165, 155);
			this->btnRun->Name = L"btnRun";
			this->btnRun->Size = System::Drawing::Size(112, 21);
			this->btnRun->TabIndex = 3;
			this->btnRun->Text = L"Run";
			this->btnRun->UseVisualStyleBackColor = true;
			// 
			// pictureBox1
			// 
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox1->Location = System::Drawing::Point(291, 37);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(145, 140);
			this->pictureBox1->TabIndex = 2;
			this->pictureBox1->TabStop = false;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(146, 118);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(26, 12);
			this->label15->TabIndex = 0;
			this->label15->Text = L"Key:";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(131, 96);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(41, 12);
			this->label10->TabIndex = 0;
			this->label10->Text = L"Length:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(158, 70);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(13, 12);
			this->label9->TabIndex = 0;
			this->label9->Text = L"y:";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(158, 43);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(13, 12);
			this->label8->TabIndex = 0;
			this->label8->Text = L"x:";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(14, 20);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(56, 12);
			this->label13->TabIndex = 0;
			this->label13->Text = L"Rectangle";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(289, 20);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(98, 12);
			this->label7->TabIndex = 0;
			this->label7->Text = L"Rectangle Picture:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(129, 22);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(87, 12);
			this->label6->TabIndex = 0;
			this->label6->Text = L"Sample Position";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(14, 93);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(35, 12);
			this->label12->TabIndex = 0;
			this->label12->Text = L"Width:";
			// 
			// txtRectHeight
			// 
			this->txtRectHeight->Location = System::Drawing::Point(55, 117);
			this->txtRectHeight->Name = L"txtRectHeight";
			this->txtRectHeight->Size = System::Drawing::Size(65, 19);
			this->txtRectHeight->TabIndex = 1;
			// 
			// txtKey
			// 
			this->txtKey->Location = System::Drawing::Point(177, 115);
			this->txtKey->Name = L"txtKey";
			this->txtKey->Size = System::Drawing::Size(101, 19);
			this->txtKey->TabIndex = 1;
			// 
			// txtSampleLenght
			// 
			this->txtSampleLenght->Location = System::Drawing::Point(177, 93);
			this->txtSampleLenght->Name = L"txtSampleLenght";
			this->txtSampleLenght->Size = System::Drawing::Size(101, 19);
			this->txtSampleLenght->TabIndex = 1;
			// 
			// txtSampleOriginY
			// 
			this->txtSampleOriginY->AcceptsReturn = true;
			this->txtSampleOriginY->Location = System::Drawing::Point(177, 67);
			this->txtSampleOriginY->Name = L"txtSampleOriginY";
			this->txtSampleOriginY->Size = System::Drawing::Size(101, 19);
			this->txtSampleOriginY->TabIndex = 1;
			// 
			// txtSampleOriginX
			// 
			this->txtSampleOriginX->Location = System::Drawing::Point(177, 40);
			this->txtSampleOriginX->Name = L"txtSampleOriginX";
			this->txtSampleOriginX->Size = System::Drawing::Size(101, 19);
			this->txtSampleOriginX->TabIndex = 1;
			// 
			// txtRectTop
			// 
			this->txtRectTop->Location = System::Drawing::Point(55, 65);
			this->txtRectTop->Name = L"txtRectTop";
			this->txtRectTop->Size = System::Drawing::Size(65, 19);
			this->txtRectTop->TabIndex = 1;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(14, 120);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(40, 12);
			this->label11->TabIndex = 0;
			this->label11->Text = L"Height:";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->txtWindowTitle);
			this->groupBox2->Controls->Add(this->label14);
			this->groupBox2->Controls->Add(this->txtWindowClass);
			this->groupBox2->Controls->Add(this->label1);
			this->groupBox2->Controls->Add(this->txControlID);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Controls->Add(this->txtHandle);
			this->groupBox2->Controls->Add(this->label4);
			this->groupBox2->Location = System::Drawing::Point(12, 12);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(446, 96);
			this->groupBox2->TabIndex = 3;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Target Window Information";
			// 
			// txtWindowTitle
			// 
			this->txtWindowTitle->Enabled = false;
			this->txtWindowTitle->Location = System::Drawing::Point(88, 18);
			this->txtWindowTitle->Name = L"txtWindowTitle";
			this->txtWindowTitle->Size = System::Drawing::Size(348, 19);
			this->txtWindowTitle->TabIndex = 1;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(6, 21);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(72, 12);
			this->label14->TabIndex = 0;
			this->label14->Text = L"Window Title:";
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &frmMain::timer1_Tick);
			// 
			// frmMain
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(470, 326);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Name = L"frmMain";
			this->Text = L"genkeytool";
			this->TopMost = true;
			this->Load += gcnew System::EventHandler(this, &frmMain::frmMain_Load);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &frmMain::frmMain_KeyUp);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &frmMain::frmMain_KeyDown);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void frmMain_Load(System::Object^  sender, System::EventArgs^  e) {
				 this->txtRectWidth->Text = "50";
				 this->txtRectHeight->Text = "50";
				 this->txtSampleOriginX->Text = "0";
				 this->txtSampleOriginY->Text = "0";
				 this->txtSampleLenght->Text = "20";
				 this->m_hWnd = 0;
				 this->m_pWindowInfo = 0;
			 }

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				 POINT pt;

				 GetCursorPos(&pt);

				 WINDOWINFO wi;

				 m_hWnd = WindowFromPoint(pt);

				 GetWindowInfo(m_hWnd, &wi);
				 m_pWindowInfo = &wi;

				 RECT rt;

				 GetWindowRect(m_hWnd, &rt);

				 if( m_hWnd )
				 {
					 POINT pt;

					 GetCursorPos(&pt);

					 RECT rt;

					 GetWindowRect(m_hWnd, &rt);

					 pt.x -= rt.left ;
					 pt.y -= rt.top ;

					 HDC hdc;
					 hdc = GetDC(m_hWnd);

					 HDC hdcTarget = (HDC) GetDC((HWND)this->pictureBox1->Handle.ToInt32()) ;

					 COLORREF color;

					 int i, j;
					 int h, w;

					 h = Convert::ToInt32(txtRectHeight->Text);
					 w = Convert::ToInt32(txtRectWidth->Text);

					 for(j = 0; j < h ; j ++)
					 {
						 for( i = 0; i < w; i++)
						 {
							 color = GetPixel(hdc, pt.x + i,pt.y + j);
							 SetPixel(hdcTarget,  i,  j, color);
						 }
					 }

					 ReleaseDC(m_hWnd, hdc);
					 ReleaseDC((HWND)this->pictureBox1->Handle.ToInt32(), hdcTarget);

					 this->txtHandle->Text = String::Format("{0}", (unsigned int)m_hWnd);
					 this->txtRectTop->Text = String::Format("{0}",pt.x) ;
					 this->txtRectLeft->Text = String::Format("{0}",pt.y) ;
					Capture3D(m_hWnd, "szFileName.bmp");
				 }

			 }
	private: System::Void frmMain_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
					 if( m_hWnd )
					Capture3D(m_hWnd, "szFileName.bmp");
			 }
private: System::Void frmMain_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
					 if( m_hWnd )
					Capture3D(m_hWnd, "szFileName.bmp");
		 }
};
}