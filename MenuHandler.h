#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include "StudentManager.h"
#include "Validator.h"

// 防止 windows.h 中的 max 宏干扰 std::numeric_limits::max()
#ifdef max
#undef max
#endif

class MenuHandler {
private:
    static void clearInput() {
        std::cin.clear();
        std::cin.ignore(10000, '\n');  // 简化写法，清空输入缓冲区
    }

    // 读取整数，支持输入 q 退出（返回 -999 表示退出）
    static int readIntOrQuit(const std::string& prompt) {
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);
            if (input == "q" || input == "Q") return -999;  // 退出标记
            if (input.empty()) {
                std::cout << "× 不能为空，请重新输入\n";
                continue;
            }
            try {
                return std::stoi(input);
            }
            catch (...) {
                std::cout << "× 请输入有效数字\n";
            }
        }
    }

    static std::string readString(const std::string& prompt) {
        std::string val;
        std::cout << prompt;
        std::getline(std::cin, val);
        return val;
    }

    // 判断是否是退出指令
    static bool isQuit(const std::string& s) {
        return s == "q" || s == "Q";
    }

    static void showMenu() {
        std::cout << "\n====== 学生信息管理系统 ======\n"
            << "1. 录入学生信息\n"
            << "2. 删除学生（按姓名）\n"
            << "3. 修改学生（按姓名）\n"
            << "4. 查询学生（按专业）\n"
            << "5. 显示全部学生\n"
            << "0. 保存并退出\n"
            << "==============================\n";
    }

    static void showMajors() {
        std::cout << "可选专业: ";
        for (const auto& m : Validator::getValidMajors()) {
            std::cout << m << " | ";
        }
        std::cout << "\n";
    }

    // ========== 1. 录入 ==========
    static void handleAdd() {
        auto& mgr = StudentManager::getInstance();
        std::cout << "\n--- 录入学生 (任意输入项输入 q 可退出) ---\n";

        while (true) {
            Student stu;

            // 学号（循环直到输入正确或退出）
            while (true) {
                stu.xh = readString("学号(12位数字): ");
                if (isQuit(stu.xh)) return;
                if (stu.xh.empty()) {
                    std::cout << "× 学号不能为空，请重新输入\n";
                    continue;
                }
                if (!Validator::isValidXh(stu.xh)) {
                    std::cout << "× 学号必须是12位数字，请重新输入\n";
                    continue;
                }
                break;  // 输入正确，跳出循环
            }

            // 姓名（循环直到输入正确或退出）
            while (true) {
                stu.xm = readString("姓名: ");
                if (isQuit(stu.xm)) return;
                if (stu.xm.empty()) {
                    std::cout << "× 姓名不能为空，请重新输入\n";
                    continue;
                }
                if (!Validator::isValidXm(stu.xm)) {
                    std::cout << "× 姓名不能包含数字或特殊符号，请重新输入\n";
                    continue;
                }
                break;
            }

            // 性别（循环直到输入正确或退出）
            while (true) {
                stu.xb = readString("性别(男/女/其他/M/F): ");
                if (isQuit(stu.xb)) return;
                if (stu.xb.empty()) {
                    std::cout << "× 性别不能为空，请重新输入\n";
                    continue;
                }
                if (!Validator::isValidXb(stu.xb)) {
                    std::cout << "× 性别只能是: 男/女/其他/M/F，请重新输入\n";
                    continue;
                }
                break;
            }

            // 年龄（循环直到输入正确或退出）
            while (true) {
                std::string ageStr = readString("年龄(1-150): ");
                if (isQuit(ageStr)) return;
                if (ageStr.empty()) {
                    std::cout << "× 年龄不能为空，请重新输入\n";
                    continue;
                }
                if (!Validator::isValidNlStr(ageStr)) {
                    std::cout << "× 年龄必须是纯数字，请重新输入\n";
                    continue;
                }
                stu.nl = std::stoi(ageStr);
                if (!Validator::isValidNl(stu.nl)) {
                    std::cout << "× 年龄必须在1-150之间，请重新输入\n";
                    continue;
                }
                break;
            }

            // 专业（循环直到输入正确或退出）
            while (true) {
                showMajors();
                stu.zy = readString("专业: ");
                if (isQuit(stu.zy)) return;
                if (stu.zy.empty()) {
                    std::cout << "× 专业不能为空，请重新输入\n";
                    continue;
                }
                if (!Validator::isValidZy(stu.zy)) {
                    std::cout << "× 专业必须从上述列表中选择，请重新输入\n";
                    continue;
                }
                break;
            }

            // 检查学号是否已存在
            std::string errMsg;
            if (mgr.addStudent(stu, errMsg)) {
                std::cout << "√ 添加成功，当前共 " << mgr.count() << " 人\n\n";
            }
            else {
                std::cout << "× 失败: " << errMsg << "\n\n";
            }
        }
    }

    // ========== 2. 删除（按姓名） ==========
    static void handleDelete() {
        auto& mgr = StudentManager::getInstance();
        std::cout << "\n--- 删除学生 (输入 q 可退出) ---\n";
        
        std::string name = readString("输入要删除的学生姓名: ");
        if (isQuit(name)) return;

        auto range = mgr.findByName(name);
        auto begin = range.first;
        auto end = range.second;
        if (begin == end) {
            std::cout << "未找到姓名为「" << name << "」的学生\n";
            return;
        }

        // 收集匹配项
        std::vector<Student*> matches;
        for (auto it = begin; it != end; ++it) {
            matches.push_back(&it->second);
        }

        // 显示列表
        std::cout << "找到 " << matches.size() << " 条记录:\n";
        for (size_t i = 0; i < matches.size(); ++i) {
            std::cout << i + 1 << ". "
                << matches[i]->xh << " - "
                << matches[i]->xm << " - "
                << matches[i]->zy << "\n";
        }

        int idx = readIntOrQuit("输入序号删除 (0取消, q退出): ");
        if (idx == -999) return;  // 退出
        if (idx == 0) return;     // 取消
        
        if (idx > 0 && idx <= (int)matches.size()) {
            std::string xh = matches[idx - 1]->xh;
            std::string confirm = readString("确认删除学号 " + xh + " ? (y/n): ");
            if (confirm == "y" || confirm == "Y") {
                mgr.deleteByXh(xh);
                std::cout << "√ 已删除\n";
            } else {
                std::cout << "已取消删除\n";
            }
        } else {
            std::cout << "× 无效序号\n";
        }
    }

    // ========== 3. 修改（按姓名） ==========
    static void handleModify() {
        auto& mgr = StudentManager::getInstance();
        std::cout << "\n--- 修改学生 (输入 q 可退出) ---\n";
        
        std::string name = readString("输入要修改的学生姓名: ");
        if (isQuit(name)) return;

        auto range = mgr.findByName(name);
        auto begin = range.first;
        auto end = range.second;
        if (begin == end) {
            std::cout << "未找到姓名为「" << name << "」的学生\n";
            return;
        }

        // 收集匹配项
        std::vector<Student*> matches;
        for (auto it = begin; it != end; ++it) {
            matches.push_back(&it->second);
        }

        // 显示列表
        std::cout << "找到 " << matches.size() << " 条记录:\n";
        for (size_t i = 0; i < matches.size(); ++i) {
            std::cout << i + 1 << ". "
                << matches[i]->xh << " - "
                << matches[i]->xm << "\n";
        }

        int idx = readIntOrQuit("输入序号修改 (0取消, q退出): ");
        if (idx == -999) return;  // 退出
        if (idx == 0) return;     // 取消
        
        if (idx < 1 || idx > (int)matches.size()) {
            std::cout << "× 无效序号\n";
            return;
        }

        Student* stu = matches[idx - 1];
        std::cout << "当前: " << stu->xh << ", " << stu->xm << ", "
            << stu->xb << ", " << stu->nl << ", " << stu->zy << "\n";
        std::cout << "(直接回车保持原值, 输入 q 退出修改)\n";

        std::string input;

        // 修改性别
        input = readString("新性别(男/女/其他/M/F): ");
        if (isQuit(input)) return;
        if (!input.empty()) {
            if (Validator::isValidXb(input)) {
                stu->xb = input;
                std::cout << "√ 性别已更新\n";
            } else {
                std::cout << "× 性别格式错误，保持原值\n";
            }
        }

        // 修改年龄
        input = readString("新年龄(1-150): ");
        if (isQuit(input)) return;
        if (!input.empty()) {
            if (Validator::isValidNlStr(input)) {
                int age = std::stoi(input);
                if (Validator::isValidNl(age)) {
                    stu->nl = age;
                    std::cout << "√ 年龄已更新\n";
                } else {
                    std::cout << "× 年龄范围错误(1-150)，保持原值\n";
                }
            } else {
                std::cout << "× 年龄必须是纯数字，保持原值\n";
            }
        }

        // 修改专业
        showMajors();
        input = readString("新专业: ");
        if (isQuit(input)) return;
        if (!input.empty()) {
            if (Validator::isValidZy(input)) {
                stu->zy = input;
                std::cout << "√ 专业已更新\n";
            } else {
                std::cout << "× 专业不在列表中，保持原值\n";
            }
        }

        std::cout << "√ 修改完成\n";
    }

    // ========== 4. 查询（按专业） ==========
    static void handleSearch() {
        auto& mgr = StudentManager::getInstance();
        std::cout << "\n--- 查询学生 (输入 q 可退出) ---\n";
        showMajors();
        
        std::string zy = readString("输入要查询的专业: ");
        if (isQuit(zy)) return;

        auto results = mgr.searchByZy(zy);
        if (results.empty()) {
            std::cout << "未找到该专业的学生\n";
        }
        else {
            std::cout << "找到 " << results.size() << " 人:\n";
            for (const auto& s : results) {
                std::cout << s.xh << " - " << s.xm << " - "
                    << s.xb << " - " << s.nl << "岁\n";
            }
        }
    }

public:
    static void run() {
        auto& mgr = StudentManager::getInstance();
        std::cout << "系统启动，已加载 " << mgr.count() << " 条数据\n";

        while (true) {
            showMenu();
            int choice = readIntOrQuit("请选择: ");
            if (choice == -999) {
                // 主菜单输入 q 也保存退出
                std::cout << (mgr.save() ? "数据已保存，再见！\n" : "警告：保存失败！\n");
                return;
            }

            switch (choice) {
            case 1: handleAdd(); break;
            case 2: handleDelete(); break;
            case 3: handleModify(); break;
            case 4: handleSearch(); break;
            case 5: mgr.displayAll(); break;
            case 0:
                std::cout << (mgr.save() ? "数据已保存，再见！\n" : "警告：保存失败！\n");
                return;
            default:
                std::cout << "× 无效选项，请输入 0-5\n";
            }
        }
    }
};