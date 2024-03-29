#include "gmock/gmock.h"    

#include "Soundex.h"


using ::testing::Eq;
using ::testing::Test;
using ::testing::StartsWith;

TEST(SoundexEncoding, RetainsSoleLetterOfOneLetterWord) { 
    ASSERT_THAT(Soundex::encode("A"), Eq("A000"));
}

TEST(SoundexEncoding, PadsWithZerosToEnsureThreeDigits) {
    ASSERT_THAT(Soundex::encode("I"), Eq("I000"));
}

TEST(SoundexEncoding, EncodesTwoConsonants) {
    ASSERT_THAT(Soundex::encode("Ixl"), Eq("I240"));
}

TEST(SoundexEncoding, EncodesThreeConsonants) {
    ASSERT_THAT(Soundex::encode("Ixlr"), Eq("I246"));
}

TEST(SoundexEncoding, ReplacesLabialConsonantsWithAppropriateDigits) { 
    EXPECT_THAT(Soundex::encode("Ab"), Eq("A100"));
    EXPECT_THAT(Soundex::encode("Af"), Eq("A100"));
    EXPECT_THAT(Soundex::encode("Ap"), Eq("A100"));
    EXPECT_THAT(Soundex::encode("Av"), Eq("A100"));
}

TEST(SoundexEncoding, ReplacesFricativeConsonantsWithAppropriateDigits) { 
    EXPECT_THAT(Soundex::encode("Ac"), Eq("A200"));
    EXPECT_THAT(Soundex::encode("Ag"), Eq("A200"));
    EXPECT_THAT(Soundex::encode("Aj"), Eq("A200"));
    EXPECT_THAT(Soundex::encode("Ak"), Eq("A200"));
    EXPECT_THAT(Soundex::encode("Aq"), Eq("A200"));
    EXPECT_THAT(Soundex::encode("As"), Eq("A200"));
    EXPECT_THAT(Soundex::encode("Ax"), Eq("A200"));
    EXPECT_THAT(Soundex::encode("Az"), Eq("A200"));
}

TEST(SoundexEncoding, ReplacesAffricativeConsonantsWithAppropriateDigits) { 
    EXPECT_THAT(Soundex::encode("Ad"), Eq("A300"));
    EXPECT_THAT(Soundex::encode("At"), Eq("A300"));
}

TEST(SoundexEncoding, ReplacesEllWithAppropriateDigits) { 
    EXPECT_THAT(Soundex::encode("Al"), Eq("A400"));
}

TEST(SoundexEncoding, ReplacesEmEnWithAppropriateDigits) { 
    EXPECT_THAT(Soundex::encode("Am"), Eq("A500"));
    EXPECT_THAT(Soundex::encode("An"), Eq("A500"));
}

TEST(SoundexEncoding, IgnoresNonAlphabetics) {
    ASSERT_THAT(Soundex::encode("A#"), Eq("A000"));
    ASSERT_THAT(Soundex::encode("Ca+e=ci$%od#'uhyl"), Eq("C234"));
}

TEST(SoundexEncoding, ExplicitNotEncodedFlag) {
    EXPECT_THAT(Soundex::isValidEncoding(Soundex::encodeLetter('#')), false);
    EXPECT_THAT(Soundex::isValidEncoding(Soundex::encodeLetter('A')), false);
    EXPECT_THAT(Soundex::isValidEncoding(Soundex::encodeLetter('X')), true);
}

TEST(SoundexEncoding, ReplacesMultipleConsontantsWithDigits) {
    ASSERT_THAT(Soundex::encode("Acdl"), Eq("A234"));
}

TEST(SoundexEncoding, ReplacesWithUpToThreeDigits) {
    ASSERT_THAT(Soundex::encode("Acdlcdl"), Eq("A234"));
}

TEST(SoundexEncoding, LimitsLengthToFourCharacters) {
    ASSERT_THAT(Soundex::encode("Dcdlb").length(), Eq(4u));
}

TEST(SoundexEncoding, IgnoresVowelLikeLetters) {
    ASSERT_THAT(Soundex::encode("Caecioduhyl"), Eq("C234"));
    ASSERT_THAT(Soundex::encode("CaAeEciIoOdUuhHYyl"), Eq("C234"));
}

TEST(SoundexEncoding, IgnoresVowelLikeLettersSimple) {
    ASSERT_THAT(Soundex::encode("Caaaaaaaaaaaaaaaacdl"), Eq("C234"));
}

TEST(SoundexEncoding, CombinesDuplicates) {
    EXPECT_THAT(Soundex::encode("llama"), Soundex::encode("lama"));
    EXPECT_THAT(Soundex::encode("lhama"), Soundex::encode("lama"));
    EXPECT_THAT(Soundex::encode("lamma"), Soundex::encode("lama"));
    EXPECT_THAT(Soundex::encode("lamna"), Soundex::encode("lama"));
}

TEST(SoundexEncoding, CombinesDuplicatesWithSameEncoding) {
    ASSERT_THAT(Soundex::encodeLetter('c'), Eq(Soundex::encodeLetter('k')));
    ASSERT_THAT(Soundex::encodeLetter('d'), Eq(Soundex::encodeLetter('t')));
    ASSERT_THAT(Soundex::encodeLetter('m'), Eq(Soundex::encodeLetter('n')));
    
    ASSERT_THAT(Soundex::encode("Rckdtmn"), Eq("R235"));
}

TEST(SoundexEncoding, IgnoresInitialLetterCase) {
    ASSERT_THAT(Soundex::encode("Dcdlb"), Soundex::encode("dcdlb"));
}

