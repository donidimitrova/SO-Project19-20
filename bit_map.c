#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define LIVELLI 9

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

int levelIdx(size_t idx){
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

int verifica(BitMap* bitmap, int idx){
	if(idx>= bitmap->num_bits) return 1;
	if(BitMap_bit(bitmap,idx)==1) return 0;
	else return verifica(bitmap, idx*2) && verifica(bitmap, idx*2+1);
}

int prova_nodo(BitMap* bitmap, int idx){
	if(!BitMap_bit(bitmap, idx) && verifica(bitmap, idx)){
		printf("Imposto l'indice %d a 1\n", idx);
		BitMap_setBit(bitmap, idx, 1);
		if(BitMap_bit(bitmap, getBuddy(idx))){
			printf("Padre occupato, setto il parent a 1");
			idx=parentIdx(idx);
			BitMap_setBit(bitmap,idx,1);
			
			while(idx!=0 && BitMap_bit(bitmap, getBuddy(idx))){
				printf("Buddy parent occupato, setto a 1: %d\n",idx);
				idx=parentIdx(idx);
				BitMap_setBit(bitmap,idx,1);
			}
		}
		return 1;
	}
	else return 0;

}

