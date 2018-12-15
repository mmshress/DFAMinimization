To build the project, please run cmake . on the directory first, then make all in the build directory. The executables are generator, valmari and hopcroft. CMake 2.8+ and C++11 are required.

generator generates automata input files for the hopcroft/valmari executables. The output file is always "automata.txt". The usage is: ./generator type_of_automaton size_parameter size_of_alphabet

The resulting created file can be piped into the hopcroft/valmari executables as follows: 

cat "automata.txt" | ./valmari
cat "automata.txt" | ./hopcroft

Depending on the type of automaton selected, the size_of_alphabet argument may or may not be supported.
The values and the respective types of automata that can be generated are:

0 - words of length at least n. size_parameter controls n. As this is slow for Moore's, it is also slow for Hopcroft. Some timing tests I ran on varying sizes and averaging around 10 trials for each are shown below. All times are in microseconds and recorded using the <chrono> library: 
(n = 10): Hopcroft 200us, Valmari-Lehtinen 60us
(n = 100):  Hopcroft 1200us, Valmari-Lehtinen 250us
(n = 1000): Hopcroft 11000us, Valmari-Lehtinen 1900us

1 - random automata with n states, the size_parameter controls n, size_of_alphabet argument is supported.
Some tests again:
(n = 40, alphabet size = 12): Hopcroft 3300us, Valmari-Lehtinen 200us
(n = 40, alphabet size = 30): Hopcroft 7000us, Valmari-Lehtinen 1700us

2 - Cyclic automata with final states corresponding to 1s in a binary Fibonacci word of order n can cause Hopcroft to run slow (Berstel/Boasson/Carton/Fagnot 2010). size_parameter controls n, number of states grows like the Fibonacci sequence. Some tests:
(n = 9): Hopcroft 1300us, Valmari-Lehtinen 230us
(n = 20): Hopcroft 64000us Valmari-Lehtinen 16000us
(n = 25): Hopcroft 910000us (almost a second) Valmari-Lehtinen 200000us

3 - Cyclic automata similar to the above but with de Bruijn sequences of binary alphabet and order n instead of Fibonacci words also cause Hopcroft to run slow. (same paper as above). size_parameter controls n, number of states is 2^n. Some tests:
(n = 8) Hopcroft 3700us, Valmari-Lehtinen 880us
(n = 14) Hopcroft 43000us, Valmari-Lehtinen 14000us

Custom automata can also be fed into the valmari/hopcroft executables in a similar manner as above. The format for the file should be:
    num_of_states
    num_of_transitions
    initial_state
    num_of_final_states
    tail_0 label_0 head_0
    tail_1 label_1 head_1
    .
    .
    .
    final_state0 final_state1 final_state2 ...
    alphabet_size
    EOF

In general, Valmari-Lehtinen seems to run much faster than Hopcroft, providing around a 4-5x speedup at most in the implementations here. Something to note is that my implementation of Hopcroft does not remove unreachable states from the DFA, whereas Valmari's implementation does, and despite that, there is an obvious runtime advantage.
