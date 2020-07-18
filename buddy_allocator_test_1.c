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
   printf("\n TEST 1 \n \n");
  //1 we see if we have enough memory for the buffers
  int req_size=BuddyAllocator_calcSize(BUDDY_LEVELS);
  printf("Dimensione richiesta per inizializzare: %d/BUFFER_SIZE \n", req_size);

  //2 we initialize the allocator
  printf("\n Init... ");
  BuddyAllocator_init(&alloc, BUDDY_LEVELS,
                      buffer,
                      BUFFER_SIZE,
                      memory,
                      MIN_BUCKET_SIZE);
  printf("DONE\n");

  int test[NUM_TEST]={374,12,66,33,67,1,1};
  void* test_indirizzi[NUM_TEST];
  for(int i=0;i<NUM_TEST;i++){
    void* res=BuddyAllocator_malloc(&alloc,test[i]);
    if(res){
        test_indirizzi[i]=res;
        printf("Ho allocato %d nel blocco %p\n",test[i],res);                
    }
    else{
        test_indirizzi[i]=res;
        printf("Non ho potuto allocare %d byte\n",test[i]);
    }
    printf("\n");
    BitMap_idx_occupati(&alloc.bitmap);
  }

  BitMap_idx_occupati(&alloc.bitmap);

  printf("\n\n\n LIBERO MEMORIA ALLOCATA \n\n");
  
  for(int i=0;i<NUM_TEST;i++){
    if(test_indirizzi[NUM_TEST-1-i]){
        BuddyAllocator_free(&alloc,test_indirizzi[NUM_TEST-1-i]);
        BitMap_idx_occupati(&alloc.bitmap);
    }
    else{
        printf("Risultato della malloc era NULL,quindi non posso fare free\n");
        //BuddyAllocator_free(&alloc,test_indirizzi[NUM_TEST-1-i]);
    }
  }

  assert(BitMap_is_empty(&alloc.bitmap));
}
