#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100
#define GRAPH_WIDTH 40
#define GRAPH_HEIGHT 15
 
typedef struct {
    double a, b, c, d, x;
} Spline;

void computeSpline(double* x, double* y, int n, Spline* splines) {
    double h[MAX_POINTS], alpha[MAX_POINTS];
    double l[MAX_POINTS + 1], mu[MAX_POINTS + 1], z[MAX_POINTS + 1];

    
    for (int i = 0; i < n; i++) 
        h[i] = x[i + 1] - x[i];

   
    for (int i = 1; i < n; i++)
        alpha[i] = (3.0 / h[i]) * (y[i + 1] - y[i]) - (3.0 / h[i - 1]) * (y[i] - y[i - 1]);

   
    l[0] = 1.0;
    mu[0] = 0.0;
    z[0] = 0.0;

   
    for (int i = 1; i < n; i++) {
        l[i] = 2.0 * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }

    
    l[n] = 1.0;
    z[n] = 0.0;

    splines[n].c = 0.0; 

   
    for (int j = n - 1; j >= 0; j--) {
        splines[j].c = z[j] - mu[j] * splines[j + 1].c;
        splines[j].b = (y[j + 1] - y[j]) / h[j] - h[j] * (splines[j + 1].c + 2.0 * splines[j].c) / 3.0;
        splines[j].d = (splines[j + 1].c - splines[j].c) / (3.0 * h[j]);
        splines[j].a = y[j];
    }
}

void drawGraph(Spline* splines, double* x, double* y, int n) {
    FILE* output = fopen("output.txt", "a");
    char graph[GRAPH_HEIGHT][GRAPH_WIDTH];

    
    for (int i = 0; i < GRAPH_HEIGHT; i++)
        for (int j = 0; j < GRAPH_WIDTH; j++)
            graph[i][j] = ' ';

    int x_axis = GRAPH_HEIGHT - 1;
    int y_axis = 7;

   
    for (int j = y_axis; j < GRAPH_WIDTH; j++) graph[x_axis][j] = '-';
    for (int i = 0; i < GRAPH_HEIGHT; i++) graph[i][y_axis] = '|';
    graph[x_axis][y_axis] = '+';

    
    for (double xi = x[0]; xi <= x[n]; xi += (x[n] - x[0]) / GRAPH_WIDTH) {
        double yi = 0.0;
        for (int i = 0; i < n; i++) {
            if (xi >= splines[i].x && xi <= x[i + 1]) {
                double dx = xi - splines[i].x;
                yi = splines[i].a + splines[i].b * dx + splines[i].c * dx * dx + splines[i].d * dx * dx * dx;
                break;
            }
        }

        int plot_x = y_axis + 1 + (int)((xi - x[0]) / (x[n] - x[0]) * (GRAPH_WIDTH - y_axis - 1));
        int plot_y = GRAPH_HEIGHT - 1 - (int)((yi - y[0]) / (y[n] - y[0]) * (GRAPH_HEIGHT - 1));

        if (plot_x >= y_axis + 1 && plot_x < GRAPH_WIDTH && plot_y >= 0 && plot_y < GRAPH_HEIGHT)
            graph[plot_y][plot_x] = '*';
    }

    
    for (int i = 0; i < GRAPH_HEIGHT; i++) {
        if (i % 3 == 0) {
            printf("%7.2f | ", y[n] - ((y[n] - y[0]) / (GRAPH_HEIGHT - 1)) * i);
            fprintf(output, "%7.2f | ", y[n] - ((y[n] - y[0]) / (GRAPH_HEIGHT - 1)) * i);
        } else {
            printf("        | ");
            fprintf(output, "        | ");
        }

        for (int j = 0; j < GRAPH_WIDTH; j++) {
            printf("%c", graph[i][j]);
            fprintf(output, "%c", graph[i][j]);
        }
        printf("\n");
        fprintf(output, "\n");
    }

    
    printf("        ");
    fprintf(output, "        ");
    for (int j = y_axis + 5; j < GRAPH_WIDTH; j += GRAPH_WIDTH / 5) {
        printf("%7.1f", x[0] + (x[n] - x[0]) * (j - y_axis) / (GRAPH_WIDTH - y_axis));
        fprintf(output, "%7.1f", x[0] + (x[n] - x[0]) * (j - y_axis) / (GRAPH_WIDTH - y_axis));
    }
    printf("\n");
    fprintf(output, "\n");

    fclose(output);
}

int main() {
    double x[MAX_POINTS], y[MAX_POINTS];
    double temp_x, temp_y;
    int choice, n;
    Spline splines[MAX_POINTS];
    printf("--------------------------------------DA NANG UNIVERSITY OF SCIENCE AND TECHNOLOGY--------------------------------------\n");
    printf("-------------------------------------------DO AN LAP TRINH TOAN-DE TAI 01-----------------------------------------------\n");
    printf("---------------------------------------------SPLINE INTERPOLATION GRAPH-------------------------------------------------\n\n");
    printf("                                  GIANG VIEN HUONG DAN : PGS.TS. NGUYEN TAN KHOI\n\n");
    printf("----NAME : PHONGSY PHOUMANAT                                                          NAME : NENHOUANMALA XACHALUEN ----\n");
    printf("--STUDENT CODE: 102240425                                                                      STUDENT CODE: 102240424--\n");
    printf("--CLASS: 24T-DT4                                                                                        CLASS: 24T-DT4--\n\n");
    printf("                              ------------------------------------------------------------                               \n"); 
    printf("Nhap 1 de nhap du lieu tu file\n");
    printf("Nhap 2 de nhap du lieu bang tay\n");
    printf("Lua chon cua ban: ");
    scanf("%d", &choice);

    if (choice == 1) {
        FILE* input = fopen("input.txt", "r");
        if (input == NULL) {
            printf("Khong the mo file input.txt\n");
            return 1;
        }
        
        n = 0;
        while (fscanf(input, "%lf %lf", &x[n], &y[n]) == 2) {
            n++;
        }
        fclose(input);
    } else {
        printf("Nhap so luong diem du lieu: ");
        scanf("%d", &n);
        
        FILE* outputFile = fopen("input.txt", "w");
        printf("Nhap cac cap gia tri x y:\n");
        for (int i = 0; i < n; i++) {
            printf("Nhap x[%d]: ", i + 1);
            scanf("%lf", &x[i]);
            printf("Nhap y[%d]: ", i + 1);
            scanf("%lf", &y[i]);
            fprintf(outputFile, "%lf %lf\n", x[i], y[i]);
        }
        fclose(outputFile);
    }

    n--; 

    FILE* output = fopen("output.txt", "w");
    printf("Spline coefficients:\n");
    fprintf(output, "Spline coefficients:\n");
    
    computeSpline(x, y, n, splines);
    
    for (int i = 0; i < n; i++) {
        printf("S[%d]: %.4f %.4f %.4f %.4f\n", 
               i, splines[i].a, splines[i].b, splines[i].c, splines[i].d);
        fprintf(output, "S[%d]: %.4f %.4f %.4f %.4f\n", 
                i, splines[i].a, splines[i].b, splines[i].c, splines[i].d);
    }
    
    drawGraph(splines, x, y, n);
    fclose(output);
    system("\"D:\\gnuplot\\bin\\gnuplot.exe\" -p script.gnu");
    return 0;
}
