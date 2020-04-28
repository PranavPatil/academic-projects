#pragma once

// ASSIGNMENT NO 10: STRUCTURED EXCEPTION HANDLING
// PRANAV A PATIL


#include "exfunction.h"
// Define all the constants, used just for the sake of readability
#define exception 1
#define trycatch 2
#define __try__except_EX_EXECUTE_HANDLER 3
#define __try__except_EX_CONTINUE_SEARCH 4
#define __try__except_EX_CONTINUE_EXECUTION 5
#define __try__finally 6
#define UnhandledExceptionFilter_EXHD 7
#define UnhandledExceptionFilter_EXCS 8
#define UnhandledExceptionFilter_EXCE 9
#define VectoredExceptionHandling 10
#define VectoredContinueHandling 11


namespace SEHModel {

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

			// Set all components to default
			tryexptcombo->SelectedIndex = 0;
			unhanexfilcombo->SelectedIndex = 0;
			vexprtycombo->SelectedIndex = 1;
			vcnprtycombo->SelectedIndex = 1;
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
	private: System::Windows::Forms::ListBox^  inputlist;
	private: System::Windows::Forms::Button^  trycatchbtn;
	private: System::Windows::Forms::Button^  _tryexceptbtn;
	private: System::Windows::Forms::Button^  _tryfinallybtn;
	private: System::Windows::Forms::Button^  unhanexfilbtn;
	private: System::Windows::Forms::ComboBox^  tryexptcombo;
	private: System::Windows::Forms::Button^  vectorexhdbtn;
	private: System::Windows::Forms::Button^  executebtn;
	private: System::Windows::Forms::Button^  exitbtn;
	private: System::Windows::Forms::ComboBox^  unhanexfilcombo;
	private: System::Windows::Forms::Button^  vectorchhdbtn;
	private: System::Windows::Forms::ListBox^  outputlist;
	private: System::Windows::Forms::Label^  TitleLbl;
	private: System::Windows::Forms::Button^  exceptionbtn;
	private: System::Windows::Forms::Button^  clearbtn;
	private: System::Windows::Forms::Label^  priorityLbl1;
	private: System::Windows::Forms::Label^  priorityLbl2;
	private: System::Windows::Forms::ComboBox^  vexprtycombo;
	private: System::Windows::Forms::ComboBox^  vcnprtycombo;
	private: System::Windows::Forms::Label^  notelbl;







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
			this->inputlist = (gcnew System::Windows::Forms::ListBox());
			this->trycatchbtn = (gcnew System::Windows::Forms::Button());
			this->_tryexceptbtn = (gcnew System::Windows::Forms::Button());
			this->_tryfinallybtn = (gcnew System::Windows::Forms::Button());
			this->unhanexfilbtn = (gcnew System::Windows::Forms::Button());
			this->tryexptcombo = (gcnew System::Windows::Forms::ComboBox());
			this->vectorexhdbtn = (gcnew System::Windows::Forms::Button());
			this->executebtn = (gcnew System::Windows::Forms::Button());
			this->exitbtn = (gcnew System::Windows::Forms::Button());
			this->unhanexfilcombo = (gcnew System::Windows::Forms::ComboBox());
			this->vectorchhdbtn = (gcnew System::Windows::Forms::Button());
			this->outputlist = (gcnew System::Windows::Forms::ListBox());
			this->TitleLbl = (gcnew System::Windows::Forms::Label());
			this->exceptionbtn = (gcnew System::Windows::Forms::Button());
			this->clearbtn = (gcnew System::Windows::Forms::Button());
			this->priorityLbl1 = (gcnew System::Windows::Forms::Label());
			this->priorityLbl2 = (gcnew System::Windows::Forms::Label());
			this->vexprtycombo = (gcnew System::Windows::Forms::ComboBox());
			this->vcnprtycombo = (gcnew System::Windows::Forms::ComboBox());
			this->notelbl = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// inputlist
			// 
			this->inputlist->FormattingEnabled = true;
			this->inputlist->HorizontalScrollbar = true;
			this->inputlist->Location = System::Drawing::Point(12, 52);
			this->inputlist->Name = L"inputlist";
			this->inputlist->ScrollAlwaysVisible = true;
			this->inputlist->Size = System::Drawing::Size(286, 108);
			this->inputlist->TabIndex = 0;
			// 
			// trycatchbtn
			// 
			this->trycatchbtn->Location = System::Drawing::Point(319, 79);
			this->trycatchbtn->Name = L"trycatchbtn";
			this->trycatchbtn->Size = System::Drawing::Size(72, 23);
			this->trycatchbtn->TabIndex = 1;
			this->trycatchbtn->Text = L"trycatch";
			this->trycatchbtn->UseVisualStyleBackColor = true;
			this->trycatchbtn->Click += gcnew System::EventHandler(this, &Form1::trycatchbtn_Click);
			// 
			// _tryexceptbtn
			// 
			this->_tryexceptbtn->Location = System::Drawing::Point(319, 108);
			this->_tryexceptbtn->Name = L"_tryexceptbtn";
			this->_tryexceptbtn->Size = System::Drawing::Size(72, 23);
			this->_tryexceptbtn->TabIndex = 2;
			this->_tryexceptbtn->Text = L"_tryexcept";
			this->_tryexceptbtn->UseVisualStyleBackColor = true;
			this->_tryexceptbtn->Click += gcnew System::EventHandler(this, &Form1::_tryexceptbtn_Click);
			// 
			// _tryfinallybtn
			// 
			this->_tryfinallybtn->Location = System::Drawing::Point(319, 137);
			this->_tryfinallybtn->Name = L"_tryfinallybtn";
			this->_tryfinallybtn->Size = System::Drawing::Size(72, 23);
			this->_tryfinallybtn->TabIndex = 3;
			this->_tryfinallybtn->Text = L"_tryfinally";
			this->_tryfinallybtn->UseVisualStyleBackColor = true;
			this->_tryfinallybtn->Click += gcnew System::EventHandler(this, &Form1::_tryfinallybtn_Click);
			// 
			// unhanexfilbtn
			// 
			this->unhanexfilbtn->Location = System::Drawing::Point(319, 166);
			this->unhanexfilbtn->Name = L"unhanexfilbtn";
			this->unhanexfilbtn->Size = System::Drawing::Size(72, 23);
			this->unhanexfilbtn->TabIndex = 4;
			this->unhanexfilbtn->Text = L"UnhanExFil";
			this->unhanexfilbtn->UseVisualStyleBackColor = true;
			this->unhanexfilbtn->Click += gcnew System::EventHandler(this, &Form1::unhanexfilbtn_Click);
			// 
			// tryexptcombo
			// 
			this->tryexptcombo->FormattingEnabled = true;
			this->tryexptcombo->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"EX_EXECUTE_HANDLER", L"EX_CONTINUE_SEARCH", 
				L"EX_CONTINUE_EXECUTION"});
			this->tryexptcombo->Location = System::Drawing::Point(400, 110);
			this->tryexptcombo->Name = L"tryexptcombo";
			this->tryexptcombo->Size = System::Drawing::Size(136, 21);
			this->tryexptcombo->TabIndex = 5;
			// 
			// vectorexhdbtn
			// 
			this->vectorexhdbtn->Location = System::Drawing::Point(319, 195);
			this->vectorexhdbtn->Name = L"vectorexhdbtn";
			this->vectorexhdbtn->Size = System::Drawing::Size(72, 23);
			this->vectorexhdbtn->TabIndex = 6;
			this->vectorexhdbtn->Text = L"VectorExHd";
			this->vectorexhdbtn->UseVisualStyleBackColor = true;
			this->vectorexhdbtn->Click += gcnew System::EventHandler(this, &Form1::vectorexhdbtn_Click);
			// 
			// executebtn
			// 
			this->executebtn->Location = System::Drawing::Point(319, 252);
			this->executebtn->Name = L"executebtn";
			this->executebtn->Size = System::Drawing::Size(72, 23);
			this->executebtn->TabIndex = 7;
			this->executebtn->Text = L"Execute";
			this->executebtn->UseVisualStyleBackColor = true;
			this->executebtn->Click += gcnew System::EventHandler(this, &Form1::executebtn_Click);
			// 
			// exitbtn
			// 
			this->exitbtn->Location = System::Drawing::Point(475, 252);
			this->exitbtn->Name = L"exitbtn";
			this->exitbtn->Size = System::Drawing::Size(72, 23);
			this->exitbtn->TabIndex = 8;
			this->exitbtn->Text = L"Exit";
			this->exitbtn->UseVisualStyleBackColor = true;
			this->exitbtn->Click += gcnew System::EventHandler(this, &Form1::exitbtn_Click);
			// 
			// unhanexfilcombo
			// 
			this->unhanexfilcombo->FormattingEnabled = true;
			this->unhanexfilcombo->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"EX_EXECUTE_HANDLER", L"EX_CONTINUE_SEARCH", 
				L"EX_CONTINUE_EXECUTION"});
			this->unhanexfilcombo->Location = System::Drawing::Point(400, 167);
			this->unhanexfilcombo->Name = L"unhanexfilcombo";
			this->unhanexfilcombo->Size = System::Drawing::Size(136, 21);
			this->unhanexfilcombo->TabIndex = 9;
			// 
			// vectorchhdbtn
			// 
			this->vectorchhdbtn->Location = System::Drawing::Point(319, 223);
			this->vectorchhdbtn->Name = L"vectorchhdbtn";
			this->vectorchhdbtn->Size = System::Drawing::Size(72, 23);
			this->vectorchhdbtn->TabIndex = 10;
			this->vectorchhdbtn->Text = L"VectorCnHd";
			this->vectorchhdbtn->UseVisualStyleBackColor = true;
			this->vectorchhdbtn->Click += gcnew System::EventHandler(this, &Form1::vectorchhdbtn_Click);
			// 
			// outputlist
			// 
			this->outputlist->FormattingEnabled = true;
			this->outputlist->HorizontalScrollbar = true;
			this->outputlist->Location = System::Drawing::Point(12, 167);
			this->outputlist->Name = L"outputlist";
			this->outputlist->ScrollAlwaysVisible = true;
			this->outputlist->Size = System::Drawing::Size(286, 108);
			this->outputlist->TabIndex = 11;
			// 
			// TitleLbl
			// 
			this->TitleLbl->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->TitleLbl->Location = System::Drawing::Point(54, 9);
			this->TitleLbl->Name = L"TitleLbl";
			this->TitleLbl->Size = System::Drawing::Size(258, 23);
			this->TitleLbl->TabIndex = 12;
			this->TitleLbl->Text = L"Structured Exception Handling";
			// 
			// exceptionbtn
			// 
			this->exceptionbtn->Location = System::Drawing::Point(319, 50);
			this->exceptionbtn->Name = L"exceptionbtn";
			this->exceptionbtn->Size = System::Drawing::Size(72, 23);
			this->exceptionbtn->TabIndex = 13;
			this->exceptionbtn->Text = L"exception";
			this->exceptionbtn->UseVisualStyleBackColor = true;
			this->exceptionbtn->Click += gcnew System::EventHandler(this, &Form1::exceptionbtn_Click);
			// 
			// clearbtn
			// 
			this->clearbtn->Location = System::Drawing::Point(397, 252);
			this->clearbtn->Name = L"clearbtn";
			this->clearbtn->Size = System::Drawing::Size(72, 23);
			this->clearbtn->TabIndex = 14;
			this->clearbtn->Text = L"Clear";
			this->clearbtn->UseVisualStyleBackColor = true;
			this->clearbtn->Click += gcnew System::EventHandler(this, &Form1::clearbtn_Click);
			// 
			// priorityLbl1
			// 
			this->priorityLbl1->Location = System::Drawing::Point(400, 200);
			this->priorityLbl1->Name = L"priorityLbl1";
			this->priorityLbl1->Size = System::Drawing::Size(47, 23);
			this->priorityLbl1->TabIndex = 15;
			this->priorityLbl1->Text = L"Handler";
			// 
			// priorityLbl2
			// 
			this->priorityLbl2->Location = System::Drawing::Point(400, 225);
			this->priorityLbl2->Name = L"priorityLbl2";
			this->priorityLbl2->Size = System::Drawing::Size(47, 23);
			this->priorityLbl2->TabIndex = 16;
			this->priorityLbl2->Text = L"Handler";
			// 
			// vexprtycombo
			// 
			this->vexprtycombo->FormattingEnabled = true;
			this->vexprtycombo->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"0", L"1"});
			this->vexprtycombo->Location = System::Drawing::Point(449, 197);
			this->vexprtycombo->Name = L"vexprtycombo";
			this->vexprtycombo->Size = System::Drawing::Size(87, 21);
			this->vexprtycombo->TabIndex = 17;
			// 
			// vcnprtycombo
			// 
			this->vcnprtycombo->FormattingEnabled = true;
			this->vcnprtycombo->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"0", L"1"});
			this->vcnprtycombo->Location = System::Drawing::Point(449, 226);
			this->vcnprtycombo->Name = L"vcnprtycombo";
			this->vcnprtycombo->Size = System::Drawing::Size(87, 21);
			this->vcnprtycombo->TabIndex = 18;
			// 
			// notelbl
			// 
			this->notelbl->Location = System::Drawing::Point(12, 288);
			this->notelbl->Name = L"notelbl";
			this->notelbl->Size = System::Drawing::Size(528, 18);
			this->notelbl->TabIndex = 19;
			this->notelbl->Text = L"Note: The input blocks are executed in bottom up manner.";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(555, 315);
			this->Controls->Add(this->notelbl);
			this->Controls->Add(this->vcnprtycombo);
			this->Controls->Add(this->vexprtycombo);
			this->Controls->Add(this->priorityLbl2);
			this->Controls->Add(this->priorityLbl1);
			this->Controls->Add(this->clearbtn);
			this->Controls->Add(this->exceptionbtn);
			this->Controls->Add(this->TitleLbl);
			this->Controls->Add(this->outputlist);
			this->Controls->Add(this->vectorchhdbtn);
			this->Controls->Add(this->unhanexfilcombo);
			this->Controls->Add(this->exitbtn);
			this->Controls->Add(this->executebtn);
			this->Controls->Add(this->vectorexhdbtn);
			this->Controls->Add(this->tryexptcombo);
			this->Controls->Add(this->unhanexfilbtn);
			this->Controls->Add(this->_tryfinallybtn);
			this->Controls->Add(this->_tryexceptbtn);
			this->Controls->Add(this->trycatchbtn);
			this->Controls->Add(this->inputlist);
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->Text = L"SEH Tool";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);

		}
