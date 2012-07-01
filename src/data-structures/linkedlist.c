#include <stdlib.h>

#include "linkedlist.h"

listNode *nextNode(listNode *currentNode) {
	return (listNode *) currentNode->next;
}

listNode *prevNode(listNode *currentNode) {
	return (listNode *) currentNode->prev;
}

listNode *createList() {
	listNode *created = malloc(sizeof(listNode));
	created->prev = NULL;
	created->next = NULL;
	created->payload = NULL;
	
	return created;
}

void deleteList(listNode *toDelete) {
	// find the beginning
	while(prevNode(toDelete)) {
		toDelete = prevNode(toDelete);
	}
	
	listNode *next = nextNode(toDelete);
	
	do {
		free(toDelete);
		toDelete = next;
		next = nextNode(toDelete);
	} while(nextNode);
}

void map(listNode *linkedList, void(*mapfn)(void *, void *), void *aux) {
	while(prevNode(linkedList)) {
		linkedList = prevNode(linkedList);
	}
	while(linkedList) {
		mapfn(linkedList->payload, aux);
		linkedList = nextNode(linkedList);
	}
}

listNode *insertNode(listNode *destination, listNode *toInsert) {
	if(nextNode(destination))
		(nextNode(destination))->prev = toInsert;
	toInsert->next = (nextNode(destination));
	
	toInsert->prev = destination;
	destination->next = toInsert;
	
	return destination;
}

listNode *removeNode(listNode *toRemove) {
	listNode *toReturn = NULL;
	
	if(nextNode(toRemove)) {
		(nextNode(toRemove))->prev = toRemove->prev;
		toReturn = nextNode(toRemove);
	}
	
	if(prevNode(toRemove)) {
		(prevNode(toRemove))->next = toRemove->next;
		toReturn = prevNode(toRemove);
	}
	
	free(toRemove);
	return toReturn;
}
