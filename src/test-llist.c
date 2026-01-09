#include <stdlib.h>
#include <stdio.h> 
#include <string.h> 

#include "llist.h"

typedef struct item item;

struct item
{
  int id;
  char *name;
};

llist_node *new_node(void);
llist_node *dup_node(llist_node *node);
void free_node(llist_node *node);
int cmp_node(llist_node *a, llist_node *b);
void print_llist(llist *ll);

int _id = 0;

int main() 
{ 
  llist *ll = NULL;
  llist *ll_dup = NULL;
  llist_node *node = NULL;
  item it = { 0, NULL };
  llist_node needle = { NULL, NULL, &it };
  int i;

  printf("llist_new()\n");
  ll = llist_new();
  printf("ll = %p\n", ll);

  print_llist(ll);

  printf("llist_set_new(%p, %p)\n", ll, new_node);
  llist_set_new(ll, new_node);

  printf("llist_set_dup(%p, %p)\n", ll, dup_node);
  llist_set_dup(ll, dup_node);

  printf("llist_set_free(%p, %p)\n", ll, free_node);
  llist_set_free(ll, free_node);

  printf("llist_set_cmp(%p, %p)\n", ll, cmp_node);
  llist_set_cmp(ll, cmp_node);

  for (i = 0; i < 10; i++)
  {
    node = new_node();
    printf("llist_add(%p, %d, %p, %p)\n", ll, llist_position_tail, NULL, node);
    llist_add(ll, llist_position_tail, NULL, node);
    free_node(node);
  }

  print_llist(ll);

  it.id = 5;
  printf("llist_find(%p, &needle)\n", ll);
  node = llist_find(ll, &needle);
  printf("node=%p: payload(it)->id=%d\n", node, ((item *)node->payload)->id);

  printf("llist_remove(%p, %p)\n", ll, node);
  llist_remove(ll, node);

  print_llist(ll);

  it.id = 7;
  node = llist_find(ll, &needle);
  it.id = 99;
  it.name = "Howdy";
  printf("llist_add(%p, %d, %p, %p)\n", ll, llist_position_before, node, &needle);
  llist_add(ll, llist_position_before, node, &needle);

  print_llist(ll);

  it.id = 98;
  it.name = "Doody";
  printf("llist_add(%p, %d, %p, %p)\n", ll, llist_position_after, node, &needle);
  llist_add(ll, llist_position_after, node, &needle);

  print_llist(ll);

  node = llist_head(ll);
  it.id = -1;
  it.name = "BOOM1";
  printf("llist_add(%p, %d, %p, %p)\n", ll, llist_position_before, node, &needle);
  llist_add(ll, llist_position_before, node, &needle);

  print_llist(ll);

  node = llist_tail(ll);
  it.id = -2;
  it.name = "BOOM2";
  printf("llist_add(%p, %d, %p, %p)\n", ll, llist_position_after, node, &needle);
  llist_add(ll, llist_position_after, node, &needle);

  print_llist(ll);

  printf("llist_dup(%p)\n", ll);
  ll_dup = llist_dup(ll);

  print_llist(ll_dup);

  printf("llist_free(%p)\n", ll);
  llist_free(ll);

  printf("llist_free(%p)\n", ll_dup);
  llist_free(ll_dup);

  return 0; 
} 

llist_node *new_node(void)
{
  llist_node *node = NULL;
  item *it = NULL;
  char name[50];

  if (!(it = malloc(sizeof(item)))) goto exit;

  it->id = ++_id;
  sprintf(name, "name%d", _id);
  it->name = strdup(name);

  node = llist_node_new((void *)it);

exit:
  return node;
}

llist_node *dup_node(llist_node *node)
{
  llist_node *new_node = NULL;
  item *node_it = NULL;
  item *it = NULL;

  if (!node) goto exit;

  if (node->payload)
  {
    it = malloc(sizeof(item));
    node_it = (item *)node->payload;
    memcpy(it, node_it, sizeof(item));
    if (node_it->name) it->name = strdup(node_it->name);
  }

  new_node = llist_node_new((void *)it);

exit:
  return new_node;
}

void free_node(llist_node *node)
{
  item *it;

  if (!node) return;

  if (node->payload)
  {
    it = (item *)node->payload;
    if (it->name) free(it->name);
    free(it);
  }

  free(node);
}

int cmp_node(llist_node *a, llist_node *b)
{
  item *a_it, *b_it;

  if (!a || !b) return 0;

  a_it = (item *)a->payload;
  b_it = (item *)b->payload;

  if (!a_it || !b_it) return 0;

  if (a_it->id < b_it->id) return -1;
  if (a_it->id > b_it->id) return 1;
  return 0;
}

void print_llist(llist *ll)
{
  llist_node *node = NULL;
  item *it;

  if (!ll) return;

  printf("LLIST:\n");
  printf("  head=%p\n", ll->head);
  printf("  tail=%p\n", ll->tail);
  printf("  current=%p\n", ll->current);
  printf("  new_node=%p\n", ll->new_node);
  printf("  dup_node=%p\n", ll->dup_node);
  printf("  free_node=%p\n", ll->free_node);
  printf("  cmp_node=%p\n", ll->cmp_node);
  printf("  NODES:\n");

  node = ll->head;

  while (node)
  {
    it = (item *)node->payload;
    printf("    NODE (%p)\n", node);
    if (it)
    {
      printf("      id=%d\n", it->id);
      printf("      name=%s\n", it->name);
    }
    node = node->next;
  }
}

