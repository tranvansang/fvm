//
// Created by transang on 7/28/18.
//

#include "pcg_ext.h"
#include "struct_ext.h"

 int seq_to_col(int seq_index){
    int color  = seq_index % NCOLORtot;
    int thread = seq_index / NCOLORtot;
    return color * PEsmpTOT + thread;
}

//inverse function of seq_to_col
 int col_to_seq(int col_index){
    int color = col_index % PEsmpTOT;
    int thread = col_index / PEsmpTOT;
    return color + thread * NCOLORtot;
}