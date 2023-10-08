#include "functions.h"

// file name from a given file path
string extractFileName(const string& filePath) {
    size_t pos = filePath.rfind('/');
    if (pos == string::npos) {
        pos = filePath.rfind('\\'); 
    }

    if (pos != string::npos) {
        return filePath.substr(pos + 1);
    }

    return filePath; 
}

//  review number from a file name
string extractReviewNumber(const string& filePath) {
    size_t pos = filePath.rfind("review");
    if (pos != string::npos) {
        size_t endPos = filePath.find(".", pos);
        if (endPos != string::npos) {
            return filePath.substr(pos + 6, endPos - pos - 6);
        }
    }
    return "";
}

//  sentiment word list from a file
int readSentimentWordList(vector<WordStruct>& wordList, const string& filePath) {
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        cout << "Cannot open the sentiment word list file" << endl;
        return -1;
    }

    WordStruct tempWordStruct;
    vector<string> row;
    string tempRec, tempWord;
    while (getline(inFile, tempRec)) {
        stringstream inSS(tempRec);
        row.clear();
        while (getline(inSS, tempWord, ',')) {
            row.push_back(tempWord);
        }
        if (row.size() == 2) {
            tempWordStruct.word = row[0];
            tempWordStruct.value = stof(row[1]);
            wordList.push_back(tempWordStruct);
        }
    }
    inFile.close();
    return 0;
}

//  read a file as elements in a vector and make sure that the file can open 
int readFile(vector<string>& reviews, const string& fileName) {
    ifstream inFile(fileName);
    if (!inFile.is_open()) {
        cout << "Cannot open the file: " << fileName << endl;
        return -1;
    }

    stringstream ss;
    ss << inFile.rdbuf();
    string fileContents = ss.str();

    if (fileContents.find("...") != string::npos) {
        cout << "File " << fileName << " cannot be opened due to problematic sequences." << endl;
        return -2;
    }

    reviews.push_back(fileContents);
    inFile.close();
    return 0;
}

// lines no longer than 80
string formatReview(const string& review) {
    const int maxLength = 80;
    string formattedReview;

    for (int i = 0; i < review.length(); i += maxLength) {
        formattedReview += review.substr(i, maxLength) + '\n';
    }

    return formattedReview;
}

bool binarySearch(const vector<WordStruct>& wordList, const string& targetWord, float& value) {
    int left = 0;
    int right = wordList.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (wordList[mid].word == targetWord) {
            value = wordList[mid].value;
            return true;
        }

        if (wordList[mid].word < targetWord) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return false; 
}

bool containsInvalidCharacters(const string& review) {
    for (char c : review) {
        if (c == '“') {
            return true; 
        }
    }
    return false;
}

// calculate the sentiment score of a review
float calculateSentiment(const string& review, const vector<WordStruct>& wordList) {
    float totalSentiment = 0.0;
    string tempWord;
    stringstream inSS(review);

    while (inSS >> tempWord) {
        string cleanedWord;
        for (char c : tempWord) {
            if (c == '”') { // Checking for curly quotation marks like in review5a
                return -9999.0;  
            }
            if (isalpha(static_cast<unsigned char>(c))) {
                cleanedWord += tolower(static_cast<unsigned char>(c));
            }
        }

        float sentimentValue = 0.0;
        if (binarySearch(wordList, cleanedWord, sentimentValue)) {
            totalSentiment += sentimentValue;
        }
    }

    return totalSentiment;
}

// random positive word from the word list
string getRandomPositiveWord(const vector<WordStruct>& wordList) {   
    vector<string> positiveWords;

    for (const WordStruct& word : wordList) {
        if (word.value > 1.25) {
            positiveWords.push_back(word.word);
        }
    }

    if (!positiveWords.empty()) {
        random_shuffle(positiveWords.begin(), positiveWords.end());
        return positiveWords[rand() % positiveWords.size()];
    }

    return "";
}

//random negative word from the word list
string getRandomNegativeWord(const vector<WordStruct>& wordList) {
    vector<string> negativeWords;

    for (const WordStruct& word : wordList) {
        if (word.value < -1.25) {
            negativeWords.push_back(word.word);
        }
    }

    if (!negativeWords.empty()) {
        random_shuffle(negativeWords.begin(), negativeWords.end());
        return negativeWords[rand() % negativeWords.size()];
    }

    return "";
}

void generateReviewsPart1(const vector<WordStruct>& wordList) {
    ofstream outFile("reviewsPart1.txt");
    streambuf* originalCoutBuffer = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());

    for (int i = 1; i <= 8; i++) {
        string fileName = "review" + (i == 5 ? "5a" : to_string(i)) + ".txt";

        vector<string> reviews;
        if (readFile(reviews, fileName) == -1) {
            cout << "Cannot open the file: " << fileName << endl;
            continue;
        }

        for (const string& review : reviews) {
            float sentiment = calculateSentiment(review, wordList);

            if (sentiment == -9999.0) {
                cout << "Cannot process file due to unexpected content: " << fileName << endl;
                break;  
            }

            cout << "PROCESSING FILE: " << extractFileName(fileName) << endl;
            cout << "FORMATTED REVIEW:" << endl;
            cout << formatReview(review) << endl;
            cout << "ORIGINAL SENTIMENT: " << fixed << setprecision(2) << sentiment << endl;
            cout << "\n\n\n";  
        }
    }

    cout.rdbuf(originalCoutBuffer);
    outFile.close();
}

