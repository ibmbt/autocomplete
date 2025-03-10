#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;


int compareStrings(const char* str1, const char* str2) {
    return strcmp(str1, str2);
}

void copyString(char* dest, const char* src) {
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

char* allocateWord(const char* word) {
    int length = strlen(word) + 1;
    char* newWord = new char[length];
    copyString(newWord, word);
    return newWord;
}

void readWordsFromFile(const char* filename, char*** wordList, int& wordCount) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    wordCount = 0;
    char tempWord[100];

    while (file >> tempWord) {
        wordCount++;
    }
    file.clear();
    file.seekg(0);

    *wordList = new char* [wordCount];

    int index = 0;
    while (file >> tempWord) {
        (*wordList)[index] = allocateWord(tempWord);
        index++;
    }
    file.close();
}

void suggestWords(const char* input, char** wordList, int wordCount) {

    system("cls");
    cout << "Suggestions: " << endl;
    int suggestions = 0;

    for (int i = 0; i < wordCount; i++) {
        if (strncmp(wordList[i], input, strlen(input)) == 0) {
            cout << wordList[i] << endl;
            suggestions++;
        }

        if (suggestions >= 5) {
            break;
        }
    }

    if (suggestions == 0) {
        cout << "No suggestions found." << endl;
    }
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
        cout << "\nEnter a partial word (or 'exit' to quit): ";
        cin >> input;

        if (strcmp(input, "exit") == 0) {
            break;
        }

        suggestWords(input, wordList, wordCount);
    }

    cleanup(wordList, wordCount);

    return 0;
}
