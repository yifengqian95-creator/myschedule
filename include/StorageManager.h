#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <vector>
#include <string>
#include "Task.h"

class StorageManager {
private:
    std::string filepath;

public:
    explicit StorageManager(const std::string& username);
    std::vector<Task> loadTasks();
    bool appendTask(const Task& task);
    bool saveAllTasks(const std::vector<Task>& tasks);
    int getNextId(const std::vector<Task>& tasks);
    static bool isStartTimeUnique(const std::vector<Task>& tasks, const std::string& startTime, int currentTaskId = -1);
};

#endif // STORAGE_MANAGER_H
