#include <assert.h>
#include <stdio.h>
#include "nfa.h"
#include "new.h"
#include "Dstates.h"

static int countOfStates=0;








int  States_isEqu(Nfa_t nfa,States a,States b)
{

    int i = 0;

    for( i = 0;i <= nfa->accept;i++)
    {

      if(a->states[i] != b->states[i])
          return 0;
    }

    return 1;
}
void addState(Nfa_t nfa, Node_t state,States s)
{
    s->states[state->num] = 1;
    Edge_t edge;
    for(edge = state->edges;edge ; edge = edge->next)
    {
        if(edge->c == EPS && s->states[edge->to->num] == 0)
            addState(nfa,edge->to,s);
    }

}

States Nfa_espClosure(Nfa_t nfa,States s )
{
    States newStates = malloc(sizeof(struct States));

    newStates->states = malloc(sizeof(char)*(nfa->accept+1));
    int i;
    for(i = 0;i <= nfa->accept;i++)
    {
        newStates->states[i] = 0;
    }
    Node_t node = nfa->nodes;
    while(node)
    {
        if( s->states[node->num] == 1)
        {
            addState(nfa,node,newStates);
        }
        node = node->next;
    }
    return newStates;
}

States Nfa_move(Nfa_t nfa,States T,int a)
{
    int isChanged = 0;
    States M = malloc(sizeof(struct States));
    M->states = malloc(sizeof(char)*(nfa->accept+1));
    int i = 0;
    for(i = 0;i <= nfa->accept;i++)
    {
        M->states[i] = 0;
    }
    M->isAccept = 0;
    for(i = 0; i <= nfa->accept;i++)
    {
        if(T->states[i] == 1)
        {
            Node_t node = Nfa_lookupOrInsert(nfa,i);

            Edge_t edge = node->edges;
            while(edge)
            {
                if(edge->c == a)
                {
                    M->states[edge->to->num] = 1;
                    isChanged = 1;
                }
                edge = edge->next;
            }
        }
    }
    if(isChanged)
        return M;
    else
        return 0;

}
//return the states
States Dstates_findStates(Nfa_t nfa, States states,Dstates Ds)
{
    DstatesNode node = Ds->head;
    while(node)
    {
        if(States_isEqu(nfa,node->states,states))
            return node->states;
        node = node->next;
    }
    return 0;
}


void PrintStates(Nfa_t nfa,States states)
{
    printf("the num is :%d\n",states->num);
    int i=0;
    for(i=0;i<=nfa->accept;i++)
    {
        printf("%d ",states->states[i]);
    }
    printf("\n");
}
Nfa_t NfaToDfa(Nfa_t nfa)
{
    Nfa_t dfa = Nfa_new();
    countOfStates = 0;
    States s0 = (States) malloc(sizeof(struct States));
    s0->states = (char*) malloc(sizeof(char)*(nfa->accept+1));
    int i = 0;
    for(i = 0; i <= nfa->accept;i++)
    {
        s0->states [i] = 0;
    }
    s0->states[0] = 1;

    Dstates dstates = Dstates_new();
    s0 = Nfa_espClosure(nfa,s0);
    s0->num = countOfStates++;
    s0->set = 0;
    s0->isAccept = 0;
    Dstates_push(dstates,s0);

    while(!Dstates_isEmpty(dstates))
    {
        States T = Dstates_pop(dstates);

        int a;
        for( a=33;a <= 126 ;a++ )
        {
            States M = Nfa_move(nfa,T,a);

            if(M == 0)
                continue;
            //PrintStates(nfa,M);
            States U = Nfa_espClosure(nfa,M);
            //PrintStates(nfa,U);
            States F = Dstates_findStates(nfa,U,dstates);

            if(!F)
            {
               // printf("%d\n",U->num);
                U->num = countOfStates++;
                for(i = 0;i <= nfa->accept;i++)
                {
                    if(U->states[i] == 1 && i == nfa->accept)
                    {
                        U->isAccept = 1;
                        break;
                    }
                }
                Dstates_push(dstates,U);

            }
            else
            {
                 U->num = F->num;
                 U->isAccept = F->isAccept;
            }

            Nfa_addEdge(dfa,T->num,U->num,a);
            Nfa_lookupOrInsert(dfa,U->num)->isAccept = U->isAccept;
        }
    }
    return dfa;
    //return 0;
}

Nfa_t Hopcroft(Nfa_t dfa)
{

}

// list head insert
static Node_t Node_new (int num, Node_t n)
{
  Node_t p;
  NEW(p);
  p->num = num;
  p->visited = 0;
  p->edges = 0;
  p->next = n;
  p->isAccept = 0;
  return p;
}

static Edge_t Edge_new (Node_t from
                        , Node_t to
                        , int c
                        , Edge_t edge)
{
  Edge_t p;
  NEW(p);
  p->c = c;
  p->from = from;
  p->to = to;
  p->next = edge;
  return p;
}

Nfa_t Nfa_new ()
{
  Nfa_t p;
  NEW(p);
  p->start = -1;
  p->accept = -1;
  p->nodes = 0;
  return p;
}

// lookup a node "num" in the nfa and return
// it; if that
// node does not exist, create a fresh one
// and return it.
Node_t Nfa_lookupOrInsert (Nfa_t nfa, int num)
{
  assert (nfa);
  Node_t nodes = nfa->nodes;
  while (nodes){
    if (nodes->num == num)
      return nodes;
    nodes = nodes->next;
  }
  Node_t p = Node_new (num, nfa->nodes);
  nfa->nodes = p;
  return p;
}

void Nfa_addEdge(Nfa_t nfa, int from, int to, int c)
{
  Node_t f = Nfa_lookupOrInsert (nfa, from);
  Node_t t = Nfa_lookupOrInsert (nfa, to);
  f->edges = Edge_new (f, t, c, f->edges);
  return;
}

void Nfa_print (Nfa_t nfa)
{
  assert (nfa);
  Node_t nodes = nfa->nodes;
  assert (nodes);
  while (nodes){
    Edge_t edges = nodes->edges;
    while (edges){
      printf ("%d ----(%d)----> %d\n"
              , edges->from->num
              , edges->c
              , edges->to->num);

      edges = edges->next;
    }
    nodes = nodes->next;
  }
  return;
}
