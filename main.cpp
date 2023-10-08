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
    if (readSentimentWordList(wordList, "C:/Users/abbyb/source/repos/base project2/sentiment.txt") == -1) {
        return 1;
    }

    vector<string> fileNames = {
        "C:/Users/abbyb/source/repos/base project2/review1.txt",
        "C:/Users/abbyb/source/repos/base project2/review2.txt",
        "C:/Users/abbyb/source/repos/base project2/review3.txt",
        "C:/Users/abbyb/source/repos/base project2/review4.txt",
        "C:/Users/abbyb/source/repos/base project2/review6.txt",
        "C:/Users/abbyb/source/repos/base project2/review7.txt",
        "C:/Users/abbyb/source/repos/base project2/review8.txt"
    };

    ofstream outFile1("C:/Users/abbyb/source/repos/base project2/reviewsPart1.txt");
    streambuf* originalCoutBuffer = cout.rdbuf();
    cout.rdbuf(outFile1.rdbuf());

    for (const string& fileName : fileNames) {
        vector<string> reviews;
        if (readFile(reviews, fileName) == -1) {
            continue;
        }

        if (containsInvalidCharacters(reviews[0])) {
            cout << "Cannot process file " << extractFileName(fileName) << " due to invalid characters." << endl;
            continue;  
        }
    }

    cout.rdbuf(originalCoutBuffer);
    outFile1.close();

    generateReviewsPart1(wordList); 

    ofstream outFile("C:/Users/abbyb/source/repos/base project2/reviewsPart2.txt");
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

    ifstream inFile("C:/Users/abbyb/source/repos/base project2/reviewsPart2.txt");
    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();

    return 0;
}
