#!/usr/bin/env bash
#
# Copyright (c) 2016-present, Facebook, Inc.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.
#

DATASET=(
  wikipedia
)

MODELDIR=data/starspace/models
DATADIR=data/starspace/data

mkdir -p "${MODELDIR}"
mkdir -p "${DATADIR}"

if [ ! -f "${DATADIR}/${DATASET[i]}_shuf_train5M.txt" ]
then
    echo "Downloading wikipedia train data"
    wget -c "https://s3.amazonaws.com/fair-data/starspace/wikipedia_train5M.tgz" -O "${DATADIR}/${DATASET[0]}_train.tar.gz"
    tar -xzvf "${DATADIR}/${DATASET[0]}_train.tar.gz" -C "${DATADIR}"
  fi

echo "Compiling StarSpace"

make

echo "Start to train on wikipedia data (meant to replicate experiment from paper, this will take a while to train):"

./starspace train \
  -trainFile "${DATADIR}"/wikipedia_shuf_train5M.txt \
  -model "${MODELDIR}"/wikipedia_article_search_full \
  -trainMode 2 \
  -initRandSd 0.01 \
  -adagrad true \
  -ngrams 1 \
  -lr 0.05 \
  -margin 0.05 \
  -epoch 20 \
  -thread 40 \
  -dim 300 \
  -negSearchLimit 100 \
  -dropoutRHS 0.8 \
  -fileFormat labelDoc \
  -similarity "cosine" \
  -minCount 5 \
  -normalizeText true \
  -verbose true

if [ ! -f "${DATADIR}/${DATASET[i]}_test10k.txt" ]
then
    echo "Downloading wikipedia test data"
    wget -c "https://s3.amazonaws.com/fair-data/starspace/wikipedia_devtst.tgz" -O "${DATADIR}/${DATASET[0]}_test.tar.gz"
    tar -xzvf "${DATADIR}/${DATASET[0]}_test.tar.gz" -C "${DATADIR}"
fi

echo "Start to evaluate trained model:"

./starspace test \
  -testFile "${DATADIR}"/wikipedia_test10k.txt \
  -basedoc "${DATADIR}"/wikipedia_test_basedocs.txt \
  -model "${MODELDIR}"/wikipedia_article_search_full \
  -thread 20 \
  -trainMode 2 \
  -normalizeText true \
  -verbose true
