#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <array>

using std::string;
using std::pair;

class HuffmanTree{
    private:
    
        struct nodo { 
            string codifica;
            nodo* padre;
            nodo* destra;
            nodo* sinistra;

            nodo(){
                codifica = string{};
                padre = destra = sinistra = nullptr;
            }
        }; 

        typedef nodo* albero;
        albero head;

        struct dizionario { 
            pair<char, int> info;
            dizionario* next;
            dizionario* prev;

            dizionario() : info('\0', 0), next(nullptr), prev(nullptr) {};
        };

        // Deallocazione ricorsiva memoria albero binario
        void destroyTree(nodo* node) {
            if (node == nullptr) {
                return;
            }
            destroyTree(node->sinistra);
            destroyTree(node->destra);
            delete node;
        }


        dizionario* dict_head;
        dizionario* dict_tail;

        void insert(char);
        void bubble_sort();

    public:

        HuffmanTree() : head(nullptr), dict_head(nullptr), dict_tail(nullptr) {};
        ~HuffmanTree(){
            while(dict_head) {
                dizionario* aux = dict_head;
                dict_head = dict_head->next;
                delete aux;
            }
            destroyTree(head);
        };
        friend std::istream& operator>>(std::istream& lhs, HuffmanTree& rhs);

        albero get_tree_head();
        void swap (dizionario* &, dizionario* &);
        void stampa_dizionario();

};

HuffmanTree::albero HuffmanTree::get_tree_head(){
    return head;
}

void HuffmanTree::insert(char x){
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
    pair<char, int> aux = a->info;
    a->info = b->info;
    b->info = aux;
}

void HuffmanTree::bubble_sort() {
    //i want the dictionary to be sorted with the most common letter in the first place    
    dizionario *aux1, *aux2;
    for (aux1 = dict_head; aux1 != nullptr; aux1 = aux1->next) {
        for (aux2 = dict_head; aux2 != nullptr; aux2 = aux2->next) {
            if (aux2->next != nullptr && aux2->info.second < aux2->next->info.second) {
                swap(aux2, aux2->next);
            }
        }
    }
}


std::istream& operator>>(std::istream& lhs, HuffmanTree& rhs){
    char c;
    do {
        c = lhs.get();
        if (c != -1) rhs.insert(c);
    } while (c != -1);
    //rhs.stampa_dizionario();
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

int main(){
    HuffmanTree prova;
    HuffmanTree prova2;
    string s = "qww wee";
    std::istringstream st(s);
    std::ifstream file("prova.txt");
    st>>prova;
    file>>prova2;
    prova.stampa_dizionario();
    std::cout<<" - - - - - - - - - - - "<<std::endl;
    prova2.stampa_dizionario();
}

