#pragma once
#include <string>
#include <vector>
#include <regex>
#include <cctype>

class Validator {
public:
    // 学号：必须是12位数字
    static bool isValidXh(const std::string& xh) {
        if (xh.length() != 12) return false;
        for (char c : xh) {
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;
    }

    // 姓名：不能为空，不能包含数字，不能包含特殊符号（只允许中文、英文、空格）
    static bool isValidXm(const std::string& xm) {
        if (xm.empty()) return false;
        for (size_t i = 0; i < xm.length(); ++i) {
            unsigned char c = static_cast<unsigned char>(xm[i]);
            // 允许中文（UTF-8 多字节）、英文字母、空格
            if (std::isdigit(c)) return false;  // 不允许数字
            // 检查常见特殊符号
            if (c == '!' || c == '@' || c == '#' || c == '$' || c == '%' ||
                c == '^' || c == '&' || c == '*' || c == '(' || c == ')' ||
                c == '_' || c == '=' || c == '+' || c == '[' || c == ']' ||
                c == '{' || c == '}' || c == '|' || c == '\\' || c == '/' ||
                c == '<' || c == '>' || c == '?' || c == ',' || c == '.' ||
                c == ';' || c == ':' || c == '"' || c == '\'') {
                return false;
            }
        }
        return true;
    }

    // 年龄：1-150之间的整数
    static bool isValidNl(int nl) {
        return nl >= 1 && nl <= 150;
    }

    // 年龄字符串：必须是纯数字
    static bool isValidNlStr(const std::string& nlStr) {
        if (nlStr.empty()) return false;
        for (char c : nlStr) {
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;
    }

    // 性别：男/女/其他/M/F
    static bool isValidXb(const std::string& xb) {
        return xb == "男" || xb == "女" || xb == "其他" ||
            xb == "M" || xb == "m" || xb == "F" || xb == "f";
    }

    // 获取有效专业列表
    static const std::vector<std::string>& getValidMajors() {
        static std::vector<std::string> majors = {
            "计算机科学与技术", "软件工程", "人工智能",
            "数据科学", "网络工程", "信息安全"
        };
        return majors;
    }

    // 专业：必须在列表中
    static bool isValidZy(const std::string& zy) {
        for (const auto& m : getValidMajors()) {
            if (m == zy) return true;
        }
        return false;
    }
};