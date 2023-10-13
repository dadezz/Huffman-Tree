#include "Huffman.hpp"


#if PROVE
int main(int argc, char* argv[]) {
    
    if (argc != 3 || (argv[1] != "-e" && argv[1] != "-d")) {
        // I am expecting 2 arguments: -e/-d (encode/decode) andinput file
        std::cerr << "Usage: " << argv[0] << " -e input_file.txt (for compression)" << std::endl;
        std::cerr << "       " << argv[0] << " -d input_file.huf (for decompression)" << std::endl;
        return 1; // Invalid usage, exit with an error code
    }

    // open input file
    std::string input_file = argv[2];
    std::ifstream infile(input_file, std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }
    
    // Generate the output file name based on the input file
    std::string output_file_name;

    HuffmanTree a;
    
    if (argv[1] == "-e") {

        // checks if the extension exist; removes it and adds ".huf" as output filename
        size_t extension_pos = input_file.find_last_of(".");
        if (extension_pos != std::string::npos) {
            output_file_name = input_file.substr(0, extension_pos) + ".huf";
        } else {
            // if file passed by input doesn't have any extension
            output_file_name = input_file + ".huf";
        }
        std::ofstream outfile(output_file_name, std::ios::binary);

        // code for decompression
        a.compress(infile, outfile);
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

    return 0;
}
#endif