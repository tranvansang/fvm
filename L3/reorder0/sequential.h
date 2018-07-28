//
// Created by transang on 7/28/18.
//

#ifndef FVM_SEQUENTIAL_H
#define FVM_SEQUENTIAL_H

//convert index after sequential ordering backto index after re-coloring
//all (input, output) are 0-based index
inline int seq_to_col(int seq_index);

//inverse function of seq_to_col
inline int col_to_seq(int col_index);
#endif //FVM_SEQUENTIAL_H
