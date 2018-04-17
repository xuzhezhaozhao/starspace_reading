#pragma once

#include <string>
#include <vector>

#include "../starspace.h"

namespace starspace {

class StarSpaceApi {
 public:
  StarSpaceApi();

  bool initFromSavedModel(const std::string &model);
  bool loadBaseDocs(const std::string &basedoc);

  void predictOneForCF(
      const std::vector<std::string> &query,
      int k,
      std::vector<std::pair<Real, std::string>> &result);

 private:
  StarSpace sp_;
};

}  // starspace
