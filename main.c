// Created by: Dinushan Vimukthi
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int totalRAM;
    int usedRAM;
    int totalBandwidth;
    int usedBandwidth;
    int totalStorage;
    int usedStorage;
    float cpuUtilization;
} HostDetails;

typedef struct {
    char *hostname;
    HostDetails details;
} Host;

typedef struct {
    Host *hosts;
    size_t size;
    size_t capacity;
} HostMap;

#define INT_MAX 2147483647


#define CPU_UTILIZATION_WEIGHT 0.25
#define RAM_WEIGHT 0.25
#define BANDWIDTH_WEIGHT 0.25
#define STORAGE_WEIGHT 0.25



void initHostMap(HostMap *map, size_t capacity) {
    map->hosts = (Host *)malloc(capacity * sizeof(Host));
    map->size = 0;
    map->capacity = capacity;
}

void addHost(HostMap *map, const char *hostname, HostDetails details) {
    if (map->size >= map->capacity) {
        map->capacity *= 2;
        map->hosts = (Host *)realloc(map->hosts, map->capacity * sizeof(Host));
    }
    map->hosts[map->size].hostname = strdup(hostname);
    map->hosts[map->size].details = details;
    map->size++;
}

void freeHostMap(HostMap *map) {
    for (size_t i = 0; i < map->size; i++) {
        free(map->hosts[i].hostname);
    }
    free(map->hosts);
}

void initializeHostMapWithValues(HostMap *map) {
    HostDetails details1 = {16000, 8000, 1000, 500, 2000, 1000, 75.5};
    HostDetails details2 = {32000, 16000, 2000, 1000, 4000, 2000, 60.0};
    HostDetails details3 = {64000, 32000, 4000, 2000, 8000, 4000, 85.0};

    addHost(map, "host1", details1);
    addHost(map, "host2", details2);
    addHost(map, "host3", details3);
}

bool isSuitableHost(Host *host, int requiredRAM, int requiredBandwidth, int requiredStorage, float requiredCPUUtilization) {
    return host->details.totalRAM - host->details.usedRAM >= requiredRAM &&
           host->details.totalBandwidth - host->details.usedBandwidth >= requiredBandwidth &&
           host->details.totalStorage - host->details.usedStorage >= requiredStorage &&
           host->details.cpuUtilization + requiredCPUUtilization <= 100.0;
}

printAllSuitableHostsWastage(HostMap *map, int requiredRAM, int requiredBandwidth, int requiredStorage, float requiredCPUUtilization) {
    // Define array of suitable hosts and initialize with NULL
    Host *suitableHosts[map->size];
    for (size_t i = 0; i < map->size; i++) {
        suitableHosts[i] = NULL; // Initialize all pointers to NULL
    }

    // Iterate through all hosts and check if they are suitable
    size_t suitableHostCount = 0; // Counter for suitable hosts
    for (size_t i = 0; i < map->size; i++) {
        Host *host = &map->hosts[i];
        if (isSuitableHost(host, requiredRAM, requiredBandwidth, requiredStorage, requiredCPUUtilization)) {
            suitableHosts[suitableHostCount++] = host; // Add only valid hosts
        }
    }

    // Print all suitable hosts and their wastage
    for (size_t i = 0; i < suitableHostCount; i++) {
        Host *host = suitableHosts[i];
        if (host) { // Ensure the pointer is valid
            int ramWastage = host->details.totalRAM - host->details.usedRAM - requiredRAM;
            int bandwidthWastage = host->details.totalBandwidth - host->details.usedBandwidth - requiredBandwidth;
            int storageWastage = host->details.totalStorage - host->details.usedStorage - requiredStorage;
            float cpuUtilizationWastage = host->details.cpuUtilization + requiredCPUUtilization;

            printf("Host: %s\n", host->hostname);
            printf("RAM Wastage: %d\n", ramWastage);
            printf("Bandwidth Wastage: %d\n", bandwidthWastage);
            printf("Storage Wastage: %d\n", storageWastage);
            printf("CPU Utilization Wastage: %.2f\n", cpuUtilizationWastage);
            printf("\n");
        }
    }
}


