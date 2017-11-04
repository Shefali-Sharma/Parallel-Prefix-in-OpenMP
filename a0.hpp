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
    #pragma omp parallel
    {
        int thread_num = omp_get_thread_num();
        int thread_total = omp_get_num_threads();
        
        int chunk_size = ceil(((float) n)/((float) thread_total));
        
        #pragma omp single
        {
            int temp_sum_size = ceil(((float)n)/((float)thread_total));
            temp_sum = new int[temp_sum_size];
            temp_sum[0] = in[0];
        }
        
        int sum = in[0];
        #pragma omp for schedule(static, chunk_size) nowait
        for(int i = 0; i<n; i++){
            if(i%chunk_size == 0){
                sum = in[i];
            }
            else{
                sum = op(sum, in[i]);
            }
            out[i] = sum;
        }
        
        temp_sum[thread_num + 1] = sum;
        #pragma omp barrier
        int steps = temp_sum[1];
        if(thread_num == 1){
            steps = temp_sum[1];
        }
        if(thread_num > 1){
            for(int i =2; i<=(thread_num); i++){
                steps = op(steps, temp_sum[i]);
            }
        }
        #pragma omp for schedule(static, chunk_size)
        for(int i=0; i<n; i++){
            if(thread_num != 0){
                out[i] = op(out[i], steps);
            }
        }
        
    }
    
    
} // omp_scan

#endif // A0_HPP
