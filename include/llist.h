/*
 *  Copyright 2021,2024,2025 Patrick T. Head
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation, either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 *  @file llist.h
 *  @brief Header file for simple C library for doubly linked lists
 */

#ifndef LLIST_H
#define LLIST_H

  /**
   *  @typedef enum llist_position
   *  @brief used by llist_add() to determine insertion point
   */

typedef enum
{
  llist_position_tail = 0,  /**<  add at end of list        */
  llist_position_head,      /**<  add at beginning of list  */
  llist_position_before,    /**<  add before @b where node  */
  llist_position_after      /**<  add after @b where node   */
} llist_position;

  /**
   *  @typedef llist_node
   *  @brief creates a type for struct @a llist_node
   */

typedef struct llist_node llist_node;

  /**
   *  @struct llist_node
   *  @brief linked list node structure, contains forward, reverse and payload pointers
   */

struct llist_node
{
  struct llist_node *previous;   /**<  points to previous node  */
  struct llist_node *next;       /**<  points to next node      */
  void *payload;                 /**<  generic node data        */
};

  /**
   *  @typedef llist_node *(*llist_new_node)(void);
   *  @brief   creates a type for function prototype to create a new @a llist_node
   *           struct
   */

typedef llist_node *(*llist_new_node)(void);

  /**
   *  @typedef llist_node *(*llist_dup_node)(llist_node *node);
   *  @brief   creates a type for function prototype to copy an @a llist_node
   *           struct
   */

typedef llist_node *(*llist_dup_node)(llist_node *node);

  /**
   *  @typedef void (*llist_free_node)(llist_node *node);
   *  @brief   creates a type for function prototype to free all memory
   *           allocated to an @a llist_node struct
   */

typedef void (*llist_free_node)(llist_node *node);

  /**
   *  @typedef int (*llist_cmp_node)(llist_node *a, llist_node *b);
   *  @brief   creates a type for function prototype to compare two @a llist_node
   *           structs
   */

typedef int (*llist_cmp_node)(llist_node *a, llist_node *b);

  /**
   *  @typedef llist
   *  @brief creates a type for struct @a llist
   */

typedef struct llist llist;

  /**
   *  @struct llist
   *  @brief LLIST structure, contains metadata for a linked list
   */

struct llist
{
  llist_node *head;                               /**<  points to head of list  */
  llist_node *tail;                               /**<  points to tail of list  */
  llist_node *current;                            /**<  points to current @a llist_node in list  */
  llist_new_node new_node;    /**<  user supplied function to create a new @a llist_node  */
  llist_dup_node dup_node;    /**<  user supplied function to duplicate a @a llist_node  */
  llist_free_node free_node;  /**<  user supplied function to free a @a llist_node  */
  llist_cmp_node cmp_node;    /**<  user supplied function to compare two @a llist_node structs  */
};

  /*
   *  LLIST_POSITION functions
   */

llist_position llist_position_string_to_type(char *string);
char *llist_position_type_to_string(llist_position position);

  /*
   *  LLIST functions
   */

llist *llist_new(void);
llist *llist_dup(llist *ll);
void llist_free(llist *ll);
void llist_set_new(llist *ll, llist_new_node new_func);
void llist_set_dup(llist *ll, llist_dup_node dup_func);
void llist_set_free(llist *ll, llist_free_node free_func);
void llist_set_cmp(llist *ll, llist_cmp_node cmp_func);
void llist_add(llist *ll,
               llist_position position,
               llist_node *where,
               llist_node *node);
void llist_remove(llist *ll, llist_node *node);
llist_node *llist_head(llist *ll);
llist_node *llist_tail(llist *ll);
llist_node *llist_current(llist *ll);
llist_node *llist_previous(llist *ll);
llist_node *llist_next(llist *ll);
llist_node *llist_find(llist *ll, llist_node *needle);
llist_node *llist_find_payload(llist *ll, void *payload);

  /*
   *  LLIST_NODE functions
   */

llist_node *llist_node_new(void *payload);

#endif //LLIST_H
