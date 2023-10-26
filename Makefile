CXX = g++
CXXFLAGS = -std=c++11 -o3
SRC_DIR = src/
SOURCES = $(SRC_DIR)Huffman_masking.cpp $(SRC_DIR)Huffman_tree.cpp $(SRC_DIR)Huffman_core_logic.cpp $(SRC_DIR)Huffman_main.cpp
TARGET = HuffmanLauncher

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)