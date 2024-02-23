#ifndef ISE_COMMANDS_HPP_
#define ISE_COMMANDS_HPP_

#include "command.hpp"

class Commands {
public:
  static void call(char **args, int argSize) {
    if (argSize <= 0) 
    return;

    bool found = false;
    for (auto cmd : sCommands) {
      bool is = false;
      if (!(is = (strcmp(cmd->getName(), args[0]) == 0)) && cmd->getAliases().size() > 0) {
        auto aliases = cmd->getAliases();
        int alsIdx = 0;
        while (!is && alsIdx < aliases.size()) {
          char *alias = (char*)aliases[alsIdx];
          is = strcmp(alias, args[0]) == 0;
          alsIdx++;
        }
      }
      if (is) {
        found = true;
        if (cmd->check(args, argSize))
          cmd->call(args, argSize);
        else {
          printf("Invalid arguments! Usage: `%s`\n", cmd->getUsage());
        }
        break;
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
    Command("help", "[command]")
  {}

  ~HelpCommand() {

  }

  virtual void call(char **args, int argSize) override {
    printf("Help:\n");
    if (argSize == 1) {
      printf("  Startup info:\n    - Data Dir: %s\n", sOptions->DataDir);
      printf("  Commands:\n");
      for (auto cmd : Commands::getCommands()) {
        printf("    - `%s`:\n", cmd->getName());
        auto usage = cmd->getUsage();
        printf("      Usage: `%s`\n", usage.c_str());
        printf("      Description: %s\n", cmd->getDescription());;
      }
    } else {
      bool found = false;
      Command *cmd = Commands::getCommand(args[1], &found);
      if (!found)
        printf("Command `%s` not found!\n", args[1]);
      else {
        auto usage = cmd->getUsage();
        printf("`%s`:\n  Usage: `%s`\n  Description: %s\n", cmd->getName(), usage.c_str(), cmd->getDescription());
        printf("  Aliases:\n");
        for (auto als : cmd->getAliases()) printf("    - %s\n", als);
      }
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
    Command("clear", "", { "cls" })
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
    Command("exit", "", { "quit" })
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
    Command("create", "<dir> <name...>")
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
    Command("upd", "<name...>", { "update" })
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
    Command("del", "<name...>", { "delete" })
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
    Command("refresh", "")
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
    Command("list", "")
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