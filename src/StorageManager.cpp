#include "StorageManager.h"
#include <fstream>
#include <algorithm>

StorageManager::StorageManager(const std::string& username) {
    filepath = "tasks_" + username + ".txt";
}

std::vector<Task> StorageManager::loadTasks() {
    std::vector<Task> tasks;
    std::ifstream file(filepath);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (!line.empty()) {
                tasks.push_back(Task::deserialize(line));
            }
        }
    }
    return tasks;
}

bool StorageManager::appendTask(const Task& task) {
    std::ofstream file(filepath, std::ios::app);
    if (file.is_open()) {
        file << task.serialize() << "\n";
        return true;
    }
    return false;
}

bool StorageManager::saveAllTasks(const std::vector<Task>& tasks) {
    std::ofstream file(filepath, std::ios::trunc);
    if (file.is_open()) {
        for (const auto& task : tasks) {
            file << task.serialize() << "\n";
        }
        return true;
    }
    return false;
}

int StorageManager::getNextId(const std::vector<Task>& tasks) {
    int maxId = 0;
    for (const auto& task : tasks) {
        if (task.id > maxId) {
            maxId = task.id;
        }
    }
    return maxId + 1;
}

// 检查开始时间是否唯一
// currentTaskId 用于修改任务时排除自身，避免“自己跟自己的开始时间冲突”
bool StorageManager::isStartTimeUnique(const std::vector<Task>& tasks, const std::string& startTime, int currentTaskId) {
    for (const auto& task : tasks) {
        if (task.id != currentTaskId && task.startTime == startTime) {
            return false; // 启动时间重复
        }
    }
    return true;
}
