#ifndef ISE_COMMANDS_HPP_
#define ISE_COMMANDS_HPP_

#include "command.hpp"

class Commands {
public:
  static void call(char **args, int argSize) {
    if (argSize <= 0) return;
    bool found = false;
    for (auto cmd : sCommands) {
      if (strcmp(cmd->getName(), args[0]) == 0) {
        found = true;
        if (cmd->check(args, argSize))
          cmd->call(args, argSize);
        else {
          printf("Invalid arguments! Usage: `%s`\n", cmd->getUsage());
        }
      }
    }
    if (!found) {
      printf("Command `%s` not found!\n", args[0]);
    }
  }

  static void initCommand(Command *cmd) { sCommands.push_back(cmd); }

  static std::vector<Command*> getCommands() { return sCommands; }
  static Command *getCommand(const char *name, bool *found) {
    for (auto cmd_ : sCommands) {
      if (strcmp(name, cmd_->getName()) == 0) {
        *found = true;
        return cmd_;
      }
    }
    return nullptr;
  }
private:
  inline static std::vector<Command*> sCommands{};
};

class HelpCommand : public Command {
public:
  HelpCommand():
    Command("help", "help [command]")
  {}

  ~HelpCommand() {

  }

  virtual void call(char **args, int argSize) override {
    printf("Help:\n");
    if (argSize == 1) {
      printf("  Startup info:\n    - Data Dir: %s\n", sOptions->DataDir);
      printf("  Commands:\n");
      for (auto cmd : Commands::getCommands())
        printf("    - `%s`:\n      Usage: %s\n      Description: %s\n", cmd->getName(), cmd->getUsage(), cmd->getDescription());
    } else {
      bool found = false;
      Command *cmd = Commands::getCommand(args[1], &found);
      if (!found) {
        printf("Command `%s` not found!\n", args[1]);
      } else
        printf("`%s`:\n  Usage: %s\n  Description: %s\n", cmd->getName(), cmd->getUsage(), cmd->getDescription());
    }
  }

  virtual bool check(char **args, int argSize) override {
    return argSize > 0 && argSize <= 2;
  }

  virtual const char *getDescription() override {
    return "Sends help.";
  }
};

class ClearCommand : public Command {
public:
  ClearCommand():
    Command("clear", "clear")
  {}

  ~ClearCommand() {

  }

  virtual void call(char **args, int argSize) override {
    system("clear");
  }

  virtual bool check(char **args, int argSize) override {
    return argSize > 0;
  }

  virtual const char *getDescription() override {
    return "Clears console.";
  }
};

void Stop();
class QuitCommand : public Command {
public:
  QuitCommand():
    Command("quit", "quit")
  {}

  ~QuitCommand() {

  }

  virtual void call(char **args, int argSize) override {
    Stop();
  }

  virtual bool check(char **args, int argSize) override {
    return argSize > 0;
  }

  virtual const char *getDescription() override {
    return "Stops this program.";
  }
};

class CreateCommand : public Command {
public:
  CreateCommand():
    Command("create", "create <dir> <name...>")
  {}

  ~CreateCommand() {

  }

  virtual void call(char **args, int argSize) override {
    std::string dir = std::string(args[1]);
    std::string name = "";
    for (int i = 2; i < argSize; ++i) {
      if (i > 2) name += " ";
      name += args[i];
    }

    SpyingObject *obj = new SpyingObject((char*)name.c_str(), (char*)dir.c_str());
    obj->save();
    Objects::refresh();
  }

  virtual bool check(char **args, int argSize) override {
    return argSize >= 3;
  }

  virtual const char *getDescription() override {
    return "Creates new spying object.";
  }
};

class UpdateCommand : public Command {
public:
  UpdateCommand():
    Command("update", "update <name...>")
  {}

  ~UpdateCommand() {

  }

  virtual void call(char **args, int argSize) override {
    std::string name = "";
    for (int i = 1; i < argSize; ++i) {
      if (i > 1) name += " ";
      name += args[i];
    }

    Objects::update((char*)name.c_str());
  }

  virtual bool check(char **args, int argSize) override {
    return argSize > 1;
  }

  virtual const char *getDescription() override {
    return "Opens update editor for selected object.";
  }
};

class DeleteCommand : public Command {
public:
  DeleteCommand():
    Command("del", "del <name...>")
  {}

  ~DeleteCommand() {

  }

  virtual void call(char **args, int argSize) override {
    std::string name = "";
    for (int i = 1; i < argSize; ++i) {
      if (i > 1) name += " ";
      name += args[i];
    }

    Objects::removeObject((char*)name.c_str());
    Objects::save();
    Objects::refresh();
  }

  virtual bool check(char **args, int argSize) override {
    return argSize > 1;
  }

  virtual const char *getDescription() override {
    return "Opens update editor for selected object.";
  }
};

void Refresh();
class RefreshCommand : public Command {
public:
  RefreshCommand():
    Command("refresh", "refresh")
  {}

  ~RefreshCommand() {

  }

  virtual void call(char **args, int argSize) override {
    Refresh();
  }

  virtual bool check(char **args, int argSize) override {
    return argSize == 1;
  }

  virtual const char *getDescription() override {
    return "Updates state of application.";
  }
};

class ListCommand : public Command {
public:
  ListCommand():
    Command("list", "list")
  {}

  ~ListCommand() {

  }

  virtual void call(char **args, int argSize) override {
    printf("Object list:\n");
    for (auto obj : Objects::getObjects()) 
      printf("  - \"%s\": Directory: \"%s\"\n", obj->getName(), obj->getDir());
  }

  virtual bool check(char **args, int argSize) override {
    return argSize == 1;
  }

  virtual const char *getDescription() override {
    return "Displays list of every spying object.";
  }
};

#endif