#include <vector>
#include <iostream>
#include <tuple>
#include <string>
#include <unordered_map>
#include <string>

using namespace std;


// The alphabet: {0, 1, +, _} where "_" are the tape boundaries
// tape format: _ _ _ _ _ _ _ _ a + b _ _ _ _ _ _ _ _
vector<char> tape;

// The finite states
enum state {
    MOVE_TO_END_OF_A,
    INCREMENT_A,
    MOVE_TO_END_OF_B,
    DECREMENT_B,
    CLEAN_UP,
    STOP
};

// Convert state enum to string
string stateToString(state s) {
    switch (s) {
        case MOVE_TO_END_OF_A: return "MOVE_TO_END_OF_A";
        case INCREMENT_A: return "INCREMENT_A";
        case MOVE_TO_END_OF_B: return "MOVE_TO_END_OF_B";
        case DECREMENT_B: return "DECREMENT_B";
        case CLEAN_UP: return "CLEAN_UP";
        case STOP: return "STOP";
        default: return "UNKNOWN";
    }
}

enum direction {
    RIGHT = 1,
    LEFT = -1,
};

typedef string currstate_input;
typedef tuple<state, char, direction> newstate_output_direction;



int main() {
    string input;
    getline(cin, input);

    for (int i = 0; i < input.length(); i++) {
        tape.push_back(input[i]);
    }

    // Transition Rules (Tuples):
    // Format: (current_state, input_symbol) -> (new_state, output_symbol, direction).
    unordered_map<currstate_input, newstate_output_direction> dict;
    
    // move to end of a
    dict[stateToString(MOVE_TO_END_OF_A) + "0"] = {MOVE_TO_END_OF_A, '0', LEFT};  // (0, 0) -> (0, 0, L)
    dict[stateToString(MOVE_TO_END_OF_A) + "1"] = {MOVE_TO_END_OF_A, '1', LEFT};  // (0, 1) -> (0, 1, L)
    dict[stateToString(MOVE_TO_END_OF_A) + "+"] = {INCREMENT_A, '+', LEFT};       // (0, +) -> (1, +, L)


    // increment a
    dict[stateToString(INCREMENT_A) + "1"] = {INCREMENT_A, '0', LEFT};           // (1, 1) -> (1, 0, L) flip all 1"s to 0"s
    dict[stateToString(INCREMENT_A) + "0"] = {MOVE_TO_END_OF_B, '1', RIGHT};     // (1, 0) -> (2, 1, R) flip the first 0 to 1 and change state to "move to end of a"
    dict[stateToString(INCREMENT_A) + "_"] = {MOVE_TO_END_OF_B, '1', RIGHT};     // (1, _) -> (2, 1, R)

    // move to end of b
    dict[stateToString(MOVE_TO_END_OF_B) + "0"] = {MOVE_TO_END_OF_B, '0', RIGHT}; // (2, 0) -> (2, 0, R)
    dict[stateToString(MOVE_TO_END_OF_B) + "1"] = {MOVE_TO_END_OF_B, '1', RIGHT}; // (2, 1) -> (2, 1, R)
    dict[stateToString(MOVE_TO_END_OF_B) + "+"] = {MOVE_TO_END_OF_B, '+', RIGHT}; // (2, +) -> (2, +, R)
    dict[stateToString(MOVE_TO_END_OF_B) + "_"] = {DECREMENT_B, '_', LEFT};       // (2, _) -> (3, _, L)

    // decrement b
    dict[stateToString(DECREMENT_B) + "0"] = {DECREMENT_B, '1', LEFT};            // (3, 0) -> (3, 1, L) flip all 0"s to 1"s
    dict[stateToString(DECREMENT_B) + "1"] = {MOVE_TO_END_OF_A, '0', LEFT};       // (3, 1) -> (0, 0, L) flip the first 1 to 0 and change state to "move to end of a"
    dict[stateToString(DECREMENT_B) + "+"] = {CLEAN_UP, '_', RIGHT}; // (3, +) -> (4, _ R) termination condition: if we reach the + sign without meeting a 1 bit

    // clean up
    // (4, 0) -> (4, _, R)
    dict[stateToString(CLEAN_UP) + "0"] = {CLEAN_UP, '_', RIGHT};
    dict[stateToString(CLEAN_UP) + "1"] = {CLEAN_UP, '_', RIGHT};
    dict[stateToString(CLEAN_UP) + "_"] = {STOP, '_', RIGHT};

    int index_of_plus = std::find(tape.begin(), tape.end(), '+') -  tape.begin();
    int head = index_of_plus; // at the + sign 
    enum state curr_state = MOVE_TO_END_OF_B; 
    int count = 0;


    while (head >= 0 && head < tape.size() && curr_state != STOP && dict.find(stateToString(curr_state) + tape[head]) != dict.end()) {
        auto [new_state, output, dir] = dict[stateToString(curr_state) + tape[head]];
        curr_state = new_state;
        tape[head] = output;
        head += dir;
        count++;
        for (int i = 0; i < tape.size(); i++) {
            cout << tape[i];
        }
        cout << "\n";
    }

    // for (int i = 0; i < input.length(); i++) {
    //     cout << tape[i] << " ";
    // }

    cout << "Count: " << count << "\n"; 

    
}


