#include <iostream>
#include <cstring>
#include <locale.h>
#include <fstream>

#define TAM 10 // Limite de livros que podem ser inseridos. Caso queira aumentar ou diminuir, mude o valor de TAM

using namespace std;

//------------------------------------------ESTRUTURA LISTA DE ARQUIVOS(LIVROS)-------------------------------------------------//

struct Livros
{
    string nome;            //Nome do livro
    int codigo;            //Código do livro para saber em quais livros as palavras estão.
    struct Livros* prox;  // Prox livro da lista
    struct Livros* ant;  // Ant livro da lista

};

Livros* criaNoLivros ()
{
    Livros* novoLivro;

    novoLivro = new Livros;     //Alocação dinâmica da estrutura Livros

    novoLivro->ant = NULL;
    novoLivro->prox = NULL;

    return novoLivro;

}

Livros* insereLivro (string nome, Livros* biblioteca )
{
    Livros* novo;               //livro novo que está entrando na lista
    Livros* auxBiblioteca;     //Aux para percorrer a lista
    Livros* ant = NULL;       //Guarda o livro anterior para atualizar a lista

    auxBiblioteca = biblioteca;

    novo = criaNoLivros();


    novo->nome = nome;

    //Se não tiver nenhum livro na lista
    if (auxBiblioteca == NULL)
    {
        novo->codigo = 1; // cod de acordo com qtd de livros na lista

        auxBiblioteca = novo;
    }
    else
    {
        //Se tiver apenas um livro na lista
        if (auxBiblioteca->prox == NULL)
        {
            auxBiblioteca->prox = novo;
            novo->ant = auxBiblioteca;
            novo->codigo = auxBiblioteca->codigo + 1; // cod recebe o valor do último livro da lista + 1
        }
        //Vários livros na lista
        else
        {
            //Vai para último livro da lista
            while(auxBiblioteca->prox != NULL)
            {
                ant = auxBiblioteca;
                auxBiblioteca = auxBiblioteca->prox;
            }

            auxBiblioteca->prox = novo;
            novo->ant = ant;
            novo->codigo = auxBiblioteca->codigo + 1; // cod recebe o valor do último livro da lista + 1
        }

    }

    //Volta lista para raiz (primeiro da lista)
    while(auxBiblioteca->ant != NULL)
    {
        auxBiblioteca = auxBiblioteca->ant;
    }

    return auxBiblioteca;

}

//--------------------------------------------ESTRUTURA ARVORE AVL--------------------------------------------------------------//

struct No
{
    char* palavra;
    int quantidadeVezesAparece;
    int codLivro [TAM];         //Serve para saber em quais livros aparece a palavra
    Livros* listaLivros;
    struct No* esquerda;
    struct No* direita;
};

//Cria um nó
//recebe a nova palavra
No* criaNoArv (char* palavra)
{
    No* novoNo;
    int i;

    novoNo = new No;

    novoNo->palavra = palavra;
    novoNo->direita = NULL;
    novoNo->esquerda = NULL;
    novoNo->listaLivros = NULL;
    novoNo->quantidadeVezesAparece = 1;

    //Inicializando vetor de cod de livros em 0
    for (i = 0; i < TAM; i++)
    {
        novoNo->codLivro[i] = 0;

    }

    return novoNo;
}

//Recebe a palavra tipo string e seu tamanho
//Converte para tipo char*
//Transforma a palavra toda para letras maiúsculas
char* convertePalavra (string palavra, int tam)
{
    char* palavraM;

    palavraM = new char[tam + 1];

    strcpy(palavraM, palavra.c_str());

    //Aqui o toupper() é usado para transformar a palavra em letras maiúsculas
    for (int i = 0; i <= tam; i++)
    {
        palavraM[i] = toupper(palavraM[i]);
        //cout << palavraM[i];
    }

    return palavraM;
}

