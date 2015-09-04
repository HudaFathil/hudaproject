/*============================================================================ 
| (c) Resources Allocation in Virtualized Cluster
============================================================================*/ 

#include "ga.h" 

int eval_vms();    /*--- Forward declaration ---*/ 
                     /*these variables can be used in any part of this program*/ 

<<<<<<< HEAD
#define MAXVMS 600    /* The maximum number of tasks */ 
struct{ 
	float time, 
	      mem,
              cpu; 
=======
#define MAXVMS 100    /* The maximum number of tasks */ 
struct{ 
	float time, 
	      mem,
               cpu; 
>>>>>>> a5e427bb9ce6b924e382488f2021a684c57cece0
} VM[MAXVMS]; 

int Num_VMs; 
int Num_Nodes;
float Node_CPU;
float Node_MEM;
<<<<<<< HEAD
float make_time;
int vm_id[MAXVMS];
int bin_id[MAXVMS];

=======
>>>>>>> a5e427bb9ce6b924e382488f2021a684c57cece0
/*---------------------------------------------------------------------------- 
| main() 
----------------------------------------------------------------------------*/ 
main(argc, argv) 
   int  argc; 
   char *argv[]; 
<<<<<<< HEAD
{ int i;
=======
{ 
>>>>>>> a5e427bb9ce6b924e382488f2021a684c57cece0
   GA_Info_Ptr ga_info; 

   /*--- Initialize the genetic algorithm ---*/ 
   printf("Reading GA config\n"); 
<<<<<<< HEAD
   ga_info = GA_config("vmtest100.cfg", eval_vms); 
=======
   ga_info = GA_config("vmtest.cfg", eval_vms); 
>>>>>>> a5e427bb9ce6b924e382488f2021a684c57cece0
   printf("GA config read successfully\n"); 

   read_vms(ga_info->user_data); 
   
   ga_info->chrom_len = Num_VMs; 

if(argc > 1) 
 {X_select(ga_info, argv[1]); 
 }; 
 
 /*--- Run the GA ---*/ 
<<<<<<< HEAD
  printf("Running GA\n"); 
   GA_run(ga_info); 
printf("---------------------------------\n");
printf("[%d] of VMS are to deploy over [%d] servers\n", Num_VMs, Num_Nodes);
printf("---------------------------------\n");
printf("Total Used Nodes [%f]---Total Makespan[%f]\n",ga_info->best->nodes, ga_info->best->makespan);
printf("---------------------------------\n");
printf("The Make_time=%f\n", make_time);
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
     
=======
   printf("Running GA\n"); 
   GA_run(ga_info); 
   printf("the total Makespan=%f, the total server required=%f\n", ga_info->best->Makespan, ga_info->best->nodes);
   printf("GA run completed\n"); 
   
>>>>>>> a5e427bb9ce6b924e382488f2021a684c57cece0
} 

/*---------------------------------------------------------------------------- 
| obj_fun() - user specified objective function 
----------------------------------------------------------------------------*/ 
int eval_vms(chrom) 
   Chrom_Ptr chrom; 
{ 
<<<<<<< HEAD
int i; 
int vm_index[chrom->length];/*temp var to get vm index*/
int bin_used[chrom->length];
float fit_fun, fit_fun_mem, fit_fun_cpu;
double tot_mem, vm_time,tot_time, max_time;
float vm_cpu,vm_mem, tot_cpu, no_of_nodes;
float f_bin_cpu, f_bin_mem;
float tot_fbins_mem, tot_fbins_cpu;
 
=======
   int i; 
float fit_fun, tot_time, max_time;
float tot_mem, vm_time, vm_mem, vm_cpu, tot_cpu, no_of_nodes; 
>>>>>>> a5e427bb9ce6b924e382488f2021a684c57cece0

/* Trivial case no VMs */ 
   if(chrom->length < 1)  { 
	chrom->fitness = 0; 
	    return; 
   } 

/* Initialization*/ 

tot_time = 0.0; 
max_time = 0.0; 
tot_mem = 0.0; 
tot_cpu = 0.0; 
<<<<<<< HEAD
no_of_nodes = 1;
f_bin_cpu = 0.0;
f_bin_mem = 0.0;
tot_fbins_mem = 0.0;
tot_fbins_cpu = 0.0;

/*--Place each Task using next fit Its 2-D Multi Capacity bin Packing---- */ 
for(i = 0; i < chrom->length; i++)  {
	vm_time = VM[(int)chrom->gene[i]-1].time; 
   	vm_mem = VM[(int)chrom->gene[i]-1].mem; 
        vm_cpu = VM[(int)chrom->gene[i]-1].cpu;
        vm_index[i] = (int)chrom->gene[i];
 /* ---Place VM on Bins----*/ 
   
   if(vm_mem + tot_mem > Node_MEM ||  vm_cpu + tot_cpu > Node_CPU )   {                             /*--Too much memory or CPU---*/ 
       tot_time = 0.0;
       f_bin_cpu = (1/Node_CPU) * (tot_cpu * tot_cpu); /* find fbcpu*/
       tot_fbins_cpu += f_bin_cpu;
       f_bin_mem = (1/Node_MEM) * (tot_mem * tot_mem); /* find fb Mem*/
       tot_fbins_mem += f_bin_mem; 
       no_of_nodes++;
       bin_used[i] = no_of_nodes;
       tot_mem = vm_mem;
       tot_cpu   = vm_cpu;
       tot_time = vm_time;
       if(i - chrom->length  == -1) /* find fbR for last bin */
       {
	f_bin_cpu = (1/Node_CPU) * (tot_cpu * tot_cpu); 
       	tot_fbins_cpu += f_bin_cpu;
       	f_bin_mem = (1/Node_MEM) * (tot_mem * tot_mem);
       	tot_fbins_mem += f_bin_mem;
       }
       if(tot_time >= max_time)
          { max_time = tot_time;
           }	 
      } 
	else { 
              bin_used[i] = no_of_nodes;
	      tot_time += vm_time;
               tot_mem += vm_mem;
               tot_cpu += vm_cpu;
              if(i-chrom->length == -1) /* find fbR for last bin */
               {
		f_bin_cpu = (1/Node_CPU) * (tot_cpu * tot_cpu);
                tot_fbins_cpu += f_bin_cpu;			
                f_bin_mem = (1/Node_MEM) * (tot_mem * tot_mem); 
                tot_fbins_mem += f_bin_mem;
               }
		 if(tot_time >= max_time)
                 { max_time = tot_time;}
      } 
	} 
for (i = 0; i < chrom->length; i++) 
{
vm_id[i] = vm_index[i];
bin_id[i] = bin_used[i];
}      /* copy 1D array to another*/
make_time = max_time;
fit_fun_mem = (1/no_of_nodes) * tot_fbins_mem;
fit_fun_cpu = (1/no_of_nodes) * tot_fbins_cpu;
fit_fun = fit_fun_mem + fit_fun_cpu; 
chrom->fitness = fit_fun;
chrom->nodes = no_of_nodes; 
chrom->makespan = make_time;
=======
no_of_nodes =1.0;

/*--Place each Task using next fit Its 2-D Multi Capacity bin Packing---- */ 
for(i = 0; i < chrom->length; i++)  
     {
	vm_time = VM[(int)chrom->gene[i]-1].time; 
   	vm_mem = VM[(int)chrom->gene[i]-1].mem; 
        vm_cpu = VM[(int)chrom->gene[i]-1].cpu;
  
 /* ---Place VM on Bins----*/ 
   
   if(vm_mem + tot_mem > Node_MEM ||  vm_cpu + tot_cpu > Node_CPU ) 
{                              /*--Too much memory or CPU---*/ 
    no_of_nodes++;     
  if(tot_time >= max_time)
	{max_time = tot_time;
        }
       tot_time = 0.0;
       tot_mem = vm_mem;
       tot_cpu   = vm_cpu;
       tot_time = vm_time; 	 
      } 
	else { 
	   tot_time += vm_time;
               tot_mem += vm_mem;
               tot_cpu += vm_cpu;
	} 
} /*compare the last bin tot time longest time at the node--*/ 
if(tot_time >= max_time)
	{max_time = tot_time;} 
fit_fun = no_of_nodes*max_time;
chrom->fitness = fit_fun;
chrom->Makespan = max_time;
chrom->nodes = no_of_nodes; 
>>>>>>> a5e427bb9ce6b924e382488f2021a684c57cece0
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
fscanf(fid,"%d",&Num_Nodes);
<<<<<<< HEAD
=======
printf("Number of free nodes in the server are:,%d\n", Num_Nodes);
>>>>>>> a5e427bb9ce6b924e382488f2021a684c57cece0

  /*--get number of VMs----*/ 
fscanf(fid,"%d", &Num_VMs); 
  
  if(Num_VMs < 1 || Num_VMs > MAXVMS) 
  { 
    printf("Number of VMS ,%d, out of bounds [1...%d]\n", Num_VMs, MAXVMS); 
    exit(1); 
  } 
<<<<<<< HEAD
=======
printf("Number of VMS to deploy are,%d\n", Num_VMs);
>>>>>>> a5e427bb9ce6b924e382488f2021a684c57cece0

/*get the CPUand Memory constraints for any node*/

fscanf(fid,"%f%f", &Node_CPU, &Node_MEM);


/*----get the VMs time and the memory requirement -----*/ 

  for(i=0; i < Num_VMs; i++) 
   
     fscanf(fid,"%f%f%f", &VM[i].time, &VM[i].cpu, &VM[i].mem); 
     
  

/*---Close the file--*/ 
fclose(fid); 
} 
