// Generating Candidate Busy Beaver Machines (Or How to Build the Zany Zoo) - https://arxiv.org/abs/1610.03184

/* *** DEFINITIONS ***

TRANSITION FORM - (State, Input, Output, Direction, NewState)

We say a Turing machine M is
1. K-HALTING if there are k transitions of the form (_,_,_,_,z) in M.
2. EXHAUSTIVE if transition function is TOTAL function -
    for every pair (State, Symbol), there exists at least one matching triplet (NewState, NewSymbol, Direction).
3. N-STATE FULL if |Q| = n.
4. M-SYMBOL FULL if |GAMMA| = m.
5. MAXIMISING if for every tuple of the form (_,_,O,_,z) in M, O != 0.
6. MINIMISING if for every tuple of the form (_,_,O,_,z) in M, O is 0.

NOTES:
- machines which are EXHAUSTIVE but not 1-HALTING are either guaranteed not to terminate (as there is no transition into halting state) or
  have multiple halting transitions, of which only one can ever be used in a given computation, making the other halting transitions
  spurious.

- the final transition that is executed in a terminating computation in a MAXIMISING machine, i.e. the halting transition, can be
  guaranteed not to reduce the number of non-blank symbols on the tape.

- when generating machines, we can satisfy the requirement for the machine to be MAXIMISING by ensuring that
  O = 1 in any halting transition.

- if there is more than one halting transition, it is possible for the machine to be neither MAXIMISING nor MINIMISING.

- when searching for busy beaver machines, it seems natural to focus on machines which are 1-HALTING, EXHAUSTIVE and MAXIMISING.

- we may assume that the (unique) halting transition is always of the form (S,I,1,_,z).

- there are the same number of 5-states 2-symbols machines as there are with 2-states 5-symbols, this is why we will speak of generating
  machines of a given dimension rather than given number of states.

*/

#define N 2 // states
#define M 4 // symbols

#define HALT_STATE 'z'
#define START_STATE 'a'
#define BLANK_SYMBOL 0

int main(int argc, char *argv[]){
  return 0;
}
