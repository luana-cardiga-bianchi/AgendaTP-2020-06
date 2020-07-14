////////////////////////////////
/// Inclusao das bibliotecas ///
////////////////////////////////
#include <string>
#include <iostream>

// Se estiver rodando no windows
// precisamos incluir a biblioteca
// de modo diferente

#ifdef __WIN32
	#include <mysql.h>
#else
	// Em outros sistemas, creio eu,
	// desse outro jeito
	#include <mysql/mysql.h>
#endif

///////////////////
/// Prototipos ////
///////////////////
void exibeMenu(); 
void desconectar();
void verCompromissos();
void mostrarErroDoMysql();
void adicionarCompromisso();

/////////////////////////////
/// Variaveis globais X<( ///
/////////////////////////////

/**
 * Contem a conexao com o banco de dados
 */
MYSQL *connexao;
bool conectado = false;

//////////////////
/// Constantes ///
//////////////////

const unsigned int ADICIONAR_COMPROMISSO = 0;
const unsigned int REMOVER_COMPROMISSO = 1;
const unsigned int MOSTRAR_COMPROMISSO = 2;
const unsigned int EDITAR_COMPROMISSO = 3;
const unsigned int SAIR = 4;

/**
 * Funcao principal
 */
int main(int argc, char **argv) {

	// Exibe a mensagem de boas-vindas
	std::cout << "Boas-vindas da agenda!" << std::endl;

	exibeMenu();

	// Guarda a opcao escolhida
	int opcao;

	// Entra em um loop infinito perguntando
	// o que o usuario deseja fazer
	while (true) {

		// Pergunta a opcao do usuario
		std::cin >> opcao;

		// Executa a opcao escolhida
		switch (opcao) {
			case ADICIONAR_COMPROMISSO:
				adicionarCompromisso();
				break;
			case REMOVER_COMPROMISSO:
				 removerCompromisso();
				 break;
			case MOSTRAR_COMPROMISSO:
				 mostrarCompromisso();
				 break;
			case EDITAR_COMPROMISSO:
				 editarCompromisso();
				 break;
		}

		// Exibe o menu novamente
		exibeMenu();
	}

	desconectar();
	return 0;
}

/**
 * Exibe um menu para o usuario
 */
void exibeMenu() {
	std::cout << "Escolha uma opcao:" << std::endl;
	std::cout << ADICIONAR_COMPROMISSO << " - Adicionar compromisso" << std::endl;
	std::cout << REMOVER_COMPROMISSO << " - Remover compromisso" << std::endl;
	std::cout << MOSTRAR_COMPROMISSO << " - Mostrar compromisso" << std::endl;
	std::cout << EDITAR_COMPROMISSO << " - Editar compromisso" << std::endl;
	std::cout << SAIR << " - SAIR" << std::endl;
}

/**
 * Mostra os erros que podem ocorrer na conexao
 * @param mysql
 */
void mostrarErroDoMysql(MYSQL *mysql) {
	printf("Erro: (%d) [%s] \"%s\"", mysql_errno(mysql), mysql_sqlstate(mysql), mysql_error(mysql));
	mysql_close(mysql);
	exit(-1);
}

/**
 * Conecta no banco de dados
 * @return - true: Conectou, false: Falha ao conectar
 */
bool conectar() {

	if (conectado) return true;

	connexao = mysql_init(NULL);
	if (!mysql_real_connect(connexao, "127.0.0.1", "root", "1234", "Agenda", 3306, NULL, 0)) {
		mostrarErroDoMysql(connexao);

		conectado = false;
		return false;
	}

	conectado = true;
	return true;
}

/**
 * Desconecta do banco de dados
 */
void desconectar() {
	mysql_close(connexao);
	conectado = false;
}

/**
 * Adiciona um compromisso
 */
void adicionarCompromisso() {

	// Verifica se a conexao foi realizada com sucesso
	if (!conectar()) {
		std::cout << "Falha ao conectar ao banco de dados!" << std::endl;
		return;
	}

	// Variaveis usadas para montar o query
	std::string dia, mes, ano, descricao;

	// Solicita o dia do compromisso
	std::cout << "Informe o dia:";
	std::cin >> dia;

	// Solicita o mes do compromisso
	std::cout << "Informe o mês:";
	std::cin >> mes;

	// Solicita o ano do compromisso
	std::cout << "Informe o ano:";
	std::cin >> ano;

	// Solicita a descricao do compromisso
	std::cout << "Descreva o compromisso:";
	std::cin >> descricao;

	// Monta a query
	std::string sql = "insert into Compromisso (data, descricao)values('" + ano + "-" + mes + "-" + dia + "','" + descricao + "')";

	// Executa a query
	int statusDeExecucao = mysql_query(connexao, sql.data());

	// Verifica se deu tudo certo
	if (statusDeExecucao == 0) {
		// Deu certo!
		std::cout << "Compromisso inserido" << std::endl;
	} else {
		// Algo deu errado!
		mostrarErroDoMysql(connexao);
		std::cout << "Falha ao inserir compromisso!" << std::endl;
	}

}

void verificarCompromisso() {

	// Verifica se a conexao foi realizada com sucesso
	if (!conectar()) {
		std::cout << "Falha ao conectar ao banco de dados!" << std::endl;
		return;
	}

	// Guarda um registro
	MYSQL_ROW registro;

	// Guarda todos os registros retornados
	MYSQL_RES *resultados;

	// Executa a query
	int statusDeExecucao = mysql_query(connexao, "SELECT cod, data, descricao FROM Compromisso");

	// Verifica se deu tudo certo
	if (statusDeExecucao == 0) {

		// Deu certo! Recupera os registros retornados;
		resultados = mysql_store_result(connexao);

		// Mostra os registros retornados
		while ((registro = mysql_fetch_row(resultados))) {
			std::cout << registro[0] << "\t" << registro[1] << "\t" << registro[2] << std::endl;
		}
	} else {
		// Algo deu errado!
		mostrarErroDoMysql(connexao);
		std::cout << "Falha ao recuperar os registros!" << std::endl;
	}
}

void editarCompromisso ();{
	    // Pergunte qual dia, mes e ano deseja alterar
		std::cout <<"Informe o dia do compromisso:";
		std:cin>>dia;
		std::cout <<"Informe o mes do compromisso:";
		std:cin>>mes;
		std::cout <<"Informe o ano do compromisso:";
		std:cin>>ano;
		}

std::cout << "Informe o novo mes do compromisso:";

std::cin >> mes;


// Solicite o novo ano do compromisso

std::cout << "Informe o novo ano do compromisso:";

std::cin >> ano;


// Solicite uma nova descrição do compromisso

std::cout << "Descreva o compromisso:";

std::cin >> descricao;


// Mostre a query

std::string sql = "update Compromisso set data = " + ano + "-" + mes + "-" + dia + ", descricao = " + descricao + " where cod = " + codigo;


// Execute a query

int statusDeExecucao = mysql_query(connexao, sql.data());

} else {

// Algo deu errado!

mostrarErroDoMysql(connexao);

std::cout << "Encontramos uma falha ao recuperar os registros!" << std::endl;

}

}

