#include <stdio.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "buddy_allocator.h"


//restituisce il livello della dimensione
int level_from_size(BuddyAllocator* alloc,int size){
  int mem_size=(1<<alloc->num_levels)*alloc->min_bucket_size;
  int minimo=(int) (mem_size/alloc->min_bucket_size);
  //printf("%d\n",minimo);
  int res=(int) (mem_size/(size));
  //printf("%d\n",res);
  if(res > minimo)
    res=minimo;
  int level=(int) floor(log2(res));
  //printf("%d\n",level);
  return level; 
}

//ritorna indirizzo dell'indice idx
char* from_idx_to_memory_address(BuddyAllocator* alloc,int idx){
  
  int level=levelIdx(idx);
  int dimensione_level=(1<<(alloc->num_levels-level))*alloc->min_bucket_size;
  //printf("Level: %d Idx: %d Dimensione: %d\n",level,idx,dimensione_level);
  
  //caso particolare alloco tutto lo spazio sul livello 0 e non avrò un buddy
  if(idx==1){
    printf("CASO PARTICOLARE: Sto allocando al livello 0 e non ho un buddy\n");
    char* address=alloc->memory;
    //non avendo un buddy mi scrivo l'indirizzo stesso al posto del buddy
    void** start= (void**) address;
    *start=address;
    return (char*) (address);

  }
  
  int indice_buddy=buddyIdx(idx);
  //printf("IDX: %d\tIDX_BUDDY: %d\n",idx,buddyIdx);
  char* address=alloc->memory+(idx-(1<<levelIdx(idx)))*dimensione_level;
  char* address_buddy=alloc->memory+(indice_buddy-(1<<levelIdx(indice_buddy)))*dimensione_level;
  printf("Indirizzo Buddy: %p\t Indirizzo BUDDY: %p\n",address,address_buddy);
  int differenza=address-address_buddy;
  if(differenza<0)
    differenza*=-1;
  //printf("Differenza: %d\n",differenza);

  void** start= (void**) address;
  *start=address_buddy;
  printf("Indirizzo: %p\n",start);
  //printf("address: %p\n",address);
  //*address=address_buddy;
  printf("Indirizzo BUDDY: %p\n",*start);
  //printf("address_BUDDY: %p\n",*address);
  return (char*) (address);
}





// computes the size in bytes for the allocator
int BuddyAllocator_calcSize(int num_levels) {
  int num_bits=1<<(num_levels+1); // maximum number of allocations, used to determine the max list items
  return num_bits/8 + ((num_bits%8)!=0);
}





void BuddyAllocator_init(BuddyAllocator* alloc,
                         int num_levels,
                         uint8_t* buffer,
                         int buffer_size,
                         char* memory,
                         int min_bucket_size){

  // we need room also for level 0
  alloc->num_levels=num_levels;
  alloc->memory=memory;
  alloc->min_bucket_size=min_bucket_size;
  assert (num_levels<MAX_LEVELS);
  // we need enough memory to handle internal structures
  assert (buffer_size>=BuddyAllocator_calcSize(num_levels));

  int num_bits=1<<(num_levels+1); // maximum number of allocations
  int num_bytes=num_bits/8 + ((num_bits%8)!=0);

  printf("BUDDY INITIALIZING\n");
  printf("\tLivelli: %d", num_levels);
  printf("\tSize Bitmap: %d bytes\n", num_bytes);
  printf("\tBucket size:%d\n", min_bucket_size);
  printf("\tManaged memory %d bytes\n", (1<<num_levels)*min_bucket_size);
  
  // the buffer for the list starts where the bitmap ends
  BitMap_init(&alloc->bitmap,num_bits,buffer);

};




