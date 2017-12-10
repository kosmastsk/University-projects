SHELL := /bin/bash # Use bash syntax

CC = sm_20

NVCC	= nvcc -arch=$(CC)
SAMPLE	= Kernel1
KERNEL2 = Kernel2
ARGS	= -ptx



sample:
	$(NVCC) $(ARGS) $(SAMPLE).cu $(KERNEL2).cu
kernel2:
	$(NVCC) $(ARGS) $(KERNEL2).cu

clean:
	rm -rf *~ *.ptx
