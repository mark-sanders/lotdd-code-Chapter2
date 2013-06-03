#include <string>
#include <unordered_map>

#include "gmock/gmock.h"    

using ::testing::Eq;
using ::testing::Test;



class Soundex {

public:
    std::string encode(const std::string& word) const {
        return zeroPad(head(word) + encodedDigits(word));
    }

private:
    static const unsigned int MaxCodeLength { 4 };
    
    const std::unordered_map<char, std::string> encodings {
        { 'b', "1" },
        { 'c', "2" },
        { 'd', "3" },
    };
    
    std::string head(const std::string& word) const {
        return word.substr(0, 1);
    };

    std::string encodedDigit(char letter) const {
    	return encodings.find(letter)->second;
    };

    std::string encodedDigits(const std::string& word) const {
        if (word.length() > 1) {
            return encodedDigit(word[1]);
        }
        else {
            return "";
        }
    };

    std::string zeroPad(const std::string& code) const {
        auto zerosNeeded = MaxCodeLength - code.length();
        return code + std::string(zerosNeeded, '0');
    }
};


class SoundexEncoding : public Test {
public:
    Soundex soundex;
};

TEST_F(SoundexEncoding, RetainsSoleLetterOfOneLetterWord) { 
    ASSERT_THAT(soundex.encode("A"), Eq("A000"));
}

TEST_F(SoundexEncoding, PadsWithZerosToEnsureThreeDigits) {
    ASSERT_THAT(soundex.encode("I"), Eq("I000"));
}

TEST_F(SoundexEncoding, ReplacesConsonantsWithAppropriateDigits) { 
    ASSERT_THAT(soundex.encode("Ab"), Eq("A100"));
    ASSERT_THAT(soundex.encode("Ac"), Eq("A200"));
    ASSERT_THAT(soundex.encode("Ad"), Eq("A300"));
}

