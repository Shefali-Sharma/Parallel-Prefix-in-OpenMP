# Parllel-Prefix-in-OpenMP

## Objective

Task is to implement efficient C++/OpenMP parallel prefix function omp_scan. We make two assumptions: first, the number of input elements is much larger than the number of available processors, second, the number of available processors is relatively small.

## Description

Arguments of the omp_scan are as follows:

**T** : type of the elements on which prefix will be applied.

**Op** : type representing a binary associative operator compatible with T.

**n** : number of input elements.

**in** : pointer to the array of size n storing input items.

**out** : pointer to the output array of size n to store output prefix.

**op** : object representing associative operator of type Op. Just in case, invocation like out[1] = op(in[0], in[1]); applies the operator to items in[0] and in[1] and stores the result in out[1].

