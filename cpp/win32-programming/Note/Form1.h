#pragma once
#include < windows.h >

namespace Note {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
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
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  textpane;
	protected: 



	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  ggToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  FileNew;

	private: System::Windows::Forms::ToolStripMenuItem^  FileOpen;
	private: System::Windows::Forms::ToolStripMenuItem^  FileSave;


	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  FileSaveAsAscii;
	private: System::Windows::Forms::ToolStripMenuItem^  FileSaveAsUnicode;
	private: System::Windows::Forms::ToolStripMenuItem^  Exit;




	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		System::String ^filename;
		System::Text::Encoding ^encoding;
		bool docSaved;           // check if current document is saved

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textpane = (gcnew System::Windows::Forms::TextBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->ggToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->FileNew = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->FileOpen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->FileSave = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->FileSaveAsAscii = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->FileSaveAsUnicode = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Exit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// textpane
			// 
			this->textpane->Location = System::Drawing::Point(12, 27);
			this->textpane->Multiline = true;
			this->textpane->Name = L"textpane";
			this->textpane->Size = System::Drawing::Size(331, 237);
			this->textpane->TabIndex = 0;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->ggToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(355, 24);
			this->menuStrip1->TabIndex = 3;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// ggToolStripMenuItem
			// 
			this->ggToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->FileNew, 
				this->FileOpen, this->FileSave, this->exitToolStripMenuItem, this->Exit});
			this->ggToolStripMenuItem->Name = L"ggToolStripMenuItem";
			this->ggToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->ggToolStripMenuItem->Text = L"File";
			// 
			// FileNew
			// 
			this->FileNew->Name = L"FileNew";
			this->FileNew->Size = System::Drawing::Size(152, 22);
			this->FileNew->Text = L"New";
			this->FileNew->Click += gcnew System::EventHandler(this, &Form1::FileNew_Click);
			// 
			// FileOpen
			// 
			this->FileOpen->Name = L"FileOpen";
			this->FileOpen->Size = System::Drawing::Size(152, 22);
			this->FileOpen->Text = L"Open";
			this->FileOpen->Click += gcnew System::EventHandler(this, &Form1::FileOpen_Click);
			// 
			// FileSave
			// 
			this->FileSave->Name = L"FileSave";
			this->FileSave->Size = System::Drawing::Size(152, 22);
			this->FileSave->Text = L"Save";
			this->FileSave->Click += gcnew System::EventHandler(this, &Form1::FileSave_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->FileSaveAsAscii, 
				this->FileSaveAsUnicode});
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->exitToolStripMenuItem->Text = L"Save As";
			// 
			// FileSaveAsAscii
			// 
			this->FileSaveAsAscii->Name = L"FileSaveAsAscii";
			this->FileSaveAsAscii->Size = System::Drawing::Size(118, 22);
			this->FileSaveAsAscii->Text = L"ASCII";
			this->FileSaveAsAscii->Click += gcnew System::EventHandler(this, &Form1::FileSaveAsAscii_Click);
			// 
			// FileSaveAsUnicode
			// 
			this->FileSaveAsUnicode->Name = L"FileSaveAsUnicode";
			this->FileSaveAsUnicode->Size = System::Drawing::Size(118, 22);
			this->FileSaveAsUnicode->Text = L"Unicode";
			this->FileSaveAsUnicode->Click += gcnew System::EventHandler(this, &Form1::FileSaveAsUnicode_Click);
			// 
			// Exit
			// 
			this->Exit->Name = L"Exit";
			this->Exit->Size = System::Drawing::Size(152, 22);
			this->Exit->Text = L"Exit";
			this->Exit->Click += gcnew System::EventHandler(this, &Form1::Exit_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(355, 276);
			this->Controls->Add(this->textpane);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Notepad";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
	  docSaved = true;
	}

    private: System::Void FileOpen_Click(System::Object^  sender, System::EventArgs^  e) {

     // Displays an OpenFileDialog so the user can select a File.
      OpenFileDialog ^ openFileDialog1 = gcnew OpenFileDialog();
      openFileDialog1->Filter = "Text Files|*.txt";
      openFileDialog1->Title = "Select a Text File";

      // Show the Dialog.
      // If the user clicked OK in the dialog and
      // a .Text file was selected, open it.
      if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {

		  // first get the encoding of the file.
		  System::IO::StreamReader ^ SR;

		  try {
		    filename = openFileDialog1->FileName;
  	        // open the file and display it.
		    SR = gcnew System::IO::StreamReader(filename);
		    this->textpane->Text=SR->ReadToEnd();  // read the file and display in textbox

		    encoding = System::Text::Encoding::ASCII; // set default encoding to ASCII
		    docSaved = false;
		  }
		  catch (Exception^ e) { 
			  MessageBox::Show("Unexpected Error : " + e, L"Error");
		  }
		  finally {
		  }
      }
	}

    private: System::Void Exit_Click(System::Object^  sender, System::EventArgs^  e) {
		
		if(!docSaved) {
           MessageBoxButtons buttons = MessageBoxButtons::YesNo;

		   // Displays the MessageBox.
		   System::Windows::Forms::DialogResult result;
		   result = MessageBox::Show(L"Do you want to exit without saving the document ?",  
			                         L"Confirm", buttons);

           if ( result == System::Windows::Forms::DialogResult::Yes ) {
            // Closes the parent form.
            this->Close();
			Application::Exit();   // exit from application
           }
		}
		else
			Application::Exit();
	}

	private: System::Void FileSaveAsAscii_Click(System::Object^  sender, System::EventArgs^  e) {

      // Displays an OpenFileDialog so the user can select a File.
      SaveFileDialog ^ saveFileDialog1 = gcnew SaveFileDialog();
      saveFileDialog1->Filter = "Text Files|*.txt";
      saveFileDialog1->Title = "Save File";
	  saveFileDialog1->InitialDirectory = "C:\\";

      // Show the Dialog.
      // If the user clicked OK in the dialog and
      // the Text file is saved in ascii format.
      if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
      {
    	  System::IO::StreamWriter^ swFromFile;

		  try {
    		swFromFile = gcnew System::IO::StreamWriter(saveFileDialog1->FileName,
				               false, System::Text::Encoding::Unicode);
			swFromFile->Write( this->textpane->Text);
            swFromFile->Flush();
			docSaved = true;
		  }
		  catch(System::Exception ^ ) {
			MessageBox::Show(L"Unexpected Error", L"Error");
		  }
		  finally {
            swFromFile->Close();
		  }
      }				 
	}

    private: System::Void FileSaveAsUnicode_Click(System::Object^  sender, System::EventArgs^  e) {

     // Displays an OpenFileDialog so the user can select a File.
      SaveFileDialog ^ saveFileDialog1 = gcnew SaveFileDialog();
      saveFileDialog1->Filter = "Text Files|*.txt";
      saveFileDialog1->Title = "Save File";
	  saveFileDialog1->InitialDirectory = "C:\\";

      // Show the Dialog.
      // If the user clicked OK in the dialog and
      // the Text file is saved in unicode format.
      if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
      {
		  System::IO::StreamWriter^ swFromFile;

		  try {
    		swFromFile = gcnew 
			System::IO::StreamWriter(saveFileDialog1->FileName, false, System::Text::Encoding::ASCII);
			swFromFile->Write( this->textpane->Text);
            swFromFile->Flush();
			docSaved = true;
		  }
		  catch(System::Exception ^ ) { 
			MessageBox::Show(L"Unexpected Error", L"Error");
		  }
		  finally {
            swFromFile->Close();
		  }
      }
	}

    private: System::Void FileSave_Click(System::Object^  sender, System::EventArgs^  e) {
        
          System::IO::StreamWriter^ swFromFile;

		  try {
			if(filename != nullptr) { // check if filename is not null
			  // save the file in its encoding format.
			  swFromFile = gcnew System::IO::StreamWriter(filename, false, encoding);
			  swFromFile->Write(this->textpane->Text);  // writing the text to the file
              swFromFile->Flush();  // flushing the stream
			  docSaved = true;
		    }
		  }
		  catch(System::Exception ^ ) {  
			MessageBox::Show(L"Unexpected Error", L"Error");
		  }
		  finally {
            swFromFile->Close();
		  }
	}

    private: System::Void FileNew_Click(System::Object^  sender, System::EventArgs^  e) {
		    // check if file saved, if not display dialog else reset the textbox and
			// set docsave to false.
			if(docSaved) {
			  this->textpane->Text=gcnew System::String(L"");
			  docSaved = false;
			}
			else {
              MessageBoxButtons buttons = MessageBoxButtons::YesNo;

              // Displays the MessageBox.
	          System::Windows::Forms::DialogResult result;
	          result = MessageBox::Show(L"Do you want to create new document without saving the document ?",  
	                                    L"Confirm", buttons);

              if ( result == System::Windows::Forms::DialogResult::Yes ) {
               // Closes the parent form.
			    this->textpane->Text=gcnew System::String(L"");
			    docSaved = false;
              }
			}
	}
};
}
