// A01345407 A01377048 A01329182 A01339333
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x, y;
    int vx, vy;
} Particle;

int readFile(FILE *input, FILE *output, int *pw, int *ph, int *pt, Particle **ps) {
    if (fscanf(input, "%d %d %d", pw, ph, pt) != 3) {
        fprintf(output, "Error: input.txt has wrong format\n");
        return 0;
    }

    int numParticles = 0;
    int a, b, c, d;
    char ch;
    while ((ch = fgetc(input)) != 'E') {
        ungetc(ch, input);
        if (fscanf(input, "%d,%d,%d,%d ", &a, &b, &c, &d) != 4) {
            fprintf(output, "Error: input.txt has wrong format\n");
            return 0;
        }
        numParticles++;
    }

    rewind(input);
    fscanf(input, "%d %d %d", pw, ph, pt);

    *ps = malloc(numParticles * sizeof(Particle));
    if (*ps == NULL) {
        fprintf(output, "Error: Memory allocation failed\n");
        return 0;
    }

    for (int i = 0; i < numParticles; i++) {
        Particle *p = *ps + i;
        fscanf(input, "%d,%d,%d,%d ", &p->x, &p->y, &p->vx, &p->vy);
    }

    return numParticles;
}

void printParticles(Particle *ps, int nP) {
    for (int i = 0; i < nP; i++) {
        Particle *p = ps + i;
        printf("x = %d, y = %d, vx = %d, vy = %d\n", p->x, p->y, p->vx, p->vy);
    }
}

void simulateParticles(Particle *ps, int w, int h, int t, int nP) {
    for (int i = 0; i < nP; i++) {
        Particle *p = ps + i;
        for (int j = 0; j < t; j++) {
            if (p->x + p->vx < 0 || p-> x + p->vx >= w) p->vx = -p->vx;
            if (p->y + p->vy < 0 || p-> y + p->vy >= h) p->vy = -p->vy;
            p->x += p->vx;
            p->y += p->vy;
        }
    }
}

void displayGrid(Particle *ps, int w, int h, int nP) {
    // for (int y = 0; y < h; y++) {
    //     for (int x = 0; x < w; x++) {
    //         int particlePresent = 0;
    //         for (int i = 0; i < nP; i++) {
    //             Particle *p = ps + i;
    //             if (p->x == x && p->y == y) {
    //                 particlePresent = 1;
    //                 break;
    //             }
    //         }
    //         if (particlePresent) {
    //             printf("+"); // Print symbol representing a particle
    //         } else {
    //             printf("*"); // Print an empty space
    //         }
    //     }
    //     printf("\n"); // Move to the next line after printing each row
    // }
}

int main(int argc, char *argv[])
{
    // Initialize file pointer
    char *inputFileName = argv[1];
    char *outputFileName = argv[2];
    FILE *inputFile, *outputFile;
    inputFile = fopen(inputFileName, "r");
    outputFile = fopen(outputFileName, "w");

    // Check argument
    if (argc != 3)
    {
        fprintf(outputFile, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // Check if files are opened successfully
    if (inputFile == NULL || outputFile == NULL)
    {
        fprintf(outputFile, "Error: cannot open the files.\n");
        // Quit
        return 1;
    }

    int width, height, time;
    Particle *particles;

    int numParticles = readFile(inputFile, outputFile, &width, &height, &time, &particles);
    printParticles(particles, numParticles);

    simulateParticles(particles, width, height, time, numParticles);
    printParticles(particles, numParticles);

    displayGrid(particles, width, height, numParticles);

    free(particles);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
