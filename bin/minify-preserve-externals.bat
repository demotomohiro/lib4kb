%~dp0glslpp.exe "%1" > "%2\%1"
%~dp0shader_minifier.exe --preserve-externals -o "%1.hpp" "%2\%1"
