#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include <windows.h>
#include "StringConvertor.h"
#include "BitmapEx.h"
#include "Murmurhash.h"
#include <string>
#include <fstream>
#include "Json/Json.h"
#include "MurmurHash.h"
#include "MouseHook.h"
extern "C" {
	void Capture3D(HWND hWnd, LPCSTR szFileName);
};

using namespace StringUtilities;

namespace genkeytool {

#define PIECE_NUM (14)


struct PieceHashValue
{
	char cPiece;
	DWORD dwPieceHashValue;
	int  nSquareX;
	int  nSquareY;
};

extern 
PieceHashValue _PieceHashValues[PIECE_NUM];

	BOOL CALLBACK frmGetKey_EnumChildProc(HWND hwnd,  LPARAM lParam)
	{

	}

	/// <summary>
	/// Summary for frmGetKey
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmGetKey : public System::Windows::Forms::Form
	{
		int TURN1_X;
		int TURN1_Y;
		int TURN2_X;
		int TURN2_Y;

		long m_ptBoardOriginX;
		long m_ptBoardOriginY;
		long m_sizeSquareCX;
		long m_sizeSquareCY;
		long m_ptSampleOriginX;
		long m_ptSampleOriginY;
		long m_nSampleLen;


		CBitmapEx * m_pBitmap;
	private: System::Windows::Forms::Label^  lblDrag;
	private: System::Windows::Forms::TextBox^  txtWindowInfo;
	private: System::Windows::Forms::FontDialog^  dlgFont;
	private: System::Windows::Forms::Button^  btnFont;


	public:
		frmGetKey(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pBitmap = new CBitmapEx();
			CMouseHook::UWM_DRAGEEND = ::RegisterWindowMessage(UWM_DRAGEND_MSG);

			this->dlgFont->Font  = gcnew Drawing::Font("Consolas", 8);

			LoadSettingFile("hv_qq.ini");

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmGetKey()
		{
			if (components)
			{
				delete components;
			}
			if( m_pBitmap)
				delete m_pBitmap;
		}
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  btnSaveSetting;

	private: System::Windows::Forms::TextBox^  txtSquareHeight;
	private: System::Windows::Forms::TextBox^  txtSquareWidth;
	private: System::Windows::Forms::TextBox^  txtBoardOriginY;
	private: System::Windows::Forms::TextBox^  txtBoardOriginX;









	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::PictureBox^  picCaptureBmp;

	private: System::Windows::Forms::OpenFileDialog^  dlgOpenFile;

	private: System::Windows::Forms::Button^  btnSelectFile;
	private: System::Windows::Forms::Button^  btnLoadSetting;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  lblX;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  txtSampleLen;
	private: System::Windows::Forms::TextBox^  txtSampleOriginY;
	private: System::Windows::Forms::TextBox^  txtSampleOriginX;



	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::TextBox^  txtTURN2_Y;
	private: System::Windows::Forms::TextBox^  txtTURN2_X;
	private: System::Windows::Forms::TextBox^  txtTURN1_Y;
	private: System::Windows::Forms::TextBox^  txtTURN1_X;
	private: System::Windows::Forms::SaveFileDialog^  dlgSaveFile;
	private: System::Windows::Forms::Button^  btnShowRect;

	protected: 

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
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->btnFont = (gcnew System::Windows::Forms::Button());
			this->btnShowRect = (gcnew System::Windows::Forms::Button());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->txtWindowInfo = (gcnew System::Windows::Forms::TextBox());
			this->lblDrag = (gcnew System::Windows::Forms::Label());
			this->btnLoadSetting = (gcnew System::Windows::Forms::Button());
			this->btnSaveSetting = (gcnew System::Windows::Forms::Button());
			this->btnSelectFile = (gcnew System::Windows::Forms::Button());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->lblX = (gcnew System::Windows::Forms::Label());
			this->txtTURN2_Y = (gcnew System::Windows::Forms::TextBox());
			this->txtTURN2_X = (gcnew System::Windows::Forms::TextBox());
			this->txtTURN1_Y = (gcnew System::Windows::Forms::TextBox());
			this->txtTURN1_X = (gcnew System::Windows::Forms::TextBox());
			this->txtSampleLen = (gcnew System::Windows::Forms::TextBox());
			this->txtSampleOriginY = (gcnew System::Windows::Forms::TextBox());
			this->txtSampleOriginX = (gcnew System::Windows::Forms::TextBox());
			this->txtSquareHeight = (gcnew System::Windows::Forms::TextBox());
			this->txtSquareWidth = (gcnew System::Windows::Forms::TextBox());
			this->txtBoardOriginY = (gcnew System::Windows::Forms::TextBox());
			this->txtBoardOriginX = (gcnew System::Windows::Forms::TextBox());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->picCaptureBmp = (gcnew System::Windows::Forms::PictureBox());
			this->dlgOpenFile = (gcnew System::Windows::Forms::OpenFileDialog());
			this->dlgSaveFile = (gcnew System::Windows::Forms::SaveFileDialog());
			this->dlgFont = (gcnew System::Windows::Forms::FontDialog());
			this->panel1->SuspendLayout();
			this->panel3->SuspendLayout();
			this->panel2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picCaptureBmp))->BeginInit();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->btnFont);
			this->panel1->Controls->Add(this->btnShowRect);
			this->panel1->Controls->Add(this->panel3);
			this->panel1->Controls->Add(this->label10);
			this->panel1->Controls->Add(this->label9);
			this->panel1->Controls->Add(this->label8);
			this->panel1->Controls->Add(this->label7);
			this->panel1->Controls->Add(this->label6);
			this->panel1->Controls->Add(this->label5);
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->lblX);
			this->panel1->Controls->Add(this->txtTURN2_Y);
			this->panel1->Controls->Add(this->txtTURN2_X);
			this->panel1->Controls->Add(this->txtTURN1_Y);
			this->panel1->Controls->Add(this->txtTURN1_X);
			this->panel1->Controls->Add(this->txtSampleLen);
			this->panel1->Controls->Add(this->txtSampleOriginY);
			this->panel1->Controls->Add(this->txtSampleOriginX);
			this->panel1->Controls->Add(this->txtSquareHeight);
			this->panel1->Controls->Add(this->txtSquareWidth);
			this->panel1->Controls->Add(this->txtBoardOriginY);
			this->panel1->Controls->Add(this->txtBoardOriginX);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Left;
			this->panel1->Location = System::Drawing::Point(0, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(181, 543);
			this->panel1->TabIndex = 1;
			// 
			// btnFont
			// 
			this->btnFont->Location = System::Drawing::Point(13, 307);
			this->btnFont->Name = L"btnFont";
			this->btnFont->Size = System::Drawing::Size(50, 21);
			this->btnFont->TabIndex = 5;
			this->btnFont->Text = L"Font...";
			this->btnFont->UseVisualStyleBackColor = true;
			this->btnFont->Click += gcnew System::EventHandler(this, &frmGetKey::btnFont_Click);
			// 
			// btnShowRect
			// 
			this->btnShowRect->Location = System::Drawing::Point(75, 306);
			this->btnShowRect->Name = L"btnShowRect";
			this->btnShowRect->Size = System::Drawing::Size(95, 23);
			this->btnShowRect->TabIndex = 4;
			this->btnShowRect->Text = L"Show Grid";
			this->btnShowRect->UseVisualStyleBackColor = true;
			this->btnShowRect->Click += gcnew System::EventHandler(this, &frmGetKey::btnShowRect_Click);
			// 
			// panel3
			// 
			this->panel3->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panel3->Controls->Add(this->txtWindowInfo);
			this->panel3->Controls->Add(this->lblDrag);
			this->panel3->Controls->Add(this->btnLoadSetting);
			this->panel3->Controls->Add(this->btnSaveSetting);
			this->panel3->Controls->Add(this->btnSelectFile);
			this->panel3->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->panel3->Location = System::Drawing::Point(0, 335);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(181, 208);
			this->panel3->TabIndex = 3;
			// 
			// txtWindowInfo
			// 
			this->txtWindowInfo->Location = System::Drawing::Point(18, 108);
			this->txtWindowInfo->Multiline = true;
			this->txtWindowInfo->Name = L"txtWindowInfo";
			this->txtWindowInfo->Size = System::Drawing::Size(144, 52);
			this->txtWindowInfo->TabIndex = 3;
			// 
			// lblDrag
			// 
			this->lblDrag->AutoSize = true;
			this->lblDrag->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->lblDrag->Location = System::Drawing::Point(11, 78);
			this->lblDrag->Name = L"lblDrag";
			this->lblDrag->Size = System::Drawing::Size(26, 16);
			this->lblDrag->TabIndex = 2;
			this->lblDrag->Text = L"[+]";
			this->lblDrag->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &frmGetKey::lblDrag_MouseDown);
			// 
			// btnLoadSetting
			// 
			this->btnLoadSetting->Location = System::Drawing::Point(14, 14);
			this->btnLoadSetting->Name = L"btnLoadSetting";
			this->btnLoadSetting->Size = System::Drawing::Size(148, 24);
			this->btnLoadSetting->TabIndex = 1;
			this->btnLoadSetting->Text = L"Load Setting...";
			this->btnLoadSetting->UseVisualStyleBackColor = true;
			this->btnLoadSetting->Click += gcnew System::EventHandler(this, &frmGetKey::btnLoadSetting_Click);
			// 
			// btnSaveSetting
			// 
			this->btnSaveSetting->Location = System::Drawing::Point(14, 44);
			this->btnSaveSetting->Name = L"btnSaveSetting";
			this->btnSaveSetting->Size = System::Drawing::Size(148, 24);
			this->btnSaveSetting->TabIndex = 1;
			this->btnSaveSetting->Text = L"Save Setting...";
			this->btnSaveSetting->UseVisualStyleBackColor = true;
			this->btnSaveSetting->Click += gcnew System::EventHandler(this, &frmGetKey::btnSaveSetting_Click);
			// 
			// btnSelectFile
			// 
			this->btnSelectFile->Location = System::Drawing::Point(40, 74);
			this->btnSelectFile->Name = L"btnSelectFile";
			this->btnSelectFile->Size = System::Drawing::Size(122, 24);
			this->btnSelectFile->TabIndex = 1;
			this->btnSelectFile->Text = L"Select bitmap...";
			this->btnSelectFile->UseVisualStyleBackColor = true;
			this->btnSelectFile->Click += gcnew System::EventHandler(this, &frmGetKey::btnSelectFile_Click);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(39, 280);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(53, 12);
			this->label10->TabIndex = 2;
			this->label10->Text = L"TURN2_Y";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(39, 255);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(53, 12);
			this->label9->TabIndex = 2;
			this->label9->Text = L"TURN2_X";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(39, 230);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(53, 12);
			this->label8->TabIndex = 2;
			this->label8->Text = L"TURN1_Y";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(39, 205);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(53, 12);
			this->label7->TabIndex = 2;
			this->label7->Text = L"TURN1_X";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(32, 180);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(60, 12);
			this->label6->TabIndex = 2;
			this->label6->Text = L"SampleLen";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(13, 155);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(79, 12);
			this->label5->TabIndex = 2;
			this->label5->Text = L"SampleOriginY";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(13, 130);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(79, 12);
			this->label4->TabIndex = 2;
			this->label4->Text = L"SampleOriginX";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(19, 100);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(73, 12);
			this->label3->TabIndex = 2;
			this->label3->Text = L"SquareHeight";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(24, 71);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(68, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"SquareWidth";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(20, 44);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(72, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"BoardOriginY";
			// 
			// lblX
			// 
			this->lblX->AutoSize = true;
			this->lblX->Location = System::Drawing::Point(20, 18);
			this->lblX->Name = L"lblX";
			this->lblX->Size = System::Drawing::Size(72, 12);
			this->lblX->TabIndex = 2;
			this->lblX->Text = L"BoardOriginX";
			// 
			// txtTURN2_Y
			// 
			this->txtTURN2_Y->Location = System::Drawing::Point(101, 277);
			this->txtTURN2_Y->Name = L"txtTURN2_Y";
			this->txtTURN2_Y->Size = System::Drawing::Size(74, 19);
			this->txtTURN2_Y->TabIndex = 0;
			// 
			// txtTURN2_X
			// 
			this->txtTURN2_X->Location = System::Drawing::Point(101, 252);
			this->txtTURN2_X->Name = L"txtTURN2_X";
			this->txtTURN2_X->Size = System::Drawing::Size(74, 19);
			this->txtTURN2_X->TabIndex = 0;
			// 
			// txtTURN1_Y
			// 
			this->txtTURN1_Y->Location = System::Drawing::Point(101, 227);
			this->txtTURN1_Y->Name = L"txtTURN1_Y";
			this->txtTURN1_Y->Size = System::Drawing::Size(74, 19);
			this->txtTURN1_Y->TabIndex = 0;
			// 
			// txtTURN1_X
			// 
			this->txtTURN1_X->Location = System::Drawing::Point(101, 202);
			this->txtTURN1_X->Name = L"txtTURN1_X";
			this->txtTURN1_X->Size = System::Drawing::Size(74, 19);
			this->txtTURN1_X->TabIndex = 0;
			// 
			// txtSampleLen
			// 
			this->txtSampleLen->Location = System::Drawing::Point(101, 177);
			this->txtSampleLen->Name = L"txtSampleLen";
			this->txtSampleLen->Size = System::Drawing::Size(74, 19);
			this->txtSampleLen->TabIndex = 0;
			// 
			// txtSampleOriginY
			// 
			this->txtSampleOriginY->Location = System::Drawing::Point(101, 152);
			this->txtSampleOriginY->Name = L"txtSampleOriginY";
			this->txtSampleOriginY->Size = System::Drawing::Size(74, 19);
			this->txtSampleOriginY->TabIndex = 0;
			// 
			// txtSampleOriginX
			// 
			this->txtSampleOriginX->Location = System::Drawing::Point(101, 127);
			this->txtSampleOriginX->Name = L"txtSampleOriginX";
			this->txtSampleOriginX->Size = System::Drawing::Size(74, 19);
			this->txtSampleOriginX->TabIndex = 0;
			// 
			// txtSquareHeight
			// 
			this->txtSquareHeight->Location = System::Drawing::Point(101, 97);
			this->txtSquareHeight->Name = L"txtSquareHeight";
			this->txtSquareHeight->Size = System::Drawing::Size(74, 19);
			this->txtSquareHeight->TabIndex = 0;
			// 
			// txtSquareWidth
			// 
			this->txtSquareWidth->Location = System::Drawing::Point(101, 71);
			this->txtSquareWidth->Name = L"txtSquareWidth";
			this->txtSquareWidth->Size = System::Drawing::Size(74, 19);
			this->txtSquareWidth->TabIndex = 0;
			// 
			// txtBoardOriginY
			// 
			this->txtBoardOriginY->Location = System::Drawing::Point(101, 44);
			this->txtBoardOriginY->Name = L"txtBoardOriginY";
			this->txtBoardOriginY->Size = System::Drawing::Size(74, 19);
			this->txtBoardOriginY->TabIndex = 0;
			this->txtBoardOriginY->TextChanged += gcnew System::EventHandler(this, &frmGetKey::textBox2_TextChanged);
			// 
			// txtBoardOriginX
			// 
			this->txtBoardOriginX->Location = System::Drawing::Point(101, 18);
			this->txtBoardOriginX->Name = L"txtBoardOriginX";
			this->txtBoardOriginX->Size = System::Drawing::Size(74, 19);
			this->txtBoardOriginX->TabIndex = 0;
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->picCaptureBmp);
			this->panel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel2->Location = System::Drawing::Point(181, 0);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(461, 543);
			this->panel2->TabIndex = 2;
			// 
			// picCaptureBmp
			// 
			this->picCaptureBmp->Dock = System::Windows::Forms::DockStyle::Fill;
			this->picCaptureBmp->Location = System::Drawing::Point(0, 0);
			this->picCaptureBmp->Name = L"picCaptureBmp";
			this->picCaptureBmp->Size = System::Drawing::Size(461, 543);
			this->picCaptureBmp->TabIndex = 0;
			this->picCaptureBmp->TabStop = false;
			// 
			// dlgFont
			// 
			this->dlgFont->ShowColor = true;
			// 
			// frmGetKey
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(642, 543);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel1);
			this->Name = L"frmGetKey";
			this->Text = L"frmGetKey";
			this->TopMost = true;
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panel3->ResumeLayout(false);
			this->panel3->PerformLayout();
			this->panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->picCaptureBmp))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void btnSelectFile_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(this->dlgOpenFile->ShowDialog()== ::DialogResult::OK  )
				 {
					 StringUtilities::StringConvertor filename(dlgOpenFile->FileName );

					 m_pBitmap->Load( filename.NativeCharPtr);

					 UpdatePictureBox();
				 }

			 }
	 private: System::Void UpdatePictureBox()
			  {
					 HBITMAP bitmap = NULL;

					 m_pBitmap->Save(bitmap);

					 this->picCaptureBmp->Image = Image::FromHbitmap(System::IntPtr((int)bitmap));

					 DeleteObject(bitmap);
					 this->picCaptureBmp->Refresh();

			  }

	private: bool LoadSettingFile(String ^filename)
			 {
				 try
				 {
					 StringConvertor sc(filename);

					 Json::Value  vRoot;
					 std::ifstream fs(sc.NativeCharPtr);
					 if(fs)
					 {			
						 Json::Reader reader;
						 reader.parse(fs, vRoot, false);

						 m_ptBoardOriginX = vRoot["BoardOriginX"].asInt();
						 m_ptBoardOriginY = vRoot["BoardOriginY"].asInt();
						 m_sizeSquareCX = vRoot["SquareWidth"].asInt();
						 m_sizeSquareCY = vRoot["SquareHeight"].asInt();
						 m_ptSampleOriginX = vRoot["SampleOriginX"].asInt();
						 m_ptSampleOriginY = vRoot["SampleOriginY"].asInt();
						 m_nSampleLen = vRoot["SampleLen"].asInt();

						 TURN1_X			=vRoot["TURN1_X"].asInt();
						 TURN1_Y			=vRoot["TURN1_Y"].asInt();
						 TURN2_X			=vRoot["TURN2_X"].asInt();
						 TURN2_Y			=vRoot["TURN2_Y"].asInt();

						 this->txtBoardOriginX->Text = (vRoot["BoardOriginX"].asInt().ToString());
						 this->txtBoardOriginY->Text = (vRoot["BoardOriginY"].asInt().ToString());
						 this->txtSquareWidth->Text  = (vRoot["SquareWidth"].asInt().ToString());
						 this->txtSquareHeight->Text = (vRoot["SquareHeight"].asInt().ToString());
						 this->txtSampleOriginX->Text =(vRoot["SampleOriginX"].asInt().ToString());
						 this->txtSampleOriginY->Text = (vRoot["SampleOriginY"].asInt().ToString());
						 this->txtSampleLen->Text = (vRoot["SampleLen"].asInt().ToString());

						 txtTURN1_X->Text			=(vRoot["TURN1_X"].asInt().ToString());
						 txtTURN1_Y->Text 			=(vRoot["TURN1_Y"].asInt().ToString());
						 txtTURN2_X->Text 			=(vRoot["TURN2_X"].asInt().ToString());
						 txtTURN2_Y->Text 			=(vRoot["TURN2_Y"].asInt().ToString());

					 }
					 fs.close();
					 return true;
				 }
				 catch(...)
				 {
					 MessageBox::Show( "Could not found setting file:" + filename, "Load setting file failed",
						 MessageBoxButtons::OK, MessageBoxIcon::Error  );

					 return false;
				 }

			 }
	private: System::Void btnLoadSetting_Click(System::Object^  sender, System::EventArgs^  e) {

				 if(this->dlgOpenFile->ShowDialog() == ::DialogResult::OK )
				 {
					 LoadSettingFile( dlgOpenFile->FileName );
				 }
			 }
	 private: System::Void ShowGrid()
			  {
				 m_ptBoardOriginX = int::Parse(this->txtBoardOriginX->Text);
				 m_ptBoardOriginY =int::Parse(this->txtBoardOriginY->Text);
				 m_sizeSquareCX = int::Parse(this->txtSquareWidth ->Text);
				 m_sizeSquareCY = int::Parse(this->txtSquareHeight->Text);
				 m_ptSampleOriginX =int::Parse(this->txtSampleOriginX->Text);
				 m_ptSampleOriginY =int::Parse(this->txtSampleOriginY->Text);
				 m_nSampleLen = int::Parse(this->txtSampleLen->Text);

				 TURN1_X			=int::Parse(this->txtTURN1_X->Text );
				 TURN1_Y			=int::Parse(this->txtTURN1_Y->Text );
				 TURN2_X			=int::Parse(this->txtTURN2_X->Text );
				 TURN2_Y			=int::Parse(this->txtTURN2_Y->Text );


				 if( m_pBitmap && m_pBitmap->IsValid())
				 {
					 HBITMAP bitmap = NULL;

					 m_pBitmap->Save(bitmap);


					 this->picCaptureBmp->Image = Image::FromHbitmap(System::IntPtr((int)bitmap));

					 Graphics^ g = Graphics::FromImage(this->picCaptureBmp->Image);

					 Drawing::Pen ^ penRed = gcnew Pen(Color::Red);
					 Drawing::Pen ^ penGreen = gcnew Pen(Color::Green);

					 g->DrawRectangle(penGreen, this->TURN1_X, this->TURN1_Y, this->m_nSampleLen,this->m_nSampleLen ); 
					 g->DrawRectangle(penGreen, this->TURN2_X, this->TURN2_Y, this->m_nSampleLen,this->m_nSampleLen ); 

					 SolidBrush^ brush = gcnew SolidBrush( this->dlgFont->Color );
					 
					 g->DrawString(this->GetHashValue(TURN1_X,TURN1_Y, this->m_nSampleLen ).ToString(), dlgFont->Font, brush, (float)TURN1_X,(float)TURN1_Y );
					 g->DrawString(this->GetHashValue(TURN2_X,TURN2_Y, this->m_nSampleLen ).ToString(), dlgFont->Font, brush, (float)TURN2_X,(float)TURN2_Y);

					 for( int qy = 0; qy < 10; qy++)
					 {
						 for( int qx = 0; qx < 9; qx ++)
						 {
							 POINT pt;
							 pt = this->GetSquareOrigin(qx,qy);
							 g->DrawRectangle(penRed, pt.x , pt.y, this->m_sizeSquareCX,this->m_sizeSquareCY ); 
							 g->DrawRectangle(penGreen, pt.x + this->m_ptSampleOriginX , pt.y + this->m_ptSampleOriginY, 
								 this->m_nSampleLen,this->m_nSampleLen );
							 static bool bFlag = false;
							 bFlag = !bFlag;
							 g->DrawString(this->GetHashValue(qx,qy).ToString(), dlgFont->Font, brush, (float)pt.x,(float)pt.y + (bFlag?dlgFont->Font->Size:0) );
						 }
					 }

					 this->picCaptureBmp->Refresh();
				 }
			  }
	private: System::Void btnShowRect_Click(System::Object^  sender, System::EventArgs^  e) {
				 if( !this->m_pBitmap->IsValid ())
					 MessageBox::Show("You have'nt select a bitmap or capture a picture yet");
				 else
					 ShowGrid();
			 }



			 DWORD GetHashValue( int x, int y, int len )
			 {
				 if( !this->m_pBitmap->IsValid() )
					 return 0;

				 DWORD *pSampleData = new DWORD[len];

				 DWORD k;

				 for( int i = 0; i < len; i ++)
				 {
					 pSampleData[i] = m_pBitmap->GetPixel( x++, y++);
				 }

				 k = base::MurmurHash2(pSampleData, len * sizeof(DWORD));

				 delete pSampleData;

				 return k;
			 }


			 DWORD GetHashValue( int sqx, int sqy )
			 {
				 POINT ptSquare;

				 ptSquare = GetSquareOrigin( sqx, sqy );

				 int ox = this->m_ptSampleOriginX  ;
				 int oy = this->m_ptSampleOriginY ;
				 int len = m_nSampleLen;

				 return GetHashValue( ptSquare.x + ox, ptSquare.y + oy, len );
			 }

			 POINT GetSquareOrigin(int squarex, int squarey)
			 {
				 POINT pt;
				 int x = m_ptBoardOriginX;
				 int y = m_ptBoardOriginY;
				 int qw = this->m_sizeSquareCX;
				 int qh = this->m_sizeSquareCY;

				 pt.x = x + qw*squarex;
				 pt.y = y + qh*squarey;
				 return pt;
			 }

	private: System::Void btnSaveSetting_Click(System::Object^  sender, System::EventArgs^  e) {

				 if(this->dlgSaveFile->ShowDialog() == ::DialogResult::OK )
				 {
					 StringConvertor sc(dlgSaveFile->FileName);


					 Json::Value  vRoot;
					 std::ofstream fs(sc.NativeCharPtr);
					 if(fs)
					 {			
						 Json::StyledStreamWriter writer;

						 vRoot["BoardOriginX"]	= m_ptBoardOriginX;
						 vRoot["BoardOriginY"]	= m_ptBoardOriginY ;
						 vRoot["SquareWidth"]	= m_sizeSquareCX;
						 vRoot["SquareHeight"]	= m_sizeSquareCY;
						 vRoot["SampleOriginX"]	= m_ptSampleOriginX ;
						 vRoot["SampleOriginY"]	= m_ptSampleOriginY;
						 vRoot["SampleLen"]		= m_nSampleLen;


						 vRoot["TURN1_X"]	= TURN1_X;
						 vRoot["TURN1_Y"]	= TURN1_Y;
						 vRoot["TURN2_X"]	= TURN2_X;
						 vRoot["TURN2_Y"]	= TURN2_Y;

						 vRoot["TURN_WHITE_KEY"]	= (unsigned int)GetHashValue(TURN1_X, TURN1_Y, m_nSampleLen);
						 vRoot["TURN_BLACK_KEY"]	= (unsigned int)GetHashValue(TURN2_X, TURN2_Y, m_nSampleLen);



						 Json::Value vPieceHashTable;


						 int size = PIECE_NUM ;
						 DWORD k;

						 for ( int index =0; index < size; ++index )
						 {
							 k = this->GetHashValue( _PieceHashValues[index].nSquareX,_PieceHashValues[index].nSquareY);
							 vPieceHashTable[index]["PieceKey"] = _PieceHashValues[index].cPiece ;
							 vPieceHashTable[index]["PieceHash"] = (unsigned int)k;
						 }

						 vRoot["PieceHashTable"] = vPieceHashTable;

						 writer.write(fs, vRoot);
					 }
					 fs.close();
				 }
			 }
	private: System::Void lblDrag_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 this->Cursor = Cursors::Cross ;
				 CMouseHook::StartHook((HWND)this->Handle.ToInt32());	
				 lblDrag->Text = "[ ]";
			 }
