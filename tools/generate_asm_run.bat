compiler-frontend.exe -i %1 -o %1.ll
llvm-as.exe -f %1.ll
lli.exe %1.bc