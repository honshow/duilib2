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

#ifndef PPX_CEF_CEF_UTIL_H__
#define PPX_CEF_CEF_UTIL_H__
#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <comdef.h>
#include "include/cef_v8.h"
#include "include/internal/cef_types_wrappers.h"

namespace DuiLib {
	namespace Internal {

		const char BROWSER_2_RENDER_CPP_CALL_JS_MSG[] = "BROWSER_2_RENDER_CPP_CALL_JS_MSG";
		const char RENDER_2_BROWSER_JS_CALL_CPP_MSG[] = "RENDER_2_BROWSER_JS_CALL_CPP_MSG";
		const char RENDER_2_BROWSER_CONTEXT_CREATED_MSG[] = "RENDER_2_BROWSER_CONTEXT_CREATED_MSG";
		const char JSNotifyCppFunctionName[] = "JSNotifyCppFunc";
		const char RegisterCppNotifyJSFunctionName[] = "RegisterCppNotifyCppFunc";

		// Returns the current time in microseconds.
		uint64_t GetTimeNow();

		// Set the window's user data pointer.
		void SetUserDataPtr(HWND hWnd, void* ptr);

		// Return the window's user data pointer.
		template <typename T>
		T GetUserDataPtr(HWND hWnd) {
			return reinterpret_cast<T>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

		// Set the window's window procedure pointer and return the old value.
		WNDPROC SetWndProcPtr(HWND hWnd, WNDPROC wndProc);

		int GetCefMouseModifiers(WPARAM wparam);
		int GetCefKeyboardModifiers(WPARAM wparam, LPARAM lparam);
		bool IsKeyDown(WPARAM wparam);

		// Returns the device scale factor. For example, 200% display scaling will return 2.0.
		float GetDeviceScaleFactor();


		// Convert value from logical coordinates to device coordinates.
		int LogicalToDevice(int value, float device_scale_factor);
		CefRect LogicalToDevice(const CefRect& value, float device_scale_factor);

		// Convert value from device coordinates to logical coordinates.
		int DeviceToLogical(int value, float device_scale_factor);
		void DeviceToLogical(CefMouseEvent& value, float device_scale_factor);


		void VARINAT2V8Value(VARIANT * pvRet, CefRefPtr<CefV8Value>& retval);
		void V8Value2VARINAT(CefRefPtr<CefV8Value>& retval, VARIANT * pvRet);
		void VARINATList2V8ValueList(std::vector<VARIANT>& vListArg, CefV8ValueList& arguments);
		void V8ValueList2VARINATList(CefV8ValueList arguments, std::vector<VARIANT>& vListArg);
		void VARINATList2CefListValue(const std::vector<VARIANT>& vListArg, CefRefPtr<CefListValue>& arguments, size_t cef_list_init_index = 0);
		void CefListValue2VARINATList(CefRefPtr<CefListValue>& arguments, std::vector<VARIANT>& vListArg, size_t cef_list_begin_index = 0);

		CefRefPtr<CefV8Value> CefValueToCefV8Value(CefRefPtr<CefValue> value);
		CefRefPtr<CefValue> CefV8ValueToCefValue(CefRefPtr<CefV8Value> value);
	}
} 

#endif  // PPX_CEF_CEF_UTIL_H__
