#ifndef AUTOMATON_H
#define AUTOMATON_H
#include <boost/unordered_map.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>
#include "Partition.h"

class Automaton{
    public:
        int Q; //number of states of the DFA

        int q0; //initial state

        std::vector<bool> F; //number of final states

        int alpha_size; //size of the alphabet

        //Each transition is a tuple (q, a, q') such that q goes to q' under symbol a
        std::vector<int> Heads;
        std::vector<int> Labels;
        std::vector<int> Tails;

        Automaton(int Q, int initial_state, std::vector<bool> final_states, int alphabet_size, std::vector<int> Heads, std::vector<int> Labels, std::vector<int> Tails);

        static Automaton* complete(const Automaton* old, Partition &P);

        void print(); 
};
#endif