/*Bibliotecas*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*Variáveis globais*/
FILE *registroCadastroColaborador;
FILE *registroCadastroCliente;
int contadorCadastroColaboradorEscrita = 0;
int contadorCadastroColaboradorLeitura = 0;
int contadorCadastroClienteEscrita = 0;
int contadorCadastroClienteLeitura = 0;

/*Structs*/
typedef struct colaborador {
    char nome[100], usuario[100], senha[100];
    int grupo;
} colaborador;
colaborador colaboradorNovo[500];

typedef struct auterarSenha {
    char novaSenhaTeste1[100];
    char novaSenhaTeste2[100];
}auterarSenha;
auterarSenha cadastroNovaSenha;

typedef struct colaboradorLogado {
    char nome[100], usuario[100], senha[100];
    int grupo;
}colaboradorLogado;
colaboradorLogado colaboradorLogado1;

typedef struct cliente {
    char nomeRazaoSocial[100];
    char cpfCnpj[100], telefone[100], email[100];
    char dataDeNascimentoFundacao[100];
    char dataContratacao[100], sevicoContratado[100], usuarioAtendimento[100];
    int status;
    double mensalidade;
} cliente;
cliente clienteNovo[500];

/*Prototipos de Funções*/
void tabulacaoTitulo();
void quebraDeLinhaDescricao();
void cadastroColaborador();
void alterarSenhaColaborador();
void edicaoArquivoColaborador();
void escritaArquivoColaborador();
void leituraArquivoColaborador();
void relatorioCadastroColaboradores();
int login();
int validacaoColaborador(char usuario[], char senha[]);
void cadastroCliente();
void escritaArquivoCliente();
void leituraArquivoCliente();
void relatorioDeClientes();
void relatorioGeralDeClientes();
void relatorioDeClientesPorUsuario();
void atualizacaoAreceberMesAtual();
void validacaoAtendimento();
void usuarioAdmin();
void barraDeStatus();
void manual();
void contatoSuporte();
void menuAjuda();
void menuInicial();

/*Funções*/
void tabulacaoTitulo() {
    printf("\n\t\t\t\t\t");
}

void quebraDeLinhaDescricao() {
    printf("\n\n\n");
}

void cadastroColaborador() {
    if(colaboradorLogado1.grupo == 2) {
        printf("\nUsuario sem permissao\n");
    }
    else if(colaboradorLogado1.grupo == 1) {
        int grupo;
        char nome[100], usuario[100], senha[100], resposta;
        contadorCadastroColaboradorEscrita = 0;

        do {
            system("clear");
            barraDeStatus();
            tabulacaoTitulo();
            printf("CADASTRO DE USUARIO");
            quebraDeLinhaDescricao();
            printf("\nNome: ");
            scanf(" %99[^\n]", nome);
            printf("Usuario: ");
            scanf(" %99[^\n]", usuario);
            printf("Senha: ");
            scanf(" %99[^\n]", senha);
            tabulacaoTitulo();
            printf("GRUPO\nDigite 1 para usuario administrador e 2 para os demais usuarios: ");
            scanf("%d", &grupo);

            strcpy(colaboradorNovo[contadorCadastroColaboradorEscrita].nome, nome);
            strcpy(colaboradorNovo[contadorCadastroColaboradorEscrita].usuario, usuario);
            strcpy(colaboradorNovo[contadorCadastroColaboradorEscrita].senha, senha);
            colaboradorNovo[contadorCadastroColaboradorEscrita].grupo = grupo;
            contadorCadastroColaboradorEscrita++;

            quebraDeLinhaDescricao();
            printf("Deseja cadastrar outro usuario? ");
            scanf(" %c", &resposta);

            while (resposta != 's' && resposta != 'S' && resposta != 'n' && resposta != 'N') {
                printf("Resposta invalida! Deseja cadastrar outro usuario? ");
                scanf(" %c", &resposta);
            }
        } while (resposta == 's' || resposta == 'S');
        escritaArquivoColaborador();
    }
}

