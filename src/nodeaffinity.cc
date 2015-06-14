// urlParser.cc
//Author: Saquib Khan
//Email: saquibofficial@gmail.com

#include <node.h>
#include <v8.h>
#include <iostream>

#if defined(V8_OS_POSIX)
 #include <sched.h>
 #include <unistd.h>
#elif defined (V8_OS_WIN)
  #include <windows.h>
#elif defined (V8_OS_MACOSX)

#endif


// #include <mach_init.h>
// #include <thread_policy.h>
// #include <sched.h>
// #include <pthread.h>


using namespace v8;
using namespace std;

void getAffinity(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() > 0) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid number of arguments")));
  }

  long ulCpuMask = -1;

#if V8_OS_POSIX && !V8_OS_MACOSX
  pid_t p = 0;
  int ret;
  cpu_set_t curMask;
  CPU_ZERO(&curMask);

  ret = sched_getaffinity(p, sizeof(cpu_set_t), &curMask);
  //printf(" sched_getaffinity = %d, cur_mask = %08lx\n", ret, cur_mask);
if (ret != -1)
{
  ulCpuMask = 0;
  for ( int i = 0; i < sizeof(cpu_set_t); i++ )
  {
    if ( CPU_ISSET_S(i, sizeof(cpu_set_t), &curMask) )
    {
      ulCpuMask = ulCpuMask | (1<<i);
    }
  }
}
#endif

#if V8_OS_WIN
  HANDLE hCurrentProc, hDupCurrentProc;
  DWORD_PTR dwpSysAffinityMask, dwpProcAffinityMask;

  // Obtain a usable handle of the current process
  hCurrentProc = GetCurrentProcess();
  DuplicateHandle(hCurrentProc, hCurrentProc, hCurrentProc,
                  &hDupCurrentProc, 0, FALSE, DUPLICATE_SAME_ACCESS);

  // Get the old affinity mask
  GetProcessAffinityMask(hDupCurrentProc,
                         &dwpProcAffinityMask, &dwpSysAffinityMask);

  ulCpuMask = dwpProcAffinityMask;

  CloseHandle(hDupCurrentProc);
#endif

  Local<Number> num = Number::New(isolate, ulCpuMask);

  args.GetReturnValue().Set(num);
}

void setAffinity(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() != 1 && !args[0]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid argument")));
  }

  long ulCpuMask = args[0]->NumberValue();

#if V8_OS_POSIX && !V8_OS_MACOSX
  pid_t p = 0;
  int ret;
  cpu_set_t newMask;
  CPU_ZERO(&newMask);

  for ( int i = 0; i < sizeof(cpu_set_t); i++ )
  {
    if (ulCpuMask & 1<<i)
    {
      CPU_SET(i, &newMask);
    }
  }

  ret = sched_setaffinity(p, sizeof(cpu_set_t), &newMask);
  if (ret == -1)
  {
    ulCpuMask = -1; 
  }
  //printf(" sched_getaffinity = %d, cur_mask = %08lx\n", ret, cur_mask);
#endif

#if V8_OS_WIN
  HANDLE hCurrentProc;
  DWORD_PTR dwpProcAffinityMask = ulCpuMask;

  // Obtain a usable handle of the current process
  hCurrentProc = GetCurrentProcess();

  // Get the old affinity mask
  BOOL bRet = SetProcessAffinityMask(hCurrentProc, dwpProcAffinityMask);
  if (bRet == false)
  {
    ulCpuMask = -1;
  }

  CloseHandle(hCurrentProc);
#endif

  Local<Number> num = Number::New(isolate, ulCpuMask);
  args.GetReturnValue().Set(num);
}

void Init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "getAffinity", getAffinity);
  NODE_SET_METHOD(exports, "setAffinity", setAffinity);
}

NODE_MODULE(nodeaffinity, Init)