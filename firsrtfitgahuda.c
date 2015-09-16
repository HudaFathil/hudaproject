/*============================================================================ 
| (c) Resources Allocation in Virtualized Cluster
============================================================================*/ 

#include "ga.h" 

int eval_vms();    /*--- Forward declaration ---*/ 
                     /*these variables can be used in any part of this program*/ 

#define MAXVMS 700 
#define MAXSER 700   /* The maximum number of tasks */ 
struct{ 
	float mem,
              cpu; 
} VM[MAXVMS]; 
struct{ 
	float mem,
              cpu; 
} Serv[MAXSER]; 

int Num_VMs; 
int Num_Serv;
float Node_CPU;
float Node_MEM;
int vm_id[MAXVMS];
int bin_id[MAXVMS];

/*---------------------------------------------------------------------------- 
| main() 
----------------------------------------------------------------------------*/ 
main(argc, argv) 
   int  argc; 
   char *argv[]; 
{ int i;
   GA_Info_Ptr ga_info; 

   /*--- Initialize the genetic algorithm ---*/ 
   printf("Reading GA config\n"); 
   ga_info = GA_config("vm.cfg", eval_vms); 
   printf("GA config read successfully\n"); 

   read_vms(ga_info->user_data); 
   
   ga_info->chrom_len = Num_VMs; 

if(argc > 1) 
 {X_select(ga_info, argv[1]); 
 }; 
 
 /*--- Run the GA ---*/ 
  printf("Running GA\n"); 
   GA_run(ga_info); 
printf("---------------------------------\n");
printf("[%d] of VMS are to deploy over [%d] servers\n", Num_VMs, Num_Serv);
printf("---------------------------------\n");
printf("Total Used Nodes [%d]",ga_info->best->nodes);
printf("---------------------------------\n");

printf("The VMs will be distributed over server nodes as:\n");
printf("---------------------------------\n");
printf("[");     
for (i=0;i<Num_VMs;i++)
     {
          printf("[%d to %d],",vm_id[i],bin_id[i]);
     }
printf("]\n");
printf("GA run completed\n");
     
} 

/*---------------------------------------------------------------------------- 
| obj_fun() - user specified objective function 
----------------------------------------------------------------------------*/ 
int eval_vms(chrom) 
   Chrom_Ptr chrom; 
{ 
int i, x,no_of_nodes, j, z; 
int vm_index[chrom->length];/*temp var to get vm index*/
int bin_used[chrom->length];
double fit_fun; 
float vm_cpu,vm_mem;
/*initiate  the servers cpu and memory arrays*/
for(i=0; i < chrom->length ; i++)
{ Serv[i].cpu = Node_CPU;
  Serv[i].mem = Node_MEM;
}

/* Trivial case no VMs */ 
   if(chrom->length < 1)  { 
	chrom->fitness = 0; 
	    return; 
   } 

/* Initialization*/ 
x = 0;
no_of_nodes = 0;

/*Place each Task by first-fit Its 2-D Multi Capacity bin Packing */ 
for(i = 0; i < chrom->length; i++)  {
   	vm_mem = VM[(int)chrom->gene[i]-1].mem; 
        vm_cpu = VM[(int)chrom->gene[i]-1].cpu;
        vm_index[i] = (int)chrom->gene[i];
/* ---Place VM on the first fit Bin----*/ 
   
  for(j=0;j<Num_Serv;j++)
     {
         
         if(Serv[j].cpu>= vm_cpu && Serv[j].mem>=vm_cpu)
         {
            Serv[j].cpu-=vm_cpu;
            Serv[j].mem-=vm_mem;
            bin_used[i] = j+1;   
         if(x <= j+1){
            x=j+1;}

	  break;            
             }      
   if(j==Num_VMs)  /* There is no enough servers to host all jobs*/
         {printf("Not enough memory for process %d",i);
          break;} 
     
   }  
if(x >= no_of_nodes)
            {no_of_nodes = x;
            }else
             { z=0;
           }
        }
for(i=0;i<chrom->length;i++) 
{
vm_id[i] = vm_index[i];
bin_id[i] = bin_used[i];
}      /* copy 1D array to another*/
chrom->fitness = no_of_nodes;
chrom->nodes = no_of_nodes; 
} 

/*--------------------------------------------------------------------------------------------- 
|read data from file 
---------------------------------------------------------------------------------------------------*/ 

read_vms(filename) 
 char *filename; 
{ 
  FILE *fid; 
  int i; 
 
  /*--open datafile----*/ 
  if((fid = fopen(filename,"r")) == NULL) 
  { 
   printf("Errors in opening the packages datafile <%s>\n", filename); 
   exit(1); 
  } 

 /*--get number of Nodes----*/ 
fscanf(fid,"%d",&Num_Serv);

  /*--get number of VMs----*/ 
fscanf(fid,"%d", &Num_VMs); 
  
  if(Num_VMs < 1 || Num_VMs > MAXVMS) 
  { 
    printf("Number of VMS ,%d, out of bounds [1...%d]\n", Num_VMs, MAXVMS); 
    exit(1); 
  } 

/*get the CPUand Memory constraints for any node*/

fscanf(fid,"%f%f", &Node_CPU, &Node_MEM);


/*----get the VMs time and the memory requirement -----*/ 

  for(i=0; i < Num_VMs; i++) 
   
     fscanf(fid,"%f%f",  &VM[i].cpu, &VM[i].mem); 
     
  

/*---Close the file--*/ 
fclose(fid); 
} 