void alterarSenhaColaborador() {
    int contadorAlterarSenha = 0;
    int validacao;
    char resposta;

    do {
        leituraArquivoColaborador();

        system("clear");
        barraDeStatus();
        tabulacaoTitulo();
        printf("Auteracao de senha");
        quebraDeLinhaDescricao();
        printf("Digite a nova senha: ");
        scanf(" %99[^\n]", cadastroNovaSenha.novaSenhaTeste1);
        printf("Digite novamente para validacao: ");
        scanf(" %99[^\n]", cadastroNovaSenha.novaSenhaTeste2);

        validacao = strcmp(cadastroNovaSenha.novaSenhaTeste1, cadastroNovaSenha.novaSenhaTeste2);

        if(validacao != 0) {
            printf("Senhas divergentes!\nDeseja tentar novamente? ");
            scanf(" %c", &resposta);

            while (resposta != 's' && resposta != 'S' && resposta != 'n' && resposta != 'N') {
                printf("Resposta invalida! Deseja cadastrar outro usuario? ");
                scanf(" %c", &resposta);
            }
        }
    } while ((resposta == 's' || resposta == 'S') && validacao != 0);

    while(contadorAlterarSenha < contadorCadastroColaboradorLeitura) {
        validacao = strcmp(colaboradorLogado1.usuario, colaboradorNovo[contadorAlterarSenha].usuario);
        if(validacao == 0) {
            strcpy(colaboradorNovo[contadorAlterarSenha].senha, cadastroNovaSenha.novaSenhaTeste1); 
            contadorAlterarSenha = contadorCadastroColaboradorLeitura + 1;
        }
        contadorAlterarSenha++;
    }
    edicaoArquivoColaborador();
}

void edicaoArquivoColaborador() {
    int contadorEdicao = 0;
    registroCadastroColaborador = fopen("arquivoCadastroColaborador.txt", "w");
    if (registroCadastroColaborador == NULL) {
        printf("Erro ao abrir/criar o arquivo!");
    }
    else {
        while (contadorEdicao < contadorCadastroColaboradorLeitura) {
            fprintf(registroCadastroColaborador, "%s-%s-%s-%d\n", colaboradorNovo[contadorEdicao].nome, colaboradorNovo[contadorEdicao].usuario, colaboradorNovo[contadorEdicao].senha, colaboradorNovo[contadorEdicao].grupo);
            contadorEdicao++;
        }
    }
    fclose(registroCadastroColaborador);
}

void escritaArquivoColaborador() {
    int contadorEscrita = 0;
    registroCadastroColaborador = fopen("arquivoCadastroColaborador.txt", "a");
    if (registroCadastroColaborador == NULL) {
        printf("Erro ao abrir/criar o arquivo!");
    }
    else {
        while (contadorEscrita < contadorCadastroColaboradorEscrita) {
            fprintf(registroCadastroColaborador, "%s-%s-%s-%d\n", colaboradorNovo[contadorEscrita].nome, colaboradorNovo[contadorEscrita].usuario, colaboradorNovo[contadorEscrita].senha, colaboradorNovo[contadorEscrita].grupo);
            contadorEscrita++;
        }
    }
    fclose(registroCadastroColaborador);
}

void leituraArquivoColaborador() {
    registroCadastroColaborador = fopen("arquivoCadastroColaborador.txt", "r");
    if (registroCadastroColaborador == NULL) {
        printf("Erro ao abrir/criar o arquivo!");
    }
    else {
        contadorCadastroColaboradorLeitura = 0;
        rewind(registroCadastroColaborador);
        while (fscanf(registroCadastroColaborador, " %99[^-]-%99[^-]-%99[^-]-%d", colaboradorNovo[contadorCadastroColaboradorLeitura].nome, colaboradorNovo[contadorCadastroColaboradorLeitura].usuario, colaboradorNovo[contadorCadastroColaboradorLeitura].senha, &colaboradorNovo[contadorCadastroColaboradorLeitura].grupo) != EOF) {
            contadorCadastroColaboradorLeitura++;
        }
    }
    fclose(registroCadastroColaborador);
}

void relatorioCadastroColaboradores() {
    if(colaboradorLogado1.grupo == 2) {
        printf("\nUsuario sem permissao\n");
    }
    else if(colaboradorLogado1.grupo == 1) {
        leituraArquivoColaborador();
        int contadorRelatorio = 0;
        char resposta;

        system("clear");
        barraDeStatus();
        tabulacaoTitulo();
        printf("RELATORIO DE USUARIOS");
        quebraDeLinhaDescricao();
        while (contadorRelatorio < contadorCadastroColaboradorLeitura) {
            printf("Nome: %s\t Usuario: %s \t Grupo de permissoes: %d\n\n", colaboradorNovo[contadorRelatorio].nome, colaboradorNovo[contadorRelatorio].usuario, colaboradorNovo[contadorRelatorio].grupo);
            contadorRelatorio++;
        }
    }
}

