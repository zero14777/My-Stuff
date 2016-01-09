// $Id: queue.c,v 1.1 2013-11-17 01:22:28-08 - - $
//
// Peter Greer-Berezovsky pbgreerb
//
// NAME
//   queue
//
// SYNOPSIS
//   Contains type definitions for queue and functions pertaining 
//   to the queue structure.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

#define STUBPRINTF(...) fprintf (stderr, __VA_ARGS__);

typedef struct queue_node queue_node;
struct queue_node {
   queue_item_t item;
   queue_node *link;
};

struct queue {
   queue_node *front;
   queue_node *rear;
};

//Creates and returns a new queue containing nothing.

queue *new_queue (void) {
   queue *newQueue = malloc(sizeof(queue));
   assert(newQueue != NULL);
   newQueue->front = NULL;
   newQueue->rear = NULL;
   return newQueue;
}

//Frees a queue from memory.

void free_queue (queue *this) {
   assert (isempty_queue (this));
   free (this);
}

//Takes in a queue item to be stored and a queue to store it to.
//Creates a new node, stores the given item in the node, and then
//places the node at the rear of the list.

void insert_queue (queue *this, queue_item_t item) {
   assert(this != NULL);
   queue_node *newNode = malloc(sizeof(queue_node));
   assert(newNode != NULL);
   newNode->item = item;
   newNode->link = NULL;
   if(this->front == NULL){
      this->front = newNode;
   } else {
      this->rear->link = newNode;
   }
   this->rear = newNode;
}

//Removes a queue node and returns the pointer to the value it stored.

queue_item_t remove_queue (queue *this) {
   assert (! isempty_queue (this));
   queue_item_t frontItem = this->front->item;
   queue_node *remover = this->front;
   this->front = this->front->link;
   free(remover);
   
   return frontItem;
}

// Will return true if the queue contains no nodes, or false if the
// queue does contain nodes.

bool isempty_queue (queue *this) {
   assert (this != NULL);
   if(this->front == NULL){
      return true;
   }
   return false;
}
