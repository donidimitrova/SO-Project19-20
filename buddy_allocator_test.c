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
  printf("Dimensione richiesta per l'inizializzazione: %d/BUFFER_SIZE\n", req_size);

  //2 we initialize the allocator
  printf("init... ");
  BuddyAllocator_init(&alloc, BUDDY_LEVELS,
                      buffer,
                      BUFFER_SIZE,
                      memory,
                      MIN_BUCKET_SIZE);
  printf("DONE\n");

  void* p1=BuddyAllocator_malloc(&alloc, 100);
  BitMap_idx_occupati(&alloc.bitmap);
  void* p2=BuddyAllocator_malloc(&alloc, 100);
  BitMap_idx_occupati(&alloc.bitmap);
  void* p3=BuddyAllocator_malloc(&alloc, 100000);
  BitMap_idx_occupati(&alloc.bitmap);


  printf("p1: %p\n",p1);
  char* p1p=(char*) p1;
  p1p=p1p-8;
  printf("%p\n",p1p);
  char** buddy_ptr=(char**)p1p;
  char* buddy=*buddy_ptr;
  printf("buddy di p1: %p\n",buddy);
  printf("p2: %p\n",p2);
  printf("p3: %p\n",p3);


  BuddyAllocator_free(&alloc, p1);
  BitMap_idx_occupati(&alloc.bitmap);
  BuddyAllocator_free(&alloc, p2);
  BitMap_idx_occupati(&alloc.bitmap);
  BuddyAllocator_free(&alloc, p3);
  BitMap_idx_occupati(&alloc.bitmap);

  assert(BitMap_is_empty(&alloc.bitmap));


  
  
}
