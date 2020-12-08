#!/bin/bash

which leaks && exec leaks --atExit -- $@
which valgrind && exec valgrind --error-exitcode=1 --leak-check=yes $@
exit 1
