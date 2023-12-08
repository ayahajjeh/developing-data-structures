# CS50 Lab 3
# Aya Hajjeh
# I copied the `bag` README.md file and edited so it fits the `hashtable` module

### hashtable

A *hashtable* is a set of (key,item) pairs.  It acts just like a set, 
but is far more efficient for large collections.

### Usage

The *hashtable* module, defined in `hashtable.h` and implemented in `hashtable.c`, implements an unordered collection of (ket,item) pairs but in a more efficient way than a `set` and exports the following functions:

```c
hashtable_t* hashtable_new(const int num_slots);
bool hashtable_insert(hashtable_t* ht, const char* key, void* item);
void* hashtable_find(hashtable_t* ht, const char* key);
void hashtable_print(hashtable_t* ht, FILE* fp, 
                     void (*itemprint)(FILE* fp, const char* key, void* item));
void hashtable_iterate(hashtable_t* ht, void* arg,
                       void (*itemfunc)(void* arg, const char* key, void* item) );
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) );
```

### Implementation

We implement this hashtable as a linked list.
The *hashtable* itself is represented as a `struct hashtable` containing a pointer to the head of the list aka the hashtable array, and the number of slots in the array; the head pointer is NULL when the hashtable is empty.

Each node in the array is a `struct hashtableNode`, a type defined internally to the module.
Each hashtableNode includes a `char* key`, `void* item`.

To insert a new key in the hashtable we use the `hash_jenkins` function to figure out which slot in the array is appropriate for that key.

To extract a key from the hashtable we figure out its equivalent index by using the `hash-jenkins` function and return its item, emptying its place in the array.
Of course, if the hashtable is empty, we return NULL instead.

The `hashtable_print` method prints a little syntax around the list, and between indices. It uses the function `itemprint` to print the items in the hashtable.

The `hashtable_iterate` method calls the `itemfunc` function on each key by scanning the array.

The `hashtable_delete` method deletes each item by scanning the hash table, freeing all its indices as it proceeds.
It concludes by freeing the `struct hashtable`.

### Assumptions

No assumptions beyond those that are clear from the spec.

The `item` inserted cannot be NULL, and thus a NULL return from `hashtable_extract` must indicate either error or empty hashtable, not simply a NULL `item` coming out of the hashtable.



### Files

* `Makefile` - compilation procedure
* `hashtable.h` - the interface
* `hashtable.c` - the implementation
* `hashtabletest.c` - unit test driver
* `test.names` - test data
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make hashtable.o`.

### Testing

The `hashtabletest.c` program reads lines from stdin and stuffs them into a hashtable, then pulls them back out.
It tests a few error and edge cases.
This test is somewhat minimal.
A lot more could be done!

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

Try testing with `MEMTEST` by editing Makefile to turn on that flag and then `make test`.

To test with valgrind, `make valgrind`.