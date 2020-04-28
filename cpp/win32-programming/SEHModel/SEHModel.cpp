// SEHModel.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"

using namespace SEHModel;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 
	Application::SetUnhandledExceptionMode(UnhandledExceptionMode::ThrowException);

	// Create the main window and run it
	Application::Run(gcnew Form1());

	return 0;
}
