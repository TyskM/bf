//
// Tyler Tucker
// 2022-02-22
// Brainfuck interpreter
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

const int  cellCount = 255;
const char valueIncrement   = '+';
const char valueDecrement   = '-';
const char pointerIncrement = '>';
const char pointerDecrement = '<';
const char loopStart        = '[';
const char loopEnd          = ']';
const char valueOutput      = '.';
const char valueInput       = ',';


size_t getMatchingBracePosition(std::string& str, size_t startPos)
{
    _ASSERT(str[startPos] == loopStart || str[startPos] == loopEnd);

    const char startBrace = str[startPos];
    size_t loopDepth = 0;

    if (startBrace == loopStart)
        for (size_t ip = startPos + 1; ip < str.size(); ++ip)
        {
            if (str[ip] == startBrace) loopDepth += 1;
            else if (str[ip] == loopEnd)
            {
                if (loopDepth > 0) loopDepth -= 1;
                else return ip;
            }
        }
    else if (startBrace == loopEnd)
        for (size_t ip = startPos - 1; ip != static_cast<unsigned>(-1); --ip)
        {
            if (str[ip] == startBrace) loopDepth += 1;
            else if (str[ip] == loopStart)
            {
                if (loopDepth > 0) loopDepth -= 1;
                else return ip;
            }
        }
}

void interpretBF(std::string str)
{
    std::vector<short> cells;
    cells.resize(cellCount);
    size_t currentCell = 0;

    for (size_t ip = 0; ip < str.size(); ip++)
    {
        //std::cout << std::endl << "Interpreting char " << str[ip] << " at position " << ip << std::endl;

        switch (str[ip])
        {
        case valueIncrement:
            cells[currentCell] += 1; break;

        case valueDecrement:
            cells[currentCell] -= 1; break;

        case pointerDecrement:
            currentCell -= 1;
            if (currentCell < 0) currentCell = cells.size() - 1;
            break;

        case pointerIncrement:
            currentCell += 1;
            if (currentCell >= cells.size()) currentCell = 0;
            break;

        case loopStart:
            if (cells[currentCell] == 0)
            { ip = getMatchingBracePosition(str, ip); }
            break;

        case loopEnd:
            ip = getMatchingBracePosition(str, ip) - 1;
            break;

        case valueInput:
            std::cin >> cells[currentCell];
            break;

        case valueOutput:
            std::cout << char(cells[currentCell]);
            break;

        default: break;
        }
    }
}

std::string readFile(const std::string& filePath)
{
    // http://insanecoding.blogspot.de/2011/11/how-to-read-in-file-in-c.html
    std::ifstream in(filePath, std::ios::in | std::ios::binary);
    if (!in) { std::cerr << "Failed to read file: " << filePath << std::endl; abort(); }
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    { std::cout << "Usage: bf.exe file.bf" << std::endl; return 1; }

    interpretBF(readFile(argv[1]));
}