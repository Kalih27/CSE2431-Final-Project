/********* index.c ***********/
// Export virtual file into actual file
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#define ACTUAL_FILE_PATH "linux_log_file.csv"
#define PROC_FILE_PATH "/proc/log_file"
#define BUFFER_SIZE 32768
#define WHITE_SPACE " "
#define COMMA ","
#define NEXT_LINE "\n"

int complete_time;
FILE *fp, *outfp;

// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
char *trim_white_space(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

// Output data into csv file
// Each column separated by comma in cse file
void output_log_file()
{
    char buffer[BUFFER_SIZE];
    ssize_t read;
    char *line = NULL;
    size_t len = 0;
    int start_time = 1;
    char *token;

    // Loop indefinitely to continuously read from the file
    while (start_time <= complete_time)
    {
        printf("Current time: %d \n", start_time);

        // Read a line from the file
        line = fgets(buffer, BUFFER_SIZE, fp);
        char updated_buffer[BUFFER_SIZE];
        // printf("Current buffer: %s \n", buffer);
        // printf("Current line: %s \n", line);

        if (line != NULL)
        {
            token = strtok(buffer, WHITE_SPACE);

            while (token != NULL)
            {
                strcat(updated_buffer, trim_white_space(token));
                token = strtok(NULL, WHITE_SPACE);
                if (token != NULL)
                {
                    strcat(updated_buffer, COMMA);
                }
                else
                {
                    strcat(updated_buffer, NEXT_LINE);
                    break;
                }
            }

            fprintf(outfp, "%s \n", updated_buffer);
        }
        else
        {
            // No new data in the file yet
            continue;
        }

        memset(updated_buffer, 0, BUFFER_SIZE);
        sleep(1);
        start_time++;
    }
}

int main()
{
    char filename[] = PROC_FILE_PATH;
    char output_filename[] = ACTUAL_FILE_PATH;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("fopen");
        return 1;
    }

    outfp = fopen(output_filename, "w+");
    if (outfp == NULL)
    {
        perror("fopen");
        fclose(fp);
        return 1;
    }

    printf("Enter complete time in seconds (any positive integer): ");
    scanf("%d", &complete_time);
    if (complete_time <= 0)
    {
        perror("Invalid Input Of Number Of Process");
        return 1;
    }

    output_log_file();

    fclose(fp);
    fclose(outfp);

    return 0;
}