# Documentation:

 python scripts for Linux or Mac in this directory can be used to create new sensor data csv file from the one downloaded from beehive server. This scripts need python libraries of math, dataset, xlrd, argparse, datetime, sys, os, subprocess, shlex, numpy.

The
Documentation:

The bash shell scripts (for Linux or Mac OS X) in this directory can be used to create sensor reports in PDF for nodes - The scripts in this directory depend on wget, gunzip and gnuplot for downloading, extracting and plotting data. Please make sure they are installed in the system where they are intended to be used.

Usage:

$./report part_of_nodeid last_day how_many_days

Node IDs can be obtained from the Beehive Portal.

For example, to get reports on node 001E061130FE for 5 days ending today,

$./report 30FE 0 4

This will generate a report for each sensor in the plot folder. The files are named based on VSN start_time end_time and parameter.

For the same node the report for a week ending yesterday can be generated using -

$./report 30FE 1 6

To clean the directories:

$./clean

This will delete all the generated files.
```
python3 beehive_csv.py -n nodeID -ds start -de end

nodeID: last four digit of node id
start: 0 means collect data from today (0 day before today)
end: 1 means collect data to yesterday (1 day before today)
```

If you just excute ```python3 beehive_csv.py``` then all live nodes will be shown as a result.
