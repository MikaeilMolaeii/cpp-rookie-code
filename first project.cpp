#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

string toLowerCase(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + ('a' - 'A');
        }
    }
    return str;
}

string removePunctuation(string word) {
    string result = "";
    for (int i = 0; i < word.length(); i++) {
        if ((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z')) {
            result += word[i];
        }
    }
    return result;
}

string fixPunctuation(string sentence) {
    vector<string> sentences;
    string currentSentence = "";
    string result = "";

    for (char c : sentence) {
        if (c != '.' && c != '?' && c != '!') {
            currentSentence += c;
        } else {
            currentSentence += c;
            sentences.push_back(currentSentence);
            currentSentence = "";
        }
    }
    if (!currentSentence.empty()) {
        sentences.push_back(currentSentence);
    }

    for (string s : sentences) {
        string trimmed = "";
        bool spaceSeen = false;

        for (char ch : s) {
            if (ch != ' ' || !spaceSeen) {
                trimmed += ch;
                spaceSeen = (ch == ' ');
            }
        }

        while (!trimmed.empty() && trimmed.front() == ' ') {
            trimmed.erase(trimmed.begin());
        }
        while (!trimmed.empty() && trimmed.back() == ' ') {
            trimmed.pop_back();
        }
        
        string firstWord = "";
        stringstream ss(trimmed);
        ss >> firstWord;
        firstWord = toLowerCase(firstWord);

        bool isQuestion = firstWord == "who" || firstWord == "what" || firstWord == "when" ||
                  firstWord == "where" || firstWord == "why" || firstWord == "how" ||
                  firstWord == "do" || firstWord == "does" || firstWord == "did" ||
                  firstWord == "is" || firstWord == "are" || firstWord == "am";

        while (!trimmed.empty() && (trimmed.back() == '.' || trimmed.back() == '?' || trimmed.back() == '!')) {
            trimmed.pop_back();
        }

        if (isQuestion) {
            trimmed += '?';
        } else {
            trimmed += '.';
        }

        if (!result.empty()) {
            result += ' ';
        }
        result += trimmed;
    }

    return result;
}

string fixAuxiliaryVerbs(string sentence) {
    stringstream ss(sentence);
    string word;
    vector<string> words;
    vector<string> punctuations;

    while (ss >> word) {
        string punctuation = "";
        while (!word.empty() && ispunct(word.back())) {
            punctuation = word.back() + punctuation;
            word.pop_back();
        }
        words.push_back(word);
        punctuations.push_back(punctuation);
    }

    for (int i = 0; i < words.size(); i++) {
        string currentWord = toLowerCase(words[i]);
        string nextWord = (i + 1 < words.size()) ? toLowerCase(words[i + 1]) : "";
        string prevWord = (i > 0) ? toLowerCase(words[i - 1]) : "";

        if ((currentWord == "he" || currentWord == "she" || currentWord == "it") && nextWord == "have") {
            words[i + 1] = "has";
        }
        if ((currentWord == "they" || currentWord == "we" || currentWord == "you") && nextWord == "has") {
            words[i + 1] = "have";
        }
        if (currentWord == "i" && (nextWord == "is" || nextWord == "are")) {
            words[i + 1] = "am";
        }
        if (currentWord == "i" && nextWord == "does") {
            words[i + 1] = "do";
        }
        if (currentWord == "i" && nextWord == "doesn't") {
            words[i + 1] = "don't";
        }
        if ((currentWord == "they" || currentWord == "we" || currentWord == "you") && (prevWord == "is" || prevWord == "am")) {
            words[i - 1] = "are";
        }
        if ((currentWord == "he" || currentWord == "she" || currentWord == "it") && (prevWord == "are" || prevWord == "am")) {
            words[i - 1] = "is";
        }
        if (currentWord == "they" && nextWord == "am") {
            words[i + 1] = "are";
        }
        if (currentWord == "they" && nextWord == "is") {
            words[i + 1] = "are";
        }
        if (currentWord == "we" && nextWord == "is") {
            words[i + 1] = "are";
        }
        if ((currentWord == "he" || currentWord == "she" || currentWord == "it") && nextWord == "am") {
            words[i + 1] = "is";
        }
        if (prevWord == "is" && (currentWord == "they" || currentWord == "we" || currentWord == "you")) {
            words[i - 1] = "are";
        }
        if (prevWord == "are" && currentWord == "i") {
            words[i - 1] = "am";
        }
        if ((currentWord == "he" || currentWord == "she" || currentWord == "it") && nextWord == "do") {
            words[i + 1] = "does";
        }
        if ((currentWord == "they" || currentWord == "we" || currentWord == "you") && nextWord == "does") {
            words[i + 1] = "do";
        }
        if ((currentWord == "he" || currentWord == "she" || currentWord == "it") && nextWord == "don't") {
            words[i + 1] = "doesn't";
        }
        if ((currentWord == "they" || currentWord == "we" || currentWord == "you") && nextWord == "doesn't") {
            words[i + 1] = "don't";
        }
    }

    string correctedSentence = "";
    for (int i = 0; i < words.size(); i++) {
        correctedSentence += words[i] + punctuations[i] + " ";
    }

    if (!correctedSentence.empty() && correctedSentence.back() == ' ') {
        correctedSentence.pop_back();
    }

    return correctedSentence;
}

int countCharacter(string sentence, char c) {
    int count = 0;
    for (int i = 0; i < sentence.length(); i++) {
        if (tolower(sentence[i]) == tolower(c)) {
            count++;
        }
    }
    return count;
}

vector<pair<string, int>> mostFrequentWords(string sentence) {
    vector<string> words;
    vector<pair<string, int>> frequency;
    stringstream ss(sentence);
    string word;

    while (ss >> word) {
        word = toLowerCase(removePunctuation(word));

        bool isNumber = true;
        for (char c : word) {
            if (!isdigit(c)) {
                isNumber = false;
                break;
            }
        }

        if (!isNumber && !word.empty()) {
            bool found = false;
            for (int i = 0; i < frequency.size(); i++) {
                if (frequency[i].first == word) {
                    frequency[i].second++;
                    found = true;
                    break;
                }
            }
            if (!found) {
                frequency.push_back({word, 1});
            }
        }
    }

    for (int i = 0; i < frequency.size(); i++) {
        for (int j = i + 1; j < frequency.size(); j++) {
            if (frequency[j].second > frequency[i].second) {
                swap(frequency[i], frequency[j]);
            }
        }
    }
    return frequency;
}

pair<int, int> countWordsAndCharacters(string sentence) {
    int charCount = 0, wordCount = 0;
    string word;
    stringstream ss(sentence);

    while (ss >> word) {
        while (!word.empty() && ispunct(word.front())) word.erase(word.begin());
        while (!word.empty() && ispunct(word.back())) word.pop_back();

        if (!word.empty()) {
            wordCount++;
        }
    }

    for (char c : sentence) {
        if (!isspace(c)) {
            charCount++;
        }
    }

    return {wordCount, charCount};
}

int sumNumbers(string sentence) {
    int sum = 0;

    for (int i = 0; i < sentence.length(); i++) {
        if (sentence[i] >= '0' && sentence[i] <= '9') {
            sum += sentence[i] - '0';
        }
    }

    return sum;
}

void menu() {
    string sentence = "";

    while (true) {
        cout << "\n--- Menu ---\n";
        cout << "1. Enter a new sentence\n";
        cout << "2. Fix punctuation\n";
        cout << "3. Fix auxiliary verbs\n";
        cout << "4. Count a character\n";
        cout << "5. Find most frequent words\n";
        cout << "6. Count words and characters\n";
        cout << "7. Sum numbers in the sentence\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 8) {
            cout << "Exiting program...\n";
            break;
        }

        if (choice == 1) {
            cout << "Enter your sentence: ";
            getline(cin, sentence);
            continue;
        }

        if (sentence.empty()) {
            cout << "Please enter a sentence first (option 1).\n";
            continue;
        }

        switch (choice) {
            case 2:
                sentence = fixPunctuation(sentence);
                cout << "Fixed sentences: " << sentence << endl;
                break;

            case 3:
                sentence = fixAuxiliaryVerbs(sentence);
                cout << "Fixed auxiliary verbs: " << sentence << endl;
                break;

            case 4: {
                char c;
                cout << "Enter a character: ";
                cin >> c;
                cout << "Character '" << c << "' appears " << countCharacter(sentence, c) << " times.\n";
                break;
            }
            case 5: {
                auto words = mostFrequentWords(sentence);
                cout << "Most frequent words:\n";
                for (int i = 0; i < words.size() && i < 3; i++) {
                    cout << words[i].first << ": " << words[i].second << endl;
                }
                break;
            }
            case 6: {
                auto counts = countWordsAndCharacters(sentence);
                cout << "Words: " << counts.first << ", Characters: " << counts.second << endl;
                break;
            }

            case 7: {
                cout << "Sum of numbers in the sentence: " << sumNumbers(sentence) << endl;
                break;
            }

            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}

int main() {
    menu();
    return 0;
}
