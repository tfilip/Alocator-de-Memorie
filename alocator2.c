#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0

#define GESTIUNE 3*sizeof(uint32_t)
#define INT_SIZE sizeof(uint32_t)
#define CHAR_SIZE sizeof(unsigned char)


unsigned char *arena;
uint32_t n;
int arena_index;


int initialize(int n){

    arena = calloc(n, CHAR_SIZE);
    arena_index = 0;

    if(arena == NULL){
        return -1;
    }

    return 0;

}

void finalize(){

    free(arena);

}

void dump(){

    uint32_t i;

    for(i=0;i<n;i++){

        if( i%16 == 0){
            printf("%08X\t", i);
        }

        printf("%02X ", *(arena+i));

        if( (i%8 == 0) && (i != 0) ){
            printf(" ");
        }

        if( i%16 == 0 ){
            printf("\n");
        }

    }

    if( (i%16 != 0) || i<=16 )
        printf("\n");

}

void alloc(int size){


    int schimbat, octet;
    

    schimbat=0;
    octet=0;
        //Parcurgere blocuri
    for(int i=arena_index;i<n;){
        
        uint32_t* int_arena=(uint32_t*)(arena+i);

        //printf("CAI PUTERE: %d\n", *(int_arena+1) == 0 && 3*sizeof(uint32_t) + size );

        
        if(*(int_arena+2) == 0 && arena_index == 0 && 3*sizeof(uint32_t)+size<=n){
            //Este primul bloc de initializat 
            *(int_arena+2) = size; //Initializez pe byte 3 marime
            octet = i*sizeof(uint32_t)+3*sizeof(uint32_t); // Aflu pe ce octet incepe
            schimbat=1;
            printf("%ld\n", octet);
            break;

        }
        //daca nu mai are nimic in stanga verific daca are loc acolo
        else if(*(int_arena+1) == 0 && 3*sizeof(uint32_t) + size <= arena_index ){

        ///    printf("AVION CU MOTOR\n");

            int_arena = (uint32_t*) arena;
            *int_arena = 0;
            *(int_arena) = arena_index;
            *(int_arena+2) = size;
            arena_index = 0;
            printf("%ld\n", 3*sizeof(uint32_t));
            schimbat = 1;
            break;

        }// verifc daca are loc intre doua blocuri
        else if( *(int_arena) - arena[*(int_arena) + sizeof(uint32_t)] > size + 3*sizeof(uint32_t)){

         //   printf("GOLF GTR\n");

            int_arena = (uint32_t*) arena+i;
            octet = *(int_arena+2) + 3*sizeof(uint32_t);
            //pun ca next block fix ce urmeaza dupa acesta
            *(int_arena) = octet;
            //pun previous la urmatorul blocul acesta
            arena[*(int_arena+1)] = octet;
            int next = *(int_arena+1);
            int prev =  i;

            int_arena = (uint32_t*) arena+octet;
            //Aloc blocul
            *(int_arena) = next;
            *(int_arena+1) = prev;
            *(int_arena+2) = size;
            schimbat=1;

            printf("%ld\n", octet+3*sizeof(uint32_t));
            break;

        }
        else if(*int_arena == 0 &&  6*sizeof(uint32_t)+size+*(int_arena+2)+arena[*(int_arena+1)] <= n){ //Alloc daca e cel mai din dreapta bloc         
       
           // printf("GOLF R\n");
           // printf("NASINA %d\n", arena[(*int_arena)]);
            int auxi=i;
            int octet = i+3*sizeof(uint32_t) + *(int_arena+2);
            schimbat = 1;                
            //salvez blocul curent 
           // printf("i:%d oc:%d\n", i,octet);               
            //Schimb adresea blocului din stanga
            *(int_arena) = octet;                
            //Creez blocul
            arena[octet] = 0;
            arena[octet+sizeof(uint32_t)] = auxi;
            arena[octet+2*sizeof(uint32_t)] = size;

            //printf("%d NASINA\n",n-(6*sizeof(uint32_t)+size+*(int_arena+2)) );
            printf("%ld\n", octet+3*sizeof(uint32_t));
            //A gasit deci nu mai este nevoie sa parcurg memoria
            break;

        }
        else if(  6*sizeof(uint32_t)+size+*(int_arena+2)+arena[*(int_arena+1)] > n){
                //Nu mai are loc la dreapta
                break;
        }
        else{
            //Daca nu are loc pe blocul curent ma duc la bitul de la inceputul urmatorului bloc
            i = *(int_arena);
        }


    }

    if(!schimbat)
        printf("%d\n", 0);   

}

