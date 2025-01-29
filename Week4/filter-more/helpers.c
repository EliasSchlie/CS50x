#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed)/3.0);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE row[width]; 
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            row[width-j-1] = image[i][j];
        }
        for (int j=0; j<width; j++){
            image[i][j] = row[j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE b[height][width]; 
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            int count = 0;
            int r_sum = 0;
            int g_sum = 0;
            int b_sum = 0;
            
            for (int di=-1; di<=1;di++){
                for (int dj=-1; dj<=1;dj++){
                    int ni = i + di;
                    int nj = j + dj;
                    
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width){
                        r_sum += image[ni][nj].rgbtRed;
                        b_sum += image[ni][nj].rgbtBlue;
                        g_sum += image[ni][nj].rgbtGreen;
                        count++;
                    }
                }
            }
            b[i][j].rgbtRed = round((float)r_sum/count);
            b[i][j].rgbtGreen = round((float)g_sum/count);
            b[i][j].rgbtBlue = round((float)b_sum/count);
        }
    }
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            image[i][j] = b[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE b[height][width]; 
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            int r_gx = 0;
            int g_gx = 0;
            int b_gx = 0;
            int r_gy = 0;
            int g_gy = 0;
            int b_gy = 0;
            
            for (int di=-1; di<=1;di++){
                for (int dj=-1; dj<=1;dj++){
                    int ni = i + di;
                    int nj = j + dj;
                    
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width){
                           
                        r_gx += image[ni][nj].rgbtRed * dj * (2-(di*di));
                        b_gx += image[ni][nj].rgbtBlue * dj * (2-(di*di));
                        g_gx += image[ni][nj].rgbtGreen * dj * (2-(di*di));
                        r_gy += image[ni][nj].rgbtRed * di * (2-(dj*dj));
                        b_gy += image[ni][nj].rgbtBlue * di * (2-(dj*dj));
                        g_gy += image[ni][nj].rgbtGreen * di * (2-(dj*dj));
                    }
                }
            }
            b[i][j].rgbtRed = fmin(round(sqrt(r_gx*r_gx+r_gy*r_gy)), 255);
            b[i][j].rgbtGreen = fmin(round(sqrt(g_gx*g_gx+g_gy*g_gy)), 255);
            b[i][j].rgbtBlue = fmin(round(sqrt(b_gx*b_gx+b_gy*b_gy)), 255);
        }
    }
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            image[i][j] = b[i][j];
        }
    }
    return;
}
