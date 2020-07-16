#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define BUDDY_LEVELS 4

#define MIN_PAGE 64

#define MEMORY_SIZE (1<<(BUDDY_LEVELS))*MIN_PAGE

char memory[MEMORY_SIZE];



int main(){
    
    /* 
    printf("MEMORY: %d\n",MEMORY_SIZE);


    int bit=1<<(BUDDY_LEVELS+1);

    char* start=memory;
    char* end=memory+bit;
    printf("Indirizzo memoria start: %p\n",memory);
    printf("Indirizzo memoria end: %p\n",memory+bit);
    
    printf("Differenza: %ld\n",end-start);

    printf("Numero di bit %d\n",bit);
    int byte=BitMap_getBytes(bit);
    
    printf("numero di bytes %d\n",byte);
    
    uint8_t* buffer=(uint8_t*) malloc(byte*sizeof(uint8_t));
    for(int i=0;i<byte;i++){
        buffer[i]=0;
        printf("%d %d\n",i,buffer[i]);
    }

    BitMap* bitmap=(BitMap*) malloc(sizeof(BitMap));
    bitmap->buffer=buffer;
    bitmap->num_bits=bit;
    bitmap->buffer_size=byte;
    bitmap->memory=memory;

     
    for(int i=1;i<bit;i++){
        //printf("setto il bit %d a 1\n",i);
        BitMap_setBit(bitmap,i,1);
        //printf("%d\n",buffer[i/8]);
        if(!BitMap_bit(bitmap,i)){
            printf("Ci dovrebbe essere stato un errore\n");
        } 
    }

    

    for(int i=0;i<byte;i++){
        printf("%d %d\n",i,buffer[i]);

    }


    for(int i=1;i<bit;i++){
        //printf("setto il bit %d a 0\n",i);
        BitMap_setBit(bitmap,i,0);
        //printf("%d\n",buffer[i/8]);
        if(BitMap_bit(bitmap,i)){
            printf("Ci dovrebbe essere stato un errore\n");
        }   
    }

    

    for(int i=0;i<byte;i++){
        printf("%d %d\n",i,buffer[i]);
    }

    int test[7]={374,12,66,33,67,1,1};
    void* test_indirizzi[7];
    for(int i=0;i<7;i++){
        void* res=alloc(bitmap,test[i]);
        if(res){
            test_indirizzi[i]=res;
            printf("ho allocato %d nel blocco %p\n",test[i],res);
            //char** ind=(char**) res;
            //printf("Indirizzo del buddy %p\n",*(ind));
                        
        }
        else{
            printf("Non ho potuto allocare %d byte\n",test[i]);
        }
    }
    printf("\n\n\n\n\n\n\n");
    for(int i=0;i<7;i++){
        free_bitmap(bitmap,test_indirizzi[6-i]);
    }

    for(int i=1;i<bit;i++){
        
        if(BitMap_bit(bitmap,i)){
            printf("%d\n",i);
        }   
    }

    for(int i=0;i<byte;i++){
        printf("%d %d\n",i,buffer[i]);
    }
    */

    /*

    free(bitmap,24);
    free(bitmap,31);



    for(int i=1;i<bit;i++){
        
        if(BitMap_bit(bitmap,i)){
            printf("%d\n",i);
        }   
    }

    for(int i=0;i<byte;i++){
        printf("%d %d\n",i,buffer[i]);
    }

    




    free(bitmap);
    free(buffer);
    */

   /*
   printf("livello: %d\n",level_from_size(bitmap,511));
   printf("livello: %d\n",level_from_size(bitmap,32));
   printf("livello: %d\n",level_from_size(bitmap,1));
   printf("livello: %d\n",level_from_size(bitmap,64));
   printf("livello: %d\n",level_from_size(bitmap,100));
   printf("livello: %d\n",level_from_size(bitmap,63));
   printf("livello: %d\n",level_from_size(bitmap,65));
   printf("livello: %d\n",level_from_size(bitmap,200));
   printf("livello: %d\n",level_from_size(bitmap,257));
    */
   /*
   free(bitmap);
   free(buffer);
    */
    return(0);
}