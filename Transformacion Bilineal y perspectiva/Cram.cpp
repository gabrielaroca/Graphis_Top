#include <bits/stdc++.h> 
using namespace std; 
  
#define M 10 
float a[M][M];
void PrintMatrix(float a[][M], int n) 
{ 
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j <= n; j++)  
          cout << a[i][j] << " "; 
        cout << endl; 
    } 
} 
  
int PerformOperation(float a[][M], int n) { 
    int i, j, k = 0, c, flag = 0, m = 0; 
    float pro = 0; 
    for (i = 0; i < n; i++) 
    { 
        if (a[i][i] == 0)  
        { 
            c = 1; 
            while (a[i + c][i] == 0 && (i + c) < n)  
                c++;             
            if ((i + c) == n) { 
                flag = 1; 
                break; 
            } 
            for (j = i, k = 0; k <= n; k++)  
                swap(a[j][k], a[j+c][k]); 
        } 
  
        for (j = 0; j < n; j++) { 
              
            if (i != j) { 
                float pro = a[j][i] / a[i][i]; 
  
                for (k = 0; k <= n; k++)                  
                    a[j][k] = a[j][k] - (a[i][k]) * pro;                 
            } 
        } 
    } 
    return flag; 
} 
  
void PrintResult(float a[][M], int n, int flag) 
{ 
  
    if (flag == 2)      
      cout << "Infinite Solutions Exists" << endl;     
    else if (flag == 3)      
      cout << "No Solution Exists" << endl; 
      
    else { 
        for (int i = 0; i < n; i++)          
            cout << a[i][n] / a[i][i] << " ";         
    } 
} 
  
// To check whether infinite solutions  
// exists or no solution exists 
int CheckConsistency(float a[][M], int n, int flag) 
{ 
    int i, j; 
    float sum; 
      
    // flag == 2 for infinite solution 
    // flag == 3 for No solution 
    flag = 3; 
    for (i = 0; i < n; i++)  
    { 
        sum = 0; 
        for (j = 0; j < n; j++)         
            sum = sum + a[i][j]; 
        if (sum == a[i][j])  
            flag = 2;         
    } 
    return flag; 
} 
  
// Driver code 


void gaus(){

// Order of Matrix(n) 
    int n = 4, flag = 0; 
      
    // Performing Matrix transformation 
    flag = PerformOperation(a, n); 
      
    if (flag == 1)      
        flag = CheckConsistency(a, n, flag);     
  
    // Printing Final Matrix 
    cout << "Final Augumented Matrix is : " << endl; 
    PrintMatrix(a, n); 
    cout << endl; 
      
    // Printing Solutions(if exist) 
    PrintResult(a, n, flag); 

}

