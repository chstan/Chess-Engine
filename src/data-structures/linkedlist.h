#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct {
	void *next;
	void *prev;
	void *payload;
} listNode;


listNode *nextNode(listNode *currentNode);

listNode *prevNode(listNode *currentNode);

listNode *createList();

void map(listNode *linkedList, void(*mapfn)(void *, void *), void *aux);

listNode *deleteList(listNode *toDelete);

listNode *insertNode(listNode *destination, listNode *toInsert);

listNode *removeNode(listNode *toRemove);

#endif
