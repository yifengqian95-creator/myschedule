#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QLabel>

MainWindow::MainWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent), currentUsername(username) {
    setWindowTitle(QString("MySchedule 日程管理系统 - 当前用户: %1").arg(currentUsername));
    resize(850, 500);

    storage = new StorageManager(currentUsername.toStdString());
    allTasks = storage->loadTasks();

    setupUI();
    refreshTable();

    // 启动定时器，每 10 秒扫描一次是否有到期提醒
    reminderTimer = new QTimer(this);
    connect(reminderTimer, &QTimer::timeout, this, &MainWindow::checkReminders);
    reminderTimer->start(10000); 
}

MainWindow::~MainWindow() {
    delete storage;
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 1. 顶部操作栏（筛选与搜索）
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(new QLabel("搜索:"));
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("按任务名称搜索...");
    topLayout->addWidget(searchEdit);

    topLayout->addWidget(new QLabel("优先级:"));
    priorityFilterCombo = new QComboBox();
    priorityFilterCombo->addItems({"全部", "高", "中", "低"});
    topLayout->addWidget(priorityFilterCombo);

    topLayout->addWidget(new QLabel("分类:"));
    categoryFilterCombo = new QComboBox();
    categoryFilterCombo->addItems({"全部", "学习", "娱乐", "生活"});
    topLayout->addWidget(categoryFilterCombo);

    mainLayout->addLayout(topLayout);

    // 2. 中间表格
    taskTable = new QTableWidget();
    taskTable->setColumnCount(6);
    taskTable->setHorizontalHeaderLabels({"ID", "任务名称", "开始时间", "优先级", "分类", "提醒时间"});
    taskTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    taskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(taskTable);

    // 3. 底部按钮栏
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *addBtn = new QPushButton("新增任务");
    QPushButton *editBtn = new QPushButton("编辑选中");
    QPushButton *delBtn = new QPushButton("删除选中");
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(editBtn);
    btnLayout->addWidget(delBtn);

    mainLayout->addLayout(btnLayout);
    setCentralWidget(centralWidget);

    // 信号槽绑定
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(editBtn, &QPushButton::clicked, this, &MainWindow::editTask);
    connect(delBtn, &QPushButton::clicked, this, &MainWindow::deleteTask);
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::filterTasks);
    connect(priorityFilterCombo, &QComboBox::currentTextChanged, this, &MainWindow::filterTasks);
    connect(categoryFilterCombo, &QComboBox::currentTextChanged, this, &MainWindow::filterTasks);
}

void MainWindow::refreshTable() {
    allTasks = storage->loadTasks();
    filterTasks();
}

void MainWindow::updateTableDisplay(const std::vector<Task> &tasksToDisplay) {
    taskTable->setRowCount(0);
    for (const auto &task : tasksToDisplay) {
        int row = taskTable->rowCount();
        taskTable->insertRow(row);
        taskTable->setItem(row, 0, new QTableWidgetItem(QString::number(task.id)));
        taskTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(task.name)));
        taskTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(task.startTime)));
        taskTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(task.priority)));
        taskTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(task.category)));
        taskTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(task.remindTime)));
    }
}

void MainWindow::filterTasks() {
    QString keyword = searchEdit->text().trimmed();
    QString selectedPriority = priorityFilterCombo->currentText();
    QString selectedCategory = categoryFilterCombo->currentText();

    std::vector<Task> filtered;
    for (const auto &task : allTasks) {
        bool matchName = keyword.isEmpty() || QString::fromStdString(task.name).contains(keyword, Qt::CaseInsensitive);
        bool matchPriority = (selectedPriority == "全部") || (task.priority == selectedPriority.toStdString());
        bool matchCategory = (selectedCategory == "全部") || (task.category == selectedCategory.toStdString());

        if (matchName && matchPriority && matchCategory) {
            filtered.push_back(task);
        }
    }
    updateTableDisplay(filtered);
}

