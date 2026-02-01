#include "ErrorBlock.h"

std::string_view getContext(std::string_view fileView, size_t pos, int linesBefore, int linesAfter) {
    
    // 1. Ищем ГРАНИЦУ НАЧАЛА (идем назад)
    size_t start = pos;
    for (int i = 0; i < linesBefore; ++i) {
        // rfind ищет последнее вхождение (то есть ближайшее слева)
        size_t found = fileView.rfind('\n', start - 1);
        
        if (found == std::string_view::npos) {
            start = 0; // Дошли до начала файла
            break;
        }
        start = found;
    }
    // Сдвигаем на +1, чтобы не захватывать сам символ \n в начале
    if (start != 0) start++; 

    // 2. Ищем ГРАНИЦУ КОНЦА (идем вперед)
    size_t end = pos;
    for (int i = 0; i < linesAfter; ++i) {
        size_t found = fileView.find('\n', end + 1);
        
        if (found == std::string_view::npos) {
            end = fileView.size(); // Дошли до конца файла
            break;
        }
        end = found;
    }

    // 3. Возвращаем view на этот кусок
    return fileView.substr(start, end - start);
}