#include <iostream>
#include <fstream>
#include <iomanip>

void convertToHexHeader(const std::string& inputFile, const std::string& outputFile, bool frag) {
   
    // Open the input file
    std::ifstream inputFileStream(inputFile, std::ios::binary);
    if (!inputFileStream.is_open()) {
        std::cerr << "Error opening input file: " << inputFile << std::endl;
        return;
    }

    // Open the output file
    std::ofstream outputFileStream(outputFile);
    if (!outputFileStream.is_open()) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return;
    }

    // Read the input file to determine the count of bytes
    inputFileStream.seekg(0, std::ios::end);
    int count = inputFileStream.tellg();
   
    inputFileStream.seekg(0, std::ios::beg);

    // Write header information to the output file
 //   outputFileStream << "#pragma once\n\n";
    if(frag)
        outputFileStream << "constexpr unsigned char shader_frag[" << count << "] = {\n";
    else
    outputFileStream << "constexpr unsigned char shader_vert[" << count << "] = {\n";
   
    // Write the count as a constexpr
    outputFileStream << "    // Length of the SPIR-V binary\n";
   // outputFileStream << "    " << count << ",\n";

    // Read the input file and write the hexadecimal values to the output file
    char byte;
    while (inputFileStream.read(&byte, 1)) {
        // Format the hexadecimal value
        outputFileStream << "    0x" << std::setw(2) << std::setfill('0') << std::hex << (int)(unsigned char)byte << ",";

        // Add a newline character every 8 values for readability
        if (inputFileStream.tellg() % 8 == 0) {
            outputFileStream << "\n";
        }
    }

    // Remove the trailing comma from the last line
    if (count % 8 != 0) {
        outputFileStream.seekp(-1, std::ios_base::end);
        outputFileStream << "\n";
    }

    // Write the closing brace for the array
    outputFileStream << "};\n";

    // Write the count as a constexpr
    
    if(frag)  outputFileStream << "constexpr int frag_length = " << std::dec<< count << ";\n";
    else
    outputFileStream << "constexpr int vert_length = " << std::dec << count << ";\n";

    // Close the files
    inputFileStream.close();
    outputFileStream.close();

    std::cout << "Conversion completed successfully.\n";
}

int main() {
    convertToHexHeader("shader.vert.spv", "vert.h",0);
    convertToHexHeader("shader.frag.spv", "frag.h", 1);
    return 0;
}
