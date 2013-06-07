#include <string>
#include <array>
#include <iostream>

#include "Soundex.h"

EncodingTable Soundex::initTable() {

    EncodingTable tab;

    tab.fill(InvalidEncoding);

    auto init = [&tab] (const std::string& letters, char value) {
        for (auto letter : letters) {
            tab[encodingTableIndex(letter)] = value;
        }
    };

    init("bfpv",     '1');
    init("cgjkqsxz", '2');
    init("dt",       '3');
    init("l",        '4');
    init("mn",       '5');
    init("r",        '6');

    return tab;
}


constexpr char Soundex::InvalidEncoding;
constexpr unsigned int Soundex::MaxCodeLength;

const EncodingTable Soundex::encodingTable = Soundex::initTable();
