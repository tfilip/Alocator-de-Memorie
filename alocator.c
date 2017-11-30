#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

unsigned char *arena;
unsigned int n;
int arena_index;

void intialize(int n) {
    arena = calloc(n, sizeof(unsigned char *));
    arena_index = 0;
}

void finalize() {
    free(arena);

}

void dump() {

    unsigned char *i = arena;
    unsigned int k = 0;



    while (k < n) {

        if (k % 16 == 0)
            printf("%08X\t", k);


        printf("%02X ", *i);
        i++;
        k++;


        //Sa nu mai afisez spatiul la final

        if (k % 8 == 0 && k != 0)
            printf(" ");


        if (k % 16 == 0) {
            printf("\n");
        }


    }

    //Daca a fost doar o linie
    if (k <= 16 || n % 16 != 0)
        printf("\n");

}

void alloc(int size) {



    int schimbat, octet;

    //printf("SIZE:%d\n", size);
    schimbat = 0;
    octet = 0;
 //   int k=0;
    //Parcurgere blocuri
    for (int i = arena_index; i < n;) {
       /* k++;
        if(k>100)
            break;
        printf("%d\n", i);
        */

       // printf("i:%d\n", i);
        int32_t* int_arena = (int32_t*)(arena + i);
        int32_t* int_arena_mid = (int32_t*)(arena + *int_arena + sizeof(int32_t));

        //printf("muise s%d\n",6 * sizeof(int32_t) + size + * (int_arena + 2) + arena[*(int_arena + 1)] );
        if (*(int_arena + 2) == 0 && *(int_arena) == 0 && *(int_arena+1) == 0  && 3 * sizeof(int32_t) + size <= n ) {
            //Este primul bloc de initializat
          //  printf("0:%d\n", *(int_arena));
          //  printf("1:%d\n", *(int_arena+1));
          //  printf("2:%d\n", *(int_arena+2));

            //if(arena[*int_arena+2*sizeof(int32_t)] != 0)
              //  break;

            *(int_arena + 2) = size; //Initializez pe byte 3 marime
            octet = i * sizeof(int32_t) + 3 * sizeof(int32_t); // Aflu pe ce octet incepe
            schimbat = 1;
            printf("%ld\n", octet);
          //  printf("schimb1 %d\n", i);
            break;

            

        }
        //daca nu mai are nimic in stanga verific daca are loc acolo
        else if (*(int_arena + 1) == 0 && 3 * sizeof(int32_t) + size <= arena_index ) {

           // printf("schimb2\n");

            int_arena = (int32_t*) arena;
            *int_arena = 0;
            *(int_arena) = arena_index;
            *(int_arena + 2) = size;

            arena_index = 0;
            printf("%ld\n", 3 * sizeof(int32_t));
            schimbat = 1;
            break;

        }
        else if (*int_arena == 0 &&  6 * sizeof(int32_t) + size + * (int_arena + 2) + arena[*(int_arena + 1)] <= n) { //Alloc daca e cel mai din dreapta bloc

           // printf("schimb3 %d ",*(int_arena+2));
            int auxi = i;
            int octet = i + 3 * sizeof(int32_t) + *(int_arena + 2);
            schimbat = 1;

          //  printf("size: %d\n", size);

           // printf("octet: %d\n", octet);

            *(int_arena) = octet;
            //Creez blocul
            int_arena = (int32_t*) (arena + octet);
            *int_arena = 0;
            *(int_arena+1) = auxi;
            *(int_arena+2) = size;
           // printf("DAS\n");
            printf("%ld\n", octet + 3 * sizeof(int32_t));
            //A gasit deci nu mai este nevoie sa parcurg memoria
            break;

        }

         else if (  6 * sizeof(int32_t) + size + * (int_arena + 2) + (int32_t)arena[*(int_arena + 1)] > n && *int_arena == 0) {
            //Nu mai are loc la dreapta
            //printf("nu am loc\n");
            break;
        }
        // verifc daca are loc intre doua blocuri
        else if ((int32_t) (*(int_arena) -  *int_arena_mid - *(int_arena + 2)) > (int32_t)((int32_t) size + (int32_t) 6 * sizeof(int32_t))) {
            
           // printf("hai sa ma joc%d\n", *int_arena_mid);

           // printf("muie: %d\n", *(int_arena) - arena[*(int_arena) + sizeof(int32_t)] - *(int_arena + 2) > size + 3*sizeof(int32_t) );
           // printf("s: %d\n", *(int_arena) - arena[*(int_arena) + sizeof(int32_t)] - *(int_arena + 2) );
           // printf("d: %d\n", size + 3 * sizeof(int32_t));
            // printf("schimb4\n");
            int_arena = (int32_t*) arena + i;
            //printf("dasd %d\n",*(int_arena+2) );
            octet = *(int_arena + 2) + 3 * sizeof(int32_t);
            //savlez urmatorul bloc si blocul curent

           // printf("CASSA LOCO%d\n", *(int_arena));

            int next = *(int_arena);
            int prev =  i;

            //printf("nest %d\n", next);

            //printf("octet :%d\n", octet);
            //pun ca next block fix ce urmeaza dupa acesta
            *(int_arena) = octet;
            //pun previous la urmatorul blocul 
            arena[next+sizeof(int32_t)] = octet; 

            int_arena = (int32_t*) (arena + octet);
            //Aloc blocul
            *(int_arena) = next;
            *(int_arena + 1) = prev;
            *(int_arena + 2) = size;
            schimbat = 1;
          //  printf("DAI MA\n");
            printf("%ld\n", octet + 3 * sizeof(int32_t));
            break;

        }
        else {
            //Daca nu are loc pe blocul curent ma duc la bitul de la inceputul urmatorului bloc
          //  printf("NU AM LOC\n");
            i = *(int_arena);
        }


    }

    if (!schimbat){
       // printf("NU AM SCHIMBAT\n");
        printf("%d\n", 0);
    }

   //dump();

}

