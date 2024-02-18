// A01345407 A01377048 A01329182 A01339333
#include <stdio.h>
#include <stdlib.h>

// Function to check if input.txt format is correct
int checkInput(FILE *input, FILE *output)
{
    int rows, cols;

    // Read the first three lines to get the number of rows
    for (int i = 0; i < 3; i++)
    {
        if (fscanf(input, "%d", &rows) != 1)
        {
            fprintf(output, "Error: input.txt has wrong format\n");
            return 1;
        }
    }

    while (!feof(input))
    {
        int a, b, c, d;
        char end;
        if (fscanf(input, "%d,%d,%d,%d", &a, &b, &c, &d) == 4)
        {
            continue;
        }
        else
        {
            // Check the E at the end of input.txt
            fscanf(input, "%c", &end);
            if (end == 'E')
            {
                rewind(input);
                return 0;
            }
            else
            {
                fprintf(output, "Error: input.txt has wrong format\n");
                return 1;
            }
        }
    }
}

// Function to draw based on coordinates
void drawLine(FILE *input, FILE *output)
{
    // Initialize variables
    int x = 0, y = 0, time = 0;

    // Read from input.txt
    fscanf(input, "%d", &x);
    fscanf(input, "%d", &y);
    fscanf(input, "%d", &time);

    // Allocate memory for the 2D array
    char **grid = (char **)malloc((maxY + 1) * sizeof(char *));
    for (int i = 0; i <= maxY; i++)
    {
        grid[i] = (char *)malloc((maxX + 1) * sizeof(char));
    }

    // Initialize the grid with spaces
    for (int i = 0; i <= maxY; i++)
    {
        for (int j = 0; j <= maxX; j++)
        {
            grid[i][j] = ' ';
        }
    }

    // Draw the line and fill the space
    int curX, curY;
    fscanf(input, "%d,%d", &curX, &curY);

    int prevX = curX, prevY = curY;
    while (fscanf(input, "%d,%d", &curX, &curY) == 2)
    {
        if (curX == prevX)
        {
            // If line segment is vertical
            int lY = (curY < prevY) ? curY : prevY;
            int rY = (curY > prevY) ? curY : prevY;

            for (int i = lY; i <= rY; i++)
            {
                grid[i][curX] = '*';
            }
        }
        else if (curY == prevY)
        {
            // If line segment is horizontal
            int lX = (curX < prevX) ? curX : prevX;
            int rX = (curX > prevX) ? curX : prevX;

            for (int j = lX; j <= rX; j++)
            {
                grid[curY][j] = '*';
            }
        }
        else
        {
            // If line segment is 45 degree
            if (curX > prevX && curY > prevY)
            {
                for (int j = curX, i = curY; j >= prevX && i >= prevY; j--, i--)
                {
                    grid[i][j] = '*';
                }
            }
            else if (curX > prevX && curY < prevY)
            {
                for (int j = curX, i = curY; j >= prevX && i <= prevY; j--, i++)
                {
                    grid[i][j] = '*';
                }
            }
            else if (curX < prevX && curY > prevY)
            {
                for (int j = curX, i = curY; j <= prevX && i >= prevY; j++, i--)
                {
                    grid[i][j] = '*';
                }
            }
            else if (curX < prevX && curY < prevY)
            {
                for (int j = curX, i = curY; j <= prevX && i <= prevY; j++, i++)
                {
                    grid[i][j] = '*';
                }
            }
        }

        prevX = curX;
        prevY = curY;
    }

    // Fill the space between line segments
    for (int i = 0; i <= maxY; i++)
    {
        // Calculate the point in the line
        int point = 0;
        for (int j = 0; j <= maxX; j++)
        {
            if (grid[i][j] == '*')
            {
                point++;
            }
        }

        // If more than one point, fill the grid
        if (point > 1)
        {
            int j = 0;
            while (grid[i][j] != '*' && j <= maxX)
            {
                j++;
            }
            while (grid[i][j] == '*' && grid[i][j + 1] != '*' && j <= maxX)
            {
                grid[i][j + 1] = '*';
                j++;
            }
        }
    }

    // Print the grid
    for (int i = maxY; i >= 0; i--)
    {
        for (int j = 0; j <= maxX; j++)
        {
            fprintf(output, "%c", grid[i][j]);
        }
        if (i != 0)
        {
            fprintf(output, "\n");
        }
    }

    // Free the memory
    for (int i = 0; i <= maxY; i++)
    {
        free(grid[i]);
    }
    free(grid);
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

    // Call the function to draw the line and fill the space
    if (checkInput(inputFile, outputFile) == 0)
    {
        drawLine(inputFile, outputFile);
    }

    // Close the files
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
