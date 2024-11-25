
set clk 4.166

open_project -reset "loc_glob_conv_proj"

set_top getCoords

add_files local_to_global.cpp
add_files -tb local_to_global_tb.cpp
add_files -tb mvtxPositions.txt
add_files -tb test_vec.txt

open_solution -reset "solution1"
set_part {xcku115-flvf1924-2-e}
create_clock -period ${clk} -name default

csim_design
csynth_design
exit

