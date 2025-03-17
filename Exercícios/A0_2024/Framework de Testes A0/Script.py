import subprocess
import random
import sys

import difflib
import jellyfish

#=================================================================================================================
#		VARIÁVEIS DE CONFIGURAÇÃO
#=================================================================================================================

EXEC_LINE = "./A0" #WIN: .\\A0.exe
TESTS_COMP = [" 10 50 505050 > OutputT1.txt", " 5 200 606060 > OutputT2.txt", " 1 4 808080 > OutputT3.txt"]
TESTS_DIFF = [(["OutputT1.txt"], ["ExecT1.txt"]), (["OutputT2.txt"], ["ExecT2.txt"]), (["OutputT3.txt"], ["ExecT3.txt"])]

#=================================================================================================================
# 		GREP
#=================================================================================================================

def grep_check(index, delete=False):

	print()
	for j in range(len(TESTS_DIFF[index][0])):
		flag1 = False
		flag2 = False
		
		print("== INÍCIO GREP:", TESTS_DIFF[index][0][j], "x", TESTS_DIFF[index][1][j], "==\n")
		print("=> TESTE DE DIFERENCIAÇÃO (EXISTE NO ARQUIVO GERADO, MAS NÃO NA REFERÊNCIA): ", TESTS_DIFF[index][0][j], " x ", TESTS_DIFF[index][1][j], "\n")
		try:
			result = subprocess.check_output("grep -vxFf " + TESTS_DIFF[index][1][j] + " " + TESTS_DIFF[index][0][j], shell=True)
			try:
				print(str(result, 'utf-8', 'ignore'))
			except:
				print(result)
		except subprocess.CalledProcessError as grepexc:                                                                                                   
			if (grepexc.returncode == 1):
				flag1 = True
				print("OK!\n")
			else:
				print("Código de erro retornado pelo grep: ", grepexc.returncode, "\n")
		
		print("=> TESTE DE DIFERENCIAÇÃO (EXISTE NA REFERÊNCIA, MAS NÃO NO ARTIGO GERADO): ", TESTS_DIFF[index][1][j], " x ", TESTS_DIFF[index][0][j], "\n")
		try:
			result = subprocess.check_output("grep -vxFf " + TESTS_DIFF[index][0][j] + " " + TESTS_DIFF[index][1][j], shell=True)
			try:
				print(str(result, 'utf-8', 'ignore'))
			except:
				print(result)
		except subprocess.CalledProcessError as grepexc: 
			if (grepexc.returncode == 1):
				flag2 = True
				print("OK!\n")
			else:
				print("Código de erro retornado pelo grep: ", grepexc.returncode, "\n")
		if  flag1 and flag2:
			print("Os arquivos são idênticos em conteúdo!\n")
		
		print("=== FIM GREP:", TESTS_DIFF[index][0][j], "x", TESTS_DIFF[index][1][j], "====\n")
		if delete:
			try:
				result = subprocess.check_output("rm " + TESTS_DIFF[index][0][j], shell=True)
			except:
				continue

#=================================================================================================================
# 		COEFICIENTE
#=================================================================================================================

def coef_check(index, delete=False, hard=False):
	
	print()
	for j in range(len(TESTS_DIFF[index][0])):
		print("== INÍCIO COEFICIENTES:", TESTS_DIFF[index][0][j], "x", TESTS_DIFF[index][1][j], "==\n")
		file_gen = open(TESTS_DIFF[index][0][j], "r+")
		data_gen = file_gen.read()
		if not hard:
			data_gen = data_gen.replace(" ", "")
			data_gen = data_gen.replace("\n", "")
		file_gen.close()

		file_base = open(TESTS_DIFF[index][1][j], "r+")
		data_base = file_base.read()
		if not hard:
			data_base = data_base.replace(" ", "")
			data_base = data_base.replace("\n", "")
		file_base.close()

		sm_ratio = difflib.SequenceMatcher(None, data_base, data_gen).ratio()
		js_ratio = jellyfish.jaro_similarity(data_base, data_gen)

		print("=> TESTE DE DIFERENCIAÇÃO (RAZÃO SEQMATCH):", sm_ratio,"\n")
		print("=> TESTE DE DIFERENCIAÇÃO (RAZÃO DE JARO):", js_ratio,"\n")

		print("=== FIM COEFICIENTES:", TESTS_DIFF[index][0][j], "x", TESTS_DIFF[index][1][j], "====\n")
		if delete:
			try:
				result = subprocess.check_output("rm " + TESTS_DIFF[index][0][j], shell=True)
			except:
				continue

