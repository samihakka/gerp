/*
 * Sam Miller & Sami Hakkarainen
 * 12/1/2022
 * 
 * Purpose - The implemenation for a parser for key words that
 *           are inputted by the user.
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <functional> 

#include "FSTree.h"
#include "stringProcessing.h"
#include "hasher.h"


    //max number of keys: 171,200
    // 1.3 * maxWords = 222,560
    // prime number near this is 222561
const int HASH_VALUE = 262144;
hasher::Node dictionary[HASH_VALUE];
hasher::sentenceObjects sentences[HASH_VALUE];
hasher::Node insensitive_dictionary[HASH_VALUE];

// int PROBING_AMOUNT;

// void printDic() {
    
//     int count = 0;

//     for ( int i = 0; i < HASH_VALUE; i++) {

//         // printing in order to debug

//         if (insensitive_dictionary[i].word != "-1") {

//         count++;

        
//         }

//     }

//     // std::cout << "count: " << count << "\n";
//     // std::cout <<"probing amount: " << PROBING_AMOUNT << "\n";

// }

/*
 * name:      buildHashTable()
 * purpose:   Build an empty Hash Table
 * arguments: None
 * returns:   None
 * effects:   Initialize a Hash table with "-1" in every index
 */
void buildHashTable() {

    // fill hash table of this value with -1
    for (int i = 0; i < HASH_VALUE; i++) {
        
        dictionary[i].word = "-1";
        sentences[i].filePath = "-1";
        insensitive_dictionary[i].word = "-1";
    }

}

int linear_probing(int j, std::string key, hasher::container addInstance, hasher::Node ref_array[]) {

    bool still_going = true;

    while (j < HASH_VALUE and still_going) {
        if (ref_array[j].word == "-1" or ref_array[j].word == key) {
            
        ref_array[j].word = key;
        ref_array[j].instances.push_back(addInstance);

        still_going = false;

        }
        else {
            j++;
            // PROBING_AMOUNT++;
        }
    }

    return j;

}

/*
 * name:      hash_it()
 * purpose:   To organize each string into a bucket
 * arguments: (std::string key, std::string filename, int lineNumber,
               hasher::Node ref_array[])
 * returns:   none
 * effects:   organizes provided data into buckets in order to parse through
              data later
 */
void hash_it(std::string key, std::string filename, int lineNumber,\
             hasher::Node ref_array[]) {

    size_t slot = hash<std::string>()(key) % HASH_VALUE;

    //create a container variable that will be added to the vector
    hasher::container addInstance = {filename, lineNumber};

    if (ref_array[slot].word == "-1" or ref_array[slot].word == key) {
        
        ref_array[slot].word = key;
        ref_array[slot].instances.push_back(addInstance);

    }
    else {

        int count = linear_probing(slot, key, addInstance, ref_array);

        if (count >= HASH_VALUE) {
           
            linear_probing(0, key, addInstance, ref_array);

        }
    }

}

/*
 * name:      fill_hash_tables()
 * purpose:   read in the text data and put it into hash table
 * arguments: (std::istream &instream, std::string filename, size_t slot)
 * returns:   none
 * effects:   organizes provided data into buckets in order to parse 
              through data later
 */
void fill_hash_tables(std::istream &instream, std::string filename,\
                      size_t slot) {
    std::string str;
    char delim = '\n';
    int lineNumber = 1;


    
    // grabbing each string w/o '/n'
    while (not instream.eof()) {

        getline(instream, str, delim);
        

        // storing the whole line
        sentences[slot].all_lines.push_back(str);

        //storing each individual string  
        stringstream lineStream(str);
        std::string word;        
        while (lineStream >> word)
        {
            word = stripNonAlphaNum(word);
            

            //pass the hash table by reference to the hasher function
            hash_it(word, filename, lineNumber, dictionary);
            std:: transform(word.begin(), word.end(), word.begin(), ::tolower);
            hash_it(word, filename, lineNumber, insensitive_dictionary);
        }
        lineNumber++;
    }
}

