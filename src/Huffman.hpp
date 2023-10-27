#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <array>

#define BIT_IN_A_BYTE 8

using std::string;
using std::pair;

/**
 * Class of the HuffmanTree data structure: 
 * It contains:
 *  - A binary tree, for the Huffman core logic
 *  - A dictionary, based on a doubly linked list, to store the frequencies
 *  - Functions definitions
*/

class HuffmanTree{
    private:
        struct node { 
            string encoding;
            node* father;
            node* right_subtree;
            node* left_subtree;
            node(){
                encoding = string{};
                father = right_subtree = left_subtree = nullptr;
            }
            ~node() {
                if (right_subtree) delete right_subtree;
                if (left_subtree) delete left_subtree;
            }
        }; 

        typedef node* tree_t;
        tree_t head;

        struct dictionary { 
            pair<string, int> info;
            dictionary* next;
            dictionary* prev;
            tree_t corresponding_node;  //while creating the tree, I'll need it 
            dictionary() : info(string{}, 0), next(nullptr), prev(nullptr), corresponding_node(nullptr) {};
            ~dictionary() {
                if (next) delete next;
            }
        };

        dictionary* dict_head;
        dictionary* dict_tail;

        void insert(string);    // update frequency dictionary
        void bubble_sort();     // sort dictionary
        void create_binary_node(dictionary* greater, dictionary* smaller);  // generate a subtree
        void print_tree_rec(tree_t, int);    // [debugging - aux] print tree structure
        void navigate_tree(char next_char, char& byte, u_short& position, std::ostream& output);    // search the encoding path
        tree_t get_tree_from_encoded_stream(std::istream&); // parse balanced parenthesis
        tree_t parse_leaf (std::istream&); 
        tree_t parse_node (std::istream&);
        void create_string_tree(tree_t, std::ostream&); // put balanced parentheses on the output
        void set_bit_one(char& byte, u_short position); // bitwise operation
        void set_bit_zero(char& byte, u_short position);// bitwise operation
        void swap (dictionary* &, dictionary* &);


    public:
        HuffmanTree() : head(nullptr), dict_head(nullptr), dict_tail(nullptr) {};
        ~HuffmanTree(){
            if (dict_head) delete dict_head;
            if (head) delete head;
        };
        void print_dictionary(){ // [debugging] print tree structure
            dictionary* aux = dict_head;
            while(aux) {
                std::cout<<"( "<<aux->info.first<<" : "<<aux->info.second<<" )"<<std::endl;
                aux = aux->next;
            }
        }       
        tree_t get_tree_head(){
            return head;
        }

        tree_t create_tree();
        void print_tree(){   // [debugging] print tree structure
            print_tree_rec(head, 0);
        };

        void encode (std::istream&, std::ostream&); 
        void decode(std::istream& input, std::ostream&);

        void compress(std::istream&, std::ostream&);    
        void decompress(std::istream&, std::ostream&);
};
