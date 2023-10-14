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
*/

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

void HuffmanTree::insert(string x){
    /**
     * if a char has yet been met, it just increases the counter of the relative cell;
     * else a new cell is created and set as the tail of the dictionary
    */
    bool inserito = false;
    dizionario* aux = dict_head;
    while(aux && !inserito){
        if (x == aux->info.first) {
            aux->info.second += 1;
            inserito = true;
        }
        aux = aux->next;
    }
    if (inserito == true) return;
    aux = new dizionario;
    aux->info.first = x;
    aux->info.second = 1;
    if (dict_head == nullptr) {
        dict_head = dict_tail = aux;
        aux->next = aux->prev = nullptr;
    }
    else {
        dict_tail->next = aux;
        aux->next = nullptr;
        aux->prev = dict_tail;
        dict_tail = aux;
    }

}

void HuffmanTree::swap(dizionario* & a, dizionario* & b){
    pair<string, int> aux = a->info;
    a->info = b->info;
    b->info = aux;

    albero aux2 = a->nodo_corrispondente;
    a->nodo_corrispondente = b->nodo_corrispondente;
    b->nodo_corrispondente = aux2;
}

void HuffmanTree::bubble_sort() {
    //i want the dictionary to be sorted with the most common letter in the first place    
    dizionario *aux1, *aux2;
    for (aux1 = dict_head; aux1 != nullptr; aux1 = aux1->next) {
        for (aux2 = dict_head; aux2 != nullptr; aux2 = aux2->next) {
            if (aux2->next != nullptr && aux2->info.second <= aux2->next->info.second) {
                swap(aux2, aux2->next);
            }
        }
    }

    aux1 = dict_head;
    while(aux1->next) 
        aux1 = aux1->next;
    dict_tail = aux1;
}


void HuffmanTree::stampa_dizionario(){
    dizionario* aux = dict_head;
    while(aux) {
        std::cout<<"( "<<aux->info.first<<" : "<<aux->info.second<<" )"<<std::endl;
        aux = aux->next;
    }
}

void HuffmanTree::create_binary_node(dizionario* maggiore, dizionario* minore){
    /**
     * this function creates a binary tree with one root and two leaf nodes 
     * (well, they can be trees, too).
     * For Example, 
     * 
     * let our dictionary contain tree elements: 
     * a : 2
     * b : 1
     * c : 4
     * 
     * the function takes the elements "a" and "b", creates a tree
     * with the node "ab" as root and replace "a" and "b" with the element
     * "ab : 3" in the dictionary
     * 
     * So, now we'll have this combination in the class:
     * 
     * The dictionary will be: [ (c:4)*, (ab:3) ], where the * is the head
     * 
     * The tree will be (ab:3) -> (a:2)
     *                        \-> (b:1)
     * 
     * minchia in effetti mi serve modificare il dizionario per far si che stori anche l'indirizzo del nodo
    */  
    
    albero radice = new nodo;
    radice->codifica = maggiore->info.first + minore->info.first;
    albero mag /*sinistra (bit 0) */, min /*destra (bit 1)*/;

    /**
     * creo l'albero bottom-up
    */

    if (maggiore->nodo_corrispondente == nullptr) {
        mag = new nodo;
        mag->destra = mag->sinistra = nullptr;
        mag->codifica = maggiore->info.first;
    }
    else mag = maggiore->nodo_corrispondente;
    if (minore->nodo_corrispondente == nullptr) {
        min = new nodo;
        min->destra = min->sinistra = nullptr;
        min->codifica = minore->info.first;
    }
    else min = minore->nodo_corrispondente;
    mag->padre = radice;
    min->padre = radice;
    radice->destra = min;
    radice->sinistra = mag;

    /**
     * risistemo il dizionario
    */

    maggiore->info.first = radice->codifica;
    maggiore->info.second = maggiore->info.second + minore->info.second;
    maggiore->nodo_corrispondente = radice;

    if (minore->next ==nullptr) delete minore;
    
    maggiore->next = nullptr;
    dict_tail = maggiore;


    this->bubble_sort();
}

HuffmanTree::albero HuffmanTree::create_tree(){
    if (dict_head == nullptr) return nullptr;
    while (dict_head->next != nullptr){
        create_binary_node(dict_tail->prev, dict_tail);
    }
    head = dict_head->nodo_corrispondente;

    /**
     * At this point, head of the tree is set, 
     * i can delete the last node of the dictionary and set the pointers to nullptr
    */

    delete dict_head;
    dict_head = dict_tail = nullptr;
    
    return head;
}

void HuffmanTree::stampa_albero_rec(albero radice, int space){
    if (radice == nullptr) return;

    //prima faccio il ramo destro
    stampa_albero_rec(radice->sinistra, space+10);

    //faccio la stampa effettiva
    std::cout<<std::endl;
    for (int i=0; i<space; i++) std::cout<<" ";
    std::cout<<radice->codifica<<std::endl;

    //prosegup col sinistro
    stampa_albero_rec(radice->destra, space+10);

}

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
    c = input.peek();
    input.clear();
    c = input.peek();
    input.seekg(0);
    c = input.peek();

    encode(input, output);

}


void HuffmanTree::decompress(std::istream& input, std::ostream& output){
    
    /*FIRST PART:
    Create tree*/
    head = get_tree_from_encoded_stream(input);

    /*SECOND PART:
    streaming input file, decoding it*/
    string output_string = decode(input);
    output<<output_string;
}

string HuffmanTree::create_string_tree(HuffmanTree::albero nodo){
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

inline void HuffmanTree::navigate_tree(char next_char, char& byte, u_short position, std::ostream& output){
    albero aux = head;
    bool found = false;
    while (!found) {

        // binary search
        int here = check_encoding_char(next_char, aux->sinistra->codifica);
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
            output<<byte;
            byte = 0;
        }
    }

}

void HuffmanTree::encode (std::istream& input, std::ostream& output){
    output<<create_string_tree(head);
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
         * 
         * 
         * NON USO STRINGHE AUSILIARIE, MA BUTTO DIRETTAMENTE TUTTO IN OUTPUT
        */

    u_short position = 0;
    char byte;
    char next_char = input.peek();
    
    while (next_char != -1) {
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
#include "Huffman.hpp"


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

    return 0;

    infile2.close();
    outfile2.close();
    
    
    std::ifstream infile("prova.huf", std::ios::binary);
    std::ofstream outfile("provad", std::ios::binary);

    a.decompress(infile, outfile);




}