#include "thread_api.h"
#include <process.h>


int pthread_mutex_init(pthread_mutex_t* mutex) {
  *mutex = CreateMutex(NULL, FALSE, NULL);
  return (*mutex == NULL) ? 0 : -1;
}

int pthread_mutex_destroy(pthread_mutex_t* mutex) {
  return CloseHandle(*mutex) ? 0 : -1;
}

int pthread_mutex_lock(pthread_mutex_t* mutex) {
  return (::WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0) ? 0 : -1;
}

int pthread_mutex_unlock(pthread_mutex_t* mutex) {
  return (::ReleaseMutex(*mutex) != 0) ? 0 : -1;
}

int pthread_cond_init(pthread_cond_t* cond) {
  cond->signal = ::CreateEvent(NULL, FALSE, FALSE, NULL);
  cond->broadcast = ::CreateEvent(NULL, TRUE, FALSE, NULL);
  return (cond->signal != NULL && cond->broadcast != NULL) ? 0 : -1;
}

int pthread_cond_destroy(pthread_cond_t* cond) {
  return (::CloseHandle(cond->signal) && ::CloseHandle(cond->broadcast)) ? 0 : -1;
}

int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
  HANDLE handles[] = { cond->signal, cond->broadcast };
  // Same as pthread_unlock(mutex)
  ::ReleaseMutex(*mutex);
  // Wake up after either pthread_cond_signal or pthread_broadcast is called
  ::WaitForMultipleObjects(2, handles, FALSE, INFINITE);
  // Same as pthread_mutex_lock(*mutex)
  return (::WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0) ? 0 : -1;
}

int pthread_cond_signal(pthread_cond_t* cond) {
  return ::SetEvent(cond->signal) == 0 ? -1 : 0;
}

int pthread_broadcast(pthread_cond_t* cond) {
  return ::PulseEvent(cond->broadcast) == 0 ? -1 : 0;
}

long thread_start(thread_func_t func, void* data) {
  return _beginthread(reinterpret_cast<void (__cdecl *)(void *)>(func), 0, data);
}

const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push, 8)
typedef struct tagTHREADNAME_INFO {
  DWORD dwType; // Must be 0x1000.
  LPCSTR szName; // Pointer to name (in user addr space).
  DWORD dwThreadID; // Thread ID (-1=caller thread).
  DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void set_thread_name(DWORD dwThreadID, const char* thread_name) {
  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = thread_name;
  info.dwThreadID = dwThreadID;
  info.dwFlags = 0;

  __try {
    RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
  } __except(EXCEPTION_EXECUTE_HANDLER) { }
}

void set_current_thread_name(const char* thread_name) {
  set_thread_name(GetCurrentThreadId(), thread_name);
}