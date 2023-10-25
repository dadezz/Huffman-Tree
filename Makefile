CXX = g++
CXXFLAGS = -std=c++11 -O3
SOURCES = Huffman_masking.cpp Huffman_tree.cpp Huffman_core_logic.cpp Huffman_main.cpp
TARGET = HuffmanLauncher

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
