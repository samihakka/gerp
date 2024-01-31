/*
 * Sam Miller & Sami Hakkarainen
 * 12/06/2022
 * 
 * Purpose - Declare struct varaibles used to stare data
 */


#ifndef __HASHER_H__
#define __HASHER_H__


class hasher {


public: 

    struct container {
        
        std::string filePath;
        int lineNumber;
    };

    struct Node {

        std::string word;
        vector<container> instances;
    };

    struct sentenceObjects {

        std::string filePath;
        vector<std::string> all_lines;
    };


};

#endif