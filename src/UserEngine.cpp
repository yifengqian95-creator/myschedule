#include "UserEngine.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <functional>
#include <iostream>

// 对明文口令进行 Hash 计算（作业要求：口令不能明文保存）
std::string UserEngine::hashPassword(const std::string& password) {
    std::hash<std::string> hasher;
    // 加个盐值 (Salt) 防止彩虹表破解
    size_t hashed = hasher(password + "my_secret_salt_2026");
    std::stringstream ss;
    ss << std::hex << hashed; // 转成十六进制字符串保存
    return ss.str();
}

bool UserEngine::authenticateOrRegister(const std::string& username, const std::string& password) {
    const std::string userFile = "users.txt";
    std::string targetHash = hashPassword(password);
    
    std::ifstream inFile(userFile);
    std::string line;
    bool userExists = false;

    // 尝试在 users.txt 中寻找该用户
    if (inFile.is_open()) {
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string savedUser, savedHash;
            if (std::getline(ss, savedUser, '|') && std::getline(ss, savedHash, '|')) {
                if (savedUser == username) {
                    userExists = true;
                    // 校验密码 Hash 是否匹配
                    if (savedHash == targetHash) {
                        return true; 
                    } else {
                        std::cerr << "错误：口令不匹配，登录失败！\n";
                        return false; 
                    }
                }
            }
        }
        inFile.close();
    }

    // 作业需求：如果用户不存在，则用这个用户名和密码自动注册一个新账户
    if (!userExists) {
        std::ofstream outFile(userFile, std::ios::app);
        if (outFile.is_open()) {
            outFile << username << "|" << targetHash << "\n";
            std::cout << "检测到新用户，已自动注册并登录。\n";
            return true;
        }
    }

    return false;
}
