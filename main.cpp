#include <iostream>
#include <string.h>
#include <iomanip>
#include <locale.h>
#include <fstream>

#include "ArvoreAVL.h"

using namespace std;

int main()
{

    No* arvore;
    No* palavraBuscada;
    Livros* biblioteca;
    string nomeLivro;
    string palavra;
    int qtd = 3;

    biblioteca = NULL;

    palavraBuscada = NULL;

    arvore = NULL;

    //Recebe nome dos arquivos e insere na lista
    for(int i = 0; i< qtd; i++)
    {
        cout << "Entre com nome do livro: ";
        cin >> nomeLivro;

        biblioteca = insereLivro(nomeLivro, biblioteca);

    }

    //Faz a leitura das palavras da lista e insere na arvore
    arvore = lerArquivo(biblioteca, arvore);

    //Loop para buscar as palavras
    for (;;)
    {

        cout << "Entre com a palavra que deseja buscar: ";
        cin >> palavra;

        palavraBuscada = buscarPalavra(arvore, palavra, biblioteca);

        if (palavraBuscada == NULL)
        {
            cout << "Palavra não encontrada" << endl;
        }
        else
        {
            cout <<"----------------------------------------------" << endl;

            cout << "Palavra encontrada foi : " << palavraBuscada->palavra << endl;
            cout << "Quantidade de vezes foi: " << palavraBuscada->quantidadeVezesAparece << endl;
            cout << "Encontrada nos arquivos: ";

            while (palavraBuscada->listaLivros->prox != NULL)
            {
                cout << palavraBuscada->listaLivros->nome << " ";
                palavraBuscada->listaLivros = palavraBuscada->listaLivros->prox;
            }

            //Imprime o nome do último elemento da lista
            cout << palavraBuscada->listaLivros->nome << endl;

            cout <<"----------------------------------------------" << endl << endl;

            //Volta ao primeiro elemento da lista
            while(palavraBuscada->listaLivros->ant != NULL)
            {
                palavraBuscada->listaLivros = palavraBuscada->listaLivros->ant;
            }

        }

    }

    return 0;
}
