/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#pragma once

#include <unordered_set>
#include <queue>

#include "utils/args.h"
#include "dict.h"
#include "matrix.h"
#include "parser.h"
#include "doc_parser.h"
#include "model.h"
#include "utils/utils.h"
#include "utils/simple_matrix.h"

namespace starspace {

typedef std::pair<Real, int32_t> Predictions;

class StarSpace {
  public:
    explicit StarSpace(std::shared_ptr<Args> args);

    void init();
    void initFromTsv(const std::string& filename);
    // TODO(zhezhaoxu) return bool
    void initFromSavedModel(const std::string& filename);

    void train();
    void evaluate();

    MatrixRow getNgramVector(const std::string& phrase);
    Matrix<Real> getDocVector(
        const std::string& line,
        const std::string& sep = " \t");
    void parseDoc(
        const std::string& line,
        std::vector<Base>& ids,
        const std::string& sep);
    void parseForCF(
        const std::string& line,
        std::vector<Base>& ids,
        const std::string& sep);
    void parseForCF(
        const std::vector<std::string> &tokens,
        std::vector<Base>& ids);

    void nearestNeighbor(const std::string& line, int k);

    void saveModel(const std::string& filename);
    void saveModelTsv(const std::string& filename);
    void printDoc(std::ostream& ofs, const std::vector<Base>& tokens);

    const std::string kMagic = "STARSPACE-2017-2";

    void loadBaseDocs();
    void initBaseDocSimpleMatrix();

    void predictOne(
        const std::vector<Base>& input,
        std::vector<Predictions>& pred);
    void getScoresForCF(
        Matrix<Real> &lhsM,
        std::priority_queue<Predictions> &heap);
    // same as getScoresForCF, but use openblas
    void fastGetScoresForCF(
        const Matrix<Real> &lhsM,
        int64_t N,
        std::priority_queue<Predictions> &heap);
    void predictOneForCF(
        const std::vector<Base>& input,
        std::vector<Predictions>& pred,
        const std::unordered_set<int>& banSet);

    std::shared_ptr<Args> args_;
    std::vector<std::vector<Base>> baseDocs_;
    std::shared_ptr<Dictionary> dict_;
  private:
    void initParser();
    void initDataHandler();
    std::shared_ptr<InternDataHandler> initData();
    Metrics evaluateOne(
        const std::vector<Base>& lhs,
        const std::vector<Base>& rhs,
        std::vector<Predictions>& pred);

    std::shared_ptr<DataParser> parser_;
    std::shared_ptr<InternDataHandler> trainData_;
    std::shared_ptr<InternDataHandler> validData_;
    std::shared_ptr<InternDataHandler> testData_;
    std::shared_ptr<EmbedModel> model_;

    std::vector<Matrix<Real>> baseDocVectors_;
    std::shared_ptr<SimpleMatrix> baseDocSimpleMatrix_;
};

}
