# /********* index.py ***********/
# https://psutil.readthedocs.io/en/latest/#processes
import csv
import psutil
import os
from sys import platform
import threading
import time


def write_header_to_csv(csv_path):
    with open(csv_path, mode='w', newline='') as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
        writer.writeheader()


def get_processes_to_csv(csv_path):
    # Get all running processes
    processes = psutil.process_iter()

    # Create a CSV file and write the header row
    with open(csv_path, mode='a', newline='') as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=fieldnames)

        # Write each process's data to the CSV file
        for process in processes:
            try:
                process_data = process.as_dict(
                    attrs=['pid', 'name', 'username', 'memory_percent', 'cpu_percent', 'nice', 'create_time'])
                mem_rss = process.memory_info().rss
                mem_vms = process.memory_info().vms
                mem_pfaults = process.memory_info().pfaults
                mem_pageins = process.memory_info().pageins
                process_data['mem_rss'] = mem_rss
                process_data['mem_vms'] = mem_vms
                process_data['mem_pfaults'] = mem_pfaults
                process_data['mem_pageins'] = mem_pageins

                num_voluntary = process.num_ctx_switches().voluntary
                num_involuntary = process.num_ctx_switches().involuntary
                process_data['num_ctx_switches_voluntary'] = num_voluntary
                process_data['num_ctx_switches_involuntary'] = num_involuntary

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


def action():
    print(
        'action to get processes to csv -> time : {:.1f}s'.format(time.time()-start_time))
    get_processes_to_csv(file_name)


class set_interval:
    def __init__(self, interval, action):
        self.interval = interval
        self.action = action
        self.stopEvent = threading.Event()
        thread = threading.Thread(target=self.__set_interval)
        thread.start()

    def __set_interval(self):
        nextTime = time.time()+self.interval
        while not self.stopEvent.wait(nextTime-time.time()):
            nextTime += self.interval
            self.action()

    def cancel(self):
        self.stopEvent.set()


# initial
processes = []
file_name = platform + "_processes.csv"
start_time = time.time()
print_attributes = True
fieldnames = [
    'pid',
    'name',
    'username',
    'memory_percent',
    'cpu_percent',
    'nice',
    'create_time',
    'mem_rss',
    'mem_vms',
    'mem_pfaults',
    'mem_pageins',
    'num_ctx_switches_voluntary',
    'num_ctx_switches_involuntary',
    'utime',
    'stime',
    'cutime',
    'cstime'
]

#
print("You are currently loading on " + platform + " platform.")

#
inter = input(
    "Enter a positive integer for interval to get process in seconds: ")
stop = input(
    "Enter a positive integer for interval to stop in seconds (>= first input): ")

#
write_header_to_csv(file_name)

# start action every `inter` seconds
interval = set_interval(int(inter), action)
print(
    'just after set_interval -> time : {:.1f}s'.format(time.time()-start_time))

# stop interval in `stop` seconds
t = threading.Timer(int(stop), interval.cancel)
t.start()
