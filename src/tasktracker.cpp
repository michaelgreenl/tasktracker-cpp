#include <iostream>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <vector>
#include <unistd.h>
#include <limits.h>

using json = nlohmann::json;

std::string timeNowToString();
void add(int id, const std::string &description);
void update(int id, const std::string &description);
void deleteTask(int id);
void markInProgress(int id);
void markDone(int id);
void list(const std::string &status);
void startREPL(int nextId);
void runCLICommand(const std::vector<std::string>& args, int& nextId);

int main(int argc, char* argv[]) {
  int nextId;

  std::ifstream in("tasks.json");
  if (in.good()) {
    json tasks;
    in >> tasks;
    in.close();

    if (!tasks.empty()) {
      const json& last = tasks.back();
      nextId = last["id"].get<int>() + 1;
    } else {
      nextId = 1;
    }
  } else {
    nextId = 1;
  } 

  if (argc > 1) {
    std::vector<std::string> args(argv + 1, argv + argc);
    runCLICommand(args, nextId);
    return 0;
  }

  // REPL mode
  startREPL(nextId);
  return 0;
}

std::string timeNowToString() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);

  std::ostringstream oss;
  oss << std::put_time(std::localtime(&now_c), "%F %T");

  return oss.str();
}

void add(int id, const std::string &description) {
  std::cout << "Adding task " << id << ": " << description << std::endl;
   
  std::ifstream in("tasks.json");
  json tasks;

  if (in.good()) {
    in >> tasks;
  } else {
    tasks = json::array();
  }
  in.close();

  json newTask;
  newTask["id"] = id;
  newTask["description"] = description;
  newTask["status"] = "todo";
  newTask["createdAt"] = timeNowToString();
  newTask["updatedAt"] = timeNowToString();

  tasks.push_back(newTask); 

  std::ofstream out("tasks.json");
  out << tasks.dump(4);
  out.close();
}

void update(int id, const std::string &description) {
  std::cout << "Updating task " << id << " with: " << description << std::endl;

  std::ifstream in("tasks.json");
  json tasks;

  if (in.good()) {
    in >> tasks;
  } else {
    std::cout << "Task list is empty, use the add command to add a task" << std::endl;
    in.close();
    return;
  } 
  in.close();

  std::string timeNow = timeNowToString();
  
  for (auto& task : tasks) {
    if (task["id"] == id) {
      task["description"] = description;
      task["updatedAt"] = timeNow;
    }
  }

  std::ofstream out("tasks.json");
  out << tasks.dump(4);
  out.close();
}

void deleteTask(int id) {
  std::cout << "Deleting task " << id << std::endl;

  std::ifstream in("tasks.json");
  json tasks;

  if (in.good()) {
    in >> tasks;
  } else {
    std::cout << "Task list is empty, use the add command to add a task" << std::endl;
    in.close();
    return; 
  }
  in.close();

  for (auto it = tasks.begin(); it != tasks.end(); ++it) {
    if ((*it)["id"] == id) {
      tasks.erase(it);
      break;
    }
  }

  std::ofstream out("tasks.json");
  out << tasks.dump(4);
  out.close();
}

void markInProgress(int id) {
  std::cout << "Marking task " << id << " as in-progress" << std::endl;

  std::ifstream in("tasks.json");
  json tasks;

  if (in.good()) {
    in >> tasks;
  } else {
    std::cout << "Task list is empty, use the add command to add a task" << std::endl;
    in.close();
    return;
  }
  in.close();

  std::string timeNow = timeNowToString();

  for (auto& task : tasks) { 
    if (task["id"] == id) {
      task["status"] = "in-progress";
      task["updatedAt"] = timeNow;
    }
  }

  std::ofstream out("tasks.json");
  out << tasks.dump(4);
  out.close();
}

void markDone(int id) {
  std::cout << "Marking task " << id << " as done" << std::endl;

  std::ifstream in("tasks.json");
  json tasks;

  if (in.good()) {
    in >> tasks;
  } else {
    std::cout << "Task list is empty, use the add command to add a task" << std::endl;
    in.close();
    return;
  }
  in.close();

  std::string timeNow = timeNowToString();

  for (auto& task : tasks) { 
    if (task["id"] == id) {
      task["status"] = "done";
      task["updatedAt"] = timeNow;
    }
  }

  std::ofstream out("tasks.json");
  out << tasks.dump(4);
  out.close();
}