size_t probe_sentences(size_t slot, std::string filename) {

     while (sentences[slot].filePath != "-1" and (slot < HASH_VALUE)) {
            slot++;
        }
        if (slot < HASH_VALUE) {
            sentences[slot].filePath = filename;
        }
        else {
            slot = 0;
            while (sentences[slot].filePath != "-1" and (slot < HASH_VALUE)) {
            slot++;
        }
    }
    return slot;

}

/*
 * name:      openFileandRead()
 * purpose:   open the files and read in the data
 * arguments: (std::string filename)
 * returns:   None
 * effects:   text data is put into hash table
 */
void openFileandRead(std::string filename) {

    std::ifstream instream;
    std::string currString;

    size_t slot = hash<std::string>()(filename) % HASH_VALUE;
    if (sentences[slot].filePath == "-1" or
        sentences[slot].filePath == filename) {
        
        sentences[slot].filePath = filename;

    }
    else {

        slot = probe_sentences(slot, filename);
    }


    instream.open(filename);
    if (not instream.is_open()) {       
        std::cerr << "Error: could not open file " << filename << "\n";
        exit(EXIT_FAILURE);       // opening file
    }

    fill_hash_tables(instream, filename, slot);
    instream.close();
}

/*
 * name:      recursiveTreeTraverse()
 * purpose:   Traverse the directory of text files
 * arguments: (std::string filePath, DirNode *currSubDir)
 * returns:   none
 * effects:   builds the file pathes of each text file and reads the data
 *            in each text file
 */
void recursiveTreeTraverse(std::string filePath, DirNode *currSubDir) {

    DirNode currDirNode(*currSubDir);

    filePath += currDirNode.getName() + "/";
    std::string fileName = "";


        for (int i = 0; i  < currDirNode.numSubDirs(); i++) {

            recursiveTreeTraverse(filePath, currDirNode.getSubDir(i));
        }
    

    for (int j = 0; j < currDirNode.numFiles(); j++) {

        // std::cout << filePath << currDirNode.getFile(j) << "\n";
        fileName = filePath + currDirNode.getFile(j);
        openFileandRead(fileName);
    }

}

/*
 * name:      treeTraverse()
 * purpose:   
 * arguments: TO DO
 * returns:   TO DO
 * effects:   TO DO
 */
void treeTraverse(std::string root) {

    std::string filePath = root;
    std::string fileName = "";

    unsigned long str_len = filePath.length();

    if (filePath[str_len - 1] != '/') {
        filePath += '/';
    }

    FSTree ourTree(root);
    DirNode *root_node = ourTree.getRoot();
    DirNode currNode(*root_node);

    recursiveTreeTraverse("", root_node);

    // for (int i = 0; i < currNode.numFiles(); i++) {
    //     // std::cout << filePath << currNode.getFile(i) << "\n";
    //     fileName = filePath + currNode.getFile(i);
    //     openFileandRead(fileName);
    // }

    // for (int j = 0; j < currNode.numSubDirs(); j++) {
    //     recursiveTreeTraverse(filePath, currNode.getSubDir(j));
    // }
}



/*
 * name:      buildHashTable()
 * purpose:   TO DO
 * arguments: TO DO
 * returns:   TO DO
 * effects:   TO DO
 */
void print_instances(size_t index, std::ostream& stream,\
    hasher::Node ref_array[]) {

    std::string filePath = "";
    std::string prevFilePath = "oinfoinwfoiewnfo";
    int lineNumber = -2;
    int prevLineNumber = -1;


    for (size_t i = 0; i < ref_array[index].instances.size(); i++) {

        prevFilePath = filePath;
        prevLineNumber = lineNumber;

        filePath = ref_array[index].instances.at(i).filePath;
        lineNumber = ref_array[index].instances.at(i).lineNumber;

         if (not ((lineNumber == prevLineNumber) and
                  (filePath == prevFilePath))) {

            // stream << "prev file path: " << prevFilePath << "\n";
            // stream << "prev line number: " << prevLineNumber << "\n";
            // stream << "file path: " << filePath << "\n";
            // stream << "line number" << lineNumber << "\n";
            
            size_t slot = hash<std::string>()(filePath) % HASH_VALUE;

            if (sentences[slot].filePath != filePath) {

                while (sentences[slot].filePath != filePath) {
                    slot++;
                }
            }

            stream << filePath << ":" << lineNumber << ": " 
                   << sentences[slot].all_lines.at(lineNumber - 1) << "\n";
        }
        

    }
    // for (size_t i = 0; i < dictionary[index].instances.size(); i++) {

    //     hasher::container checkIt = dictionary[index].instances.at(i);

    //     std::cout << "file path: " << checkIt.filePath << "\n";
    //     std::cout << "line number: " << checkIt.lineNumber << "\n";

    // }


}

