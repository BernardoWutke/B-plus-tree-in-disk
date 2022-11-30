#include<stdio.h>

int main()
{
    FILE *fp;
    fp = fopen("file.bin", "rb+"); 
    if (fp == NULL) 
    { 
        printf("Could not open file"); 
        return 0; 
    }

    int n[10] = {1,2,3,4,5,6,7,8,9,10};
    int b[] = {11,11};
    int v[10];
    
    fwrite(n, sizeof(int), 10, fp);
    fseek(fp, 0, SEEK_SET);
    fread(v, sizeof(int), 10, fp);
    for(int i = 0; i < 10; i++)
    {
        printf("%i ", v[i]);
    }
    printf("\n");
    fseek(fp, 8 * sizeof(int), SEEK_SET);
    fwrite(b, sizeof(int), 2, fp);
    fseek(fp, 0, SEEK_SET);
    fread(v, sizeof(int), 10, fp);
    fclose(fp);

    for(int i = 0; i < 10; i++)
    {
        printf("%i ", v[i]);
    }
    return 0;
}