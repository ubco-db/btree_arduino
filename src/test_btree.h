/******************************************************************************/
/**
@file		test_btree.h
@author		Ramon Lawrence
@brief		This file does performance/correctness testing of BTree.
@copyright	Copyright 2021
			The University of British Columbia,		
@par Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

@par 1.Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.

@par 2.Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.

@par 3.Neither the name of the copyright holder nor the names of its contributors
	may be used to endorse or promote products derived from this software without
	specific prior written permission.

@par THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/
/******************************************************************************/
#include <time.h>
#include <string.h>

#include "btree.h"
#include "randomseq.h"


int32_t checkValues(btreeState *state, void* recordBuffer, int32_t* vals, uint32_t n)
{
     /* Verify that can find all values inserted */
    uint32_t errors = 0;
    for (uint32_t i = 0; i < n; i++) 
    { 
        int32_t key = vals[i];        
        int8_t result = btreeGet(state, &key, recordBuffer);
        if (result != 0) 
        {   errors++;
            printf("ERROR: Failed to find: %d\n", key);
            btreeGet(state, &key, recordBuffer);
        }
        else if (*((int32_t*) recordBuffer) != key)
        {   printf("ERROR: Wrong data for: %d\n", key);
            printf("Key: %d Data: %d\n", key, *((int32_t*) recordBuffer));
        }
    }
    return errors;
}

/**
 * Runs all tests and collects benchmarks
 */ 