void MainWindow::addTask() {
    bool ok;
    QString name = QInputDialog::getText(this, "新增任务", "任务名称:", QLineEdit::Normal, "", &ok);
    if (!ok || name.trimmed().isEmpty()) return;

    QString defaultTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    QString startTime = QInputDialog::getText(this, "新增任务", "开始时间 (YYYY-MM-DD HH:MM):", QLineEdit::Normal, defaultTime, &ok);
    if (!ok || startTime.trimmed().isEmpty()) return;

    // 校验开始时间唯一性
    if (!StorageManager::isStartTimeUnique(allTasks, startTime.toStdString())) {
        QMessageBox::warning(this, "警告", "已存在相同开始时间的任务！同一时间只能安排一个任务。");
        return;
    }

    QString priority = QInputDialog::getItem(this, "新增任务", "优先级:", {"高", "中", "低"}, 1, false, &ok);
    if (!ok) return;

    QString category = QInputDialog::getItem(this, "新增任务", "分类:", {"学习", "娱乐", "生活"}, 2, false, &ok);
    if (!ok) return;

    QString remindTime = QInputDialog::getText(this, "新增任务", "提醒时间 (YYYY-MM-DD HH:MM):", QLineEdit::Normal, startTime, &ok);
    if (!ok) return;

    int newId = storage->getNextId(allTasks);
    Task task(newId, name.toStdString(), startTime.toStdString(), priority.toStdString(), category.toStdString(), remindTime.toStdString());

    if (storage->appendTask(task)) {
        refreshTable();
        QMessageBox::information(this, "成功", "任务已添加并自动保存！");
    }
}

void MainWindow::editTask() {
    int curRow = taskTable->currentRow();
    if (curRow < 0) {
        QMessageBox::warning(this, "提示", "请先在表格中选中要编辑的任务！");
        return;
    }

    int taskId = taskTable->item(curRow, 0)->text().toInt();
    auto it = std::find_if(allTasks.begin(), allTasks.end(), [taskId](const Task &t) { return t.id == taskId; });
    if (it == allTasks.end()) return;

    bool ok;
    QString newName = QInputDialog::getText(this, "编辑任务", "任务名称:", QLineEdit::Normal, QString::fromStdString(it->name), &ok);
    if (!ok || newName.trimmed().isEmpty()) return;

    QString newTime = QInputDialog::getText(this, "编辑任务", "开始时间 (YYYY-MM-DD HH:MM):", QLineEdit::Normal, QString::fromStdString(it->startTime), &ok);
    if (!ok || newTime.trimmed().isEmpty()) return;

    // 排除自身后再校验开始时间唯一性
    if (!StorageManager::isStartTimeUnique(allTasks, newTime.toStdString(), taskId)) {
        QMessageBox::warning(this, "警告", "该开始时间已被其他任务占用！");
        return;
    }

    QString newPriority = QInputDialog::getItem(this, "编辑任务", "优先级:", {"高", "中", "低"}, (it->priority == "高"?0:(it->priority == "低"?2:1)), false, &ok);
    if (!ok) return;

    QString newCategory = QInputDialog::getItem(this, "编辑任务", "分类:", {"学习", "娱乐", "生活"}, (it->category == "学习"?0:(it->category == "娱乐"?1:2)), false, &ok);
    if (!ok) return;

    QString newRemind = QInputDialog::getText(this, "编辑任务", "提醒时间 (YYYY-MM-DD HH:MM):", QLineEdit::Normal, QString::fromStdString(it->remindTime), &ok);
    if (!ok) return;

    it->name = newName.toStdString();
    it->startTime = newTime.toStdString();
    it->priority = newPriority.toStdString();
    it->category = newCategory.toStdString();
    it->remindTime = newRemind.toStdString();

    storage->saveAllTasks(allTasks);
    refreshTable();
    QMessageBox::information(this, "成功", "任务更新完毕！");
}

void MainWindow::deleteTask() {
    int curRow = taskTable->currentRow();
    if (curRow < 0) {
        QMessageBox::warning(this, "提示", "请先选中要删除的任务！");
        return;
    }

    int taskId = taskTable->item(curRow, 0)->text().toInt();
    if (QMessageBox::question(this, "确认", "确定删除该任务吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        allTasks.erase(std::remove_if(allTasks.begin(), allTasks.end(), [taskId](const Task &t) { return t.id == taskId; }), allTasks.end());
        storage->saveAllTasks(allTasks);
        refreshTable();
    }
}

void MainWindow::checkReminders() {
    QString nowStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    for (const auto &task : allTasks) {
        if (remindedTaskIds.find(task.id) == remindedTaskIds.end()) {
            if (QString::fromStdString(task.remindTime) <= nowStr) {
                remindedTaskIds.insert(task.id);
                QMessageBox::information(this, "到期提醒", 
                    QString("⏰ 任务提醒!\n\n任务名称: %1\n开始时间: %2\n优先级: %3")
                    .arg(QString::fromStdString(task.name))
                    .arg(QString::fromStdString(task.startTime))
                    .arg(QString::fromStdString(task.priority)));
            }
        }
    }
}
