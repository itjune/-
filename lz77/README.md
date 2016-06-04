Kind of lz77 compression. 

For every position _i_ in a given string find longest common substring starts in position _i_ that also occurs in previous part of string.

Algorithm uses suffix array, longest common prefix (Kasai), counting sort and sparse table over lcp array. 