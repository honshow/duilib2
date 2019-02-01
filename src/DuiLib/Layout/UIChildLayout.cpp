#include "StdAfx.h"
#include "UIChildLayout.h"

namespace DuiLib {
    IMPLEMENT_DUICONTROL(CChildLayoutUI)

    CChildLayoutUI::CChildLayoutUI() {

    }

    void CChildLayoutUI::Init() {
        if (!m_pstrXMLFile.IsEmpty()) {
            CDialogBuilder builder;
            CContainerUI *pChildWindow = static_cast<CContainerUI *>(builder.Create(m_pstrXMLFile.GetDataPointer(), (UINT)0, NULL, m_pManager));

            if (pChildWindow) {
                this->Add(pChildWindow);
            } else {
                this->RemoveAll();
            }
        }
    }

    void CChildLayoutUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue ) {
        if( _tcsicmp(pstrName, _T("xmlfile")) == 0 )
            SetChildLayoutXML(pstrValue);
        else
            CContainerUI::SetAttribute(pstrName, pstrValue);
    }

    void CChildLayoutUI::SetChildLayoutXML( base::String pXML ) {
        m_pstrXMLFile = pXML;
    }

    base::String CChildLayoutUI::GetChildLayoutXML() {
        return m_pstrXMLFile;
    }

    LPVOID CChildLayoutUI::GetInterface( LPCTSTR pstrName ) {
        if( _tcsicmp(pstrName, DUI_CTR_CHILDLAYOUT) == 0 ) return static_cast<CChildLayoutUI *>(this);

        return CControlUI::GetInterface(pstrName);
    }

    LPCTSTR CChildLayoutUI::GetClass() const {
        return DUI_CTR_CHILDLAYOUT;
    }
} // namespace DuiLib
