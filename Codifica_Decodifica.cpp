#include "Huffman.hpp"


/**
 * BRIEF:
 * PARTE DI CODICE RELATIVO A CODIFICA E DECODIFICA
 * 
 * AUTHOR: dadezz
 * 
 * CODIFICA: 
 * Arrivo a sto punto che ho l'albero fatto e finito
 * Procedo con questi passaggi:
 *  0)  Creo stringa con l'albero, usando metodo delle parentesi
 *      bilanciate (()()), e sarà l'inizio del file di output
 *  1)  Storo la posizione, ovvero mi faccio una variabile
 *      short int che mi tenga conto della posizione nel byte,
 *      quindi un numero da 0 (bit piu significativo) a 8
 *  2)  Per ogni lettera che incontro, faccio partire la 
 *      ricerca binaria sull'albero
 *  3)  Per ogni passo, a seconda del ramo dx/sx, 
 *      storo un bit nella posizione storata nello short int 
 *      del punto (1):  
 *          In pratica, un switch-case con 8 maschere, 
 *          una per ogni bit che mi serve
 *  4)  Distruggo tutto ciò che c'è in ram (potrei
 *      infatti chiudere il programma, avendo storato su file 
 *      l'albero e il testo compresso
 * 
 * DECODIFICA:
 * Arrivo a questo punto del programma che ho NULLA in Ram,
 * nel file ho l'albero indicato da parentesi bilanciate e
 * seguito dalla serie di bit che sarebbe il testo compresso
 * Procedo con questi passaggi:
 *  1)  Parso le parentesi bilanciate per ricostruire l'albero
 *  2)  Costruisco un buffer char[8]
 *  3)  Leggo il bit 0 e lo salvo sul buffer
 *  4)  Shifto i bit verso sinistra (posiz 8 va in 7 e cosi via)
 *  5)  Ripeto i passaggi 3-4 finché non trovo una codifica valida 
 *      per una lettera o finché non è finito il byte (8 shift).
 *          Se arrivo a una codifica valida, svuoto il buffer, 
 *          storo la lettera e continuo i passaggi  
 *          Se arrivo invece alla fine del byte, passo semplicemente
 *          al prossimo, percHé tanto il buffer contiene già i bit
 *          del byte precedente.
 *  N.B.:   il buffer mi serve 
 * 
 * 
*/
std::string HuffmanTree::create_string_tree(HuffmanTree::albero nodo){
    //punto 0 della codifica
    if (nodo->destra == nullptr && nodo->sinistra == nullptr) return nodo->codifica;
    if (nodo->destra == nullptr && nodo->sinistra != nullptr) return create_string_tree(nodo->sinistra) + "," +nodo->codifica;
    if (nodo->sinistra == nullptr && nodo->destra != nullptr) return nodo->codifica + "," + create_string_tree(nodo->destra);
    else return "(" + create_string_tree(nodo->sinistra) + "," + create_string_tree(nodo->destra) + ")";
}

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

inline void HuffmanTree::set_bit_zero(char& byte, u_short position){
    // sinistra
    char mask = 1;
    for (short i=0; i<(8-position); i++){
        mask = mask<<1;
    }
    mask = (~mask);
    byte = byte & mask;
}

inline void HuffmanTree::set_bit_one(char& byte, u_short position){
    // destra
    char mask = 1;
    for (short i=0; i<(8-position); i++){
        mask = mask<<1;
    }
    byte = byte | mask;
}

/**
 * per quanto riguarda la codifica, 
 * ho due possibilità:
 *  - scannerizzare on-the-fly l'albero per trovare la codifica
 *  - scrivermi la codifica su un unordered map
 * 
 * in effetti però la codifica sarebbe qualcosa del tipo
 * 'a' = "01101"
 * 
 * per scrivere i bit serve scorrere la stringa di 5 caratteri
 * nella ricerca on-the-fly, ci sono 5 scelte di ramo da fare
 * 
 * i passaggi son quindi sempre 5, ma la ricerca on the fly non richiede spazio 
 * aggiuntivo, userò quella.
*/

int check_encoding_char(char c, string s){
    if (s.length() == 1) 
        return s[0] == c ? 1 : 0;
    bool found = 0;
    int i = 0;
    while (i<s.length() && !found){
        if (s[i] == c) 
            found = true;
        i++;
    }
    return found ? 2 : 0;
}

inline void HuffmanTree::navigate_tree(char next_char, char& byte, u_short position, string& output){
    albero aux = head;
    bool found = false;
    while (!found) {

        // binary search
        bool here = check_encoding_char(next_char, aux->sinistra->codifica);
        if (here == 1 || here == 2) {
            position = (position+1) % 8;
            set_bit_zero(byte, position);
            aux = aux->sinistra;
            if (here == 1) found = true;            
        }
        else {
            // i know for sure that the char is on this branch
            position = (position+1) % 8;
            set_bit_one(byte, position);
            here = check_encoding_char(next_char, aux->destra->codifica);
            aux = aux->destra;
            if (here == 1) found = true;
        }

        // if i completed a byte, i have to append it to the output string and reset 
        // the bits that have to be written
        if (position == 0) {
            output += byte;
            byte = 0;
        }
    }

}

string HuffmanTree::encode (std::istream input){
    string output = create_string_tree(head);
        /**
         * ho una stringa che sarà il testo finale.
         * devo aggiungere un byte SOLO quando è completo 
         * con tutti i bit settati.
         * Potrei procedere così:
         *  - tengo conto della posizione [0-7] a cui sto scrivendo.
         *  - mi porto dietro by reference sia la stringa di partenza che
         *    il byte su cui sto scrivendo
         *  - finché cerco il carattere nell'albero binario, ogni passo setto il bit
         *  - se ne ho già settati 8, appendo il byte creato, resetto il char 
         *    (posso tranquillamente stare nello heap e riutilizzare il char, perché non
         *    vado ad appenderlo alla stringa per indirizzo, ma per copia)
         *    e ricomincio a settare il tutto
         *  - A naso, il tutto risulta più facile usando la ricerca binaria iterativa
        */

    u_short position = 0;
    char byte;

    while (input.peek() != -1) {
        char next_char = input.get();
        navigate_tree(next_char, byte, position, output);
    }

    if (position > 0) output += byte;

    return output;
}

