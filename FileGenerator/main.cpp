#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

int main() {
    const std::string filename = "big_log.txt";
    const size_t LINE_COUNT = 10'000'000; // 10 миллионов строк (~500-800 МБ)
    
    std::cout << "Generating " << filename << "..." << std::endl;

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    // Рандомайзер для генерации разной длины строк
    std::vector<std::string> levels = {"INFO", "DEBUG", "TRACE", "WARN"};
    
    for (size_t i = 0; i < LINE_COUNT; ++i) {
        // 1. Формируем строку
        file << "[2023-10-27 10:00:" << (i % 60) << "] ";
        
        // 2. Вставляем ошибку раз в 500 строк
        if (i % 500 == 0) {
            file << "[ERROR] Critical failure at module X processing transaction #" << i;
        } else {
            // Обычная строка
            file << "[" << levels[i % 4] << "] Processed item number " << i 
                 << ". Everything is fine. User ID: " << (i * 123);
        }

        // 3. Конец строки
        file << "\n";
    }

    std::cout << "\nDone! Open this file in your viewer." << std::endl;
    return 0;
}