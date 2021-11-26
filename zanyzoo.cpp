// Generating Candidate Busy Beaver Machines (Or How to Build the Zany Zoo) - https://arxiv.org/abs/1610.03184

/*

Preliminaries and Definitions
-----------------------------

TRANSITION FORM - (State, Input, Output, Direction, NewState) (QUINTUPLE)
                - (State, Input, Output/Direction, NewState)  (QUADRUPLE)


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


Let M be a Turing machine, we say
1. M has ACTIVITY k if the execution of M on the blank input terminates in k steps.
2. the PRODUCTIVITY of M whose ACTIVITY is finite is the number of non-blank characters on the tape after M has halted.
3. M SATISFIES THE BLANK TAPE CONDITION iff during the computation of M on the blank input, the only configuration in which the
   tape is blank is the initial configuration.
4. an n-state M where n >= 2 is IRRELEVANT TO THE BUSY BEAVER PROBLEM if at least one of the following conditions is satisfied:
     a) activity(M) is INFINITE (M does not terminate on the blank input)
     b) activity(M) <= n (it is simple to construct n-state machines of activity n and productivity n)
     c) productivity(M) = 0 (M does not write any non-blank characters on the blank input)
     d) M DOES NOT SATISFY the blank tape condition (M may write non-blank characters but it "cancels them" by writing blank over them).
   otherwise it is RELEVANT TO THE BUSY BEAVER PROBLEM.



Quintuple versus Quadruple machines
-----------------------------------

- searching quintuple machines will find a superset of those found by searching quadruple machines.

- it is not clear that for a given n-state m-symbol quintuple machine there is an equivalent n-state m-symbol quadruple machine
  but with a little care, it is not overly difficult to show that is the case.

- an interesting point to note is that for an output transition (S,I,O,NS) the next configuration will have machine in
  state NS with input O, this means that if there are two output transitions in M of the form (S0,I,O1,S1) and (S1,O1,O2,S2), then
  we can obtain an equivalent machine by replacing the first transition with (S0,I,O2,S2), i.e. we reduced number of steps performed.
  

Let M be a quadruple Turing machine, we say
1. S0, I0 is in an OUTPUT CYCLE IN M if there is a sequence of output transitions in M of the form
   (S0,I0,O1,S1),(S1,O1,O2,S2),(S2,O2,O3,S3)...(Sk,Ok,Ii,Si) for some 0 <= i <= k.
2. S0, I0 is in an OUTPUT CHAIN IN M if there is a sequence of output transitions in M of the form
   (S0,I0,O1,S1),(S1,O1,O2,S2),(S2,O2,O3,S3)...(Sk-1,Ok-1,Ok,Sk) where there is no transition of the form (Sk,Ok,_,_) in M and
   sequence of output transitions in not output cycle.
3. S0, I0 is in an MOVEMENT CHAIN IN M if (S0, I0, D, S) is a movement transition, or there is a sequence of transitions in M of the form
   (S0,I0,O1,S1),(S1,O1,O2,S2),(S2,O2,O3,S3)...(Sk-1,Ok-1,Ok,Sk), (Sk,Ok,D,Sk+1) where (Sk,Ok,D,Sk+1) is a movement transition, and
   all other transitions are output transitions and they do not form a output cycle.

- note that a movement chain is basically an output chain which is followed by an appropriate movement transition.

- if M is a quadruple Turing machine then for any transition (S,I,_,_) in M, S,I is an either an output cycle, output chain or
  a movement chain in M.


Let M be a quadruple Turing machine. We say M is NORMALISED if for EVERY output transition (S,I,OD,NS) one of following condition holds
1. S = NS and I = OD
2. there is no transition for NS,OD
3. there is a movement transition in M of the form (NS,OD,_,_)

Proposition 12. Let M be a quadruple Turing Machine. Then there is a normalised machine M' which is productivity equivalent to M.  
Proof. Since any transition (S,I,_,_) in M, S,I is an either output cycle, output chain or a movement chain in M, we generate M' from M
       - if (S,I,OD,NS) e M and S,I is in an output cycle, replace (S,I,OD,NS) with (S,I,I,S).
       - if (S,I,OD,NS) e M and S,I is in an output chain, replace (S,I,OD,NS) with (S,I,Ok,Sk).
       - if (S,I,OD,NS) e M and S,I is in an movement chain replace (S,I,OD,NS) with (S,I,Ok,Sk). (next transition is (Sk,Ok,D,Sk+1))
       
- clearly, if an output cycle is encountered during execution, then the machine will never terminate.

Proposition 14. Let M be a normalised quadruple machine containing no transitions of the form (S,I,I,S). Then there is a quintuple
                machine M' which is productivity equivalent to M.
Proof. As M is normalised and does not contain a transition of the form (S,I,I,S), then every transition is either
       1. a movement transition (S,I,D,NS)
       2. an output transition (S,I,O,NS) where there are no transitions for NS,O
       3. an output transition (S,I,O,NS) where the transition for NS,O is a movement transition, (NS,O,D,NS1).
       
       We construct M' by transforming each transition in M as follows
       1. for each movement transition (S,I,D,NS) in M, there is a transition (S,I,I,D,NS) in M'
       2. for each output transition (S,I,O,NS) in M where there are no transitions for NS,O in M, there is a transition (S,I,O,r,z) in M'
       3. for each output transition (S,I,O,NS) in M where the transition for NS,O is a movement transition (NS,O,D,NS1),
          there is a transition (S,I,O,D,NS1) in M'.

- hence any machines relevant to the busy beaver problem will be found by searching amongst the quintuple machines alone, i.e. the 
  quadruple machines will not contribute more to the busy beaver problem than the quintuple ones will.

- searching quintuple machines subsumes seaching for quadruple ones.

*/

#define N 2 // states
#define M 4 // symbols

#define HALT_STATE 'z'
#define START_STATE 'a'
#define BLANK_SYMBOL 0

int main(int argc, char *argv[]){
  return 0;
}