int login() {
    char usuario[100], senha[100], resposta;

    do {
        system("clear");
        tabulacaoTitulo();
        printf("LOGIN\n");
        printf("\n\tUSUARIO: ");
        scanf(" %99[^\n]", usuario);
        printf("\n\tSENHA: ");
        scanf(" %99[^\n]", senha);

        if (validacaoColaborador(usuario, senha) == 0) {
            return 0;
        }
        else {
            system("clear");
            printf("Dados invalidos! Deseja tentar novamente? ");
            scanf(" %c", &resposta);

            while (resposta != 's' && resposta != 'S' && resposta != 'n' && resposta != 'N') {
                system("clear");
                printf("Resposta invalida! Deseja tentar novamente? ");
                scanf(" %c", &resposta);
            }
        }
    } while (resposta == 's' || resposta == 'S');
}

int validacaoColaborador(char usuario[], char senha[]) {
    leituraArquivoColaborador();
    int contadorValidacao = 0, teste1, teste2, teste3;

    while (contadorValidacao <= contadorCadastroColaboradorLeitura) {
        teste1 = strcmp(usuario, colaboradorNovo[contadorValidacao].usuario);
        teste2 = strcmp(senha, colaboradorNovo[contadorValidacao].senha);
        teste3 = teste1 + teste2;

        if (teste3 == 0) {
            strcpy(colaboradorLogado1.nome, colaboradorNovo[contadorValidacao].nome);
            strcpy(colaboradorLogado1.usuario, colaboradorNovo[contadorValidacao].usuario);
            strcpy(colaboradorLogado1.senha, colaboradorNovo[contadorValidacao].senha);
            colaboradorLogado1.grupo = colaboradorNovo[contadorValidacao].grupo;
            contadorValidacao = contadorCadastroColaboradorLeitura + 1;
            return 0;
        }
        contadorValidacao++;
    }
}

void cadastroCliente() {
    char nomeRazaoSocial[100], cpfCnpj[100], telefone[100], email[100];
    char dataDeNascimentoFundacao[100], dataContratacao[100], sevicoContratado[100];
    char usuarioAtendimento[100], resposta;
    int status;
    double mensalidade;
    contadorCadastroClienteEscrita = 0;

    do {
        system("clear");
        barraDeStatus();
        tabulacaoTitulo();
        printf("CADASTRO DE CLIENTE");
        quebraDeLinhaDescricao();
        printf("Razao Social: ");
        scanf(" %99[^\n]", nomeRazaoSocial);
        printf("CNPJ: ");
        scanf(" %99[^\n]", cpfCnpj);
        printf("Telefone: ");
        scanf(" %99[^\n]", telefone);
        printf("E-mail: ");
        scanf(" %99[^\n]", email);
        printf("Data de Fundacao: ");
        scanf(" %99[^\n]", dataDeNascimentoFundacao);
        printf("Data de contratacao de servico: ");
        scanf(" %99[^\n]", dataContratacao);
        printf("Servico contratado: ");
        scanf(" %99[^\n]", sevicoContratado);
        printf("Valor mensal serviço contratado: R$ ");
        scanf("%lf", &mensalidade);
        printf("Status (<1> Em andamento <2> Finalizado): ");
        scanf("%d", &status);

        strcpy(clienteNovo[contadorCadastroClienteEscrita].nomeRazaoSocial, nomeRazaoSocial);
        strcpy(clienteNovo[contadorCadastroClienteEscrita].cpfCnpj, cpfCnpj);
        strcpy(clienteNovo[contadorCadastroClienteEscrita].telefone, telefone);
        strcpy(clienteNovo[contadorCadastroClienteEscrita].email, email);
        strcpy(clienteNovo[contadorCadastroClienteEscrita].dataDeNascimentoFundacao, dataDeNascimentoFundacao);
        strcpy(clienteNovo[contadorCadastroClienteEscrita].dataContratacao, dataContratacao);
        strcpy(clienteNovo[contadorCadastroClienteEscrita].sevicoContratado, sevicoContratado);
        clienteNovo[contadorCadastroClienteEscrita].mensalidade = mensalidade;
        strcpy(clienteNovo[contadorCadastroClienteEscrita].usuarioAtendimento, colaboradorLogado1.usuario);
        clienteNovo[contadorCadastroClienteEscrita].status = status;
        contadorCadastroClienteEscrita++;

        quebraDeLinhaDescricao();
        printf("Deseja cadastrar outro cliente? ");
        scanf(" %c", &resposta);

        while (resposta != 's' && resposta != 'S' && resposta != 'n' && resposta != 'N') {
            printf("Resposta invalida! Deseja cadastrar outro usuario? ");
            scanf(" %c", &resposta);
        }
    } while (resposta == 's' || resposta == 'S');
    escritaArquivoCliente();
}

