#include <iostream>
#include <string>
#include <vector>

#include <sstream>
#include <fstream>

#include <cstring>

#include <filesystem>
namespace fs = std::filesystem;

#include "options.hpp"

static Options *sOptions = nullptr;

#include "object.hpp"
#include "commands.hpp"

static bool sRunning = true;

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch) {
  size_t pos = txt.find( ch );
  size_t initialPos = 0;
  strs.clear();

  while(pos != std::string::npos) {
    strs.push_back( txt.substr( initialPos, pos - initialPos ) );
    initialPos = pos + 1;

    pos = txt.find( ch, initialPos );
  }

  strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

  return strs.size();
}

void cmd() {
  printf("> ");
  char cmdBuf[512] = {0};
  fgets(cmdBuf, 512, stdin);

  size_t len = strlen(cmdBuf);
  if (len > 0 && cmdBuf[len - 1] == '\n') {
    cmdBuf[len - 1] = '\0';
  }

  std::vector<std::string> args{};
  (void)split(std::string(cmdBuf), args, ' ');
  std::vector<char *> arguments{};
  for (auto &arg : args) arguments.push_back(&arg[0]);

  Commands::call(arguments.data(), (int)arguments.size());
}

void InitCommands() {
  Commands::initCommand(new HelpCommand());
  Commands::initCommand(new ClearCommand());
  Commands::initCommand(new QuitCommand());
  Commands::initCommand(new CreateCommand());
  Commands::initCommand(new UpdateCommand());
  Commands::initCommand(new RefreshCommand());
  Commands::initCommand(new ListCommand());
}

void Stop() { sRunning = false; }
void Refresh() {
  Objects::refresh();
}

int main(int argc, char **argv) {
  sOptions = ParseArguments(argc, argv);
  if (!sOptions) {
    fprintf(stderr, "Failed to parse arguments!\n");
    return 1;
  }
  Refresh();

  InitCommands();

  while (sRunning) {
    cmd();
  }

  return 0;
}