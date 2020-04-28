#pragma once

#include < process.h >
#include < windows.h >
#include < vcclr.h >
#include < errno.h >    // '_wtoi' does not reset 'errno' in the case of no error
#include "inject.h"
#define BUFFERSIZE 6144  // Size of the Memory Mapped File.

#pragma comment(lib, "Ws2_32.lib")

namespace Monitor1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Runtime::InteropServices;

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
        	if(this->processId != -1) {

	          PauseThread(true);
              EjectDLL(processId);
	          ReleaseMemory();
			}

			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  TitleLbl;
	private: System::Windows::Forms::Button^  pausebtn;

	private: System::Windows::Forms::Button^  stopbtn;
	private: System::Windows::Forms::Button^  exitbtn;
	private: System::Windows::Forms::ListBox^  msglist;
	private: System::Windows::Forms::TextBox^  processtext;

	private: System::Windows::Forms::Button^  monitorbtn;
	private: System::Windows::Forms::Label^  processlbl;


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
			this->TitleLbl = (gcnew System::Windows::Forms::Label());
			this->pausebtn = (gcnew System::Windows::Forms::Button());
			this->stopbtn = (gcnew System::Windows::Forms::Button());
			this->exitbtn = (gcnew System::Windows::Forms::Button());
			this->msglist = (gcnew System::Windows::Forms::ListBox());
			this->processtext = (gcnew System::Windows::Forms::TextBox());
			this->monitorbtn = (gcnew System::Windows::Forms::Button());
			this->processlbl = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// TitleLbl
			// 
			this->TitleLbl->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->TitleLbl->Location = System::Drawing::Point(123, 9);
			this->TitleLbl->MaximumSize = System::Drawing::Size(1000, 1000);
			this->TitleLbl->Name = L"TitleLbl";
			this->TitleLbl->Size = System::Drawing::Size(169, 33);
			this->TitleLbl->TabIndex = 0;
			this->TitleLbl->Text = L"Monitor Process";
			// 
			// pausebtn
			// 
			this->pausebtn->Location = System::Drawing::Point(71, 283);
			this->pausebtn->Name = L"pausebtn";
			this->pausebtn->Size = System::Drawing::Size(75, 23);
			this->pausebtn->TabIndex = 2;
			this->pausebtn->Text = L"Pause";
			this->pausebtn->UseVisualStyleBackColor = true;
			this->pausebtn->Click += gcnew System::EventHandler(this, &Form1::pausebtn_Click);
			// 
			// stopbtn
			// 
			this->stopbtn->Location = System::Drawing::Point(180, 283);
			this->stopbtn->Name = L"stopbtn";
			this->stopbtn->Size = System::Drawing::Size(75, 23);
			this->stopbtn->TabIndex = 3;
			this->stopbtn->Text = L"Stop";
			this->stopbtn->UseVisualStyleBackColor = true;
			this->stopbtn->Click += gcnew System::EventHandler(this, &Form1::stopbtn_Click);
			// 
			// exitbtn
			// 
			this->exitbtn->Location = System::Drawing::Point(299, 283);
			this->exitbtn->Name = L"exitbtn";
			this->exitbtn->Size = System::Drawing::Size(75, 23);
			this->exitbtn->TabIndex = 4;
			this->exitbtn->Text = L"Exit";
			this->exitbtn->UseVisualStyleBackColor = true;
			this->exitbtn->Click += gcnew System::EventHandler(this, &Form1::exitbtn_Click);
			// 
			// msglist
			// 
			this->msglist->FormattingEnabled = true;
			this->msglist->HorizontalScrollbar = true;
			this->msglist->Location = System::Drawing::Point(25, 94);
			this->msglist->Name = L"msglist";
			this->msglist->ScrollAlwaysVisible = true;
			this->msglist->Size = System::Drawing::Size(382, 173);
			this->msglist->TabIndex = 5;
			// 
			// processtext
			// 
			this->processtext->Location = System::Drawing::Point(154, 56);
			this->processtext->Name = L"processtext";
			this->processtext->Size = System::Drawing::Size(120, 20);
			this->processtext->TabIndex = 6;
			this->processtext->TextChanged += gcnew System::EventHandler(this, &Form1::processtext_TextChanged);
			// 
			// monitorbtn
			// 
			this->monitorbtn->Location = System::Drawing::Point(299, 56);
			this->monitorbtn->Name = L"monitorbtn";
			this->monitorbtn->Size = System::Drawing::Size(75, 23);
			this->monitorbtn->TabIndex = 7;
			this->monitorbtn->Text = L"Monitor";
			this->monitorbtn->UseVisualStyleBackColor = true;
			this->monitorbtn->Click += gcnew System::EventHandler(this, &Form1::monitorbtn_Click);
			// 
			// processlbl
			// 
			this->processlbl->Location = System::Drawing::Point(48, 58);
			this->processlbl->Name = L"processlbl";
			this->processlbl->Size = System::Drawing::Size(100, 23);
			this->processlbl->TabIndex = 8;
			this->processlbl->Text = L"Process Id / Name";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(436, 324);
			this->Controls->Add(this->processlbl);
			this->Controls->Add(this->monitorbtn);
			this->Controls->Add(this->processtext);
			this->Controls->Add(this->msglist);
			this->Controls->Add(this->exitbtn);
			this->Controls->Add(this->stopbtn);
			this->Controls->Add(this->pausebtn);
			this->Controls->Add(this->TitleLbl);
			this->Name = L"Form1";
			this->Text = L"Monitor";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

 private: Thread ^trd;  // Thread listening to the calls
		  HANDLE mapFile;
		  HANDLE mmfMutex, mmfSemaphore;
          bool status, thrdpause; // flags for thread terminate and thread pause
		  long processId;

 public:
	      // delegate method for UpdateListViewMethod()
          delegate void UpdateListView( void );
          UpdateListView^ updateDelegate;

		  // Get the calls from shared memory mapped file
 	      void UpdateListViewMethod(void);
	      void ThreadFunction(void);
	      void InitializeThread(void);
		  void InitializeMemory(void);
          void ReleaseMemory(void);

 private:
       void ShowError(String ^);

