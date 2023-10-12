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

    /**
     * gli apici ho scelto di metterli in un secondo momento, quando ho
     * fatto il parsing durante la decodifica:
     * Se un testo da comprimere contenesse delle parentesi, il successivo albero 
     * risulterebbe avere parentesi non bilanciate (impossibile distinguere i rami dalle foglie)
    */
    if (nodo->destra == nullptr && nodo->sinistra == nullptr) return "'" + nodo->codifica + "'" ;
    if (nodo->destra == nullptr && nodo->sinistra != nullptr) return create_string_tree(nodo->sinistra) + "," + "'" + nodo->codifica + "'";
    if (nodo->sinistra == nullptr && nodo->destra != nullptr) return "'" + nodo->codifica + "'" + "," + create_string_tree(nodo->destra);
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

string HuffmanTree::encode (std::istream& input){
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

/**
 * DECODIFICA
 * inizio col parsing dell'albero
*/

HuffmanTree::albero HuffmanTree::parse_node (std::istream& input){
    
    char c;
    
    albero this_node = new nodo;
    this_node->sinistra = get_tree_from_encoded_stream(input);
    this_node->codifica += this_node->sinistra->codifica;
    this_node->sinistra->padre = this_node;

    c = input.get(); // consumes ','
    if (c != ',') std::cout<<"(virgola) ah rega sto parsing che pigna in culo"<<std::endl;


    this_node->destra = get_tree_from_encoded_stream(input);
    this_node->codifica += this_node->destra->codifica;
    this_node->destra->padre = this_node;

    if (input.peek() != ')') std::cout<<"(parentesi chiusura) ah rega sto parsing che pigna in culo"<<std::endl;
    
    return this_node;
}

inline HuffmanTree::albero HuffmanTree::parse_leaf (std::istream& input){
    char c;
    c = input.get();
    albero this_node = new nodo;
    this_node->sinistra = this_node->destra = nullptr;
    this_node->codifica = c;

    if (input.peek() != '\'') std::cout<<"(apice chiusura) ah rega sto parsing che pigna in culo"<<std::endl;

    return this_node;
}

HuffmanTree::albero HuffmanTree::get_tree_from_encoded_stream(std::istream& input){
    
    char c;
    albero this_node;
    
    if (input.eof()) {
        std::cout<<"wtf eof"<<std::endl;
        return nullptr;
    }

    c = input.get();
    
    if (c == '(')         
        this_node = parse_node(input); // consumes something like ( ... , ... )
    else if (c == '\'')
        this_node = parse_leaf(input); // consumes something like 'c'
    else std::cout<<"(parentesi o apice iniziali) ah rega sto parsing che pigna in culo"<<std::endl;
    
    c = input.get(); // consumes ')' or '''

    this_node->padre = nullptr;

    return this_node;
}

string HuffmanTree::read_bit(std::istream& input){
    //ricordo che la sinistra corrisponde allo 0

    char c;
    string output;
    albero aux = head;
    char mask = (1<<7);
    bool delimiter = false;
    
    while(input.peek() != -1 && !delimiter){
        c = input.get();
        for (u_short i = 0; i<8; i++){
            aux = c & mask == 0 ? aux->sinistra : aux->destra; // navigo nell'albero
            c = c<<1; // shifto i bit per la prosima iterazione
            if (aux->codifica.length() == 1) {
                // se arrivo a una foglia, appendo il carattere corrispondente e 
                // riporto il puntatore a root
                char tmp = aux->codifica[0];
                if (!delimiter) output += tmp;
                if (tmp == '\0') delimiter = true;
                aux = head;
            }
        }
    }

    return output;
}

string HuffmanTree::decode(std::istream & input){
    head = get_tree_from_encoded_stream(input);

    string output;

    // mi son reffato che non c'è bisogno di alcun buffer.
    // al posto di segnarmi i bit e controllare in continuazione se la codifica
    // è giusta, mi sposto man mano dentro l'albero, finché non arrivo alla foglia,
    // e appendo la codifica del nodo

    /**
     * i have the last byte of my file, for example `10010000` where the first 
     * four bits are the encoding of `a`, which is the LAST letter of the original 
     * uncompressed file. The problem is that `0000` (the last four bits) is also a 
     * valid encoding, in particular for the letter `b`. What can i do to avoid 
     * the parser to return "ab" instead of just "a"?
     * 
     * i can add a delimiter in the phase of encoding, for wxample '\0'
    */
    if (input.peek() != -1)
        output += read_bit(input);

    return output;

}