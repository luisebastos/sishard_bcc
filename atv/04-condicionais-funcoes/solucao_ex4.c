/* Implementar função solucao_ex4 */
int solucao_ex4(int a, char b) {
    if (a <= 17)
        return 18 - a; 
    b = b - 65;  
    if (b > 1)
        return -1; 
    return a - 17;  
}


