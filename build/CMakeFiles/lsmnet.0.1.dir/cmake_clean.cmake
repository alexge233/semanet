file(REMOVE_RECURSE
  "liblsmnet.0.1.pdb"
  "liblsmnet.0.1.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang)
  include(CMakeFiles/lsmnet.0.1.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
