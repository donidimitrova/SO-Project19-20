#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define BUDDY_LEVELS 5

int main(){
	//calcolo quanti bit sono necessari alla bitmap
	int bit=1<<BUDDY_LEVELS;
	printf("Number bit: %d \n", bit);
	
	//calcolo il numero di byte
	int byte= BitMap_getBytes(bit);
	printf("Number byte: %d\n", byte);
	
	printf("\n");
	
	// inizializzo un buffer quanto la memoria necessaria 
	uint8_t* buffer= (uint8_t*) malloc(byte*sizeof(uint8_t));
	//setto i byte a 0
	for( int i=0; i<byte; i++){
		buffer[i]=0;
		printf("Byte: %d valore: %d\n", i, buffer[i]);
	}
	
	printf("\n");
	
	//inizializzo una bitmap
	BitMap* bitmap=(BitMap*)malloc(sizeof(BitMap));
	bitmap-> buffer=buffer;
	bitmap->num_bits=bit;
	bitmap->buffer_size=byte;
	//metto tutti 1 a tutti i bit
	for(int i=1; i<bit;i++){
		printf("Imposto il bit: %d a 1\n", i);
		BitMap_setBit(bitmap,i,1);
		//quale byte sto toccando
		//printf("Sono nel livello dove ho dimensione blocco %d  \n", buffer[i/8]);
		if(!BitMap_bit(bitmap,i)){
			printf("Error\n");
		}
	}
	
	
	for ( int i=1; i<bit;i++){
		BitMap_setBit(bitmap, i,0);
		if(BitMap_bit(bitmap,i)){
			printf("Error\n");
		}
	}
	
	printf("\n");
	for( int i=0; i<byte;i++){
		printf(" Livello %d ho dimensione del blocco %d \n", i, buffer[i]);
	
	}
	
	free(bitmap);
	free(buffer);
	return 0;
}
