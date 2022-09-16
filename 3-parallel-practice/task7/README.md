# Ленточный алгоритм перемножения матриц на OpenMP и MPI

## Запуск

### OpenMP

`make openmp` - для компилятора clang++ (локально)    
`make openmp_xlc++` - для компилятора xlc++ (для запуска на Polus)   

### MPI

`make mpi arg1=<N> arg2=<SIZE>`, где N - число процессов, SIZE - размер матрицы     
`make mpi_test` - протестировать и записать результаты CSV файл
