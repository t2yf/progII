#!/bin/sh
clear
make

# tentar criar um arquivo sem .gbv
./gbv -a test.g a.txt


# add arquivo que não existe
valgrind ./gbv -a test.gbv d.txt e.txt

# listar algo que não existe
valgrind ./gbv -l test.gbv
valgrind ./gbv -l a.gbv

# ver .gbv que não existe
valgrind ./gbv -v test.gbv a.txt

# add 1 arquivo
valgrind ./gbv -a test.gbv a.txt

# add vários arquivos
valgrind ./gbv -a test.gbv b.txt lirio.txt pingu.jpeg sapinhoCifar10.png pingu_e_pinga.gif

# listar
valgrind ./gbv -l test.gbv
./gbv -l test.gbv

# ver algo que não existe
valgrind ./gbv -v test.gbv d.txt

# ver texto
valgrind ./gbv -v test.gbv lirio.txt

# ver imagem
valgrind ./gbv -v test.gbv pingu_e_pinga.gif

# remover do meio
valgrind ./gbv -r test.gbv lirio.txt
valgrind ./gbv -l test.gbv

# remover varios do meio
valgrind ./gbv -r b.txt pingu.jpeg
valgrind ./gbv -l test.gbv

# remover primeiro
valgrind ./gbv -r test.gbv a.txt
valgrind ./gbv -l test.gbv

# remover último
valgrind ./gbv -r test.gbv pingu_e_pinga.txt
valgrind ./gbv -l test.gbv

# remover primeiro
valgrind ./gbv -r test.gbv sapinhoCifar10.png
valgrind ./gbv -l test.gbv

# add arq vazio
valgrind ./gbv -a test.gbv empty.txt
valgrind ./gbv -v test.gbv empty.txt
valgrind ./gbv -r test.gbv empty.txt

make clean