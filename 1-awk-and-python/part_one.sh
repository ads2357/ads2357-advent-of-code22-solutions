#!/bin/bash

awk '$0~/[0-9]/{sum+=$0} $0~/^$/{if (sum > max) max=sum ; sum=0} END{print max}' < input