protected:
	void GetBmpFromHwndGDI(HWND hwnd)
	{

		 HDC hdc = GetWindowDC(hwnd);
		 HBITMAP hbmp = (HBITMAP)GetCurrentObject(hdc, OBJ_BITMAP);
		 this->m_pBitmap->Load(hbmp);

		 this->UpdatePictureBox();

		 DeleteObject(hbmp);
		 ReleaseDC(hwnd,hdc);

	}
	void GetBmpFromHwndD3D(HWND hwnd)
	{
		Capture3D(hwnd,"capture_tmp.bmp");

		this->m_pBitmap->Load("capture_tmp.bmp");

		this->UpdatePictureBox();

	}

	std::string * m_pHashString;

	void AddWindowKey(HWND hwnd)
	{

		m_pHashString->append("");
	}

	TCHAR * GetFileName(TCHAR * filepath)
	{
		const TCHAR * delimiters = "/\\";

		TCHAR * p;
		TCHAR * plast;
		p = _tcstok (filepath,delimiters); 
		while(p!=NULL) 
		{
			plast = p;
			p = _tcstok(NULL,delimiters); 
		}
		return plast;
	}

	friend BOOL CALLBACK frmGetKey_EnumChildProc(HWND hwnd,  LPARAM lParam);


	unsigned int GetWindowKey(HWND hwnd)
	{
		unsigned int key=0;

		TCHAR szBuf[1024];
		TCHAR szStr[256];

		// Get module file name
		GetWindowModuleFileName(hwnd, szStr, sizeof(szStr));

		_stprintf(szBuf,"%s,0x%x",GetFileName(szStr), GetDlgCtrlID(hwnd));

		EnumChildWindows(hwnd, frmGetKey_EnumChildProc, (LPARAM) this);

		key = base::MurmurHash2(szBuf, strlen(szBuf));

		return key;
	}

	void OnDragEnd(HWND hwnd )
	{
		this->Cursor = Cursors::Default ;
		CMouseHook::StopHook();
		lblDrag->Text = "[+]";		

		//GetBmpFromHwndGDI(hwnd);
		GetBmpFromHwndD3D(hwnd);

		this->txtWindowInfo->Text = String::Format("WindowKey=:{0:X}", GetWindowKey(hwnd));
	}

	virtual void WndProc(Message% m) override
	{
		if( m.Msg == CMouseHook::UWM_DRAGEEND )
		{
			HWND hwnd = (HWND)m.WParam.ToInt32();
			OnDragEnd(hwnd);
		}
		Form::WndProc(m);
	}

private: System::Void btnFont_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(dlgFont->ShowDialog() == ::DialogResult::OK )
			 {
				 ShowGrid();
			 }
		 }
};
}
