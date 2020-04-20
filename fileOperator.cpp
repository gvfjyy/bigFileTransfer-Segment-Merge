#include <sys/stat.h>
#include <fstream>
#include <vector>
#include <iostream>

#include "json.h"
#include "fileOperator.h"

using json = nlohmann::json;

 const string fileOperator::kSegmentFileNum = "SegmentNum";
 const string fileOperator::kSourceFileName = "SourceFileName";
 const string fileOperator::kSegmentFiles = "SegmentFiles";
 const int fileOperator::kBlockSize = 1024 * 1024;  // 1MB

void fileOperator::segment(string file_name, int segment_num, string json_file) {

    // 检查源文件是否存在
    /*
    if (!exist(file_name)) {
        cout << "file [" << file_name << "] doesn't exist!" << endl;
        return;
    }*/

    // 检查分片数量是否大于0
    if (segment_num <= 0) {
        cout << "segment number should be greater than 0!" << endl;
        return;
    }

    // 分片文件名
    vector<string> segment_files;
    for (int i = 0; i < segment_num; i++) {
        segment_files.push_back(file_name + to_string(i+1) + ".deb");
        cout << "segment_file --- " << segment_files[i] << endl;
    }

    ifstream src_file_input(file_name,ios::binary);
    // 输入文件大小
    long long src_file_size = file_size(src_file_input);
    // 分片文件大小
    long long segment_size = src_file_size / segment_num;
    //cout<<"segment_num: "<<segment_num<<endl;
    //cout<<src_file_size<<endl;
    //cout<<segment_size<<endl;

    // 分片输出文件
    for (int i = 0; i < segment_num; i++) {
        ofstream segment_file_output(segment_files[i],ios::binary);

        if (i == segment_num-1) {  // 最后一次，要将剩余文件片全部写入
            long long left_size = src_file_size % segment_size;
            //cout<<segment_size + left_size<<endl;
            copy_file(src_file_input, segment_file_output, segment_size + left_size);
        } else {
            copy_file(src_file_input, segment_file_output, segment_size);
        }
        segment_file_output.close();
    }

    src_file_input.close();

    ofstream json_output(json_file);
    json j;
    j[kSegmentFileNum] = segment_num;
    j[kSourceFileName] = file_name;
    j[kSegmentFiles] = segment_files;
    json_output << j;
    json_output.close();
}


void fileOperator::merge(string json_file) {
    json j;
    /*
    if (!exist(json_file)) {
        cout << "json file [" << json_file << "] doesn't exist!" << endl;
        return;
    }*/

    ifstream json_input(json_file);
    json_input >> j;

    // 源文件名
    string src_file = j[kSourceFileName];

    // 检查源文件是否已经存在
    /*
    if (exist(src_file)) {
        src_file += ".copy";
    }*/
    ofstream result(src_file,ios::binary);

    // 文件分片数量
    int segment_num = j[kSegmentFileNum];
    // 分片文件名
    vector<string> segment_files = j[kSegmentFiles];

    // 检查文件分片是否齐全
    /*
    for (auto it = segment_files.begin(); it != segment_files.end(); ++it) {
        if (!exist(*it)) {
            cout << "segment file [" << *it << "] doesn't exist!" << endl;
            return;
        }
    }*/

    // 合并文件
    for (auto it = segment_files.begin(); it != segment_files.end(); it++) {
        cout << "copy file [" << *it << "]" << endl;
        ifstream seg_input(*it,ios::binary);
        long long seg_input_size = file_size(seg_input);
        copy_file(seg_input, result, seg_input_size);
        seg_input.close();
    }

    json_input.close();
    result.close();
}

void fileOperator::copy_file(ifstream &input, ofstream &output, long long input_size) {
    char* data = new char[kBlockSize];
    //cout<<"input_size: "<<input_size<<endl;
    for (int block = 0; block < input_size / kBlockSize; block++) {
        //cout<<"kBlockSize:"<<kBlockSize<<endl;
        read_file_in_block(data, input);
        write_file_in_block(data, output);
    }

    int left_size = input_size % kBlockSize;
    //cout<<"left_size: "<<left_size<<endl;
    if (left_size != 0) {
        read_file_in_block(data, input, left_size);
        write_file_in_block(data, output, left_size);
    }

    delete [] data;
    data = nullptr;
}
/*
bool fileOperator::exist(string name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}*/


long long fileOperator::file_size(ifstream &file) {
    long long size;
    file.seekg(0, std::ios::end);
    size = file.tellg();
    file.seekg(0, std::ios::beg);

    return size;
}

