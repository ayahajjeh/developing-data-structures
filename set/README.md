# CS50 Lab 3
# Aya Hajjeh
# I copied the `bag` README.md file and edited so it fits the `set` module

### set

A *set* maintains an unordered collection of (key,item) pairs;
any given key can only occur in the set once. It starts out empty 
and grows as the caller inserts new (key,item) pairs.  The caller 
can retrieve items by asking for their key, but cannot remove or 
update pairs.  Items are distinguished by their key.

### Usage

The *set* module, defined in `set.h` and implemented in `set.c`, implements an unordered collection of 
(ket,item) pairs, and exports the following functions:

```c
set_t* set_new(void);
bool set_insert(set_t* set, const char* key, void* item);
void* set_find(set_t* set, const char* key);
void set_print(set_t* set, FILE* fp, 
               void (*itemprint)(FILE* fp, const char* key, void* item) );
void set_iterate(set_t* set, void* arg,
                 void (*itemfunc)(void* arg, const char* key, void* item) );
void set_delete(set_t* set, void (*itemdelete)(void* item) );
```

### Implementation

We implement this set as a linked list.
The *set* itself is represented as a `struct set` containing a pointer to the head of the list; the head pointer is NULL when the counter is empty.

Each node in the list is a `struct setNode`, a type defined internally to the module.
Each setNode includes a `char* key`, `void* item`, and a pointer to the next setNode on the list.

To insert a new key in the set we create a new setNode to hold the `key`, and insert it at the head of the list.

To extract a key from the set set we pop off the head of the list and return its item, deleting its setNode.
Of course, if the list is empty, we return NULL instead.

The `set_print` method prints a little syntax around the list, and between keys. It uses the function `itemprint` to print the items in the set.

The `set_iterate` method calls the `itemfunc` function on each key by scanning the linked list.

The `set_delete` method deletes each item by scanning the linked list, freeing setNodes as it proceeds.
It concludes by freeing the `struct set`.

### Assumptions

No assumptions beyond those that are clear from the spec.

The `item` inserted cannot be NULL, and thus a NULL return from `set_extract` must indicate either error or empty counter, not simply a NULL `item` coming out of the set.

Because of the semantics of a *set*, we have great freedom in our implementation.
Our linked-list approach actually implements a stack: `set_insert` pushes a new item on the stack, and `set_extract` pops the top item off the stack.
Other implementations are permitted, but this LIFO list approach is fast and easy to implement.

### Files

* `Makefile` - compilation procedure
* `set.h` - the interface
* `set.c` - the implementation
* `settest.c` - unit test driver
* `test.names` - test data
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make set.o`.

### Testing

The `settest.c` program reads lines from stdin and stuffs them into a set, then pulls them back out.
It tests a few error and edge cases.
This test is somewhat minimal.
A lot more could be done!

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

Try testing with `MEMTEST` by editing Makefile to turn on that flag and then `make test`.

To test with valgrind, `make valgrind`.