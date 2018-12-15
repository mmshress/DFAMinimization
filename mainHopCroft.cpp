//
// Created by mohit on 14.12.18.
//
#include <iostream>
#include <fstream>
#include <chrono>
#include "Automaton.h"
#include <unordered_set>

void write_to_file(Automaton *in){

    std::ofstream stream("output.txt");

    int finalCount = 0;
    for(int i = 0; i < in->F.size(); i++){
        if(in->F[i]) finalCount++;
    }
    stream << in->Q << ' ' << in->Heads.size() << ' ' << in->q0 << ' ' << finalCount << '\n';
    for (int t = 0; t < in->Heads.size(); ++t)
    {
        stream << in->Tails[t] << ' ' << in->Labels[t] << ' ' << in->Heads[t] << '\n';

    }
    for (int b = 0; b < in->Q; ++b)
    {
        if (in->F[b])
        {
            stream << b << '\n';
        }
    }
    stream.close();
}

Automaton* Hopcroft(const Automaton *in){
    //initialize partition, split based on final/non-final behavior
    Partition P = Partition(in->Q);
    for(int i = 0; i<in->Q; i++){
        if(in->F[i]){
            P.mark(i);
        }
    }
    P.split(0);

    //Waiting set with partition indices that might need further splitting
    //Hopcroft's only adds half of these partitions
    //initialize waiting set with final state partition
    std::unordered_set<int> waiting_set;
    waiting_set.insert(1);

    //build a map to access the inverse transition function in O(1)
    //quotient[i][j] returns the set of states that map to i under symbol j
    std::vector<std::vector<std::vector<int> > > quotient (in->Q, std::vector<std::vector<int> > (in->alpha_size, std::vector<int> (0)));
    for(int i = 0; i<in->Heads.size(); i++){
        quotient[in->Tails[i]][in->Labels[i]].push_back(in->Heads[i]);
    }
    while(!waiting_set.empty()){
        int index = *(waiting_set.cbegin()); // current partition to work on
        waiting_set.erase(index);

        std::vector<int> splitters;
        for(int i = 0; i<in->alpha_size; i++){
            //looks at inverse of the partition current_index under symbol i
            for(int elem = P.get_first(index); elem!=-1; elem = P.get_next(elem)){
                for(int node : quotient[elem][i]){
                    P.mark(node);
                    splitters.push_back(P.set(node));
                }
            }
            for(int s : splitters){
                int sizeFirst = P.size(s);
                int sizeSecond = P.split(s); // the size of the elements marked
                if(sizeSecond > 0){ //if there was a split
                    if(waiting_set.find(s) == waiting_set.end())
                        if(sizeSecond <= sizeFirst/2){
                            //we add the smaller set
                            waiting_set.insert(P.get_sets()-1);
                        }else{
                            waiting_set.insert(s);
                        }
                    else{
                        waiting_set.insert(P.get_sets()-1);
                    }
                }
            }
        }
    }
    return Automaton::complete(in, P);
}

Automaton* read(){
    /* The format of the file read should be
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

    */
    std::string line;
    int Q, q0, final_no, alphabet_size, temp_state, num_transitions;


    std::cin >> Q >> num_transitions >> q0 >> final_no;

    std::vector<int> h;
    std::vector<int> l;
    std::vector<int> t;

    for (int i = 0; i < num_transitions; i++)
    {
        int from, label, to;
        std::cin >> to >> label >> from;
        //state i goes to state temp_state under alphabet j
        h.push_back(from);
        l.push_back(label);
        t.push_back(to);
    }

    std::vector<bool> final_states = std::vector<bool>(Q, false);
    for (int count = 0; count < final_no; count++)
    {
        std::cin >> temp_state;
        final_states[temp_state] = true;
    }
    std::cin >> alphabet_size;

    return new Automaton(Q, q0, final_states, alphabet_size, h, l, t);
}

int main(){
    Automaton* aut = read();
    auto begin = std::chrono::high_resolution_clock::now();
    Automaton* minimized = Hopcroft(aut);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "DFA minimized in " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " ms." << std::endl;

    write_to_file(minimized);
    std::cout << "DFA written to output.txt" << '\n';

    delete aut;

}


