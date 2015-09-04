#include <stdio.h>
#include <stdlib.h>     
int jobs,s;
float s_cpu_array[20],s_mem_array[20];
float jobs_cpu_array[40],jobs_mem_array[40]; 
int main() 
{ 
  FILE *fid;
  int j,z,x,i; 
  char *filename = "data.txt";
  /*--open datafile----*/ 
  if((fid = fopen(filename,"r")) == NULL) 
  { 
   printf("Errors in opening the packages datafile <%s>\n", filename); 
   exit(1); 
  } 

 /*--get number of Nodes----*/ 
fscanf(fid,"%d",&s);


  /*--get number of VMs----*/ 
fscanf(fid,"%d",&jobs); 
  
  if(jobs < 1 ) 
  { 
    printf("Number of VMS ,%d, out of bounds \n", jobs); 
    exit(1); 
  } 
float s_CPU,s_MEM;

/*get the CPUand Memory constraints for any node*/

fscanf(fid,"%f%f", &s_CPU, &s_MEM);
/* get the servers cpu and memory arrays*/
for(i=0; i < s; i++)
{ s_cpu_array[i] = s_CPU;
  s_mem_array[i] = s_MEM;
}
/*----get the VMs time and the memory requirement -----*/ 

  for(i=0; i < jobs; i++) 
   
     fscanf(fid,"%f%f",&jobs_cpu_array[i],&jobs_mem_array[i]);      

/*---Close the file--*/ 
fclose(fid);
x = 0;
z = 0;
for(i=0;i<jobs;i++)
{                       
for(j=0;j<s;j++){
         
         if(s_cpu_array[j]>=jobs_cpu_array[i] && s_mem_array[j]>=jobs_mem_array[i])
         {
            s_cpu_array[j]-=jobs_cpu_array[i];
            s_mem_array[j]-=jobs_mem_array[i];
          printf("Allocating job %d to server %d\n Size remaining of CPU and Memory in it after allocation [%f,%f]\n",i+1,j+1,s_cpu_array[j],s_mem_array[j]);   
         if(x <= j+1){
            x=j+1;}
	  break;            
             }      
      if(j==s)  /* There is no enough servers to host all jobs*/
         {printf("Not enough memory for process %d",i);
          break;} 
if(x >= z)
     {z=x;
            }else
             { z=0;} } 
   }  

printf("The number of needed servers to save the %d jobs is: %d\n", jobs,z);
}
