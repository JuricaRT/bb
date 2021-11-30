#include <cstdio>
#include <vector>

#define N 4 // states
#define M 4 // symbols

#define UNDEFINED_OUTPUT -1
#define UNDEFINED_STATE -2
#define UNDEFINED_DIRECTION -3

#define RIGHT_V 1
#define LEFT_V -1
#define LEFT 'l'
#define RIGHT 'r'

#define HALT_STATE 'z'
#define HALT_STATE_V -1
#define START_STATE 'a'
#define SECOND_STATE 'b'
#define THIRD_STATE 'c'
#define BLANK_SYMBOL 0

#define D(DIRECTION) (DIRECTION == RIGHT ? RIGHT_V : LEFT_V)
#define S(STATE) (STATE-START_STATE) // converts state to a number

#define rS(x) (x == HALT_STATE_V ? HALT_STATE : x+START_STATE) // convert back

class Tape {
  private:
    // nodes instead of vectors because I like pointers
    struct Node {
      int data;
      int index;
      struct Node *previous;
      struct Node *next;
    };
    
    struct Node *current_node;
    int min_index;
    int max_index;
  
  public:
    Tape() : current_node(new Node{0, 0, nullptr, nullptr}), min_index(current_node->index), max_index(current_node->index) {};
    
    // this works because Turing machine moves ONLY ONE step to the left/right
    int& operator[](int index) {
      if (index >= min_index && index <= max_index) {
        if (index > current_node->index) {
          current_node = current_node->next;
       }
        else if (index < current_node->index) {
          current_node = current_node->previous;
        }
      } 
      else {
        int new_index;
        struct Node *new_previous, *new_next;
        if (index > max_index) {
          max_index += 1;
          new_index = max_index;
          new_previous = current_node;
          new_next = nullptr;
        }
        else if (index < min_index) {
          min_index -= 1;
          new_index = min_index;
          new_previous = nullptr;
          new_next = current_node;
        }
        Node *new_node = new Node{0, new_index, new_previous, new_next};
        current_node = new_node; 
      }
      return current_node->data;
    };
};

class Transition {
  public:
    int output;
    int direction;
    int new_state;
    Transition() {
      output = UNDEFINED_OUTPUT;
      direction = UNDEFINED_DIRECTION;
      new_state = UNDEFINED_STATE;
    };
    Transition(int out, int dir, int new_s) : output(out), direction(dir), new_state(new_s) {};
};

class Machine {
  private:
    int current_state;
    int current_position;
    int number_of_states;
    int number_of_symbols;
    Transition transition_function[N][M];
    Tape tape;

  public:
    Machine() {
      current_state = UNDEFINED_STATE;
      current_position = 0;
      number_of_states = 0; // 1 but we have to start from 0 because transition_function indexing (S('a') = 0)
      number_of_symbols = 0; // same thing
    }

    void add_transition(int state, int input, int output, int direction, int new_state) {
      // we can insist on alphabetical order of states
      // which means we can track the number of states by setting the variable to S(STATE)
      // everytime we add transition FOR some state and we might increase the number of states
      // everytime we add transition TO some new state we might increase the number of states
      // hence, max expression is needed, same goes for symbols
      number_of_states = std::max(std::max(number_of_states, state+1), new_state+1);
      number_of_symbols = std::max(std::max(number_of_symbols, input+1), output+1);

      transition_function[state][input].output = output; 
      transition_function[state][input].direction = direction;
      transition_function[state][input].new_state = new_state;
    };

      
    // let k be number_of_states+1
    // if k < N then output {a1,...,ak,ak+1}
    // else output {a1,...,an}
    // example: state_choice(4,{a1,a2}) = {a1,a2,a3} which will ensure that the next state chosen after a1 and a2 is a3
    std::vector<int> state_choice() {
      std::vector<int> states;
      
      // number_of_states+1 if we want ak+1 element
      int how_many_to_output = std::min(number_of_states+1, N);
      for (int state = S(START_STATE); state < how_many_to_output; ++state) states.push_back(state);
      return states;
    };

    std::vector<int> symbol_choice() {
      std::vector<int> symbols;

      int how_many_to_output = std::min(number_of_symbols+1, M);
      for (int symbol = BLANK_SYMBOL; symbol < how_many_to_output; ++symbol) symbols.push_back(symbol);
      return symbols;
    };
};


void generate() {
  Machine machine;
  
  // step 1
  machine.add_transition(S(START_STATE), BLANK_SYMBOL, 1, D(RIGHT), S(SECOND_STATE));
  
  // step 2
  for (int symbol : machine.symbol_choice()) {
     for (char state : {START_STATE, SECOND_STATE}) {
       machine.add_transition(S(SECOND_STATE), BLANK_SYMBOL, symbol, D(LEFT), state);
     }
  }

  if (N >= 3) {
    for (int symbol : machine.symbol_choice()) {
      for (char direction : {LEFT, RIGHT}) {
        machine.add_transition(S(SECOND_STATE), BLANK_SYMBOL, symbol, direction, S(THIRD_STATE));
      }
    } 
  }
}

int main(int argc, char *argv[]) {
  generate();
  return 0;
}
