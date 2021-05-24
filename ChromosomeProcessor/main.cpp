#include <iostream>
#include <fstream> //for std::ifstream
#include <filesystem> 
#include <ctype.h> //for isspace()

long hashWord(char s[]);
int getWord(char c, bool validPrev); //returns -1 if EOF, 0 if complete word, and n if invalid character at the nth consecutive char
bool isValidDNA(char c); 
char* deHash(int n);

char dnaChars[]{ 'A', 'a', 'C', 'c', 'G', 'g', 'T', 't' };

std::vector<int>* wordArray[1048575];
std::ifstream inf;

int main(int argc, char* argv[])
{
    
    //std::cout << std::filesystem::current_path() << '\n';
    if (argc != 2) 
    {
        std::cerr << "ERROR! Please provide name of file!";
        return 1;
    }

    inf.open(argv[1]);
    
    if (!inf.is_open())
    {
        std::cerr << "Could not open file";
        return 1;
    }

    char currentChar{ '0' };//initialize currentChar to non-null value
    int currentIndex{}; //keep track of location in file
    bool validPrev{ false };

    while (inf.get(currentChar)) //get each character in file
    {
        
        if (isValidDNA(currentChar))
        {
            currentIndex = inf.tellg();
            int result{ getWord(currentChar, validPrev) };
            switch (result)
            {
            case -1: //EOF found
                break;
            case 0: //word is valid, so we set validPrev to true and don't move the "cursor"
                validPrev = true;
                break;
            default: //word was invalid, skip to after the invalid char
                inf.seekg(currentIndex + result);
                validPrev = false;
            }
            //currentIndex = inf.tellg();
        }
        else if (!isspace(currentChar)) //if we find an invalid char, set the word to invalid
        {
            validPrev = false;
        }
        
        
        
    }

    //print out every found word and occurences
    for (int i = 0; i < 1048575; i++)
    {
        int max = 0;
        if (wordArray[i])
        {
            std::cout << "Word: " << deHash(i) << " Occurs: ";
            for (int n : *wordArray[i])
            {
                std::cout << n << ' ';
            }
            
            std::cout << '\n';
        }
    
    }
    return 0;
}

int getWord(char c, bool validPrev) //returns -1 if EOF, 0 if complete word, and n if invalid character at the nth consecutive char
{
    static char tempWord[10];
    long hashNum = 0;
    int fileIndex = inf.tellg(); 
    
    if (validPrev)
    {
        for (int i{}; tempWord[i+1]; ++i)   //shift letters of array left and assign last index to new char
        {
            tempWord[i] = tempWord[i + 1];
        }
        tempWord[9] = c;
    }
    else
    {
        tempWord[0] = c;
        for (int i = 1; i < 10;)
        {
        
            if (!inf.get(tempWord[i]))
            {
                std::cout << "REACHED END OF FILE!!!\n";
                inf.close();
                return 0;
            }
            else if (isValidDNA(tempWord[i]))
            {
                ++i;
            }
            else if (!isspace(tempWord[i]))
            {  
                return i;
            }

        }

    }
    

    hashNum = hashWord(tempWord);


    
    if (wordArray[hashNum] == NULL)
    {
        std::vector<int>* p_vec = new std::vector<int>();
        p_vec->push_back(fileIndex);
        wordArray[hashNum] = p_vec;
    }
    else
    {
        wordArray[hashNum]->push_back(fileIndex);
    }

    return 0;
}

long hashWord(char s[])
{
    long hashNumber = 0;
    for (int i = 0; i < 10; i++)
    {
        switch (s[i])
        {
        case 'A':
        case 'a':
            hashNumber += (0 * pow(4, 9 - i));
            break;
        case 'C':
        case 'c':
            hashNumber += (1 * pow(4, 9 - i));
            break;
        case 'G':
        case 'g':
            hashNumber += (2 * pow(4, 9 - i));
            break;
        case 'T':
        case 't':
            hashNumber += (3 * pow(4, 9 - i));
            break;
        }
       
    }
    return hashNumber;
}

bool isValidDNA(char c)
{
    for (char dna : dnaChars)
    {
        if (dna == c)
            return true;
    }
    return false;
}

char* deHash(int n)
{
    static char nuc[10] = { 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A' };
    int rem;
    int  step = 0;
    char letter{};

    while (n != 0)
    {
        rem = n % 4;
        switch (rem)
        {
        case 0:
            letter = 'A';
            break;

        case 1:
            letter = 'C';
            break;

        case 2:
            letter = 'G';
            break;

        case 3:
            letter = 'T';
            break;
        }
        n = n / 4;
        nuc[9 - step] = letter;
        step++;
    }

    return nuc;
}
