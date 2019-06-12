﻿/*******************************************************************************
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

#include "CefHandler.h"
#include "CefUtil.h"
#include <ShlObj.h>

namespace DuiLib {
	namespace Internal {

		ClientHandlerOsr::ClientHandlerOsr(OsrDelegate *delegate) :
			delegate_(delegate) {

		}

		ClientHandlerOsr::~ClientHandlerOsr() {
			delegate_ = nullptr;
		}

		void ClientHandlerOsr::DetachDelegate() {
			if (!CefCurrentlyOn(TID_UI)) {
				// Execute this method on the UI thread.
				CefPostTask(TID_UI, base::Bind(&ClientHandlerOsr::DetachDelegate, this));
				return;
			}

			DCHECK(delegate_);
			delegate_ = NULL;
		}

		bool ClientHandlerOsr::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) {
			CEF_REQUIRE_UI_THREAD();
			std::string msg_name = message->GetName();

			bool handled = false;
			const int browser_id = browser->GetIdentifier();
			CefRefPtr<CefListValue> value_list = message->GetArgumentList();

			if (msg_name == RENDER_2_BROWSER_JS_CALL_CPP_MSG) {
				handled = true;

				DCHECK(value_list->GetSize() >= 1);
				if (value_list->GetSize() < 1) {
					return handled;
				}

				std::string func_name = value_list->GetString(0);
				if (func_name == JSNotifyCppFunctionName) {
					DCHECK(value_list->GetSize() >= 2);
					if (value_list->GetSize() >= 2) {
						std::string business_name = value_list->GetString(1);

						VARIANT var_ret;
						std::vector<VARIANT> var_list;
						VariantInit(&var_ret);
						CefListValue2VARINATList(value_list, var_list, 2);

						if (delegate_)
							delegate_->OnJSNotify(business_name, var_list);

						// SysFreeString
						for (size_t i = 0; i < var_list.size(); i++) {
							if (var_list[i].vt == VT_BSTR) {
								SysFreeString(var_list[i].bstrVal);
							}
						}
					}
				}
			}

			return handled;
		}

		void ClientHandlerOsr::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) {
			CEF_REQUIRE_UI_THREAD();
		}

		void ClientHandlerOsr::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) {
			CEF_REQUIRE_UI_THREAD();
		}

		void ClientHandlerOsr::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen) {
			CEF_REQUIRE_UI_THREAD();
		}

		static std::wstring GetDownloadPath(const std::wstring &file_name) {
			wchar_t szFolderPath[MAX_PATH];
			std::wstring path;

			// Save the file in the user's "My Documents" folder.
			if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, szFolderPath))) {
				path = szFolderPath;
				path += L"\\" + file_name;
			}

			return path;
		}

		void ClientHandlerOsr::OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback) {
			CEF_REQUIRE_UI_THREAD();
			// Continue the download and show the "Save As" dialog.
			callback->Continue(GetDownloadPath(suggested_name), true);
		}

		void ClientHandlerOsr::OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback) {
			CEF_REQUIRE_UI_THREAD();
		}

		bool ClientHandlerOsr::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, bool* no_javascript_access) {
#if CEFVER == 3626
			CEF_REQUIRE_UI_THREAD();
#endif
			bool load_in_cur_frame = false;
			if (delegate_) {
				load_in_cur_frame = delegate_->OnBeforePopup(target_url.ToString());
			}

			if (load_in_cur_frame) {
				if (target_url.length() > 0)
					frame->LoadURL(target_url);
			}

			// Return true to cancel the popup window.
			return true;
		}

		void ClientHandlerOsr::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
			CEF_REQUIRE_UI_THREAD();
			if (delegate_)
				delegate_->OnAfterCreated(browser);
		}

		void ClientHandlerOsr::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
			CEF_REQUIRE_UI_THREAD();
			if (delegate_)
				delegate_->OnBeforeClose(browser);
		}

		bool ClientHandlerOsr::GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
			CEF_REQUIRE_UI_THREAD();
			if (!delegate_)
				return false;
			return delegate_->GetRootScreenRect(browser, rect);
		}

#if CEFVER == 3626
		void ClientHandlerOsr::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
			CEF_REQUIRE_UI_THREAD();
			if (!delegate_) {
				// Never return an empty rectangle.
				rect.width = rect.height = 1;
				return;
			}
			delegate_->GetViewRect(browser, rect);
		}

		bool ClientHandlerOsr::OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString& message, const CefString& source, int line) {
			CEF_REQUIRE_UI_THREAD();
			// Return true to stop the message from being output to the console.
			return false;
		}

		bool ClientHandlerOsr::OnAutoResize(CefRefPtr<CefBrowser> browser, const CefSize& new_size) {
			CEF_REQUIRE_UI_THREAD();
			//  Return true if the resize was handled or false for default handling.
			return true;
		}

		void ClientHandlerOsr::OnAcceleratedPaint(CefRefPtr<CefBrowser> browser, CefRenderHandler::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, void* share_handle) {
			CEF_REQUIRE_UI_THREAD();
			if (!delegate_)
				return;
			delegate_->OnAcceleratedPaint(browser, type, dirtyRects, share_handle);
		}

		void ClientHandlerOsr::OnImeCompositionRangeChanged(CefRefPtr<CefBrowser> browser, const CefRange& selection_range, const CefRenderHandler::RectList& character_bounds) {
			CEF_REQUIRE_UI_THREAD();
			if (!delegate_)
				return;
			delegate_->OnImeCompositionRangeChanged(browser, selection_range,
				character_bounds);
		}
#else
		bool ClientHandlerOsr::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
			if (!delegate_)
				return false;
			return delegate_->GetViewRect(browser, rect);
		}
#endif

		bool ClientHandlerOsr::GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY) {
			CEF_REQUIRE_UI_THREAD();
			if (!delegate_)
				return false;
			return delegate_->GetScreenPoint(browser, viewX, viewY, screenX, screenY);
		}

		bool ClientHandlerOsr::GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info) {
			CEF_REQUIRE_UI_THREAD();
			if (!delegate_)
				return false;
			return delegate_->GetScreenInfo(browser, screen_info);
		}

		void ClientHandlerOsr::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) {
			CEF_REQUIRE_UI_THREAD();
			if (!delegate_)
				return;
			return delegate_->OnPopupShow(browser, show);
		}

		void ClientHandlerOsr::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) {
			CEF_REQUIRE_UI_THREAD();
			if (!delegate_)
				return;
			return delegate_->OnPopupSize(browser, rect);
		}

		void ClientHandlerOsr::OnPaint(CefRefPtr<CefBrowser> browser, CefRenderHandler::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height) {
			CEF_REQUIRE_UI_THREAD();
			if (!delegate_)
				return;
			delegate_->OnPaint(browser, type, dirtyRects, buffer, width, height);
		}



		void ClientHandlerOsr::OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info) {
			CEF_REQUIRE_UI_THREAD();
			if (!delegate_)
				return;
			delegate_->OnCursorChange(browser, cursor, type, custom_cursor_info);
		}

		void ClientHandlerOsr::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status) {
			CEF_REQUIRE_UI_THREAD();
		}

#if CEFVER == 3626
		bool ClientHandlerOsr::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool user_gesture, bool is_redirect) {
			std::string url = request->GetURL().ToString();
			// TODO
			std::vector<std::string> allow_protocol;
			for (auto item : allow_protocol) {
				if (url.find_first_of(item) == 0)
					return true;
			}
			return false;
		}

#else
		bool ClientHandlerOsr::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect) {
			std::string url = request->GetURL().ToString();
			// TODO
			std::vector<std::string> allow_protocol;
			for (auto item : allow_protocol) {
				if (url.find_first_of(item) == 0)
					return true;
			}
			return false;
		}
#endif

        void ClientHandlerOsr::OnProtocolExecution(CefRefPtr<CefBrowser> browser, const CefString& url, bool& allow_os_execution) {
            allow_os_execution = true;
        }

        bool ClientHandlerOsr::DoClose(CefRefPtr<CefBrowser> browser) {
			CEF_REQUIRE_UI_THREAD();

			if (delegate_)
				delegate_->OnBrowserClosing(browser);

			// Allow the close. For windowed browsers this will result in the OS close event being sent.
			return false;
		}
	}
}