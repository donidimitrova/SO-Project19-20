#include "buddy_allocator.h"
#include <stdio.h>
#include <assert.h>

#define BUFFER_SIZE 102400
#define BUDDY_LEVELS 9
#define MEMORY_SIZE (1024*1024)
#define MIN_BUCKET_SIZE (MEMORY_SIZE>>(BUDDY_LEVELS))

uint8_t buffer[BUFFER_SIZE]; // 100 Kb buffer to handle memory should be enough
char memory[MEMORY_SIZE];

BuddyAllocator alloc;
int main(int argc, char** argv) {

  //1 we see if we have enough memory for the buffers
  int req_size=BuddyAllocator_calcSize(BUDDY_LEVELS);
  printf("size requested for initialization: %d/BUFFER_SIZE\n", req_size);

  //2 we initialize the allocator
  printf("init... ");
  BuddyAllocator_init(&alloc, BUDDY_LEVELS,
                      buffer,
                      BUFFER_SIZE,
                      memory,
                      MIN_BUCKET_SIZE);
  printf("DONE\n");

  void* p1=BuddyAllocator_malloc(&alloc, 100);
  void* p2=BuddyAllocator_malloc(&alloc, 100);
  void* p3=BuddyAllocator_malloc(&alloc, 100000);
  printf("p1: %p\n",p1);
  char* p1p=(char*) p1;
  p1p=p1p-8;
  printf("%p\n",p1p);
  char** buddy_ptr=(char**)p1p;
  char* buddy=*buddy_ptr;
  //char** buddy_p1=(char**) ((char*) p1-8);
  printf("buddy di p1: %p\n",buddy);
  printf("p2: %p\n",p2);
  printf("p3: %p\n",p3);
  //assert(BitMap_is_empty(&alloc.bitmap));
  BuddyAllocator_free(&alloc, p1);
  BuddyAllocator_free(&alloc, p2);
  BuddyAllocator_free(&alloc, p3);
  assert(BitMap_is_empty(&alloc.bitmap));


  
  
}
