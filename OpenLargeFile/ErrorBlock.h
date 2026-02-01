#include <iostream>
#include <vector>
#include <string_view>

// Структура для хранения найденной ошибки
struct ErrorBlock {
    size_t fileOffset; // Где начинается этот кусок в файле
    std::string_view content; // Сам текст (контекст)
    std::string type; // "ERROR" или "CRITICAL"
};

std::string_view getContext(std::string_view fileView, size_t pos, int linesBefore, int linesAfter);