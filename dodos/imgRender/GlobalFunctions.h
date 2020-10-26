//
// Created by someone on 2019/12/27.
//

#ifndef IMGRENDER_GLOBALFUNCTIONS_H
#define IMGRENDER_GLOBALFUNCTIONS_H

#include <vector>
#include <string>
#include <sstream>

//std::vector<std::string> splice(const std::string &str, const std::string &splitStr) {
//    std::vector<std::string> result;
//    int lastIndex = 0;
//    while (1) {
//        int findIndex = str.find(splitStr, lastIndex);
//        if (findIndex == -1) {
//            result.push_back(str.substr(lastIndex, str.size() - lastIndex));
//            break;
//        } else {
//            result.push_back(str.substr(lastIndex, findIndex - lastIndex));
//            lastIndex = findIndex + splitStr.size();
//        }
//    }
//
//    return result;
//}

inline float limit(float val,float min, float max){
    if(val>max)return max;
    if(val<min)return min;
    return val;
}

#endif //IMGRENDER_GLOBALFUNCTIONS_H
