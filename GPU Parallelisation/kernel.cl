kernel void gpuSum(global const int* A, global int* B, local int* scratch) {
	int id = get_global_id(0);
	int lid = get_local_id(0);
	int N = get_local_size(0);

	scratch[lid] = A[id];

	barrier(CLK_LOCAL_MEM_FENCE);

	for (int i = 1; i < N; i *= 2) { //Iterate through workgroups, produce sum of reduction.
		if (!(lid % (i * 2)) && ((lid + i) < N)) 
			scratch[lid] += scratch[lid + i];

		barrier(CLK_LOCAL_MEM_FENCE);
	}

	if (!lid) {
		atomic_add(&B[0],scratch[lid]); //Add sum of all reductions to create total sum of dataset.
	}
}

kernel void reduceMinInt (global const* input, global int* output, local int* scratch){
	int id = get_global_id(0);
	int lid = get_local_id(0);
	int N = get_local_size(0);

	scratch[lid] = input[id];

	barrier(CLK_LOCAL_MEM_FENCE);

	for(int i = N / 2; i > 0; i /= 2){ //Iterate through workgroups, produce sum of	reduction
		if(lid < i){
			if(scratch[lid + i] < scratch[lid]){
				scratch[lid] = scratch[lid + i]; 
			}
		}
	}
	if(!lid){
		atomic_min(&output[0], scratch[lid]); //Find minimum of all reductions to find minimum of dataset.
	}
}

kernel void reduceMaxInt(global const* input, global int* output, local int* scratch){
	int id = get_global_id(0);
	int lid = get_local_id(0);
	int N = get_local_size(0);

	scratch[lid] = input[id];

	barrier(CLK_LOCAL_MEM_FENCE);

	for(int i = N / 2; i < 0; i /= 2){
		if(lid < i){
			if(scratch[lid + i] < scratch[lid]){
				scratch[lid] = scratch[lid + i]; 
			}
		}
	}
	if(!lid){
		atomic_max(&output[0], scratch[lid]); //Find maximum of all reductions to find maximum of dataset.
	}
}

kernel void stdDev(global const* A, global int* E, global int* mean, local int* scratch) {
    int id = get_global_id(0);
    int lid = get_local_id(0);
    int N = get_local_size(0);
    scratch[lid] = A[id];
    for (int i = 0; i < N; i++){
        scratch[i] = (scratch[i]-mean[0])*(scratch[i]-mean[0]); //Subtract sum of squares - mean comes from the mean calculated in host code.
    }

    barrier(CLK_LOCAL_MEM_FENCE);
    for (int i = 1; i < N; i *= 2) { //Parallel Sum
        if (!(lid % (i * 2)) && ((lid + i) < N))
            scratch[lid] += scratch[lid + i];
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if (!lid) {
        atomic_add(&E[0],scratch[lid]);
    }
}