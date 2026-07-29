#ifndef USER_ENGINE_H
#define USER_ENGINE_H

#include <string>

class UserEngine {
public:
    static std::string hashPassword(const std::string& password);
    static bool authenticateOrRegister(const std::string& username, const std::string& password);
};

#endif // USER_ENGINE_H
