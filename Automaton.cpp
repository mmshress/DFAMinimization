#include "Automaton.h"
#include <vector>
#include <iostream>


Automaton::Automaton(int Q, int initial_state, std::vector<bool> final_states, int alphabet_size, std::vector<int> Heads, std::vector<int> Labels, std::vector<int> Tails){
        this->Q = Q;
        this->q0 = initial_state;
        this->F = final_states;
        this->alpha_size = alphabet_size;
        this->Heads = Heads;
        this->Labels = Labels;
        this->Tails = Tails;
}


void Automaton::print(){
    std::cout << "There are " << Q << " states." << std::endl;
    std::cout << "The initial state is state no. " << q0 << std::endl;
   
    if(F.size() == 1){
        std::cout << "The final state is state " << F[0] << std::endl; 
    }
    else{
        std::cout << "The final states are:";
        for(int index = 0; index < F.size(); index++){
            if(F[index]) std::cout << index << " ";
        }
        std::cout << std::endl;
    }

    for(int index = 0; index < Heads.size(); index++){
        int head = Heads[index];
        int symbol = Labels[index];
        int tail = Tails[index];
        std::cout << "On reading symbol " << symbol << " state " << tail << " goes to state " << head << std::endl;
    }
}

Automaton* Automaton::complete(const Automaton *in, Partition &P){
    int num_states = P.get_sets();
    int alph_size = in->alpha_size;
    int start_index = P.set(in->q0);
    std::vector<bool>finals (num_states, 0);
    for(int i = 0; i<in->Q; i++){
        if(in->F[i]){
            finals[P.set(i)] = 1;
        }
    }
    std::vector<int> Tails;
    std::vector<int> Labels;
    std::vector<int> Heads;
    for(int i = 0; i<in->Heads.size(); i++){
        int t = P.set(in->Tails[i]);
        int l = in->Labels[i];
        int h = P.set(in->Heads[i]);
        Tails.push_back(t);
        Labels.push_back(l);
        Heads.push_back(h);
    }
    return new Automaton(num_states,  start_index,  finals, alph_size, Heads, Labels, Tails);
}