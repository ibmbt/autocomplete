#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;


int distCalc(const char* s1, const char* s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    
    int** dp = new int* [len1 + 1];
    for (int i = 0; i <= len1; i++) {
        dp[i] = new int[len2 + 1];
    }

    
    for (int i = 0; i <= len1; i++) dp[i][0] = i;
    for (int j = 0; j <= len2; j++) dp[0][j] = j;

    
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i][j] = min({ dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost });
        }
    }

    
    int result = dp[len1][len2];

    
    for (int i = 0; i <= len1; i++) {
        delete[] dp[i];
    }
    delete[] dp;

    return result;
}


bool isWordInList(const char* word, char** wordList, int wordCount) {
    for (int i = 0; i < wordCount; i++) {
        if (strcmp(word, wordList[i]) == 0) {
            return true;
        }
    }
    return false;
}


void suggestCorrections(const char* input, char** wordList, int wordCount) {
    int maxDistance = 2;  
    int suggestions = 0;

    cout << "Did you mean:" << endl;

    for (int i = 0; i < wordCount; i++) {
        int dist = distCalc(input, wordList[i]);
        if (dist <= maxDistance) {
            cout << wordList[i] << endl;
            suggestions++;
            if (suggestions > 10) {
                return;
            }
        }
    }

    if (suggestions == 0) {
        cout << "No suggestions found." << endl;
    }
}


void readWordsFromFile(const char* filename, char*** wordList, int& wordCount) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    wordCount = 0;
    string tempWord;

    
    while (file >> tempWord) {
        wordCount++;
    }

    file.clear();
    file.seekg(0);

    *wordList = new char* [wordCount];
    int index = 0;

    
    while (file >> tempWord) {
        
        (*wordList)[index] = new char[tempWord.length() + 1]; 
        strcpy_s((*wordList)[index], tempWord.length() + 1, tempWord.c_str());
        index++;
    }

    file.close();
}


void cleanup(char** wordList, int wordCount) {
    for (int i = 0; i < wordCount; i++) {
        delete[] wordList[i];
    }
    delete[] wordList;
}

int main() {
    const char* filename = "words.txt";  
    char** wordList = nullptr;
    int wordCount = 0;

    readWordsFromFile(filename, &wordList, wordCount);

    char input[100];
    while (true) {
        cout << "\nEnter a word to check (or 'exit' to quit): ";
        cin >> input;

        if (strcmp(input, "exit") == 0) {
            break;
        }

        
        if (isWordInList(input, wordList, wordCount)) {
            cout << input << " is spelled correctly!" << endl;
        }
        else {
            cout << input << " is not found." << endl;
            suggestCorrections(input, wordList, wordCount);
        }
    }

    cleanup(wordList, wordCount);
    return 0;
}