void my_free(int index){

     int i;
    int block_size = arena[index-sizeof(uint32_t)] + 3*sizeof(uint32_t);
    uint32_t* int_arena = (uint32_t*) (arena+(index-3*sizeof(uint32_t)));
    
    //daca era primul bloc din arena
    if(arena_index == index-3*sizeof(uint32_t)){
        arena_index = *(int_arena);
    }else{
        arena[*(int_arena+1)] = *(int_arena); //schimb byte-ul de next la cel din stanga daca nu este primul
    }

    //schimb byte de previous la cel din dreapta
    arena[*(int_arena)+sizeof(uint32_t)] = *(int_arena+1);  
    


    //pun totul pe 0
    unsigned char* parcurge_arena = arena+(index-3*sizeof(uint32_t));
    for(i=0;i<block_size;i++){
        *(parcurge_arena+i) = 0;
    }

}


void fill(int index, int size, int value){
        unsigned char* parcurge_arena = arena+index;
    
    int max_size = *(arena+index-sizeof(uint32_t));


    for(int i=0;i<size;i++){
        if(i>=max_size){
            fill(arena[index-3*sizeof(uint32_t)]+3*sizeof(uint32_t),size-i,value);
            break;
        }
        *(parcurge_arena+i) = value;

    }
}


void allocaligned(int size, int align){

}

void my_realloc(int index, int size){

}


void parse_command(char* cmd)
{
    const char* delims = " \n";

    char* cmd_name = strtok(cmd, delims);
    if (!cmd_name) {
        goto invalid_command;
    }

    if (strcmp(cmd_name, "INITIALIZE") == 0) {
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        initialize(size);

    } else if (strcmp(cmd_name, "FINALIZE") == 0) {
        finalize();

    } else if (strcmp(cmd_name, "DUMP") == 0) {
        dump();

    } else if (strcmp(cmd_name, "ALLOC") == 0) {
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        alloc(size);

    } else if (strcmp(cmd_name, "FREE") == 0) {
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        uint32_t index = atoi(index_str);
        my_free(index);

    } else if (strcmp(cmd_name, "FILL") == 0) {
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        uint32_t index = atoi(index_str);
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        char* value_str = strtok(NULL, delims);
        if (!value_str) {
            goto invalid_command;
        }
        uint32_t value = atoi(value_str);
        fill(index, size, value);

    } else if (strcmp(cmd_name, "ALLOCALIGNED") == 0) {
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        char* align_str = strtok(NULL, delims);
        if (!align_str) {
            goto invalid_command;
        }
        uint32_t align = atoi(align_str);
        allocaligned(size, align);

    } else if (strcmp(cmd_name, "REALLOC") == 0) {
        printf("Found cmd REALLOC\n");
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        uint32_t index = atoi(index_str);
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        my_realloc(index, size);

    } else {
        goto invalid_command;
    }

    return;

invalid_command:
    printf("Invalid command: %s\n", cmd);
    exit(1);
}

int main(void)
{

    ssize_t read;
    char* line = NULL;
    size_t len;

    /* parse input line by line */
    while ((read = getline(&line, &len, stdin)) != -1) {
        /* print every command to stdout */
        printf("%s", line);

        parse_command(line);
    }

    free(line);

    return 0;
}
