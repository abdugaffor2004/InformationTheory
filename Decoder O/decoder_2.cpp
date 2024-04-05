#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>
#include <vector>
#include <set>
#include <cmath>
#include <Windows.h>

// ANSI управляющие последовательности для цветов
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Функция для преобразования текста в двоичное представление
std::string textToBinary(const std::string& text) {
    std::string binary;
    for (char c : text) {
        binary += std::bitset<8>(c).to_string();
    }
    return binary;
}

// Функция для разделения двоичной строки на сегменты фиксированной длины
std::vector<std::string> splitBinary(const std::string& binary, int n) {
    std::vector<std::string> segments;
    for (size_t i = 0; i < binary.size(); i += n) {
        segments.push_back(binary.substr(i, n));
    }
    return segments;
}

// Функция для вычисления количества уникальных сегментов
int numberOfUniqueSegments(std::vector<std::string>& binarySegments) {
    std::set<std::string> Unique;
    for (std::string& segment : binarySegments) {
        Unique.insert(segment);
    }
    std::cout << "NumberOfUniqueElements: " << Unique.size() << std::endl;
    return (ceil(log2(Unique.size())));
}

int main() {
    // Пути к файлам
    setlocale(LC_ALL, "ru_RU.UTF-8");
    std::string inputPath = "input.txt";

    // Открываем файл с входными данными
    std::ifstream inputFile(inputPath);
    if (!inputFile) {
        std::cerr << "Error opening input.txt" << std::endl;
        return 1;
    }

    // Считываем данные из файла в строку
    std::string inputText;
    std::string line;
    while (std::getline(inputFile, line)) {
        inputText += line;
    }
    inputFile.close();
    std::cout << "Input data from the file " << inputPath << ": \n" << inputText << std::endl;

    // Преобразуем входные данные в двоичное представление
    std::string binaryText = textToBinary(inputText);
    std::cout << "Input data from the file " << inputPath << " in binary form: \n" << binaryText << std::endl;

    // Запрашиваем у пользователя новую длину битсета
    int n;
    std::cout << "Bits of entire alphabet: ";
    std::cin >> n;
    if (n <= 0) {
        std::cerr << "Cannot encode 0 or fewer bits." << std::endl;
        return 1;
    }

    // Разделяем двоичное представление на сегменты
    std::vector<std::string> binarySegments = splitBinary(binaryText, n);

    // Запрашиваем у пользователя длину сегмента
    int segmentLength;
    std::cout << "Bits of output alphabet: ";
    std::cin >> segmentLength;
    if (segmentLength <= 0 || segmentLength < numberOfUniqueSegments(binarySegments)) {
        std::cerr << "Привет мир" << std::endl;
        return 1;
    }


    // Вычисляем количество уникальных сегментов
    // numberOfUniqueSegments(binarySegments);

    // Инициализируем словарь кодов
    std::map<std::string, std::string> codeMap;
    std::string currentCode;
    std::set<std::string> codes;
    if(segmentLength == 1){
        currentCode = std::string(1, '0');
    }

    else{

        for (int i = 0; i < segmentLength; i++) {
        int r = std::rand() % 2;
        currentCode += char(r + '0');
        }
    } 

    int zeros = 0;

    // Выводим словарь кодов на экран
    std::cout << "Dictionary:\n";
    for (std::string& segment : binarySegments) {
        
        while (segment.size() < n) {
            segment += '0';
            zeros++;
        }
        if (codeMap.find(segment) == codeMap.end()) {
            codeMap[segment] = currentCode;
            codes.insert(currentCode);
            std::string newCurrentCode = "";

            if(segmentLength == 1){
                newCurrentCode = std::string(1, '1');
            }
            else{
                while (codes.find(newCurrentCode) != codes.end() || newCurrentCode == ""){
                newCurrentCode.clear();
                for (int i = 0; i < segmentLength; i++) {
                    int r = std::rand() % 2;
                    newCurrentCode += char(r+'0');

                }
                }
            }

            
        currentCode = newCurrentCode;
        }

        std::cout << ANSI_COLOR_GREEN << "Input:" << segment << '\t'  << ANSI_COLOR_YELLOW << "Output:" << codeMap[segment] << std::endl;
    }
    std::cout << ANSI_COLOR_RESET;


    std::string encodedText;
    for (const std::string& segment : binarySegments) {
        encodedText += codeMap[segment];
    }
    std::cout << ANSI_COLOR_RESET;

    // Запись закодированных данных в файл
    std::ofstream encodedFile("encoded.txt", std::ios::binary);
    int outputZeros = 0;
    if (encodedText.length() % 8 != 0) {
        for (size_t j = 0; encodedText.length() % 8 != 0; j++) {
            encodedText += '0';
            outputZeros++;
        }
    }
    std::cout << "\nEncoded data: " << std::endl << encodedText << std::endl;
    for (size_t i = 0; i < encodedText.length(); i += 8) {
        std::bitset<8> byte(encodedText.substr(i, 8));
        char byteChar = static_cast<char>(byte.to_ulong());
        encodedFile.write(&byteChar, 1);
    }
    encodedFile.close();

    // Открытие файла с закодированными данными для декодирования
    std::ifstream decodedInputFile("encoded.txt", std::ios::binary);
    if (!decodedInputFile) {
        std::cerr << "Error opening encoded.txt" << std::endl;
        return 1;
    }

    // Чтение и перевод закодированных данных в бинарную систему
    std::string decodedByteText;
    char byteChar;
    std::string binaryByte;
    std::cout << "\nReading data from the encoded file:\n";
    while (decodedInputFile.read(&byteChar, 1)) {
        binaryByte = std::bitset<8>(byteChar).to_string();
        std::cout << ANSI_COLOR_MAGENTA << std::bitset<8>(byteChar).to_string() << std::endl;
        decodedByteText += binaryByte;
    }
    std::cout << ANSI_COLOR_RESET;
    decodedInputFile.close();


    if (decodedByteText.empty()) {
        std::cerr << "No data to decode." << std::endl;
        return 1;
    }

    // Декодирование
    std::string decodedOutputText;
    if (outputZeros != 0) {
        decodedByteText.erase(decodedByteText.length() - outputZeros);
    }

    for (size_t i = 0; i < decodedByteText.length(); i += segmentLength) {
        for (auto pair : codeMap) {
            if (i + segmentLength <= decodedByteText.size()) {
                if (pair.second == decodedByteText.substr(i, segmentLength)) {
                    decodedOutputText += pair.first;
                    break;
                }
            }
        }
    }

    if (zeros != 0) {
        decodedOutputText.erase(decodedOutputText.length() - zeros);
    }


    std::vector<unsigned char> decodedBytes;
    std::cout << "\nDecoded binary sequence:\n" << decodedOutputText << std::endl;
    std::cout << ANSI_COLOR_RESET << std::endl;
    for (size_t i = 0; i < decodedOutputText.size(); i += 8) {
        std::string byteStr = decodedOutputText.substr(i, 8);
        unsigned char byte = static_cast<unsigned char>(std::bitset<8>(byteStr).to_ulong());
        decodedBytes.push_back(byte);
    }

    std::string decodedText(decodedBytes.begin(), decodedBytes.end());

    // Запись декодированных данных в файл
    std::ofstream decodedFile("decoded.txt");
    decodedFile << decodedText;
    std::cout << "\nDecoded text:\n" << ANSI_COLOR_GREEN << decodedText << std::endl;
    decodedFile.close();

    std::cout << "Encoding and decoding completed" << ANSI_COLOR_RESET << std::endl;

    return 0;
}
