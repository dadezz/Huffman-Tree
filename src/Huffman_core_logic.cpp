#include "Huffman.hpp"

/*----------------------------------------------------------------------
                        FIRST PART: COMPRESSION
-----------------------------------------------------------------------*/

/**
 * COMPRESS: main logic of huffman compression algorithm
*/
void HuffmanTree::compress(std::istream& input, std::ostream& output){
    
    /*FIRST PART:
    Create frequency dictionary*/    
    char c;
    do {
        c = input.get();
        if (c != -1) insert(string{c});
    } while (c != -1);
    insert(string{'\0'}); //add delimiter for EOF
    bubble_sort();

    /*SECOND PART:
    from the dictionary, create the tree*/
    create_tree();

    /*THIRD PART:
    streaming input file, encoding it*/
    input.clear();
    input.seekg(0);
    encode(input, output);

    if (head) delete head; // free memory
}

/**
 * ENCODE: from the input stream:
 *  1.  prepends balanced parentheses on the output file
 *  2.  for every byte of input file calls navigate_tree()
*/
void HuffmanTree::encode (std::istream& input, std::ostream& output){

    output.clear();
    input.clear();

    create_string_tree(head, output);   //prepend balanced parentheses
    output.clear();

    u_short position = 0;   // bit of the byte. from 0 to 7, from most to least significant
    char byte;  // byte to be written
    char next_char = input.peek();
    
    while (next_char != -1) {   
        // for every char, navigate_tree() finds the path on the tree and sets the bits to be written
        input.clear();
        output.clear();
        next_char = input.get();
        if (next_char != -1) navigate_tree(next_char, byte, position, output);
    }

    next_char = '\0'; // added at the end of the file, it is needed for decompression
    navigate_tree(next_char, byte, position, output);

    if (position > 0) output<<byte; // if there is a byte partially written, it needs to be added to file output

}


enum char_belongs_to_string {
    CHAR_NOT_INT_STRING, 
    CHAR_MATCHES_STRING, 
    CHAR_IN_STRING
};
/**
 * auxiliary function to find char position on the tree
*/
char_belongs_to_string check_encoding_char(char c, string s){
    if (s.length() == 1) 
        return s[0] == c ? CHAR_MATCHES_STRING : CHAR_NOT_INT_STRING;
    bool found = 0;
    int i = 0;
    while (i<s.length() && !found){
        if (s[i] == c) 
            found = true;
        i++;
    }
    return found ? CHAR_IN_STRING : CHAR_NOT_INT_STRING;
}
/**
 *  1.  Binary search to locate the corresponding leaf of a char
 *  2.  for every step on the path, set a bit on the byte
 *  3.  if every bit is set, append the byte
 * 
 *  BIT = 0 IF LEFT, 1 IF RIGHT
*/
void HuffmanTree::navigate_tree(char next_char, char& byte, u_short& position, std::ostream& output){
    tree_t aux = head;
    bool found = false;
    while (!found) {

        /*
            FIRST STEP: BINARY SEARCH
        */
        char_belongs_to_string here = check_encoding_char(next_char, aux->left_subtree->encoding);
        // (I start with the left branch)
        if (here == CHAR_MATCHES_STRING || here == CHAR_IN_STRING) {
            // in this case, i know for sure that the char is on the left branch of the tree
            // So, i can write the bit to Zero
            this->set_bit_zero(byte, position);
            position = (position+1) % BIT_IN_A_BYTE;
            // and move on the left child nodeS
            aux = aux->left_subtree;
            // last thing is: if i am on a leaf, i have found the char
            if (here == CHAR_MATCHES_STRING) found = true;            
        }
        else {
            // in this case, I know for sure that the char is on the right branch of the tree
            // so, i can write the bit to One
            set_bit_one(byte, position);
            position = (position+1) % BIT_IN_A_BYTE;
            // i need to check if i am on the leaf or just on a node:
            // i call again the function, but for the right branch
            here = check_encoding_char(next_char, aux->right_subtree->encoding);
            aux = aux->right_subtree;
            // last thing is: if i am on a leaf, i have found the char
            if (here == CHAR_MATCHES_STRING) found = true;
        }

        // if i completed a byte, i have to append it to the output string and reset 
        // the bits that have to be written
        if (position == 0) {
            output<<byte;
            byte = 0;
        }
    }

}

/*----------------------------------------------------------------------
                    SECOND PART: DECOMPRESSION
-----------------------------------------------------------------------*/

/**
 * DECOMPRESS: main logic of huffman decompression algorithm
 * @param input: file_compressed.huf
 * @param output: destination file
 * 
*/
void HuffmanTree::decompress(std::istream& input, std::ostream& output){
    
    /*FIRST PART:
    Create tree*/
    head = get_tree_from_encoded_stream(input);

    /*SECOND PART:
    streaming input file, decoding it*/ 
    decode(input, output);
}


/**
 * for every byte of input, reads every bit and use them to search the
 * corresponding char on the huffman tree
*/
void HuffmanTree::decode(std::istream & input, std::ostream& output){

    char c = input.get();
    tree_t aux = head;
    char mask = (1<<7); // 0b10000000
    
    while(!input.eof()){
        // i read every byte
        
        for (u_short i = 0; i<BIT_IN_A_BYTE; i++){
            // for: every bit of the file (8 iteractions)

            aux = ((c & mask) == 0 ? aux->left_subtree : aux->right_subtree); 
            // path:
            // if the first bit is equal to 0, change the pointer to the tree on the left branch
            // else, on the right one
            
            c = c<<1; // shift bit (i'm alvways checking the first one)

            if (aux->encoding.length() == 1) {
                // coding lenght == 1 -> we're on a leaf
                // append char on the output and point to the root
                char tmp = aux->encoding[0];
                if (tmp == '\0') break;
                else output<<tmp;
                aux = head;
            }
        }
        c = input.get();
    }
}