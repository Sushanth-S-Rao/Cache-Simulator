#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int hit, miss;
int get_set_index (long long add, int sett, int blockk);
long long int get_tag (long long add, int sett, int blockk);
int get_set_in_cache (int sett);


//Structure for CACHE
typedef struct{
    int valid;
    long long tag;
    long long * block;
}cache_line;

//For Input Parameters of Cache
typedef struct {
    int s;    /* The number of set bits */
    int E;    /* The number of blocks per set */
    int b;    /* THe number of block offset bits per block */
    int v;    /* verbose flag for debug logs*/
    int h;    /* help command */
    char* t;  /* THe path of input trace file */
} csim_args_t;


// This structure stores the individual trace operations on the memory
typedef struct {
    char op;        /* OP: Load(L) or Store(S)*/
    long int addr;  /* memory address */
    size_t size;    /* size of access */
} memory_access_t;

memory_access_t *parse_memory_access(char *trace_line); 


int csim_parse_arguments(csim_args_t *cache_parameters, int argc, char **argv) 
{
    int argument;
    char *s = NULL, *E = NULL, *b = NULL, *t = NULL;
    int v = 0;

    /* set the parsing string and expected values */
    while( (argument = getopt(argc, argv, "s:E:b:t:vh")) != -1)
        switch (argument) 
        {
        case 's': s = (char *)optarg;   break;
        case 'E': E = (char *)optarg;   break;
        case 'b': b = (char *)optarg;   break;
        case 't': t = (char *)optarg;   break;
        case 'v': v = 1;                break;
        case 'h': printf("Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t "
                         "<tracefile>\n");
                 exit(1);                
                 break;
        default: exit(1);
        }

    printf("Parsed Values = s: %s, E: %s, b: %s, t: %s, v: %d\n", s, E, b, t, v);

    cache_parameters->s = atoi(s);
    cache_parameters->E = atoi(E);
    cache_parameters->b = atoi(b);
    cache_parameters->t = t;
    cache_parameters->v = v;

    return 1;
}


int main(int argc, char ** argv)
{
    csim_args_t *csim_params = (csim_args_t *)malloc(sizeof(csim_args_t));   
    csim_parse_arguments(csim_params, argc, argv);
    printf("Parsing Complete\n\n"); 
    long number_of_sets = (long)pow(2, (csim_params->s));
    long block_size = (long)pow(2, (csim_params->b));
    cache_line *cache = (cache_line *)malloc(number_of_sets);
    size_t read_line = 0;
    char * line = NULL;
    size_t len;
    FILE *tr_f = (FILE*) fopen(csim_params->t, "r");
    memory_access_t *mem_access;

    // As long as there are memory operations run the cache
    while ((read_line = getline(&line, &len, tr_f)) != -1) 
    {
        printf("Processing the Operation: %s", line);
        // Parse the line and populate the memory access fields 
        mem_access = (memory_access_t*) parse_memory_access(line);
        long long mem_dec =  mem_access->addr;
        printf("address being checked: %lld\n", mem_dec);
        int set_num = get_set_index(mem_dec, csim_params->s, csim_params->b);
        long long int tag_num = get_tag(mem_dec, csim_params->s, csim_params->b);
        printf("set_num of address is: %d\n", set_num);
        printf("tag_num of address is: %lld\n", tag_num);
        
        if(cache[set_num].valid == 1){
            if (cache[set_num].tag == tag_num){
                hit += 1;
                printf("HIT \n\n");
            } else {
                miss += 1;
                printf("MISS 1 \n\n"); //miss eviction
            }
        } else {
            miss += 1;
            printf("MISS 2\n\n");
            cache[set_num].tag = tag_num;
            cache[set_num].valid = 1;
        }
        free(mem_access); 
    }
    printf("\nEND STATS: \nHITS:\t%d\nMISSES:\t%d\n", hit, miss);
    return 1;
}


memory_access_t *parse_memory_access(char *trace_line) 
{
    char *token;
    memory_access_t *mem_access = (memory_access_t *)malloc(sizeof(memory_access_t));
    token = strtok(trace_line, " ");

    /* PART-1: Parse the op */
    if (token != NULL) {
        mem_access->op = (char)token[0];
    } else {
        return NULL;
    }

    /* PART-2: Parse the address */
    token = strtok(NULL, trace_line);
    token = strtok(token, ",");
    if (token != NULL) {
        mem_access->addr = (long)strtol(token, NULL, 16);       //we use
    } else {
        return NULL;
    }

    /* PART-3: Parse the size */
    token = strtok(NULL, trace_line);
    if (token != NULL) {
        mem_access->size = atoi(token);
    } else {
        return NULL;
    }

    printf("Parsed Values: %ld, %c, %zd\n", mem_access->addr,
                     mem_access->op, mem_access->size);

    return mem_access;
}


//Get the SET index
int get_set_index (long long add, int sett, int blockk)       //add is 64-bit address, set and block are obtained by -s and -b
{
    int temp = add >> blockk;
    //printf("SET = Decimal (%d) >> Block (%d) = %d\n", add, blockk, temp);
    int mask = pow(2, sett) - 1;
    return temp & mask;
}

//Get the TAG
long long int get_tag (long long add, int sett, int blockk)
{
    int rop = sett+blockk;
    long long temp = add >> rop;
    //printf("TAG = Decimal (%d) >> Block+Set (%d) = %d\n", add, rop, temp);
    return temp;
}

//Get SET number in decimals for comparison of tags
int get_set_in_cache (int sett)
{
  int set_num = 0, i=0, rem;
  while (sett != 0) 
  {
    rem = sett % 10;
    sett /= 10;
    set_num += rem * pow(2, i);
    ++i;
  }
  return set_num;
}