/*
 * name:      buildHashTable()
 * purpose:   TO DO
 * arguments: TO DO
 * returns:   TO DO
 * effects:   TO DO
 */
// void read_input(std::string *input, hasher::Node arr[], std::ostream& outstream) {

//     std::string local_input = *input;
    

//     if (local_input == "@i" or local_input == "@insensitive") {

//         std::cin >> local_input;

//         std:: transform(local_input.begin(), local_input.end(), local_input.begin(), ::tolower);
//         arr = insensitive_dictionary;
//     }
//     else if (local_input == "@f") {
//         std::cin >> local_input;
//         outstream.open(local_input);
//     }
//     else {
//         arr = dictionary;
//     }
// }

/*
 * name:      buildHashTable()
 * purpose:   TO DO
 * arguments: TO DO
 * returns:   TO DO
 * effects:   TO DO
 */
void locate_bucket(hasher::Node arr[], int slot, std::ostream& outstream,\
                   std::string input, bool insensitive) {

    std::cout << insensitive << "\n";

    if (input == " ") {
        if (insensitive) {
            outstream << " Not Found.\n";
        }
        else {
            outstream << " Not Found. Try with @insensitive or @i.\n";
        }
    }
    else {
        if (arr[slot].word == input) {
            print_instances(slot, outstream, arr);
        }
        else {
            size_t j = 0;
            bool keep_going = true;
            while (j < HASH_VALUE and keep_going) {
                if (arr[j].word == input) {
                    print_instances(j, outstream, arr);
                    keep_going = false;
                }
                j++;
            }
            if (j == HASH_VALUE) {
                if (insensitive) {
                    outstream << input << " Not Found.\n";
                }
                else {
                    outstream << input << " Not Found. Try with @insensitive or @i.\n";
                }
            }
        }
    }
}


/*
 * name:      command_loop()
 * purpose:   Runs the program
 * arguments: (std::string output_file)
 * returns:   none
 * effects:   allows user to interact with all functionality of program
 */
void command_loop(std::string output_file) {
    bool still_going = true;

    std::ofstream outstream;
    outstream.open(output_file);
    if (not outstream.is_open()) {
        std::cerr << "Error: could not open file " << output_file << "\n";
        exit(EXIT_FAILURE);
    }

    while (still_going) {

    std::string input = "";
    std::cout << "Query? ";

    if (std::cin.eof()) {
        return;
    }
    hasher::Node *arr = nullptr;
    std::cin >> input;
    bool insensitive = false;

    if (input == "@q" or input == "@quit") {
        return;
    }
    if (input == "@f") {

        outstream.close();
        std::string filename;
        std::cin >> filename;
        outstream.open(filename);
    }
    else {

        if (input == "@i" or input == "@insensitive") {

        std::cin >> input;

        std:: transform(input.begin(), input.end(), input.begin(), ::tolower);
        arr = insensitive_dictionary;
        insensitive = true;

        }
        else {
        arr = dictionary;
    }
        input = stripNonAlphaNum(input);
        size_t slot = hash<std::string>()(input) % HASH_VALUE;
        locate_bucket(arr, slot, outstream, input, insensitive);
    }
    }
}
    
/*
 * name:      main()
 * purpose:   Runs the whole program
 * arguments: (int argc, char *argv[])
 * returns:   None
 * effects:   Allows user to interact with all funcitonality of the program
 */
int main(int argc, char *argv[])
{
    if (argc != 3) { 
        std::cerr << "Usage: ./gerp inputDirectory outputFile\n";
        exit (EXIT_FAILURE);
    }

    buildHashTable();
    treeTraverse(argv[1]);
    command_loop(argv[2]);

    std::cout << "Goodbye! Thank you and have a nice day." << endl;

    return 0;
}