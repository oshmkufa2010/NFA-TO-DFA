#include <stdio.h>
#include "re.h"
#include "nfa.h"
int main (char argc, char **argv)
{
  // First, we make a data structure for this
  // regular expression:
  //    a b
  Re_t e = Re_Concat_new (Re_Char_new ('a')
                          , Re_Char_new ('b'));
  // and to print e1 out to make sure the
  // data structure is right: (note that
  // ASCII is now integers)
  printf ("\nthe regular expression is:\n");
  Re_print (e);

  // convert regular expressions to NFA:
  Nfa_t nfa = Re_thompson (e);
  // print the NFA out:
  printf ("\nthe NFA is:\n");
  Nfa_print (nfa);
  Nfa_t dfa=NfaToDfa(nfa);
  printf("\nDFA:\n");
  Nfa_print(dfa);
  // a second regular expression:
  //    a (b|c)*
  e = Re_Concat_new (Re_Char_new ('a')
                     , Re_Closure_new (Re_Alt_new(Re_Char_new ('b')
                                                  , Re_Char_new ('c'))));
  // print it
  printf ("\nthe second regular expression is:\n");
  Re_print (e);
  // convert it to NFA:
  nfa = Re_thompson (e);
  // print the NFA:
  printf ("\nthe second NFA is:\n");

  Nfa_print (nfa);
  dfa=NfaToDfa(nfa);
  printf("\nDFA:\n");
  Nfa_print(dfa);
  Node_t node = dfa->nodes;


  getchar();
  return 0;
}
