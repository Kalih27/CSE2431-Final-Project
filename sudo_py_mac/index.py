# https://psutil.readthedocs.io/en/latest/#processes
import csv
import psutil
import os

processes = []

def get_processes_to_csv(csv_path):
    # Get all running processes
    processes = psutil.process_iter()

    # Create a CSV file and write the header row
    with open(csv_path, mode='w', newline='') as csv_file:
        fieldnames = ['pid', 'name', 'username', 'cpu_percent', 'memory_percent', 'utime', 'stime', 'cutime', 'cstime']
        writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
        writer.writeheader()

        # Write each process's data to the CSV file
        for process in processes:
            try:
                process_data = process.as_dict(attrs=['pid', 'name', 'username', 'cpu_percent', 'memory_percent'])
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

            writer.writerow(process_data)

get_processes_to_csv('data/mac_processes.csv')