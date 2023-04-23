# /********* index.py ***********/

""" Import library """
import pandas as pd
from datetime import datetime

from ml_algorithms import linear_regression, random_forest_regression
from data_preprocessing import data_preprocessing

# print('version: {}'.format(pd.__version__))

""" Load the csv file """
platform = input(
    "Enter the platform data to start (enter either `linux` or `macBook`): ")
if (platform == "macBook"):
    # pid,name,username,memory_percent,cpu_percent,nice,create_time,mem_rss,mem_vms,mem_pfaults,mem_pageins,num_ctx_switches_voluntary,num_ctx_switches_involuntary,utime,stime,cutime,cstime
    df = pd.read_csv('../data_collecting/darwin_processes.csv')
elif (platform == "linux"):
    # PID, NAME, ELAPSED_TIME, TOTAL_TIME, utime, stime, start_time, uptime
    df = pd.read_csv('../data_collecting/linux_log_file.csv')
else:
    print("Invalid input")
    exit()

""" Preprocess data """
print("Data preprocessing starts @ ", datetime.now())
updated_df = data_preprocessing(platform, df)
print("Data preprocessing finishes @ ", datetime.now())

""" Build the model """
model = input(
    "Enter the model to build (enter either `lr` for linear regression or `rfr` random forest regression): ")
print("Model starts building @ ", datetime.now())
if model == "lr":
    linear_regression(platform, updated_df)
elif model == "rfr":
    random_forest_regression(platform, updated_df)
else:
    print("Invalid input")
    exit()
print("Model finishs building @ ", datetime.now())
