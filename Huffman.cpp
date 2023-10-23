#include "Huffman.hpp"


/**
 * AUTHOR: dadezz
 * BRIEF:
 * La codifica di Huffman usa un codice senza prefissi 
 * che esprime il carattere più frequente nella maniera più breve possibile.
 * L'algoritmo lavora in questo modo:
 *  1. compressione:
 *      - creo un dizionario che associ ad ogni simbolo la sua frequenza, e lo ordino.
 *      - prendo gli ultimi due elementi del dizionario, li uso come foglie di un albero binario,
 *        elimino l'ultimo elemento del dizionario, fondendo i 2 nel penultimo, e riordino.
 *        continuo con questo procedimento fino a creare un unico albero e un solo nodo nel dizionario.
 *      - nel file di output, appendo in testa l'albero come rappresentazione a parentesi
 *        bilanciate, poi per ogni lettera navigo l'albero e setto i bit
 *  2. decompressione:
 *      - faccio il parsing delle parentesi bilanciate e mi ricreo l'albero. 
 *      - Ad ogni successivo bit corrisponde la scelta di un ramo dell'albero binario
 *        finché non arrivo a una codifica valida (foglia) poi ricomincio.
 */

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

    delete head; //libero la memoria dell'albero (nella fase di decompressione ne creerei uno nuovo potenzialmente sullo stesso oggetto HuffmanTree)
}


void HuffmanTree::decompress(std::istream& input, std::ostream& output){
    
    /*FIRST PART:
    Create tree*/
    head = get_tree_from_encoded_stream(input);

    /*SECOND PART:
    streaming input file, decoding it*/ 
    decode(input, output);
}

enum carattere_appartiene_a_stringa {CARATTERE_NON_IN_STRINGA, STRINGA_COINCIDE_CON_CARATTERE, CARATTERE_IN_STRINGA};
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

inline void HuffmanTree::navigate_tree(char next_char, char& byte, u_short& position, std::ostream& output){
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
            position = (position+1) % BIT_IN_A_BYTE;
            set_bit_zero(byte, position);
            // and move on the left child node
            aux = aux->sinistra;
            // last thing is: if i am on a leaf, i have found the char
            if (here == STRINGA_COINCIDE_CON_CARATTERE) found = true;            
        }
        else {
            // in this case, I know for sure that the char is on the right branch of the tree
            // so, i can write the bit to One
            position = (position+1) % BIT_IN_A_BYTE;
            set_bit_one(byte, position);
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

/**
 * DECODIFICA
 * inizio col parsing dell'albero
*/

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

    char c = input.peek();
    albero aux = head;
    char mask = (1<<7); // byte 10000000
    bool delimiter = false;
    
    while(c != -1 && !delimiter){ 
        //finché non raggiungo la fine dello stream o finché non trovo il carattere'\0'
        //continuo a prendere caratteri in ingresso.
        c = input.get();

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
                if (tmp == '\0') delimiter = true;
                else output<<tmp;
                aux = head;
            }
        }
    }

}
#if PROVE
int main(int argc, char* argv[]) {
    
    #if PROVE
    std::cout<<argc<<std::endl;
    for (int i= 0; i<4; i++){
        std::cout<<argv[i]<<std::endl;
    }
    if (argc != 3 || (argv[1] != "-e" && argv[1] != "-d")) {
        // I am expecting 2 arguments: -e/-d (encode/decode) andinput file
        std::cout<<std::boolalpha<<(argc != 3)<<std::endl;
        std::cout<<std::boolalpha<<(argv[1] != "-e")<<std::endl;
        std::cout<<std::boolalpha<<(argv[1] != "-d")<<std::endl;

        std::cout << "Usage: " << argv[0] << " -e input_file.txt (for compression)" << std::endl;
        std::cout << "       " << argv[0] << " -d input_file.huf (for decompression)" << std::endl;
        return 0; // Invalid usage, exit with an error code
    }
    
    // open input file
    std::string input_file = argv[2];
    #endif
    std::ifstream infile("prova.txt", std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }
    
    // Generate the output file name based on the input file
    std::string output_file_name = "prova.huf";

    HuffmanTree a;
    #if PROVE
    if (argv[1] == "-e") {
        // checks if the extension exist; removes it and adds ".huf" as output filename
        size_t extension_pos = input_file.find_last_of(".");
        if (extension_pos != std::string::npos) {
            output_file_name = input_file.substr(0, extension_pos) + ".huf";
        } else {
            // if file passed by input doesn't have any extension
            output_file_name = input_file + ".huf";
        }
        #endif
        std::ofstream outfile(output_file_name, std::ios::binary);

        // code for decompression
        a.compress(infile, outfile);
        #if PROVE
    }
    else {

        // Check if the input file ends with .huf, and remove it to generate the output file name
        if (input_file.length() >= 4 && input_file.substr(input_file.length() - 4) == ".huf") {
            output_file_name = input_file.substr(0, input_file.length() - 4);
        } else {
            std::cerr << "Error: Input file must have a .huf extension for decompression." << std::endl;
            return 1;
        }

        std::ofstream outfile(output_file_name, std::ios::binary);

        // code for decompression
        a.decompress(infile, outfile);
    }
    #endif
    #if PROVE
    infile.clear();
    infile.close();
    outfile.clear();
    outfile.close();

    output_file_name = "decomp.txt";
    std::ifstream infile2("prova.huf", std::ios::binary);
    std::ofstream outfile2(output_file_name, std::ios::binary);

    a.decompress(infile2, outfile2);
    #endif
    return 0;
}
#endif

int main(){
    HuffmanTree a;
    std::ifstream infile2("prova.txt", std::ios::binary);
    std::ofstream outfile2("prova.huf", std::ios::binary);

    a.compress(infile2, outfile2);


    infile2.close();
    outfile2.close();
    
    
    std::ifstream infile("prova.huf", std::ios::binary);
    std::ofstream outfile("provad", std::ios::binary);

    a.decompress(infile, outfile);


    infile.close();
    outfile.close();
    

    return 0;

}
