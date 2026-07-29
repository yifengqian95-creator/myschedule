#include "Task.h"
#include <sstream>

Task::Task(int id, std::string name, std::string startTime,
           std::string priority, std::string category, std::string remindTime)
    : id(id), name(name), startTime(startTime),
      priority(priority.empty() ? "中" : priority),
      category(category.empty() ? "生活" : category),
      remindTime(remindTime.empty() ? startTime : remindTime) {}

// 用管道符 | 作为分隔符进行序列化，方便文件保存
std::string Task::serialize() const {
    std::stringstream ss;
    ss << id << "|" << name << "|" << startTime << "|"
       << priority << "|" << category << "|" << remindTime;
    return ss.str();
}

Task Task::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string token;
    Task task;

    if (std::getline(ss, token, '|')) task.id = std::stoi(token);
    if (std::getline(ss, token, '|')) task.name = token;
    if (std::getline(ss, token, '|')) task.startTime = token;
    if (std::getline(ss, token, '|')) task.priority = token;
    if (std::getline(ss, token, '|')) task.category = token;
    if (std::getline(ss, token, '|')) task.remindTime = token;

    return task;
}
