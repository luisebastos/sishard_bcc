/* Implementar função solucao_ex1 */
int solucao_ex1(long rdi, long rsi, long rdx){
    long rax; 
    long rcx;
    rax = rdi + rsi; 
    rcx = rax + rdx * 4; 
    rdi = rdi * rdi; 
    rax = rdi + 2 * rsi;  // Correção aqui!
    rdx = rdx + rax; 
    if (rcx >= rdx){
        return 1; 
    }
    return 0; 
}
