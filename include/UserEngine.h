#ifndef USER_ENGINE_H
#define USER_ENGINE_H

#include <string>

class UserEngine {
public:
    // 计算 HASH 值
    static std::string hashPassword(const std::string& password);

    // 验证用户身份
    static bool authenticateOrRegister(const std::string& username, const std::string& password);
};

#endif // USER_ENGINE_H