void exportAllSuitableHostsWastageToCSV(HostMap *map, int requiredRAM, int requiredBandwidth, int requiredStorage, float requiredCPUUtilization, const char *filename) {
    // Define array of suitable hosts and initialize with NULL
    Host *suitableHosts[map->size];
    for (size_t i = 0; i < map->size; i++) {
        suitableHosts[i] = NULL; // Initialize all pointers to NULL
    }

    // Iterate through all hosts and check if they are suitable
    size_t suitableHostCount = 0; // Counter for suitable hosts
    for (size_t i = 0; i < map->size; i++) {
        Host *host = &map->hosts[i];
        if (isSuitableHost(host, requiredRAM, requiredBandwidth, requiredStorage, requiredCPUUtilization)) {
            suitableHosts[suitableHostCount++] = host; // Add only valid hosts
        }
    }

    // Open the CSV file for writing
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Write CSV header
    fprintf(file, "Hostname,RAM Wastage,Bandwidth Wastage,Storage Wastage,CPU Utilization Wastage\n");

    // Write data for each suitable host
    for (size_t i = 0; i < suitableHostCount; i++) {
        Host *host = suitableHosts[i];
        if (host) { // Ensure the pointer is valid
            int ramWastage = host->details.totalRAM - host->details.usedRAM - requiredRAM;
            int bandwidthWastage = host->details.totalBandwidth - host->details.usedBandwidth - requiredBandwidth;
            int storageWastage = host->details.totalStorage - host->details.usedStorage - requiredStorage;
            float cpuUtilizationWastage = host->details.cpuUtilization + requiredCPUUtilization;

            fprintf(file, "%s,%d,%d,%d,%.2f\n", host->hostname, ramWastage, bandwidthWastage, storageWastage, cpuUtilizationWastage);
        }
    }

    // Close the CSV file
    fclose(file);

    printf("Data successfully exported to %s\n", filename);
}


Host* bruteForceAlgorithm(HostMap *map, int requiredRAM, int requiredBandwidth, int requiredStorage, float requiredCPUUtilization) {
    // Define array of suitable hosts and initialize with NULL
    Host *suitableHosts[map->size];
    for (size_t i = 0; i < map->size; i++) {
        suitableHosts[i] = NULL; // Initialize all pointers to NULL
    }

    // Iterate through all hosts and check if they are suitable
    size_t suitableHostCount = 0; // Counter for suitable hosts
    for (size_t i = 0; i < map->size; i++) {
        Host *host = &map->hosts[i];
        if (isSuitableHost(host, requiredRAM, requiredBandwidth, requiredStorage, requiredCPUUtilization)) {
            suitableHosts[suitableHostCount++] = host; // Add only valid hosts
        }
    }

    printAllSuitableHostsWastage(map, requiredRAM, requiredBandwidth, requiredStorage, requiredCPUUtilization);

    // Define best host based on resource wastage after VM allocation
    Host *bestHost = NULL;
    int bestScore = INT_MAX; // Initialize to maximum possible value
    for (size_t i = 0; i < suitableHostCount; i++) {
        Host *host = suitableHosts[i];
        if (host) { // Ensure the pointer is valid
            int hostScore = (host->details.totalRAM - host->details.usedRAM - requiredRAM) * RAM_WEIGHT +
                            (host->details.totalBandwidth - host->details.usedBandwidth - requiredBandwidth) * BANDWIDTH_WEIGHT +
                            (host->details.totalStorage - host->details.usedStorage - requiredStorage) * STORAGE_WEIGHT +
                            (host->details.cpuUtilization + requiredCPUUtilization) * CPU_UTILIZATION_WEIGHT;

            if (!bestHost || hostScore < bestScore) {
                bestHost = host;
                bestScore = hostScore;
            }
        }
    }

    if (bestHost) {
        printf("Best host score: %d\n", bestScore);
    } else {
        printf("No suitable host found.\n");
    }
    return bestHost;
}





int main(int argc, char *args[]) {
    // if (argc != 5) {
    //     fprintf(stderr, "Usage: %s <RAM> <Bandwidth> <Storage> <CPU Utilization>\n", argv[0]);
    //     return 1;
    // }

    //define arguments for debugging
    char *argv[] = {"main", "1000", "500", "2000", "20.5"};

    int requiredRAM = atoi(argv[1]);
    int requiredBandwidth = atoi(argv[2]);
    int requiredStorage = atoi(argv[3]);
    float requiredCPUUtilization = atof(argv[4]);

    HostMap map;
    initHostMap(&map, 10);
    initializeHostMapWithValues(&map);

    // Brute force algorithm
    Host *bestHost = bruteForceAlgorithm(&map, requiredRAM, requiredBandwidth, requiredStorage, requiredCPUUtilization);

    if (bestHost) {
        printf("Brute force algorithm selected host: %s\n", bestHost->hostname);
    } else {
        printf("No suitable host found using brute force algorithm.\n");
    }



    freeHostMap(&map);
    return 0;
}
