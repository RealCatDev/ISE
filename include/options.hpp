#ifndef ISE_OPTIONS_HPP_
#define ISE_OPTIONS_HPP_

struct Options {
  char *DataDir;
};

enum OptionMode {
  OPT_NONE = 0x0000,
  OPT_DATA = 0x0001,
};

void badOption(char *arg) {
  fprintf(stderr, "Unexpected option passed `%s`!\n", arg);
  exit(1);
}

void validateArg(char *arg) {
  if (!fs::exists(arg)) {
    fprintf(stderr, "`%s` is an invalid argument!\n", arg);
    exit(1);
  }
}

#define OPTION_ARG(mode, opt, str) if (strcmp(arg, str) == 0) { \
  if (mode != OPT_NONE) badOption(arg);                         \
  mode = opt;                                                   \
}

Options *ParseArguments(int argc, char **argv) {
  std::vector<char *> args(argv, argv + argc);
  args.erase(args.begin());
  Options *options = new Options();
  if (args.size() <= 1) {
    fprintf(stderr, "Option/s `-d` is/are required!\n");
    exit(1);
  }

  OptionMode mode = OPT_NONE;

  for (auto arg : args) {
    OPTION_ARG(mode, OPT_DATA, "-d")
    else {
      if (mode == OPT_NONE) badOption(arg);
      validateArg(arg);
      if (mode == OPT_DATA)  options->DataDir  = arg;
      mode = OPT_NONE;
    }
  }

  if (!options->DataDir) {
    fprintf(stderr, "Option `-d` (Data dir) is required!\n");
    exit(1);
  }

  {
    int len = strlen(options->DataDir);
    if (options->DataDir[len-1] != '/') {
      options->DataDir = (char*)realloc(options->DataDir, len+2);
      options->DataDir[len] = '/';
      options->DataDir[len+1] = '\0';
    }
  }

  return options;
}

#endif