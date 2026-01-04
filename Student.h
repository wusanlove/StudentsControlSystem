#pragma once
#include <string>
#include "nlohmann/json.hpp"

struct Student {
    std::string xh;  // 学号（12位，唯一）
    std::string xm;  // 姓名（作为 multimap 的 key）
    std::string xb;  // 性别
    int nl;          // 年龄
    std::string zy;  // 专业

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Student, xh, xm, xb, nl, zy)
};