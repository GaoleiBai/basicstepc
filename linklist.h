
#ifndef D_LINKLIST_H
#define D_LINKLIST_H

struct linkListEntry {
        void * elem;
        struct linkListEntry * next;
        struct linkListEntry * prev;
};

struct linkList {
        struct linkListEntry * header;
        int size;
};

typedef struct linkListEntry * linklistentry_pt;
typedef struct linkList *       linklist_pt;


int    linkList_create(linklist_pt *list);
void * linkList_getFirst(linklist_pt list);
void * linkList_getLast(linklist_pt list);
void * linkList_removeFirst(linklist_pt list);
void * linkList_removeLast(linklist_pt list);
void   linkList_addFirst(linklist_pt list, void * element);
void   linkList_addLast(linklist_pt list, void * element);
int    linkList_size(linklist_pt list);
int    linkList_isEmpty(linklist_pt list);
int    linkList_addElement(linklist_pt list, void * element);
int    linkList_removeElement(linklist_pt list, void * element);
void * linkList_removeIndex(linklist_pt list, int index);
void   linkList_clear(linklist_pt list);
void * linkList_get(linklist_pt list, int index);
void * linkList_set(linklist_pt list, int index, void * element);
linklistentry_pt linkList_entry(linklist_pt list, int index);
int linkList_indexOf(linklist_pt list, void * element);
linklistentry_pt linkList_addBefore(linklist_pt list, void * element, linklistentry_pt entry);
void * linkList_removeEntry(linklist_pt list, linklistentry_pt entry);
