#include "linkedlist.h"

listNode *nextNode(listNode *currentNode) {
	return (listNode *) currentNode->next;
}

listNode *prevNode(listNode *currentNode) {
	return (listNode *) currentNode->prev;
}

listNode *createList() {
	listNode *created = malloc(sizeof(listNode));
	listNode->prev = NULL;
	listNode->next = NULL;
	listNode->payload = NULL;
}

// will not delete payload data!
listNode *deleteList(listNode *toDelete) {
	// find the beginning
	while(prev(toDelete)) {
		toDelete = prevNode(toDelete);
	}
	
	listNode *nextNode = nextNode(toDelete);
	
	do {
		free(toDelete);
		toDelete = nextNode;
		nextNode = next(toDelete);
	} while(nextNode);
}

void map(listNode *linkedList, void(*mapfn)(void *, void *), void *aux) {
	while(prev(linkedList)) {
		linkedList = prevNode(linkedList);
	}
	while(linkedList) {
		mapfn(linkedList->payload, aux);
		linkedList = next(linkedList);
	}
}

listNode *insertNode(listNode *destination, listNode *toInsert) {
	if(next(destination))
		(next(destination))->prev = toInsert;
	toInsert->next = (next(destination));
	
	toInsert->prev = destination;
	destination->next = toInsert;
}

listNode *removeNode(listNode *toRemove) {
	if(prev(toRemove))
		(prev(toRemove))->next = toRemove->next;
	
	if(next(toRemove))
		(next(toRemove))->prev = toRemove->prev;
	
	free(toRemove);
}