void list(const std::string &status) {
  if (!status.empty()) {
    std::cout << "Listing tasks with filter: " << status << std::endl;
  } else {
    std::cout << "Listing all tasks" << std::endl;
  }

  std::ifstream in("tasks.json");
  json tasks;

  if (in.good()) {
    in >> tasks;
  } else {
    std::cout << "Task list is empty, use the add command to add a task" << std::endl;
    in.close();
    return;
  }
  in.close();

  if (status == "") {
    for (auto& task : tasks) { 
      std::cout << "ID: " << task["id"] << std::endl;
      std::cout << "Description: " << task["description"] << std::endl;
      std::cout << "Status: " << task["status"] << std::endl; 
      std::cout << "Created At: " << task["createdAt"] << std::endl; 
      std::cout << "Updated At: " << task["updatedAt"] << std::endl; 
    } 
  } else if (status == "todo") {
    for (auto& task : tasks) {
      if (task["status"] == "todo") {
        std::cout << "ID: " << task["id"] << std::endl;
        std::cout << "Description: " << task["description"] << std::endl;
        std::cout << "Status: " << task["status"] << std::endl; 
        std::cout << "Created At: " << task["createdAt"] << std::endl; 
        std::cout << "Updated At: " << task["updatedAt"] << std::endl; 
      }
    }
  } else if (status == "in-progress") {
    for (auto& task : tasks) {
      if (task["status"] == "in-progress") {
        std::cout << "ID: " << task["id"] << std::endl;
        std::cout << "Description: " << task["description"] << std::endl;
        std::cout << "Status: " << task["status"] << std::endl; 
        std::cout << "Created At: " << task["createdAt"] << std::endl; 
        std::cout << "Updated At: " << task["updatedAt"] << std::endl; 
      }
    }
  } else if (status == "done") {
    for (auto& task : tasks) {
      if (task["status"] == "done") {
        std::cout << "ID: " << task["id"] << std::endl;
        std::cout << "Description: " << task["description"] << std::endl;
        std::cout << "Status: " << task["status"] << std::endl; 
        std::cout << "Created At: " << task["createdAt"] << std::endl; 
        std::cout << "Updated At: " << task["updatedAt"] << std::endl; 
      }
    }
  }
}

void runCLICommand(const std::vector<std::string>& args, int& nextId) {
  const std::string& command = args[0];

  if (command == "add") {
    if (args.size() < 2) {
      std::cerr << "Usage: task-cli add <description>" << std::endl;
      return;
    }

    std::string description = args[1];
    for (size_t i = 2; i < args.size(); ++i)
      description += " " + args[i];

    add(nextId++, description);
  }
  else if (command == "update") {
    if (args.size() < 3) {
      std::cerr << "Usage: task-cli update <id> <description>" << std::endl;
      return;
    }
    int id = std::stoi(args[1]);
    std::string description = args[2];
    for (size_t i = 3; i < args.size(); ++i)
      description += " " + args[i];
    update(id, description);
  }
  else if (command == "delete") {
    if (args.size() < 2) {
      std::cerr << "Usage: task-cli delete <id>" << std::endl;
      return;
    }
    deleteTask(std::stoi(args[1]));
  }
  else if (command == "mark-in-progress") {
    if (args.size() < 2) {
      std::cerr << "Usage: task-cli mark-in-progress <id>" << std::endl;
      return;
    }
    markInProgress(std::stoi(args[1]));
  }
  else if (command == "mark-done") {
    if (args.size() < 2) {
      std::cerr << "Usage: task-cli mark-done <id>" << std::endl;
      return;
    }
    markDone(std::stoi(args[1]));
  }
  else if (command == "list") {
    if (args.size() == 1)
      list("");
    else
      list(args[1]);
  }
  else if (command == "help") {
    std::cout << "Commands:\n"
              << "  add <description>            - Add a new task\n"
              << "  update <id> <description>    - Update task by id\n"
              << "  delete <id>                  - Delete task by id\n"
              << "  mark-in-progress <id>        - Mark task in-progress\n"
              << "  mark-done <id>               - Mark task done\n"
              << "  list                         - List all tasks\n"
              << "  list <status>                - List tasks by status (done/todo/in-progress)\n";
  }
  else {
    std::cerr << "Unknown command: " << command << std::endl;
  }
}

void startREPL(int nextId) {
  std::string input;

  std::cout << "Task CLI - type 'help' for available commands" << std::endl;

  while (true) {
    std::cout << "task-cli> ";
    std::getline(std::cin, input);
    if (input.empty()) continue;

    std::istringstream iss(input);
    std::vector<std::string> args;
    std::string word;
    while (iss >> word)
      args.push_back(word);

    if (args.empty()) continue;
    if (args[0] == "exit" || args[0] == "quit")
      break;

    runCLICommand(args, nextId);
  }
}