void custom_free(int index) {

    int i;
    int block_size = arena[index - sizeof(int32_t)] + 3 * sizeof(int32_t);
    int32_t* int_arena = (int32_t*) (arena + (index - 3 * sizeof(int32_t)));

    //schimb byte de previous la cel din dreapta
    arena[*(int_arena) + sizeof(int32_t)] = *(int_arena + 1);




    //daca era primul bloc din arena
    if (arena_index == (index - 3 * sizeof(int32_t))) {
        arena_index = *(int_arena);
    } else {
        arena[*(int_arena + 1)] = *(int_arena); //schimb byte-ul de next la cel din stanga daca nu este primul
    }



    //pun totul pe 0
    unsigned char* parcurge_arena = arena + (index - 3 * sizeof(int32_t));
    for (i = 0; i < block_size; i++) {
        *(parcurge_arena + i) = 0;
    }
}

void fill(int index, int size, int value) {
    unsigned char* parcurge_arena = arena + index;
    int32_t* int_arena = (int32_t*) (arena+index-sizeof(int32_t));
    int32_t* int_arena_next = (int32_t*)(arena+index-3*sizeof(int32_t));
    int max_size = *int_arena;




    for (int i = 0; i < size; i++) {
        if (i >= max_size) {
            fill(*int_arena_next + 3 * sizeof(int32_t), size - i, value);
            break;
        }


        *(parcurge_arena + i) = value;

    }

}

void show() {

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
        int32_t size = atoi(size_str);
        n = size;
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
        int32_t size = atoi(size_str);
        alloc(size);

    } else if (strcmp(cmd_name, "FREE") == 0) {
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        int32_t index = atoi(index_str);
        custom_free(index);

    } else if (strcmp(cmd_name, "FILL") == 0) {
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        int32_t index = atoi(index_str);
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        int32_t size = atoi(size_str);
        char* value_str = strtok(NULL, delims);
        if (!value_str) {
            goto invalid_command;
        }
        int32_t value = atoi(value_str);
        fill(index, size, value);

    } else if (strcmp(cmd_name, "ALLOCALIGNED") == 0) {
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        int32_t size = atoi(size_str);
        char* align_str = strtok(NULL, delims);
        if (!align_str) {
            goto invalid_command;
        }
        int32_t align = atoi(align_str);
        // TODO - ALLOCALIGNED

    } else if (strcmp(cmd_name, "REALLOC") == 0) {
        printf("Found cmd REALLOC\n");
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        int32_t index = atoi(index_str);
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        int32_t size = atoi(size_str);
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

   // printf("%d\n", sizeof(int32_t));
   // printf("%d\n", sizeof(int32_t));

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
