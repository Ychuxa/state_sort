#include <stdio.h>
#include <stdlib.h>

#define MAX_ENTRIES 100

struct Record {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int status;
    int code;
};

void printRecord(struct Record *record) {
    printf("%d-%02d-%02d %02d:%02d:%02d | Status: %d | Code: %d\n", record->year, record->month, record->day,
           record->hour, record->minute, record->second, record->status, record->code);
}

void printFileContent(FILE *file) {
    struct Record record;

    rewind(file);

    while (fread(&record, sizeof(struct Record), 1, file) == 1) {
        printRecord(&record);
    }
}

void sortFile(FILE *file) {
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    long numRecords = fileSize / sizeof(struct Record);

    if (numRecords <= 1) {
        return;
    }

    struct Record *records = malloc(fileSize);

    rewind(file);
    fread(records, sizeof(struct Record), numRecords, file);

    for (long i = 0; i < numRecords - 1; i++) {
        for (long j = 0; j < numRecords - i - 1; j++) {
            if (records[j].year > records[j + 1].year ||
                (records[j].year == records[j + 1].year && records[j].month > records[j + 1].month) ||
                (records[j].year == records[j + 1].year && records[j].month == records[j + 1].month &&
                 records[j].day > records[j + 1].day) ||
                (records[j].year == records[j + 1].year && records[j].month == records[j + 1].month &&
                 records[j].day == records[j + 1].day && records[j].hour > records[j + 1].hour) ||
                (records[j].year == records[j + 1].year && records[j].month == records[j + 1].month &&
                 records[j].day == records[j + 1].day && records[j].hour == records[j + 1].hour &&
                 records[j].minute > records[j + 1].minute) ||
                (records[j].year == records[j + 1].year && records[j].month == records[j + 1].month &&
                 records[j].day == records[j + 1].day && records[j].hour == records[j + 1].hour &&
                 records[j].minute == records[j + 1].minute && records[j].second > records[j + 1].second)) {
                struct Record temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }

    rewind(file);
    fwrite(records, sizeof(struct Record), numRecords, file);

    free(records);
}

void addRecord(FILE *file, struct Record *newRecord) {
    fseek(file, 0, SEEK_END);
    fwrite(newRecord, sizeof(struct Record), 1, file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb+");

    if (file == NULL) {
        printf("n/a\n");
        return 1;
    }

    int choice;
    struct Record newRecord;

    do {
        printf("\nMenu:\n");
        printf("0. Print File Content\n");
        printf("1. Sort and Print\n");
        printf("2. Add Record, Sort, and Print\n");
        printf("-1. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                printFileContent(file);
                break;
            case 1:
                sortFile(file);
                printFileContent(file);
                break;
            case 2:
                printf("Enter year, month, day, hour, minute, second, status, code: ");
                scanf("%d %d %d %d %d %d %d %d", &newRecord.year, &newRecord.month, &newRecord.day,
                      &newRecord.hour, &newRecord.minute, &newRecord.second, &newRecord.status,
                      &newRecord.code);
                addRecord(file, &newRecord);
                sortFile(file);
                printFileContent(file);
                break;
            case -1:
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != -1);

    fclose(file);

    return 0;
}
