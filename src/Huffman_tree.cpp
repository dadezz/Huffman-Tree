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

    bool inserted = false;
    dictionary* aux = dict_head;
    while(aux && !inserted){
        if (x == aux->info.first) {
            aux->info.second += 1;
            inserted = true;
        }
        aux = aux->next;
    }
    if (inserted == true) return;
    aux = new dictionary;
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
void HuffmanTree::swap(dictionary* & a, dictionary* & b){
    pair<string, int> aux = a->info;
    a->info = b->info;
    b->info = aux;

    tree_t aux2 = a->corresponding_node;
    a->corresponding_node = b->corresponding_node;
    b->corresponding_node = aux2;
}

/**
 * BUBBLESORT: i want the dictionary to be sorted with the most common letter in the first place    
*/
void HuffmanTree::bubble_sort() {
    dictionary *aux1, *aux2;
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
 * CREATEBINARYNODE: creation of the monad "root, left, right", given the last two elements 
 * of the dictionary. 
 *      + rearrange dictionary
*/
void HuffmanTree::create_binary_node(dictionary* greater, dictionary* smaller){
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
    
    tree_t root = new node;
    root->encoding = greater->info.first + smaller->info.first;
    tree_t mag /*left (bit 0) */, min /*right (bit 1)*/;

    /*
        creation bottom-up
    */

    if (greater->corresponding_node == nullptr) {
        mag = new node;
        mag->right_subtree = mag->left_subtree = nullptr;
        mag->encoding = greater->info.first;
    }
    else mag = greater->corresponding_node;
    if (smaller->corresponding_node == nullptr) {
        min = new node;
        min->right_subtree = min->left_subtree = nullptr;
        min->encoding = smaller->info.first;
    }
    else min = smaller->corresponding_node;
    mag->father = root;
    min->father = root;
    root->right_subtree = min;
    root->left_subtree = mag;

    /**
     * rearrange dictionary
    */
    greater->info.first = root->encoding;
    greater->info.second = greater->info.second + smaller->info.second;
    greater->corresponding_node = root;

    if (smaller->next ==nullptr) delete smaller;
    
    greater->next = nullptr;
    dict_tail = greater;

    this->bubble_sort();
}

/**
 * CREATETREE: creation of the tree. it just calls create_binary_node
 * until every element of the dictionary is represented on the tree
*/
HuffmanTree::tree_t HuffmanTree::create_tree(){
    if (dict_head == nullptr) return nullptr;
    while (dict_head->next != nullptr){
        create_binary_node(dict_tail->prev, dict_tail);
    }
    head = dict_head->corresponding_node;

    /*
        At this point, head of the tree is set, 
        i can delete the last node of the dictionary and set the pointers to nullptr
    */
    delete dict_head;
    dict_head = dict_tail = nullptr;
    
    return head;
}

/**
 * PRINT_TREE_REC: for debugging, it prints on std output the binary tree structure
*/
void HuffmanTree::print_tree_rec(tree_t tree_root, int space){
    if (tree_root == nullptr) return;

    //right branch
    print_tree_rec(tree_root->left_subtree, space+10);

    std::cout<<std::endl;
    for (int i=0; i<space; i++) std::cout<<" ";
    std::cout<<tree_root->encoding<<std::endl;

    //left branch
    print_tree_rec(tree_root->right_subtree, space+10);
}

/**
 * CREATESRINGTREE: recursive function which create the balanced parentheses string based 
 * on the binary tree structure
*/
void HuffmanTree::create_string_tree(HuffmanTree::tree_t n, std::ostream& output){
    if (n->right_subtree == nullptr && n->left_subtree == nullptr) {
        output<<"'"<<n->encoding<<"'" ;
    }
    else if (n->right_subtree == nullptr && n->left_subtree != nullptr) {
        create_string_tree(n->left_subtree, output);
        output<<","<<"\'"<<n->encoding<<"'";
    }
    else if (n->left_subtree == nullptr && n->right_subtree != nullptr) {
        output<<"'"<<n->encoding<<"'"<<",";
        create_string_tree(n->right_subtree, output);
    }
    else{
        output<<"(";
        create_string_tree(n->left_subtree, output);
        output<<",";
        create_string_tree(n->right_subtree, output);
        output<<")";
    }
}

/**
 * PARSENODE: in the decompression part, it parses a node (not the leaf)
 * to buid the binary tree data structure
*/
HuffmanTree::tree_t HuffmanTree::parse_node (std::istream& input){
    
    char c;    
    tree_t this_node = new node;
    this_node->left_subtree = get_tree_from_encoded_stream(input);
    this_node->encoding += this_node->left_subtree->encoding;
    this_node->left_subtree->father = this_node;

    c = input.get(); // consumes ','
    if (c != ',') std::cout<<"(virgola) ah rega sto parsing che pigna in culo"<<std::endl;

    this_node->right_subtree = get_tree_from_encoded_stream(input);
    this_node->encoding += this_node->right_subtree->encoding;
    this_node->right_subtree->father = this_node;

    if (input.peek() != ')') std::cerr<<"parsing error"<<std::endl;
    
    return this_node;
}

/**
 * PARSELEAF: in the decompression part, it parses a leaf
 * to buid the binary tree data structure
*/
HuffmanTree::tree_t HuffmanTree::parse_leaf (std::istream& input){
    char c;
    c = input.get();
    tree_t this_node = new node;
    this_node->left_subtree = this_node->right_subtree = nullptr;
    this_node->encoding = c;

    if (input.peek() != '\'') std::cerr<<"parsing error"<<std::endl;

    return this_node;
}

/**
 * GETTREEFROMENCODEDSTREAM: in the decompression phase, parsing the balanced parentheses to
 * build the binary tree
*/
HuffmanTree::tree_t HuffmanTree::get_tree_from_encoded_stream(std::istream& input){
    
    char c;
    tree_t this_node;
    
    if (input.eof()) {
        std::cerr<<"parsing error"<<std::endl;
        return nullptr;
    }

    c = input.get();
    
    if (c == '(')         
        this_node = parse_node(input); // consumes something like ( ... , ... )
    else if (c == '\'')
        this_node = parse_leaf(input); // consumes something like 'c'
    else std::cerr<<"parsing error"<<std::endl;
    
    c = input.get(); // consumes ')' or '''

    this_node->father = nullptr;

    return this_node;
}