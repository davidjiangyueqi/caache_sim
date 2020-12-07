#include <iostream>
#include <string>
#include <bitset>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>

// TODO change these variables
int associativity = 1;
int indexLength = (int) log2(256 / associativity / 32);
int offsetLength = (int) log2(32);
int data_bytes = sizeof(char);


// ===========================
// don't change these
int tagLength = 32 - indexLength - offsetLength;
int miss_count = 0;
int access_count = 0;
// ===========================


std::map<uint32_t, std::vector<uint32_t>> map;

uint32_t getIndex(uint32_t data) {
    if (indexLength == 0) return 0;
    return (uint32_t) ((data << tagLength) >> (tagLength + offsetLength));
}

uint32_t getTag(uint32_t data) {
    return (uint32_t) data >> (indexLength + offsetLength);
}

/**
 * @param data_addr the addr of the data that need access
 * @param makeCount whether current data count towards total miss
 */ 
bool access(uint32_t data_addr, bool makeCount) {
    uint32_t temp_index_ = getIndex(data_addr);
    uint32_t tag = getTag(data_addr);
    if (map.count(temp_index_) == 0 || std::find(map[temp_index_].begin(), map[temp_index_].end(), tag) == map[temp_index_].end()) {
        if (map[temp_index_].size() >= associativity) {     
            map[temp_index_].erase(map[temp_index_].begin());
        }
        map[temp_index_].push_back(tag);
        if (makeCount) {
            std::cout << std::hex << data_addr << " miss" << std::endl;
            miss_count++;
        }
        
        return false;
    } 
    
    map[temp_index_].erase(std::find(map[temp_index_].begin(), map[temp_index_].end(), tag));
    map[temp_index_].push_back(tag);
    std::cout << std::hex << data_addr << " hit" << std::endl;
    return true;
}

int main() {

    int access_count = 0;
    access_count++;
    uint32_t data_addr = 0x77f26920;
    access(0x4b1e4a1f, true);
    access(0x4b1e4a0c, true);
    access(0x4b1e4a00, true);
    access(0x4b1a4a00, true);
    access(0x4b1e4a13, true);

    // Example CODE
    // uint32_t data_addr = 0; // array offset * data_bytes + start addr;
    // access(data_addr, true);
    // access_count++;
    std::cout << "total number of access: " <<  access_count << "\n";
    std::cout << "total number of miss: " << miss_count << "\n";
    std::cout << "miss rate: " << (double)miss_count / (double)access_count << std::endl;
    return 0;
}