void escritaArquivoCliente() {
    int contadorEscrita = 0;
    registroCadastroCliente = fopen("arquivoCadastroCliente.txt", "a");
    if (registroCadastroCliente == NULL) {
        printf("Erro ao abrir/criar o arquivo!");
    }
    else {
        while(contadorEscrita < contadorCadastroClienteEscrita) {
            fprintf(registroCadastroCliente, "%s-%s-%s-%s-%s-%s-%s-%.2lf-%s-%d\n", clienteNovo[contadorEscrita].nomeRazaoSocial, clienteNovo[contadorEscrita].cpfCnpj, clienteNovo[contadorEscrita].dataDeNascimentoFundacao, clienteNovo[contadorEscrita].telefone, clienteNovo[contadorEscrita].email, clienteNovo[contadorEscrita].dataContratacao, clienteNovo[contadorEscrita].sevicoContratado, clienteNovo[contadorEscrita].mensalidade, clienteNovo[contadorEscrita].usuarioAtendimento, clienteNovo[contadorEscrita].status);
            contadorEscrita++;
        }
    }
    fclose(registroCadastroCliente);
}

void leituraArquivoCliente() {
    registroCadastroCliente = fopen("arquivoCadastroCliente.txt", "r");
    if (registroCadastroCliente == NULL) {
        printf("Erro ao abrir/criar arquivo!");
    }
    else {
        contadorCadastroClienteLeitura = 0;
        rewind(registroCadastroCliente);

        while (fscanf(registroCadastroCliente, " %99[^-]-%99[^-]-%99[^-]-%99[^-]-%99[^-]-%99[^-]-%99[^-]-%lf-%99[^-]-%d", clienteNovo[contadorCadastroClienteLeitura].nomeRazaoSocial, clienteNovo[contadorCadastroClienteLeitura].cpfCnpj, clienteNovo[contadorCadastroClienteLeitura].dataDeNascimentoFundacao, clienteNovo[contadorCadastroClienteLeitura].telefone, clienteNovo[contadorCadastroClienteLeitura].email, clienteNovo[contadorCadastroClienteLeitura].dataContratacao, clienteNovo[contadorCadastroClienteLeitura].sevicoContratado, &clienteNovo[contadorCadastroClienteLeitura].mensalidade,clienteNovo[contadorCadastroClienteLeitura].usuarioAtendimento, &clienteNovo[contadorCadastroClienteLeitura].status) != EOF) {
            contadorCadastroClienteLeitura++;
        }
    }
    fclose(registroCadastroCliente);
}

void relatorioDeClientes() {
    if(access("arquivoCadastroCliente.txt", F_OK) == -1) {
        printf("\nNenhum cadastro encontrado\n");
    }
    else {
        leituraArquivoCliente();
        leituraArquivoColaborador();

        int contadorRelatorio = 0, teste;

        system("clear");
        barraDeStatus();
        tabulacaoTitulo();
        printf("RELATORIO DE CLIENTES");
        quebraDeLinhaDescricao();

        while(contadorRelatorio < contadorCadastroClienteLeitura) {
            teste = strcmp(colaboradorLogado1.usuario, clienteNovo[contadorRelatorio].usuarioAtendimento);
            if(teste == 0) {
                printf("Razao social: %s CNPJ: %s\nTelefone: %s E-mail %s\nData De Fundacao: %s Data de contratacao: %s\nServico contratado: %s Valor mensal: R$ %.2lf\nUsuario responsavel: %s Status: ", clienteNovo[contadorRelatorio].nomeRazaoSocial, clienteNovo[contadorRelatorio].cpfCnpj, clienteNovo[contadorRelatorio].telefone, clienteNovo[contadorRelatorio].email, clienteNovo[contadorRelatorio].dataDeNascimentoFundacao, clienteNovo[contadorRelatorio].dataContratacao, clienteNovo[contadorRelatorio].sevicoContratado, clienteNovo[contadorRelatorio].mensalidade, clienteNovo[contadorRelatorio].usuarioAtendimento);
                if(clienteNovo[contadorRelatorio].status == 0) {
                    printf("Em andamento\n\n");
                }
                else {
                    printf("Finalizado\n\n");
                }
            }
            contadorRelatorio++;
        }
    }
}

