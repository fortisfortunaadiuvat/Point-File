#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

struct point{
    float x,y,z;
    int red,green,blue;
};

double distance(struct point *p1,struct point *p2){
    return sqrt(pow((p1->x)-(p2->x),2)+pow((p1->y)-(p2->y),2)+pow((p1->z)-(p2->z),2));
}

double maxmin_distance(struct point *p3,int t,int fm,int dt,FILE *outfile){
    int i,j,index_i=0,index_j=0,index_k=0,index_l=0;
    double max=distance(&p3[0],&p3[1]);
    double min=distance(&p3[0],&p3[1]);
    double number,number2;

    for(i=0;i<t-2;i++){
        for(j=i+1;j<t-1;j++){
            number = distance(&p3[i],&p3[j]);
            number2= distance(&p3[i],&p3[j]);
            if(number>max){
                max = number;
                index_i = i;
                index_j = j;
            }else if(number2<min){
                min = number2;
                index_k = i;
                index_l = j;
            }
        }
    }
    if(fm==1 && dt==2){
        fprintf(outfile,"maximum between:\n");
        fprintf(outfile,"point %d: %f %f %f %d %d %d\n",index_i+1,p3[index_i].x,
                p3[index_i].y,p3[index_i].z,p3[index_i].red,p3[index_i].green,
                p3[index_i].blue);
        fprintf(outfile,"point %d: %f %f %f %d %d %d\n",index_j+1,p3[index_j].x,
                p3[index_j].y,p3[index_j].z,p3[index_j].red,p3[index_j].green,
                p3[index_j].blue);
        fprintf(outfile,"minimum between:\n");
        fprintf(outfile,"point %d: %f %f %f %d %d %d\n",index_k+1,p3[index_k].x,
                p3[index_k].y,p3[index_k].z,p3[index_k].red,p3[index_k].green,
                p3[index_k].blue);
        fprintf(outfile,"point %d: %f %f %f %d %d %d\n",index_l+1,p3[index_l].x,
                p3[index_l].y,p3[index_l].z,p3[index_l].red,p3[index_l].green,
                p3[index_l].blue);
    }else if(fm==2 && dt==2){
        fprintf(outfile,"maximum between:\n");
        fprintf(outfile,"point %d: %f %f %f \n",index_i+1,p3[index_i].x,
                p3[index_i].y,p3[index_i].z);
        fprintf(outfile,"point %d: %f %f %f \n",index_j+1,p3[index_j].x,
                p3[index_j].y,p3[index_j].z);
        fprintf(outfile,"minimum between:\n");
        fprintf(outfile,"point %d: %f %f %f\n",index_k+1,p3[index_k].x,
                p3[index_k].y,p3[index_k].z);
        fprintf(outfile,"point %d: %f %f %f\n",index_l+1,p3[index_l].x,
                p3[index_l].y,p3[index_l].z);
    }else if(fm==1 && dt==1){
        fwrite(&p3[index_i],sizeof(struct point),1,outfile);
        fprintf(outfile,"\n");
        fwrite(&p3[index_j],sizeof(struct point),1,outfile);
        fprintf(outfile,"\n");
        fwrite(&p3[index_k],sizeof(struct point),1,outfile);
        fprintf(outfile,"\n");
        fwrite(&p3[index_l],sizeof(struct point),1,outfile);
        fprintf(outfile,"\n");
    }else if(fm==2 && dt==1){
        fwrite(&p3[index_i],sizeof(struct point)/2,1,outfile);
        fprintf(outfile,"\n");
        fwrite(&p3[index_j],sizeof(struct point)/2,1,outfile);
        fprintf(outfile,"\n");
        fwrite(&p3[index_k],sizeof(struct point)/2,1,outfile);
        fprintf(outfile,"\n");
        fwrite(&p3[index_l],sizeof(struct point)/2,1,outfile);
        fprintf(outfile,"\n");
    }
    return max;
}

