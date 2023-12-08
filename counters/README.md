# CS50 Lab 3
# Aya Hajjeh
# I copied the `bag` README.md file and edited so it fits the `counters` module

### counters

A "counter set" is a set of counters, each distinguished by an integer key.
It's a set - each key can only occur once in the set - but instead of
storing (key,item) pairs, it tracks a counter for each key.  It starts
empty. Each time `counters_add` is called on a given key, that key's
counter is incremented. The current counter value can be retrieved by
asking for the relevant key.

### Usage

The *counters* module, defined in `counters.h` and implemented in `counters.c`, implements a set of `int` key and their equivalent counter, and exports the following functions:

```c
counters_t* counters_new(void);
int counters_add(counters_t* ctrs, const int key);
int counters_get(counters_t* ctrs, const int key);
bool counters_set(counters_t* ctrs, const int key, const int count);
void counters_print(counters_t* ctrs, FILE* fp);
void counters_iterate(counters_t* ctrs, void* arg, 
                      void (*itemfunc)(void* arg, 
                                       const int key, const int count));
void counters_delete(counters_t* ctrs);
```

### Implementation

We implement this counter as a linked list.
The *counter* itself is represented as a `struct counter` containing a pointer to the head of the list; the head pointer is NULL when the counter is empty.

Each node in the list is a `struct counterNode`, a type defined internally to the module.
Each countersNode includes an `int key`, `int count`, and a pointer to the next counterNode on the list.

To insert a new key in the counter we create a new counterNode to hold the `key`, and insert it at the head of the list.

To extract a key from the counter set we pop off the head of the list and return its count, deleting its counterNode.
Of course, if the list is empty, we return NULL instead.

The `counters_print` method prints a little syntax around the list, and between keys.

The `counters_iterate` method calls the `itemfunc` function on each key by scanning the linked list.

The `counters_delete` method deletes each item by scanning the linked list, freeing counterNodes as it proceeds.
It concludes by freeing the `struct counters`.

### Assumptions

No assumptions beyond those that are clear from the spec.

The `item` inserted cannot be NULL, and thus a NULL return from `counters_extract` must indicate either error or empty counter, not simply a NULL `item` coming out of the counter.

Because of the semantics of a *counters*, we have great freedom in our implementation.
Our linked-list approach actually implements a stack: `counters_insert` pushes a new item on the stack, and `counters_extract` pops the top item off the stack.
Other implementations are permitted, but this LIFO list approach is fast and easy to implement.

### Files

* `Makefile` - compilation procedure
* `counters.h` - the interface
* `counters.c` - the implementation
* `counterstest.c` - unit test driver
* `test.names` - test data
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make counters.o`.

### Testing

The `counterstest.c` program reads lines from stdin and stuffs them into a counter, then pulls them back out.
It tests a few error and edge cases.
This test is somewhat minimal.
A lot more could be done!

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

Try testing with `MEMTEST` by editing Makefile to turn on that flag and then `make test`.

To test with valgrind, `make valgrind`.
