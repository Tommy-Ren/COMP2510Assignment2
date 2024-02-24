// A01345407 A01377048 A01329182 A01339333
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int x, y;
    int vx, vy;
    int collision;
} Particle;

int readFile(FILE *input, FILE *output, int *pw, int *ph, int *pt, Particle **ps)
{
    // Read the first 3 lines from input.txt
    if (fscanf(input, "%d %d %d", pw, ph, pt) != 3)
    {
        fprintf(output, "Error: input.txt has wrong format\n");
        return 0;
    }

    // Check particles' format and get num of particles
    int numParticles = 0;
    int a, b, c, d;
    char ch;
    while ((ch = fgetc(input)) != 'E')
    {
        ungetc(ch, input);
        if (fscanf(input, "%d,%d,%d,%d ", &a, &b, &c, &d) != 4)
        {
            fprintf(output, "Error: input.txt has wrong format\n");
            return 0;
        }
        numParticles++;
    }

    // Read again from beginning and get width, height, and time
    rewind(input);
    fscanf(input, "%d %d %d", pw, ph, pt);

    // Create a struct that can hold all particles
    *ps = malloc(numParticles * sizeof(Particle));
    if (*ps == NULL)
    {
        fprintf(output, "Error: Memory allocation failed\n");
        return 0;
    }

    // Give values for all particles
    for (int i = 0; i < numParticles; i++)
    {
        Particle *p = *ps + i;
        fscanf(input, "%d,%d,%d,%d ", &p->x, &p->y, &p->vx, &p->vy);
        p->collision = 0;
    }

    return numParticles;
}

// void printParticles(Particle *ps, int nP)
// {
//     // ONLY FOR TESTER
//     // Print information of all particles
//     for (int i = 0; i < nP; i++)
//     {
//         Particle *p = ps + i;
//         printf("x = %d, y = %d, vx = %d, vy = %d cls = %d\n", p->x, p->y, p->vx, p->vy, p->collision);
//     }
// }

void simulateParticles(Particle *ps, int w, int h, int t, int nP)
{
    // Iterate each particle
    for (int x = 0; x < nP; x++)
    {
        // Check if they have collision
        Particle *p1 = ps + x;
        if (p1->collision == 1)
            continue;

        // Check the rest
        for (int y = x + 1; y < nP; y++)
        {
            Particle *p2 = ps + y;
            // Check the next particle if they have collision
            if (p2->collision == 1)
                continue;
            // Check if every two particles have collision
            if (p1->x == p2->x && p1->y == p2->y)
            {
                p1->collision = 1;
                p2->collision = 1;
            }
        }
    }

    // Move the particle for t turn
    for (int i = 0; i < t; i++)
    {
        for (int j = 0; j < nP; j++)
        {
            Particle *p = ps + j;
            if (p->x + p->vx < 0 || p->x + p->vx >= w)
                p->vx = -p->vx;
            if (p->y + p->vy < 0 || p->y + p->vy >= h)
                p->vy = -p->vy;
            p->x += p->vx;
            p->y += p->vy;
        }

        // Check collision after moving them
        for (int x = 0; x < nP; x++)
        {
            Particle *p1 = ps + x;
            if (p1->collision == 1)
                continue;
            for (int y = x + 1; y < nP; y++)
            {
                Particle *p2 = ps + y;
                if (p2->collision == 1)
                    continue;
                if (p1->x == p2->x && p1->y == p2->y)
                {
                    p1->collision = 1;
                    p2->collision = 1;
                }
            }
        }
    }
}

void displayGrid(FILE *output, Particle *ps, int w, int h, int nP)
{
    // Set the grid in the output.txt
    char grid[h + 2][w + 2];
    for (int i = 0; i < h + 2; i++)
    {
        for (int j = 0; j < w + 2; j++)
        {
            if (i * j == 0 || i == h + 1 || j == w + 1)
            {
                grid[i][j] = '*';
            }
            else
            {
                grid[i][j] = ' ';
            }
        }
    }

    // Set the particles in the output.txt
    for (int i = 0; i < nP; i++)
    {
        Particle *p = ps + i;
        if (p->collision == 0)
            grid[h - p->y][p->x + 1] = '+';
    }

    // Print the whole grid and particles included
    for (int i = 0; i < h + 2; i++)
    {
        for (int j = 0; j < w + 2; j++)
        {
            fputc(grid[i][j], output);
        }
        if (i < h + 1)
            fputc('\n', output);
    }
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

    // Intialize some variables we gonna use
    int width, height, time;
    Particle *particles;

    int numParticles = readFile(inputFile, outputFile, &width, &height, &time, &particles);

    // Only simulate it when num of particles is bigger than 0
    if (numParticles)
    {
        simulateParticles(particles, width, height, time, numParticles);
        displayGrid(outputFile, particles, width, height, numParticles);
    }

    free(particles);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
