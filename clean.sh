#!/bin/bash

rm *.*~
rm viennadata/*.*~
rm tests/*.*~
rm doc/*.*~
rm doc/manual/*.*~
cd tests
./clean.sh
