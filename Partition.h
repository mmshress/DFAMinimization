#include <vector>

class Partition{
    /*this class defines the partition data structure used by both Hopcroft and Valmari-Lehtinen.
    It is implemented as specified in Valmari/Lehtinen's paper (Section 3, Refinable Partitions),
    except I use zero-indexing instead of 1-indexing 
    */
   int max; //total number of elements summed from all partitions

   int sets;// total number of partitions

   int size(int s);//returns number of elements in set with index s

   int set(int e);//returns index of the set that element with index e belongs to

   int first(int s); // returns the first element of the set with index s

   int next(int e); //Works like an iterator for a set, so to traverse a set s, we do e = first(s); while(e != -1){e = next(e);}

   int mark(int e); //Marks element with index e for the splitting of a set

   //if either none or all elements of set s have been marked, then returns 0, and does nothing
   //otherwise, remove the marked elements from the set, and put them into a new set of marked elements
   //and return its index. In both cases, unmarks all elements in the set or sets.
   int split(int s); 
   
   bool no_marks(int s); //checks if none of the elements in set s are marked, and returns True otherwise False
   
   vector
}