#include "gmock/gmock.h"    

using ::testing::Eq;
using ::testing::Test;



class Soundex {

public:
    std::string encode(const std::string& word) const {
        return zeroPad(word);
    }

private:
    std::string zeroPad(const std::string& word) const {
        return word + "000";
    }
};


class SoundexEncoding : public Test {
public:
    Soundex soundex;
};

TEST_F(SoundexEncoding, RetainsSoleLetterOfOneLetterWord) { 
    auto encoded = soundex.encode("A");
    
    ASSERT_THAT(encoded, Eq("A000"));
}

TEST_F(SoundexEncoding, PadsWithZerosToEnsureThreeDigits) {
    auto encoded = soundex.encode("I");
    
    ASSERT_THAT(encoded, Eq("I000"));
}
