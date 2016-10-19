#include "CrashHandler.h"

CrashHandler::CrashHandler(QObject* /*parent*/)
{
    ::SetUnhandledExceptionFilter(CrashHandler::OurCrashHandler);
}

void CrashHandler::InvokeSimulatedCrash()
{
    int* dummyData = nullptr;
    *dummyData = 0;
}

// http://stackoverflow.com/questions/5028781/how-to-write-a-sample-code-that-will-crash-and-produce-dump-file
// http://www.codeproject.com/Articles/708670/Part-Windows-Debugging-Techniques-Debugging-Appl
void CrashHandler::make_minidump(EXCEPTION_POINTERS* e)
{
    auto hDbgHelp = LoadLibrary(L"dbghelp");
    if(hDbgHelp == nullptr)
        return;
    auto pMiniDumpWriteDump = (decltype(&MiniDumpWriteDump))GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
    if(pMiniDumpWriteDump == nullptr)
        return;

    char name[MAX_PATH];
    {
        auto nameEnd = name + GetModuleFileNameA(GetModuleHandleA(0), name, MAX_PATH);
        SYSTEMTIME t;
        GetSystemTime(&t);
        sprintf(nameEnd - strlen(".exe"),
            "_%4d%02d%02d_%02d%02d%02d.dmp",
            t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
    }

    auto hFile = CreateFileA(name, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(hFile == INVALID_HANDLE_VALUE)
        return;

    MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
    exceptionInfo.ThreadId = GetCurrentThreadId();
    exceptionInfo.ExceptionPointers = e;
    exceptionInfo.ClientPointers = FALSE;

    /*auto dumped =*/ pMiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        MINIDUMP_TYPE(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory),
        e ? &exceptionInfo : nullptr,
        nullptr,
        nullptr);

    CloseHandle(hFile);

    return;
}

LONG WINAPI CrashHandler::OurCrashHandler(EXCEPTION_POINTERS *exceptionInfo)
{
    std::wostringstream oss;
    oss << L"Exception Code: ";
    oss << std::hex << exceptionInfo->ExceptionRecord->ExceptionCode;
    std::wstring msg = oss.str();

    std::wofstream log("tombola.crashlog.txt", std::wofstream::out);
    log << msg;
    log.close();

    make_minidump(exceptionInfo);

    return /*g_showCrashDialog ? EXCEPTION_CONTINUE_SEARCH :*/ EXCEPTION_EXECUTE_HANDLER;
}

