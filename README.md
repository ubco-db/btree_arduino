# Efficient B-tree Implementation for Arduino Platform

The B-Tree implementation for Arduino has the following benefits:

1. Uses the minimum of two page buffers for performing all operations. The memory usage is less than 1.5 KB for 512 byte pages.
2. No use of dynamic memory (i.e. malloc()). All memory is pre-allocated at creation of the tree.
3. Efficient insert (put) and query (get) of arbitrary key-value data.
4. Support for iterator to traverse data in sorted order.
5. Easy to use and include in existing projects. Requires only an Arduino with an SD card.
6. Open source license. Free to use for commerical and open source projects.

## Code Files

* test_btree.h - test file demonstrating how to get, put, and iterate through data in B-tree
* main.cpp - main Arduino code file
* btree.h, btree.c - implementation of B-tree supporting arbitrary key-value data items
* dbbuffer.h, dbbuffer.c - provide interface with SD card and buffering of pages in memory

## Support Code Files

* serial_c_iface.h, serial_c_iface.cpp - allows printf() on Arduino
* sd_stdio_c_iface.h, sd_stdio_c_iface.h - allows use of stdio file API (e.g. fopen())

## Usage

### Setup B-tree and Configure Memory

```c
/* Configure buffer */
dbbuffer* buffer = (dbbuffer*) malloc(sizeof(dbbuffer));
if (buffer == NULL) {   
	printf("Failed to allocate buffer struct.\n");
	return;
}
buffer->pageSize = 512;
uint8_t M = 2;
buffer->numPages = M;
buffer->status = (id_t*) malloc(sizeof(id_t)*M);
if (buffer->status == NULL) {   
	printf("Failed to allocate buffer status array.\n");
	return;
}
        
buffer->buffer = malloc((size_t) buffer->numPages * buffer->pageSize);   
if (buffer->buffer == NULL) {   
	printf("Failed to allocate buffer.\n");
	return;
}

/* Setup output file. */
SD_FILE *fp;
fp = fopen("myfile.bin", "w+b");
if (NULL == fp) {
	printf("Error: Can't open file!\n");
	return;
}
        
buffer->file = fp;  

/* Configure btree state */
btreeState* state = (btreeState*) malloc(sizeof(btreeState));
if (state == NULL) {   
	printf("Failed to B-tree state struct.\n");
	return;
}
state->recordSize = 16;
state->keySize = 4;
state->dataSize = 12;       
state->buffer = buffer;

state->tempKey = malloc(state->keySize); 
state->tempData = malloc(state->dataSize);          	

/* Initialize B-tree structure */
btreeInit(state);
```

### Insert (put) items into tree

```c
btreePut(state, (void*) keyPtr, (void*) dataPtr);
```

### Query (get) items from tree

```c
/* keyPtr points to key to search for. dataPtr must point to pre-allocated space to copy data into. */
int8_t result = btreeGet(state, (void*) keyPtr, (void*) dataPtr);
```

### Iterate through items in tree

```c
btreeIterator it;
uint32_t minVal = 40;     /* Starting minimum value to start iterator (inclusive) */
it.minKey = &minVal;
uint32_t maxVal = 299;	  /* Maximum value to end iterator at (inclusive) */
it.maxKey = &maxVal;       

btreeInitIterator(state, &it);

uint32_t *itKey, *itData;	/* Pointer to key and data value. Valid until next call to btreeNext(). */

while (btreeNext(state, &it, (void**) &itKey, (void**) &itData))
{                      
	printf("Key: %d  Data: %d\n", *itKey, *itData);	
}
```


#### Ramon Lawrence<br>University of British Columbia Okanagan

