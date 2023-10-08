#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <locale>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

struct WordStruct {
    string word;
    float value;
};

// Function calls

string extractFileName(const string& filePath);
string extractReviewNumber(const string& filePath);
bool containsInvalidCharacters(const string& review);

int readSentimentWordList(vector<WordStruct>& wordList, const string& filePath);
int readFile(vector<string>& reviews, const string& fileName);

string formatReview(const string& review);
bool binarySearch(const vector<WordStruct>& wordList, const string& targetWord, float& value);
float calculateSentiment(const string& review, const vector<WordStruct>& wordList);
void generateReviewsPart1(const vector<WordStruct>& wordList);

string getRandomPositiveWord(const vector<WordStruct>& wordList);
string getRandomNegativeWord(const vector<WordStruct>& wordList);

void processReviews(const vector<string>& reviews, const vector<WordStruct>& wordList, const string& fileName);