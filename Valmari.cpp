#include <iostream>
#include <algorithm>
#include <chrono>
#include <fstream>

/* Refinable partition */
//shared variables among blocks and cords
int* marked; //the number of marked elements in s
int* touched; // touched (i.e. contain marked) sets
int touchedCount = 0; // temporary worksets
struct VPartition
{
    int setCount;
    int* elements;
    int* location;
    int* setOf;
    int* first;
    int* past;

    void init(int n)
    {
        setCount = bool(n);
        elements = new int[n];
        location = new int[n];
        setOf = new int[n];
        first = new int[n];
        past = new int[n];
        for (int i = 0; i < n; ++i)
        {
            elements[i] = location[i] = i;
            setOf[i] = 0;
        }
        if (setCount)
        {
            first[0] = 0;
            past[0] = n;
        }
    }

    void mark(int element)
    {
        int set = setOf[element];
        int i = location[element];
        int j = first[set] + marked[set];
        elements[i] = elements[j];
        location[elements[i]] = i;
        elements[j] = element;
        location[element] = j;
        if (!marked[set]++)
        {
            touched[touchedCount++] = set;
        }
    }

    void split()
    {
        while (touchedCount)
        {
            int set = touched[--touchedCount];
            int firstUnmarked = first[set] + marked[set];
            if (firstUnmarked == past[set])
            {
                marked[set] = 0;
                continue;
            }

            // Make the smaller half a new set
            // If same size, then make a new set out of unmarked
            if (marked[set] <= past[set] - firstUnmarked)
            {
                first[setCount] = first[set];
                past[setCount] = first[set] = firstUnmarked;
            }
            else
            {
                past[setCount] = past[set];
                first[setCount] = past[set] = firstUnmarked;
            }
            for (int i = first[setCount]; i < past[setCount]; ++i)
            {
                setOf[elements[i]] = setCount;
            }
            marked[setCount++] = 0;
            marked[set] = 0;
        }
    }
};

VPartition* blocks; // blocks (consist of states)
VPartition* cords; // cords (consist of transitions)
int num_states; // number of states
int num_transitions; // number of transitions
int num_finals; // number of final states
int alph_size; //alphabet size
int q0; // initial state
int* Tails; // tails  (i.e. to state)
int* Labels; // labels (i.e. symbol)
int* Heads; // heads (i.e. from state)
bool cmp(int i, int j)
{
    return Labels[i] < Labels[j];
}

/* Adjacent transitions */
int* adjacent;
int* F;
void make_adjacent(int states[])
{
    for (int state = 0; state <= num_states; ++state)
        F[state] = 0;

    for (int transition = 0; transition < num_transitions; ++transition)
        ++F[states[transition]];

    for (int state = 0; state < num_states; ++state)
        F[state + 1] += F[state];

    for (int transition = num_transitions; transition--; )
        adjacent[--F[states[transition]]] = transition;
}

/* Removal of irrelevant parts */
int num_reachable = 0; // number of reached states
inline void reach(int state)
{
    int i = blocks->location[state];
    if (i >= num_reachable)
    {
        blocks->elements[i] = blocks->elements[num_reachable];
        blocks->location[blocks->elements[i]] = i;
        blocks->elements[num_reachable] = state;
        blocks->location[state] = num_reachable++;
    }
}

void rem_unreachable(int tail[], int head[])
{
    make_adjacent(tail);
    // walk the DFA graph marking reachable states
    for (int i = 0; i < num_reachable; ++i)
    {
        for (int j = F[blocks->elements[i]]; j < F[blocks->elements[i] + 1]; ++j)
            reach(head[adjacent[j]]);
    }
    // remove unreachable states and transitions
    int j = 0;
    for (int t = 0; t < num_transitions; ++t)
    {
        if (blocks->location[tail[t]] < num_reachable)
        {
            head[j] = head[t];
            Labels[j] = Labels[t];
            tail[j] = tail[t];
            ++j;
        }
    }
    num_transitions = j;
    blocks->past[0] = num_reachable;
    num_reachable = 0;
}

