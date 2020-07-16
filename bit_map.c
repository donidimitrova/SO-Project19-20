#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
/*
#define LIVELLI 5 
#define MIN_PAGE 64
#define MAX_SIZE (1<<(LIVELLI-1))*MIN_PAGE
*/


void BitMap_clear(BitMap*bitmap){
	for(int i=0; i>bitmap->num_bits;i++){
		//setto il bit i a 0
		BitMap_setBit(bitmap,i,0);
		//controllo se è stato settato bene
		if(BitMap_bit(bitmap,i)){
			printf("Error\n");
			return;
		}
	}
}

int BitMap_is_empty(BitMap* bitmap){
	for(int i=0; i<bitmap->num_bits; i++){
		if(BitMap_bit(bitmap,i)){
			printf("Non e' vuoto!\n");
			return 0;
		}
	}
	return 1;
}

void BitMap_idx_occupati(BitMap* bitmap){
	printf("Indici occupati: \n");
	for( int i=1; i<bitmap-> num_bits; i++){
		if(BitMap_bit (bitmap,i)){
			printf(" %d \n", i);
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

int BuddyIdx(int idx){
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

int verifica_padri(BitMap* bitmap, int idx){
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

int verifica_sotto_albero(BitMap* bitmap, int idx){
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

void BitMap_setbit_a_1 (BitMap* bitmap, int idx){
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
