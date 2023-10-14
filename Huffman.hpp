#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <array>

#define PROVE 0

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
        inline void set_bit_one(char& byte, u_short position);
        inline void set_bit_zero(char& byte, u_short position);
        inline void navigate_tree(char next_char, char& byte, u_short position, std::ostream& output);
        albero get_tree_from_encoded_stream(std::istream&);
        inline albero parse_leaf (std::istream&);
        albero parse_node (std::istream&);
        string read_bit(std::istream&);




    public:
        string create_string_tree(albero);

        HuffmanTree() : head(nullptr), dict_head(nullptr), dict_tail(nullptr) {};
        ~HuffmanTree(){
            delete dict_head;
            delete head;
        };
        friend std::istream& operator>>(std::istream& lhs, HuffmanTree& rhs);
        void swap (dizionario* &, dizionario* &);
        void stampa_dizionario();
        albero get_tree_head(){
            return head;
        }

        albero create_tree();
        void stampa_albero(){
            stampa_albero_rec(head, 0);
        };

        void encode (std::istream&, std::ostream&);
        string decode(std::istream& input);

        void compress(std::istream&, std::ostream&);
        void decompress(std::istream&, std::ostream&);


};