double average(struct point *pa,int t2){
    double n2=((double)t2*(double)(t2-1))/2;
    int i,j;
    static double total2=0;
    double number2;

    for(i=0;i<t2-2;i++){
        for(j=i+1;j<t2-1;j++){
            number2 = distance(&pa[i],&pa[j]);
            total2 += number2;
        }
    }
    double average = total2 / n2;

    return average;
}

void distancexyz(struct point *p,int t1,double *max,double *min){
    int i;
    double maxx,maxy,maxz,minx,miny,minz;

    maxx = p[0].x;
    maxy = p[0].y;
    maxz = p[0].z;
    minx = p[0].x;
    miny = p[0].y;
    minz = p[0].z;

    for(i=1;i<t1;i++){
        if(p[i].x>maxx){
            maxx=p[i].x;
        }
    }
    for(i=1;i<t1;i++){
        if(p[i].y>maxy){
            maxy=p[i].y;
        }
    }
    for(i=1;i<t1;i++){
        if(p[i].z>maxz){
            maxz=p[i].z;
        }
    }

    if(maxy>maxx && maxy>maxz){
        *max=maxy;
    }else if(maxz>maxx && maxz>maxy){
        *max=maxz;
    }else
        *max=maxx;

    for(i=1;i<t1;i++){
        if(p[i].x<minx){
            minx=p[i].x;
        }
    }
    for(i=1;i<t1;i++){
        if(p[i].y<miny){
            miny=p[i].y;
        }
    }
    for(i=1;i<t1;i++){
        if(p[i].z<minz){
            minz=p[i].z;
        }
    }

    if(miny<minx && miny<minz){
        *min=miny;
    }else if(minz<minx && minz<miny){
        *min=minz;
    }else
        *min=minx;
}

void sphere(struct point *p7,float x,float y,float z,float r,int n,int ft,int dt,FILE *outfile){
    double *distance5;
    distance5=malloc(n*sizeof(double));
    int i;

    for(i=0;i<n;i++){
        distance5[i] = sqrt(pow(p7[i].x-x,2)+pow(p7[i].y-y,2)+pow(p7[i].z-z,2));
        if(distance5[i]<=r && ft==1 && dt==2){
            fprintf(outfile,"%d.point: %f %f %f %d %d %d\n",i+1,p7[i].x,p7[i].y,p7[i].z,
            p7[i].red,p7[i].green,p7[i].blue);
        }else if(distance5[i]<=r && ft==2 && dt==2){
            fprintf(outfile,"%d.point: %f %f %f \n",i+1,p7[i].x,p7[i].y,p7[i].z);
        }else if(distance5[i]<=r && dt==1){
            fwrite(&distance5[i],sizeof(double),1,outfile);
            fprintf(outfile,"\n");
        }
    }
}

void cube_edge(double *maxce,double *mince,FILE *outfile,int dt){
    char array[8][66];
    double array2[8][3]={
        {*mince,*mince,*mince},
        {*maxce,*mince,*mince},
        {*mince,*maxce,*mince},
        {*maxce,*maxce,*mince},
        {*mince,*mince,*maxce},
        {*mince,*mince,*maxce},
        {*mince,*maxce,*maxce},
        {*maxce,*maxce,*maxce}
    };

    snprintf(array[0],sizeof(array),"%f %f %f",*mince,*mince,*mince);
    snprintf(array[1],sizeof(array),"%f %f %f",*maxce,*mince,*mince);
    snprintf(array[2],sizeof(array),"%f %f %f",*mince,*maxce,*mince);
    snprintf(array[3],sizeof(array),"%f %f %f",*maxce,*maxce,*mince);
    snprintf(array[4],sizeof(array),"%f %f %f",*mince,*mince,*maxce);
    snprintf(array[5],sizeof(array),"%f %f %f",*maxce,*mince,*maxce);
    snprintf(array[6],sizeof(array),"%f %f %f",*mince,*maxce,*maxce);
    snprintf(array[7],sizeof(array),"%f %f %f",*maxce,*maxce,*maxce);

    if(dt==2){
        int i;

        for(i=0;i<8;i++){
            fprintf(outfile,"%s \n",array[i]);
        }
    }else if(dt==1){
        int j;

        for(j=0;j<8;j++){

            fwrite(array2[j],1,sizeof(array2[j]),outfile);
            fprintf(outfile,"\n");
        }
    }
}

