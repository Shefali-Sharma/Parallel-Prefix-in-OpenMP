/*  Shefali
 *  Sharma
 *  sharma92
 */

#include <omp.h>
#include <math.h>
#ifndef A0_HPP
#define A0_HPP

template <typename T, typename Op>
void omp_scan(int n, const T* in, T* out, Op op) {
    
    int *temp_sum;
    
    //    omp_set_num_threads(32);
    
    #pragma omp parallel
    {
        //    omp_set_num_threads(2);
        int thread_num = omp_get_thread_num();
        int thread_total = omp_get_num_threads();
        
        int chunk_size = ceil(((float) n)/((float) thread_total));
        
        #pragma omp single
        {
            //printf(" Num of threads = %d    Chunk size = %d\n", thread_total, chunk_size);
            
            int temp_sum_size = ceil(((float)n)/((float)thread_total));
            temp_sum = new int[temp_sum_size];
            
            //printf("temp_sum = %d \n", temp_sum_size);
            temp_sum[0] = in[0];
        }
        
        int sum = in[0]; 
        //        printf("Sum = in[ %d ] = %d \n", thread_num * chunk_size, sum);
        
        //        out[0] = in[0];
        //        out[thread_num * chunk_size] = sum;
        //        printf(" out [ %d ] = %d \n\n", thread_num * chunk_size, out[thread_num * chunk_size]);
        
        #pragma omp for schedule(static, chunk_size) nowait
        for(int i = 0; i<n; i++){
            //            printf(" i = %d\n", i);
            //            if(i == (thread_num * chunk_size) || (i%chunk_size == 0)){
            if(i%chunk_size == 0){
                sum = in[i];
                //        printf(" temp_sum [ i = %d * chunk_size =  %d  ] = %d \n", i, chunk_size, temp_sum[i/chunk_size]);
            }
            else{
                sum = op(sum, in[i]);
            }
            out[i] = sum;
            //            printf(" out [ %d ] = %d   ThreadID = %d \n", i, out[i], thread_num);
        }
        
        temp_sum[thread_num + 1] = sum;
        //    printf(" temp_sum [ %d + 1 ] = %d \n", thread_num, temp_sum[thread_num + 1]);
        #pragma omp barrier
        /*
         for(int j = 1; j<(floor(n/chunk_size)+1); j++){
         temp_sum[j] = op(temp_sum[j-1], out[])a
         }
         */
        //        printf("temp_sum[1] = %d", temp_sum[1]);
        int steps = temp_sum[1];
        //        printf(" 1. Steps = %d   Thread ID = %d \n", steps, thread_num);
        
        if(thread_num == 1){
            steps = temp_sum[1];
        }
        if(thread_num > 1){
            for(int i =2; i<=(thread_num); i++){
                steps = op(steps, temp_sum[i]);
            }
        }
        
        //    printf(" 2. Steps = %d   Thread ID = %d \n", step, thread_num);
        /*
         for(int i=1; i<(thread_num + 1); i++){
         step = op(offset, temp_sum[i]);
         }
         */
        #pragma omp for schedule(static, chunk_size)
        for(int i=0; i<n; i++){
            if(thread_num != 0){
                out[i] = op(out[i], steps);
                //    printf(" out [ %d ] = %d \n", i, out[i]);
            }
            //        printf(" out [ %d ] = %d      Thread ID = %d \n", i, out[i], thread_num);
        }
        
    }
    
    
} // omp_scan

#endif // A0_HPP
