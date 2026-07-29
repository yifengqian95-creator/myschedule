#ifndef USER_ENGINE_H
#define USER_ENGINE_H

#include <string>

class UserEngine {
public:
    // 计算密码的 HASH 值（加密）
    static std::string hashPassword(const std::string& password);

    // 验证用户身份（如果用户不存在，则自动注册并返回成功）
    static bool authenticateOrRegister(const std::string& username, const std::string& password);
};

#endif // USER_ENGINE_H