int main(){

    DIR *dir;
    struct dirent *sd;
    char search[]=".nkt";
    dir = opendir(".");
    int i=0,j;
    char str[200];
    int number_of_file=0;

    if(!dir){
        printf("Directory was not found!\n");
        exit(1);
    }

    while( (sd=readdir(dir)) != NULL){
        char *ptr2;
        ptr2 = strstr(sd->d_name,search);
        if(ptr2 != NULL){
            number_of_file++;
        }
    }
    printf("Number of file: %d\n",number_of_file);
    char filename[number_of_file][20];

    rewinddir(dir);

    while( (sd=readdir(dir)) != NULL){
        char *ptr;
        ptr=strstr(sd->d_name,search);
        if(ptr != NULL){
                strncpy(filename[i],sd->d_name,20);
                filename[i][20]='\0';
                i++;
        }

    }
    for(i=0;i<number_of_file;i++){
        printf("%s\n",filename[i]);
    }

    closedir(dir);

    char filename2[number_of_file][20] ;

    for(j=0;j<number_of_file;j++){
        sprintf(filename2[j],"output%s",filename[j]);
        printf("%s\n",filename2[j]);
    }

    FILE* infile[number_of_file];
    FILE* outfile[number_of_file];
    int q=0;

    while(q<number_of_file){
        int f=0,l=0,m=0,k=0,d=0,v;
        infile[q]=fopen(filename[q],"r");
        outfile[q]=fopen(filename2[q],"w");

        if(infile[q]==NULL){
            perror("Error: ");
            exit(1);
        }
        while(fgets(str,50,infile[q])!=NULL){
            if(m==2){
                break;
            }
            m++;
        }
        char area_format[]="ALANLAR x y z r g b";
        if(strcmp(str,area_format)==1){
            printf("Format1 should to begin.\n");
            f=1;
        }else {
            printf("Format2 should to begin.\n");
            f=2;
        }
        rewind(infile[q]);

        m=0;
        while(fgets(str,50,infile[q])!=NULL){
            if(m==1){
                break;
            }
            m++;
        }

        char version[] = "VERSION 1";
        if(strcmp(str,version)==1){
            printf("VERSION 1.\n");
            v=1;
        }else{
            printf("Other version.\n");
            v=0;
        }
        rewind(infile[q]);

        m=0;
        while(fgets(str,50,infile[q])!=NULL){
            if(m==4){
                break;
            }
            m++;
        }

        char data_format[] = "DATA binary";
        if(strcmp(str,data_format)==1){
            printf("DATA binary.\n");
            d=1;
        }else{
            printf("DATA ascii.\n");
            d=2;
        }
        rewind(infile[q]);

        m=0;
        while(fgets(str,50,infile[q])!=NULL){
            if(m==3){
                break;
            }
            m++;
        }
        char arr[6];

        for(i=0;i<6;i++){
            arr[i]=str[i+9];
        }

        int number_of_p = atoi(arr);
        printf("number of point(s): %d\n",number_of_p);
        rewind(infile[q]);

        struct point *p;
        p=malloc(number_of_p*sizeof(struct point));
        struct point myrecord;

        if (f==1){
            while(!feof(infile[q])){
                while(fgets(str,200,infile[q])){
                        k++;
                        if(k==5 && d==1){
                            while(1){
                                size_t number = fread(&myrecord,sizeof(myrecord),1,infile[q]);
                                l++;
                                if(number<1) break;
                                p[l-1].x = myrecord.x;
                                p[l-1].y = myrecord.y;
                                p[l-1].z = myrecord.z;
                                p[l-1].red= myrecord.red;
                                p[l-1].green = myrecord.green;
                                p[l-1].blue = myrecord.blue;
                            }
                        }else if(k>=5 && d==2){
                            fscanf(infile[q],"%f %f %f %d %d %d",&p[k-5].x,&p[k-5].y,&p[k-5].z,
                            &p[k-5].red,&p[k-5].green,&p[k-5].blue);
                            l++;
                        }
                }
            }
        }else if(f==2){
            while(!feof(infile[q])){
                while(fgets(str,200,infile[q])){
                        k++;
                        if(k==5 && d==1){
                            while(1){
                                size_t x = fread(&myrecord,sizeof(myrecord)/2,1,infile[q]);
                                l++;
                                if(x<1) break;
                                p[l-1].x = myrecord.x;
                                p[l-1].y = myrecord.y;
                                p[l-1].z = myrecord.z;
                            }
                        }else if(k>=5 && d==2){
                            fscanf(infile[q],"%f %f %f",&p[k-5].x,&p[k-5].y,&p[k-5].z);
                            l++;
                        }
                    }
                }
        }
        int number_of_point = 0;
        if(d==1){
            number_of_point = l-1;
        }else
            number_of_point = k-5;

        static double maxcubedge=0,mincubedge=0;
        distancexyz(p,number_of_point,&maxcubedge,&mincubedge);
        double average1;
        float cx,cy,cz,cr;
        int n1,n2,c,s=0;
        bool flag=true;

        while(flag==true){
            printf("Please enter your choice:");
            scanf("%d",&n1);
            if(n1!=1){
                printf("Please enter acceptable choice!\n");
            }else{
                if(number_of_p==number_of_point && f==1 && v==1){
                    fprintf(outfile[q],"CHOICE1\n");
                    fprintf(outfile[q],"All points are compatible.\n");
                }else if(number_of_p==number_of_point && f==2 && v==1){
                    fprintf(outfile[q],"CHOICE1\n");
                    fprintf(outfile[q],"All points are compatible.\n");
                }else {
                    fprintf(outfile[q],"CHOICE1\n");
                    fprintf(outfile[q],"Number of point(s) is not valid.\n");
                    fprintf(outfile[q],"Some points are no colour info!\n");
                    while(s<number_of_p){
                        if(p[s].red < 0 || p[s].red > 255 || p[s].green < 0 || p[s].green > 255 ||
                            p[s].blue < 0 || p[s].blue > 255){
                                fprintf(outfile[q],"%d. point has no colour info!\n",s+1);
                        }
                        s++;
                    }

                    printf("Execution terminated.\n");
                    flag=false;
                    break;
                }

                for(c=0;c<4;c++){
                    printf("Please enter your choice:");
                    scanf("%d",&n2);
                    switch(n2){
                        case 2:
                            fprintf(outfile[q],"CHOICE2\n");
                            maxmin_distance(p,number_of_point,f,d,outfile[q]);
                            break;
                        case 3:
                            fprintf(outfile[q],"CHOICE3\n");
                            cube_edge(&maxcubedge,&mincubedge,outfile[q],d);
                            break;
                        case 4:
                            fprintf(outfile[q],"CHOICE4\n");
                            printf("Please enter cx: ");
                            scanf("%f",&cx);
                            fprintf(outfile[q],"cx: %f\n",cx);
                            printf("Please enter cy: ");
                            scanf("%f",&cy);
                            fprintf(outfile[q],"cy: %f\n",cy);
                            printf("Please enter cz: ");
                            scanf("%f",&cz);
                            fprintf(outfile[q],"cz: %f\n",cz);
                            printf("Please enter cr: ");
                            scanf("%f",&cr);
                            fprintf(outfile[q],"cr: %f\n",cr);

                            fprintf(outfile[q],"Point(s) in sphere:\n");
                            sphere(p,cx,cy,cz,cr,number_of_point,f,d,outfile[q]);
                            break;
                        case 5:
                            fprintf(outfile[q],"CHOICE5\n");
                            average1 = average(p,number_of_point);
                            double *average2;
                            average2=&average1;

                            if(d==2){
                                fprintf(outfile[q],"average is: %f",average1);
                            }else if(d==1){
                                fwrite(average2,sizeof(double),1,outfile[q]);
                            }
                            break;
                    }
            }
            if(c==4){
                flag = false;
            }
            }
        }
        fclose(infile[q]);
        q++;
    }

    return 0;
}
