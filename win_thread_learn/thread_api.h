#include <windows.h>

// pthread api
// http://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html

typedef HANDLE pthread_mutex_t;
typedef struct {
  HANDLE signal;
  HANDLE broadcast;
} pthread_cond_t;
typedef DWORD pthread_t;

int pthread_mutex_init(pthread_mutex_t* mutex);
int pthread_mutex_destroy(pthread_mutex_t* mutex);
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);

int pthread_cond_init(pthread_cond_t* cond);
int pthread_cond_destroy(pthread_cond_t* cond);
int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex);
int pthread_cond_signal(pthread_cond_t* cond);
int pthread_cond_broadcast(pthread_cond_t* cond);

typedef void* (*thread_func_t)(void*);
// Create a thread and return it's handle
long thread_start(thread_func_t func, void* data);

void set_thread_name(DWORD dwThreadID, const char* thread_name);
void set_current_thread_name(const char* thread_name);