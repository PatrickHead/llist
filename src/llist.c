/*
 *  Copyright 2021,2024,2025 Patrick T. Head
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
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
 * @file llist.c
 * @brief Source code file for simple C library for doubly linked lists
 */

#include <stdlib.h>
#include <string.h>

#include "llist.h"

    /*
     * public functions
     */

  /**
   *  @fn llist_position llist_position_string_to_type(char *string)
   *
   *  @brief Converts @p string to @a llist_position value
   *
   *  @param string - string containing representation of @a llist_position value
   *
   *  @return @a llist_position value
   */

llist_position llist_position_string_to_type(char *string)
{
  if (!string) return llist_position_tail;

  if (!strcmp(string, "tail")) return llist_position_tail;
  else if (!strcmp(string, "head")) return llist_position_head;
  else if (!strcmp(string, "before")) return llist_position_before;
  else if (!strcmp(string, "after")) return llist_position_after;

  return llist_position_tail;
}

  /**
   *  @fn char *llist_position_type_to_string(llist_position position)
   *
   *  @brief Converts @a llist_position value to string representation
   *
   *  @param position - @a llist_position value
   *
   *  @return string containing representation of @a llist_position value
   */

char *llist_position_type_to_string(llist_position position)
{
  switch (position)
  {
    case llist_position_tail: return "tail";
    case llist_position_head: return "head";
    case llist_position_before: return "before";
    case llist_position_after: return "after";
  }

  return "tail";
}

  /**
   *  @fn llist *llist_new(void)
   *
   *  @brief Create a linked list struct
   *
   *  @par Parameters
   *       None.
   *
   *  @return pointer to new @a llist
   */

llist *llist_new(void)
{
  llist *ll = NULL;

  ll = malloc(sizeof(llist));
  if (ll) memset(ll, 0, sizeof(llist));

  return ll;
}

  /**
   *  @fn llist *llist_dup(llist *ll)
   *
   *  @brief Creates deep copy of @p ll
   *
   *  NOTE: If @p ll->dup_node is not set, then a shallow copy is performed.
   *        This can be useful when having different list sort or filters.
   *
   *  @param ll - pointer to @a llist struct
   *
   *  @return pointer to new @a llist
   */

llist *llist_dup(llist *ll)
{
  llist *new_ll = NULL;
  llist_node *node = NULL;
  llist_node *node_p = NULL;

  if (!ll) goto exit;

  if (!(new_ll = llist_new())) goto exit;

  llist_set_new(new_ll, ll->new_node);
  llist_set_dup(new_ll, ll->dup_node);
  llist_set_free(new_ll, ll->free_node);
  llist_set_cmp(new_ll, ll->cmp_node);

  node_p = ll->head;

  while (node_p)
  {
    if (!ll->dup_node) node = node_p;
    else if (!(node = ll->dup_node(node_p))) break;
    llist_add(new_ll, llist_position_tail, NULL, node);
    node_p = node_p->next;
  }

exit:
  return new_ll;
}

  /**
   *  @fn void llist_free(llist *ll)
   *
   *  @brief Frees all memory allocated to @p ll
   *
   *  NOTE:  This frees payload data in the list if ll->free_node is not NULL
   *
   *  @param ll - pointer to @a llist struct
   *
   *  @par Returns
   *       Nothing.
   */

void llist_free(llist *ll)
{
  llist_node *node, *next;

  if (!ll) goto exit;

  node = ll->head;

  while (node)
  {
    next = node->next;
    if (ll->free_node) ll->free_node(node);
    else free(node);
    node = next;
  }

  free(ll);

exit:
}

  /**
   *  @fn void llist_set_new(llist *ll, llist_node *(*new_func)(void))
   *
   *  @brief Sets new node function in @p ll
   *
   *  @param  ll - pointer to @a llist
   *  @param  new_func - pointer to function that creates new @a llist_node
   *
   *  @par Returns
   *       Nothing.
   */

void llist_set_new(llist *ll, llist_node *(*new_func)(void))
{
  if (ll) ll->new_node = new_func;
}

  /**
   *  @fn void llist_set_dup(llist *ll,
   *                         llist_node *(*dup_func)(llist_node *node))
   *
   *  @brief Sets node duplication function in @p ll
   *
   *  @param  ll - pointer to @a llist
   *  @param  dup_func - pointer to function that duplicates a @a llist_node
   *
   *  @par Returns
   *       Nothing.
   */

