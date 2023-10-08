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
#include "functions.h"

using namespace std;

//main calls all of the different functions from the other pages and puts it all together

int main() {
    srand(static_cast<unsigned int>(time(0))); // random number 

    vector<WordStruct> wordList;
    if (readSentimentWordList(wordList, "sentiment.txt") == -1) {
        return 1;
    }

    vector<string> fileNames = {
        "review1.txt",
        "review2.txt",
        "review3.txt",
        "review4.txt",
        "review5a.txt",
        "review6.txt",
        "review7.txt",
        "review8.txt"
    };

    ofstream outFile1("reviewsPart1.txt");
    streambuf* originalCoutBuffer = cout.rdbuf();
    cout.rdbuf(outFile1.rdbuf());

    for (const string& fileName : fileNames) {
        vector<string> reviews;
        int fileResult = readFile(reviews, fileName);
        if (fileResult == -1 || fileResult == -2) {
            continue; 
        }
        processReviews(reviews, wordList, fileName);
    }

    cout.rdbuf(originalCoutBuffer);
    outFile1.close();

    generateReviewsPart1(wordList); 

    ofstream outFile("reviewsPart2.txt");
    cout.rdbuf(outFile.rdbuf());

    for (const string& fileName : fileNames) {
        vector<string> reviews;
        if (readFile(reviews, fileName) == -1) {
            continue;
        }
        processReviews(reviews, wordList, fileName);
    }

    cout.rdbuf(originalCoutBuffer);
    outFile.close();

    ifstream inFile("reviewsPart2.txt");
    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();

    return 0;
}
