#include "Huffman.hpp"


/**
 * AUTHOR: dadezz
 * 
 * DESCRIPTION:
 * The Huffman encoding uses a prefix-free code that represents the most frequent characters with the shortest codes.
 * The algorithm works as follows:
 *
 * COMPRESSION:
 * - Create a dictionary associating each symbol with its frequency and sort it.
 * - Build a binary tree by repeatedly merging the two least frequent symbols in the dictionary.
 * - In the output file, prepend the tree as a balanced parentheses representation.
 * - For each character, traverse the tree and set the corresponding bits.
 *
 * DECOMPRESSION:
 * - Parse the balanced parentheses representation to reconstruct the tree.
 * - For each bit in the input, navigate the tree until a valid code (leaf) is found, then restart.
 */

bool fileExists(const std::string &filename) {
    std::ifstream file(filename);
    return file.good();
}
void print_help(string executable, bool complete){
    if (complete) {
        std::cout<<"Huffman-Tree compression"<<std::endl;
        std::cout<<"Simple method to compress your data"<<std::endl;
        std::cout<<std::endl;
    }
    std::cout<<"Usage: "<<executable<<" -e input_file.txt   (for compression)"<<std::endl;
    std::cout<<"       "<<executable<<" -d input_file.huf   (for decompression)"<<std::endl;
    std::cout<<"       "<<executable<<" --help              to show help message"<<std::endl;

}
int main(int argc, char* argv[]) {
    if (argc == 1){
        print_help(string{argv[0]}, false);
        return 1;
    }
    if (argc == 2){
        if (string(argv[1]) == "--help"){
            print_help(string{argv[0]}, true);
            return 0;
        }
        std::cout<<"Option "<<argv[1]<<" unknow."<<std::endl;
        std::cout<<std::endl;
        print_help(string{argv[0]}, false);
        return 1;
    }
    if (argc > 4) {
        std::cout<<"too many arguments!"<<std::endl;
        print_help(string{argv[0]}, false);
        return 1;
    }
    if (argc == 3 && (string(argv[1]) != "-e" && string(argv[1]) != "-d")) {
        // I am expecting 2 arguments: -e/-d (encode/decode) andinput file
        std::cout<<"Option "<<argv[1]<<" unknow."<<std::endl;
        std::cout<<std::endl;
        print_help(string{argv[0]}, false);
        return 1;   
    }
    //So, argc == 3 and option is valid
    
    // open input file
    std::string input_file = argv[2];
    std::ifstream infile(input_file, std::ios::binary);
    if (!infile.good()) {
        std::cerr << "Error: Unable to read input file." << std::endl;
        return 1;
    }

    // Creation of the output file name, checking if input is valid
    string output_file_name;
    if (string(argv[1]) == "-e") {
        // Check if the input file ends with .huf: argument invalid
        if (input_file.length() >= 4 && input_file.substr(input_file.length() - 4) == ".huf"){
            std::cout<<"Error: The option \"-e\" indicates that the file should be compressed."<<std::endl;
            std::cout<<"The argument cannot have the \".huf\" extension."<<std::endl;
            print_help(string{argv[0]}, false);
            return 1;
        }

        // checks if the extension exist; removes it and adds ".huf" as output filename
        size_t extension_pos = input_file.find_last_of(".");
        if (extension_pos != std::string::npos) {
            output_file_name = input_file.substr(0, extension_pos) + ".huf";
        } else {
            // if file passed by input doesn't have any extension
            output_file_name = input_file + ".huf";
        }
    }
    else {
        // Check if the input file ends with .huf, and remove it to generate the output file name
        if (input_file.length() >= 4 && input_file.substr(input_file.length() - 4) == ".huf") {
            output_file_name = input_file.substr(0, input_file.length() - 4);
        } else {
            std::cout<<"Error: Input file must have a .huf extension for decompression."<<std::endl;
            return 1;
        }
        if (fileExists(output_file_name)) {
            std::cout << "Warning: The output file already exists. Do you want to overwrite it? (Y/N): ";
            char overwriteChoice;
            std::cin >> overwriteChoice;
            if (tolower(overwriteChoice) != 'y') {
                std::cout << "Decompression aborted." << std::endl;
                return 1;
            }
        }
    }
    std::ofstream outfile(output_file_name, std::ios::binary);

    // Compression / decompression
    infile.clear();
    outfile.clear();
    HuffmanTree a;
    if (string(argv[1]) == "-e")
        a.compress(infile, outfile);
    else 
        a.decompress(infile, outfile);

    outfile.close();

    return 0;
}