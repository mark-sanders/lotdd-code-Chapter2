#include <string>
#include <unordered_map>

#include "gmock/gmock.h"    

using ::testing::Eq;
using ::testing::Test;



class Soundex {
private:

    static std::unordered_map<char, std::string> initial_encodings() {
        const std::unordered_map<std::string, std::string> condensed_encodings {
            { "1", "bfpv" },
            { "2", "cgjkqsxz" },
            { "3", "dt" },
            { "4", "l" },
            { "5", "mn" },
            { "6", "r" },
         };
         
         auto result = std::unordered_map<char, std::string>();
         
         for (auto kv : condensed_encodings) {
             auto encoding = kv.first;
             auto letters = kv.second;
             for (auto c : letters) {
                 result.emplace(c, encoding);
             }
         }
         
         return result;
    }


public:
    std::string encode(const std::string& word) const {
        return zeroPad(head(word) + encodedDigits(tail(word)));
    }

private:
    static const unsigned int MaxCodeLength { 4 };
    
    std::string zeroPad(const std::string& code) const {
        auto zerosNeeded = MaxCodeLength - code.length();
        return code + std::string(zerosNeeded, '0');
    }
    
    std::string head(const std::string& word) const {
        return word.substr(0, 1);
    };

    std::string tail(const std::string& word) const {
        return word.substr(1);
    };

    std::string encodedDigits(const std::string& toEncode) const {
        
    	std::string encoding;
    
    	for (auto consonant : toEncode) {
    	    encoding += encodedDigit(consonant);
    	    
    	    if (MaxCodeLength - 1 == encoding.length()) break;
    	}
    	
    	return encoding;
    };

    std::string encodedDigit(char letter) const {
        const std::unordered_map<char, std::string> encodings = initial_encodings();

        auto it = encodings.find(letter);
        return (encodings.end() == it) ? "" : it->second;
    };
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

TEST_F(SoundexEncoding, ReplacesLabialConsonantsWithAppropriateDigits) { 
    EXPECT_THAT(soundex.encode("Ab"), Eq("A100"));
    EXPECT_THAT(soundex.encode("Af"), Eq("A100"));
    EXPECT_THAT(soundex.encode("Ap"), Eq("A100"));
    EXPECT_THAT(soundex.encode("Av"), Eq("A100"));
}

TEST_F(SoundexEncoding, ReplacesFricativeConsonantsWithAppropriateDigits) { 
    EXPECT_THAT(soundex.encode("Ac"), Eq("A200"));
    EXPECT_THAT(soundex.encode("Ag"), Eq("A200"));
    EXPECT_THAT(soundex.encode("Aj"), Eq("A200"));
    EXPECT_THAT(soundex.encode("Ak"), Eq("A200"));
    EXPECT_THAT(soundex.encode("Aq"), Eq("A200"));
    EXPECT_THAT(soundex.encode("As"), Eq("A200"));
    EXPECT_THAT(soundex.encode("Ax"), Eq("A200"));
    EXPECT_THAT(soundex.encode("Az"), Eq("A200"));
}

TEST_F(SoundexEncoding, ReplacesAffricativeConsonantsWithAppropriateDigits) { 
    EXPECT_THAT(soundex.encode("Ad"), Eq("A300"));
    EXPECT_THAT(soundex.encode("At"), Eq("A300"));
}

TEST_F(SoundexEncoding, ReplacesEllWithAppropriateDigits) { 
    EXPECT_THAT(soundex.encode("Al"), Eq("A400"));
}

TEST_F(SoundexEncoding, ReplacesEmEnWithAppropriateDigits) { 
    EXPECT_THAT(soundex.encode("Am"), Eq("A500"));
    EXPECT_THAT(soundex.encode("An"), Eq("A500"));
}

TEST_F(SoundexEncoding, IgnoresNonAlphabetics) {
    ASSERT_THAT(soundex.encode("A#"), Eq("A000"));
}

TEST_F(SoundexEncoding, ReplacesMultipleConsontantsWithDigits) {
    ASSERT_THAT(soundex.encode("Acdl"), Eq("A234"));
}

TEST_F(SoundexEncoding, ReplacesWithUpToThreeDigits) {
    ASSERT_THAT(soundex.encode("Acdlcdl"), Eq("A234"));
}

TEST_F(SoundexEncoding, LimitsLengthToFourCharacters) {
    ASSERT_THAT(soundex.encode("Dcdlb").length(), Eq(4u));
}

