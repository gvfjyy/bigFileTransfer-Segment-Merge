#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class fileOperator {
public:
    void segment(string file_name, int segment_num, string json_file="config.json");
    void merge(string json_file="config.json");
private:
    bool exist(string name);
    long long file_size(ifstream &file);
    void copy_file(ifstream &input, ofstream &output, long long input_size);

    inline void read_file_in_block(char* data, ifstream &input, int size=kBlockSize) {
        //cout<<"read: "<<size<<endl;
        input.read(data, size);
    }
    inline void write_file_in_block(char* data, ofstream &output, int size=kBlockSize) {
        //cout<<"write: "<<size<<endl;
        output.write(data, size);
    }
private:
    // json key
    static const string kSegmentFileNum;
    static const string kSourceFileName;
    static const string kSegmentFiles;

    static const int kBlockSize;

};
#endif // FILEOPERATOR_H
