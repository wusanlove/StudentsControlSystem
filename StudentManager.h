#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "Student.h"
#include "Validator.h"
#include "JsonHelper.h"

class StudentManager {
private:
    // 核心容器：姓名 → 学生（一对多）
    std::unordered_multimap<std::string, Student> students;

    StudentManager() {
        JsonHelper::load(students);
    }

    // 内部方法：检查学号是否已存在（O(n)遍历）
    bool xhExists(const std::string& xh) const {
        for (const auto& pair : students) {
            if (pair.second.xh == xh) return true;
        }
        return false;
    }

public:
    static StudentManager& getInstance() {
        static StudentManager instance;
        return instance;
    }

    StudentManager(const StudentManager&) = delete;
    StudentManager& operator=(const StudentManager&) = delete;

    // ========== FR-1/FR-2: 录入学生 ==========
    bool addStudent(const Student& stu, std::string& errMsg) {
        // 校验
        if (!Validator::isValidXh(stu.xh)) {
            errMsg = "学号格式错误（需12位数字）";
            return false;
        }
        if (!Validator::isValidNl(stu.nl)) {
            errMsg = "年龄范围错误（1-150）";
            return false;
        }
        if (!Validator::isValidXb(stu.xb)) {
            errMsg = "性别格式错误（男/女/其他/M/F）";
            return false;
        }
        if (!Validator::isValidZy(stu.zy)) {
            errMsg = "专业不在允许列表中";
            return false;
        }
        // 学号唯一性
        if (xhExists(stu.xh)) {
            errMsg = "学号已存在";
            return false;
        }

        students.insert({ stu.xm, stu });  // 姓名为 key
        return true;
    }

    // ========== FR-3: 按姓名查找（返回迭代器范围） ==========
    auto findByName(const std::string& name) {
        return students.equal_range(name);  // O(1) 查找同名所有人
    }

    // ========== FR-3: 按学号删除 ==========
    bool deleteByXh(const std::string& xh) {
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->second.xh == xh) {
                students.erase(it);
                return true;
            }
        }
        return false;
    }

    // ========== FR-4: 按学号查找（用于修改） ==========
    Student* findByXh(const std::string& xh) {
        for (auto& pair : students) {
            if (pair.second.xh == xh) return &pair.second;
        }
        return nullptr;
    }

    // ========== FR-5: 按专业查询 ==========
    std::vector<Student> searchByZy(const std::string& zy) const {
        std::vector<Student> result;
        for (const auto& pair : students) {
            if (pair.second.zy == zy) result.push_back(pair.second);
        }
        return result;
    }

    // ========== FR-6: 显示全部（按学号排序） ==========
    void displayAll() const {
        if (students.empty()) {
            std::cout << "暂无学生数据\n";
            return;
        }

        // 提取并按学号排序
        std::vector<Student> sorted;
        sorted.reserve(students.size());
        for (const auto& pair : students) {
            sorted.push_back(pair.second);
        }
        std::sort(sorted.begin(), sorted.end(),
            [](const Student& a, const Student& b) { return a.xh < b.xh; });

        // 表格输出
        std::cout << std::left
            << std::setw(14) << "学号"
            << std::setw(10) << "姓名"
            << std::setw(8) << "性别"
            << std::setw(6) << "年龄"
            << "专业\n";
        std::cout << std::string(55, '-') << "\n";

        for (const auto& s : sorted) {
            std::cout << std::left
                << std::setw(14) << s.xh
                << std::setw(10) << s.xm
                << std::setw(8) << s.xb
                << std::setw(6) << s.nl
                << s.zy << "\n";
        }
    }

    bool save() { return JsonHelper::save(students); }
    size_t count() const { return students.size(); }
};