void llist_set_dup(llist *ll, llist_node *(*dup_func)(llist_node *node))
{
  if (ll) ll->dup_node = dup_func;
}

  /**
   *  @fn void llist_set_free(llist *ll, void (*free_func)(llist_node *node))
   *
   *  @brief Sets node free function in @p ll
   *
   *  @param  ll - pointer to @a llist
   *  @param  free_func - pointer to function that frees a @a llist_node
   *
   *  @par Returns
   *       Nothing.
   */

void llist_set_free(llist *ll, void (*free_func)(llist_node *node))
{
  if (ll) ll->free_node = free_func;
}

  /**
   *  @fn void llist_set_cmp(llist *ll,
   *                         int (*cmp_func)(llist_node *a, llist_node *b))
   *
   *  @brief Sets node value comparison function in @p ll
   *
   *  @param  ll - pointer to @a llist
   *  @param  cmp_func - pointer to function that compares @p a to @p b
   *
   *  @par Returns
   *       Nothing.
   */

void llist_set_cmp(llist *ll, int (*cmp_func)(llist_node *a, llist_node *b))
{
  if (ll) ll->cmp_node = cmp_func;
}

  /**
   *  @fn void llist_add(llist *ll,
   *                     llist_position position,
   *                     llist_node *where,
   *                     llist_node *node)
   *
   *  @brief Sets node value comparison function in @p ll
   *
   *  NOTE:  Memory assocated with @p node is the responsibility of the caller.
   *         When @p ll->dup_node is set, then @p node should be freed after
   *         it is added to @p ll.  When @p ll->dup_node is NOT set, then
   *         removing a node with <i> llist_remove </i> will result in a
   *         memory leak, unless the caller tracks the payload outside of @p ll.
   *
   *  NOTE:  ll->current will point to the added node on success
   *
   *  @param  ll - pointer to @a llist
   *  @param  position - llist_position value, fine tunes insertion point
   *  @param  where - @a llist_node in list to use as insertion reference
   *  @param  node - pointer to @a llist_node to insert into list
   *
   *  @par Returns
   *       Nothing.
   */

void llist_add(llist *ll,
               llist_position position,
               llist_node *where,
               llist_node *node)
{
  llist_node *added = NULL;

  if (!ll || !node) goto exit;

  if (!ll->dup_node) added = node;
  else if (!(added = ll->dup_node(node))) goto exit;

  added->previous = added->next = NULL;

  switch (position)
  {
    case llist_position_head:
      added->next = ll->head;
      if (ll->head) ll->head->previous = added;
      ll->head = added;
      if (!ll->tail) ll->tail = added;
      break;
    case llist_position_tail:
      added->previous = ll->tail;
      if (ll->tail) ll->tail->next = added;
      ll->tail = added;
      if (!ll->head) ll->head = added;
      break;
    case llist_position_before:
      if (!where) where = ll->current;
      if (!where) where = ll->head;
      if (!where) break;
      added->previous = where->previous;
      if (where->previous) where->previous->next = added;
      added->next = where;
      where->previous = added;
      if (ll->head == where) ll->head = added;
      break;
    case llist_position_after:
      if (!where) where = ll->current;
      if (!where) where = ll->tail;
      if (!where) break;
      added->previous = where;
      added->next = where->next;
      where->next = added;
      if (ll->tail == where) ll->tail = added;
      break;
  }

  if (!ll->head) ll->head = added;
  if (!ll->tail) ll->tail = added;
  ll->current = added;

exit:
}

  /**
   *  @fn void llist_remove(llist *ll, llist_node *node)
   *
   *  @brief Deletes @p node from @p ll if it exists in the list
   *
   *  NOTE:  ll->current will point to ll->head on if ll->current is @p node
   *
   *  @param  ll - pointer to @a llist
   *  @param  node - pointer to @a llist_node
   *
   *  @par Returns
   *       Nothing.
   */

