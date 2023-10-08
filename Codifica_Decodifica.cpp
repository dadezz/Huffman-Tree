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
string HuffmanTree::codifica (std::istream input){
    string output = create_string_tree(head);
}