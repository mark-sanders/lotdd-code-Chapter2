#include <string>
#include <unordered_map>

using ::std::toupper;
using ::std::tolower;

class Soundex {
private:
    static const std::unordered_map<char, char> encodings;
    static const char InvalidEncoding { '\0' };
    static const unsigned int MaxCodeLength { 4 };

public:
    std::string encode(const std::string& word) const {
    	std::string code;
    	
        encodeHead(code, word);
        encodeTail(code, tail(word));
    	
        return zeroPad(code);
    }

    char encodeLetter(char letter) const {
        auto it = encodings.find(tolower(letter));
        
        if (encodings.end() == it) {
            return InvalidEncoding;
        }
        
        return it->second;
    };
    
    static bool isValidEncoding(char letter) {
        return InvalidEncoding != letter;
    }
    
private:

    void encodeHead(
            std::string& code, 
            const std::string& word) const {
        code += toupper(head(word));
    }
    
    char head(const std::string& word) const {
        return word[0];
    };
    
    std::string zeroPad(std::string& code) const {
        auto zerosNeeded = MaxCodeLength - code.length();
        return code += std::string(zerosNeeded, '0');
    }
    
    std::string tail(const std::string& word) const {
        return word.substr(1);
    };

    void encodeTail(
            std::string& code,
            const std::string& tail) const {
            
        auto encodingToSkip = encodeLetter(code[0]);
        
        for (auto letter : tail) { 
        
    	    char nextEncoding = encodeLetter(letter);

            if (isValidEncoding(encodingToSkip)) {
                if (   isValidEncoding(nextEncoding) 
                    && encodingToSkip == nextEncoding) {
                    continue; // skip letter with same encoding as initial letter
                }
                else {
                    encodingToSkip = InvalidEncoding;
                }
            }
            
    	    
    	    if (    isValidEncoding(nextEncoding) 
    	        &&  nextEncoding != lastEncoding(code)) {
    	        code += nextEncoding;
    	        if (isComplete(code)) break;
    	    }
    	}
    };
    
    char lastEncoding(const std::string& code) const {
        if (0 == code.length()) return InvalidEncoding;
        return code[code.length() - 1];
    }

    bool isComplete(const std::string& code) const {
        return code.length() >= MaxCodeLength;
    }
};

static std::unordered_map<char, char> initial_encodings() {

    const std::string encodings [][2] {
        { "1", "bfpv" },
        { "2", "cgjkqsxz" },
        { "3", "dt" },
        { "4", "l" },
        { "5", "mn" },
        { "6", "r" },
     };

    auto result = std::unordered_map<char, char>();

    for (auto encoding : encodings) {
        auto value = encoding[0][0];
        auto letters = encoding[1];
        for (auto letter : letters) {
            result.emplace(letter, value);
        }
    }

    return result;
}

const std::unordered_map<char, char> Soundex::encodings = initial_encodings();


#include "gmock/gmock.h"    


using ::testing::Eq;
using ::testing::Test;

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

TEST_F(SoundexEncoding, EncodesTwoConsonants) {
    ASSERT_THAT(soundex.encode("Ixl"), Eq("I240"));
}

TEST_F(SoundexEncoding, EncodesThreeConsonants) {
    ASSERT_THAT(soundex.encode("Ixlr"), Eq("I246"));
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
    ASSERT_THAT(soundex.encode("Ca+e=ci$%od#'uhyl"), Eq("C234"));
}

TEST_F(SoundexEncoding, ExplicitNotEncodedFlag) {
    EXPECT_THAT(Soundex::isValidEncoding(soundex.encodeLetter('#')), false);
    EXPECT_THAT(Soundex::isValidEncoding(soundex.encodeLetter('A')), false);
    EXPECT_THAT(Soundex::isValidEncoding(soundex.encodeLetter('X')), true);
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

TEST_F(SoundexEncoding, IgnoresVowelLikeLetters) {
    ASSERT_THAT(soundex.encode("Caecioduhyl"), Eq("C234"));
    ASSERT_THAT(soundex.encode("CaAeEciIoOdUuhHYyl"), Eq("C234"));
}

TEST_F(SoundexEncoding, IgnoresVowelLikeLettersSimple) {
    ASSERT_THAT(soundex.encode("Caaaaaaaaaaaaaaaacdl"), Eq("C234"));
}

TEST_F(SoundexEncoding, CombinesDuplicates) {
    EXPECT_THAT(soundex.encode("llama"), soundex.encode("lama"));
    EXPECT_THAT(soundex.encode("lhama"), soundex.encode("lama"));
    EXPECT_THAT(soundex.encode("lamma"), soundex.encode("lama"));
    EXPECT_THAT(soundex.encode("lamna"), soundex.encode("lama"));
}

TEST_F(SoundexEncoding, CombinesDuplicatesWithSameEncoding) {
    ASSERT_THAT(soundex.encodeLetter('c'), Eq(soundex.encodeLetter('k')));
    ASSERT_THAT(soundex.encodeLetter('d'), Eq(soundex.encodeLetter('t')));
    ASSERT_THAT(soundex.encodeLetter('m'), Eq(soundex.encodeLetter('n')));
    
    ASSERT_THAT(soundex.encode("Rckdtmn"), Eq("R235"));
}

TEST_F(SoundexEncoding, IgnoresInitialLetterCase) {
    ASSERT_THAT(soundex.encode("Dcdlb"), soundex.encode("dcdlb"));
}

TEST_F(SoundexEncoding, IgnoresCaseWhenEncoding) {
    ASSERT_THAT(soundex.encode("Dcdlb"), soundex.encode("DCDLB"));
    ASSERT_THAT(soundex.encode("dcdlb"), soundex.encode("DCDLB"));
    ASSERT_THAT(soundex.encode("dCdLb"), soundex.encode("DcDlB"));
}

TEST_F(SoundexEncoding, DoesNotCombineInitialDuplicatesWhenSeparatedByVowels) {
    ASSERT_THAT(soundex.encode("Cacdl"), Eq("C234"));
}

TEST_F(SoundexEncoding, DoesNotCombineDuplicatesWhenSeparatedByVowels) {
    ASSERT_THAT(soundex.encode("Rcacdl"), Eq("R234"));
}

TEST_F(SoundexEncoding, CombinesDuplicateInitialVowels) {
    ASSERT_THAT(soundex.encode("Aerdman"), Eq(soundex.encode("Ardman")));
    ASSERT_THAT(soundex.encode("Aardman"), Eq(soundex.encode("Ardman")));
}

TEST_F(SoundexEncoding, CombinesDuplicateCodesWhen2ndLetterDuplicates1st) {
    EXPECT_THAT(soundex.encode("Cccddll"), Eq("C340"));
    EXPECT_THAT(soundex.encode("Rrccddll"), Eq("R234"));
    EXPECT_THAT(soundex.encode("Cccddll"), Eq("C340"));
}