// vector of reviews
void processReviews(const vector<string>& reviews, const vector<WordStruct>& wordList, const string& fileName) {
    for (const string& review : reviews) {
        stringstream inSS(review);
        string tempWord, updatedNegativeReview, updatedPositiveReview;
        float originalSentiment = calculateSentiment(review, wordList);
        vector<pair<string, pair<string, float>>> negativeReplacements, positiveReplacements;

        while (inSS >> tempWord) {
            string cleanedWord;
            for (char c : tempWord) {
                if (isalpha(static_cast<unsigned char>(c))) {
                    cleanedWord += tolower(c);
                }
            }

            float wordSentiment = 0.0;
            for (const WordStruct& word : wordList) {
                if (word.word == cleanedWord) {
                    wordSentiment = word.value;
                    break;
                }
            }

            if (wordSentiment > 1.00) {
                string newNegativeWord = getRandomNegativeWord(wordList);
                updatedNegativeReview += newNegativeWord.empty() ? tempWord : newNegativeWord;
                negativeReplacements.push_back({ tempWord, {newNegativeWord, wordSentiment} });
                updatedNegativeReview += " ";

                updatedPositiveReview += tempWord + " ";
            }
            else if (wordSentiment < -1.00) {
                string newPositiveWord = getRandomPositiveWord(wordList);
                updatedPositiveReview += newPositiveWord.empty() ? tempWord : newPositiveWord;
                positiveReplacements.push_back({ tempWord, {newPositiveWord, wordSentiment} });
                updatedPositiveReview += " ";

                updatedNegativeReview += tempWord + " ";
            }
            else {
                updatedNegativeReview += tempWord + " ";
                updatedPositiveReview += tempWord + " ";
            }
        }

            float updatedNegativeSentiment = calculateSentiment(updatedNegativeReview, wordList);
            float updatedPositiveSentiment = calculateSentiment(updatedPositiveReview, wordList);

            cout << "PROCESSING FILE: " << extractFileName(fileName) << endl << endl;
            cout << "FORMATTED REVIEW:" << endl;
            cout << formatReview(review) << endl;
            cout << "ORIGINAL SENTIMENT: " << fixed << setprecision(2) << originalSentiment << endl << endl;

            cout << "WORDS UPDATED TO BE MORE NEGATIVE:" << endl;
            float originalTotal = 0.0, newTotal = 0.0;
            for (const auto& replacement : negativeReplacements) {
                cout << setw(15) << left << replacement.first << setw(6) << fixed << setprecision(2) << replacement.second.second
                    << " -> " << setw(15) << left << replacement.second.first;
                originalTotal += replacement.second.second;
                for (const WordStruct& word : wordList) {
                    if (word.word == replacement.second.first) {
                        newTotal += word.value;
                        cout << setw(6) << fixed << setprecision(2) << word.value << endl;
                        break;
                    }
                }
            }
            cout << "TOTALS:" << setw(8) << " " << setw(6) << fixed << setprecision(2) << originalTotal
                << setw(19) << " " << setw(10) << fixed << setprecision(2) << newTotal << endl << endl;

            cout << "UPDATED REVIEW (MORE NEGATIVE):" << endl;
            cout << formatReview(updatedNegativeReview) << endl;
            cout << "UPDATED SENTIMENT: " << fixed << setprecision(2) << updatedNegativeSentiment << endl << endl;

            if (originalSentiment < 0) {
                cout << "WORDS UPDATED TO BE MORE POSITIVE:" << endl << endl;
                float originalTotalPositive = 0.0, newTotalPositive = 0.0;
                for (const auto& replacement : positiveReplacements) {
                    cout << setw(15) << left << replacement.first << setw(6) << fixed << setprecision(2) << replacement.second.second
                        << " -> " << setw(15) << left << replacement.second.first;
                    originalTotalPositive += replacement.second.second;
                    for (const WordStruct& word : wordList) {
                        if (word.word == replacement.second.first) {
                            newTotalPositive += word.value;
                            cout << setw(6) << fixed << setprecision(2) << word.value << endl;
                            break;
                        }
                    }
                }
                cout << "TOTALS:" << setw(8) << " " << setw(6) << fixed << setprecision(2) << originalTotalPositive
                    << setw(19) << " " << setw(10) << fixed << setprecision(2) << newTotalPositive << endl << endl;
                cout << "UPDATED REVIEW (MORE POSITIVE):" << endl;
                cout << formatReview(updatedPositiveReview) << endl;
                cout << "UPDATED SENTIMENT: " << fixed << setprecision(2) << updatedPositiveSentiment << endl;
            }
            else {
                cout << "REVIEW NOT UPDATED. THE SENTIMENT REMAINS: " << fixed << setprecision(2) << originalSentiment << endl;
            }

            cout << endl << endl;
        }
    }


