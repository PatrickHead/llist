# llist - A Simple (Doubly) Linked List libary in C

A C language library to create and maintain doubly linked list

NOTE:  Please see test-llist.c for an example of how to use this library

Normally, <i> llist_set_new() </i>, <i> llist_set_dup() </i>, <i> llist_set_cmp() </i> and <i> llist_set_free() </i> should be called to provide full linked list capability, integrated with user data.

However, if one or more llist helper functions are not provided, then the following will alter the behavior of certain llist functions:

If <i> llist_set_cmp() </i> is not called then <i> llist_find() </i> will not work.

If <i> llist_set_free() </i> is not called, then <i> llist_free() </i> and <i> llist_remove() </i> will not free the payload of each node.  This may be desireable when the caller is tracking all payload memory, or if two or more linked lists are accessing the same payload memory.

If <i> llist_set_dup() </i> is not called, then <i> llist_dup() </i> will not create a deep copy of a node, it just copies the pointers.  Subsequently, <i> llist_add() </i> will do the same.  This may be may be desireable in a linked list where the nodes are used just for traversal and all the payload memory is managed outside of the linked list.

If <i> llist_set_new() </i> is not called there will be no adverse effects.  This function is just a convenient placeholder for the user defined function that allocates a new node.
