#include "gmock/gmock.h"    

using ::testing::Eq;
using ::testing::Test;



class Soundex {

public:
    std::string encode(const std::string& word) const {
        auto encoded = word.substr(0, 1);
        
        if (word.length() > 1) {
            encoded += "1";
        }
        
        return zeroPad(encoded);
    }

private:
    std::string zeroPad(const std::string& code) const {
        auto zerosNeeded = 4 - code.length();
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
}

