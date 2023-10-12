#include "Huffman.hpp"


#if PROVE
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " -e input_file.txt (for compression)" << std::endl;
        std::cerr << "       " << argv[0] << " -d input_file.huf (for decompression)" << std::endl;
        return 1; // Invalid usage, exit with an error code
    }

    std::string mode = argv[1];
    std::string input_file = argv[2];
    std::string output_file;

    if (mode == "-e") {
        // Compression mode
        std::ifstream infile(input_file, std::ios::binary);
        
        if (!infile.is_open()) {
            std::cerr << "Error: Unable to open input file." << std::endl;
            return 1;
        }

        // Generate the output file name based on the input file
        size_t extension_pos = input_file.find_last_of(".");
        if (extension_pos != std::string::npos) {
            output_file = input_file.substr(0, extension_pos) + ".huf";
        } else {
            output_file = input_file + ".huf";
        }

        std::ofstream outfile(output_file, std::ios::binary);

        // Call your compression function here
        // For example: Huffman::compress(infile, outfile);

    } else if (mode == "-d") {
        // Decompression mode
        std::ifstream infile(input_file, std::ios::binary);

        if (!infile.is_open()) {
            std::cerr << "Error: Unable to open input file." << std::endl;
            return 1;
        }

        // Check if the input file ends with .huf, and remove it to generate the output file name
        if (input_file.length() >= 4 && input_file.substr(input_file.length() - 4) == ".huf") {
            output_file = input_file.substr(0, input_file.length() - 4);
        } else {
            std::cerr << "Error: Input file must have a .huf extension for decompression." << std::endl;
            return 1;
        }

        std::ofstream outfile(output_file, std::ios::binary);

        // Call your decompression function here
        // For example: Huffman::decompress(infile, outfile);

    } else {
        std::cerr << "Invalid mode. Use -e for compression or -d for decompression." << std::endl;
        return 1; // Invalid mode, exit with an error code
    }

    return 0; // Successful execution
}
#endif