#ifndef ISE_OBJECT_HPP_
#define ISE_OBJECT_HPP_

class SpyingObject {
public:
  SpyingObject(const char *name, const char *dir):
    mName(name), mDir(dir)
  {}

  ~SpyingObject() {

  }

  void update() {
    mDirOld = mDir;
    mDirOld = mDirOld.replace(0, std::string(sOptions->DataDir).length(), "");
    char buf[256] = {0};
    
    system("clear");
    printf("Name (%s): ", mName.c_str());
    fgets(buf, 256, stdin);
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
      buf[len - 1] = '\0';
    }

    if (buf[0] != '\0') mName = buf;

    printf("Directory (%s): ", mDir.c_str());
    fgets(buf, 256, stdin);
    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
      buf[len - 1] = '\0';
    }

    if (buf[0] != '\0') mDir = buf;
    save();
  }

  void save() {
    std::string filename(sOptions->DataDir);
    if (!filename.empty() && filename.back() != '/') filename.push_back('/');
    filename += "objects.ise";

    std::ifstream inFile(filename.c_str());

    std::string strTemp;
    std::ostringstream result{};
    bool found = false;
    std::string newDir = mDir;
    newDir = newDir.replace(0, std::string(sOptions->DataDir).length(), "");
    while(std::getline(inFile, strTemp)) {
      std::istringstream iss(strTemp);
      std::string dir;
      iss >> std::quoted(dir);
      if(dir == mDirOld){
        strTemp = "\""+newDir+"\": \""+mName+"\"";
        found = true;
      }
      result << strTemp << std::endl;
    }
    if (!found) result << "\""+newDir+"\": \""+mName+"\"";

    inFile.close();
    std::ofstream outFile(filename.c_str(), std::ios_base::trunc);
    outFile << result.str();
    outFile.close();
  }

  const char *getName() const { return mName.c_str(); }
  const char *getDir () const { return mDir.c_str() ; }
private:
  std::string mName;
  std::string mDir;
  std::string mDirOld;
};

class Objects {
public:
  static void addObject(SpyingObject *obj) { sObjects.push_back(obj); }
  static void removeObject(const char *name) {
    size_t i = 0, found = sObjects.size();
    for (auto obj : sObjects) {
      if (strcmp(obj->getName(), name) == 0)
      { found = i; break; }
      i++;
    }
    if (found >= 0 && found < sObjects.size()) sObjects.erase(sObjects.begin() + found);
  }

  static void refresh() {
    std::string dir(sOptions->DataDir);
    if (!dir.empty() && dir.back() != '/') dir.push_back('/');
    dir += "objects.ise";
    std::ifstream file(dir.c_str(), std::ios_base::in);
    if (!file.good()) {
      fprintf(stderr, "Failed to refresh objects! (File \"%s\" can't be opened).\n", dir.c_str());
      return;
    }

    sObjects.clear();

    std::string line{};
    while (std::getline(file, line)) {
      std::stringstream ss(line);
      char delimiter;
      std::string dir, name;
      ss >> std::quoted(dir) >> delimiter >> std::quoted(name) >> delimiter;
      dir = std::string(sOptions->DataDir) + dir;
      SpyingObject *obj = new SpyingObject((char*)name.c_str(), (char*)dir.c_str());
      sObjects.push_back(obj);
    }

    file.close();
  }

  static void update(const char *name) {
    bool found = false;
    SpyingObject *obj = getObject(name, &found);
    if (!found) {
      printf("Couldn't find `%s`!\n", name);
      return;
    }
    obj->update();
  }

  static void save() {
    std::string filename(sOptions->DataDir);
    filename += "objects.ise";
    std::ostringstream result{};
    for (auto obj : sObjects) {
      std::string newDir = obj->getDir();
      newDir = newDir.replace(0, std::string(sOptions->DataDir).length(), "");
      result << "\"" << newDir << "\": \"" << obj->getName() << "\"" << std::endl;
    }

    std::ofstream file(filename.c_str(), std::ios_base::trunc);
    file << result.str();
    file.close();
  }

  static SpyingObject *getObject(const char *name, bool *found) {
    for (auto obj : sObjects) if (strcmp(name, obj->getName()) == 0) 
      return (*found = true, obj);
    return nullptr;
  }

  static std::vector<SpyingObject*> getObjects() { return sObjects; }
private:
  inline static std::vector<SpyingObject*> sObjects{};
};

#endif