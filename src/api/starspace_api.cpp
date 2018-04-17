
#include "starspace_api.h"

#include <memory>

namespace starspace {

StarSpaceApi::StarSpaceApi() : sp_(std::make_shared<Args>()) {}

bool StarSpaceApi::initFromSavedModel(const std::string &model) {
  sp_.args_->model = model;
  sp_.args_->fileFormat = "fastText";
  sp_.args_->dropoutLHS = 0.0;
  sp_.args_->dropoutRHS = 0.0;

  sp_.initFromSavedModel(model);

  return true;
}

bool StarSpaceApi::loadBaseDocs(const std::string &basedoc) {

  return true;
}

void StarSpaceApi::predictOneForCF(
    const std::vector<std::string> &query,
    int k,
    std::vector<std::pair<Real, std::string>> &result) {
  sp_.args_->K = k;
  std::vector<Base> query_vec;
  sp_.parseForCF(query, query_vec);
  std::unordered_set<int> banSet;
  banSet.clear();
  for (auto &p: query_vec) {
    banSet.insert(p.first);
  }
  std::vector<Predictions> predictions;
  sp_.predictOneForCF(query_vec, predictions, banSet);

  for (auto &p : predictions) {
    int id = sp_.baseDocs_[p.second][0].first;
    std::string symbol = sp_.dict_->getSymbol(id);
    result.push_back( {p.first, symbol} );
  }
}

}
