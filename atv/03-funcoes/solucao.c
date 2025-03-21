// TODO implemente aqui uma funcao chamada "solucao"
int solucao(int edi, int rsi, int *rdx){
    int eax; 

    eax = rsi + rsi * 4; 
    eax += edi; 
    *rdx = eax; 
    eax = rsi + rsi * 2; 
    eax = eax + edi * 4; 

    return eax; 
}