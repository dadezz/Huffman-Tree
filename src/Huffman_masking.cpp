#include "Huffman.hpp"

/**
 * BIT MASKING
 * Bitmasking can be done using bitwise operators:
 *  - Bitwise AND operator: extract a subset of bits in a value
 *  - Bitwise OR operator: set a subset of bits in a value
 *  - Bitwise XOR operator: toggle a subset of bits in a value
 * 
 * GET A BIT
 * Find out the third bit from the right.
 * 
 * For example, let's take the number 12, which is encoded as:
 * 
 * 00000000000000000000000000001100
 * 
 * We use the mask as the number 1, which is encoded as:
 * 
 * 00000000000000000000000000000001
 * 
 * Shift it 2 bits to the left: 1 << 2
 * 
 * 00000000000000000000000000000100
 * 
 * If (12 & (1 << 2)) is not equal to 0, we have 1 in that position; otherwise, we have 0.
 * 
 * SET A BIT
 * We want to set the fourth bit to zero, starting from the right, of 12.
 * 
 * 00000000000000000000000000001100
 * 
 * We use the mask as the number 1, which is encoded as:
 * 
 * 00000000000000000000000000000001
 * 
 * Move the 1 to the fourth position: 1 << 3
 * 
 * 00000000000000000000000000001000
 * 
 * Now, invert the bits with the complement: ~(1 << 3)
 * 
 * 11111111111111111111111111110111
 * 
 * Now, with the AND operator, we can see that 12 & (~(1 << 3)) is:
 * 
 * 00000000000000000000000000000100
 * 
 * int a = 12;
 * int mask = 1;
 * mask = mask << 3;
 * mask = ~mask;
 * a = a & mask;
 * 
 * Similarly, I want to set the 5th bit from the right to 1, still for the number 12:
 * 
 * So, we have:
 * 00000000000000000000000000001100     // 12
 * 00000000000000000000000000000001     // 1
 * 00000000000000000000000000010000     // 1 << 4
 * 00000000000000000000000000011100     // 12 | (1 << 4)
 * 
 * int a = 12;
 * int mask = 1 << 4;
 * a = a | mask;
 */


void HuffmanTree::set_bit_zero(char& byte, u_short position){
    // left
    /**
     * position 0: 1<<7  8-0-1=7
     * position 1: 1<<6  =  8-1-1 = 6
     * position 2: 1<<5  =  8-2-1 = 5
     * position 3: 1<<4  =  8-3-1 = 4
     * position 4: 1<<3  =  8-4-1 = 3
     * position 5: 1<<2  =  8-5-1 = 2
     * position 6: 1<<1  =  8-6-1 = 1
     * position 7: 1<<0  =  8-7-1 = 0
    */
    char mask = 1 << (BIT_IN_A_BYTE - position - 1);
    byte = byte & ~mask;
}

void HuffmanTree::set_bit_one(char& byte, u_short position){
    // right
    char mask = 1 << (BIT_IN_A_BYTE - position - 1);
    byte = byte | mask;
}