#include"Dstates.h"
#include"new.h"
Dstates Dstates_new()
{
    Dstates p;
    NEW(p);
    p->front = 0;
    p->tail = 0;
    return p;
}

int Dstates_isEmpty(Dstates dstates)
{
    return dstates->front == 0;
}

States Dstates_pop(Dstates dstates)
{
    if(Dstates_isEmpty(dstates))
        return 0;
    DstatesNode tNode = dstates->front;
    States s = tNode->states;
    dstates->front = tNode->next;
    free(tNode);
    return s;
}

void Dstates_push(Dstates dstates,States states)
{
    DstatesNode p;
    NEW(p);
    p->next = 0;
    p->states = states;
    if(Dstates_isEmpty(dstates))
    {
        dstates->front = p;
        dstates->tail = p;
    }
    else
    {
        dstates->tail->next = p;
        dstates->tail = p;
    }
}



