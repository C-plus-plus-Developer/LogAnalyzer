#include "ErrorBlock.h"

std::string_view getContext(std::string_view fileView, size_t pos, int linesBefore, int linesAfter) {
    
    size_t start = pos;
    for (int i = 0; i < linesBefore; ++i) {
        size_t found = fileView.rfind('\n', start - 1);
        
        if (found == std::string_view::npos) {
            start = 0; 
            break;
        }
        start = found;
    }
    
    if (start != 0) start++; 

    size_t end = pos;
    for (int i = 0; i < linesAfter; ++i) {
        size_t found = fileView.find('\n', end + 1);
        
        if (found == std::string_view::npos) {
            end = fileView.size(); 
            break;
        }
        end = found;
    }

    return fileView.substr(start, end - start);
}
