#pragma once
#include <string>
#include <fstream>
#include <unordered_map>
#include "Student.h"
#include "nlohmann/json.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

class JsonHelper {
private:
    // 获取 exe 所在目录
    static std::string getExeDir() {
#ifdef _WIN32
        char buf[MAX_PATH] = { 0 };
        DWORD len = GetModuleFileNameA(NULL, buf, MAX_PATH);
        std::string full(buf, len);
        size_t pos = full.find_last_of("\\/");
        return (pos == std::string::npos) ? "" : full.substr(0, pos + 1);
#else
        return "";
#endif
    }

    // 获取数据文件路径（优先新文件名，兼容旧文件名）
    static std::string getDataPathForRead() {
        std::string dir = getExeDir();
        std::string newPath = dir + "data.json";
        std::string oldPath = dir + "data. json";  // 旧文件名（有空格）
        
        // 优先尝试新文件名
        std::ifstream testNew(newPath);
        if (testNew.good()) {
            return newPath;
        }
        // 兼容旧文件名
        std::ifstream testOld(oldPath);
        if (testOld.good()) {
            return oldPath;
        }
        // 都不存在，返回新文件名（用于创建）
        return newPath;
    }

    // 保存时统一用新文件名
    static std::string getDataPathForWrite() {
        return getExeDir() + "data.json";
    }

public:
    // 保存
    static bool save(const std::unordered_multimap<std::string, Student>& students) {
        try {
            nlohmann::json j = nlohmann::json::array();
            for (const auto& pair : students) {
                j.push_back(pair.second);
            }
            std::ofstream out(getDataPathForWrite());
            if (!out.is_open()) return false;
            out << j.dump(4);
            return true;
        }
        catch (...) {
            return false;
        }
    }

    // 加载
    static bool load(std::unordered_multimap<std::string, Student>& students) {
        try {
            std::string path = getDataPathForRead();
            std::ifstream in(path);
            if (!in.is_open()) return false;
            nlohmann::json j;
            in >> j;
            for (const auto& item : j) {
                Student stu = item.get<Student>();
                students.insert({ stu.xm, stu });
            }
            return true;
        }
        catch (...) {
            return false;
        }
    }
};