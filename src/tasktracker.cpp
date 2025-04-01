#include <iostream>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>

using json = nlohmann::json;

struct Task {
  int id;
  std::string description;
  std::string status;
  std::string createdAt;
  std::string updatedAt;
};

std::string timeNowToString();

void add(int id, const std::string &description);

void update(int id, const std::string &description);

void deleteTask(int id);

void markInProgress(int id);

void markDone(int id);

void list(const std::string &status);

int main() {
  int nextId;
  std::ifstream in("data/tasks.json");

  if (in.good()) {
    json tasks;
    in >> tasks;

    const json& last = tasks.back();
    int prevId = last["id"];
    nextId = prevId + 1;
  } else {
    nextId = 1;
  }
  in.close();

  std::string input;

  std::cout << "Task CLI - type 'help' for available commands" << std::endl;
  
  while (true) {
    std::cout << "task-cli>";
    std::getline(std::cin, input);

    if (input.empty()) continue;

    std::istringstream iss(input);
    std::string command;
    iss >> command;

    if (command == "exit" || command == "quit") {
      break;
    } else if (command == "help") {
        std::cout << "Commands:\n"
                  << "  add <description>            - Add a new task\n"
                  << "  update <id> <description>    - Update task by id\n"
                  << "  delete <id>                  - Delete task by id\n"
                  << "  mark-in-progress <id>        - Mark task in-progress\n"
                  << "  mark-done <id>               - Mark task done\n"
                  << "  list                         - List all tasks\n"
                  << "  list <status>                - List tasks by status (done/todo/in-progress)\n";
    } else if (command == "add") {
      std::string description;
      std::getline(iss, description);
      description.erase(0, description.find_first_not_of(" \t"));

      add(nextId, description);
      nextId++;  
    } else if (command == "update") {
      int inid;
      std::string description;
      iss >> inid;
      std::getline(iss, description);
      description.erase(0, description.find_first_not_of(" \t")); 

      update(inid, description);
    } else if (command == "delete") {
      int inid;
      iss >> inid;
      deleteTask(inid);
    } else if (command == "mark-in-progress") {
      int inid;
      iss >> inid;
      
      markInProgress(inid);
    } else if (command == "mark-done") {
      int inid;
      iss >> inid;

      markDone(inid);
    } else if (command == "list") {
      std::string status;
      iss >> status;

      list(status); 
    } else {
      std::cout << "Command not found" << std::endl;
    }
  }

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
  
  std::ifstream in("data/tasks.json");
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

  std::ofstream out("data/tasks.json");
  out << tasks.dump(4);
  out.close();
}

void update(int id, const std::string &description) {
  std::cout << "Updating task " << id << " with: " << description << std::endl;

  std::ifstream in("data/tasks.json");
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

  std::ofstream out("data/tasks.json");
  out << tasks.dump(4);
  out.close();
}

void deleteTask(int id) {
  std::cout << "Deleting task " << id << std::endl;

  std::ifstream in("data/tasks.json");
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

  std::ofstream out("data/tasks.json");
  out << tasks.dump(4);
  out.close();
}

void markInProgress(int id) {
  std::cout << "Marking task " << id << " as in-progress" << std::endl;

  std::ifstream in("data/tasks.json");
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

  std::ofstream out("data/tasks.json");
  out << tasks.dump(4);
  out.close();
}

void markDone(int id) {
  std::cout << "Marking task " << id << " as done" << std::endl;

  std::ifstream in("data/tasks.json");
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
}

void list(const std::string &status) {
  std::cout << "Listing tasks with filter: " << status << std::endl;

  std::ifstream in("data/tasks.json");
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
