// hello.cc
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <napi.h>
#include <string>
#include <thread>

using namespace Napi;

static void utilSleep(unsigned int ms)
{
#ifdef _WIN32
  Sleep(ms);
#else
  struct timespec ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) * 1000000;
  nanosleep(&ts, NULL);
#endif
}

static volatile int exit_id = 0;

static void actuallyExit(int id, int timeout) {
  utilSleep(timeout);
  if (exit_id != id) {
    return;
  }
  #ifdef _WIN32
    // From SDL:
    // "if you do not know the state of all threads in your process, it is better to call TerminateProcess than ExitProcess"
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ms682658(v=vs.85).aspx
    TerminateProcess(GetCurrentProcess(), 0);
    ExitProcess(0);
  #else
    _exit(0);
  #endif
}

Value exitFunc(const CallbackInfo &info) {
  Env env = info.Env();

  int id = ++exit_id;
  int timeout = 0;
  if (info.Length() == 1) {
    if (!info[0].IsNumber()) {
      TypeError::New(env, "Argument must be a number").ThrowAsJavaScriptException();
      return env.Null();
    }
    timeout = info[0].As<Number>();
  }

  if (!timeout) {
    actuallyExit(id, timeout);
  } else {
    std::thread t1(actuallyExit, id, timeout);
  }

  return env.Null();
}

Value exitCancel(const CallbackInfo &info) {
  Env env = info.Env();
  ++exit_id;
  return env.Null();
}

Object Init(Env env, Object exports) {
  exports.Set(String::New(env, "exit"), Function::New(env, exitFunc));
  exports.Set(String::New(env, "exitCancel"), Function::New(env, exitCancel));
  return exports;
}

NODE_API_MODULE(addon, Init)