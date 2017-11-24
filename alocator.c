#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

unsigned char *arena;
unsigned int n;
int arena_index;

void intialize(int n){
    arena = calloc(n, sizeof(unsigned char *));
    arena_index = 0;
}

void finalize(){    
    free(arena);
    
}

void dump(){

    unsigned char *i=arena;
    unsigned int k=0;



    while(k<n){

        if(k%16==0)
            printf("%08X\t", k);
        

        printf("%02X ",*i);
        i++;
        k++;
        

        //Sa nu mai afisez spatiul la final

        if (k%8==0&&k!=0)
            printf(" ");

        
        if (k%16==0){
              printf("\n");
        }

        
    }

    //Daca a fost doar o linie
    if(k<=16 || n%16 != 0)
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

        
        if(*(int_arena+2) == 0 && arena_index == 0){
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
        else if(*int_arena == 0 && n-(6*sizeof(uint32_t)+size+*(int_arena+2)) >= 0){ //Alloc daca e cel mai din dreapta bloc         
       
           // printf("GOLF R\n");

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

            printf("%ld\n", octet+3*sizeof(uint32_t));
            //A gasit deci nu mai este nevoie sa parcurg memoria
            break;

        }
        else if( n-(6*sizeof(uint32_t)+size+*(int_arena+2)) < 0 ){
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

void custom_free(int index){

    int i;
    int block_size = arena[index-sizeof(uint32_t)] + 3*sizeof(uint32_t);
    uint32_t* int_arena = (uint32_t*) (arena+(index-3*sizeof(uint32_t)));
    
    //daca era primul bloc din arena
    if(*(int_arena+1) == 0){
        arena_index = *(int_arena);
    }else{
        arena[*(int_arena+1)] = *int_arena; //schimb byte-ul de next la cel din stanga daca nu este primul
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
        if(i>max_size)
            break;
        *(parcurge_arena+i) = value;

    }
    
}

void show(){

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
        n=size;
        intialize(size);

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
        custom_free(index);

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
        // TODO - ALLOCALIGNED

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
       // TODO - REALLOC

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
    size_t read;
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
