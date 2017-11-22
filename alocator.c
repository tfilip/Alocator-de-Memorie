#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

unsigned char *arena;
unsigned int n;
int arena_index;

struct block{
    uint32_t next_index;
    uint32_t prev_index;
    uint32_t current_index;
    uint32_t block_size;
};


void intialize(int n){
    arena = calloc(n, sizeof(unsigned char *));
}

void finalize(){    
    free(arena);
    
}

void dump(){
    unsigned char *i=arena;
    unsigned int k=0;
    while(k<n){

        if(k%16==0)
            printf("%08X \t", k);


        printf("%02X ",*i);
        i++;
        k++;

        if (k%8==0&&k!=0)
            printf(" ");

        if (k%16==0&&k!=0)
            printf("\n");
        
    }

    //DE SCOS SPACE DE LA FINAL

}

void alloc(int size){

    int schimbat, octet;
    
    schimbat=0;
    octet=0;
    if(arena_index==0){
        //Parcurgere blocuri
        for(int i=0;i<n;){
            uint32_t* int_arena=(uint32_t*)(arena+i);
            if(*(int_arena+2) == 0){
                //Este primul bloc de initializat 
                *(int_arena+2) = size; //Initializez pe byte 3 marime
                octet = i*sizeof(uint32_t)+3*sizeof(uint32_t); // Aflu pe ce octet incepe
                schimbat=1;
                break;

            }else if( *(int_arena) - (i + *(int_arena+2)) >= size){         

                int right=0;
                int auxi=i;
                //Verific daca este ultimul bloc din coada
                if(*int_arena == 0 && n-(6*sizeof(uint32_t)+size+*(int_arena+2)) > 0 ){
                    i=3*sizeof(uint32_t) + *(int_arena+2);
                    right=1;
                }else if( n-(6*sizeof(uint32_t)+size+*(int_arena+2)) > 00 ){
                    //Nu mai are loc
                    break;
                }


                
                //urmatorul bloc
                int aux1 = *int_arena;
                //blocul curent
                int aux2 = arena[*(int_arena+1)];
                
                //In cazul in care are loc -- ii aflu bitul
                if(right)
                    octet = i;
                else
                    octet = i+size;

                schimbat = 1;

                printf("i:%d oc:%d\n", i,octet);
                


                //Schimb adresele celor doua blocuri vecine
                //STANGA
                *(int_arena) = octet;

                //DREAPTA
                if(!right)
                    arena[*(int_arena+1)] = octet;

                //Creez blocul
                if(!right)
                    arena[octet] = aux1;
                else
                    arena[octet] = 0;

                if(right)
                    arena[octet+sizeof(uint32_t)] = auxi;
                else
                    arena[octet+sizeof(uint32_t)] = aux2;

                arena[octet+2*sizeof(uint32_t)] = size;
                

                //A gasit deci nu mai este nevoie sa parcurg matricea
                break;

            }
            else{

                //Daca nu are loc pe blocul curent ma duc la bitul de la inceputul urmatorului bloc
                i = *(int_arena);

                printf("MAAAI %d\n", i);
            }


        }
    }

    printf("%d\n", octet);


}

void custom_free(int index){
    
}

void fill(int index, int size, int value){
    
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