//Insere a palavra na árvore
No* inserirPalavra (No* arvore, char* novaPalavra, int cod)
{
    No* novoNo;

    if (arvore == NULL)
    {
        novoNo = criaNoArv(novaPalavra);

        arvore = novoNo;
        arvore->codLivro[cod - 1] = cod;

    }
    else
    {
        //Já existe a palavra na árvore. Incrementa o contador da palavra e atualiza vetor de livros
        if( strcmp(arvore->palavra, novaPalavra) == 0 )
        {
            arvore->quantidadeVezesAparece++;
            arvore->codLivro[cod - 1] = cod;
        }

        //Palavra nova é maior que a atual na árvore
        else if ( strcmp(arvore->palavra, novaPalavra) < 0 )
        {
            arvore->direita = inserirPalavra(arvore->direita, novaPalavra, cod);

        }

        //Palavra nova é menor que a atual na árvore
        else if ( strcmp(arvore->palavra, novaPalavra) > 0 )
        {
            arvore->esquerda = inserirPalavra(arvore->esquerda, novaPalavra, cod);

        }

    }

    return arvore;
}

//Para inserir o nome do livro na lista de cada palavra será necessário
// string nome e Livro* biblioteca
No* insereInfoLista (No* palavra, Livros* biblioteca)
{
    while (biblioteca != NULL)
    {
        for (int i = 0; i < TAM; i++)
        {

            if (palavra->codLivro[i] == biblioteca->codigo)
            {
                palavra->listaLivros = insereLivro (biblioteca->nome, palavra->listaLivros);

            }
        }

        biblioteca = biblioteca->prox;

    }

    //Nessa lista o código do livro é irrelevante até o presente momento, por isso é
    //desprezado recebendo valor -1
    while (palavra->listaLivros->prox != NULL)
    {
        palavra->listaLivros->codigo = -1;
        palavra->listaLivros = palavra->listaLivros->prox;
    }

    palavra->listaLivros->codigo = -1;

    //Volta a lista para primeiro elemento da lista
    while (palavra->listaLivros->ant != NULL)
    {
        palavra->listaLivros = palavra->listaLivros->ant;
    }

    return palavra;
}

//Busca a palavra passada como parâmetro
//Se não encontrar retorna NULL
//Se encontrar retorna o nó
No* buscarPalavra (No* arvore, string palavra, Livros* biblioteca)
{
    char* palavraM;

    palavraM = convertePalavra(palavra, palavra.length());

    //Árvore vazia ou palavra não encontrada
    if (arvore == NULL)
    {
        return NULL;
    }
    else
    {
        //Se 0, palavra encontrada
        if ( strcmp(arvore->palavra, palavraM) == 0 )
        {
            if (arvore->listaLivros == NULL)
            {
                arvore = insereInfoLista (arvore, biblioteca);
            }

            return arvore;
        }
        //Se palavraM for maior que a atual, vai para próximo a direita
        else if ( strcmp(arvore->palavra, palavraM) < 0 )
        {
            return buscarPalavra(arvore->direita, palavra, biblioteca);
        }
        //Se palavraM for menor que atual, vai para próximo a esquerda
        else if ( strcmp(arvore->palavra, palavraM) > 0 )
        {
            return buscarPalavra(arvore->esquerda, palavra, biblioteca);
        }
    }

    return NULL;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------------//

//Lê os arquivos salvos na lista
//Separa cada palavra para converter e inserir na árvore
No* lerArquivo(Livros* biblioteca, No* arvore)
{
    setlocale(LC_ALL, "");

    ifstream arquivoEntrada;
    string palavraS;
    string dado;
    char* palavraM;


    while(biblioteca != NULL)
    {
        arquivoEntrada.open(biblioteca->nome);


        if (arquivoEntrada.is_open())
        {
            while (getline(arquivoEntrada, palavraS, ' '))
            {

                //Pula os espaços
                if (palavraS == " ")
                {
                    continue;
                }

                palavraM = convertePalavra(palavraS, palavraS.length());


                //Aqui entra método para inserir a palavraM na árvore
                arvore = inserirPalavra(arvore, palavraM, biblioteca->codigo);

                //cout << endl;

            }

            arquivoEntrada.close();

            //cout << "Quantidade de linhas lidas foi: " << qtdLinhasLidas << endl;

        }
        else
        {
            cout << "Não foi possível abrir o arquivo" << endl;
        }

        cout << "-----------------------------------------------------------------" << endl;

        biblioteca = biblioteca->prox;
    }

    return arvore;
}