#=================================================================================================================
#		VALGRIND
#=================================================================================================================

def valgrind_check():

	print("\n##================== TESTE: VALGRIND ==================##\n")
	try:
		result = subprocess.check_output("valgrind " + EXEC_LINE + TESTS_COMP[0], shell=True)
		print(str(res_compile, 'utf-8', 'ignore'))
	except:
		print(result)

	for file in TESTS_DIFF[0][0]:
		try:
			result = subprocess.call(["rm", file], stderr=subprocess.PIPE)
		except:
			continue
	print("\n##===========================================================##\n")

#=================================================================================================================
#		MAIN
#=================================================================================================================

#---
try:
	res_compile = subprocess.check_output("gcc -Wall C/A0-Ref.c -o A0-Ref", shell=True)
except:
	print("ERRO FATAL: A REFERÊNCIA NÃO COMPILOU!")
for t_exec in TESTS_COMP:
	try:
		result = subprocess.check_output("./A0-Ref" + t_exec, shell=True, timeout=10)
	except subprocess.CalledProcessError as grepexc:
		print("./A0-Ref" + t_exec)                                                                                                   
		print("ERRO FATAL - código de erro retornado pelo programa de referência: ", grepexc.returncode)
		exit()
for file_set in TESTS_DIFF:
	for index in range(len(file_set[0])):
		res_compile = subprocess.check_output("mv " + file_set[0][index] + " " + file_set[1][index], shell=True)
#---

if (sys.argv[1] == "make"):
	SOURCES = "make"
else:
	SOURCES = "gcc -Wall " + "".join([sys.argv[i] + " " for i in range(1, len(sys.argv))]) + " -o " + EXEC_LINE

print("\n##================== TESTE: Compilação ==================##\n")
try:
	if (sys.argv[1] == "make"):
		res_compile = subprocess.call(["make"], stderr=subprocess.PIPE)
	else:
		res_compile = subprocess.check_output(SOURCES, shell=True)
except:	
	print("ERRO FATAL: O PROGRAMA NÃO COMPILOU!")
	exit(1)
print("\n##===========================================================##\n")

t_i = 0
for t_exec in TESTS_COMP:
	result = None
	try:
		print("\n##================== TESTE: " + EXEC_LINE + t_exec + " ==================##")
		try:
			result = subprocess.check_output(EXEC_LINE + t_exec, shell=True, timeout=10)
		except subprocess.CalledProcessError as grepexc:                                                                                                   
			print("Código de erro retornado pelo programa: ", grepexc.returncode)
		except Exception as e:
			print("Parada por timeout! O teste não foi concluído.")
		print(str(result, 'utf-8', 'ignore'))
	except Exception as e:
		if result != None:
			try:
				print(str(result, 'utf-8', 'ignore'))
			except:
				print(result)

	grep_check(t_i)
	coef_check(t_i, True)
	valgrind_check()

	print("\n##================ FIM TESTE " + EXEC_LINE + t_exec + " =================##\n")
	t_i += 1

try:
	if SOURCES == "make":
		result = subprocess.call(["make", "clean"], stderr=subprocess.PIPE)
		result = subprocess.call(["rm", EXEC_LINE], stderr=subprocess.PIPE)
	else:
		result = subprocess.call(["rm", EXEC_LINE], stderr=subprocess.PIPE)

	for file_set in TESTS_DIFF:
		for file_name in file_set[1]:
			res_compile = subprocess.check_output("rm " + file_name, shell=True)
	res_compile = subprocess.check_output("rm A0-Ref", shell=True)
except:
	pass