inline __device__ __host__ int divCeil( int a, int b ) {return (a%b != 0)?(a/b+1):(a/b);}
