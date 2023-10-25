#include "Huffman.hpp"

/*
 * ALL FUNCTIONS CONCERNING CREATION OF THE FREQUENCY DICTIONARY
 * AND THE BINARY TREE DATA STRUCTURES
*/


/**
 * INSERT: x is a single char casted to a std::string.
 * The function creates the frequency dictionary
 * if a char has yet been met, it just increases the counter of the relative cell;
 * else a new cell is created and set as the tail of the dictionary
*/
void HuffmanTree::insert(string x){

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

/**
 * SWAP: swap between two elements of the dictionary
 * (used in bubble_sort)
*/
void HuffmanTree::swap(dizionario* & a, dizionario* & b){
    pair<string, int> aux = a->info;
    a->info = b->info;
    b->info = aux;

    albero aux2 = a->nodo_corrispondente;
    a->nodo_corrispondente = b->nodo_corrispondente;
    b->nodo_corrispondente = aux2;
}

/**
 * BUBBLESORT: i want the dictionary to be sorted with the most common letter in the first place    
*/
void HuffmanTree::bubble_sort() {
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

/**
 * STAMPADIZIONARIO: it prints the dictionary on the standard output, 
 * one element per line, format: (key : frequency)
*/
void HuffmanTree::stampa_dizionario(){
    dizionario* aux = dict_head;
    while(aux) {
        std::cout<<"( "<<aux->info.first<<" : "<<aux->info.second<<" )"<<std::endl;
        aux = aux->next;
    }
}

/**
 * CREATEBINARYNODE: creation of the monad "root, left, right", given the last two elements 
 * of the dictionary. 
 *      + rearrange dictionary
*/
void HuffmanTree::create_binary_node(dizionario* maggiore, dizionario* minore){
    /**
     * this function creates a binary tree with one root and two leaf nodes 
     * (which can be trees, too).
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

/**
 * CREATETREE: creation of the tree. it just calls create_binary_node
 * until every element of the dictionary is represented on the tree
*/
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

/**
 * STAMPAALBEROREC: for debugging, it prints on std output the binary tree structure
*/
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

/**
 * CREATESRINGTREE: recursive function which create the balanced parentheses string based 
 * on the binary tree structure
*/
void HuffmanTree::create_string_tree(HuffmanTree::albero nodo, std::ostream& output){
    if (nodo->destra == nullptr && nodo->sinistra == nullptr) {
        output<<"'"<<nodo->codifica<<"'" ;
    }
    else if (nodo->destra == nullptr && nodo->sinistra != nullptr) {
        create_string_tree(nodo->sinistra, output);
        output<<","<<"\'"<<nodo->codifica<<"'";
    }
    else if (nodo->sinistra == nullptr && nodo->destra != nullptr) {
        output<<"'"<<nodo->codifica<<"'"<<",";
        create_string_tree(nodo->destra, output);
    }
    else{
        output<<"(";
        create_string_tree(nodo->sinistra, output);
        output<<",";
        create_string_tree(nodo->destra, output);
        output<<")";
    }
}

/**
 * PARSENODE: in the decompression part, it parses a node (not the leaf)
 * to buid the binary tree data structure
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

/**
 * PARSELEAF: in the decompression part, it parses a leaf
 * to buid the binary tree data structure
*/
HuffmanTree::albero HuffmanTree::parse_leaf (std::istream& input){
    char c;
    c = input.get();
    albero this_node = new nodo;
    this_node->sinistra = this_node->destra = nullptr;
    this_node->codifica = c;

    if (input.peek() != '\'') std::cout<<"(apice chiusura) ah rega sto parsing che pigna in culo"<<std::endl;

    return this_node;
}

/**
 * GETTREEFROMENCODEDSTREAM: in the decompression phase, parsing the balanced parentheses to
 * build the binary tree
*/
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