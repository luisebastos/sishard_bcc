// Seu código da funcao1_solucao
long funcao1_solucao(long rdi, long rsi){
    long a = 0; 
    while (rdi >= rsi){
        rdi = rdi / rsi; 
        a += 1; 
    }
    return a; 
}

