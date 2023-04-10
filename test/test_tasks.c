#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void print_task_info(char *task_name, int pid, int ppid)
{
	printf("Task: %s, PID: %d, Parent PID: %d\n", task_name, pid, ppid);
}

void print_tasks()
{
	DIR *dp;
	struct dirent *dirp;
	char task_path[1024], task_name[1024], stat_path[1024], buf[1024];
	int pid, ppid;

	dp = opendir("/proc/");
	if (dp == NULL)
	{
		perror("opendir");
		exit(1);
	}

	while ((dirp = readdir(dp)) != NULL)
	{
		if (dirp->d_type == DT_DIR && strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0)
		{
			pid = atoi(dirp->d_name);
			sprintf(task_path, "/proc/%d/exe", pid);
			memset(task_name, 0, sizeof(task_name));
			readlink(task_path, task_name, sizeof(task_name) - 1);
			sprintf(stat_path, "/proc/%d/stat", pid);
			FILE *fp = fopen(stat_path, "r");
			if (fp)
			{
				fgets(buf, sizeof(buf), fp);
				sscanf(buf, "%d %s %*c %d", &pid, task_name, &ppid);
				fclose(fp);
				print_task_info(task_name, pid, ppid);
			}
		}
	}
	closedir(dp);
}

int main()
{
	print_tasks();
	return 0;
}
