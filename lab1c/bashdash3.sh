#!/bin/sh


tr [:blank:] '\n' < a0.txt | sort | tr a-d A-D | tr C-G k-o | sort | rev  > fil$

times