void runalltests_btree()
{    
    printf("HERE1\n");
    uint32_t errors = 0;
    uint32_t i, j;

    srand(1);
    randomseqState rnd;
    rnd.size = 100;
    uint32_t n = 100; 
    rnd.prime = 0;
/*
        printf("HERE2\n");
    randomseqInit(&rnd);
    uint32_t *vals = (uint32_t*) malloc(n * sizeof(uint32_t));

    for (uint32_t i=0; i < n; i++)
    {
        vals[i] = randomseqNext(&rnd);
        // printf("%d\n", vals[i]);
        // See if number exists 
        for (j=0; j < i; j++)
            if (vals[j] == vals[i])
            {   printf("Error: Number generated: %d\n", vals[j]);
           //     return;
            }
    }

    // Check if can regenerate sequence 
    srand(1);
    randomseqInit(&rnd);
    for (i=0; i < n; i++)
    {
        uint32_t tmp = randomseqNext(&rnd);
        printf("%d %d\n", tmp, vals[i]);
        if (tmp != vals[i])
            printf("ERROR with sequence.\n");
    }
    
printf("HERE3\n");
   */

    int8_t M = 3;        
   
    /* Configure buffer */
    dbbuffer* buffer = (dbbuffer*) malloc(sizeof(dbbuffer));
    buffer->pageSize = 512;
    buffer->numPages = M;
    buffer->status = (id_t*) malloc(sizeof(id_t)*M);
    buffer->buffer  = malloc((size_t) buffer->numPages * buffer->pageSize);   
   
    /* Configure btree state */
    btreeState* state = (btreeState*) malloc(sizeof(btreeState));

    state->recordSize = 16;
    state->keySize = 4;
    state->dataSize = 12;       

    /* Connections betwen buffer and btree */
    buffer->activePath = state->activePath;
    buffer->state = state;    

    state->tempKey = malloc(sizeof(int32_t)); 
    state->tempData = malloc(12); 
    int8_t* recordBuffer = (int8_t*) malloc(state->recordSize);

    state->mappingBufferSize = 5000;
    state->mappingBuffer = malloc(state->mappingBufferSize);	
printf("HERE4\n");
    /* Setup output file. TODO: Will replace with direct memory access. */
    ION_FILE *fp;
    fp = fopen("myfile.bin", "w+b");
    if (NULL == fp) {
        printf("Error: Can't open file!\n");
        return;
    }
    printf("HERE5\n");
    buffer->file = fp;

    state->parameters = 0;    
    state->buffer = buffer;

    /* Initialize btree structure with parameters */
    btreeInit(state);
  printf("HERE6\n");
    /* Data record is empty. Only need to reset to 0 once as reusing struct. */        
    for (i = 0; i < (uint16_t) (state->recordSize-4); i++) // 4 is the size of the key
    {
        recordBuffer[i + sizeof(int32_t)] = 0;
    }

    unsigned long start = millis();   
  printf("HERE7\n");
    srand(1);
    randomseqInit(&rnd);
  printf("HERE8\n");
    for (i = 0; i < n ; i++)
    {           
        id_t v = randomseqNext(&rnd);// vals[i]; 
        
       // printf("\n****STARTING KEY: %d\n",v);
        // btreePrint(state);    
/*
        if (v == 375 || v == 151)       
        {
            printf("KEY: %d\n",v);
            btreePrint(state);    
            btreePrintMappings(state);       
        }
    */
       // printf("KEY: %d\n",v);
        *((int32_t*) recordBuffer) = v;
        *((int32_t*) (recordBuffer+4)) = v;             

        if (btreePut(state, recordBuffer, (void*) (recordBuffer + 4)) == -1)
        {  
            btreePrint(state);               
            printf("INSERT ERROR: %d\n", v);
            return;
        }
        /*
        if (v == 204 || v == 151)       
        {
            printf("KEY: %d\n",v);
            btreePrint(state);  
            btreePrintMappings(state);         
        }
        */
       /*
        if (checkValues(state, recordBuffer, vals, i) > 0)
        {   printf("Error finding value. Key: %d\n", v);
            btreePrint(state);   
            return;
        }
        */
        if (i % 1 == 0)
        {           
            printf("Num: %lu KEY: %lu\n", i, v);
            btreePrint(state);               
        }        
    }    

    unsigned long end = millis();   

    // btreePrint(state);     
    
    printStats(state->buffer);

    printf("Elapsed Time: %lu s\n", (end - start));
    printf("Records inserted: %d\n", n);

    printf("\nVerifying and searching for all values.\n");
    start = millis();

    srand(1);
    randomseqInit(&rnd);

    /* Verify that can find all values inserted */    
    for (i = 0; i < n; i++) 
    { 
        int32_t key = randomseqNext(&rnd);// vals[i];
        int8_t result = btreeGet(state, &key, recordBuffer);
        if (result != 0) 
        {   errors++;
            printf("ERROR: Failed to find: %d\n", key);
            btreeGet(state, &key, recordBuffer);
        }
        else if (*((int32_t*) recordBuffer) != key)
        {   printf("ERROR: Wrong data for: %d\n", key);
            printf("Key: %d Data: %d\n", key, *((int32_t*) recordBuffer));
        }
    }

    if (errors > 0)
        printf("FAILURE: Errors: %d\n", errors);
    else
        printf("SUCCESS. All values found!\n");
    
    end = millis();
    printf("Elapsed Time: %lu s\n", (end - start));
    printf("Records queried: %d\n", n);   
    printStats(state->buffer);     

    /* Below minimum key search */
    int32_t key = -1;
    int8_t result = btreeGet(state, &key, recordBuffer);
    if (result == 0) 
        printf("Error1: Key found: %d\n", key);

    /* Above maximum key search */
    key = 3500000;
    result = btreeGet(state, &key, recordBuffer);
    if (result == 0) 
        printf("Error2: Key found: %d\n", key);
    
    free(recordBuffer);
    
    btreeIterator it;
    uint32_t mv = 40;     // For all records, select mv = 1.
    it.minKey = &mv;
    int v = 299;
    it.maxKey = &v;       

    // btreePrint(state); 

    btreeInitIterator(state, &it);
    i = 0;
    int8_t success = 1;    
    uint32_t *itKey, *itData;

    while (btreeNext(state, &it, (void**) &itKey, (void**) &itData))
    {                      
        // printf("Key: %d  Data: %d\n", *itKey, *itData);
        if (i+mv != *itKey)
        {   success = 0;
            printf("Key: %d Error\n", *itKey);
        }
        i++;        
    }
    printf("\nRead records: %d\n", i);

    if (success && i == (v-mv+1))
        printf("SUCCESS\n");
    else
        printf("FAILURE\n");    
    
    // printStats(buffer);

    /* Perform various queries to test performance */
    closeBuffer(buffer);    
    
    free(state->buffer->buffer);
}

