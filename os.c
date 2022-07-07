#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<string.h>
#include <sys/wait.h>
 
int* heap_sort(int a[],int n)               //Heap_sort to sort an array efficiently in O(nlogn)
{
    int i,j;
  for (i=1;i<n;i++)
    { 
        int j=i;
        if (a[(i-1)/2]<a[i])
        {
            while (a[(j-1) / 2]<a[j])
            {
                int t=a[j];
            a[j]=a[(j-1)/2];            //This code is for swapping 
            a[(j-1)/2]=t;
                j = (j-1)/2;
            }
        }
    }
    for (i=n-1;i>0;i--)
    {
        int ind;
        int t=a[0];
        a[0]=a[i];
        a[i]=t;
        j=0;
        do
        {
            ind=(2*j+1);
            if (ind < (i - 1) && (a[ind]<a[ind + 1]))
                ind++;
            if ((ind<i) && (a[j]<a[ind]))
            {
                int t=a[j];
                a[j]=a[ind];                //This code is for swapping
                a[ind]=t;
            }
            j=ind;
        } while(ind<i);
    }
    return a;
}
 
int main()
{    
    int pid1,pid2;
    //Creating 6 pipes below:
    int p1[2];  int p2[2];
    int p3[2];  int p4[2];
    int p5[2];  int p6[2];
 
    // To open all pipes
    if (pipe(p1)==-1)
    {
        printf("ERROR! Can't create pipe");
        return 10;
    }
    if (pipe(p2)==-1)
    {
        printf("ERROR! Can't create pipe");
        return 11;
    }
    if (pipe(p3)==-1)
    {
        printf("ERROR! Can't create pipe");
        return 12;
    }
    if (pipe(p4)==-1)
    {
        printf("ERROR! Can't create pipe");
        return 13;
    }
    if (pipe(p5)==-1)
    {
        printf("ERROR! Can't create pipe");
        return 14;
    }
    if (pipe(p6)==-1)
    {
        printf("ERROR! Can't create pipe");
        return 15;
    }
    pid1=fork();
 
    if (pid1==0)                             //First child process starts here
    {
 
       close(p1[1]);close(p2[1]);close(p3[0]);
       int n;
       if(read(p1[0], &n, sizeof(n)) == -1)
       return 16;
 
       int a[n];
       if(read(p2[0], &a, sizeof(a)) == -1)
       return 17;
 
       //Sorting using heap_sort algorithm mentioned above
       int* b=heap_sort(a,n);
 
 
       if(write(p3[1], b, sizeof(*b)*n)==-1)
       return 18;
       exit(0);
    }
    else                                    //below code is for parent process
    {
        pid2=fork();
        if(pid2==0)                         //second child process starts here
        {
            //Closing pipes' exit and entry respectively
           close(p4[1]); close(p5[1]); close(p6[0]); 
 
           int n;
           if(read(p4[0], &n, sizeof(n)) == -1) 
           return 18;
 
           int a[n];
           if(read(p5[0], &a, sizeof(a)) == -1) 
           return 19;
 
           //Sorting using heap_sort algorithm mentioned above
           int *c=heap_sort(a,n);
 
           if(write(p6[1], c, sizeof(*c)*n)==-1)
           return 20;  
           exit(0);
        }
 
        else                            
        {
            
            int n1;                     //to store size of array 1
            int n2;                     //to store size of array 2
            char file1[100];            //to read file 1 where array 1 is stored
            char file2[100];            //to read file 2 where array 2 is stored
            
            //User inputs data 
            printf("Enter size of array 1: ");
            scanf("%d", &n1);
            printf("Enter the file name where array 1 is stored (No need of extension .txt): ");
            scanf("%s", file1);
            printf("Enter size of array 2: ");
            scanf("%d", &n2);
            printf("Enter the file name where array 2 is stored (No need of extension .txt): ");
            scanf("%s", file2);
            int a3[n1+n2];                          //to store the final merged array
 
            FILE* ptr1 = fopen(file1, "r");         //file 1 opened and ptr1 pointing to 1st integer
            int a1[n1];
            if(ptr1==NULL)
            {
                printf("ERROR! Can't read file/File doesn't exist");
                exit (0);
            }
            for(int i=0; i<n1;i++)
            {
                fscanf(ptr1, "%d ", &a1[i]);        //reading integers from file 1
            }
            fclose(ptr1);
 
            FILE* ptr2 = fopen(file2, "r");         //file 2 opened and ptr2 pointing to 1st integer
            int a2[n2];
            if(ptr2==NULL)
            {
                printf("ERROR! Can't read file/File doesn't exist");
                exit(0);
            }
            for(int i=0; i<n2;i++)
            {
                fscanf(ptr2, "%d ", &a2[i]);        //reading integers from file 2
            }
            fclose(ptr2);   
 
        if(write(p1[1], &n1, sizeof(n1))==-1)       //sending array size to first child process
        return 21;
 
        if(write(p4[1], &n2, sizeof(n2))==-1)       //sending array size to second child process
        return 22;
 
        if(write(p2[1], &a1, sizeof(a1))==-1)       //sending array elements to first child process
        return 23;
      printf("\nArray 1 after reading from 1st file is: ");
        for(int i=0;i<n1;i++)
        printf("%d ", a1[i]);
 
        if(write(p5[1], &a2, sizeof(a2))==-1)       //sending array elements to second child process
        return 24;
      printf("\nArray 2 after reading from 2nd file is: ");
        for(int i=0;i<n2;i++)
        printf("%d ", a2[i]);
        
        printf("\n");
 
        if(read(p3[0], &a1, sizeof(a1)) == -1)      //receiving sorted array from first child process
        return 25;
        if(read(p6[0], &a2, sizeof(a2)) == -1)      //receiving sorted array from second child process
        return 26;
 
        //a1 and a2 arrays are now sorted
        //below is the code for merging the two sorted arrays in O(n) complexity
        int i=0,j=0;
        while(i<n1 && j<n2)
        {
            if(a1[i]<=a2[j])
            {
                a3[i+j]=a1[i];
                i++;
            }
            else
            {
                a3[i+j]=a2[j];
                j++;
            }
        }
        while(i<n1)
        {   
            a3[i+j]=a1[i];
            i++;
        }
        while(j<n2)
        {
            a3[i+j]=a2[j];
            j++;
        }
 
        printf("\nThe Final merged array is: ");
        for(i=0;i<n1+n2;i++)
        {
            printf("%d ",a3[i]);
        }
      printf("\n");
 
        close(p1[0]);   close(p3[0]);
        close(p2[1]);   close(p4[0]);
        close(p5[0]);   close(p6[1]);
        waitpid(-1, NULL, 0);  
        }
    }
 
    return 0;
}
