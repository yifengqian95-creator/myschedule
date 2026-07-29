#ifndef TASK_H
#define TASK_H

#include <string>

// 任务模型类
class Task {
public:
    int id;                 // 唯一的任务ID
    std::string name;       // 任务名称
    std::string startTime;  // 启动时间 (格式: YYYY-MM-DD HH:MM)
    std::string priority;   // 优先级 (高/中/低，默认: 中)
    std::string category;   // 分类 (学习/娱乐/生活，默认: 生活)
    std::string remindTime; // 提醒时间 (格式: YYYY-MM-DD HH:MM)

    // 构造函数：包含缺省值设定
    Task(int id = 0,
         std::string name = "",
         std::string startTime = "",
         std::string priority = "中",
         std::string category = "生活",
         std::string remindTime = "");

    // 将任务格式化为文件存储的单行字符串
    std::string serialize() const;

    // 从文件读取的单行字符串解析出 Task 对象
    static Task deserialize(const std::string& line);
};

#endif // TASK_H
