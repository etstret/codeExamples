/*The implemented functionality includes events and queue profiling to allow for the calculation of memory transfer, kernel execution and total program operation times.
In the Kernal, implemented functionality includes a Parallel Sum algorithm, a Reduced Minimum and Maximum algorithm, and a Standard Deviation algorithm. */


#include <iostream>
#include <vector>

#include "Utils.h"

void print_help() {
	std::cerr << "Application usage:" << std::endl;

	std::cerr << "  -p : select platform " << std::endl;
	std::cerr << "  -d : select device" << std::endl;
	std::cerr << "  -l : list all platforms and devices" << std::endl;
	std::cerr << "  -h : print this message" << std::endl;
}

int main(int argc, char** argv) {
	int platform_id = 0;
	int device_id = 0;

	for (int i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "-p") == 0) && (i < (argc - 1))) { platform_id = atoi(argv[++i]); }
		else if ((strcmp(argv[i], "-d") == 0) && (i < (argc - 1))) { device_id = atoi(argv[++i]); }
		else if (strcmp(argv[i], "-l") == 0) { std::cout << ListPlatformsDevices() << std::endl; }
		else if (strcmp(argv[i], "-h") == 0) { print_help(); return 0; }
	}

	try {

		cl::Context context = GetContext(platform_id, device_id);
		std::cout << "Running on " << GetPlatformName(platform_id) << ", " << GetDeviceName(platform_id, device_id) << std::endl;

		cl::CommandQueue queue(context, CL_QUEUE_PROFILING_ENABLE);

		cl::Program::Sources sources;

		AddSources(sources, "kernels/kernel.cl");

		cl::Program program(context, sources);

		try {
			program.build();
		}
		catch (const cl::Error& err) {
			std::cout << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			std::cout << "Build Options:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			std::cout << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			throw err;
		}

		//File IO
		fstream infile("../temp_lincolnshire_datasets/temp_lincolnshire.txt"); //FStream the file in
		int b, c, d, e, f; //define columns 2,3,4,5,6 as ints
		string a, line; //first column is a string, line for iterator
		typedef int mytype;
		vector<mytype> tempData; //create the vector storing the temp data
		while (getline(infile, line)) {
			istringstream iss(line); //define the line as a stringstream
			iss >> a >> b >> c >> d >> e >> f; //columns
			if (infile.eof()) { break; } //break at end of file to avoid out of bounds
			tempData.push_back(f); //add the temp data to vector
		}


		//Padding, check if total size of data is divisible, add if not.
		//Padding adds neutral elements to dataset so as to allow for even workgroups. Neutral elements have little effect on totals.
		size_t local_size = 10;

		size_t padding_size = tempData.size() % local_size;

		if (padding_size) {
			std::vector<int> tempData_ext(local_size - padding_size, 0);
			tempData.insert(tempData.end(), tempData_ext.begin(), tempData_ext.end());
		}

		size_t input_elements = tempData.size();
		size_t input_size = tempData.size() * sizeof(mytype);
		size_t nr_groups = input_elements / local_size;


		//Create output vectors
		std::vector<mytype> B(1);
		size_t output_size = B.size() * sizeof(mytype);
		std::vector<mytype> C(1);
		std::vector<mytype> D(1);
		std::vector<mytype> E(1);

		cl::Event bufferEvent[6]; //Create 6 events to link to each buffer


		//Create output buffers B-E and Buffer A and Mean as input buffers
		cl::Buffer buffer_A(context, CL_MEM_READ_ONLY, input_size);
		cl::Buffer buffer_Mean(context, CL_MEM_READ_ONLY, output_size);
		cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, output_size);
		cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, output_size);
		cl::Buffer buffer_D(context, CL_MEM_READ_WRITE, output_size);
		cl::Buffer buffer_E(context, CL_MEM_READ_WRITE, output_size);


		//Fill buffer A with the tempData, fill buffers B-E with temporary data
		queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, input_size, &tempData[0], NULL, &bufferEvent[0]);
		queue.enqueueFillBuffer(buffer_B, 0, 0, output_size);
		queue.enqueueFillBuffer(buffer_C, 0, 0, output_size);
		queue.enqueueFillBuffer(buffer_D, 0, 0, output_size);
		queue.enqueueFillBuffer(buffer_E, 0, 0, output_size);


		//Create the kernels
		cl::Kernel kernel_1 = cl::Kernel(program, "gpuSum");
		cl::Kernel kernel_2 = cl::Kernel(program, "reduceMinInt");
		cl::Kernel kernel_3 = cl::Kernel(program, "reduceMaxInt");
		cl::Kernel kernel_4 = cl::Kernel(program, "stdDev");
		//Set arguments for each kernal. Input Buffer, Output Buffer, size.
		kernel_1.setArg(0, buffer_A);
		kernel_1.setArg(1, buffer_B);
		kernel_1.setArg(2, cl::Local(local_size * sizeof(mytype)));

		kernel_2.setArg(0, buffer_A);
		kernel_2.setArg(1, buffer_C);
		kernel_2.setArg(2, cl::Local(local_size * sizeof(mytype)));

		kernel_3.setArg(0, buffer_A);
		kernel_3.setArg(1, buffer_D);
		kernel_3.setArg(2, cl::Local(local_size * sizeof(mytype)));
		

		cl::Event kernelevent[4]; //Create 4 events to link to kernels
		queue.enqueueNDRangeKernel(kernel_1, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(local_size), NULL, &kernelevent[0]); //Execute kernel
		queue.enqueueNDRangeKernel(kernel_2, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(local_size), NULL, &kernelevent[1]);
		queue.enqueueNDRangeKernel(kernel_3, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(local_size), NULL, &kernelevent[2]);

		queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, output_size, &B[0], NULL, &bufferEvent[1]); //Read output buffer
		queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, output_size, &C[0], NULL, &bufferEvent[2]);
		queue.enqueueReadBuffer(buffer_D, CL_TRUE, 0, output_size, &D[0], NULL, &bufferEvent[3]);

		int buffAMem = bufferEvent[0].getProfilingInfo<CL_PROFILING_COMMAND_END>() - bufferEvent[0].getProfilingInfo<CL_PROFILING_COMMAND_START>(); //Get the memory transfer time for the buffer
		int buffBMem = bufferEvent[1].getProfilingInfo<CL_PROFILING_COMMAND_END>() - bufferEvent[1].getProfilingInfo<CL_PROFILING_COMMAND_START>();
		int buffCMem = bufferEvent[2].getProfilingInfo<CL_PROFILING_COMMAND_END>() - bufferEvent[2].getProfilingInfo<CL_PROFILING_COMMAND_START>();
		int buffDMem = bufferEvent[3].getProfilingInfo<CL_PROFILING_COMMAND_END>() - bufferEvent[3].getProfilingInfo<CL_PROFILING_COMMAND_START>();

		int sumKernel1 = buffAMem + buffBMem; //Combine the buffer memory transfer time for each vector to get the kernel memory transfer time
		int sumKernel2 = buffAMem + buffCMem;
		int sumKernel3 = buffAMem + buffDMem;

		int kernel1ExeTime = kernelevent[0].getProfilingInfo<CL_PROFILING_COMMAND_END>() - kernelevent[0].getProfilingInfo<CL_PROFILING_COMMAND_START>(); //Get the execution time of the kernel by subtracting the start time from the end
		int kernel2ExeTime = kernelevent[1].getProfilingInfo<CL_PROFILING_COMMAND_END>() - kernelevent[1].getProfilingInfo<CL_PROFILING_COMMAND_START>();
		int kernel3ExeTime = kernelevent[2].getProfilingInfo<CL_PROFILING_COMMAND_END>() - kernelevent[2].getProfilingInfo<CL_PROFILING_COMMAND_START>();


		std::cout << "Mean = " << float(B[0]) / float(tempData.size()) << std::endl; //Calculate and output the mean of the reduced sum
		std::cout << "Kernel Execution Time [ns]: " << kernel1ExeTime << std::endl; //Output the kernel exec time 
		std::cout << "Memory Transfer Time [ns]: " << sumKernel1 << '\n'; //Output memory transfer time
		std::cout << "Min = " << C[0] << std::endl; //Output the minimum value in the dataset
		std::cout << "Kernel Execution Time [ns]: " << kernel2ExeTime << std::endl;
		std::cout << "Memory Transfer Time [ns]: " << sumKernel2 << '\n';
		std::cout << "Max = " << D[0] << std::endl; //Output the maximum value in the dataset
		std::cout << "Kernel Execution Time [ns]: " << kernel3ExeTime << std::endl;
		std::cout << "Memory Transfer Time [ns]: " << sumKernel3 << '\n';

		vector<mytype> mean; //Create a new vector to store the previously calculated mean
		mean.push_back(B[0] / tempData.size()); //Add the mean to the vector

		queue.enqueueWriteBuffer(buffer_Mean, CL_TRUE, 0, output_size, &mean[0], NULL, &bufferEvent[4]); //Create a new buffer for the mean

		//Set args for the new kernel
		kernel_4.setArg(0, buffer_A);
		kernel_4.setArg(1, buffer_E);
		kernel_4.setArg(2, buffer_Mean);
		kernel_4.setArg(3, cl::Local(local_size * sizeof(mytype)));

		queue.enqueueNDRangeKernel(kernel_4, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(local_size), NULL, &kernelevent[3]); //Attach a kernel event, start kernel

		queue.enqueueReadBuffer(buffer_E, CL_TRUE, 0, output_size, &E[0], NULL, &bufferEvent[5]);

		int buffMeanMem = bufferEvent[4].getProfilingInfo<CL_PROFILING_COMMAND_END>() - bufferEvent[4].getProfilingInfo<CL_PROFILING_COMMAND_START>();
		int buffEMem = bufferEvent[5].getProfilingInfo<CL_PROFILING_COMMAND_END>() - bufferEvent[5].getProfilingInfo<CL_PROFILING_COMMAND_START>();
		int kernel4ExeTime = kernelevent[3].getProfilingInfo<CL_PROFILING_COMMAND_END>() - kernelevent[3].getProfilingInfo<CL_PROFILING_COMMAND_START>();

		int sumKernel4 = buffAMem + buffMeanMem + buffEMem;

		int totalProgramOperation = sumKernel1 + sumKernel2 + sumKernel3 + sumKernel4 + kernel1ExeTime + kernel2ExeTime + kernel3ExeTime + kernel4ExeTime; //Sum of all kernel memory transfers and execution times = total program operation time

		std::cout << "Std. Dev = " << sqrt(E[0] / tempData.size()) << std::endl;
		std::cout << "Kernel Execution Time [ns]: " << kernel4ExeTime << std::endl;
		std::cout << "Memory Transfer Time [ns]: " << sumKernel4 << '\n';
		std::cout << "Total Program Operation Time [ns]: " << totalProgramOperation << std::endl;
	}


	catch (cl::Error err) {
		std::cerr << "ERROR: " << err.what() << ", " << getErrorString(err.err()) << std::endl;
	}

	return 0;
}