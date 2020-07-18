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



//ritorna 0 se tutti i padri dell'indice idx sono liberi
//altrimenti ritorna il livello relativo alla posizione dell'1 più alto trovato
int verifica_padri(BitMap* bitmap,int idx);

//ritorna 1 se tutti gli elementi del sottoalbero dell'indice idx
//sono liberi (settati a 0), ritorna 0 altrimenti
int verifica_sotto_albero(BitMap* bitmap,int idx);

//setta il bit corrispondente all'indice idx a 1
//e inoltre se il buddy è occupato(1) setta a 1 anche il padre 
void BitMap_setBit_a_1(BitMap* bitmap,int idx);

//setta il bit corrispondente all'indice idx a 0
//setto tutti i padri di idx a 0 fino a che non trovo padre con 0
void BitMap_setBit_a_0(BitMap* bitmap, int idx);
//setta a zero tutti i bit della bitmap
void Bitmap_clear(BitMap* bitmap);

//ritorna 1 se tutti i bit della bitmap sono zero, altrimenti 0
int BitMap_is_empty(BitMap* bitmap);

//stampa tutti gli indici dove bitmap[index]=1
void BitMap_idx_occupati(BitMap* bitmap);
