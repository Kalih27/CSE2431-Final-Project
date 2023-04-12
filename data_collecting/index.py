# /********* index.py ***********/
# https://psutil.readthedocs.io/en/latest/#processes
import csv
import psutil
import os
from sys import platform

processes = []

def get_processes_to_csv(csv_path):
    # Get all running processes
    processes = psutil.process_iter()

    # Create a CSV file and write the header row
    with open(csv_path, mode='w', newline='') as csv_file:
        fieldnames = ['pid', 'name', 'username', 'num_ctx_switches', 'memory_info', 'cpu_percent',
                      'memory_percent', 'nice', 'create_time', 'utime', 'stime', 'cutime', 'cstime']
        writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
        writer.writeheader()

        # Write each process's data to the CSV file
        for process in processes:
            try:
                process_data = process.as_dict(
                    attrs=['pid', 'name', 'username', 'num_ctx_switches', 'memory_info', 'cpu_percent', 'memory_percent', 'nice', 'create_time'])
                utime = process.cpu_times().user
                stime = process.cpu_times().system
                cutime = process.cpu_times().children_user
                cstime = process.cpu_times().children_system
                process_data['utime'] = utime
                process_data['stime'] = stime
                process_data['cutime'] = cutime
                process_data['cstime'] = cstime
            except psutil.NoSuchProcess:
                # Process may have terminated since we got the process list
                continue

            # Adding boolean col which represents original value as missing or not along with imputed column
            # for col in process_data:
            #     if process_data[col] is None:
            #         process_data[col+"_missing"] = 1
            
            writer.writerow(process_data)

file_name = platform + "_processes.csv"
print("You are currently loading on " + platform + " platform.")
get_processes_to_csv(file_name)
