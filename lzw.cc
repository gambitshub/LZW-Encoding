/**************************************************************
 * Author: James Daniels
 * 
 * C++ code reads in .txt file, generates LZW Encoding,
 * 
 * Instructions:
 * 1. Include "grail_testfile.txt" and the makefile in
 *     current directory.
 * 2. Make.
 * 3. ./lzw to view output.
**************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
// #include <bits/stdc++.h>


using namespace std;

vector<int> encoder(string src_str){
    // Initialize dixtionary containing all strings of length 1
    unordered_map<string, int> dict;
    // where key is a string, store decimal index as int
    string byte = "";
    byte = char(3);
    dict[byte] = 0;
    byte = char(10);
    dict[byte] = 1;
    byte = char(13);
    dict[byte] = 2;
    byte = char(32);
    dict[byte] = 3;
    for(int i = 97; i < 123; i++){
        byte = char(i);
        dict[byte] = (i - 93);
    }
    // for(int i = 0; i < 123; i++){
    //     byte = char(i);
    //     cout << dict[byte]<<endl;
    // }

    string Symbols = "";
    // Parse text file from beginning
    Symbols = src_str[0];
    int dec_code = 31;
    vector<int> encoded_text;
    // cout << "String\tencoded_text\tAddition\n";
        // Loop til end of source text
    for (int i = 0; i < (src_str.length() - 1); i++){
        // Store next char byte
        if(i != src_str.length() - 1)
            byte = src_str[i + 1];
        // If string already in dictionary, add next byte to string S
        if(dict.find(Symbols + byte) != dict.end()){
            Symbols = Symbols + byte;
        }
        // If dictionary doesnt contain string
        else{
            // cout << p << "\t" << dict[p] << "\t\t" 
            //      << p + c << "\t" << code << endl;
            encoded_text.push_back(dict[Symbols]);
            dict[Symbols + byte] = dec_code;
            dec_code++;
            Symbols = byte;
        }
    }
    // cout << p << "\t" << dict[p] << endl;
    encoded_text.push_back(dict[Symbols]);
    return encoded_text;
}

// vector<double> probability();

string decoder(vector<int> encoded_text){
    // Initialize dixtionary containing all strings of length 1
    // Decoder is the reverse of the encoder.
    unordered_map<int, string> dict;
    // now key is decimal integer, symbols are string value
    string byte = "";
    byte = char(3);
    dict[0] = byte;
    byte = char(10);
    dict[1] = byte;
    byte = char(13);
    dict[2] = byte;
    byte = char(32);
    dict[3] = byte;
    for(int i = 97; i <= 122; i++){
        byte = char(i);
        dict[i - 93] = byte;
    }
    string decoded_text = "";
    int dec_code = encoded_text[0];
    int key;
    string Symbol = dict[dec_code];
    byte = Symbol[0];
    decoded_text += Symbol;
    // cout << s;
    int index = 31;
    // loop while can read in new code pointer
    for (int i = 0; i < encoded_text.size() - 1; i++){
        key = encoded_text[i + 1];
        // End of code case
        if(dict.find(key) == dict.end()){
            Symbol = dict[dec_code];
            Symbol = Symbol + byte;
        }
        // Normal case, lookup key
        else{
            Symbol = dict[key];
        }
        // cout << Symbol;
        // Add current symbol to end of text string
        decoded_text += Symbol;
        byte = Symbol[0];
        dict[index] = dict[dec_code] + byte;
        index++;
        dec_code = key;
    }
    return decoded_text;
}

double lavg(vector<int> encoded_text){
    unordered_map<int, string> dict;
    string byte = "";
    byte = char(3);
    dict[0] = byte;
    byte = char(10);
    dict[1] = byte;
    byte = char(13);
    dict[2] = byte;
    byte = char(32);
    dict[3] = byte;
    for(int i = 97; i <= 122; i++){
        byte = char(i);
        dict[i - 93] = byte;
    }
    vector<double> probs(65535, 0.0);
    int dec_code = encoded_text[0];
    int key;
    string Symbol = dict[dec_code];
    byte = Symbol[0];
    int index = 31;
    for (int i = 0; i < encoded_text.size() - 1; i++){
        key = encoded_text[i + 1];
        if(dict.find(key) == dict.end()){
            Symbol = dict[dec_code];
            Symbol = Symbol + byte;
        }
        else{
            Symbol = dict[key];
        }
        byte = Symbol[0];
        dict[index] = dict[dec_code] + byte;
        index++;
        probs[key] += 1;
        dec_code = key;
    }
    double lavg = 0;
    for(int i = 0; i < dict.size(); i++){
        lavg += probs[i]/dict.size()*dict[i].length();
    }
    string longest_string = "";
    for(int i = 0; i < dict.size(); i++){
        if(dict[i].length() > longest_string.length()){
            longest_string = dict[i];
        }
    }
    cout << "The longest string in the encoding table is: \'" << longest_string << "\' with " << longest_string.length() << " characters.\n";
    cout << "\nL_avg: " << lavg << " output bytes per input byte\n\n";
    return lavg;
}

string dec2bin16(vector<int> encoded){
    string bin16 = "";
    string temp = "";
    for(int i = 0; i < encoded.size(); i++){
        int tmp = encoded[i];
        for(int i = 17; i>=0; i--){
            int k = tmp >> i;
            if (k & 1){
                bin16 += "1";
            }    
            else{
                bin16 += "0";
            }
        }
        bin16 += " ";
    }
    return bin16;
}

int main(){
    // Input from File
    ifstream ins;
    ins.open("EE6743_grail_testfile.txt");
    stringstream stream;
    stream << ins.rdbuf();
    string src_str = stream.str();
    ins.close();
    string stc_str_cpy = src_str;
    // cout << src_str << src_str.length() == 61,392 << "\n"; // Input check
    // cout << src_str.length() << endl;

    vector<int> encoded = encoder(src_str);

    // Convert to string for output
    string encoded_str = "";
    for(int i = 0; i < encoded.size(); i++){
        encoded_str += to_string(encoded[i]);
        encoded_str += " ";
    }
    // Change output into 16 bit binary sequence
    string encoded_bin16 = dec2bin16(encoded);
    // cout << encoded_bin16;

    // Output encoded binary to text file
    ofstream output_file;
    output_file.open("LZW_Encoded.txt");
    output_file << encoded_bin16;
    output_file.close();

    // Run decoder on encoded int vector
    string decoded = decoder(encoded);
    // cout<<decoded;

    // Check reconstructed message matches original source message
    if(src_str == decoded){cout<<"\nSUCCESS! Both texts match.\n";}
    else{cout<<"\nERROR: Text Misconstruction\n";}

    // Output Length of final encoding
    cout << "\nLength Of Compressed Binary Output File: " << encoded_bin16.size() << " bits\n";
    double comp_length = encoded_bin16.size() / 8.0;
    cout << "\nLength Of Compressed Binary Output File: " << comp_length << " bytes\n";

    // Calculate Compression Ratio
    cout << "\nCompression Ratio: " << 61392.0 / comp_length << "\n\n";

    lavg(encoded);

}