/* Main program */
int main()
{

    /* Read sizes and reserve most memory */
    std::cin >> num_states >> num_transitions >> q0 >> num_finals;
    Tails = new int[num_transitions];
    Labels = new int[num_transitions];
    Heads = new int[num_transitions];
    blocks = new VPartition;
    blocks->init(num_states);
    adjacent = new int[num_transitions];
    F = new int[num_states + 1];
    /* Read transitions */
    for (int t = 0; t < num_transitions; ++t)
    {
        std::cin >> Heads[t] >> Labels[t] >> Tails[t];
    }
    auto begin = std::chrono::high_resolution_clock::now();
    /* Remove states that cannot be reached
    from the initial state, and from which
    final states cannot be reached */
    reach(q0);
    rem_unreachable(Tails, Heads);
    for (int i = 0; i < num_finals; ++i)
    {
        int q;
        std::cin >> q;
        if (blocks->location[q] < blocks->past[0])
        {
            reach(q);
        }
    }
    num_finals = num_reachable;
    rem_unreachable(Heads, Tails);
    std::cin >> alph_size;
    /* Make initial partition */
    touched = new int[num_transitions + 1];
    marked = new int[num_transitions + 1];
    marked[0] = num_finals;
    if (num_finals)
    {
        touched[touchedCount++] = 0;
        blocks->split();
    }
    cords = new VPartition;
    cords->init(num_transitions);
    if (num_transitions)
    {
        std::sort(cords->elements, cords->elements + num_transitions, cmp);
        cords->setCount = marked[0] = 0;
        int currentLabel = Labels[cords->elements[0]];
        for (int i = 0; i < num_transitions; ++i)
        {
            int t = cords->elements[i];
            if (Labels[t] != currentLabel)
            {
                currentLabel = Labels[t];
                cords->past[cords->setCount++] = i;
                cords->first[cords->setCount] = i;
                marked[cords->setCount] = 0;
            }
            cords->setOf[t] = cords->setCount;
            cords->location[t] = i;
        }
        cords->past[cords->setCount++] = num_transitions;
    }

    /* Split blocks and cords */
    make_adjacent(Heads);
    int b = 1, c = 0, i, j;
    while (c < cords->setCount)
    {
        for (i = cords->first[c]; i < cords->past[c]; ++i)
        {
            blocks->mark(Tails[cords->elements[i]]);
        }
        blocks->split(); ++c;
        while (b < blocks->setCount)
        {
            for (i = blocks->first[b]; i < blocks->past[b]; ++i)
            {
                for (j = F[blocks->elements[i]]; j < F[blocks->elements[i] + 1]; ++j)
                {
                    cords->mark(adjacent[j]);
                }
            }
            cords->split(); ++b;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "DFA minimized in " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " ms." << std::endl;
    std::cout << "DFA written to output.txt"<< std::endl;

    /* Count the numbers of transitions
    and final states in the result */
    int mo = 0, fo = 0;
    for (int t = 0; t < num_transitions; ++t)
    {
        if (blocks->location[Tails[t]] == blocks->first[blocks->setOf[Tails[t]]])
        {
            ++mo;
        }
    }
    for (int b = 0; b < blocks->setCount; ++b)
    {
        if (blocks->first[b] < num_finals)
        {
            ++fo;
        }
    }

    std::ofstream stream("output.txt");

    /* Print the result */
    stream << blocks->setCount << ' ' << mo << ' ' << blocks->setOf[q0] << ' ' << fo << '\n';
    for (int t = 0; t < num_transitions; ++t)
    {
        if (blocks->location[Tails[t]] == blocks->first[blocks->setOf[Tails[t]]])
        {
            stream << blocks->setOf[Tails[t]] << ' ' << Labels[t] << ' ' << blocks->setOf[Heads[t]] << '\n';
        }
    }
    for (int b = 0; b < blocks->setCount; ++b)
    {
        if (blocks->first[b] < num_finals)
        {
            stream << b << '\n';
        }
    }

    stream << alph_size << '\n';

    stream.close();
    delete blocks;
    delete cords;
    delete Tails, Labels, Heads, adjacent, F;
}