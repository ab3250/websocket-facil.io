#include <stdlib.h>

/**
 * Shuffle (randomize) the elements in the array.
 *
 * This is my own C implementation of the Fisher-Yates algorithm as
 * described by Donald Knuth in the 3rd editition of The Art of Computer
 * Programming Volume 2, Seminumerical algorithms (pp145-146).
 *
 * Algorithm P (Shuffling). Let X_1, X_2, ..., X_t be a set of t numbers to
 * be shuffled.
 *
 * P1. [Initialize.] Set j <-- t.
 * P2. [Generate U.] Generate a random number U, uniformly distributed
 *     between zero and one.
 * P3. [Exchange.] Set k <-- floor(j*U) + 1. (Now k is a random integer
 *     between 1 and j. Exercise 3.4.1-3 explains that k should *not* be
 *     computed by taking a ramainder modulo j.) Exchange X_k <--> X_j.
 * P4. [Decrease j.] Decrease j by 1. if j > 1, return to step P2.
 * END
 *
 * The above algorithm is equivalent to the algorithm found on the
 * Fisher-Yates Shuffle Wikipedia page. The latter is a more accurate
 * description of this implementation:
 *
 * To shuffle an array a of n elements (indices 0..n-1):
 *   for i from n - 1 downto 1 do
 *          j <-- random integer with 0 <= j <= i
 *                 exchange a[j] and a[i]
 *
 * PARAM: array - the array to shuffle
 *
 * SEE: http://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
 *      http://blog.codinghorror.com/the-danger-of-naivete/
 *      The art of computer programming Volume 2, Seminumerical
 *        algorithms / Donald E. Knuth
 */
void kfyshuffle(int array[])
{
  for (int i = ARRAY_SIZE - 1; i >= 1; --i)
    swap(array[i],              // swap the current element
         array[randgen(0, i)]);      // with some random element

}