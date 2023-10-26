#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <array>

#define BIT_IN_A_BYTE 8

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
            ~nodo() {
                if (destra) delete destra;
                if (sinistra) delete sinistra;
            }
        }; 

        typedef nodo* albero;
        albero head;

        struct dizionario { 
            pair<string, int> info;
            dizionario* next;
            dizionario* prev;
            albero nodo_corrispondente;
            dizionario() : info(string{}, 0), next(nullptr), prev(nullptr), nodo_corrispondente(nullptr) {};
            ~dizionario() {
                if (next) delete next;
            }
        };

        dizionario* dict_head;
        dizionario* dict_tail;

        void insert(string);
        void bubble_sort();
        void create_binary_node(dizionario* maggiore, dizionario* minore);
        void stampa_albero_rec(albero, int);
        void navigate_tree(char next_char, char& byte, u_short& position, std::ostream& output);
        albero get_tree_from_encoded_stream(std::istream&);
        albero parse_leaf (std::istream&);
        albero parse_node (std::istream&);
        void create_string_tree(albero, std::ostream&);
        void set_bit_one(char& byte, u_short position);
        void set_bit_zero(char& byte, u_short position);
        void swap (dizionario* &, dizionario* &);


    public:
        HuffmanTree() : head(nullptr), dict_head(nullptr), dict_tail(nullptr) {};
        ~HuffmanTree(){
            if (dict_head) delete dict_head;
            if (head) delete head;
        };
        void stampa_dizionario();
        albero get_tree_head(){
            return head;
        }

        albero create_tree();
        void stampa_albero(){
            stampa_albero_rec(head, 0);
        };

        void encode (std::istream&, std::ostream&);
        void decode(std::istream& input, std::ostream&);

        void compress(std::istream&, std::ostream&);
        void decompress(std::istream&, std::ostream&);
};
