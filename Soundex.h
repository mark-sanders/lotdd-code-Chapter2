#include <string>
#include <array>

typedef std::array<char, 26> EncodingTable;


class Soundex {
private:
    static constexpr char InvalidEncoding { '\0' };

    static constexpr unsigned int MaxCodeLength { 4 };

    static const EncodingTable encodingTable;

public:
    static std::string encode(const std::string& word) {
        Soundex soundex(word);
        
        soundex.encodeInitial();
        
        soundex.encodeWordAfterInitial(soundex.wordAfterInitial());
        
        soundex.zeroPad();
        
        return soundex.code;
    }

    static char encodeLetter(char letter) {
        letter = std::tolower(letter);

        if (letter < 'a' || letter > 'z') {
            return InvalidEncoding;
        }
        
        return encodingTable[encodingTableIndex(letter)];
    };
    
    static bool isValidEncoding(char letter) {
        return InvalidEncoding != letter;
    }
    
private:
    const std::string& word;
    std::string code;
        
    Soundex(const std::string& wordToEncode) 
        : word(wordToEncode)
        {}
        
        
    void encodeInitial() {
        code += std::toupper(word[0]);
    }
    
    void zeroPad() {
        auto zerosNeeded = MaxCodeLength - code.length();
        code += std::string(zerosNeeded, '0');
    }
    
    std::string wordAfterInitial() const {

        auto wordAfterInitial = word.substr(1);

        auto encodingToSkip = encodeLetter(code[0]);

        if (isValidEncoding(encodingToSkip)) {
        
            for (unsigned int i = 0; i < wordAfterInitial.length(); i++) {
                auto nextEncoding = encodeLetter(wordAfterInitial[i]);
                
                if (encodingToSkip != nextEncoding) {
                    return wordAfterInitial.substr(i);
                }
            }
            
            return ""; // there were only repeats of initial letter
        } 
        else {
            return wordAfterInitial;
        }
    }
    
    
    void encodeWordAfterInitial(const std::string& wordAfterInitial) {
    
        auto lastEncoding = InvalidEncoding;
        
        for (auto letter : wordAfterInitial) {
            auto nextEncoding = encodeLetter(letter);
            
            if (isValidEncoding(nextEncoding) && nextEncoding != lastEncoding) {
                code += nextEncoding;
                if (isComplete()) break;
    	    }
    	    
    	    lastEncoding = nextEncoding;
    	}
    }

  
    bool isComplete() const {
        return code.length() >= MaxCodeLength;
    }
    
    static unsigned int encodingTableIndex(char letter) {
        return letter - 'a';
    }

    static EncodingTable initTable();
};
