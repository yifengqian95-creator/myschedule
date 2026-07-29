#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>
#include <vector>
#include <set>
#include "Task.h"
#include "StorageManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QString currentUsername;
    StorageManager *storage;
    std::vector<Task> allTasks;
    std::set<int> remindedTaskIds; // 记录已提醒过的任务，防止重复弹窗

    // UI 控件
    QTableWidget *taskTable;
    QLineEdit *searchEdit;
    QComboBox *priorityFilterCombo;
    QComboBox *categoryFilterCombo;
    
    // 定时器用于实时提醒检测
    QTimer *reminderTimer;

public:
    explicit MainWindow(const QString &username, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void refreshTable();
    void addTask();
    void editTask();
    void deleteTask();
    void filterTasks();
    void checkReminders();

private:
    void setupUI();
    void updateTableDisplay(const std::vector<Task> &tasksToDisplay);
};

#endif // MAIN_WINDOW_H
