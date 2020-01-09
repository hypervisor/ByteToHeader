#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//
// Change this if you want more/less bytes per line.
//

#define MAX_BYTES_PER_LINE 8

int main(int argc, char **argv)
{
    FILE *fp;
    long len;
    char *buf;
    clock_t start, end;
    double diff;

    if (argc < 3) {
        printf("Incorrect usage!\n");
        return 1;
    }

    start = clock();
    
    //
    // Read file to buffer.
    //

    fp = fopen(argv[1], "rb");
    if (!fp) {
        printf("Failed to open file \"%s\"!\n", argv[1]);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    buf = malloc(len);
    fread(buf, 1, len, fp);
    fclose(fp);

    //
    // Create file to write to.
    //

    fp = fopen(argv[2], "w");
    if (!fp) {
        printf("Failed to open file \"%s\"!\n", argv[2]);
        return 1;
    }

    long lines;     // We only want to print x amount of bytes per line
    char *arrname;  // Name of array, if argument used

    lines = len / MAX_BYTES_PER_LINE;

    if (argc > 3) {
        arrname = argv[3];
    } else {
        arrname = "bFileArray";
    }

    printf("File size: 0x%lX\n", len);

    //
    // Now we print the bytes, first print the standard header stuff.
    // Then we loop through each line and print MAX_BYTES_PER_LINE per line.
    // This code is somewhat obscure and there are definitely better ways
    // to do this, but it is insignificant.
    //

    fprintf(fp, "#pragma once\n\n// This file was auto-generated.\n\nunsigned char %s[0x%lX] = {\n    ", arrname, len);

    for (long i = 0; i < lines; ++i) {
        const char last_line = i == lines - 1;
        for (unsigned h = 0; h < MAX_BYTES_PER_LINE; ++h) {
            const char last_byte = h == MAX_BYTES_PER_LINE - 1;
            const long cur_byte = h + i * MAX_BYTES_PER_LINE;
            fprintf(fp, "0x%02X", buf[cur_byte]);
            if (last_line && last_byte)
                goto done;
            fprintf(fp, ", ");
            if (!last_line && last_byte)
                fprintf(fp, "\n    ");
        }
    }

done:
    fprintf(fp, "\n};");

    end = clock();
    diff = (double)(end - start);
    double time = diff / CLOCKS_PER_SEC;

    free(buf);
    fclose(fp);

    printf("Done! (%fs)\n", time);
}