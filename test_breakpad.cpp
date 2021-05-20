#include "client/windows/handler/exception_handler.h"
#include "client/windows/sender/crash_report_sender.h"
#include "client/windows./crash_generation/crash_generation_server.h"
#include "rea.h"
#include <WinInet.h>
#include <QDir>
#include <QQmlApplicationEngine>

void sendReport(const wchar_t* aID, const wchar_t* aPath){
    google_breakpad::CrashReportSender sd(L"crash/crash.checkpoint");
    std::map<std::wstring, std::wstring> prms;
    prms.insert(std::pair<std::wstring, std::wstring>(L"id", aID));
    prms.insert(std::pair<std::wstring, std::wstring>(L"type", L"crash"));
    std::map<std::wstring, std::wstring> fls;
    fls.insert(std::pair<std::wstring, std::wstring>(L"data0", aPath));
    fls.insert(std::pair<std::wstring, std::wstring>(L"data1", L".version"));
    std::wstring ret;
    auto tmp = sd.SendCrashReport(L"http://127.0.0.1:3000/test/upload", prms, fls, &ret); //type: multi/form-data
    std::cout << tmp << std::endl;
}

static bool dumpCallback(const wchar_t *dump_path, const wchar_t *id,
  void *context, EXCEPTION_POINTERS *exinfo,
  MDRawAssertionInfo *assertion,
  bool succeeded) {
  if (succeeded) {
      std::wstring pth = dump_path;
      pth += L"/";
      pth += id;
      pth += L".dmp";
      printf("dump path is %ws\n", pth.data());

      sendReport(id, pth.data());
  }
  else {
    printf("dump failed\n");
  }
  fflush(stdout);
  return succeeded;
}

static void connectedCallback(void* context, const google_breakpad::ClientInfo* client_info){
    std::cout << "breakpad client is connected!" << std::endl;
}

static void dumpedCallback(void* context, const google_breakpad::ClientInfo* client_info, const std::wstring* file_path){
    std::cout << "lalala" << std::endl;
    std::cout << file_path->data() << std::endl;
    //sendReport(file_path->data());
}

static void exitedCallback(void* context, const google_breakpad::ClientInfo* client_info){
    std::cout << "lll" << std::endl;
}

static void uploadCallback(void* context, const DWORD crash_id){
    std::cout << "fff" << std::endl;
}

static rea::regPip<QQmlApplicationEngine*> test_breakpad([](rea::stream<QQmlApplicationEngine*>* aInput){
    QDir().mkdir("crash");
    std::wstring pth = L"./crash", pip = L"\\\\.\\pipe\\crash0";
    bool single = true;
   /* static google_breakpad::CrashGenerationServer sv(pip, NULL, connectedCallback, NULL, dumpedCallback, NULL, exitedCallback, NULL, uploadCallback, NULL, true, &pth);
    bool single = !sv.Start();
    if (single)
        std::cout << "breakpad reporter start failed!" << std::endl;
    */
    static google_breakpad::ExceptionHandler eh(pth, NULL, dumpCallback, NULL, google_breakpad::ExceptionHandler::HANDLER_ALL,MiniDumpNormal, single ? NULL : pip.data(), NULL);
}, QJsonObject(), "initRea");

static void CrashFunction() {
  int *i = reinterpret_cast<int*>(0x45);
  *i = 5;  // crash!
}

static rea::regPip<double> test_breakpad2([](rea::stream<double>* aInput){
    CrashFunction();
}, rea::Json("name", "breakpad", "external", "qml"));