#pragma endregion


	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }

    private: System::Void trycatchbtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 inputlist->Items->Add("try catch block");
				 AddFunction(trycatch);
		 }

	private: System::Void _tryexceptbtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 String^ str = "__try __except block (" + tryexptcombo->Text + ")";
				 inputlist->Items->Add(str);

				 if(tryexptcombo->Text->Equals("EX_EXECUTE_HANDLER"))
				   AddFunction(__try__except_EX_EXECUTE_HANDLER);
				 else if(tryexptcombo->Text->Equals("EX_CONTINUE_SEARCH"))
				   AddFunction(__try__except_EX_CONTINUE_SEARCH);
				 else if(tryexptcombo->Text->Equals("EX_CONTINUE_EXECUTION"))
				   AddFunction(__try__except_EX_CONTINUE_EXECUTION);
		 }

    private: System::Void exceptionbtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 inputlist->Items->Add("exception");
				 AddFunction(exception);
		 }

	private: System::Void _tryfinallybtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 inputlist->Items->Add("__try __finally block");
				 AddFunction(__try__finally);
		 }

	private: System::Void unhanexfilbtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 String^ str = "UnhandledExceptionFilter Block (" + unhanexfilcombo->Text + ")";
				 inputlist->Items->Add(str);

				 if(unhanexfilcombo->Text->Equals("EX_EXECUTE_HANDLER"))
				   AddFunction(UnhandledExceptionFilter_EXHD);
				 else if(unhanexfilcombo->Text->Equals("EX_CONTINUE_SEARCH"))
				   AddFunction(UnhandledExceptionFilter_EXCS);
				 else if(unhanexfilcombo->Text->Equals("EX_CONTINUE_EXECUTION"))
				   AddFunction(UnhandledExceptionFilter_EXCE);
		 }

	private: System::Void vectorexhdbtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 String^ str = "Vectored Exception Handling (" + vexprtycombo->Text + ")";
				 inputlist->Items->Add(str);

				 if(vexprtycombo->Text->Equals("0"))
				 AddFunction(VectoredExceptionHandling, 0);
				 else
				 AddFunction(VectoredExceptionHandling, 1);
		 }

	private: System::Void vectorchhdbtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 String^ str = "Vectored Continue Handling (" + vcnprtycombo->Text + ")";
				 inputlist->Items->Add(str);

				 if(vcnprtycombo->Text->Equals("0"))
				 AddFunction(VectoredContinueHandling, 0);
				 else
				 AddFunction(VectoredContinueHandling, 1);
		 }

	private: System::Void executebtn_Click(System::Object^  sender, System::EventArgs^  e) {

			outputlist->Items->Clear();  // clear the output
			int size = ExeFunction();  // execute the function and get size of the output vector
			int i = 0;
			wchar_t* text = NULL;

			// Dump all the output text from the output vector to the list
			for(i = 0; i < size; i++) {
			 text = getOutputText(i);

			 if(text != NULL) {
		      outputlist->Items->Add(gcnew String(text));
			 }
		    }
	}

    private: System::Void exitbtn_Click(System::Object^  sender, System::EventArgs^  e) {

  	   this->Close();          // close the dialog
	   this->~Form1();   // call the destructor
	   delete this;            // delete the object
	}

    private: System::Void clearbtn_Click(System::Object^  sender, System::EventArgs^  e) {

	    // clear all buffers
		inputlist->Items->Clear();
		outputlist->Items->Clear();
		flushBuffer();
    }
};
}

