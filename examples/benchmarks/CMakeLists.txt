
foreach(tut oop dense sparse-address sparse-custom)
   add_executable(${tut}-bench ${tut}.cpp)
endforeach()


