#!/bin/bash

VERSION=$(git describe --tags --abbrev=0 | sed 's/^v//')

sed -i "s/project(.* VERSION .*)/project(MyProject VERSION $VERSION)/" CMakeLists.txt

sed -i "s/PROJECT_NUMBER\s*=\s*.*/PROJECT_NUMBER = $VERSION/" config.doxy

doxygen config.doxy

git add CMakeLists.txt config.doxy
git commit -m "version: update to $VERSION"