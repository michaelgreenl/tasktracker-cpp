#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

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
      // Remove leading whitespace from the description, if any.
      description.erase(0, description.find_first_not_of(" \t"));
      add(nextId, description);
      nextId++; // Increment task id after adding a task. 
    } else {
      std::cout << "Command not found" << std::endl;
    }
  }

  return 0;
}

void add(int id, const std::string &description) {
    std::cout << "Adding task " << id << ": " << description << std::endl;
}

// Stub implementations for other functions
void update(int id, const std::string &description) {
    std::cout << "Updating task " << id << " with: " << description << std::endl;
}

void deleteTask(int id) {
    std::cout << "Deleting task " << id << std::endl;
}

void markInProgress(int id) {
    std::cout << "Marking task " << id << " as in-progress" << std::endl;
}

void markDone(int id) {
    std::cout << "Marking task " << id << " as done" << std::endl;
}

void list(const std::string &toOutput) {
    std::cout << "Listing tasks with filter: " << toOutput << std::endl;
}
