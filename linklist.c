
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
