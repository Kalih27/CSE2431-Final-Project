
// ktime_t current_time = ktime_get();
// s64 current_time_ns = ktime_to_ns(current_time);
// long current_time_s = current_time_ns / 1000000000;
// start_time_s = current_time;
// unsigned long long cutime, cstime, start_time;

// unsigned long long utime_sec, stime_sec, start_time_sec;
// unsigned long long utime_msec, stime_msec, start_time_msec;
// unsigned long long total_time;
// long long cpu_usage = 0;
// long long elapsed_nsec;
// long long usage_nsec;
// long long usage_sec;
// unsigned long long utime, stime, cutime, cstime;

// static int clk_tck = 100;
// static int number_of_cpu = 2;

// // pointer for buffer location in read
// static char *buff_ptr;

// // Copy the virtual file's contents to the buffer
// ssize_t ret = kernel_read(virtual_file, buffer, PAGE_SIZE, &virtual_file->f_pos);

// Read data from the virtual file and write it to the actual file on disk
// while ((bytes_read = kernel_read(virtual_file, buffer, PAGE_SIZE, &virtual_file->f_pos)) > 0)
// {
//     ssize_t bytes_written = kernel_write(actual_file, buffer, bytes_read, &actual_file->f_pos);
//     if (bytes_written != bytes_read)
//     {
//         pr_err("Failed to write data to %s\n", ACTUAL_FILE_PATH);
//     }
// }
// // Open the virtual file
// virtual_file = filp_open(PROC_FILE_PATH, O_RDONLY, 0);
// if (IS_ERR(virtual_file))
// {
// 	pr_err("Failed to open virtual file\n");
// }

// char *temp = (char *)v;
// do
// {
// 	seq_putc(s, *temp);
// 	temp++;
// } while (*temp != '\n');
// seq_putc(s, '\n');