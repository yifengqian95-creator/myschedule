#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <vector>
#include <string>
#include "Task.h"

class StorageManager {
private:
    std::string filepath;

public:
    // 构造函数：根据当前登录用户名绑定各自的储存文件（如 tasks_user1.txt）
    explicit StorageManager(const std::string& username);

    // 从文件加载当前用户的所有任务
    std::vector<Task> loadTasks();

    // 追加单个任务（满足“输入完成后自动保存到文件”）
    bool appendTask(const Task& task);

    // 覆盖保存整个任务列表（用于删除、修改任务后的自动同步）
    bool saveAllTasks(const std::vector<Task>& tasks);

    // 自动计算下一个可用的唯一 ID
    int getNextId(const std::vector<Task>& tasks);

    // 校验启动时间是否重复（要求：每个任务的启动时间不能相同）
    static bool isStartTimeUnique(const std::vector<Task>& tasks, const std::string& startTime, int currentTaskId = -1);
};

#endif // STORAGE_MANAGER_H
