/*******************************************************************************
* Copyright (C) 2018 - 2020, winsoft666, <winsoft666@outlook.com>.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
*
* Expect bugs
*
* Please use and enjoy. Please let me know of any bugs/improvements
* that you have found/implemented and I will fix/incorporate them into this
* file.
*******************************************************************************/
#include "CefGloablContext.h"


CefGlobalContext::CefGlobalContext() :
	m_bCefCache(false)
{

}

CefGlobalContext::~CefGlobalContext() {

}

void CefGlobalContext::SetWithCef(bool b) {
	m_bWithCef = b;
}


bool CefGlobalContext::GetWithCef() const {
	return m_bWithCef;
}

void CefGlobalContext::SetCefApp(CefRefPtr<CefApp> app) {
	m_pCefApp = app;
}

CefRefPtr<CefApp> CefGlobalContext::GetCefApp() const {
	return m_pCefApp;
}

void CefGlobalContext::SetCefCache(bool b) {
	m_bCefCache = b;
}

bool CefGlobalContext::GetCefCache() const {
	return m_bCefCache;
}