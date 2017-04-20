// Drzewo czerwono-czarne
// Data: 11.06.2013
// (C)2013 mgr Jerzy Wa³aszek
//------------------------------

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAXN = 30;           // Liczba wêz³ów

// Typ wêz³ów drzewa RBT

struct RBTNode
{
  RBTNode * up;
  RBTNode * left;
  RBTNode * right;
  int key;
  char color;
};

// Definicja typu obiektowego TRBTree
class TRBTree
{
  private:
    RBTNode S;             // Wêze³ stra¿nika
    RBTNode * root;        // Korzeñ drzewa czerwono-czarnego
    string cr,cl,cp;       // £añcuchy do znaków ramek

    void printRBT(string sp, string sn, RBTNode * p); // Wypisuje drzewo

  public:
    TRBTree();             // Konstruktor klasy
    ~TRBTree();            // Destruktor klasy
    void DFSRelease(RBTNode * p); // Usuwa rekurencyjnie drzewo
    void print();          // Wypisuje drzewo
    RBTNode * findRBT(int k); // Wyszukuje wêze³ o kluczu k
    RBTNode * minRBT(RBTNode * p); // Wyszukuje najmniejszy wêze³ w p
    RBTNode * succRBT(RBTNode * p);// Wyszukuje nastêpnik p
    void rot_L(RBTNode * A); // Rotacja w lewo wzglêdem A
    void rot_R(RBTNode * A); // Rotacja w prawo wzglêdem A
    void insertRBT(int k);   // Wstawia wêze³ o kluczu k
    void removeRBT(RBTNode * X); // Usuwa wêze³ X
};

// Konstruktor drzewa
//-------------------
TRBTree::TRBTree()
{
  cr = cl = cp = "  ";
  cr[0] = 218; cr[1] = 196;
  cl[0] = 192; cl[1] = 196;
  cp[0] = 179;
  S.color = 'B';          // Inicjujemy stra¿nika
  S.up    = &S;
  S.left  = &S;
  S.right = &S;
  root    = &S;           // Korzeñ wskazuje stra¿nika
}

// Destruktor drzewa
//------------------
TRBTree::~TRBTree()
{
  DFSRelease(root);       // Rekurencyjnie usuwamy wêz³y
}

// Usuwa rekurencyjnie drzewo czerwono-czarne
//-------------------------------------------
void TRBTree::DFSRelease(RBTNode * p)
{
  if(p != &S)
  {
    DFSRelease(p->left);   // usuwamy lewe poddrzewo
    DFSRelease(p->right);  // usuwamy prawe poddrzewo
    delete p;              // usuwamy sam wêze³
  }
}

// Wypisuje zawartoœæ drzewa
//--------------------------
void TRBTree::printRBT(string sp, string sn, RBTNode * p)
{
  string t;

  if(p != &S)
  {
    t = sp;
    if(sn == cr) t[t.length() - 2] = ' ';
    printRBT(t+cp,cr,p->right);

    t = t.substr(0,sp.length()-2);
    cout << t << sn << p->color << ":" << p->key << endl;

    t = sp;
    if(sn == cl) t[t.length() - 2] = ' ';
    printRBT(t+cp,cl,p->left);
  }
}

// Wypisuje zawartoœæ drzewa
//--------------------------
void TRBTree::print()
{
  printRBT("","",root);
}

// Wyszukuje wêze³ o kluczu k
// Jeœli wêze³ nie zostanie znaleziony, to zwraca
// wskazanie puste NULL
//-----------------------------------------------
RBTNode * TRBTree::findRBT(int k)
{
  RBTNode * p;

  p = root;
  while((p != &S) && (p->key != k))
    if(k < p->key) p = p->left;
    else           p = p->right;
  if(p == &S) return NULL;
  return p;
}

// Wyszukuje najmniejszy wêze³ w poddrzewie
// o korzeniu p
//-----------------------------------------
RBTNode * TRBTree::minRBT(RBTNode * p)
{
  if(p != &S)
    while(p->left != &S) p = p->left;
  return p;
}

