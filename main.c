#include "AssemblyHandler.c"
int main(){
    InstructionsArr* ir ;
    ir = fetch() ;
    InstructionArrPrint(ir);
    free(ir) ;
    return 0 ;
}
