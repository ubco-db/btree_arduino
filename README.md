# Efficient B-tree Implementation for Arduino Platform

The B-Tree implementation for Arduino has the following benefits:

1. Uses the minimum of two page buffers for performing all operations. The memory usage is less than 1.5 KB for 512 byte pages.
2. No use of dynamic memory (i.e. malloc()). All memory is pre-allocated at creation of the tree.
3. Efficient insert (put) and query (get) of arbitrary key-value data.
4. Support for iterator to traverse data in sorted order.
5. Easy to use and include in existing projects. Requires only an Arduino with an SD card.
6. Open source license. Free

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


### Insert (put) items into tree

### Query (get) items from tree

### Iterate through items in tree



#### Ramon Lawrence\nUniversity of British Columbia Okanagan