void relatorioGeralDeClientes() {
    if(colaboradorLogado1.grupo == 2) {
        printf("\nUsuario sem permissao\n");
    }
    else if(colaboradorLogado1.grupo == 1) {
        if (access("arquivoCadastroCliente.txt", F_OK) == -1) {
            printf("\nNenhum cadastro encontrado\n");
        }
        else {
            leituraArquivoCliente();
            int contadorRelatorio = 0;
            char resposta;

            system("clear");
            barraDeStatus();
            tabulacaoTitulo();
            printf("RELATORIO GERAL DE CLIENTES");
            quebraDeLinhaDescricao();

            while (contadorRelatorio < contadorCadastroClienteLeitura) {
                printf("Razao social: %s CNPJ: %s\nTelefone: %s E-mail %s\nData De Fundacao: %s Data de contratacao: %s\nServico contratado: %s Valor mensal: R$ %.2lf\nUsuario responsavel: %s Status: ", clienteNovo[contadorRelatorio].nomeRazaoSocial, clienteNovo[contadorRelatorio].cpfCnpj, clienteNovo[contadorRelatorio].telefone, clienteNovo[contadorRelatorio].email, clienteNovo[contadorRelatorio].dataDeNascimentoFundacao, clienteNovo[contadorRelatorio].dataContratacao, clienteNovo[contadorRelatorio].sevicoContratado, clienteNovo[contadorRelatorio].mensalidade,clienteNovo[contadorRelatorio].usuarioAtendimento);
                if(clienteNovo[contadorRelatorio].status == 0) {
                    printf("Em andamento\n\n");
                }
                else {
                    printf("Finalizado\n\n");
                }
                contadorRelatorio++;
            }
        }
    }
}

void relatorioDeClientesPorUsuario()
{
    if(colaboradorLogado1.grupo == 2) {
        printf("\nUsuario sem permissao\n");
    }
    else if(colaboradorLogado1.grupo == 1) {
        if (access("arquivoCadastroCliente.txt", F_OK) == -1) {
            printf("\nNenhum cadastro encontrado\n");
        }
        else {
            leituraArquivoColaborador();
            leituraArquivoCliente();

            int contadorRelatorio = 0, teste;
            char usuario[100];

            system("clear");
            barraDeStatus();
            printf("Informe o usuario: ");
            scanf(" %99[^\n]", usuario);

            tabulacaoTitulo();
            printf("RELATORIO DE CLIENTES POR USUARIO");
            quebraDeLinhaDescricao();

            while (contadorRelatorio < contadorCadastroClienteLeitura) {
                teste = strcmp(usuario, clienteNovo[contadorRelatorio].usuarioAtendimento);
                if (teste == 0) {
                    printf("Razao social: %s CNPJ: %s\nTelefone: %s E-mail %s\nData De Fundacao: %s Data de contratacao: %s\nServico contratado: %s Usuario responsavel: %sStatus: ", clienteNovo[contadorRelatorio].nomeRazaoSocial, clienteNovo[contadorRelatorio].cpfCnpj, clienteNovo[contadorRelatorio].telefone, clienteNovo[contadorRelatorio].email, clienteNovo[contadorRelatorio].dataDeNascimentoFundacao, clienteNovo[contadorRelatorio].dataContratacao, clienteNovo[contadorRelatorio].sevicoContratado, clienteNovo[contadorRelatorio].usuarioAtendimento);
                    if(clienteNovo[contadorRelatorio].status == 0) {
                        printf("Em andamento\n\n");
                    }
                    else {
                        printf("Finalizado\n\n");
                    }
                }
                contadorRelatorio++;
            }
        }
    }
}

void atualizacaoAreceberMesAtual() {
    if(colaboradorLogado1.grupo == 2) {
        printf("\nUsuario sem permissao\n");
    }
    else if(colaboradorLogado1.grupo == 1) {
        if (access("arquivoCadastroCliente.txt", F_OK) == -1) {
            printf("\nNenhum cadastro encontrado\n");
        }
        else {
            leituraArquivoCliente();

            float totalAreceber = 0;
            int contadorRelatorio = 0;

            while(contadorRelatorio < contadorCadastroClienteLeitura) {
                if(clienteNovo[contadorRelatorio].status == 1) {
                    totalAreceber = totalAreceber + clienteNovo[contadorRelatorio].mensalidade;
                }
                contadorRelatorio++;
            }
            system("clear");
            barraDeStatus();
            tabulacaoTitulo();
            printf("ATUALIZACAO MENSAL A RECEBER");
            quebraDeLinhaDescricao();
            tabulacaoTitulo();
            printf("TOTAL: R$ %.2lf", totalAreceber);
            quebraDeLinhaDescricao();
        }
    }
}

