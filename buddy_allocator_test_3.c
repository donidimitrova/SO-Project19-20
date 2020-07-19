#include "buddy_allocator.h"
#include <stdio.h>
#include <assert.h>

#define BUFFER_SIZE 1024
#define BUDDY_LEVELS 4
#define MEMORY_SIZE 1024
#define MIN_BUCKET_SIZE (MEMORY_SIZE>>(BUDDY_LEVELS))
#define NUM_TEST 7

uint8_t buffer[BUFFER_SIZE]; // 100 Kb buffer to handle memory should be enough
char memory[MEMORY_SIZE];

BuddyAllocator alloc;
int main(int argc, char** argv) {
   printf("\n TEST 3 \n \n");
  //1 we see if we have enough memory for the buffers
  int req_size=BuddyAllocator_calcSize(BUDDY_LEVELS);
  printf("Dimensione richiesta per inizializzare: %d/BUFFER_SIZE \n", req_size);


  //2 we initialize the allocator
  printf("\nInit... ");
  BuddyAllocator_init(&alloc, BUDDY_LEVELS,
                      buffer,
                      BUFFER_SIZE,
                      memory,
                      MIN_BUCKET_SIZE);
  printf("DONE\n");

  void* p1=BuddyAllocator_malloc(&alloc,200); 
  BitMap_idx_occupati(&alloc.bitmap);
  void* p2=BuddyAllocator_malloc(&alloc,12);
  BitMap_idx_occupati(&alloc.bitmap);
  void* p3=BuddyAllocator_malloc(&alloc,300);
  BitMap_idx_occupati(&alloc.bitmap);
  
  BuddyAllocator_free(&alloc,p2);
  BitMap_idx_occupati(&alloc.bitmap);

  void* p4=BuddyAllocator_malloc(&alloc,200);
  BitMap_idx_occupati(&alloc.bitmap);

  BuddyAllocator_free(&alloc,p1);
  BitMap_idx_occupati(&alloc.bitmap);

  BuddyAllocator_free(&alloc,p3);
  BitMap_idx_occupati(&alloc.bitmap);

  void* p5=BuddyAllocator_malloc(&alloc,68);
  BitMap_idx_occupati(&alloc.bitmap);

  BuddyAllocator_free(&alloc,p4);
  BitMap_idx_occupati(&alloc.bitmap);

  BuddyAllocator_free(&alloc,p5);
  BitMap_idx_occupati(&alloc.bitmap);
 

  
  assert(BitMap_is_empty(&alloc.bitmap));
}
