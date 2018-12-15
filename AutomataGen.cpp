//
// Created by mohit on 14.12.18.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
using std::string;
#include <vector>
#include <bits/stdc++.h>

/* for de bruijn sequences */
typedef std::uint64_t uint_type;

// Simple bitvector with methods to set and test bits
struct bitvector {
    unsigned char *bits;
};

static void init(bitvector &bv, size_t size) {
    bv.bits = new unsigned char[size]();
}

static void destroy(bitvector &bv) {
    delete[] bv.bits;
}

static void set_bit(bitvector &bv, size_t pos) {
    bv.bits[pos / 8] |= 1 << (pos % 8);
}

static bool test_bit(bitvector &bv, size_t pos) {
    return bv.bits[pos / 8] & (1 << (pos % 8));
}

// Prints the de Bruijn sequence B(2, order) using the prefer-one algorithm
// Precondition: 1 <= order <= 8 * sizeof(uint_type)

void debruijn(int order, std::stringstream &result) {
    // MASK serves as both a bitmask and a size
    uint_type MASK = (order == 8 * sizeof(uint_type)) ? -1 : ((uint_type)1 << order) - 1;
    uint_type window = 0;

    bitvector sequences;
    init(sequences, MASK / 8 + 1);

    while (true) {
        window = (window << 1) & MASK;

        if (!test_bit(sequences, window + 1)) {
            set_bit(sequences, ++window);
            result << 1;
        }
        else if (!test_bit(sequences, window)) {
            set_bit(sequences, window);
            result << 0;
        }
        else break;
    }

    destroy(sequences);
}

void cyclic_de_bruijn(int n){
    std::stringstream str;
    debruijn(n, str);
    string de_bruijn_word = str.str(); //amazing variable naming
    std::ofstream stream("automata.txt");
    int alphabet_size = 1;
    int initial_state = 0;
    int num_of_states = de_bruijn_word.length();
    int num_of_transitions = num_of_states;
    int num_of_finals = 0;
    for(char c : de_bruijn_word){
        if(c == '1') num_of_finals++;
    }
    stream << num_of_states << " " << num_of_transitions << ' ' << initial_state << " " << num_of_finals << '\n';

    for(int i = 0; i < num_of_states; i++){//cyclic transitions
        stream << (i + 1) % num_of_states << " " << 0 << " " << i << '\n';
    }

    for(int i = 0; i < de_bruijn_word.length(); i++){
        if(de_bruijn_word[i] == '1') stream << i << '\n';
    }

    stream << alphabet_size << '\n';
    stream.close();
}

string fibWord(int n)
{
    string Sn_1 = "0";
    string Sn = "01";
    string tmp;
    for (int i=2; i<=n; i++)
    {
        tmp = Sn;
        Sn += Sn_1;
        Sn_1 = tmp;
    }

    return Sn;
}

void cyclic_fibonacci(int n){
    string word = fibWord(n);
    std::ofstream stream("automata.txt");
    int alphabet_size = 1;
    int initial_state = 0;
    int num_of_states = word.length();
    int num_of_transitions = num_of_states;
    int num_of_finals = 0;
    for(char c : word){
        if(c == '1') num_of_finals++;
    }
    stream << num_of_states << " " << num_of_transitions << ' ' << initial_state << " " << num_of_finals << '\n';

    for(int i = 0; i < num_of_states; i++){//cyclic transitions
        stream << (i + 1) % num_of_states << " " << 0 << " " << i << '\n';
    }

    for(int i = 0; i < word.length(); i++){
        if(word[i] == '1') stream << i << '\n';
    }

    stream << alphabet_size << '\n';
    stream.close();
}

void random(int n, int alph_size){
    std::vector<int> Tails;
    std::vector<int> Labels;
    std::vector<int> Heads;
    for(int i = 0; i<n; i++){
        Tails.push_back(i);
        Labels.push_back(0);
        Heads.push_back((i+1) % n);
    }
    for(int x = 1; x<alph_size; x++){
        for(int i = 0; i<n; i++){
            Tails.push_back(i);
            Labels.push_back(x);
            Heads.push_back(rand() % n);
        }
    }
    std::vector<bool> finals(n, 0);
    for(int i = 0; i<n; i++)
        finals[i] = rand() % 2;
    std::ofstream stream("automata.txt");


    int finalCount = 0;
    for(int i = 0; i < finals.size(); i++){
        if(finals[i]) finalCount++;
    }
    stream << n << ' ' << Heads.size() << ' ' << 0 << ' ' << finalCount << '\n';
    for (int t = 0; t < Heads.size(); ++t)
    {
        stream << Tails[t] << ' ' << Labels[t] << ' ' << Heads[t] << '\n';

    }
    for (int b = 0; b < n; ++b)
    {
        if (finals[b])
        {
            stream << b << '\n';
        }
    }
    stream << alph_size << '\n';
    stream.close();
}


void length_over_n(int n){ // automaton for words with length at least n
    std::ofstream stream("automata.txt");
    int alphabet_size = 1;
    int initial_state = 0;
    int num_of_states = n + 1;
    int num_of_finals = 1;
    int num_of_transitions = n + 1;
    stream << num_of_states << " " << num_of_transitions << " " << initial_state <<  " " << num_of_finals << '\n';
    for(int i = 0; i < n; i++){
        int to = i + 1;
        int from = i;
        int label = 0;
        stream << to << ' ' << label << ' ' << from << '\n';
    }
    stream << n << ' ' << 0 << ' ' << n << '\n';
    stream << n << '\n'; //final state
    stream << alphabet_size << '\n';
    stream.close();
}

int main(int argc, char** argv){
    // run by ./autogen x n, x is type of automaton, n is size
    //writes to automata.txt

    if(argc < 3 || argc > 4){
        return 0;
    }
    else{
        switch(std::stoi(argv[1])){
            case 0:
                length_over_n(std::stoi(argv[2]));
                break;
            case 1:
                srand(time(NULL));
                random(std::stoi(argv[2]), std::stoi(argv[3]));
                break;
            case 2:
                cyclic_fibonacci(std::stoi(argv[2]));
                break;
            case 3:
                cyclic_de_bruijn(std::stoi(argv[2]));
                break;
        }

    }
}