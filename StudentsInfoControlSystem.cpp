#include "MenuHandler.h"
#include <windows.h>
#include <iostream>

int main() {
    // 设置控制台代码页为 UTF-8（解决中文乱码）
    SetConsoleOutputCP(65001);  // 输出 UTF-8
    SetConsoleCP(65001);        // 输入 UTF-8
    
    // 确保 C++ 流使用正确编码
    std::ios::sync_with_stdio(false);
    
    MenuHandler::run();
    return 0;
}