#include <vector>

class Partition
{
    /*this class defines the partition data structure used by both Hopcroft and Valmari-Lehtinen.
    It is implemented as specified in Valmari/Lehtinen's paper (Section 3, Refinable Partitions),
    except I use zero-indexing instead of 1-indexing 
    */
  private:
    int max; //total number of elements summed from all partitions

    int sets; // total number of partitions

    std::vector<int> elems; //Contains 0, ...., max - 1, such that the elements that belong to the same set are grouped together

    std::vector<int> loc; //Returns the index for an element e in elems, i.e., elems[loc[e]] = e

    std::vector<int> sidx; //the index of the set that element e belongs to

    //for each set s, first[s] defines the first element of set s in elems, and last[s] defines the last element
    //since in elems, the elements are grouped together, for a certain set s, the elements in elems would be
    //elems[first[s]], elems[first[s] + 1], ..., elems[end[s] - 1]
    std::vector<int> first;

    std::vector<int> end;

    //let f = first[s], and l = end[s], and m = mid[s]. Then, the marked elements are on the left side of m, i.e., elems[f], ..., elems[m - 1]
    //and the unmarked elements are elems[m], ..., elems[l - 1]
    std::vector<int> mid;

  public:
    int size(int s); //returns number of elements in set with index s

    int set(int e); //returns index of the set that element with index e belongs to

    int get_first(int s); // returns the first element of the set with index s

    int get_next(int e); //Works like an iterator for a set, so to traverse a set s, we do e = first(s); while(e != -1){e = next(e);}

    int mark(int e); //Marks element with index e for the splitting of a set

    //if either none or all elements of set s have been marked, then returns 0, and does nothing
    //otherwise, remove the marked elements from the set, and put them into a new set of marked elements
    //and return its index. In both cases, unmarks all elements in the set or sets.
    int split(int s);

    int get_max();// return number of elements summed from all partitions

    int get_sets(); //return total number of partitionsf

    bool no_marks(int s); //checks if none of the elements in set s are marked, and returns True otherwise False

    void pretty_print();//does exactly what the function name says
    
    Partition(int max); //default constructor for each partition instance
};