// Zwraca nastêpnik p
//-------------------
RBTNode * TRBTree::succRBT(RBTNode * p)
{
  RBTNode * r;

  if(p != &S)
  {
    if(p->right != &S) return minRBT(p->right);
    else
    {
      r = p->up;
      while((r != &S) && (p == r->right))
      {
        p = r;
        r = r->up;
      }
      return r;
    }
  }
  return &S;
}

// Wykonuje rotacjê w lewo wzglêdem A
//-----------------------------------
void TRBTree::rot_L(RBTNode * A)
{
  RBTNode * B, * p;

  B = A->right;
  if(B != &S)
  {
    p = A->up;
    A->right = B->left;
    if(A->right != &S) A->right->up = A;

    B->left = A;
    B->up = p;
    A->up = B;

    if(p != &S)
    {
      if(p->left == A) p->left = B; else p->right = B;
    }
    else root = B;
  }
}

// Wykonuje rotacjê w prawo wzglêdem A
//------------------------------------
void TRBTree::rot_R(RBTNode * A)
{
  RBTNode * B, * p;

  B = A->left;
  if(B != &S)
  {
    p = A->up;
    A->left = B->right;
    if(A->left != &S) A->left->up = A;

    B->right = A;
    B->up = p;
    A->up = B;

    if(p != &S)
    {
      if(p->left == A) p->left = B; else p->right = B;
    }
    else root = B;
  }
}

// Wstawia do drzewa wêze³ o kluczu k
//-----------------------------------
void TRBTree::insertRBT(int k)
{
  RBTNode * X, * Y;

  X = new RBTNode;        // Tworzymy nowy wêze³
  X->left  = &S;          // Inicjujemy pola
  X->right = &S;
  X->up    = root;
  X->key   = k;
  if(X->up == &S) root = X; // X staje siê korzeniem
  else
  while(true)             // Szukamy liœcia do zast¹pienia przez X
  {
    if(k < X->up->key)
    {
      if(X->up->left == &S)
      {
        X->up->left = X;  // X zastêpuje lewy liœæ
        break;
      }
      X->up = X->up->left;
    }
    else
    {
      if(X->up->right == &S)
      {
        X->up->right = X; // X zastêpuje prawy liœæ
        break;
      }
      X->up = X->up->right;
    }
  }
  X->color = 'R';         // Wêze³ kolorujemy na czerwono
  while((X != root) && (X->up->color == 'R'))
  {
    if(X->up == X->up->up->left)
    {
      Y = X->up->up->right; // Y -> wujek X

      if(Y->color == 'R')  // Przypadek 1
      {
        X->up->color = 'B';
        Y->color = 'B';
        X->up->up->color = 'R';
        X = X->up->up;
        continue;
      }

      if(X == X->up->right) // Przypadek 2
      {
        X = X->up;
        rot_L(X);
      }

      X->up->color = 'B'; // Przypadek 3
      X->up->up->color = 'R';
      rot_R(X->up->up);
      break;
    }
    else
    {                  // Przypadki lustrzane
      Y = X->up->up->left;

      if(Y->color == 'R') // Przypadek 1
      {
        X->up->color = 'B';
        Y->color = 'B';
        X->up->up->color = 'R';
        X = X->up->up;
        continue;
      }

      if(X == X->up->left) // Przypadek 2
      {
        X = X->up;
        rot_R(X);
      }

      X->up->color = 'B'; // Przypadek 3
      X->up->up->color = 'R';
      rot_L(X->up->up);
      break;
    }
  }
  root->color = 'B';
}

