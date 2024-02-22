#ifndef ISE_COMMAND_HPP_
#define ISE_COMMAND_HPP_

class Command {
public:
  Command(const char *name, const char *usage):
    mName(name), mUsage(usage)
  {}

  ~Command() {
    free((void*)mName);
    free((void*)mUsage);
  }

  virtual void call (char **args, int argSize) = 0;
  virtual bool check(char **args, int argSize) = 0;

  const char *getName() { return mName; }
  const char *getUsage() { return mUsage; }
  virtual const char *getDescription() = 0;
private:
  const char  *mName;
  const char  *mUsage;
};

#endif