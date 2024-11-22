
set clk 4.166

open_project -reset "loc_glob_conv_proj"

set_top getCoords

add_files local_to_global.cpp

open_solution -reset "solution1"
set_part {xcku115-flvf1924-2-e}
create_clock -period ${clk} -name default

csynth_design

