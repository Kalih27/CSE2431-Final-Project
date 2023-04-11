/********* export_log_file.c ***********/
// Temporary file to export virtual file into actual file
#include <stdio.h>

int main()
{
    char filename[] = "/proc/log_file";
    char output_filename[] = "/home/vboxuser/Documents/linux_log_file.txt";
    FILE *fp, *outfp;
    char buffer[1024];
    size_t bytes_read;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("fopen");
        return 1;
    }

    outfp = fopen(output_filename, "w");
    if (outfp == NULL)
    {
        perror("fopen");
        fclose(fp);
        return 1;
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0)
    {
        fwrite(buffer, 1, bytes_read, outfp);
    }

    fclose(fp);
    fclose(outfp);

    return 0;
}