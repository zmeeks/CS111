#!/bin/sh

tr [:blank:] '\n' < a0.txt | sort | tr 'k' '\n' | sort | rev | wc > file1.txt

times

