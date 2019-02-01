#include "StdAfx.h"
#include <strsafe.h>
#include "TrayIcon.h"

namespace DuiLib {
    CTrayIcon::CTrayIcon(void) {
        memset(&m_trayData, 0, sizeof(m_trayData));
        m_bEnabled = false;
        m_bVisible = false;
        m_hWnd = NULL;
        m_uMessage = UIMSG_TRAYICON;
    }

    CTrayIcon::~CTrayIcon(void) {
        DeleteTrayIcon();
    }

    void CTrayIcon::CreateTrayIcon( HWND _RecvHwnd, UINT _IconIDResource, LPCTSTR _ToolTipText, UINT _Message) {
        if(!_RecvHwnd || _IconIDResource <= 0 ) {
            return;
        }

        if(_Message != 0) m_uMessage = _Message;

        m_hIcon = LoadIcon(CPaintManagerUI::GetInstance(), MAKEINTRESOURCE(_IconIDResource));
        m_trayData.cbSize = sizeof(NOTIFYICONDATA);
        m_trayData.hWnd  = _RecvHwnd;
        m_trayData.uID   = _IconIDResource;
        m_trayData.hIcon = m_hIcon;
        m_trayData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
        m_trayData.uCallbackMessage = m_uMessage;

        if(_ToolTipText) _tcscpy(m_trayData.szTip, _ToolTipText);

        Shell_NotifyIcon(NIM_ADD, &m_trayData);
        m_bEnabled = true;
    }

    void CTrayIcon::DeleteTrayIcon() {
        Shell_NotifyIcon(NIM_DELETE, &m_trayData);
        m_bEnabled = false;
        m_bVisible = false;
        m_hWnd = NULL;
        m_uMessage = UIMSG_TRAYICON;
    }

    bool CTrayIcon::SetTooltipText( LPCTSTR _ToolTipText ) {
        if (_ToolTipText) {
#if (NTDDI_VERSION < NTDDI_WIN2K)
            StringCchCopy(m_trayData.szTip, 64, _ToolTipText);
#else
            StringCchCopy(m_trayData.szTip, 128, _ToolTipText);
#endif
        }

        if (!m_bEnabled) 
            return FALSE;

        m_trayData.uFlags = NIF_TIP;
        return Shell_NotifyIcon(NIM_MODIFY, &m_trayData) == TRUE;
    }

    bool CTrayIcon::SetTooltipText( UINT _IDResource ) {
#if (NTDDI_VERSION < NTDDI_WIN2K)
        TCHAR mbuf[64] = { 0 };
        LoadString(CPaintManagerUI::GetInstance(), _IDResource, mbuf, 64);
        return SetTooltipText(mbuf);
#else
        TCHAR mbuf[128] = { 0 };
        LoadString(CPaintManagerUI::GetInstance(), _IDResource, mbuf, 128);
        return SetTooltipText(mbuf);
#endif
    }

    base::String CTrayIcon::GetTooltipText() const {
        return m_trayData.szTip;
    }

    bool CTrayIcon::SetIcon( HICON _Hicon ) {
        if(_Hicon) m_hIcon = _Hicon;

        m_trayData.uFlags = NIF_ICON;
        m_trayData.hIcon = _Hicon;

        if (!m_bEnabled) 
            return FALSE;

        return Shell_NotifyIcon(NIM_MODIFY, &m_trayData) == TRUE;
    }

    bool CTrayIcon::SetIcon( LPCTSTR _IconFile ) {
        HICON hIcon = LoadIcon(CPaintManagerUI::GetInstance(), _IconFile);
        return SetIcon(hIcon);
    }

    bool CTrayIcon::SetIcon( UINT _IDResource ) {
        HICON hIcon = LoadIcon(CPaintManagerUI::GetInstance(), MAKEINTRESOURCE(_IDResource));
        return SetIcon(hIcon);
    }

    HICON CTrayIcon::GetIcon() const {
        return m_trayData.hIcon;
    }

    void CTrayIcon::SetHideIcon() {
        if (IsVisible()) {
            SetIcon((HICON)NULL);
            m_bVisible = TRUE;
        }
    }

    void CTrayIcon::SetShowIcon() {
        if (!IsVisible()) {
            SetIcon(m_hIcon);
            m_bVisible = FALSE;
        }
    }

    void CTrayIcon::RemoveIcon() {
        m_trayData.uFlags = 0;
        Shell_NotifyIcon(NIM_DELETE, &m_trayData);
        m_bEnabled = FALSE;
    }
}
