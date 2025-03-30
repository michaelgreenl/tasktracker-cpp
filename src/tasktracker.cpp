#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Task {
  int id;
  std::string description;
  std::string status;
};

void add(int id, const std::string &description);

void update(int id, const std::string &description);

void deleteTask(int id);

void markInProgress(int id);

void markDone(int id);

void list(const std::string &toOutput);

int main() {
  // FIXME: Are the variables tasks || nextId necessary? Maybe nextId but the tasks vector isn't necessary if tasks.json is accessed consistently.
  std::vector<Task> tasks;
  int nextId = 1;
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
      // list();
    } else {
      std::cout << "Command not found" << std::endl;
    }
  }

  return 0;
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

  // TODO: Add createdAt & updatedAt properties, then get the current time here 
  
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

  if (id > tasks.size() || id < 1) {
    std::cout << "Task id does not exist" << std::endl;
    return;
  }
  
  for (auto& task : tasks) {
    if (task["id"] == id) {
      task["description"] = description;
      // update updatedAt
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

  if (id > tasks.size() || id < 1) {
    std::cout << "Task id does not exist" << std::endl;
    return;
  }

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

  if (id > tasks.size() || id < 1) {
    std::cout << "Task id does not exist" << std::endl;
    return;
  }

  for (auto& task : tasks) { 
    if (task["id"] == id) {
      task["status"] = "in-progress";
      // update updatedAt 
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

  if (id > tasks.size() || id < 1) {
    std::cout << "Task id does not exist" << std::endl;
    return;
  }

  for (auto& task : tasks) { 
    if (task["id"] == id) {
      task["status"] = "done";
      // update updatedAt
    }
  }
}

void list(const std::string &toOutput) {
  std::cout << "Listing tasks with filter: " << toOutput << std::endl;
}
