#pragma once
#include <stdint.h>

// simple bit array
typedef struct  {
  uint8_t *buffer;  // externally allocated buffer
  int buffer_size;
  int num_bits; 
  //char* memory;
} BitMap;

// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits);

// initializes a bitmap on an external array
void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer);

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap* bit_map, int bit_num, int status);

// inspects the status of the bit bit_num
int BitMap_bit(const BitMap* bit_map, int bit_num);

//returns the level of an index
int levelIdx(int idx);

//returns the buddy of index
int buddyIdx(int idx);

//returns the parent of the index
int parentIdx(int idx);



//ritorna 1 se tutti i padri dell'indice idx sono liberi
//(settati a zero) ritorna 0 altrimenti
int verifica_padri(BitMap* bitmap,int idx);

//ritorna 1 se tutti gli elementi del sottoalbero dell'indice idx
//sono liberi (settati a 0), ritorna 0 altrimenti
int verifica_sotto_albero(BitMap* bitmap,int idx);

//setta il bit corrispondente all'indice idx a 1
//e inoltre se il buddy è occupato(1) setta a 1 anche il padre 
void BitMap_setBit_a_1(BitMap* bitmap,int idx);

int startIdx(int idx);


void Bitmap_clear(BitMap* bitmap);

int BitMap_is_empty(BitMap* bitmap);

void BitMap_idx_occupati(BitMap* bitmap);
