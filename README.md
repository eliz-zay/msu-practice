# Moscow State University practice

Here I collect all the practices completed during my Bachelor degree studies in Moscow State University, in the department of Computational Mathematics and Cybernectics. The list of subjects covered:   

- System programming - C/C++, socket API
- Neural networks - Python3, Keras
- Mathematical models and algorithms - C++, OpenMP, MPI
- Parallel computations - POSIX threads, STD threads, MPI, OpenMP, CUDA
- Quantum informatics
- Computer graphics - OpenGL
- Distributed systems and algorithms - RabbitMQ
- Graphics programming on GPU and CPU - CUDA
- C/C++ code profiling and optimizing


## Second year

- System programming ([source code](2-sql-practice))  
    I have developed a query parser for Data Base Management System.

- Neural networks ([source code](2-neural-networks-practice))  
    I designed an architecture of convolutional neural network used for image analysis.


## Third year

- Parallel computations ([source code](3-parallel-practice))  
    The goal was to implement a number of distributed mathematical algorithms (such as matrix multiplication) using OpenMP, MPI, POSIX and STD threads and measure the efficiency of parallelizing.

- Quantum informatics ([source code](3-quantum-practice))  
    This practice was dedicated to implementation of various quantum algorithms using OpenMP and MPI.

- OpenGL practice ([source code](3-opengl-practice))  
    I have developed 2D game engine with a wide range of features, such as animation, UI system, map loader, etc.  
    I have also developed 2D roguelike multilevel animated game based on this engine.

- Ray tracing practice ([source code](3-ray-tracer-practice))  
    I have developed ray tracer with ray marching and anti-aliasing. The generated scene includes local and global lighting models, a glass pyramid, different kinds of surfaces and fire imitation.
    An example of rendered image can be found [here](3-ray-tracer-practice/rendered-image.png).  


## Fourth year

- Graphics programming on GPU and CPU ([source code](4-cuda-practice))  
    During this practice CUDA platform was used for image processing by applying kernels to input images.

- Distributed systems and algorithms ([source code](4-rabbitmq-practice))  
    In this subject RabbitMQ was used in various message-based distributed algorithms, such as worker pool, broadcasting techniques and logical(Lamport) clock approach.

- C/C++ code profiling and optimizing ([source code](4-graph-generator))  
    During this practice I have implemented a graph generation algorithm and parallelized it using STD threads.


## Graduate work ([source code](graduate-work))  
### Development of methods for detecting anomalies in supercomputer applications using monitoring data
I have developed a method which is used to analyse the applications timeline and detect an anomaly and its type. This method was tested on historical supercomputer data and showed 95% accuracy on over a 1000 applications.
