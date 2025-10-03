# 📂 | Gerenciador GBV 
> **Programação II | A1 <br> Thamiris Yamate Fischer | GRR20243604**


## Arquivos e diretórios
- __`main.c` :__ Arquivo principal, recebe o comando do usuário e chama as funções para processar a instrução;
- __`gbv.h` :__ Contém a estrutura do diretório e dos documentos e os protótipos das operações;
- __`gbv.c` :__ Contém a implementação das operações,funções auxiliares das operações e funções para movimentação, escrita e leitura de documentos em disco; 
- __`util.h` :__ Contém os protótipos de funções auxiliares para manipulação do diretório e verificaçõs úteis;
- __`util.c` :__ Contém a implementação das funções do `util.h` ;
- __`Makefile` :__ Contém definições de comandos de compilação e regras de dependência. 
- __`testes` :__ Arquivos utilizados nos testes


## Implementação

### Alterações
- __`main.c` :__  
Após o `for` nas operações add (-a), remove (-r) e quando a operação é inválida o vetor de documentos alocado dinamicamente no diretório em RAM (lib) é liberado. 
<br>Caso a liberação do bloco de memória ficasse para as funções das operações, nos casos de adicionar ou remover mais de um documento, após a primeira operação não haveria mais o lib em RAM, assim não sendo possível executar o comando. 
<br> Já para o caso de operação inválida, o vetor não seria liberado sem alterações.

- __`gbv.h`__  e __`gbv.c` :__ <br>Nas funções das operações remove (-r) e view (-v) foram adicionados o parâmetro `const char *archive` para que seja possível acessar o arquivo `.gbv` e remover o documento do disco, no caso do remove e visualizar o documento dentro do .gbv, no caso do view.

- __`util.h` :__ e __`util.c` :__ <br>Foram adicionados funções úteis para manipular o diretório (lib) e verificar o gbv, por exemplo, para saber se ele está vazio e se a extensão está correta.

### Dificuldades
__1. Movimentação__ <br>Durante a implementação do inserir, principalmente da parte de substituição de documentos, e do remover percebeu-se a necessidade de movimentar os documentos. O maior desafio do código foi entender a lógica e implementar como movimentar em disco, no fim, tratei de forma genérica, ou seja, a movimentação não é feita membro a membro, mas sim considerando um bloco que deve ser movimentado, caso esse bloco seja maior que o buffer, será necessário movimentar aos poucos. 

__2. Manipular documentos grandes__ <br>Considerando um gerenciador genérico, documentos muito maiores que o buffer necessitam ser manipulados. Entretanto, foi um desafio pensar em como dividir o documento em partes menores para manipular pouco a pouco.

__3. Geral__ <br>Em geral, no início do trabalho o maior desafio foi interpretar o que fazer em cada operação e transportar as ideias para código.

__4. Testes__ <br>Pensar em casos de uso e considerar os limites do código, ou seja, usos inadequados e como tratá-los. Pode ser que nem todos os casos de uso e limites tenham sido testados.


### Bugs Conhecidos
Casos conhecidos já foram arrumados nas alterações mencionadas
