/*============================================================================ 
| (c) Resources Allocation in Virtualized Cluster
============================================================================*/ 

#include "ga.h" 

int eval_vms();    /*--- Forward declaration ---*/ 
                     /*these variables can be used in any part of this program*/ 

#define MAXVMS 700   /* The maximum number of tasks */ 
struct{ 
	float  mem,
              cpu;
         } VM[MAXVMS]; 
float servers; 
int Num_VMs; 
int Num_Nodes;
float Node_CPU;
float Node_MEM;

int bin_id[MAXVMS];

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
int i;

printf("------------------------------------------\n");    printf("The Best Fitness Fun || The used servers\n");
printf(" -------------------  -----------------\n");
 
printf(" %f                     %d\n", ga_info->best->fitness, ga_info->best->nodes);
printf("--------------------------------------------\n");
printf(" %d VMs will be deploying to %f as:\n", Num_VMs, servers);
printf("----------------------------------------------\n");
printf("----------------------------------------------\n"); 
printf("{");
for (i=0; i<ga_info->chrom_len; i++)
{printf("[%G,%d]", ga_info->best->gene[i], bin_id[i]);
} 
printf("}\n");
printf("----------------------------------------------\n"); 
printf("GA run completed\n"); 

} 

/*---------------------------------------------------------------------------- 
| obj_fun() - user specified objective function 
----------------------------------------------------------------------------*/ 
int eval_vms(chrom) 
   Chrom_Ptr chrom; 
{ 
int i, no_of_nodes; 
double fit_fun, fit_fun_mem, fit_fun_cpu, fmem, fcpu;
float tot_mem, vm_mem, vm_cpu, tot_cpu;
float f_bin_cpu, f_bin_mem;
float tot_fbins_mem, tot_fbins_cpu; 

/* Trivial case no VMs */ 
   if(chrom->length < 1)  { 
	chrom->fitness = 0; 
	    return; 
   } 

/* Initialization*/ 
 
tot_mem = 0.0; 
tot_cpu = 0.0; 
no_of_nodes = 1;
f_bin_cpu = 0.0;
f_bin_mem = 0.0;
tot_fbins_mem = 0.0;
tot_fbins_cpu = 0.0;

/*--Place each Task using next fit Its 2-D Multi Capacity bin Packing---- */ 
for(i = 0; i < chrom->length; i++)  
     { 
   	vm_mem = VM[(int)chrom->gene[i]-1].mem; 
        vm_cpu = VM[(int)chrom->gene[i]-1].cpu;
        /* ---Place VM on Bins----*/ 
      if((vm_mem + tot_mem)> Node_MEM||(vm_cpu  + tot_cpu) > Node_CPU) 
     { 
      no_of_nodes++;
      fcpu = ((1 / Node_CPU) * tot_cpu);       
      f_bin_cpu = fcpu * fcpu; /* find fbcpu*/
       tot_fbins_cpu += f_bin_cpu;
       fmem = ((1 / Node_MEM) * tot_mem);
       f_bin_mem = fmem*fmem; /* find fb Mem*/           
       tot_fbins_mem += f_bin_mem;   
       bin_id[i] = no_of_nodes;
       tot_mem = vm_mem;
       tot_cpu   = vm_cpu;
       if((chrom->length - i) == 1) /*find fbR for last bin*/
         { 
          fcpu = ((1 / Node_CPU)* tot_cpu);
          f_bin_cpu = fcpu * fcpu; 
       	  tot_fbins_cpu += f_bin_cpu;
          fmem = ((1 / Node_MEM) * tot_mem);
          f_bin_mem = fmem * fmem;
       	  tot_fbins_mem += f_bin_mem;
         }	 
      } 
	else 
       { 
        bin_id[i] = no_of_nodes;
        tot_mem += vm_mem;
        tot_cpu += vm_cpu;
        if((chrom->length - i) == 1) /*find fbR for last bin */
          { 
           fcpu = ((1 / Node_CPU) * tot_cpu);
           f_bin_cpu = fcpu * fcpu; 
       	   tot_fbins_cpu += f_bin_cpu;
           fmem = ((1 / Node_MEM) * tot_mem);
           f_bin_mem = fmem * fmem;
       	   tot_fbins_mem += f_bin_mem;          
           }	      
         } 
}  
servers = no_of_nodes ;
fit_fun = (tot_fbins_mem+tot_fbins_cpu)/no_of_nodes;
chrom->fitness = fit_fun;
chrom->nodes = no_of_nodes; 
} 
/*--------------------------------------------------------------------------------------------- 
|read data from file   --------------------------------------------------------------------------------*/read_vms(filename) 
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

  /*--get number of VMs----*/ 
fscanf(fid,"%d", &Num_VMs); 
  
  if(Num_VMs < 1 || Num_VMs > MAXVMS) 
  { 
    printf("Number of VMS ,%d, out of bounds [1...%d]\n", Num_VMs, MAXVMS); 
    exit(1); 
  } 
printf("VMS to deploy      ||      No Free Nodes\n");
printf("-------------             --------------\n");
printf(" %d                        %d\n", Num_VMs,Num_Nodes);
/*get the CPUand Memory constraints for any node*/

fscanf(fid,"%f%f", &Node_CPU, &Node_MEM);


/*----get the VMs time and the memory requirement -----*/ 

  for(i=0; i < Num_VMs; i++) 
   
     fscanf(fid,"%f%f", &VM[i].cpu, &VM[i].mem); 
fclose(fid);     
                   /*---Close the file--*/  
} 