// Usuwa z drzewa wêze³ X
//-----------------------
void TRBTree::removeRBT(RBTNode * X)
{
  RBTNode * W, * Y, * Z;

  if((X->left == &S) || (X->right == &S)) Y = X;
  else                                    Y = succRBT(X);

  if(Y->left != &S) Z = Y->left;
  else              Z = Y->right;

  Z->up = Y->up;

  if(Y->up == &S) root = Z;
  else if(Y == Y->up->left) Y->up->left  = Z;
  else                      Y->up->right = Z;

  if(Y != X) X->key = Y->key;

  if(Y->color == 'B')   // Naprawa struktury drzewa czerwono-czarnego
    while((Z != root) && (Z->color == 'B'))
      if(Z == Z->up->left)
      {
        W = Z->up->right;

        if(W->color == 'R')
        {              // Przypadek 1
          W->color = 'B';
          Z->up->color = 'R';
          rot_L(Z->up);
          W = Z->up->right;
        }

        if((W->left->color == 'B') && (W->right->color == 'B'))
        {              // Przypadek 2
          W->color = 'R';
          Z = Z->up;
          continue;
        }

        if(W->right->color == 'B')
        {              // Przypadek 3
          W->left->color = 'B';
          W->color = 'R';
          rot_R(W);
          W = Z->up->right;
        }

        W->color = Z->up->color; // Przypadek 4
        Z->up->color = 'B';
        W->right->color = 'B';
        rot_L(Z->up);
        Z = root;         // To spowoduje zakoñczenie pêtli
      }
      else
      {                // Przypadki lustrzane
        W = Z->up->left;

        if(W->color == 'R')
        {              // Przypadek 1
          W->color = 'B';
          Z->up->color = 'R';
          rot_R(Z->up);
          W = Z->up->left;
        }

        if((W->left->color == 'B') && (W->right->color == 'B'))
        {              // Przypadek 2
          W->color = 'R';
          Z = Z->up;
          continue;
        }

        if(W->left->color == 'B')
        {              // Przypadek 3
          W->right->color = 'B';
          W->color = 'R';
          rot_L(W);
          W = Z->up->left;
        }

        W->color = Z->up->color;  // Przypadek 4
        Z->up->color = 'B';
        W->left->color = 'B';
        rot_R(Z->up);
        Z = root;         // To spowoduje zakoñczenie pêtli
      }

  Z->color = 'B';

  delete Y;
}

//**********************
//*** PROGRAM G£ÓWNY ***
//**********************

int main()
{
  int Tk[MAXN];          // Tablica kluczy wêz³ów
  int i,x,i1,i2;
  TRBTree * RBT;         // Obiekt drzewa czerwono-czarnego

  srand(time(NULL));     // Inicjujemy generator pseudolosowy

  RBT = new TRBTree;     // Tworzymy puste drzewo

  for(i = 0; i < MAXN; i++) // Tablicê wype³niamy wartoœciami kluczy
    Tk[i] = i + 1;

  for(i = 0; i < 300; i++)  // Mieszamy tablicê
  {
    i1 = rand() % MAXN;     // Losujemy 2 indeksy
    i2 = rand() % MAXN;

    x = Tk[i1];             // Wymieniamy Tk[i1] <-->. Tk[i2]
    Tk[i1] = Tk[i2];
    Tk[i2] = x;
  }

  for(i = 0; i < MAXN; i++) // Na podstawie tablicy tworzymy drzewo czerwono-czarne
  {
    cout << Tk[i] << " ";
    RBT->insertRBT(Tk[i]);
  }

  cout << endl << endl;

  RBT->print();             // Wyœwietlamy zawartoœæ drzewa

  cout << endl << endl;

  for(i = 0; i < 300; i++)  // Ponownie mieszamy tablicê
  {
    i1 = rand() % MAXN; i2 = rand() % MAXN;
    x = Tk[i1]; Tk[i1] = Tk[i2]; Tk[i2] = x;
  }

  for(i = 0; i < MAXN >> 1; i++) // Usuwamy po³owê wêz³ów
  {
    cout << Tk[i] << " ";
    RBT->removeRBT(RBT->findRBT(Tk[i]));
  }

  cout << endl << endl;

  RBT->print();            // Wyœwietlamy zawartoœæ drzewa

  delete RBT;              // Usuwamy drzewo z pamiêci

  return 0;
} 