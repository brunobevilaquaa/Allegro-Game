#include <iostream>
#include <vector>
using namespace std;

struct t{
  int x;
  int y;
};


int main(){

    vector<t> vetor;
    int i;

   for(i = 0; i < 5; i++){
      t aux;
      aux.x = i;
      aux.y = i * 10;

      vetor.push_back(aux);
   }
    // PEGANDO O TAMANHO DO VETOR
    cout << "Tamanho do Vetor = " << vetor.size() << endl;

    // ACESSANDO OS 5 VALORES DO VETOR PASSANDO PELO INDEX
    for(i = 0; i < vetor.size(); i++){
      cout << "Valor do vetor [" << i << "] = " << vetor[i].x << endl;
    }

    cout << "SEPARANDOOOO" << endl;


    vetor.erase(vetor.begin() + 0);

    for(i = 0; i < vetor.size(); i++){
      cout << "Valor do vetor [" << i << "] = " << vetor[i].x << endl;
    }

    for(i = 0; i < vetor.size(); i++){
      vetor[i].x = i;
    }


    cout << "SEPARANDOOOO" << endl;


    for(i = 0; i < vetor.size(); i++){
      cout << "Valor do vetor [" << i << "] = " << vetor[i].x << endl;
    }


    return 0;
}
