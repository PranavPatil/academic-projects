#pragma once

/* Assignment No 3 : PRANAV PATIL */

#include < windows.h >
#include < tlhelp32.h >
#include < vcclr.h >
#include "DetailsDlg.h"

namespace ProcessEmulator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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
	private: System::Windows::Forms::ListView^  processList;
	private: System::Windows::Forms::ColumnHeader^  ProcessName;
	private: System::Windows::Forms::ColumnHeader^  ProcessId;
	private: System::Windows::Forms::ColumnHeader^  CpuUsage;
	private: System::Windows::Forms::ColumnHeader^  Threads;

	protected: 

	protected: 

	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::ColumnHeader^  Priority;
	private: System::Windows::Forms::ColumnHeader^  ParentProcess;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->processList = (gcnew System::Windows::Forms::ListView());
			this->ProcessName = (gcnew System::Windows::Forms::ColumnHeader());
			this->ProcessId = (gcnew System::Windows::Forms::ColumnHeader());
			this->CpuUsage = (gcnew System::Windows::Forms::ColumnHeader());
			this->Threads = (gcnew System::Windows::Forms::ColumnHeader());
			this->Priority = (gcnew System::Windows::Forms::ColumnHeader());
			this->ParentProcess = (gcnew System::Windows::Forms::ColumnHeader());
			this->SuspendLayout();
			// 
			// processList
			// 
			this->processList->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(6) {this->ProcessName, this->ProcessId, 
				this->CpuUsage, this->Threads, this->Priority, this->ParentProcess});
			this->processList->GridLines = true;
			this->processList->Location = System::Drawing::Point(12, 12);
			this->processList->Name = L"processList";
			this->processList->Size = System::Drawing::Size(532, 318);
			this->processList->Sorting = System::Windows::Forms::SortOrder::Ascending;
			this->processList->TabIndex = 0;
			this->processList->UseCompatibleStateImageBehavior = false;
			this->processList->View = System::Windows::Forms::View::Details;
			this->processList->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::processList_SelectedIndexChanged_1);
			// 
			// ProcessName
			// 
			this->ProcessName->Tag = L"";
			this->ProcessName->Text = L"Process Name";
			this->ProcessName->Width = 144;
			// 
			// ProcessId
			// 
			this->ProcessId->Text = L"Process Id";
			this->ProcessId->Width = 70;
			// 
			// CpuUsage
			// 
			this->CpuUsage->Text = L"CPU Usage";
			this->CpuUsage->Width = 72;
			// 
			// Threads
			// 
			this->Threads->Text = L"Threads";
			this->Threads->Width = 64;
			// 
			// Priority
			// 
			this->Priority->Text = L"Priority";
			this->Priority->Width = 59;
			// 
			// ParentProcess
			// 
			this->ParentProcess->Text = L"ParentProcess";
			this->ParentProcess->Width = 85;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(556, 342);
			this->Controls->Add(this->processList);
			this->Name = L"Form1";
			this->Text = L"TaskManager";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			ThreadFunc(); // Calling the function to load data
			return;
	}

  private: void ThreadFunc() {
  	// Get the process list snapshot.
    HANDLE hProcessSnapShot;

    // Initialize the process entry structure.
    PROCESSENTRY32 ProcessEntry = { 0 };
    ProcessEntry.dwSize = sizeof( ProcessEntry );

    // Get the first process info.
	hProcessSnapShot = CreateToolhelp32Snapshot( TH32CS_SNAPALL, 0 );
      if( hProcessSnapShot == INVALID_HANDLE_VALUE ) {
         this->ShowError();
         return;
      }

      BOOL Return = FALSE;
      Return = Process32First( hProcessSnapShot,  // Get the first process entry
                              &ProcessEntry );

      // Getting process info failed.
      if( !Return ) {
		  this->ShowError();
		  CloseHandle( hProcessSnapShot );  // close handle
         return;
      }

	  try {

      DWORD dwPriorityClass;
	  HANDLE hProcess;

	  do {
	     // Retrieve the priority class.
         dwPriorityClass = 0;

		 // Open the process to get priority info using process id
         hProcess = OpenProcess( PROCESS_QUERY_LIMITED_INFORMATION, FALSE, ProcessEntry.th32ProcessID );
         if( hProcess == NULL )
			 dwPriorityClass = 0;

         else {
			 // get process priority
           dwPriorityClass = GetPriorityClass( hProcess );
           if( !dwPriorityClass )
        	  this->ShowError();
           CloseHandle( hProcess );  // close handle to the process
         }
		  
		 // print the process details.
	     ListViewItem^ item1 = gcnew ListViewItem(gcnew String(ProcessEntry.szExeFile) );

         // Place a check mark next to the item.
         wchar_t buf[80];
         _ultow_s(ProcessEntry.th32ProcessID, buf, 80, 10);
		 item1->SubItems->Add( gcnew String(buf) );
		 _ultow_s(ProcessEntry.dwSize, buf, 80, 10);
         item1->SubItems->Add( gcnew String(buf) );
         _ultow_s(ProcessEntry.cntThreads, buf, 80, 10);
		 item1->SubItems->Add( gcnew String(buf) );
         _ultow_s(dwPriorityClass , buf, 80, 10);
		 item1->SubItems->Add( gcnew String(buf) );
		 _ultow_s(ProcessEntry.th32ParentProcessID , buf, 80, 10);
		 item1->SubItems->Add( gcnew String(buf) );

		 processList->Items->Add(item1);
         // check the PROCESSENTRY32 for other members.
      }
      while( Process32Next( hProcessSnapShot, &ProcessEntry ));
	  } 
	  catch(System::Exception^ e) { 
	  MessageBox::Show(TEXT("Unexpected Error : ") + e, TEXT("Error"), MessageBoxButtons::OK,
                          MessageBoxIcon::Error);
	  }

	  // Close the handle
      CloseHandle( hProcessSnapShot );
	return;
  }

// event handling on selecting a row from the list
private: System::Void processList_SelectedIndexChanged_1(System::Object^  sender, System::EventArgs^  e) {

		 // check for the selected process
		if(this->processList->SelectedItems->Count > 0 ) {

			// get the listviewitem for the selected process
           ListViewItem ^itm = ((ListViewItem^)processList->SelectedItems[0]);
           char* b = nullptr;
		   // get process id and convert it from String to char*
		   To_CharStar(itm->SubItems[1]->Text , b);
		   const char* buf = b;
		   // convert process id from char* to DWORD
           DWORD Pid = strtol(buf, nullptr, 10);
           free(b); // free allocated memory for char buffer
		   // initialize and display the details dialog
		   DetailsDlg ^ details = gcnew DetailsDlg(Pid); 
		   details->Show(this);
		}
	}

// Convert String to char*
private: size_t To_CharStar( String^ source, char*& target )
{
   // Pin memory so GC can't move it while native function is called
   pin_ptr<const wchar_t> wch = PtrToStringChars(source);

   size_t convertedChars = 0;
   size_t  sizeInBytes = ((source->Length + 1) * 2); // find the size of the char buffer
   size_t err = false;
   target = (char *)malloc(sizeInBytes);  // allocate a char buffer

   // convert wide characters to multibyte characters
   err = wcstombs_s(&convertedChars,  
                    target, sizeInBytes,
                    wch, sizeInBytes);
   return err;
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
