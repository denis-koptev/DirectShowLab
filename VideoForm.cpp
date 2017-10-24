#include "VideoForm.h"

#include <dshow.h>

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void Main(array<String ^> ^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    dshow_lab3::VideoForm form;

    Application::Run( % form);
}