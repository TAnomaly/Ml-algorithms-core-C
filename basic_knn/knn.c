#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_IRIS 150
#define K 3
#define FILE_NAME "iris.csv"
typedef struct {
    float sepal_length;
    float sepal_width;
    float petal_length;
    float petal_width;
    char species[32];
} Iris;
Iris iris[MAX_IRIS];
int iris_count=0;
int scan_file(char *file_name){
    FILE *fp=fopen(file_name,"r");
    if(fp==NULL){
        printf("Filecouldnotbeopened.\n");
        return 1;
    }
    char line[100];
    int i=0;
    while(fgets(line,sizeof(line),fp)!=NULL&&i<MAX_IRIS){
        sscanf(line,"%f,%f,%f,%f,%31s",&iris[i].sepal_length,&iris[i].sepal_width,&iris[i].petal_length,&iris[i].petal_width,iris[i].species);
        i++;
    }
    iris_count=i;
    fclose(fp);
    return 0;
}
float distance(Iris *a,float sl,float sw,float pl,float pw){
    return sqrt(
        pow(a->sepal_length-sl,2)+
        pow(a->sepal_width-sw,2)+
        pow(a->petal_length-pl,2)+
        pow(a->petal_width-pw,2)
    );
}
void knn_predict(float sl,float sw,float pl,float pw){
    float distances[MAX_IRIS];
    int indices[MAX_IRIS];
    for(int i=0;i<iris_count;i++){
        distances[i]=distance(&iris[i],sl,sw,pl,pw);
        indices[i]=i;
    }
    for(int i=0;i<iris_count-1;i++){
        for(int j=i+1;j<iris_count;j++){
            if(distances[j]<distances[i]){
                float tmp=distances[i];
                distances[i]=distances[j];
                distances[j]=tmp;
                int tmpi=indices[i];
                indices[i]=indices[j];
                indices[j]=tmpi;
            }
        }
    }
    int setosa=0,versicolor=0,virginica=0;
    for(int i=0;i<K;i++){
        char *type=iris[indices[i]].species;
        if(strcmp(type,"Iris-setosa")==0)setosa++;
        else if(strcmp(type,"Iris-versicolor")==0)versicolor++;
        else if(strcmp(type,"Iris-virginica")==0)virginica++;
    }
    printf("K=%dnearestneighbors:\n",K);
    printf("Setosa:%d,Versicolor:%d,Virginica:%d\n",setosa,versicolor,virginica);
    if(setosa>=versicolor&&setosa>=virginica)
        printf("Predictedspecies:Iris-setosa\n");
    else if(versicolor>=setosa&&versicolor>=virginica)
        printf("Predictedspecies:Iris-versicolor\n");
    else
        printf("Predictedspecies:Iris-virginica\n");
}
int main(){
    if(scan_file(FILE_NAME)==1){
        printf("Filecouldnotbescanned.\n");
        return 1;
    }
    float sl,sw,pl,pw;
    printf("Enter4featuresfortesting(sepal_length,sepal_width,petal_length,petal_width):\n");
    scanf("%f%f%f%f",&sl,&sw,&pl,&pw);
    knn_predict(sl,sw,pl,pw);
    return 0;
}