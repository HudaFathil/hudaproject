/*============================================================================ 
| (c) Resources Allocation in Virtualized Cluster
============================================================================*/ 

#include "ga.h" 

int eval_vms();    /*--- Forward declaration ---*/ 
                     /*these variables can be used in any part of this program*/ 

#define MAXVMS 500    /* The maximum number of tasks */ 
struct{ 
	float time, 
	      mem,
               cpu;
         
} VM[MAXVMS]; 
 
int Num_VMs; 
int Num_Nodes;
float Node_CPU;
float Node_MEM;
float make_time;
float bins;

/*---------------------------------------------------------------------------- 
| main() 
----------------------------------------------------------------------------*/ 
main(argc, argv) 
   int  argc; 
   char *argv[]; 
{ 
   GA_Info_Ptr ga_info; 

   /*--- Initialize the genetic algorithm ---*/ 
   printf("Reading GA config\n"); 
   ga_info = GA_config("vmtest.cfg", eval_vms); 
   printf("GA config read successfully\n"); 
   read_vms(ga_info->user_data); 
   ga_info->chrom_len = Num_VMs; 

if(argc > 1) 
 {X_select(ga_info, argv[1]); 
 }; 
 
 /*--- Run the GA ---*/ 
   printf("Running GA\n"); 
   GA_run(ga_info); 

   printf("the Best Makespan=%f, The Best server required=%f\n", ga_info->best->Makespan, ga_info->best->nodes);
printf("The %d vms will be deploying to %f as:\n", Num_VMs,ga_info->best->nodes);

printf("GA run completed\n"); 
printf("The total Makespan=%f, The total server required=%f\n", make_time, bins);   
} 

/*---------------------------------------------------------------------------- 
| obj_fun() - user specified objective function 
----------------------------------------------------------------------------*/ 
int eval_vms(chrom) 
   Chrom_Ptr chrom; 
{ 
int i; 
float fit_fun, tot_time, max_time;
float tot_mem, vm_time, vm_mem, vm_cpu, tot_cpu, no_of_nodes; 

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
no_of_nodes =1.0;

/*--Place each Task using next fit Its 2-D Multi Capacity bin Packing---- */ 
for(i = 0; i < chrom->length; i++)  
     {
	vm_time = VM[(int)chrom->gene[i]-1].time; 
   	vm_mem = VM[(int)chrom->gene[i]-1].mem; 
        vm_cpu = VM[(int)chrom->gene[i]-1].cpu;
        
        /* ---Place VM on Bins----*/ 
   
   if(vm_mem + tot_mem > Node_MEM ||  vm_cpu + tot_cpu > Node_CPU ) 
     {                              /*Too muchmemory or CPU*/ 
    no_of_nodes++;   
      if(tot_time >= max_time)
	{
         max_time = tot_time;
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
} 
/*compare the last bin tot time longest time at the node--*/ 

if(tot_time >= max_time)
	{max_time = tot_time;} 
make_time = max_time;
bins = no_of_nodes;
fit_fun = no_of_nodes*max_time;
chrom->fitness = fit_fun;
chrom->Makespan = max_time;
chrom->nodes = no_of_nodes; 
} 
/*--------------------------------------------------------------------------------------------- 
|read data from file 
--------------------------------------------------------------------------------------
*/read_vms(filename) 
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
printf("Number of free nodes in the server are:,%d\n", Num_Nodes);

  /*--get number of VMs----*/ 
fscanf(fid,"%d", &Num_VMs); 
  
  if(Num_VMs < 1 || Num_VMs > MAXVMS) 
  { 
    printf("Number of VMS ,%d, out of bounds [1...%d]\n", Num_VMs, MAXVMS); 
    exit(1); 
  } 
printf("Number of VMS to deploy are,%d\n", Num_VMs);

/*get the CPUand Memory constraints for any node*/

fscanf(fid,"%f%f", &Node_CPU, &Node_MEM);


/*----get the VMs time and the memory requirement -----*/ 

  for(i=0; i < Num_VMs; i++) 
   
     fscanf(fid,"%f%f%f", &VM[i].time, &VM[i].cpu, &VM[i].mem); 
fclose(fid);     
                   /*---Close the file--*/  
} 