void llist_remove(llist *ll, llist_node *node)
{
  llist_node *located = NULL;

  if (!ll || !node) goto exit;

  located = ll->head;
  while (located)
  {
    if (located == node)
    {
      if (located->next) located->next->previous = located->previous;
      if (located->previous) located->previous->next = located->next;
      if (ll->head == located) ll->head = located->next;
      if (ll->tail == located) ll->tail = located->previous;
      if (ll->free_node) ll->free_node(located);
      else free(located);
      break;
    }
    located = located->next;
  }

  if (ll->current == located)
    ll->current = ll->head;

exit:
}

  /**
   *  @fn llist_node *llist_head(llist *ll)
   *
   *  @brief Returns the head of @p ll, if any
   *
   *  @param  ll - pointer to @a llist
   *
   *  @return pointer to @a llist_node, or NULL on empty list or failure
   */

llist_node *llist_head(llist *ll) { return ll ? ll->current = ll->head : NULL; }

  /**
   *  @fn llist_node *llist_tail(llist *ll)
   *
   *  @brief Returns the tail of @p ll, if any
   *
   *  @param  ll - pointer to @a llist
   *
   *  @return pointer to @a llist_node, or NULL on empty list or failure
   */

llist_node *llist_tail(llist *ll) { return ll ? ll->current = ll->tail : NULL; }

  /**
   *  @fn llist_node *llist_current(llist *ll)
   *
   *  @brief Returns the current @a llist_node in @p ll
   *
   *  @param  ll - pointer to @a llist
   *
   *  @return pointer to @a llist_node, or NULL on empty list or failure
   */

llist_node *llist_current(llist *ll) { return ll ? ll->current : NULL; }

  /**
   *  @fn llist_node *llist_previous(llist *ll)
   *
   *  @brief Returns the node previous to the @p ll current node
   *
   *  @param  ll - pointer to @a llist
   *
   *  @return pointer to @a llist_node, or NULL on empty list or failure
   */

llist_node *llist_previous(llist *ll)
{
  return (ll && ll->current) ? ll->current = ll->current->previous : NULL;
}

  /**
   *  @fn llist_node *llist_next(llist *ll)
   *
   *  @brief Returns the node after the @p ll current node
   *
   *  @param  ll - pointer to @a llist
   *
   *  @return pointer to @a llist_node, or NULL on empty list or failure
   */

llist_node *llist_next(llist *ll)
{
  return (ll && ll->current) ? ll->current = ll->current->next : NULL;
}

  /**
   *  @fn llist_node *llist_find(llist *ll, llist_node *needle)
   *
   *  @brief Searches for first @p ll node that has the same value as @p needle
   *
   *  NOTE:  @p ll compare node function must be set before calling this function
   *
   *  @param  ll - pointer to @a llist
   *  @param  needle - @a llist_node that contains payload value to search for
   *
   *  @return pointer to @a llist_node, or NULL if not found
   */

llist_node *llist_find(llist *ll, llist_node *needle)
{
  llist_node *node = NULL;

  if (!ll || !needle) goto exit;
  if (!ll->cmp_node) goto exit;

  node = ll->head;
  while (node)
  {
    if (!ll->cmp_node(node, needle)) break;
    node = node->next;
  }

exit:
  return node;
}

  /**
   *  @fn llist_node *llist_find_payload(llist *ll, void *payload)
   *
   *  @brief Searches for first @p ll node contains *p payload pointer
   *
   *  @param  ll - pointer to @a llist
   *  @param  payload - @a void @a * that contains payload pointer
   *
   *  @return pointer to @a llist_node, or NULL if not found
   */

llist_node *llist_find_payload(llist *ll, void *payload)
{
  llist_node *node = NULL;

  if (!ll || !payload) goto exit;
  if (!ll->cmp_node) goto exit;

  node = ll->head;
  while (node)
  {
    if (node->payload == payload) break;
    node = node->next;
  }

exit:
  return node;
}

  /**
   *  @fn llist_node *llist_node_new(void *payload)
   *
   *  @brief Creates a 'blank' @a llist_node struct
   *
   *  @param payload - points to user supplied payload for new @a llist_node
   *
   *  @return pointer to @a llist_node, or NULL if not found
   */

llist_node *llist_node_new(void *payload)
{
  llist_node *node = malloc(sizeof(llist_node));
  if (node) memset(node, 0, sizeof(llist_node));

  node->payload = payload;

  return node;
}

