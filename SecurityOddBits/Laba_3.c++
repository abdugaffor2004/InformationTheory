#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <set>
#include <cmath>
#include <sstream>

using namespace std;

int controlBits = 0;

// Управляющие последовательности ANSI для цветов
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

std::string textToBinary(const std::string& text) {
    std::string binary;
    for (char c : text) {
        binary += std::bitset<8>(c).to_string();
    }
    return binary;
}
std::string binaryToText(const std::string& binary)
{
    std::string text;
    for (int i = 0; i < binary.length(); i += 8) {
        text += static_cast<char>(std::bitset<8>(binary.substr(i, 8)).to_ulong());
    }
    return text;
}

std::vector<std::string> splitBinary(const std::string& binary, int n) {
    std::vector<std::string> segments;
    for (size_t i = 0; i < binary.size(); i += n) {
        segments.push_back(binary.substr(i, n));
    }
    return segments;
}

string readBinaryFile(const string& filename = "input.txt") {
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        cerr << "Ne udalos schitat dannie iz faila " << filename << endl;;
    }
    else
    {
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    return string(8, '0');
}

bool writeBinaryFile(const string& binaryString, const string& filename = "output.bin")
{
    ofstream output(filename, ios::binary);
    if (output.is_open())
    {
        for (int unsigned i = 0; i < binaryString.length(); i += 8) {
            char c = static_cast<char>(bitset<8>(binaryString.substr(i, 8)).to_ulong());
            output.write(&c, 1);
        }
        return true;
    }
    return false;

}

string hammingCoding(const std::vector<std::string>& segments)
{
    bool controlBit = false;
    string hammingCode;
    string a;
    for (const string& segment : segments)
    {
        string codingSegment = segment;
        bool bit = 0;
        for (auto c: codingSegment)
        {
            if (c == '1')
            {
                bit = !bit;
            }
        }
        a += (bit ? '1' : '0');
        hammingCode += codingSegment;
    }

    hammingCode += a;
    return hammingCode;
}

std::string hammingDecoding(const std::vector<std::string>& segments)
{
    string hammingDecodetText;
    for (const string& word : segments) {
        string codeWord = word;
        bool bit = 0;
        bool preBit = (codeWord[codeWord.size()-1] == '0'? 0:1);
        codeWord.erase(codeWord.length() - 1);
        for (auto c : codeWord)
        {
            if (c == '1')
            {
                bit = !bit;
            }
        }
        
        if (preBit != bit)
        {
            cout << ANSI_COLOR_RED << codeWord << ANSI_COLOR_RESET << endl;
        }
        else
        {
            cout << codeWord << endl;
        }

        hammingDecodetText += codeWord;
    }
    return hammingDecodetText;
}
int main() {
    setlocale(0, "rus");

    std::string inputPath = "input.txt";
    std::string outputPath;
    

    std::string inputText = readBinaryFile(inputPath);

    std::cout << "Vkhodnie dannie iz faila " << inputPath << ": \n" << inputText << std::endl;

    std::string binaryText = textToBinary(inputText);

    std::cout << "Vkhodnie dannie iz faila " << inputPath << " v binarnom vvide: \n" << binaryText << std::endl;

    int n;
    std::cout << "Vvedite chislo bit dliya razbieniya na segmenti: ";
    std::cin >> n;
    if (n <= 0)
    {
        std::cerr << "Nevozmozhno zakodirovat po 0 i menshe bit!\n";
        return 1;
    }

    std::vector<std::string> binarySegments = splitBinary(binaryText, n);

    std::cout << "Binarnie segmenti: \n";
    cout << ANSI_COLOR_CYAN;
    for (auto segment : binarySegments)
    {
        cout << segment << endl;
    }
    cout << ANSI_COLOR_RESET << endl;
    string hammingCode = hammingCoding(binarySegments);

    int encodedBitsSplit = n + 1;


    std::vector<std::string> splittedHammingCode = splitBinary(hammingCode, encodedBitsSplit);
    cout << "Segmenti s kontrolnimi bitami (kontrolnie biti otmecheni zelenim):\n";
    for (const string& segment : splittedHammingCode)
    {
       
        for (size_t i = 0; i < segment.length(); i++)
        {
            if (i == segment.length()-1)
            {
                cout << ANSI_COLOR_GREEN;
            }
            
            cout << segment[i];
            cout << ANSI_COLOR_RESET;
        }
        cout << endl;
    }

    int outputZeros = 8 - hammingCode.length() % 8;
    cout << "Binarnaya stroka s kontrolnimi bitami:" << endl;
    if (outputZeros != 8){
        cout << "(Zholtim otmecheni neznachashie nuli)" << endl;
        string zeros(outputZeros, '0');
        cout << "-";
        for (int i = 0; i < hammingCode.length(); i += 8)
        {
            cout << hammingCode.substr(i, 8) << "-";
        }
        hammingCode += zeros;
        cout << ANSI_COLOR_YELLOW;
        cout << zeros;
        cout << ANSI_COLOR_RESET << "-\n\n";;

    }

    else {
        cout << "-";
        for (int i = 0; i < hammingCode.length(); i += 8)
        {
            cout << hammingCode.substr(i, 8) << "-";
        }
        cout << endl << endl;

    }


    cin.ignore();
    getline(cin, outputPath);
    if (outputPath.empty())
    {
        writeBinaryFile(hammingCode);
    }
    else
    {
        writeBinaryFile(hammingCode, outputPath);
    }
    cout << ANSI_COLOR_GREEN << "Fail sohranion" << ANSI_COLOR_RESET << endl;
    cout << "Izmenite binarnie dannie, dliya proverki rabotosposobnosti koda\n";
    system("pause");

    string newHammingCode = textToBinary(readBinaryFile((outputPath.empty() ? "output.bin" : outputPath)));

    if (outputZeros != 8)
        newHammingCode.erase(newHammingCode.end() - outputZeros, newHammingCode.end());
    std::vector<std::string> newSplittedHammingCode = splitBinary(newHammingCode, encodedBitsSplit);
    cout << "Segmenti s kontrolnimi bitami (kontrolnie biti otmecheni zelionim)\n(zholtim otmecheni izmenionnie biti):\n";


    string decoded = hammingDecoding(newSplittedHammingCode);
    // cout << "Декодированый текст:\n";
    // cout << decoded << endl;
    // cout << "Декодированый текст в char:\n";
    // cout << binaryToText(decoded) << endl;

    return 0;
}
