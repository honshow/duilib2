#include "StdAfx.h"
#include <exdisp.h>
#include <comdef.h>
#include "ControlEx.h"
#include "resource.h"
#include <ShellAPI.h>
#include "SkinFrame.h"
#include "MainWnd.h"
#include "PopWnd.h"
#include "resource.h"
#include "TranparentWnd.h"

#define _CRTDBG_MAP_ALLOC
#include<stdlib.h>
#include<crtdbg.h>


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	if (!DuiLib::CefProcessTypeCheck(hInstance)) {
		return 0;
	}

	_CrtDumpMemoryLeaks();
	
    DuiLib::Initialize(hInstance, true, false);

    CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
    CPaintManagerUI::SetResourceZip(IDR_ZIPRES1);
    CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);

    REGIST_DUICONTROL(CCircleProgressUI);
    REGIST_DUICONTROL(CMyComboUI);
    REGIST_DUICONTROL(CChartViewUI);
    REGIST_DUICONTROL(CWndUI);


	//CMainWnd* pMainWnd = new CMainWnd();
 //   if (pMainWnd) {
 //       pMainWnd->Create(NULL, _T("DuiLib Demo"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
 //       pMainWnd->CenterWindow();
 //   }

	CTranparentWnd * pDlg = new CTranparentWnd();
	pDlg->Create(NULL, TEXT("Transparent"), UI_WNDSTYLE_FRAME, 0, 0, 0, 600, 400);
	pDlg->CenterWindow();
	pDlg->ShowWindow();

    CPaintManagerUI::MessageLoop();

    //if (pMainWnd) {
    //    delete pMainWnd;
    //    pMainWnd = NULL;
    //}


    DuiLib::UnInitialize();

	return 0;
}