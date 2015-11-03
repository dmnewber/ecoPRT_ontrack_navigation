/* Ring buffer test file */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Ring.h"

/* List constructor */
void listCreate(List_t **head)
{
  /* List_t structure place holder for new data */
  List_t * new, *current=*head;

  /* Allocate to memory and get pointer */
  new = malloc(sizeof(List_t));

  /* If the head pointer is non-existent, put new in the head position */
  if(*head==NULL)
  {
    new->next = NULL;
    new->prev = NULL;
    *head = new;
  }
  else
  {
    /* Set placehold previous to the new struct */
    current->prev = new;

    /* Set the address of head as the next node */
    new->next = *head;

    /* Set the previous address as null */
    new->prev = NULL;

    /* Set new as the new head */
    *head = new;
  }

}


/* List reader */
void listRead(List_t *head)
{
  /* List iterator to be read */
  List_t * read;

  /* While read exists, print the data */
  for(read = head; read!=NULL; read=read->next)
  {
    printf("%d\n",read->data);
  }
}

void ringInit(List_t **head, int ringSize)
{
  int i;
  List_t *endoflist, *startoflist;

  /* Create a list with ringSize elements */
  for(i=0;i<ringSize;i++)
  {
    listCreate(head);
  }
  startoflist = *head;
  endoflist = *head;

  /* Iterate through the new list and link the end of the list to the head */
  /* Also initialize all data to 0 */
  for(i=0;i<ringSize-1;i++)
  {
    endoflist = endoflist->next;
  }
  endoflist->next = *head;
  startoflist->prev = endoflist;
}

/* Read through the ring buffer and stop at the end */
void ringRead(List_t **head, int ringSize)
{
  int i; //iteration variable
  List_t *read=*head;

  /* Iterate backwards through the ring */
  for(i=0;i<ringSize;i++)
  {
    printf("%d\n",read->Data);
    read = read->prev;
  }
}

void ringPush(List_t **head, Data_t data)
{
  List_t *add = *head;

  /* Move head struct */
  add = add->next;

  /* Add data to struct */
  add->Data = data;

  /* Update header */
  *head = add;
}

void getPrevious(List_t **current)
{
  /* Place holder list */
  List_t *hold;

  /* Set current list into hold */
  hold = *current;

  /* Move hold to the previous list item */
  hold = hold->prev;

  /* Set dereferenced current to hold */
  *current = hold;

}