int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level){
  //printf("Livello: %d\n",level);
  int dimensione_level=(1<<(alloc->num_levels-level))*alloc->min_bucket_size;
  printf("Livello: %d\t Dimensione Livello: %d\n",level,dimensione_level);
  
  int start_idx=1<<level;
  int end_idx=(1<<(level+1))-1;
  //printf("StartIdx: %d\tEndIdx: %d\n",start_idx,end_idx);
  int cont=start_idx;
  while(cont<=end_idx){
    int ultimo_livello_occupato=0;
    if(!BitMap_bit(&alloc->bitmap,cont)){
      ultimo_livello_occupato=verifica_padri(&alloc->bitmap,cont);
      if(ultimo_livello_occupato==0 && verifica_sotto_albero(&alloc->bitmap,cont)){
        BitMap_setBit_a_1(&alloc->bitmap,cont);
        printf("Ho allocato all'indice: %d\n",cont);
        //char* start=bitmap->memory+(cont-(1<<levelIdx(cont)))*dimensione_level;
        //return (void*) start;
        return cont;
        
      }
      else{
        if(ultimo_livello_occupato!=0){
          //Ho trovato l'ultimo 1 a ultimo_livello_occupato livelli di distanza
		  //int nodi_nel_livello=end_idx-start_idx+1;
		  //int blocchi_nel_livello=nodi_nel_livello/(1<<ultimo_livello_occupato);
		  //Nel livello level ho nodi_nel_livello nodi
          int sono_nel_blocco=(cont-start_idx)/(1<<ultimo_livello_occupato);
          //cont=start_idx+(1<<ultimo_livello_occupato);
          cont=(sono_nel_blocco+1)*(1<<ultimo_livello_occupato)+start_idx;
        }
        else{
          cont++;
        }
      }    
    }
    else{
      //printf("bitmap[%d]=0 e aumento il cont di 1\n",cont);
      cont++;
    }
    
    
  }
  //non ho trovato il blocco 
  return 0;

}

void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {
  printf("Allocazione di dimensione %d\n",size);
  
  int level=level_from_size(alloc,size+8);
  int cont=BuddyAllocator_getBuddy(alloc,level);
  if(!cont)
    return NULL;
  return (void*) (from_idx_to_memory_address(alloc,cont)+8);
}



void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc, int indice){
  
  assert(BitMap_bit(&alloc->bitmap,indice));


  //Verifico se l'indirizzo passato alla free è stato restituito
  //da una malloc oppure è stato modificato
  assert(verifica_sotto_albero(&alloc->bitmap,indice));
  BitMap_setBit_a_0(&alloc->bitmap,indice);
  
  printf("\n");
}



void BuddyAllocator_free(BuddyAllocator* alloc, void* address){
  if(!address){
    assert(0);
    return;
  }
  
  char* address_originale=(char*) address;
  address_originale=address_originale-8;
  void** buddy_ptr=(void**) address_originale;
  void* buddy=*buddy_ptr;
  
  printf("Indirizzo: %p\tIndirizzo buddy: %p\n",address_originale,buddy);
  
  char* address_buddy=(char*) buddy;
  int differenza=address_originale-address_buddy;
  
  if(differenza==0){
    
    printf("Sono nel caso particolare che devo liberare l'indice 1 al livello 0\n");
    
    BuddyAllocator_releaseBuddy(alloc,1);
    return;
  }

  if(differenza<0)
    differenza*=-1;
  
  //printf("Differenza: %d\n",differenza);
  int level=level_from_size(alloc,differenza);
  int dimensione_level=(1<<(alloc->num_levels-level))*alloc->min_bucket_size;
  
  //printf("Differenza: %d\tDimensione_livello: %d\n",differenza,dimensione_level);
  printf("Sono al livello %d di dimensione %d\n",level,dimensione_level);
  
  assert(differenza==dimensione_level);
  
  int indice=(address_originale-alloc->memory)/dimensione_level+(1<<level);
  int indice_buddy=(address_buddy-alloc->memory)/dimensione_level+(1<<level);
  
  assert(indice_buddy==buddyIdx(indice));
  
  printf("Indice: %d\tIndice Buddy: %d\n",indice,indice_buddy);
  
  BuddyAllocator_releaseBuddy(alloc,indice);
  

}




