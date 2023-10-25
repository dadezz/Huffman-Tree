#include "Huffman.hpp"

void HuffmanTree::compress(std::istream& input, std::ostream& output){
    
    /*FIRST PART:
    Create frequency dictionary*/    
    char c;
    do {
        c = input.get();
        if (c != -1) insert(string{c});
    } while (c != -1);
    insert(string{'\0'}); //aggiungo un delimitatore per il fine testo
    bubble_sort();

    /*SECOND PART:
    from the dictionary, create the tree*/
    create_tree();



    /*THIRD PART:
    streaming input file, encoding it*/
    input.clear();
    input.seekg(0);

    encode(input, output);

    if (head) delete head; //libero la memoria dell'albero (nella fase di decompressione ne creerei uno nuovo potenzialmente sullo stesso oggetto HuffmanTree)
}

void HuffmanTree::decompress(std::istream& input, std::ostream& output){
    
    /*FIRST PART:
    Create tree*/
    head = get_tree_from_encoded_stream(input);

    /*SECOND PART:
    streaming input file, decoding it*/ 
    decode(input, output);
}

void HuffmanTree::encode (std::istream& input, std::ostream& output){
/* CODIFICA: 
 * head punta alla radice dell'albero
 * Procedo con questi passaggi:
 *  0)  Creo stringa con l'albero, usando metodo delle parentesi
 *      bilanciate (()()), e sarà l'inizio del file di output
 *  1)  la variabile short int position tiene conto della posizione nel byte,
 *      quindi un numero da 0 (bit piu significativo) a 8
 *  2)  Per ogni lettera che incontro, faccio partire la 
 *      ricerca binaria sull'albero
 *  3)  Per ogni passo, a seconda del ramo dx/sx, 
 *      storo un bit nella posizione "position" (bit masking)
 *  4)  Distruggo tutto ciò che c'è in heap
 */
    output.clear();

    create_string_tree(head, output);
    /**
     * devo aggiungere un byte all'output SOLO quando è completo 
     * con tutti i bit settati.
     *  - tengo conto della posizione [0-7] a cui sto scrivendo.
     *  - mi porto dietro by reference sia la stringa di partenza che
     *    il byte su cui sto scrivendo
     *  - finché cerco il carattere nell'albero binario, ogni passo setto il bit
     *  - se ne ho già settati 8, appendo il byte creato, resetto il char
     *    e ricomincio a settare il tutto
    */

    input.clear();
    output.clear();

    u_short position = 0;
    char byte;
    char next_char = input.peek();
    
    while (next_char != -1) {

        input.clear();
        output.clear();

        next_char = input.get();
        if (next_char != -1) navigate_tree(next_char, byte, position, output);
    }
    next_char = '\0'; // added at the end of the file, it is needed for decompression
    navigate_tree(next_char, byte, position, output);

    if (position > 0) output<<byte;

}

void HuffmanTree::decode(std::istream & input, std::ostream& output){
    //ricordo che il ramo a sinistra corrisponde allo 0, la destra a 1

    /**
     * i have the last byte of my file, for example `10010000` where the first 
     * four bits are the encoding of `a`, which is the LAST letter of the original 
     * uncompressed file. The problem is that `0000` (the last four bits) is also a 
     * valid encoding, in particular for the letter `b`. What can i do to avoid 
     * the parser to return "ab" instead of just "a"?
     * 
     * i can add a delimiter in the phase of encoding, for example '\0'
    */

    char c = input.get();
    albero aux = head;
    char mask = (1<<7); // byte 10000000
    bool delimiter = false;
    
    while(!input.eof() && !delimiter){ 
        //finché non raggiungo la fine dello stream o finché non trovo il carattere'\0'
        //continuo a prendere caratteri in ingresso.
        
        for (u_short i = 0; i<BIT_IN_A_BYTE; i++){
            // for: devo leggere ogni bit del byte in ingresso (quindi 8 iterazioni)

            aux = ((c & mask) == 0 ? aux->sinistra : aux->destra); 
            // navigo nell'albero:
            // se il bit è uno 0, sposto il puntatore dell'albero al ramo di sinistra.
            // altrimenti lo sposto al ramo di destra
            
            c = c<<1; // shifto i bit per la prosima iterazione (guardo sempre il primo bit)

            if (aux->codifica.length() == 1) {
                // se arrivo a una foglia (la lunghezza della codifica è 1),
                // appendo il carattere corrispondente allo stream di output e 
                // riporto il puntatore a root
                // Il tutto, solo se la foglia trovata è un carattere valido e non il '\0'
                char tmp = aux->codifica[0];
                if (tmp == '\0') {
                    delimiter = true;
                    break;
                }
                else output<<tmp;
                aux = head;
            }
        }
        c = input.get();
    }

}

enum carattere_appartiene_a_stringa {
    CARATTERE_NON_IN_STRINGA, 
    STRINGA_COINCIDE_CON_CARATTERE, 
    CARATTERE_IN_STRINGA
};
carattere_appartiene_a_stringa check_encoding_char(char c, string s){
    if (s.length() == 1) 
        return s[0] == c ? STRINGA_COINCIDE_CON_CARATTERE : CARATTERE_NON_IN_STRINGA;
    bool found = 0;
    int i = 0;
    while (i<s.length() && !found){
        if (s[i] == c) 
            found = true;
        i++;
    }
    return found ? CARATTERE_IN_STRINGA : CARATTERE_NON_IN_STRINGA;
}

void HuffmanTree::navigate_tree(char next_char, char& byte, u_short& position, std::ostream& output){
    albero aux = head;
    bool found = false;
    while (!found) {

        /*
            FIRST STEP: BINARY SEARCH
        */
        carattere_appartiene_a_stringa here = check_encoding_char(next_char, aux->sinistra->codifica);
        // (I start with the left branch)
        if (here == STRINGA_COINCIDE_CON_CARATTERE || here == CARATTERE_IN_STRINGA) {
            // in this case, i know for sure that the char is on the left branch of the tree
            // So, i can write the bit to Zero
            this->set_bit_zero(byte, position);
            position = (position+1) % BIT_IN_A_BYTE;
            // and move on the left child nodeS
            aux = aux->sinistra;
            // last thing is: if i am on a leaf, i have found the char
            if (here == STRINGA_COINCIDE_CON_CARATTERE) found = true;            
        }
        else {
            // in this case, I know for sure that the char is on the right branch of the tree
            // so, i can write the bit to One
            set_bit_one(byte, position);
            position = (position+1) % BIT_IN_A_BYTE;
            // i need to check if i am on the leaf or just on a node:
            // i call again the function, but for the right branch
            here = check_encoding_char(next_char, aux->destra->codifica);
            aux = aux->destra;
            // last thing is: if i am on a leaf, i have found the char
            if (here == STRINGA_COINCIDE_CON_CARATTERE) found = true;
        }

        // if i completed a byte, i have to append it to the output string and reset 
        // the bits that have to be written
        if (position == 0) {
            output<<byte;
            byte = 0;
        }
    }

}