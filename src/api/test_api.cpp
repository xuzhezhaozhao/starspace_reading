
#include "starspace_api.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: <model> <k> <query ...>" << std::endl;
    exit(-1);
  }

  starspace::StarSpaceApi sp;
  sp.initFromSavedModel(argv[1]);
  sp.loadBaseDocs("");
  std::vector<std::string> query;
  int k = std::stoi(argv[2]);
  std::vector<std::pair<float, std::string>> result;
  for (int i = 3; i < argc; ++i) {
    query.push_back(argv[i]);
  }
  sp.predictOneForCF(query, k, result);
  for (auto &r: result) {
    std::cout << r.second << " : " << r.first << std::endl;
  }

  return 0;
}
