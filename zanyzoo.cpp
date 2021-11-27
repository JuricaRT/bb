// Generating Candidate Busy Beaver Machines (Or How to Build the Zany Zoo) - https://arxiv.org/abs/1610.03184

/*

Preliminaries and Definitions
-----------------------------

Our machines are dextrous, or right-handed. The choice of direction, left or right, is entirely arbitrary; as long as this choice
is applied consistently, it will have no bearing on the results. For every right-handed machine there is left-handed machine which
has exactly the same execution behaviour except that the direction of each transition is reversed.

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



Normal Form
-----------

- in order to solve busy beaver problem, we are only interested in evaluating machines on the blank input, which means we are able to
  reduce the number of machines that require non-trivial analysis.


Lemma 15. Let M be a Turing machine containing a tuple of the form (a,0,_,_,a). Then activity(M) = infinity.
Lemma 16. Let M be a Turing machine containing a tuple of the form (a,0,_,_,z). Then activity(M) = 1. 

- Lemma 15. and Lemma 16. show that we only need to consider machines whose first transition is of the form (a,0,_,_,b)


- we can assume that the second state used in the machine is b (rather than say c or d), as justified by the following lemma

Lemma 17. Let M be a k-halting n-state m-symbol Turing machine containing a tuple of the form (a,0,_,_,S) where S is not a, b or z.
          Then there is another k-halting n-state m-symbol Turing machine M' containing the tuple (a,0,_,_,b)
          such that M and M' are productivity and activity equivalent.
Proof. Let M' be the machine found by swapping all the occurences of S and b in M. The result then trivially follows.

- Lemma 17. shows that we can insist that the second state encountered in the machine (after the start state a) is b. Similarly we
  can insist that the third state (if any) encountered is c, and so on as justified by the following lemmas.


Lemma 18./19. Let M be a Turing machine, and let (S1, S2)/(O1, O2) be two distinct (states)/(symbols) in M such that
              (S1, S2 != a or z)/(O1, O2 != 0). Let M' be the machine found by swapping the (states)/(symbols) (S1, S2)/(O1, O2)
              in every transition in M. Then M' is productivity and activity equivalent to M and M' contains the same number of
              states, symbols and halting transitions as M.

- Lemma 18. and Lemma 19. show that we can insist on a specific order in which the states and symbols appear in the execution of 
  the machine. In particular, given that the first state must be a, we can insist that the second state encountered be b, the
  third c and so forth. Similarly, we can insist that the first non-blank symbol encountered be 1, the second 2, and so on.


Lemma 20. Let M be a k-halting n-state m-symbol Turing machine of finite activity and productivity >= 1 containing a tuple of the form
          (a,0,O,_,NS) where NS != z. Then there is another k-halting n-state m-symbol Turing machine M' of finite activity
          containing the tuple (a,0,O,_,NS) where O != 0 such that M' is productivity equivalent to M.
Proof. As productivity(M) >= 1, there must be a transition of the form (S,0,O,_,_) where S != a and O != 0 in M, and that this is 
       the first transition in the execution of M which writes a non-blank symbol on the tape. Let M' be the machine found by
       swapping all occurrences of a and S in M. Then the result trivially follows.

- note that a similar property will follow for machine with infinite activity, however this case is irrelevant to the busy beaver problem,
  also, the activity of M' will be less than M, as this change effectively ignores the initial execution steps which do not change the
  blank tape.


Lemma 21. Let M be a k-halting n-state m-symbol Turing machine of finite activity and productivity >= 1 which violates blank tape
          condition. Then there is another k-halting n-state m-symbol Turing machine M' of finite activity which is productivity
          equivalent to M and which satisfies the blank tape condition.
Proof. As the execution of M on the blank tape terminates, there must be a final configuration in this execution trace in which the tape
       is blank. Let S be the state in which this occurs. As the activity of M is finite and the productivity is at least 1, this state
       cannot be either a or z. Let M' be the machine found by swapping all occurrences of a and S in M. Then result trivially follows.


Lemma 22. Let M be a Turing machine containing tuples of the form (a,0,_,r,b) and (b,0,_,r,S) where S = a or b.
          Then M has infinite activity.

- this means that we can restrict our attention to transitions of the form (b,0,_,l,_) or (b,0,_r,c). Furthermore, it is clear that
  a machine with transitions (a,0,_,r,b) and (b,0,_,_,z) has activity 2, and is hence irrelevant.

- this observation, together with Lemmas 15, 17, 20, 21, 18, 19 and 22 means that we only need to consider machines with particular
  initial transition and some constraints to the second one.


Definition 23. An n-state Turing machine is 0-DEXTROUS(right handed) if there are n transitions of the form (_,0,_,_,_) and all such
               n transitions are of the form (_,0,_r,_).

- the reason that we identify the 0-dextrous class of machines is because they are all irrelevant.

Lemma 24. Let M be a 0-dextrous Turing machine. Then M is irrelevant to the busy beaver problem.
Proof. Let the number of states in M be n. If M does not terminate on blank input, then its activity is infinite and 
       M is hence irrelevant. Otherwise, M terminates on the blank input, which, as M is 0-dextrous, is only possible if there is a
       transition in M of the form (_,0,_r,z). This means that M halts in at most n steps and so activity(M) <= n, which means
       that M is irrelevant.

- as this property requires that at least all of the transitions for input 0 are known, this can generally only be implemented as a 
  constraint on the final machine, i.e. that any 0-dextrous machine that is generated is ignored.


Definition 25. A Turing machine is NORMAL iff it has all of the following properties
1. M contains the tuple (a,0,1,r,b)  
2. M contains either the tuple of the form (b,0,O,l,S) where S is a, b or c and O is 0, 1 or 2,
   or tuple of the form (b,0,O,r,c) where O is 0, 1 or 2
3. M is not 0-dextrous
4. When executing M on the blank input
   - states are encountered in alphabetical order
   - symbols are encountered in numberical order
   - the blank tape condition is satisfied

Proposition 26. Let M be a n-state Turing machine. If M is releveant to the busy beaver problem, then M is productivity
                equivalent to a normal machine.
Proof. Let M be a Turing machine which is relevant to the busy beaver problem.
       
       Then M has finite activity, activity(M) > n, productivity(M) > 0 and satisfies the blank tape condition.
       
       Consider the four conditions of Definition 25.
       By the combination of Lemmas 15, 16, 17, 18, 19 and 20, M is productivity equivalent to a machine
       which satisfies the first condition.
       
       By the combination of Lemmas 18, 19 and 22, M is productivity equivalent to a machine which satisfies second condition.
       
       As M has finite activity, by Lemma 24 it cannot be 0-dextrous.

       By Lemmas 18 and 19, M is productivity equivalent to a machine which satisfies the first two parts of the fourth condition.
       The third part of the fourth condition follows immediately from the definition of relevance.

- this means that in order to solve the busy beaver problem, it is sufficient enough to only consider normal machines, i.e. this results
  allow us to define an appropriate normal form for machines, which means that there are certain machines that need not to be generated
  at all, thus reducing the work we need to do, and other which once generated can be immediately dismissed as irrelevant, thus reducing
  the number of machines to be stored.

*/

#define N 2 // states
#define M 4 // symbols

#define HALT_STATE 'z'
#define START_STATE 'a'
#define BLANK_SYMBOL 0

int main(int argc, char *argv[]){
  return 0;
}
