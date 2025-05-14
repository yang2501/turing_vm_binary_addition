# turing_vm_binary_addition
A simple turing virtual machine able to do binary addition.

This turing VM sums two numbers a and b and leaves the result in this format: _ _ c _ _
The tape alphabet: {0, 1, +, _}
The tape format: _ _ a + b _ _ where the underscore denotes the tape boundaries. The number of underscores in the final output
is subject to change depending on the magnitude of the numbers being added.

To compile and run:
g++ -std=c++17 turing_vm.cpp -o turing_vm
./turing_vm < test.txt
