# EXPERIMENT RUNNER

import argparse
import copy
import os
import subprocess
import shlex
import sys

EXEC = "./tester/main"

# LOOP THROUGH PARAMETERS

t = 8 # number of threads
s = 1 # time for each run
o = 1 # number of ops per txn
r = 1 # ratio of gets:puts (not used currently)

k = "uniform" # choose distribution

variables = []
labels = []
experiments = []

def add_variable(variable, vals):
    global labels
    global experiments
    new_labels = []
    variables.append(variable)
    new_experiments = []
    for i in range(0, len(experiments)):
        num_vals = 0
        for j in vals:
            num_vals += 1
            new_arg = copy.copy(experiments[i])
            new_arg[variable] = j
            new_experiments.append(new_arg)

            new_label = copy.copy(labels[i])
            new_label.append(j)
            new_labels.append(new_label)

    labels = new_labels
    experiments = new_experiments
    return num_vals

def experiment(num_vals):
    for i in range(0, len(labels), num_vals):
        print str(variables),
        for j in range(0, num_vals):
            print("\t%s" % labels[i+j]),
        print("")
        for cc in [ 'hle', 'rtm', 'tbl', 'spin', 'sspin', 'rwtbl']:
            results = []
            for j in range(0, num_vals):
                val = experiments[i+j]

                cmd = [ EXEC, '-s'+str(val['s']), '-t'+str(val['t']), '-o'+str(val['o']), '-k'+val['k'], '-e0', cc]
                #print str(cmd)
                task =  subprocess.Popen(cmd, 
                                         stdout=subprocess.PIPE,
                                         stderr=subprocess.PIPE
                                         )
                (out, err) = task.communicate()
                results.append(int(out) * int(val['o']) / int(val['s']))
                
            print("%s" % cc),
            for result in results:
                print("\t%s" % result),
            print("")

def convert(val, default):
    if val == "":
        return default
    elif ',' in val:
        val = eval(val)
        (start, end, step) = val
        return range(start, end, step)
    else:
        return [val]
        

def usage():
    print """experiment.py [--t [min,max,step]] [--o [min,max,step]]
   [--s secc] [--k]"""
    sys.exit(1)

if __name__ == "__main__":
    experiments.append({'s':s, 't':t, 'o':o, 'k':k})

    labels.append([])
    num_vals = 1

    for i in range(0, len(sys.argv)):
        arg = sys.argv[i]
        val = ""
        if i < len(sys.argv) - 1:
            if '--' not in sys.argv[i+1]:
                val = sys.argv[i+1]

        if arg == '--o' or arg == '--ops':
            num_vals = add_variable('o', convert(val, range(1, 10, 2)))
        elif arg == '--t' or arg == '--threads':
            num_vals = add_variable('t', convert(val, range(1, 10, 2)))
        elif arg == '--s' or arg == '--sec':
            num_vals = add_variable('s', convert(val, [val]))
        elif arg == '--k' or arg == '--key-dist':
            num_vals = add_variable('k', convert(val, ['uniform', 'zipf']))
        elif arg == '--r' or arg == '--ratio':
            num_vals = add_variable('r', convert(val, ['10000:1', '1:1', '1:10000']))
        elif arg == '--h' or arg == '--help':
            usage()

    experiment(num_vals)
