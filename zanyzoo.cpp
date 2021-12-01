#include <cstdio>
#include <vector>
#include <queue>

#define DEBUG

#define N 3 // states
#define M 2 // symbols

#define NUMBER_OF_EXECUTION_STEPS 200

#define UNDEFINED_OUTPUT -1
#define UNDEFINED_STATE -2
#define UNDEFINED_DIRECTION 0

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
#define UNDEFINED_SYMBOL 'U'

#define D(DIRECTION) (DIRECTION == RIGHT ? RIGHT_V : LEFT_V)
#define S(STATE) (STATE == HALT_STATE ? HALT_STATE_V : STATE-START_STATE) // converts state to a number

#define rS(x) (x == HALT_STATE_V ? HALT_STATE : x+START_STATE) // convert back
#define rD(x) (x == RIGHT_V ? RIGHT : LEFT)

class Machine;
std::priority_queue<Machine> machines_to_consider;
std::vector<Machine> generated_machines;

class Tape {
  private:
    // nodes instead of vectors because I like pointers
    struct Node {
      int index;
      int data;
      struct Node *previous;
      struct Node *next;
    };
    
    struct Node *current_node;
    int min_index;
    int max_index;
  
  public:
    Tape() : current_node(new Node{0, 0, nullptr, nullptr}), min_index(current_node->index), max_index(current_node->index) {};
    
    bool is_blank() {
      bool blank = true;
      struct Node *node = current_node;
      // go to the leftmost node
      while (node->previous != nullptr) node = node->previous;
      for (int i = min_index; i <= max_index; ++i) {
        if (node->data != 0) {
          blank = false;
          break;
        }
        node = node->next;
      }
      return blank;
    }

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
        Node *new_node = new Node{0, 0, nullptr, nullptr};
        if (index > max_index) {
          max_index += 1;
          new_node->index = max_index;
          new_node->previous = current_node;
          current_node->next = new_node;
        }
        else if (index < min_index) {
          min_index -= 1;
          new_node->index = min_index;
          new_node->next = current_node;
          current_node->previous = new_node;
        }
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
    int number_of_states;
    int number_of_symbols;

  public:
    int current_state;
    int current_position;
    int activity;
    Transition transition_function[N][M];
    Tape tape;
    
