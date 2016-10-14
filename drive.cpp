#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <sstream>
#include <getopt.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>
using namespace std;
//#include <unistd.h>


#define no_argument 0
#define required_argument 1
#define optional_argument 2
/*
* Essa função recebe um arquivo e um vetor de strings 
* salvando as linhas dos arquivos como itens do vetor.
* 
* @param filePipe nome do arquivo passado por linha de comando
* @param lines vetor de strings
*/
void openFile(char** filePipe, std::vector<std::string> *lines){
	//std::vector<std::string> lines;
	std::vector<std::string> instruc, op1, op2, op3;
	string syscall  = "syscall";
	std::ifstream filePine;
	std::string line;
					string save;
					string save2;
    filePine.open(*filePipe);
    if (!filePine.is_open()){
        std::cout << "Arquivo " << *filePipe << " não foi encontrado" << std::endl;
    }else{
		std::cout << "Arquivo " << *filePipe << " foi encontrado" << std::endl;
		std::cout << "Prosseguindo operação: " << std::endl;
		while(!filePine.eof()){
			getline(filePine, line);
			// Retira as vírgulas do arquivo de entrada
			line.erase(std::remove(line.begin(), line.end(),','), line.end());
			lines->push_back(line);
			std::string teste = {"oi oi1 oi2 oi3"};
			std::istringstream ss {line};
			string token;
			int aux = 0;
			while (ss >> token){
				if (aux == 0){
					instruc.push_back(token);
					aux++;
				}else if (aux == 1){
					if (instruc[instruc.size()-1] == syscall){
						 op1.push_back("$v0");
					}else if (instruc[instruc.size()-1] == "mult"){
						op1.push_back("HI");
						op2.push_back(token);
						aux++;
						//op2.push_back(token);
					}else if(instruc[instruc.size()-1] == "div"){
						op1.push_back("LO");
						op2.push_back(token);
						aux++;
					}else if (instruc[instruc.size()-1] == "mfhi"){
						op1.push_back(token);
						op2.push_back("HI");
						op3.push_back("\0");
						break;
					}else if (instruc[instruc.size()-1] == "j"){
						op1.push_back("LABEL");
						op2.push_back("\0");
						op3.push_back("\0");
						break;
					}else if(instruc[instruc.size()-1] == "lw"){
						op2.push_back(token);
					}else if(instruc[instruc.size()-1] == "beq" || instruc[instruc.size()-1] == "bne"){
						op2.push_back(token);
					}else{
						op1.push_back(token);
					}
					//op1.push_back(token);
					aux++;
				}else if(aux == 2){
					if (instruc[instruc.size()-1] == syscall){
						 op2.push_back("$v0");
					}else if (instruc[instruc.size()-1] == "mult"){
						save2 = token;
						op2.push_back(save);
					}else if(instruc[instruc.size()-1] == "beq" || instruc[instruc.size()-1] == "bne"){
						op3.push_back(token);
					}else if(instruc[instruc.size()-1] == "lw"){
						op1.push_back(token);
						break;
					}else{
						op2.push_back(token);
					}
					aux++;

				}else if(aux == 3){
					if (instruc[instruc.size()-1] == syscall){
						 op3.push_back("\0");
					}else if (instruc[instruc.size()-1] == "j"){
						op3.push_back("\0");
					 }else if(instruc[instruc.size()-1] == "mfhi"){
						op3.push_back("\0");
					}else if (instruc[instruc.size()-1] == "mult"){
						op3.push_back(token);
					}else if(instruc[instruc.size()-1] == "div"){
						op3.push_back(token);
					}else if(instruc[instruc.size()-1] == "beq" || instruc[instruc.size()-1] == "bne"){
						op1.push_back(token);
					}else if(instruc[instruc.size()-1] == "lw"){
						op3.push_back(token);
					}else{
						op3.push_back(token);
					}
					aux++;
				}else{
					aux = 0;
				}
			}
			
		}
	}
	for (unsigned int j = 0; j < instruc.size(); j++){
		std::cout << instruc[j] << std::endl;
		std::cout << op1[j] << std::endl;
		std::cout << op2[j] << std::endl;
		std::cout << op3[j] << std::endl;
		std::cout << "---------------------" << std::endl;
	}
	filePine.close();
}

// Usar essa lógica para adicionar nos vetores (ver se da certo)
void instruction(std::string string_0, std::string string1, std::string string2, std::string string3){
	std::string string0 = string_0;
	std::string dest;
	std::string op1, op2, op3; 
	// Padrões básicos com 3 registradores
	if (string0 == "add" || string0 == "sub" || string0 == "and" || string0 == "or" || string0 == "slt"){
		dest = string1;
		op1 = string2;
		op2 = string3;
	// Padrões com 2 registradores
	}else if(string0 == "addi" || string0 == "ori" || string0 == "sll" || string0 == "slr"){
		dest = string1;
		op1 = string2;
		op2 = string3;
		//Syscall
	}else if(string0 == "syscall"){
		dest = "$v0";
		op1 = "$v0";
		op2 = nullptr;
		// Jump
	}else if(string0 == "j"){
		dest = string1;
		op1 = nullptr;
		op2 = nullptr;
	}else if(string0 == "mfhi"){
		dest = string1;
		op1 = "LO";
		op2 = nullptr;
	}else if(string0 == "mult" || string0 == "div"){
		dest = "HI LO";
		op1 = string1;
		op2 = string2;
	}else if(string0 == "lw"){
		dest = string2;
		op1 = string1;
		op2 = nullptr;
	}else if(string0 == "beq" || string0 == "bne"){
		dest = string3;
		op1 = string1;
		op2 = string2;
	}else{
		dest = nullptr;
		op2 = nullptr;
		op3 = nullptr;
	}

}


void ajuda(){
	std::cout << "Sintaxe geral de comandos: " << std::endl;
	std::cout << "<executavel> <opcao> <nome_arquivo>" << std::endl;
	std::cout << "\n";
	std::cout << "Os comandos permitidos são: " << std::endl;
	std::cout << "-i" << "     "<<"<nome_arquivo>" << "Insere arquivo contendo lista de instruções" << std::endl;
}


int main(int argc, char * argv[]){
	std::string teste;
	std::vector<std::string> lines;
	const struct option longopts[] = {
	    {"inserir",        required_argument,        0, 'i'},
    	{0,0,0,0},
	};
	bool iflag = false;
	int index;
	int iarg = 0;
	opterr = 0;
	//Fim variáveis	
	while (iarg != -1){
		iarg = getopt_long_only(argc, argv, "i", longopts, &index);
		switch(iarg){
			case 'i':
			//std::cout << "Entrou I" << std::endl;
				if (iflag){
					std::cout << "Flag repetida" << std::endl;
					return 0;
				}
				iflag = true;
				index = optind;
				openFile(&argv[index], &lines);
				break;
			case '?':
				std::cout << "Comando inválido" << std::endl;
				ajuda();
				break;
		}
	}
	for (unsigned int i = 0; i < lines.size(); i++){
			std::cout << lines[i] << std::endl;
	}
	teste = atoi(lines[0].c_str());
	std::cout << teste << std::endl;
	return 0;
}

