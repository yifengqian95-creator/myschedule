#include "UserEngine.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <functional>
#include <iostream>

std::string UserEngine::hashPassword(const std::string& password) {
    std::hash<std::string> hasher;
    size_t hashed = hasher(password + "my_secret_salt_2026");
    std::stringstream ss;
    ss << std::hex << hashed; 
    return ss.str();
}

bool UserEngine::authenticateOrRegister(const std::string& username, const std::string& password) {
    const std::string userFile = "users.txt";
    std::string targetHash = hashPassword(password);
    
    std::ifstream inFile(userFile);
    std::string line;
    bool userExists = false;

    if (inFile.is_open()) {
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string savedUser, savedHash;
            if (std::getline(ss, savedUser, '|') && std::getline(ss, savedHash, '|')) {
                if (savedUser == username) {
                    userExists = true;
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
