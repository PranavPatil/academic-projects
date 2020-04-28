#pragma once

#include < process.h >
#include < windows.h >
#include < vcclr.h >
#include < tchar.h >
#define BUFFERSIZE 4096

//#pragma data_seg("MyShared")
//volatile LONG InstancesRunning = 0;
//volatile BOOL isWriteProgress = FALSE;
//#pragma data_seg()
//#pragma comment(linker, "/SECTION:MyShared,RWS")

CRITICAL_SECTION CriticalSection; 

namespace SharedMemory {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

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
		  // INCOMPLETE: Check if no process is running then release the memory.
  	      ReleaseMemory();

		  if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  MemText;
	protected: 

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
			this->MemText = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// MemText
			// 
			this->MemText->Location = System::Drawing::Point(12, 46);
			this->MemText->Multiline = true;
			this->MemText->Name = L"MemText";
			this->MemText->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->MemText->Size = System::Drawing::Size(259, 170);
			this->MemText->TabIndex = 0;
			this->MemText->TextChanged += gcnew System::EventHandler(this, &Form1::MemText_TextChanged);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 264);
			this->Controls->Add(this->MemText);
			this->Name = L"Form1";
			this->Text = L"SharedMemory";
			this->Load += gcnew System::EventHandler(this, &Form1::Form_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

   private: Thread ^trd; 
		    HANDLE mapFile;
		    LPCTSTR pBuf;
		    bool status;
			LONG * InstancesRunning;
            BOOL *  isWriteProgress;

   public:
   delegate void UpdateMemBox( void );
   UpdateMemBox^ myDelegate;

   public:
	   void UpdateMemBoxMethod(void);
	   void ThreadFunction(void);
	   void InitializeThread(void);
	   void InitializeMemory(void);
       void ReleaseMemory(void);

   private:
       void ShowError(String ^);
       size_t To_CharStar( String^, TCHAR*&);

	private: System::Void Form_Load(System::Object^  sender, System::EventArgs^  e) {
       InitializeMemory();
	   InitializeThread();
    }

	private: System::Void MemText_TextChanged(System::Object^  sender, System::EventArgs^  e) {

	    TCHAR* b = nullptr;

		To_CharStar(MemText->Text , b);

	while (InterlockedExchange ((LONG *)isWriteProgress, TRUE) == TRUE)
          Sleep(10);

		EnterCriticalSection(&CriticalSection);

        ZeroMemory((PVOID)pBuf, BUFFERSIZE);
	    CopyMemory((PVOID)pBuf, b, (_tcslen(b) * sizeof(TCHAR)));

		LeaveCriticalSection(&CriticalSection);

	    InterlockedExchange((LONG *)isWriteProgress, FALSE);
    }

	public: BOOL getThreadStatus() {
		return this->status;
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

		myForm1->Invoke( myForm1->myDelegate);
		Thread::Sleep( 100 );
      }
     }
 };

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

    pBuf = (LPTSTR) MapViewOfFile(mapFile, // handle to map object
                                  FILE_MAP_ALL_ACCESS,  // read/write permission
                                  0, 0, (BUFFERSIZE - sizeof(INT) - sizeof(BOOL)));
 
    if (pBuf == NULL)  { 
	  ShowError("Could not map view for Buffer");
      CloseHandle(mapFile);
      return;
    }
	
    InstancesRunning = (LONG *) MapViewOfFile(mapFile, // handle to map object
                                  FILE_MAP_ALL_ACCESS,  // read/write permission
                                  0, 0, sizeof(LONG));
 
    if (InstancesRunning == NULL)  { 
	  ShowError("Could not map view for InstanceRunning");
      CloseHandle(mapFile);
      return;
    }

    isWriteProgress = (BOOL *) MapViewOfFile(mapFile, // handle to map object
                                  FILE_MAP_ALL_ACCESS,  // read/write permission
                                  0, 0, sizeof(BOOL));
 
    if (isWriteProgress == NULL)  { 
	  ShowError("Could not map view for isWriteProgress");
      CloseHandle(mapFile);
      return;
    }

	this->MemText->MaxLength = (BUFFERSIZE / sizeof(TCHAR)) - 1;

	bool flag = FALSE;
	CopyMemory((PVOID)isWriteProgress, &flag, sizeof(flag));
	long value = 0;
	CopyMemory((PVOID)InstancesRunning, &value, sizeof(value));
	InterlockedExchangeAdd(InstancesRunning, 1);

	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x80000400) ) {
	  ShowError("Could not initialize critical section");
      UnmapViewOfFile(pBuf);
	  UnmapViewOfFile((LPVOID)InstancesRunning);
      UnmapViewOfFile((LPVOID)isWriteProgress);
      CloseHandle(mapFile);
	  return;
	}
 }

 void Form1 :: ReleaseMemory() {

	 status = false;
	 trd->Abort();

	 if(*InstancesRunning == 1) {
        UnmapViewOfFile(pBuf);
        UnmapViewOfFile((LPVOID)InstancesRunning);
        UnmapViewOfFile((LPVOID)isWriteProgress);
        CloseHandle(mapFile);
	 }

	 // Release resources used by the critical section object.
     DeleteCriticalSection(&CriticalSection);
 }

	  // Display Error Info
      // IMPORTANT : Change the CLR Support from /clr:pure to /clr to avoid exceptions
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
                                    (PTSTR) &hlocal,
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

 // Convert String to char*
 size_t Form1 :: To_CharStar( String^ source, TCHAR*& target ) {

     // Pin memory so GC can't move it while native function is called
     pin_ptr<const wchar_t> wch = PtrToStringChars(source);
     size_t err = false;

	 #ifdef UNICODE

	 //wchar_t *wchar;
	 //wchar = (wchar_t*)((void*)wch);
	 //mbstowcs( wcstring, search.c_str(), newsize);
     target = (wchar_t*) wch;
     //err = wcscpy_s(target, source->Length + 1, wch);

//	 err = wmemcpy_s(target, source->Length + 1, wch, source->Length + 1);

     #else
       size_t convertedChars = 0;
       size_t  sizeInBytes = ((source->Length + 1) * sizeof(TCHAR)); // find the size of the char buffer
       target = (TCHAR *)malloc(sizeInBytes);  // allocate a char buffer
   	   err = wcstombs_s(&convertedChars, target, sizeInBytes, wch, sizeInBytes);
     #endif

     return err;
 }

 void Form1 :: ThreadFunction() {
    status = true;
    MyThreadClass^ myThreadClassObject = gcnew MyThreadClass( this );
    myThreadClassObject->Run();
 }

 void Form1 :: InitializeThread() {
	myDelegate = gcnew UpdateMemBox( this, &Form1::UpdateMemBoxMethod );
	ThreadStart ^myThreadDelegate = gcnew ThreadStart(this, &Form1::ThreadFunction); 
    trd = gcnew Thread(myThreadDelegate); 
    trd->IsBackground = true; 
    trd->Start(); 
 }

 void Form1 :: UpdateMemBoxMethod() {

	EnterCriticalSection(&CriticalSection);
    MemText->Text = gcnew String((LPCTSTR) pBuf);
    LeaveCriticalSection(&CriticalSection);
 }
}
