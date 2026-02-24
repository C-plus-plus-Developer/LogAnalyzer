#include <iostream>
#include <vector>
#include <string_view>

struct ErrorBlock {
    size_t fileOffset;
    std::string_view content; 
    std::string type; 
};

std::string_view getContext(std::string_view fileView, size_t pos, int linesBefore, int linesAfter);
