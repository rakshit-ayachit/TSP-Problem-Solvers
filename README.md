# Traveling Salesman Problem Solvers

This repository contains three different implementations of the Traveling Salesman Problem (TSP) using parallel computing techniques: OpenMP, MPI, and CUDA. Each implementation demonstrates a different approach to solving the TSP efficiently using parallelization.

## Key Features

- **OpenMP Implementation**: Parallelizes the TSP solution using OpenMP for shared-memory parallelism.
- **MPI Implementation**: Uses MPI for distributed-memory parallelism, allowing the TSP to be solved across multiple nodes.
- **CUDA Implementation**: Leverages the power of GPUs to solve the TSP using CUDA.

## Significance and Impact

The Traveling Salesman Problem is a classic optimization problem with significant practical applications in logistics, manufacturing, and DNA sequencing. By exploring different parallel computing techniques, this project provides insights into how computational efficiency can be greatly enhanced, which is crucial for large-scale, real-world problems.

## Prerequisites

- C/C++ Compiler (GCC, MSVC, etc.)
- OpenMP library
- MPI library (e.g., MPICH or OpenMPI)
- CUDA Toolkit and a compatible NVIDIA GPU
- CMake (optional, for building the project)

## Getting Started and Running the Application

### OpenMP Implementation

1. **Compile and Run:**
    ```sh
    gcc -fopenmp -o tsp_openmp tsp_openmp.c
    ./tsp_openmp
    ```

### MPI Implementation

1. **Compile and Run:**
    ```sh
    mpicc -o tsp_mpi tsp_mpi.c
    mpirun -np 10 ./tsp_mpi
    ```

### CUDA Implementation

1. **Compile and Run:**
    ```sh
    nvcc -o tsp_cuda tsp_cuda.cu
    ./tsp_cuda
    ```

## Expected Performance and Timing Analysis


| Implementation | Computational Resources Used | Expected Performance | Minimum Cost | Time Taken (seconds) |
|----------------|------------------------------|----------------------|--------------|----------------------|
| **OpenMP**     | Multi-core CPU               | Medium               | 2198         | 27.591               |
| **MPI**        | Multi-node Cluster           | High                 | 2198         | 21.835               |
| **CUDA**       | GPU                          | Very High            | 2198         | 6.307                |


- **OpenMP**: Suitable for shared-memory systems. Performance improves with the number of CPU cores but is limited by memory bandwidth and cache coherence.
- **MPI**: Ideal for distributed-memory systems. Performance scales with the number of nodes, but communication overhead can be a bottleneck.
- **CUDA**: Best for massively parallel computations on GPUs. Achieves significant speedup due to the high number of cores and parallel execution, but performance depends on efficient memory access patterns.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes.
4. Commit your changes (`git commit -m 'Add some feature'`).
5. Push to the branch (`git push origin feature-branch`).
6. Open a pull request.

## Contributors and Contact

- **Rakshit Ayachit** - _Project Maintainer_ - [rakshit.ayachit@gmail.com](mailto:rakshit.ayachit@gmail.com)
- **Sonakshi Badlani** - _Project Contributor_ - [sonakshibadlani@gmail.com](mailto:sonakshibadlani@gmail.com)

For any questions or suggestions, feel free to contact us.

## Project Status

The project is currently in the initial implementation phase. Future improvements include optimizing performance, adding more documentation, and extending the code to handle larger instances of TSP.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
