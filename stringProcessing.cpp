/*
 * Sam Miller & Sami Hakkarainen
 * 12/1/2022
 * 
 * Purpose - The implemenation for a parser for key words that
 *           are inputted by the user.
 */

#include "stringProcessing.h"
#include <iostream>

std::string stripNonAlphaNum(std::string input)
{
    size_t i = 0;
    bool keep_going;
    size_t count;
    // looking for first alpha numeric
    while ((not isValid(input[i])) and (i < input.size())) {
                i++;    
        }

        // std::cout << "word: " << input << "\n";
        // std::cout << "size: " << input.size() << "\n";
        // std::cout << "i " << i << "\n";
    
    // cutting off leading invalid chars
    input = input.substr(i,input.size()); 

    // std::cout << "broooooo what\n";

    for (unsigned long j = 0; j < input.size(); j++) {

        count = 0;
        keep_going = true;

        

        if (not isValid(input[j])) {
            
            j++;
            count++;
            // std::cout << "j: " << j << "\n";
            // std::cout << "input: " << input << "\nSize:";
            // std::cout << input.size() << "\n";

            while (j < input.size() and keep_going) {

                if (isValid(input[j])) {
                    keep_going = false;
                }
                else {
                    count++;
                    j++;
                }
            //     std::cout << "j: " << j << "\n";
            // std::cout << "input: " << input << "\nSize:";
            // std::cout << input.size() << "\n";
            }
        }
   }

//     std::cout << "OUTSIDE!!!!!!!\n";
//    std::cout << "count: " << count << "\n";
//             std::cout << "input: " << input << "\nSize:";
//             std::cout << input.size() << "\n";

    if (input.empty()) {
        return " ";
    }
   else if (count >= input.size()) {
    return " ";
   }
   else if (keep_going == false) {
        return input;
   }
   else {
        return input = input.substr(0, input.size() - count);
   }

}

bool isValid(char input)
{
    if ((65 <= input and 90 >= input) or 
        (97 <= input and 122 >= input) or 
        (48 <= input and 57 >= input)) {

        return true;
    }
    
    return false;
}