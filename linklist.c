
#include <stdio.h>
#include <stdlib.h>

#include "d_linklist.h" 

int linkList_create(linklist_pt *list)
{       
        linklist_pt kLinkList = (linklist_pt) malloc( sizeof(*kLinkList) );
        if(kLinkList)
        {       
                kLinkList->header = (linklistentry_pt) malloc( sizeof(*(kLinkList->header)) );
                if(kLinkList->header)
                {       
                        kLinkList->header->elem = NULL;
                        kLinkList->header->next = kLinkList->header;
                        kLinkList->header->prev = kLinkList->header;
                        kLinkList->size = 0;
                        
                        *list = kLinkList;
                        
                        return 0;
                }
        }
        return -1;
}
void * linkList_getFirst(linklist_pt list)
{       
        if (list->size == 0)
                return NULL;

        return list->header->next->elem;
}


void * linkList_getLast(linklist_pt list)
{
        if (list->size == 0)
                return NULL;

        return list->header->prev->elem;
}


void * linkList_removeFirst(linklist_pt list)
{
        return linkList_removeEntry(list, list->header->next);
}


void * linkList_removeLast(linklist_pt list)
{
        return linkList_removeEntry(list, list->header->prev);
}

void linkList_addFirst(linklist_pt list, void * element)
{
        linkList_addBefore(list, element, list->header->next);
}


void linkList_addLast(linklist_pt list, void * element)
{
        linkList_addBefore(list, element, list->header);
}
void linkList_addLast(linklist_pt list, void * element)
{
        linkList_addBefore(list, element, list->header);
}


int linkList_size(linklist_pt list)
{
        return list->size;
}

int linkList_isEmpty(linklist_pt list)
{
        return linkList_size(list) == 0;
}

int linkList_addElement(linklist_pt list, void * element)
{
        linkList_addBefore(list, element, list->header);
        return 0;
}
int linkList_removeElement(linklist_pt list, void * element)
{
        if (element == NULL) {
                linklistentry_pt entry;
                for (entry = list->header->next; entry != list->header; entry = entry->next) {
                        if (entry->elem == NULL) {
                                linkList_removeEntry(list, entry);
                                return 0;
                        }
                }
        } else {
                linklistentry_pt entry;
                for (entry = list->header->next; entry != list->header; entry = entry->next) {
                        if (element == entry->elem) {
                                linkList_removeEntry(list, entry);
                                return 0;
                        }
                }
        }
        return -1;
}
void * linkList_removeIndex(linklist_pt list, int index)
{
        return linkList_removeEntry(list, linkList_entry(list, index));
}

void linkList_clear(linklist_pt list)
{
        linklistentry_pt entry = list->header->next;
        while (entry != list->header) {
                linklistentry_pt next = entry->next;
                entry->next = entry->prev = NULL;
                // free(entry->element);
                entry->elem = NULL;
                free(entry);
                entry = next;
        }
        list->header->next = list->header->prev = list->header;
        list->size = 0;
}

void * linkList_get(linklist_pt list, int index)
{
        return linkList_entry(list, index)->elem;
}


void * linkList_set(linklist_pt list, int index, void * element)
{
        linklistentry_pt entry = linkList_entry(list, index);
        void * old = entry->elem;
        entry->elem = element;
        return old;
}


linklistentry_pt linkList_entry(linklist_pt list, int index) {
        linklistentry_pt entry;
        int i;
        if (index < 0 || index >= list->size) {
                return NULL;
        }

        entry = list->header;
        if (index < (list->size >> 1)) {
                for (i = 0; i <= index; i++) {
                        entry = entry->next;
                }
        } else {
                for (i = list->size; i > index; i--) {
                        entry = entry->prev;
                }
        }
        return entry;
}
int linkList_indexOf(linklist_pt list, void * element) {
        int index = 0;
        if (element == NULL) {
                linklistentry_pt entry;
                for (entry = list->header->next; entry != list->header; entry = entry->next) {
                        if (entry->elem == NULL) {
                                return index;
                        }
                        index++;
                }
        } else {
                linklistentry_pt entry;
                for (entry = list->header->next; entry != list->header; entry = entry->next) {
                        if (element == entry->elem) {
                                return index;
                        }
                        index++;
                }
        }
        return -1;
}
linklistentry_pt linkList_addBefore(linklist_pt list, void * element, linklistentry_pt entry)
{
        linklistentry_pt new = NULL;

        new = (linklistentry_pt) malloc (sizeof(*new));
        if (new)
        {
                new->elem = element;
                new->next = entry;
                new->prev = entry->prev;

                new->prev->next = new;
                new->next->prev = new;

                list->size++;
                return new;
        }

        return NULL;
}



void * linkList_removeEntry(linklist_pt list, linklistentry_pt entry)
{
        void * result;
        if (entry == list->header)
                return NULL;

        result = entry->elem;

        entry->prev->next = entry->next;
        entry->next->prev = entry->prev;

        entry->next = entry->prev = NULL;       
        free(entry);

        list->size--;

        return result;
}
     
        
