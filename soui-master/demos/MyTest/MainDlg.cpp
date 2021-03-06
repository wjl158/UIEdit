// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"

	
#ifdef DWMBLUR	//win7毛玻璃开关
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
#endif
	
CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
}

CMainDlg::~CMainDlg()
{
	shellNotifyIcon.Hide();
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	#ifdef DWMBLUR	//win7毛玻璃开关
	MARGINS mar = {5,5,30,5};
	DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
	#endif

	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	shellNotifyIcon.Create(m_hWnd,GETRESPROVIDER->LoadIcon(_T("ICON_LOGO"),16));
	shellNotifyIcon.Show();
	btn_test = FindChildByName2<SMyButton>(L"btn_mytest");
    btn_new = FindChildByName2<SButton>(L"btn_new");
	return 0;
}


//TODO:消息映射
void CMainDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

void CMainDlg::OnBtnTest()
{


}

void CMainDlg::OnBtnNew()
{
	//btn_test->Move(10, 10, 100, 100);
    SwndLayout *layout = btn_test->GetLayout();
	layout->pos[2].pit = PIT_SIZE;
	layout->pos[2].nPos = 50; 

	layout->pos[3].pit = PIT_SIZE;
	layout->pos[3].nPos = 50; 
	btn_test->GetParent()->UpdateChildrenPosition();


}

void CMainDlg::OnBtnInput()
{
	SDlgCreatePro fileDlg(_T("layout:XML_dlg_pro"));
	fileDlg.DoModal(m_hWnd);
	btn_test->SetWindowTextW(fileDlg.m_strinput);

}

void CMainDlg::OnBtnEdit()
{
	SWindow *pContainer = FindChildByName2<SWindow>(L"wnd_layout");
	SASSERT(pContainer);
	//remove all children at first.
	SWindow *pChild = pContainer->GetWindow(GSW_FIRSTCHILD);
	while(pChild)
	{
		SWindow *pNext = pChild->GetWindow(GSW_NEXTSIBLING);
		pChild->DestroyWindow();
		pChild = pNext;
	}
	//using SWindow::CreateChildren to Create Children described in the input xml string.

	wchar_t *s = L"<window pos=\"[10,10,200,200\" name=\"wnd_layout\" colorBkgnd=\"#FF0000\">解决方案管理器</window>";

	pContainer->CreateChildren(s);

}




#include <helper/smenu.h>
LRESULT CMainDlg::OnIconNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/)
{
	switch (lParam)
	{
	case  WM_RBUTTONDOWN:
	{
            //显示右键菜单
            SMenu menu;
            menu.LoadMenu(_T("menu_tray"),_T("smenu"));
            POINT pt;
            GetCursorPos(&pt);
            menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);
	}break;
	case WM_LBUTTONDOWN:
	{
		//显示/隐藏主界面
		if (IsIconic())
		   ShowWindow(SW_SHOWNORMAL);
		else
		   ShowWindow(SW_MINIMIZE);
	}break;
	default:
		break;
	}
	return S_OK;
}

//演示如何响应菜单事件
void CMainDlg::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (uNotifyCode == 0)
	{
		switch (nID)
		{
		case 6:
			PostMessage(WM_CLOSE);
			break;
		default:
			break;
		}
	}
}
