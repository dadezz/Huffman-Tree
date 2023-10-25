#include "Huffman.hpp"

/**
 * BIT MASKING
 * posso fare bitmasking con operatori bitwise:
 *  - Bitwise AND operator: estract a subset of bits in a value
 *  - Bitise OR operator: set a subset of bits in a value
 *  - Bitwise XOR operator: toggle a subset of bits in a value
 * 
 * GET A BIT
 * find out the third bit from the right.
 * 
 * Per esempio, ho il numero 12 che è codificato come:
 * 
 * 00000000000000000000000000001100
 * 
 * uso come maschera il numero 1, così codificato:
 * 
 * 00000000000000000000000000000001
 * 
 * lo shifto di 2 bit a sinistra: 1<<2
 * 
 * 00000000000000000000000000000100
 * 
 * se (12 & (1<<2)) è diverso da 0, abbiamo 1 in quella posizione, altrimenti abbiamo 0
 * 
 * SET A BIT
 * we want to set fourth bit to zero of 12 from the right
 * 
 * 00000000000000000000000000001100
 * 
 * uso come maschera il numero 1, così codificato:
 * 
 * 00000000000000000000000000000001
 * 
 * muovo il 1 alla quarta posizione:  1<<3:
 * 
 * 00000000000000000000000000001000
 * 
 * ora inverto i bit col complementare: ~(1<<3)
 * 
 * 11111111111111111111111111110111
 * 
 * ora con l'operatore AND possiamo vedere che 12&(~(1<<3)) è:
 * 
 * 00000000000000000000000000000100
 * 
 * int a = 12;
 * int mask = 1;
 * mask = mask<<3;
 * mask = (~mask);
 * a = a & mask;
 * 
 * Ugualmente, voglio settare a 1 il 5° bit da destra, sempre del numero 12
 * 
 * quindi ho:
 * 00000000000000000000000000001100     // 12
 * 00000000000000000000000000000001     // 1
 * 00000000000000000000000000010000     // 1<<4
 * 00000000000000000000000000011100     // 12 | (1<<4)
 * 
 * int a = 12;
 * int mask =  1<<4;
 * a = a | mask;
*/


void HuffmanTree::set_bit_zero(char& byte, u_short position){
    // sinistra
    /**
     * position 0: bit + significativo -> 1<<7  8-0-1=7
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
    // destra
    char mask = 1 << (BIT_IN_A_BYTE - position - 1);
    byte = byte | mask;
}