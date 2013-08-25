#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

#include "thread_api.h"

const int kMaxBufferSize = 100;

typedef struct {
  pthread_mutex_t mutex;
  pthread_cond_t cond;

  int buffer[kMaxBufferSize];
  int buffer_head;
  int buffer_tail;
  bool buffer_predicate;
} context_t;

const int kProducerNumber = 1;
const int kConsumerNumber = 3;

void consumer_thread(void* data) {
  long thread_id = ::GetCurrentThreadId();
  char thread_name[100];
  sprintf(thread_name, "consumer_%ld", thread_id);
  set_current_thread_name(thread_name);

  context_t* context = reinterpret_cast<context_t*>(data);
  printf("%s: thread start.\n", thread_name);
  
  pthread_mutex_lock(&context->mutex);
  printf("%s: locked mutex.\n", thread_name);

  while (!context->buffer_predicate) {
    printf("%s: predicate not ready, waiting\n", thread_name);
    pthread_cond_wait(&context->cond, &context->mutex);
    printf("%s: waked up by signal.\n", thread_name);

    // Read content from buffer
    assert(context->buffer_head != context->buffer_tail);
    while (context->buffer_head != context->buffer_tail) {
      printf("%s: data read, %d\n", thread_name, context->buffer[context->buffer_head]);
      context->buffer_head = (context->buffer_head + 1) % kMaxBufferSize;
    }
    printf("%s: read finished.\n", thread_name);
  }
  pthread_mutex_unlock(&context->mutex);
}

void producer_thread(void* data) {
  long thread_id = ::GetCurrentThreadId();
  char thread_name[100];
  sprintf(thread_name, "producer_%ld", thread_id);

  context_t* context = reinterpret_cast<context_t*>(data);
  printf("%s: thread start.\n", thread_name);

  // TODO: support multi producer
  while (true) {
    Sleep(rand() % 5000 + 1000);
    int value = rand() % 10000;

    // TODO: check if the buffer is full
    printf("\n%s: put data, %d.\n", thread_name, value);
    context->buffer[context->buffer_tail] = value;
    context->buffer_tail = (context->buffer_tail + 1) % kMaxBufferSize;  
    
    printf("%s: notify consumer.\n", thread_name);
    pthread_mutex_lock(&context->mutex);
    pthread_cond_signal(&context->cond);
    pthread_mutex_unlock(&context->mutex);
  }
}

void test_producer_consumer() {
  context_t* context = new context_t;
  memset(context, 0, sizeof(context_t));
  pthread_cond_init(&context->cond);
  pthread_mutex_init(&context->mutex);

  printf("test_producer_consumer: start.\n");
  for (int i = 0; i < kConsumerNumber; i++) {
    thread_start((thread_func_t)consumer_thread, context);
  }
  for (int i = 0; i < kProducerNumber; i++) {
    thread_start((thread_func_t)producer_thread, context);
  }
  getchar();
  delete context;
  printf("test_producer_consumer%s: end.\n");
}

int main() {
  srand(time(NULL));
  test_producer_consumer();

  return 0;
}
