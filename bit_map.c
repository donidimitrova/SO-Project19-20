#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define LIVELLI 5 
#define MIN_PAGE 64
#define MAX_SIZE (1<<(LIVELLI-1))*MIN_PAGE

int level_from_size(BitMap* bitmap, int size){
	int minimo=(int)(MAX_SIZE/MIN_PAGE);
	int res=(int)(MAX_SIZE/size);
	if(res>minimo) res=minimo;
	int level= (int)floor(log2(res));
	return level;
}

// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits){
  return bits/8 + ((bits%8)!=0);
}

// initializes a bitmap on an external array
void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer){
  bit_map->buffer=buffer;
  bit_map->num_bits=num_bits;
  bit_map->buffer_size=BitMap_getBytes(num_bits);
}

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap* bit_map, int bit_num, int status){
  // get byte
  int byte_num=bit_num>>3;
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte=7- (byte_num&0x07);
  //printf("Change bit %d in byte %d \n", bit_in_byte, byte_num);
  if (status) {
    bit_map->buffer[byte_num] |= (1<<bit_in_byte);
  } else {
    bit_map->buffer[byte_num] &= ~(1<<bit_in_byte);
  }
}

// inspects the status of the bit bit_num
int BitMap_bit(const BitMap* bit_map, int bit_num){
  int byte_num=bit_num>>3; 
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte=7- (byte_num&0x07);
  return (bit_map->buffer[byte_num] & (1<<bit_in_byte))!=0;
}

int levelIdx(int idx){
	return (int)floor(log2(idx));
}

int getBuddy(int idx){
	//faccio verifica se pari o dispari
	if(idx&0x1) return idx-1;
	return idx+1;
}

int parentIdx(int idx){
	return idx/2;
}

int startIdx(int idx){
	return (idx-(1<<levelIdx(idx)));
}

int verifica_sopra(BitMap* bitmap, int idx){
	int padre=idx/2;
	while(padre!=0){
		if(BitMap_bit(bitmap,padre)){
			//indice padre occupato
			return 0;
		}
		padre=padre/2;
	}
	//indice libero
	return 1;
}

int verifica_sotto(BitMap* bitmap, int idx){
	int cont=idx*2;
	int level=1;
	while(cont< bitmap->num_bits){
		int start_idx=cont;
		int end_idx=cont+((1<<(level))-1);
		for(int i=start_idx; i<=end_idx; i++){
			if(BitMap_bit(bitmap,i)) return 0;
		}
		cont=cont*2;
		level++;
	}
	return 1;
}

void setbita1(BitMap* bitmap, int idx){
	BitMap_setBit(bitmap, idx,1);
	int padre=idx;
	while(padre!=0){
		if(BitMap_bit(bitmap, getBuddy(padre))){
			BitMap_setBit(bitmap, padre/2,1); //setto il padre a 1
		}
		else{
			return;
		}
		padre=padre/2;
	}
	return;
}

void* from_idx_to_memory_address(BitMap* bitmap, int idx){
	int level=levelIdx(idx);
	int dimensione_level=(1<<(LIVELLI-level-1))*MIN_PAGE;
	int buddyIdx=getBuddy(idx);
	char* address=bitmap->memory+(idx-(1<<levelIdx(idx)))*dimensione_level;
	char* address_buddy=bitmap-> memory+(buddyIdx-(1<<levelIdx(buddyIdx)))*dimensione_level;
	printf("Indirizzo: %p\t Indirizzo Buddy: %p \n", address, address_buddy);
	int differenza=abs(address-address_buddy);
	
	void** start=(void**) address;
	*start=address_buddy;
	printf("Indirizzo: %p \n", start);
	printf("Indirizzo Buddy: %p \n", *start);
	return (void*) (address+8);
}

void* alloc(BitMap* bitmap,int size){
  int level=level_from_size(bitmap,size+8); //Livello
  int dimensione_level=(1<<(LIVELLI-level-1))*MIN_PAGE; //dimensione livello
  int start_idx=1<<level;
  int end_idx=(1<<(level+1))-1;
  int cont=start_idx;
  while(cont <=end_idx){
	  if(!BitMap_bit(bitmap, cont)){
		  if(verifica_sopra(bitmap,cont) && verifica_sotto(bitmap, cont)){
			  setbita1(bitmap, cont);
			  printf( " Ho allocato %d all'inidice: %d\n", size, cont);
			  return from_idx_to_memory_address(bitmap, cont);
		  }
	  }
	  cont++;
  }
  return NULL;
}

void free_bitmap(BitMap* bitmap,void* address){
  char* address_originale=(char*) address;
  address_originale=address_originale-8;
  void** buddy_ptr=(void**) address_originale;
  void* buddy=*buddy_ptr;
  printf("Indirizzo: %p\tIndirizzo_buddy: %p\n",address_originale,buddy);
  char* address_buddy=(char*) buddy;
  int differenza=abs(address_originale-address_buddy);
  //printf("Differenza: %d\n",differenza);
  int level=level_from_size(bitmap,differenza);
  int dimensione_level=(1<<(LIVELLI-level-1))*MIN_PAGE;
  assert(differenza==dimensione_level);
  printf("Sono al livello %d di dimensione %d\n",level,dimensione_level);
  int indice=(address_originale-bitmap->memory)/dimensione_level+(1<<level);
  int indice_buddy=(address_buddy-bitmap->memory)/dimensione_level+(1<<level);
  assert(indice_buddy==getBuddy(indice));
  printf("Indice: %d\tIndice Buddy: %d\n",indice,indice_buddy);
  assert(BitMap_bit(bitmap,indice));
  BitMap_setBit(bitmap,indice,0);
  while(indice!=0){
    if(!BitMap_bit(bitmap,getBuddy(indice))){
      printf("Setto il padre di %d a 0, il padre ha indice %d\n",indice,indice/2);
      BitMap_setBit(bitmap,indice/2,0);
    }
    else{
      printf("\n");
      return;
    }
    indice=indice/2;  
  }
  printf("\n");
}
