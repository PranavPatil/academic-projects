#pragma once

#include < windows.h >
#include < tlhelp32.h >
#include < process.h >
#include < strsafe.h >

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ProcessEmulator {

	/// <summary>
	/// Summary for DetailsDlg
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class DetailsDlg : public System::Windows::Forms::Form
	{
	public:
		DetailsDlg(DWORD pid)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			// get the process id from the parent
			this->Pid = pid;
			// retrive the module details of the process
		    this->ListProcessModules();
			// get the details of all the threads running under the process
			this->ListProcessThreads();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~DetailsDlg()
		{
			this->Pid = 0;

			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  detailsTxt;
	protected: 

	private: System::Windows::Forms::Button^  OkBtn;
	private: System::Windows::Forms::Label^  DetailLbl;
	protected: 


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		DWORD Pid;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->detailsTxt = (gcnew System::Windows::Forms::TextBox());
			this->OkBtn = (gcnew System::Windows::Forms::Button());
			this->DetailLbl = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// detailsTxt
			// 
			this->detailsTxt->Location = System::Drawing::Point(12, 42);
			this->detailsTxt->Multiline = true;
			this->detailsTxt->Name = L"detailsTxt";
			this->detailsTxt->ReadOnly = true;
			this->detailsTxt->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->detailsTxt->Size = System::Drawing::Size(388, 243);
			this->detailsTxt->TabIndex = 0;
			// 
			// OkBtn
			// 
			this->OkBtn->Location = System::Drawing::Point(176, 308);
			this->OkBtn->Name = L"OkBtn";
			this->OkBtn->Size = System::Drawing::Size(75, 23);
			this->OkBtn->TabIndex = 1;
			this->OkBtn->Text = L"OK";
			this->OkBtn->UseVisualStyleBackColor = true;
			this->OkBtn->Click += gcnew System::EventHandler(this, &DetailsDlg::OkBtn_Click);
			// 
			// DetailLbl
			// 
			this->DetailLbl->AutoSize = true;
			this->DetailLbl->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->DetailLbl->Location = System::Drawing::Point(151, 9);
			this->DetailLbl->Name = L"DetailLbl";
			this->DetailLbl->Size = System::Drawing::Size(78, 25);
			this->DetailLbl->TabIndex = 2;
			this->DetailLbl->Text = L"Details";
			// 
			// DetailsDlg
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(414, 343);
			this->Controls->Add(this->DetailLbl);
			this->Controls->Add(this->OkBtn);
			this->Controls->Add(this->detailsTxt);
			this->Name = L"DetailsDlg";
			this->Text = L"DetailsDlg";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
	private: System::Void OkBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();          // close the dialog
				 this->~DetailsDlg();   // call the destructor
				 delete this;            // delete the object
	}

 // function to list the module info of the given process
  private: BOOL ListProcessModules()
 {
  HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
  MODULEENTRY32 me32;

  // Take a snapshot of all modules in the specified process.
  hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, Pid );
  if( hModuleSnap == INVALID_HANDLE_VALUE )
  {
	this->ShowError();
    return false;
  }

  // Set the size of the structure before using it.
  me32.dwSize = sizeof( MODULEENTRY32 );

  // Retrieve information about the first module,
  // and exit if unsuccessful
  if( !Module32First( hModuleSnap, &me32 ) )
  {
	this->ShowError();
    CloseHandle( hModuleSnap );           // clean the snapshot object
    return false;
  }

  // Now walk the module list of the process,
  // and display information about each module
  do  {
	  // load all the info in a String
	  String ^text = TEXT("     MODULE NAME:     ") + gcnew String(me32.szModule);
	  text = System::String::Concat(text, TEXT("\r\n\r\n     Executable     =  "));
	  text = System::String::Concat(text, gcnew String(me32.szExePath));

      wchar_t buf[80];
      _ultow_s(me32.th32ProcessID, buf, 80, 10);

	  text = System::String::Concat(text, TEXT( "\r\n\r\n     Process ID     = "));
	  text = System::String::Concat(text, gcnew String(buf));


      _ultow_s(me32.GlblcntUsage, buf, 80, 10);

	  text = System::String::Concat(text, TEXT( "\r\n\r\n     Ref count (g)  = "));
	  text = System::String::Concat(text, gcnew String(buf));

      _ultow_s(me32.ProccntUsage, buf, 80, 10);

	  text = System::String::Concat(text, TEXT( "\r\n\r\n     Ref count (p)  = "));
	  text = System::String::Concat(text, gcnew String(buf));

      _ultow_s((DWORD)me32.modBaseAddr, buf, 80, 10);

	  text = System::String::Concat(text, TEXT( "\r\n\r\n     Base address  = "));
	  text = System::String::Concat(text, gcnew String(buf));

      _ultow_s((DWORD)me32.modBaseSize, buf, 80, 10);

	  text = System::String::Concat(text, TEXT( "\r\n\r\n     Base size  = "));
	  text = System::String::Concat(text, gcnew String(buf));

	  // set the textpane with the text
	  this->detailsTxt->Text = text;
  } while( Module32Next( hModuleSnap, &me32 ) );

  CloseHandle( hModuleSnap ); // close handle for the Module Snap object
  return true;
 }

// To display the list of threads running under the current process.
 BOOL ListProcessThreads() 
 { 
  HANDLE hThreadSnap = INVALID_HANDLE_VALUE; 
  THREADENTRY32 te32; 
 
  // Take a snapshot of all running threads  
  hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); 
  if( hThreadSnap == INVALID_HANDLE_VALUE ) 
    return( FALSE ); 
 
  // Fill in the size of the structure before using it. 
  te32.dwSize = sizeof(THREADENTRY32); 
 
  // Retrieve information about the first thread,
  // and exit if unsuccessful
  if( !Thread32First( hThreadSnap, &te32 ) ) 
  {
	this->ShowError();
    CloseHandle( hThreadSnap );          // clean the snapshot object
    return( FALSE );
  }

  // Now walk the thread list of the system,
  // and display information about each thread
  // associated with the specified process

  this->detailsTxt->Text = System::String::Concat(this->detailsTxt->Text, TEXT( "\r\n\r\n     LIST OF ALL THREADS RUNNING IN PROCESS   "));

  do 
  { 
    // check if the owner process id of the thread and the selected process id are same
    if( te32.th32OwnerProcessID == this->Pid )
    {
	  String ^text;
      wchar_t buf[80];

	  // fill all the thread info in the string
      _ultow_s((DWORD)te32.th32ThreadID, buf, 80, 10);
	  text = System::String::Concat(text, TEXT( "\r\n\r\n     THREAD ID      =  "));
	  text = System::String::Concat(text, gcnew String(buf));

      _ultow_s((DWORD)te32.tpBasePri, buf, 80, 10);
	  text = System::String::Concat(text, TEXT( "\r\n\r\n     Base priority      =  "));
	  text = System::String::Concat(text, gcnew String(buf));

      _ultow_s((DWORD)te32.tpDeltaPri, buf, 80, 10);
	  text = System::String::Concat(text, TEXT( "\r\n\r\n     Delta priority      =  "));
	  text = System::String::Concat(text, gcnew String(buf));

	  this->detailsTxt->Text = System::String::Concat(this->detailsTxt->Text, text);
    }
	// Continue to walk through the ThreadEntry to get the other threads associated with
	// the given process id.
  } while( Thread32Next(hThreadSnap, &te32 ) );

  CloseHandle( hThreadSnap );  // close handle to the thread snap object
  return( TRUE );
 }

// Display Error Info
// IMPORTANT : Change the CLR Support from /clr:pure to /clr to avoid exceptions

 private: void ShowError() {
  LPVOID  hlocal = NULL;
  DWORD dw = GetLastError();  // get the last error code

  try {
	  // use formatmessage to get the error code message
    DWORD ok = FormatMessage( 
          FORMAT_MESSAGE_ALLOCATE_BUFFER | 
          FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
          NULL,
          dw,
          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
          (PTSTR) &hlocal,
          0, NULL );

    if(ok)
	   MessageBox::Show(TEXT("Error : ") + gcnew String( (PTSTR) hlocal), TEXT("Message"), MessageBoxButtons::OK,
                    MessageBoxIcon::Error);
    else
	   MessageBox::Show(TEXT("Unknown Error : ") + GetLastError(), TEXT("Error"), MessageBoxButtons::OK,
                    MessageBoxIcon::Error);
  } catch(Exception ^e) {
	   MessageBox::Show(TEXT("Unknown Error : ") + e, TEXT("Error"), MessageBoxButtons::OK,
                    MessageBoxIcon::Error);
  }
 }
};
}
