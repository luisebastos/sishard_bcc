0x00000000000012b8 <+0>:     endbr64 
0x00000000000012bc <+4>:     mov    %rdi,%rax
0x00000000000012bf <+7>:     movslq %esi,%rsi
0x00000000000012c2 <+10>:    mov    $0x0,%edx
0x00000000000012c7 <+15>:    div    %rsi
0x00000000000012ca <+18>:    cmp    $0x1,%rdx
0x00000000000012ce <+22>:    je     0x12d6 <level4+30>
0x00000000000012d0 <+24>:    mov    $0x0,%eax
0x00000000000012d5 <+29>:    ret    
0x00000000000012d6 <+30>:    mov    $0x1,%eax
0x00000000000012db <+35>:    ret 


int solucao_4(long rdi, long rsi){
    long rax; 
    rax = rdi; 
    rdx = rax % rsi; 
    if (rdx == 0x1){
        rax = 0x1; 
    }
    return rax; 
}

___________________________________________________________________________

0x00000000000012dc <+0>:     endbr64 
0x00000000000012e0 <+4>:     lea    (%rdi,%rdi,4),%rax
0x00000000000012e4 <+8>:     lea    0x51(%rax,%rsi,4),%rax
0x00000000000012e9 <+13>:    cmp    $0x138aee6,%rax
0x00000000000012ef <+19>:    sete   %r8b
0x00000000000012f3 <+23>:    cmp    %rsi,%rdi
0x00000000000012f6 <+26>:    setne  %cl
0x00000000000012f9 <+29>:    test   %cl,%r8b
0x00000000000012fc <+32>:    je     0x131a <level5+62>
0x00000000000012fe <+34>:    movl   $0x0,(%rdx)
0x0000000000001304 <+40>:    cmp    $0xda43,%rax
0x000000000000130a <+46>:    setg   %al
0x000000000000130d <+49>:    cmp    $0xfffffffffffffff2,%rdi
0x0000000000001311 <+53>:    setl   %dl
0x0000000000001314 <+56>:    and    %edx,%eax
0x0000000000001316 <+58>:    movzbl %al,%eax
0x0000000000001319 <+61>:    ret    
0x000000000000131a <+62>:    movl   $0x1,(%rdx)

int solucao_5(){


    
}