#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
#define LIVELLI 4
#define MIN_PAGE 64
#define MAX_SIZE (1<<(LIVELLI))*MIN_PAGE
*/



void BitMap_clear(BitMap* bitmap){
  for(int i=1;i<bitmap->num_bits;i++){
    //printf("setto il bit %d a 0\n",i);
    BitMap_setBit(bitmap,i,0);
    //printf("%d\n",buffer[i/8]);
    if(BitMap_bit(bitmap,i)){
      printf("Ci dovrebbe essere stato un errore\n");
      return;
    }   
  }
}

int BitMap_is_empty(BitMap* bitmap){
  for(int i=1;i<bitmap->num_bits;i++){
    if(BitMap_bit(bitmap,i)){
      printf("It's not empty\n");
      return 0;
    }   
  }
  return 1;

}


void BitMap_idx_occupati(BitMap* bitmap){
  printf("Indici occupati:\n");
  for(int i=1;i<bitmap->num_bits;i++){
    if(BitMap_bit(bitmap,i)){
      printf("%d\n",i);
      
    }   
  }

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
  BitMap_clear(bit_map);
}

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap* bit_map, int bit_num, int status){
  // get byte
  int byte_num=bit_num>>3;
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte=7-(bit_num&0x07);
  //printf("devo modificare il bit %d nel byte %d\n",bit_in_byte,byte_num);
  if (status) {
    bit_map->buffer[byte_num] |= (1<<bit_in_byte);
  } else {
    bit_map->buffer[byte_num] &= ~(1<<bit_in_byte);
  }
}

// inspects the status of the bit bit_num
int BitMap_bit(const BitMap* bit_map, int bit_num){
  //printf("Bit num: %d\n",bit_num);
  int byte_num=bit_num>>3; 
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte=7-(bit_num&0x07);
  return (bit_map->buffer[byte_num] & (1<<bit_in_byte))!=0;
}



int levelIdx(int idx){
  return (int)floor(log2(idx));
};

int buddyIdx(int idx){
  if (idx&0x1){		//verifica se pari o dispari	
    return idx-1;
  }
  return idx+1;
}

int parentIdx(int idx){
  return idx/2;
}

int startIdx(int idx){
  return (idx-(1<<levelIdx(idx)));
}


int verifica_padri(BitMap* bitmap,int idx){
  int padre=idx/2;
  while(padre!=0){
    if(BitMap_bit(bitmap,padre)){
      //printf("IDX: %d occupato\n",padre);
      return 0;
    }
    padre=padre/2;
  }
  //printf("IDX: %d libero\n",idx);
  return 1;

}

int verifica_sotto_albero(BitMap* bitmap,int idx){
  int cont=idx*2;
  int level=1;
  while(cont<bitmap->num_bits){
    //int start_idx=cont+(1<<level);
    int start_idx=cont;
    int end_idx=cont+((1<<(level))-1);
    //printf("Level: %d %d\tStartIdx: %d\tEndIdx: %d\n",levelIdx(start_idx),levelIdx(end_idx),start_idx,end_idx);
    for(int i=start_idx;i<=end_idx;i++){
      if(BitMap_bit(bitmap,i)){
        //printf("IDX: %d occupato\n",i);
        return 0;
      }
    }
    cont=cont*2;
    level++;
  }
  //printf("IDX: %d libero\n",idx);
  return 1;  
}




void BitMap_setBit_a_1(BitMap* bitmap,int idx){
  BitMap_setBit(bitmap,idx,1);
  int padre=idx;
  while(padre!=0){
    if(BitMap_bit(bitmap,buddyIdx(padre))){
      //printf("Setto il padre a 1: %d\n", padre/2);
      BitMap_setBit(bitmap,padre/2,1);
    }
    else{
      return;
    }
    padre=padre/2;
  }
  return;
}






