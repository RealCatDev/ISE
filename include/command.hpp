#ifndef ISE_COMMAND_HPP_
#define ISE_COMMAND_HPP_

class Command {
public:
  Command(const char *name, const char *usage, std::vector<const char*> aliases = {}):
    mName(name), mUsage(usage), mAliases(aliases)
  {}

  ~Command() {
    free((void*)mName);
    free((void*)mUsage);
  }

  virtual void call (char **args, int argSize) = 0;
  virtual bool check(char **args, int argSize) = 0;

  const char              *getName()    { return mName;    }
  std::vector<const char*> getAliases() { return mAliases; }
  virtual const char *getDescription() = 0;

  std::string getUsage() { 
    std::ostringstream usageStr = {};
    if (mAliases.size() == 0) usageStr << mName;
    else {
      usageStr << "<" << mName;
      for (auto als : mAliases) usageStr << "|" << als;
      usageStr << ">";
    }
    if (mUsage && strlen(mUsage) > 0) usageStr << " " << mUsage;
    return usageStr.str();
  }
private:
  const char              *mName;
  const char              *mUsage;
  std::vector<const char*> mAliases;
};

#endif