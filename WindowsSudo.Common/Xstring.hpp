#pragma once
#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <utility>
#include <stdexcept>
#include <system_error>

namespace std {

    struct Xstring_extension {};

#if defined(_UNICODE) || defined(UNICODE)
    class Xstring final : public wstring {
    public:

        using wstring::wstring;
        using wstring::operator=;

        Xstring(const wstring& wstr) : wstring(wstr) {}

        Xstring(wstring&& wstr) : wstring(std::move(wstr)) {}

        Xstring(const Xstring& Other) = default;

        Xstring(Xstring&& Other) = default;

        Xstring& operator=(const Xstring& Other) = default;

        Xstring& operator=(Xstring&& Other) = default;

        Xstring(Xstring_extension, const string& str, DWORD CodePage = CP_ACP) {
            auto len = MultiByteToWideChar(CodePage, 0, str.c_str(), -1, NULL, 0);
            if (len == 0) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            resize(static_cast<size_t>(len) - 1);

            len = MultiByteToWideChar(CodePage, 0, str.c_str(), -1, data(), len);
            if (len == 0) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }

        Xstring(Xstring_extension, const char* lpstr, DWORD CodePage = CP_ACP) {
            auto len = MultiByteToWideChar(CodePage, 0, lpstr, -1, NULL, 0);
            if (len == 0) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            resize(static_cast<size_t>(len) - 1);

            len = MultiByteToWideChar(CodePage, 0, lpstr, -1, data(), len);
            if (len == 0) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }
#else
    class Xstring final : public string {
    public:

        using string::string;
        using string::operator=;

        Xstring(const string& str) : string(str) {}

        Xstring(string&& str) : string(std::move(str)) {}

        Xstring(const Xstring& Other) = default;

        Xstring(Xstring&& Other) = default;

        Xstring& operator=(const Xstring& Other) = default;

        Xstring& operator=(Xstring && Other) = default;

        Xstring(Xstring_extension, const string& str, DWORD CodePage = CP_ACP) {
            if (CodePage == CP_ACP || CodePage == GetACP()) {
                assign(str);
            } else {
                std::wstring wstr;

                auto len = MultiByteToWideChar(CodePage, 0, str.c_str(), -1, NULL, 0);
                if (len == 0) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                wstr.resize(len - 1);

                len = MultiByteToWideChar(CodePage, 0, str.c_str(), -1, wstr.data(), len);
                if (len == 0) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
                if (len == 0) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                resize(len - 1);

                len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, data(), len, NULL, NULL);
                if (len == 0) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }
            }
        }

        Xstring(Xstring_extension, const char* lpstr, DWORD CodePage = CP_ACP) {
            if (CodePage == CP_ACP || CodePage == GetACP()) {
                assign(lpstr);
            } else {
                std::wstring wstr;

                auto len = MultiByteToWideChar(CodePage, 0, lpstr, -1, NULL, 0);
                if (len == 0) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                wstr.resize(len - 1);

                len = MultiByteToWideChar(CodePage, 0, lpstr, -1, wstr.data(), len);
                if (len == 0) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
                if (len == 0) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                resize(len - 1);

                len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, data(), len, NULL, NULL);
                if (len == 0) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }
            }
        }

        Xstring(Xstring_extension, const wstring& wstr) {
            auto len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
            if (len == 0) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            resize(len - 1);

            len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, data(), len, NULL, NULL);
            if (len == 0) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }

        Xstring(Xstring_extension, const wchar_t* lpwstr) {
            auto len = WideCharToMultiByte(CP_ACP, 0, lpwstr, -1, NULL, 0, NULL, NULL);
            if (len == 0) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            resize(len - 1);

            len = WideCharToMultiByte(CP_ACP, 0, lpwstr, -1, data(), len, NULL, NULL);
            if (len == 0) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }
#endif

        std::string explicit_string(DWORD CodePage = CP_ACP) const {
#if defined(_UNICODE) || defined(UNICODE)
            std::string str;

            auto len = WideCharToMultiByte(CodePage, 0, c_str(), -1, NULL, 0, NULL, NULL);
            if (len == 0) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            str.resize(static_cast<size_t>(len) - 1);

            len = WideCharToMultiByte(CodePage, 0, c_str(), -1, str.data(), len, NULL, NULL);
            if (len == 0) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            return str;
#else
            if (CodePage == CP_ACP || CodePage == GetACP()) {
                return *this;
            } else {
                std::string str;
                std::wstring wstr;

                auto len = MultiByteToWideChar(CP_ACP, 0, c_str(), -1, NULL, 0);
                if (len == 0) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                wstr.resize(len - 1);

                len = MultiByteToWideChar(CP_ACP, 0, c_str(), -1, wstr.data(), len);
                if (len == 0) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                len = WideCharToMultiByte(CodePage, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
                if (len == 0) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                str.resize(len - 1);

                len = WideCharToMultiByte(CodePage, 0, wstr.c_str(), -1, str.data(), len, NULL, NULL);
                if (len == 0) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                return str;
            }
#endif
        }

        std::wstring explicit_wstring() const {
#if defined(_UNICODE) || defined(UNICODE)
            return *this;
#else
            std::wstring wstr;

            auto len = MultiByteToWideChar(CP_ACP, 0, c_str(), -1, NULL, 0);
            if (len == 0) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            wstr.resize(len - 1);

            len = MultiByteToWideChar(CP_ACP, 0, c_str(), -1, wstr.data(), len);
            if (len == 0) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            return wstr;
#endif
        }

        template<typename... __Ts>
        static Xstring format(const Xstring& Format, __Ts&&... Args) {
            Xstring s;
            
            auto len = _sctprintf(Format.c_str(), std::forward<__Ts>(Args)...);
            if (len == -1) {
                throw std::invalid_argument("_sctprintf failed.");
            }

            s.resize(len);

            _sntprintf_s(s.data(), s.length() + 1, _TRUNCATE, Format.c_str(), std::forward<__Ts>(Args)...);

            return s;
        }

        template<typename... __Ts>
        static Xstring format(PCTSTR lpszFormat, __Ts&& ... Args) {
            Xstring s;

            auto len = _sctprintf(lpszFormat, std::forward<__Ts>(Args)...);
            if (len == -1) {
                throw std::invalid_argument("_sctprintf failed.");
            }

            s.resize(len);

            _sntprintf_s(s.data(), s.length() + 1, _TRUNCATE, lpszFormat, std::forward<__Ts>(Args)...);

            return s;
        }
    };

#if defined(UNICODE) || defined(_UNICODE)
    using Xstring_view = wstring_view;
#else
    using Xstring_view = string_view;
#endif

}
