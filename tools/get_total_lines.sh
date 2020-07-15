#!/bin/bash

find src -name "*.cpp" -or -name "*.h" | xargs wc -l
