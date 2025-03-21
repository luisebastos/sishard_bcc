long vezes2(long a);

/* Implementar função solucao_ex2 */

long solucao_ex2(long rdi, long rsi) { 
    long rbx; 
    long rax; 

    rbx = rdi;  
    rdi = rsi;  

    rax = vezes2(rdi);  

    if (rax > rbx) {  
        rbx = rbx + rbx; 
    }   

    return rbx + rax; 
}