// check if the thread is currently in execution
 public: BOOL getThreadStatus() {
	        return this->status;
          }
 
// Causes thread to pause reulting no listening to calls in memory mapped file
 public: void PauseThread(bool pause) {
	        this->thrdpause = pause;
          }

 public: BOOL isThreadPaused() {
	        return this->thrdpause;
          }

 private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			  
   processId = -1;
   monitorbtn->Enabled = false;
   msglist->Enabled = false;
   pausebtn->Enabled = false;
   stopbtn->Enabled = false;
 }

 private: System::Void processtext_TextChanged(System::Object^  sender, System::EventArgs^  e) {

	if(processtext->Text != nullptr && processtext->Text->Length > 0) {
       monitorbtn->Enabled = true;
	}
	else
       monitorbtn->Enabled = false;
 }

 private: System::Void monitorbtn_Click(System::Object^  sender, System::EventArgs^  e) {

	if(processtext->Text != nullptr && processtext->Text->Length > 0) {

		// convert from String to wchar_t*
		pin_ptr<const wchar_t> wch = PtrToStringChars(processtext->Text);
        wchar_t* procName = (wchar_t*) wch;

		long processid = -1;
		processid = _wtol(procName);  // convert from wchar_t* to int

		if (processid == 0 || errno == ERANGE) { // check for out of range entries.
			// get the process id from process name
            processid = GetTargetProcessIdFromProcname(procName);
			errno = 0;
	    }

	    if(processid != -1) {
		   this->processId = processid;

		   // if DLL injected then carry out the further operations
		   if(InjectDLL(processid)) {
			   //Initialization of Memory and Thread
	         InitializeMemory();
 	         InitializeThread();
	         processtext->Enabled = false;
 	         monitorbtn->Enabled = false;
	         msglist->Enabled = true;
	         pausebtn->Enabled = true;
	         stopbtn->Enabled = true;
		   }
	    }
	    else {
	       MessageBox::Show(TEXT("Process not in Execution"), TEXT("Error"), 
	                   MessageBoxButtons::OK, MessageBoxIcon::Error);
	    }
	}
  }

  private: System::Void pausebtn_Click(System::Object^  sender, System::EventArgs^  e) {
              
	 if(isThreadPaused()) {
	   PauseThread(false);
	   pausebtn->Text = "Pause";
	 }
	 else {
	   PauseThread(true);
	   pausebtn->Text = "Resume";
	 }
  }

  private: System::Void stopbtn_Click(System::Object^  sender, System::EventArgs^  e) {

	if(this->processId != -1) {
	  PauseThread(true);
	  if(EjectDLL(processId)) {
	    processId = -1;
	    ReleaseMemory();
	    msglist->Items->Clear();
        processtext->Enabled = true;
        monitorbtn->Enabled = true;
        msglist->Enabled = false;
        pausebtn->Enabled = false;
        stopbtn->Enabled = false;
	  }
	}
  }

  private: System::Void exitbtn_Click(System::Object^  sender, System::EventArgs^  e) {
	 this->Close();          // close the dialog
	 this->~Form1();   // call the destructor
	 delete this;            // delete the object
  }
};

 ref class MyThreadClass  {
  private:
    Form1^ myForm1;

  public:
     MyThreadClass( Form1^ myForm ) {
       myForm1 = myForm;
     }

     void Run() {
	  while(myForm1->getThreadStatus()) {

		myForm1->Invoke( myForm1->updateDelegate);
		Thread::Sleep( 100 );
      }
     }
 };

 // Initialize Memory Mapped File , Mutexes And Semaphores
 void Form1 :: InitializeMemory(void) {

	TCHAR MemoryName[]=TEXT("GlobalSharedMemory");

	mapFile = OpenFileMapping( FILE_MAP_ALL_ACCESS,   // read/write access
                               FALSE,                 // do not inherit the name
                               MemoryName);               // name of mapping object 
 
    if(mapFile == NULL)  { 

	  mapFile=CreateFileMapping( INVALID_HANDLE_VALUE,    // use paging file
	  	                         NULL, PAGE_READWRITE, 0,
							     BUFFERSIZE, MemoryName);
	}

    if (mapFile == NULL)  { 
	  ShowError("Could not open file mapping object");
      return;
    } 

    strt = (int*) MapViewOfFile(mapFile, // handle to map object
                                FILE_MAP_ALL_ACCESS,  // read/write permission
                                0, 0, BUFFERSIZE);
 
    if (strt == NULL)  { 
	  ShowError("Could not map view for Buffer");
      CloseHandle(mapFile);
      return;
    }

	mmfMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"MMFMutex"); // Create Mutex, calling thread being not the
                                              // owner of the mutex.

	if (mmfMutex == NULL) 
    {
	  mmfMutex = CreateMutex(NULL, FALSE, L"MMFMutex"); // Create Mutex, calling thread being not the
                                                // owner of the mutex.
	}

	if (mmfMutex == NULL) 
    {
	    ShowError("Create MMFMutex error");
        return;
    }

    mmfSemaphore = OpenSemaphore( 
		           SEMAPHORE_ALL_ACCESS,           // default security attributes
                   FALSE,  // maximum count
                   TEXT("MMFSemaphore"));          // named semaphore

    if (mmfSemaphore == NULL) 
    {
        mmfSemaphore = CreateSemaphore( 
           NULL,           // default security attributes
           1,  // initial count
           1,  // maximum count
           TEXT("MMFSemaphore"));          // named semaphore
	}

    if (mmfSemaphore == NULL) 
    {
	    ShowError("Create MMFSemaphore error");
        return;
    }
 }	

 // Release all the resources
 void Form1 :: ReleaseMemory() {
	 status = false;
	 trd->Abort();

     UnmapViewOfFile(strt);
     CloseHandle(mapFile);
     CloseHandle(mmfMutex);
	 CloseHandle(mmfSemaphore);
 }

 void Form1 :: ThreadFunction() {
    status = true;
    MyThreadClass^ myThreadClassObject = gcnew MyThreadClass( this );
    myThreadClassObject->Run();
 }

 void Form1 :: InitializeThread() {
	updateDelegate = gcnew UpdateListView( this, &Form1::UpdateListViewMethod );
	ThreadStart ^myThreadDelegate = gcnew ThreadStart(this, &Form1::ThreadFunction); 
    trd = gcnew Thread(myThreadDelegate); 
    trd->IsBackground = true; 
    trd->Start(); 
 }

 // Get the calls from the Memory Mapped File and Display them
 void Form1 :: UpdateListViewMethod() {

   if(!isThreadPaused()) {

	   // Wait for the Mutex
	 DWORD result = WaitForSingleObject(mmfMutex, INFINITE);           // zero-second time-out interval
	 if(result == WAIT_OBJECT_0) {
      // check the first integer data field in memory mapped file
	  // representing the call function, then get the call parameter structure and
	  // then get the string stored after the struct represented in the parameter structure
	  if(*strt == 1) {
		connstruct = (connStruct*)(strt + sizeof(int));

		if(connstruct != NULL && (*connstruct).s  > 0) {
			String^ str;

			if((*connstruct).nameoffset != -1) {

				if((*connstruct).namelen == sizeof(SOCKADDR)) {
				  struct sockaddr* addr = (sockaddr*)(connstruct + (*connstruct).nameoffset);

				  str = "Connect(" + (*connstruct).s + ","  + gcnew String(addr->sa_data) + "," +
				                     (*connstruct).namelen + ")";
				}
				else if((*connstruct).namelen == sizeof(SOCKADDR_IN)) {
					struct sockaddr_in* addr = (sockaddr_in*)(connstruct + (*connstruct).nameoffset);
					str = "Connect(" + (*connstruct).s + ","  + addr->sin_port + ": " +
					           gcnew String(inet_ntoa(addr->sin_addr)) + "," +
				                     (*connstruct).namelen + ")";
				}
			}
			else {
				str = "Connect(" + (*connstruct).s + ", ," +
					                    (*connstruct).namelen + ")";

			}

	        msglist->Items->Add(str);

            //connstruct = NULL;
			(*connstruct).s = 0;
		}
	  }
	  else if(*strt == 2) {
		sendstruct = (sendStruct*)(strt + sizeof(int));

		if(sendstruct != NULL && (*sendstruct).s  > 0) {
			
			String^ str;

			if((*sendstruct).bufoffet != -1) {

				char* tbuf = (char*)(sendstruct + (*sendstruct).bufoffet);

				str = "Send(" + (*sendstruct).s + ","  + gcnew String(tbuf) + "," +
                                         (*sendstruct).len + "," + (*sendstruct).flags + ")";
			}
			else {
				str = "Send(" + (*sendstruct).s + ", ," +
                                         (*sendstruct).len + "," + (*sendstruct).flags + ")";

			}

	        msglist->Items->Add(str);

            //sendstruct = NULL;
           (*sendstruct).s = 0;
		}
	  }
	  else if(*strt == 3){
 		recvstruct = (recvStruct*)(strt + sizeof(int));

		if(recvstruct != NULL && (*recvstruct).s  > 0) {
			
			String^ str;

			if((*recvstruct).bufoffet != -1) {

				char* tbuf = (char*)(recvstruct + (*recvstruct).bufoffet);

				str = "Recv(" + (*recvstruct).s + ","  + gcnew String(tbuf) + "," +
                                         (*recvstruct).len + "," + (*recvstruct).flags + ")";
			}
			else {
				str = "Recv(" + (*recvstruct).s + ", ," +
                                         (*recvstruct).len + "," + (*recvstruct).flags + ")";

			}

	        msglist->Items->Add(str);
            //recvstruct = NULL;
			(*recvstruct).s = 0;
		}
	  }
	  else if(*strt == 4){
    	 closstruct = (closStruct*)(strt + sizeof(int));

		 if(closstruct != NULL && (*closstruct).s  > 0) {
             String^ str = "CloseSocket(" + (*closstruct).s + ")";
             msglist->Items->Add(str);
             //closstruct = NULL;
			 (*closstruct).s = 0;
		 }
	  }

	 if (! ReleaseMutex(mmfMutex)) {   // release mutex.
	   ShowError("Release MMFMutex error");
	   status = false;
	   return;
	 }
	}
   }

   // When the data in MMF file is read in, release the semaphore to allow the
   // injected dll fetch the next call in mmf file
   if (*strt > 0 && *strt < 5 )       // check to see it its valid mmf data.
   {
	 *strt= -1;                      // set a unused mmf file.
	 if(!ReleaseSemaphore( mmfSemaphore,  // handle to semaphore
                           1,            // increase count by one
  						   NULL)) {
        ShowError("Release MMFSemaphore error");
   	    status = false;
        return;
	 }
   }
}

  void Form1 :: ShowError(String ^ message) {
        LPVOID  hlocal = NULL;
        DWORD dw = GetLastError();  // get the last error code
 
        try {
	      // use formatmessage to get the error code message

          DWORD ok = FormatMessage( 
                                    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                                    FORMAT_MESSAGE_FROM_SYSTEM |
                                    FORMAT_MESSAGE_IGNORE_INSERTS,
                                    NULL, dw,
                                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                    (LPTSTR) &hlocal,
                                    0, NULL );

          if(ok)
			  MessageBox::Show(TEXT("Error : ") + message +
			                 TEXT(" : ") + gcnew String( (PTSTR) hlocal), 
			                 TEXT("Message"), MessageBoxButtons::OK,
                             MessageBoxIcon::Error);
          else
	        MessageBox::Show(TEXT("Unknown Error : ") + GetLastError(), 
			                 TEXT("Error"), MessageBoxButtons::OK,
                             MessageBoxIcon::Error);
        } catch(Exception ^e) {
	      MessageBox::Show(TEXT("Unknown Error : ") + e, 
			               TEXT("Error"), MessageBoxButtons::OK,
                           MessageBoxIcon::Error);
        }
 }

