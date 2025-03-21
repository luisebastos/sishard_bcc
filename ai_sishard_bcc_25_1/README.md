# Prova Sistemas Hardware Software

<!-- markdown_py README.md > README.html -->
<!-- pandoc README.md -f markdown -t html -s -o README.html --metadata title="AI SisHard BCC 25-1" -->

### Duração: 120 minutos
### 3 Questões

**ATENÇÃO:** O SMOWL estará com tempo superior a 120 minutos, desconsidere.

* Em cada questão é indicado um arquivo para colocar sua resposta.
* É permitido consultar o material da disciplina durante a prova (repositório da disciplina e seu repositório de atividades), além de seus materiais físicos. **Ficam proibidas consultas a materiais de outros alunos**
* Pesquisas no **Google** para consultar o manual de assembly são aceitas; pesquise algo como "nome do comando" assembly x86.
* Fica proibido o uso de ferramentas de IA ou auto-complete
* A prova é individual. Qualquer consulta a outras pessoas durante a prova constitui violação ao Código de Ética do Insper. Não abra E-mail, Whatsapp, Discord, etc.
* A prova deve ser um exercício intelectual seu. Fica proibido utilizar programas ou qualquer outra ferramenta que resolva para você os exercícios (engenharia reversa).
* Fica proibido o uso de ferramentas de geração de código como o copilot. Sujeito a código de ética.
* Para conversões de base, pode utilizar a calculadora do Ubuntu.

## Instalação

Se receber reclamações como `cannot find -lsystemd`, instale com:

`sudo apt update`

`sudo apt install build-essential libsystemd-dev`

## Questão 01

Se não conseguir rodar o executável `./q1`, execute o comando:

`chmod +x q1`

Neste exercício, você deve responder `V` ou `F` para as afirmações retornadas.

Passe suas respostas para o arquivo `solucao.txt`, uma resposta em cada linha (deixe uma linha em branco no final) e execute com

./q1 < solucao.txt

**Onde deixo minhas respostas?**: no arquivo `solucao.txt`. Uma resposta em cada linha, com uma linha em branco no final!

**Critérios de avaliação**: sua nota será a proporção de respostas **corretas** (por motivos óbvios, correção posterior a prova)

**Pontos possíveis**: 2,0


## Questão 02

Se não conseguir rodar o executável `./q2`, execute o comando:

`chmod +x q2`

Este exercício é um mini **HackerLab**. Utilize seus conhecimentos adquiridos para descobrir a senha de cada nível!

Coloque as respostas no arquivo `solucao.txt` e rode

> `./q2 < solucao.txt`

**Preciso fazer engenharia reversa?** Você vai precisar analisar o assembly para entender as operações envolvidas, mas não precisa entregar esta tradução. Sua entrega deve ser apenas as entradas no arquivo `solucao.txt`.

**Onde deixo minhas respostas?**: no arquivo `solucao.txt`

**Critérios de avaliação**: A nota será disponibilizada na saída padrão!

**Pontos possíveis**: 4,0


## Questão 3

Faça engenharia reversa da função `funcao1` contida em `q3.o`.

Coloque sua solução no arquivo `solucao.c`, chamando sua função de `funcao1_solucao`.

Para compilar os testes e sua solução use

> `gcc -Og -g -fno-stack-protector solucao.c q3.o -lsystemd -lm -o q3`

Este exercício possui testes automáticos para servir como auxílio.

Execute os testes automáticos com:
> `./q3`

Observe, nos critérios de avaliação, que acertar o cabeçalho da função e fazer uma versão legível do código também fazem parte da nota. Também escreva uma explicação breve e direta (uma frase ou linha) do que a função `funcao1` faz, no arquivo `explicacao.md`.


**Critérios de avaliação**:

* 50% - pela proporção de testes com PASS (sem truques para burlá-los)
* 20% - acertar os tipos das chamadas de função
* 20% - código legível (e correto), sem construções estranhas (entregue a melhor versão do seu exercício, se fez if-goto e depois C legível, entregue apenas C legível!)
* 10% - explicação do que a função faz

**OBS**:

* Os testes não são exaustivos. Você pode passar em todos e sua função ainda não estar completamente correta. Neste caso, a nota com "proporção de testes com PASS" ficará completa, mas as demais poderão sofrer desconto.

**Pontos possíveis**: 4,0

