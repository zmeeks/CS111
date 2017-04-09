#!/bin/sh

tr [:blank:] '\n' < a0.txt | sort | rev | shuf | rev | wc > file2.txt

times


