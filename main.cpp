// Project UID db1f506d06d84ab787baf250c265e24e
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include "csvstream.h"
#include <cmath>

using namespace std;


class Jazzyfier {
    private: 

    // should keep track of the classifier parameters learned from the training data
    set<string> vocab;
    map<string, int> labelPosts;
    map<pair<string, string>, int> wLabelPosts; 
    string trainFile;
    string testFile;
    bool debug;
    int numPosts;



    public:

    Jazzyfier(string trFile, string teFile, bool omgItsABug) : 
    trainFile(trFile), testFile(teFile), debug(omgItsABug), numPosts(0) {}

    set<string> uniquaWords(string &s) {
        istringstream ss_input(s);
        set<string> waka;
        string placeHolder;
        while(ss_input >> placeHolder) {
            waka.insert(placeHolder);
        }
        return waka;
    }

    void labelCounter(string s) {
        map<string, int>::iterator i = labelPosts.find(s);
        if(i == labelPosts.end()) {
            labelPosts.insert({s, 1});
        }
        else {
            i->second++;
        }
    }

    void wLabelPostsFiller(set<string> uniqueWords, string s) {
        for(auto &word : uniqueWords) {
            pair<string, string> stringy(s, word);
            map<pair<string, string>, int>::iterator i = wLabelPosts.find(stringy);
            vocabulary(word);
            if(i == wLabelPosts.end()) {
                wLabelPosts.insert({stringy, 1});
            }
            else {
                i->second++;
            }
        }
    } 

    void vocabulary(string s) {
        if(vocab.find(s) == vocab.end()) {
            vocab.insert(s);
        }
    }

    int howManyPostsWithDatWord(string word) {
        int lala = 0;
        for(auto &label : labelPosts) {
            map<pair<string, string>, int>::iterator i = 
            wLabelPosts.find({label.first, word});
            if(i != wLabelPosts.end()) {
                lala += i->second;
            }
        }
        return lala;
    }     

    int howManyPostsWithDatLabel(string label) {
        int lala = 0;
        for(auto &l : labelPosts) {
            if(l.first == label) {
                lala = l.second;
            }
        }
        return lala;
    }

    double logPrior(string label) {
        int postsWithDatLabel = howManyPostsWithDatLabel(label);
        double logPriorProb = log(postsWithDatLabel / (double)numPosts);
        return logPriorProb;
    }

    double logLikelihood(string label, string word) {
        int postsWithDatLabel = howManyPostsWithDatLabel(label);
        double logLikelihoodProb = 0.0;
        map<pair<string, string>, int>::iterator i = wLabelPosts.find({label, word});

         if(vocab.find(word) == vocab.end()) {
            logLikelihoodProb = log(1 / (double)numPosts);
         }

         else if(i != wLabelPosts.end()) {
            logLikelihoodProb = log(i->second / (double)postsWithDatLabel);
         }

         else {
            logLikelihoodProb = log(howManyPostsWithDatWord(word) / (double)numPosts);
         }
         return logLikelihoodProb;
    }
    
    double logProbability(set<string> bwah, string labelDUPE) {
        double gaggle = logPrior(labelDUPE);
        for(auto &word : bwah) {
            gaggle += logLikelihood(labelDUPE, word);
        }
        return gaggle;
    }              

    // provide an interface that allows you to train the classifier 
    // and make predictions for new piazza posts
    void train() {
        csvstream trainFileIn(trainFile);
        map<string, string> imTheMap;

        if(debug == true) {
            cout << "training data:" << endl;
        }
        
        while(trainFileIn >> imTheMap) {
            if(debug == true) {
                cout << "  label = " << imTheMap["tag"] << ", " << "content = " << 
                imTheMap["content"] << endl;
            }
            labelCounter(imTheMap["tag"]);
            set<string> uniqueWords = uniquaWords(imTheMap["content"]);
            wLabelPostsFiller(uniqueWords, imTheMap["tag"]);
            numPosts++;
        }
        cout << "trained on " << numPosts << " examples" << endl;
        if(debug == true) {
            cout << "vocabulary size = " << vocab.size() << endl;
        }
        cout << endl;

        if(debug == true) {
            cout << "classes:" << endl;
            for(auto &label : labelPosts) {
                cout << "  " << label.first << ", " << label.second << " examples, " << 
                "log-prior = " << logPrior(label.first) << endl;

            }
            cout << "classifier parameters:" << endl;
            for(auto &label : wLabelPosts) {
                cout << "  " << label.first.first << ":" << label.first.second << 
                ", count = " << label.second  << ", log-likelihood = " 
                << logLikelihood(label.first.first, label.first.second) << endl;
            }
            cout << endl;
        }   
    }

    pair<string,double> prediction(set<string> content) {
        string stringHolder;
        double doubleHolder = -1000000000;

        for(auto &label : labelPosts) {
            double logProb = logProbability(content, label.first);
            if(logProb > doubleHolder) {
                doubleHolder = logProb;
                stringHolder = label.first;
            }
        }

        return {stringHolder, doubleHolder};
    }

    void test() {
        csvstream testFileIn(testFile);
        map<string, string> imTheMap;
        int rightOnes = 0;
        int testPosts = 0;

        cout << "test data:" << endl;
        while(testFileIn >> imTheMap) {
            set<string> content = uniquaWords(imTheMap["content"]);
            pair<string, double> predict = prediction(content);

            cout << "  correct = " << imTheMap["tag"] <<  ", predicted = " 
            << predict.first << ", log-probability score = " << predict.second << endl;
            cout << "  content = " << imTheMap["content"] << endl << endl;
            testPosts++;

            if(predict.first == imTheMap["tag"]) {
                rightOnes++;
            }
        }

        cout << "performance: " << rightOnes << " / " << testPosts 
        << " posts predicted correctly" << endl;
    }
};

int main(int argc, char* argv[]) {
    cout.precision(3);
    if(argc != 3 && argc != 4) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 3;
    }
    if(argc == 4 && strcmp(argv[3], "--debug") != 0) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 4;
    }
    string testFile = argv[2];
    string trainFile = argv[1];

    ifstream yunder(trainFile);
    if(!yunder.is_open()) {
        cout << "Error opening file: " << trainFile << endl;
        return 5;
    }
    yunder.close();
    ifstream blunder(testFile);
    if(!blunder.is_open()) {
        cout << "Error opening file: " << testFile << endl;
        return 6;
    }
    blunder.close();

    bool inDebugMode = false;
    if(argc == 4 && strcmp(argv[3], "--debug") == 0) {
        inDebugMode = true;
    }

    Jazzyfier jazzybob(trainFile, testFile, inDebugMode);
    jazzybob.train();
    jazzybob.test();

}