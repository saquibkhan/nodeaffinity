// Author: Saquib Khan
// Email: saquibofficial@gmail.com

#include <iostream>
#include <napi.h>

#if defined(_POSIX_C_SOURCE)
#include <sched.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)

#endif

using namespace Napi;
using namespace std;

Number getAffinity(const CallbackInfo &info) {
  auto env = info.Env();

  if (info.Length() > 0) {
    TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
  }

  long ulCpuMask = -1;

#if _POSIX_C_SOURCE && !__APPLE__
  pid_t p = 0;
  int ret;
  cpu_set_t curMask;
  CPU_ZERO(&curMask);

  ret = sched_getaffinity(p, sizeof(cpu_set_t), &curMask);
  // printf(" sched_getaffinity = %d, cur_mask = %08lx\n", ret, cur_mask);
  if (ret != -1) {
    ulCpuMask = 0;
    for (unsigned int i = 0; i < sizeof(cpu_set_t); i++) {
      if (CPU_ISSET_S(i, sizeof(cpu_set_t), &curMask)) {
        ulCpuMask = ulCpuMask | (1 << i);
      }
    }
  }
#endif

#if _WIN32
  HANDLE hCurrentProc, hDupCurrentProc;
  DWORD_PTR dwpSysAffinityMask, dwpProcAffinityMask;

  // Obtain a usable handle of the current process
  hCurrentProc = GetCurrentProcess();
  DuplicateHandle(hCurrentProc, hCurrentProc, hCurrentProc, &hDupCurrentProc, 0, FALSE, DUPLICATE_SAME_ACCESS);

  // Get the old affinity mask
  GetProcessAffinityMask(hDupCurrentProc, &dwpProcAffinityMask, &dwpSysAffinityMask);

  ulCpuMask = dwpProcAffinityMask;

  CloseHandle(hDupCurrentProc);
#endif

  return Number::New(env, ulCpuMask);
}

Number setAffinity(const CallbackInfo &info) {
  auto env = info.Env();

  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
  }

  long ulCpuMask = info[0].As<Number>().Int64Value();

#if _POSIX_C_SOURCE && !__APPLE__
  pid_t p = 0;
  int ret;
  cpu_set_t newMask;
  CPU_ZERO(&newMask);

  for (unsigned int i = 0; i < sizeof(cpu_set_t); i++) {
    if (ulCpuMask & 1 << i) {
      CPU_SET(i, &newMask);
    }
  }

  ret = sched_setaffinity(p, sizeof(cpu_set_t), &newMask);
  if (ret == -1) {
    ulCpuMask = -1;
  }
  // printf(" sched_getaffinity = %d, cur_mask = %08lx\n", ret, cur_mask);
#endif

#if _WIN32
  HANDLE hCurrentProc;
  DWORD_PTR dwpProcAffinityMask = ulCpuMask;

  // Obtain a usable handle of the current process
  hCurrentProc = GetCurrentProcess();

  // Get the old affinity mask
  BOOL bRet = SetProcessAffinityMask(hCurrentProc, dwpProcAffinityMask);
  if (bRet == false) {
    ulCpuMask = -1;
  }

  CloseHandle(hCurrentProc);
#endif

  return Number::New(env, ulCpuMask);
}

Object Init(Env env, Object exports) {
  exports["getAffinity"] = Function::New(env, &getAffinity);
  exports["setAffinity"] = Function::New(env, &setAffinity);

  return exports;
}

NODE_API_MODULE(nodeaffinity, Init)