    Machine() {
      current_state = S(START_STATE);
      current_position = 0;
      number_of_states = 0;
      number_of_symbols = 0;
      activity = 0;
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

    void execute() {
      Transition &transition = transition_function[current_state][tape[current_position]];
      
      tape[current_position] = transition.output;
      current_position += transition.direction;
      current_state = transition.new_state;
      
      activity += 1; 
    };
  
    bool is_0_dextrous() {
      int count = 0;
      for (int i = 0; i < N; ++i) {
        if (transition_function[i][0].new_state != UNDEFINED_STATE)
          count += (transition_function[i][0].direction == D(RIGHT));
      }
      return count == N;
    }

    bool n_of_execution_steps_exceeded() {
      return activity > NUMBER_OF_EXECUTION_STEPS;
    }

    bool failed_blank_tape_condition() {
      return (activity > 0 && tape.is_blank());
    }

    bool undefined_combination_is_found() {
      return transition_function[current_state][tape[current_position]].new_state == UNDEFINED_STATE;
    }

    bool is_n_m_full() {
      return (number_of_states == N && number_of_symbols == M);
    }

    bool halted() {
      return current_state == S(HALT_STATE);
    }

    bool operator <(const Machine &rhs) const {
      return activity < rhs.activity;
    }

    int card() {
      return number_of_states*number_of_symbols;
    }

    void print_transition_function() const {
      for (int i = S(START_STATE); i < N; ++i) {
        for (int j = 0; j < M; ++j) {
          Transition t = transition_function[i][j];
          if (t.new_state == UNDEFINED_STATE) printf("(%c,%d): %c%c%c ", rS(i), j, UNDEFINED_SYMBOL, UNDEFINED_SYMBOL, UNDEFINED_SYMBOL);
          else {
            printf("(%c,%d): %d%c%c ", rS(i), j, t.output, rD(t.direction), rS(t.new_state));
          }
        }
        printf("\n");
      }
      printf("\n");
    }

};

void generate() {
  Machine machine;
  // for state and symbol choice
  std::vector<int> states, symbols;
  
  // step 1
  machine.add_transition(S(START_STATE), BLANK_SYMBOL, 1, D(RIGHT), S(SECOND_STATE));
  
  // step 2
  symbols = machine.symbol_choice();
  
  for (int symbol : symbols) {
     for (char state : {START_STATE, SECOND_STATE}) {
       machine.add_transition(S(SECOND_STATE), BLANK_SYMBOL, symbol, D(LEFT), S(state));
       machines_to_consider.push(machine);
     }
  }
  
  if (N >= 3) {
    for (int symbol : symbols) {
      for (char direction : {LEFT, RIGHT}) {
        machine.add_transition(S(SECOND_STATE), BLANK_SYMBOL, symbol, D(direction), S(THIRD_STATE));
        machines_to_consider.push(machine);
      }
    } 
  } 
   
  while (!machines_to_consider.empty()) {
    machine = machines_to_consider.top();
    machines_to_consider.pop();
    
    // step 3
    // break == halting machine/stopping execution
    while (true) {
      Transition &t = machine.transition_function[machine.current_state][machine.tape[machine.current_position]];
      
      printf("Total: %lu - Candidates: %lu - Left: %lu --- Steps:%d State:%c Position:%d Input:%d Output:%d Direction:%c NewState:%c\n",
             machines_to_consider.size()+generated_machines.size(), generated_machines.size(),
             machines_to_consider.size(), machine.activity, rS(machine.current_state), machine.current_position,
             machine.tape[machine.current_position], t.output, rD(t.direction), rS(t.new_state));
      
      // Clearly we need to store the machines whose computation length which exceeds the given bound, but strictly speaking,
      // we could insist that machines that are known to be irrelevant are not stored at all.
      // Whilst this would reduce the number of machines considered, we have chosen to retain such machines in order to
      // simplify the definition of the generation process.
      if (machine.n_of_execution_steps_exceeded() || machine.failed_blank_tape_condition()) {
        generated_machines.push_back(machine);
        break;
      }
      
      if (machine.undefined_combination_is_found()) {
        // step 4
        if (machine.is_n_m_full()) {
          machine.add_transition(machine.current_state, machine.tape[machine.current_position], 1, D(RIGHT), S(HALT_STATE));
          if (!machine.is_0_dextrous()) {
            generated_machines.push_back(machine);
          }
        }
        
        states = machine.state_choice();
        symbols = machine.symbol_choice();
        for (int state : states) {
          for (int symbol : symbols) {
            for (char dir : {LEFT, RIGHT}) {
              // we do not want to alter original machine
              Machine m_copy = machine;
              m_copy.add_transition(m_copy.current_state, m_copy.tape[m_copy.current_position], symbol, D(dir), state);
              if (!m_copy.is_0_dextrous())
                machines_to_consider.push(m_copy);
            }
          }
        }
        break;
      }
      
      // step 5
      if (machine.card() == N*M-1) {
        // find appropriate S and I
        for (int i = 0; i < N; ++i) {
          for (int j = 0; j < M; ++j) {
            if (machine.transition_function[i][j].new_state == UNDEFINED_STATE) {
              machine.add_transition(i, j, 1, D(RIGHT), S(HALT_STATE));
              generated_machines.push_back(machine);
            }
          }
        }
        break;
      }
      
      machine.execute();
      if (machine.halted()) {
        generated_machines.push_back(machine);
        break;
      }
    }
  }
}

int main(int argc, char *argv[]) {
  generate();
  return 0;
}
