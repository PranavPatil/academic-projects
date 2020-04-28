// ProcessEmulator.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"

using namespace ProcessEmulator;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	try {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	}
	catch(System::Exception ^e) {
	   MessageBox::Show(TEXT("Unknown Error : ") + e, TEXT("Error"), MessageBoxButtons::OK,
                    MessageBoxIcon::Error);
	}
	return 0;
}
