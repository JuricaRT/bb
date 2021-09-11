// Generating Candidate Busy Beaver Machines (Or How to Build the Zany Zoo) - https://arxiv.org/abs/1610.03184

/* *** DEFINITIONS ***

TRANSITION FORM - (State, Input, Output, Direction, NewState)

if NewState == z (halting state) then it is a 'halting transition' otherwise it is a 'standard transition'

We say a Turing machine M is
 - K-HALTING if there are k transitions of the form (_,_,_,_,z) in M.
 
 - EXHAUSTIVE if transition function is a total function which means that it produces output for each state and symbol combination
   - DELTA(q, y) = <q', y', D>

 - MAXIMISING if for every tuple of the form (_,_,O,_,z) in M, O != 0
 - MINIMISING if for every tuple of the form (_,_,O,_,z) in M, O = 1
 - i.e the halting transition, can be guaranteed (not) to reduce the number of non-blank symbols on the tape

- WHEN SEARCHING FOR BB MACHINES, IT SEEMS NATURAL TO FOCUS ON MACHINES WHICH ARE 1-halting, exhaustive AND maximising

- Why 1-halting? - As there is only one input on which the machine is evaluated (the blank input),
  there is only ever one halting transition that can be executed
- Why exhaustive? - For similar reasons, as non-exhaustive machines are potentially wasteful
- Why maximising - trivial

- It should be noted that we have a rather unusual property for Turing machines, which is that we are only interested in
  their execution on the blank input - we can make some significant reductions on the machines that need to be considered

- we say M has ACTIVITY k if the execution of M on the blank input terminates in k steps
- we say M has PRODUCTIVITY is the number of non-blank characters on the tape after M has halted
- productivity for machine that never halts is undefined because activity for such machine is infinite

- we say a Turing machine M SATISFIES THE BLANK TAPE CONDITION iff during the computation of M on the blank input, the only
  configuration in which the tape is blank is the intial configuration

- we say an n-state Turing machine M where n >= 2 is IRRELEVANT TO THE BUSY BEAVER PROBLEM if at least one of the following
  conditions is satisfied otherwise it is RELEVANT TO THE BUSY BEAVER PROBLEM
  - activity(M) = infinity
  - activity(M) <= n
  - productivity(M) = 0
  - M does not satisfy the blank tape condition

- chapter 4 shows that searching quintuple machines will find a superset of those found by searching quadruple machines,
  i.e. searching quintuple machines subsumes searching for quadruple ones

- in order to solve the busy beaver problem, we are only interested in evaluating machines on the blank input, which
  means we are able to reduce the number of machines that require non trivial analysis


- Lemma 15. Let M be a Turing machine containing a tuple of the form (a,0,_,_,a). Then the activity of M is infinite.
- Lemma 16. Let M be a Turing machine containing a tuple of the form (a,0,_,_,z). Then the activity of M is 1.
- hence we need only to consider machines whose first transition is of the form (a,0,_,_,b), i.e. State != NewState && NewState != z.


- Lemma 17. Let M be a k-halting n-state m-symbol Turing machine containing a tuple of the form (a,0,_,_,S) where S != {a,b,z}.
            Then there is another k-halting n-state m-symbol Turing machine M' containing the tuple (a,0,_,_b) such that M and M'
            are productivity and activity equivalent.
  Proof. Let M' be the machine found by swapping all the occurrences of S and b in M. The result the trivially follows.
- this means that we can insist that the second state encountered in the machine (after the start state a) is b. Similarly we can
  insist that the third state (if any) encountered is c, and so on.


- Lemma 18. Let M be a Turing machine, and let S1 and S2 be two distinct states in M such that S1, S2 != {a,z}. Let M' be the machine
  obtained by swapping the states S1 and S2 in every transition in M. Then M' is productivity and activity equivalent to M and M'
  contains the same number of states, symbols and halting transitions as M.
- it is similarly straightforward to show a similar result for symbols.

- Lemma 19. Let M be a Turing machine and let O1 and O2 be two distinct symbols in M such that O1, O2 != 0. Let M' be the machine
  obtained by swapping the symbols O1 and O2 in every transition in M. Then M' is productivity and activity equivalent to M and M'
  contains the same number of states, symbols and halting transitions as M.

- taken together, Lemmas 18 and 19 mean that we can insist on a specific order in which states and symbols appear in the execution
  of the machine. In particular, given that the first state must be a, we can insist that the second state encountered be b,
  the third one c and so forth. Similarly, we can insist that the first non-blank symbol countered be 1, the second 2, and so on.
  In addition, as we know that the second step executed will always be the b, 0 transition, we can insist that in any transition of
  the form (b,0,O,D,S) we have O element of {0,1,2}. This means that we can assume that the first transition is of the form
  (a,0,O,D,b) for some output O and some direction D. Now if O is blank the transition is of the form (a,0,0,D,b), then either the
  tape remains blank throughout the entire execution of the machine or there is a transition (s,0,O,D,NS) where s != a and O != 0,
  in which case we simply swap a and s.

- Lemma 20. Let M be a k-halting n-state m-symbol Turing machine of finite activity and productivity >= 1 containing a tuple of the
            form (a,0,0,_,NS) where NS != z. Then there is another k-halting n-state m-symbol Turing machine M' of finite activity
            containing the tuple (a,0,O,_,_) where O != 0 such that M' is productivity equivalent of M.

  Proof. As productivity(M) >= 1, there must be a transition of the form (S,0,O,_,_) where S != a and O != 0 in M, and that this
         is the first transition in the execution of M which writes a non-blank symbol on the tape. Let M' be the machine found
         by swapping all occurrences of a and S in M. The result then trivially follows.

- Lemma 22. Let M be a Turing machine containing tuples of th eform (a,0,_,r,b) and (b,0,_,r_,S) where S is element of {a,b}.
            Then M has infinite activity.

- Definition 23. An n-state Turing machine is 0-dextrous if there are n transitions of the form (_,0_,_,_) and all n such transitions
                 are of the form (_,0,_,r,_).
                 The reason that we identify the 0-dextrous class of machines is that they are all irrelevant.

- Lemma 24. Let M be a 0-dextrous Turing machine. Then M is irrelevant to the busy beaver problem.

  Proof. Let the number of states in M be n. If M does not terminate on the blank input, then its activity is infinite and M is hence
         irrelevant. Otherwise, M terminates on the blank input, which, as M is 0-dextrous, is only possible if there is a transition
         in M of the form (_,0,_,r,z). This means that M halts in at most n steps, and so activity(M) <= n, which means
         that M is irrelevant.

  As this property requires that at least all of the transitions for input 0 are known, this can generally only be implemented
  as a constraint on the final machine, i.e. that any 0-dextrous machine that is generated is ignored. 


-  Definition 25. A Turing machine M is normal iff it has all of the following properties:

     - M contains the tuple (a,0,1,r,b)

     - M contains either a tuple of the form (b,0,O,l,S) where S in {a,b,c} and O in {0,1,2}, or a tuple of the form (b,0,O,r,c) where
       O in {0,1,2}.

     - M is not 0-dextrous

     - When executing M on the blank input,
       - states are encountered in alphabetical order
       - symbols are encountered in numerical order
       - the blank tape condition is satisfied

*/

#define N 2 // states
#define M 4 // symbols

#define HALT_STATE 'z'
#define START_STATE 'a'
#define BLANK_SYMBOL 0

int main(int argc, char *argv[]){
  return 0;
}
