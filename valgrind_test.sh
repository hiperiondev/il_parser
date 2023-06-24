#! /bin/bash

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose Release/IL_parser_new
rm -f vgcore.*