void usuarioAdmin() {
    if (access("arquivoCadastroColaborador.txt", R_OK) == -1) {
        registroCadastroColaborador = fopen("arquivoCadastroColaborador.txt", "a");
        int grupo;
        char nomeAdmin[100], usuarioAdmin[100], senhaAdmin[100];
        strcpy(nomeAdmin, "admin");
        strcpy(usuarioAdmin, "admin");
        strcpy(senhaAdmin, "admin");
        grupo = 1;

        fprintf(registroCadastroColaborador, "%s-%s-%s-%d\n", nomeAdmin, usuarioAdmin, senhaAdmin, grupo);
        fclose(registroCadastroColaborador);
    }
}

void barraDeStatus() {
    printf("NOME: %s \tUSUARIO: %s\n\n", colaboradorLogado1.nome,colaboradorLogado1.usuario);
}

void manual() {
    printf("Pressione qualquer teclar para continuar...");
    getchar();getchar();
}

void contatoSuporte() {
    system("clear");
    barraDeStatus();
    tabulacaoTitulo();
    printf("CONTATOS SUPORTE");
    quebraDeLinhaDescricao();
    printf("\tE-MAIL: suporte@suporte.com.br\tCHAT: chat.com.br\n");
    printf("\tTELEFONE: (00) 0000-0000\tCELULAR: (00)00000-0000\n");
    quebraDeLinhaDescricao();

    printf("Pressione qualquer tecla para continuar...");
    getchar();getchar();
}

void menuAjuda() {
    int opcao;
    
    system("clear");
    barraDeStatus();
    tabulacaoTitulo();
    printf("MENU AJUDA");
    quebraDeLinhaDescricao();
    printf("-1.Contato Suporte\n-2.Manual de instrucoes\n\nOpcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        
        case 1:
            contatoSuporte();
            break;      
        case 2:
            manual();
            break;
        default :
            printf("Opcao invalida!\n");
            break;
    }
}

void menuInicial() {
    int opcao;
    char resposta;
    do {
        system("clear");
        barraDeStatus();
        tabulacaoTitulo();
        printf("MENU INICIAL");
        quebraDeLinhaDescricao();
        printf("-1.Cadastro de usuario\n-2.Alterar senha\n-3.Relatorio de usuarios\n-4.Cadastro de cliente\n-5.Relatorio de clientes\n-6.Relatorio geral de clientes\n-7.Relatorio de clientes por usuario\n-8.Atualizacao a receber mes atual\n-9.Ajuda\n-10.Sair\n\nOpcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
        
        case 1:
            cadastroColaborador();
            break;

        case 2:
            alterarSenhaColaborador();
            break;
        case 3:
            relatorioCadastroColaboradores();
            break;
        case 4:
            cadastroCliente();
            break;
        case 5:
            relatorioDeClientes();
            break;
        case 6:
            relatorioGeralDeClientes();
            break;
        case 7:
            relatorioDeClientesPorUsuario();
            break;
        case 8:
            atualizacaoAreceberMesAtual();
            break;
        case 9:
            menuAjuda();
            break;
        case 10:
            printf("Deseja realmente sair?: ");
            scanf(" %c", &resposta);
            break;
        default:
            printf("Opcao invalida!\n");
            break;
        }
        
        if(opcao != 10) {
            printf("Deseja retornar ao menu inicial? Digite (s) para retornar e (n) para sair: ");
            scanf(" %c", &resposta);

            while (resposta != 's' && resposta != 'S' && resposta != 'n' && resposta != 'N') {
                printf("Resposta invalida! Digite (s) para retornar e (n) para sair: ");
                scanf(" %c", &resposta);
            }
        }
        else {
            resposta = 'n';
        }
    } while (resposta == 's' || resposta == 'S');
}

/*Função principal main*/
int main() {

    usuarioAdmin();

    if (login() == 0) {
        menuInicial();
    }

    return 0;
}