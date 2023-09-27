#include "Huffman.hpp"

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

std::istream& operator>>(std::istream& lhs, HuffmanTree& rhs){
    char c;
    do {
        c = lhs.get();
        if (c != -1) rhs.insert(string{c});
    } while (c != -1);
    rhs.bubble_sort();
    return lhs;
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
    albero mag /*sinistra*/, min /*destra*/;

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

    delete minore;
    
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

int main(){
    HuffmanTree prova;
    //HuffmanTree prova2;
    string s = "qwwwwwtttsssssss";
    std::istringstream st(s);
    ///std::ifstream file("prova.txt");
    st>>prova;
    //file>>prova2;
    prova.stampa_dizionario();
    //std::cout<<" - - - - - - - - - - - "<<std::endl;
   // prova2.stampa_dizionario();
    std::cout<<" - - - - - - - - - - - "<<std::endl;
    std::cout<<" - - - - - - - - - - - "<<std::endl;
    std::cout<<" - - - - - - - - - - - "<<std::endl;
    std::cout<<" PROVA ALBERO "<<std::endl;
    prova.create_tree();
    prova.stampa_albero();
  

    

}
