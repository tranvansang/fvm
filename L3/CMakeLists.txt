SET(CMAKE_C_COMPILER icc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -D__PURE_INTEL_C99_HEADERS__ -O0 -qopenmp -ipo -xCORE-AVX2 -align")

add_executable(src0
        src0/input.c
        src0/pointer_init.c
        src0/boundary_cell.c
        src0/cell_metrics.c
        src0/mc.c
        src0/cm.c
        src0/rcm.c
        src0/cmrcm.c
        src0/poi_gen.c
        src0/solver_ICCG_mc.c
        src0/outucd.c
        src0/allocate.c
        src0/main.c)

add_executable(reorder0
        reorder0/input.c
        reorder0/pointer_init.c
        reorder0/boundary_cell.c
        reorder0/cell_metrics.c
        reorder0/mc.c
        reorder0/cm.c
        reorder0/rcm.c
        reorder0/cmrcm.c
        reorder0/poi_gen.c
        reorder0/solver_ICCG_mc.c
        reorder0/outucd.c
        reorder0/allocate.c
        reorder0/main.c)