TEST(SoundexEncoding, IgnoresCaseWhenEncoding) {
    ASSERT_THAT(Soundex::encode("Dcdlb"), Soundex::encode("DCDLB"));
    ASSERT_THAT(Soundex::encode("dcdlb"), Soundex::encode("DCDLB"));
    ASSERT_THAT(Soundex::encode("dCdLb"), Soundex::encode("DcDlB"));
}

TEST(SoundexEncoding, DoesNotCombineInitialDuplicatesWhenSeparatedByVowels) {
    ASSERT_THAT(Soundex::encode("Cacdl"), Eq("C234"));
}

TEST(SoundexEncoding, DoesNotCombineDuplicatesWhenSeparatedByVowels) {
    ASSERT_THAT(Soundex::encode("Rcacdl"), Eq("R223"));
    ASSERT_THAT(Soundex::encode("Rracacad"), Eq("R223"));
}

TEST(SoundexEncoding, CombinesDuplicateInitialVowels) {
    ASSERT_THAT(Soundex::encode("Aerdman"), Eq(Soundex::encode("Ardman")));
    ASSERT_THAT(Soundex::encode("Aardman"), Eq(Soundex::encode("Ardman")));
}

TEST(SoundexEncoding, CombinesDuplicateCodesWhen2ndLetterDuplicates1st) {
    EXPECT_THAT(Soundex::encode("Cccddll"), Eq("C340"));
    EXPECT_THAT(Soundex::encode("Rrccddll"), Eq("R234"));
    EXPECT_THAT(Soundex::encode("Cccddll"), Eq("C340"));
}

TEST(SoundexEncoding, CombinesDuplicatesToEnd) {
    EXPECT_THAT(Soundex::encode("Cccc"), Eq("C000"));
    EXPECT_THAT(Soundex::encode("Aaaa"), Eq("A000"));
}


TEST(SoundexEncodingOther, RetainsSoleLetterOfOneLetterWord) {
   ASSERT_THAT(Soundex::encode("A"), Eq("A000")); 
}

TEST(SoundexEncodingOther, PadsWithZerosToEnsureThreeDigits) {
   ASSERT_THAT(Soundex::encode("I"), Eq("I000"));
}

TEST(SoundexEncodingOther, ReplacesConsonantsWithAppropriateDigits) {
   EXPECT_THAT(Soundex::encode("Ab"), Eq("A100"));
   EXPECT_THAT(Soundex::encode("Ac"), Eq("A200"));
   EXPECT_THAT(Soundex::encode("Ad"), Eq("A300"));
   EXPECT_THAT(Soundex::encode("Af"), Eq("A100"));
   EXPECT_THAT(Soundex::encode("Ag"), Eq("A200"));
   EXPECT_THAT(Soundex::encode("Aj"), Eq("A200"));
   EXPECT_THAT(Soundex::encode("Ak"), Eq("A200"));
   EXPECT_THAT(Soundex::encode("Al"), Eq("A400"));
   EXPECT_THAT(Soundex::encode("Am"), Eq("A500"));
   EXPECT_THAT(Soundex::encode("An"), Eq("A500"));
   EXPECT_THAT(Soundex::encode("Ap"), Eq("A100"));
   EXPECT_THAT(Soundex::encode("Aq"), Eq("A200"));
   EXPECT_THAT(Soundex::encode("Ar"), Eq("A600"));
   EXPECT_THAT(Soundex::encode("As"), Eq("A200"));
   EXPECT_THAT(Soundex::encode("At"), Eq("A300"));
   EXPECT_THAT(Soundex::encode("Av"), Eq("A100"));
   EXPECT_THAT(Soundex::encode("Ax"), Eq("A200"));
   EXPECT_THAT(Soundex::encode("Az"), Eq("A200"));
}

TEST(SoundexEncodingOther, ReplacesMultipleConsonantsWithDigits) {
   ASSERT_THAT(Soundex::encode("Acdl"), Eq("A234"));
}

TEST(SoundexEncodingOther, LimitsLengthToFourCharacters) {
   ASSERT_THAT(Soundex::encode("Dcdlb").length(), Eq(4u)); 
}

TEST(SoundexEncodingOther, IgnoresVowelLikeLetters) {
   ASSERT_THAT(Soundex::encode("BAaEeIiOoUuHhYycdl"), Eq("B234"));
}

TEST(SoundexEncodingOther, IgnoresNonAlphabetics) {
   ASSERT_THAT(Soundex::encode("F987654321%#.=+cdl"), Eq("F234"));
}

TEST(SoundexEncodingOther, CombinesDuplicateEncodings) {
   ASSERT_THAT(Soundex::encodeLetter('b'), Eq(Soundex::encodeLetter('f')));
   ASSERT_THAT(Soundex::encodeLetter('c'), Eq(Soundex::encodeLetter('g')));
   ASSERT_THAT(Soundex::encodeLetter('d'), Eq(Soundex::encodeLetter('t')));

   ASSERT_THAT(Soundex::encode("Abfcgdt"), Eq("A123"));
}

TEST(SoundexEncodingOther, UppercasesFirstLetter) {
   ASSERT_THAT(Soundex::encode("abcd"), StartsWith("A"));
}

TEST(SoundexEncodingOther, IgnoresCaseWhenEncodingConsonants) {
   ASSERT_THAT(Soundex::encode("BCDL"), Eq(Soundex::encode("Bcdl")));
}

TEST(SoundexEncodingOther, CombinesDuplicateCodesWhen2ndLetterDuplicates1st) {
   ASSERT_THAT(Soundex::encode("Bbcd"), Eq("B230"));
}

TEST(SoundexEncodingOther, DoesNotCombineDuplicateEncodingsSeparatedByVowels) {
   ASSERT_THAT(Soundex::encode("Jbob"), Eq("J110"));
   ASSERT_THAT(Soundex::encode("Jbaobab"), Eq("J111"));
}
