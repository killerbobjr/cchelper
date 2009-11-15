// genkeytool.cpp : main project file.

#include "stdafx.h"
#include "frmMain.h"
#include "frmGetKey.h"
#include "MouseHook.h"

using namespace genkeytool;

//
//// Creates a  message filter.
//ref class MouseHookMessageFilter: public System::Windows::Forms::IMessageFilter
//{
//public: static frmGetKey ^ g_frmGetKey ;
//
//public:
//	virtual bool PreFilterMessage( Message % m )
//	{
//		if( m.Msg == MouseHookMessageFilter::UWM_DRAGEEND )
//		{
//			CMouseHook::StopHook();
//			g_frmGetKey->MouseDragEnd((HWND)m.WParam.ToInt32());
//
//			Console::WriteLine( "Processing the messages : {0}", m.Msg );
//			return true;
//		}
//
//		return false;
//	}
//
//};

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	//// Create the main window and run it
	//MouseHookMessageFilter ^ mf = gcnew MouseHookMessageFilter();

	//Application::AddMessageFilter(mf);

	//MouseHookMessageFilter::g_frmGetKey  = gcnew frmGetKey() ;

	Application::Run(gcnew frmGetKey());


	return 0;
}
