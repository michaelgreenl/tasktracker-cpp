#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

struct Task {
  int id;
  std::string description;
  Status status;
  // createdAt
  // updatedAt
}

// Make this an acutal CLI tool 
// Add asserts when possible
//
// asdf;lkj:

void add(int id, std::string description);

void update(int id, std::string description);

void delete(int id);

void markInProgress(int id);

void markDone(int id);

void list(std::string toOutput);

int main() {
  std::vector<Task> tasks;
  int nextId = 1;

  std::cout << "Task CLI - type 'help' for available commands" << std::endl;
  
  while (true) {
    std::cout << "task-cli>";
    std::getline(std::cin, input);

    if (input.empty()) continue;

    std::isstringstream iss(input);
    std::string command;
    iss >> command;

    switch (command) {
      case "exit":
      case "quit":
        break;
      case "help":
        std::cout << "Commands:\n"
                  << "  add <description>  - Add a new task\n"
                  << "  list               - List"
    }
  }
  return 0;
}

