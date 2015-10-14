#ifndef _DSTATES_
#define _DSTATES_
typedef struct States* States;
struct States
{
    char* states;
    int num;
    int set;
    int isAccept;
};

typedef struct DstatesNode* DstatesNode;
struct DstatesNode
{
    States states;
    DstatesNode next;
};

typedef struct Dstates* Dstates;
struct Dstates
{
    DstatesNode head;
    DstatesNode front;
    DstatesNode tail;
};
Dstates Dstates_new();
States Dstates_pop(Dstates dstates);
void Dstates_push(Dstates dstates,States states);
int Dstates_isEmpty(Dstates dstates);

#endif
