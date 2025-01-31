#version 430

layout(local_size_x = 1024) in;

layout(std430, binding = 0) coherent buffer block1 {
	float inputArray[gl_WorkGroupSize.x];
};

layout(std430, binding = 1) coherent buffer block2 {
	float outputArray[gl_WorkGroupSize.x];
};

shared float shared_data[gl_WorkGroupSize.x*2];

void main() {
	uint id = gl_LocalInvocationID.x
	uint rd_id;
	uint wr_id;
	uint mask;

	const uint steps = uint(log2(gl_WorkGroupSize.x))+1;
	uint step = 0;
	
	shared_data[id*2] = inputArray[id*2];
	shared_data[id*2 +1] = inputArray[id*2 +1];

	barrier();
	memoryBarrierShared();

	for(step = 0; step < steps; step++) {
		// Calculate the read and write index in the
        // shared array
        mask = (1 << step) - 1;
        rd_id = ((id >> step) << (step + 1)) + mask;
        wr_id = rd_id + 1 + (id & mask);

        // Accumulate the read data into our element
        shared_data[wr_id] += shared_data[rd_id];

        // Synchronize again to make sure that everyone
        // has caught up with us
        barrier();
        memoryBarrierShared();
	}

	// Finally write our data back to the output image
    output_data[id * 2] = shared_data[id * 2];
    output_data[id * 2 + 1] = shared_data[id * 2 + 1];
	
}

