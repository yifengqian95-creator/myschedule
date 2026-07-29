#ifndef TASK_H
#define TASK_H

#include <string>

class Task {
public:
    int id;                
    std::string name;      
    std::string startTime;  
    std::string priority;   
    std::string category;   
    std::string remindTime; 

    Task(int id = 0,
         std::string name = "",
         std::string startTime = "",
         std::string priority = "中",
         std::string category = "生活",
         std::string remindTime = "");

   
    std::string serialize() const;

 
    static Task deserialize(const std::string& line);
};

#endif // TASK_H
