#include <iostream>
#include <cmath>
#include "bigint.h"

using namespace std;

///Função isNeg
bool BigInt::isNeg() const {
    return neg;
}
///Função Size
int BigInt::size() const {
    return nDig;
}
/// Construtor default.
/// Inicializa com um inteiro de 1 digito, valor 0 (zero).
/// NAO PODE SER MODIFICADO
BigInt::BigInt()
  : neg(false)
  , nDig(1)
  , d(new int8_t[nDig]{0})
{}

/// Destrutor
BigInt::~BigInt(){
    delete[] d;
}


/// Construtor especifico PRIVADO que recebe o sinal e a quantidade de digitos
BigInt:: BigInt(bool isNeg, int size)
:   neg(isNeg),
    nDig(size >= 1 ? size : 1),
    d(new int8_t[nDig]{0})
{}

/// Construtor por copia.
/// Delega ao construtor especifico privado.
/// NAO PODE SER MODIFICADO.
BigInt::BigInt(const BigInt& B)
  : BigInt(B.isNeg(), B.size())
{
  // Copia os digitos
  for (int i=0; i<size(); ++i) d[i] = B.d[i];
}

/// Atribuicao por copia
BigInt& BigInt::operator=(const BigInt& B){
    if(this != &B){
        int8_t* novo = new int8_t[B.nDig];
        for(int i = 0; i < B.nDig; ++i){
            novo[i] = B.d[i];
        }
        delete[] d;
        neg = B.neg;
        nDig = B.nDig;
        d = novo;
    }
    return *this;
}


/// Construtor por movimento
BigInt::BigInt(BigInt&& B) noexcept
    : neg(B.neg), nDig(B.nDig), d(B.d)
{
    // deixa B em estado seguro apos o movimento
    B.neg = false;
    B.nDig = 1;
    B.d = new int8_t[1]{0};
}

/// Atribuicao por movimento
BigInt& BigInt::operator=(BigInt&& B) noexcept
{
    if(this != &B){
        delete[] d;

        neg = B.neg;
        nDig = B.nDig;
        d = B.d;
        //mesma coisa aqui!
        B.neg = false;
        B.nDig = 1;
        B.d = new int8_t[1]{0};
    }
    return *this;
}

/// Construtor especifico a partir de inteiro longo.
/// Tambem conversor de long long int para BigInt.
/// NAO PODE SER MODIFICADO NAS PARTES JAH IMPLEMENTADAS.
/// PODE (E PRECISA) RECEBER ACRESCIMOS, APENAS
/// NAS PARTES INDICADAS POR /* ACRESCENTAR */
BigInt::BigInt(long long int N)
  : BigInt(N < 0, (N == 0 ? 1 : 1 + int(log10(fabs(N)))))
{
  // Calcula os digitos, usando divisao inteira por 10
  for (int i=0; i<size(); ++i)
  {
    d[i] = abs(N%10); // Modulo do resto da divisao
    N /= 10;          // Divisao inteira
  }
}


/// Conversor de BigInt para long long int
long long int BigInt::toInt() const
{
    long long int val = 0;


    for (int i = nDig - 1; i >= 0; --i)
    {
        val = 10 * val + d[i];


        if (val < 0)
        {
            std::cerr << "Overflow ao converter BigInt para long long\n";
            return 0;
        }
    }

    if (neg)
        val = -val;

    return val;
}
///Operator[]
int BigInt::operator[](int i) const
{
    if (i < 0 || i >= nDig) {return 0;}

    return static_cast<int>(d[i]);
}
///Função isZero()
bool BigInt::isZero() const
{
   return (nDig == 1 && d[0] == 0);
}

/// ******************
/// * FIM DA PARTE 1 *
/// ******************

/// Funcao privada que corrige o numero, caso haja inconsistencias
void BigInt::correct(){
    int newSize = size();
    // remove zeros mais significativos
    while(newSize > 1 && d[newSize-1] == 0){
        newSize--;
    }
    // copia apenas os digitos validos
    if(newSize != size()){
        BigInt prov(neg, newSize);
        for(int i = 0; i < newSize; ++i){
            prov.d[i] = d[i];
        }
        *this = prov;
    }
    if(isZero()){
        neg = false;
    }
}

/// Construtor especifico a partir de string.
/// Nao eh conversor de string para BigInt.
/// Delega ao construtor default.
/// NAO PODE SER MODIFICADO NAS PARTES JAH IMPLEMENTADAS.
/// PODE (E PRECISA) RECEBER ACRESCIMOS, APENAS
/// NAS PARTES INDICADAS POR /* ACRESCENTAR */
BigInt::BigInt(const string& S)
  : BigInt() // Valor inicial zero
{
  // Se string vazia, emite erro e permanece com valor inicial zero
  if (S.empty())
  {
    cerr << "empty string cannot create a BigInt\n";
    return;
  }

  // Posicao onde comecam os digitos, inicialmente zero
  size_t ini=0;
  // Leva em conta o sinal
  bool IsNeg = false;

  if (S[0]=='+' || S[0]=='-')
  {
    // Se nao tem nenhum digito alem do sinal, emite erro e permanece com valor inicial
    if (S.size()==1)
    {
      cerr << "sign-only string cannot create a BigInt\n";
      return;
    }
    IsNeg = (S[0]=='-');
    ++ini;
  }

  // Faz ter sinal (IsNeg) e numero de digitos (tamanho da string - ini) corretos
  *this = BigInt(IsNeg, static_cast<int>(S.size() - ini));

  // Calculo dos digitos do BigInt
  for (int i=0; i<size(); ++i)
  {
    char c = S[S.size()-1-i];
    if (!isdigit(c))
    {
      *this = BigInt(); // = 0
      cerr << "string with invalid character cannot create a BigInt\n";
      return;
    }
    d[i] = static_cast<int8_t>(c-'0');
  }
  // Corrige eventuais numeros fora da especificacao
  correct();
}

/// ******************
/// * FIM DA PARTE 2 *
/// ******************

/// Insercao (impressao)
std::ostream& operator<<(std::ostream& out, const BigInt& B)
{
    if (B.neg)
    {
        out << '-';
    }

    for (int i = B.size() - 1; i >= 0; --i)
    {
        out << B[i];
    }
    return out;
}

/// Extracao (digitacao).
/// NAO PODE SER MODIFICADO NAS PARTES JAH IMPLEMENTADAS.
/// PODE (E PRECISA) RECEBER ACRESCIMOS, APENAS
/// NAS PARTES INDICADAS POR /* ACRESCENTAR */
std::istream& operator>>(istream& I, BigInt& B)
{
  // Valor inicial zero
  B = BigInt(); // = 0

  // Testa a stream de entrada e descarta eventuais separadores iniciais.
  // Em caso de erro, encerra a digitacao.
  istream::sentry s(I);
  if (!s) return I;

  // Caractere lido da stream
  int c;

  // Inspeciona o primeiro caractere que serah lido
  c = I.peek();

  // Testa se o primeiro caractere eh um sinal.
  // Se for, consome (elimina do buffer), processa e inspeciona o proximo caractere.
  if (c=='+' || c=='-')
  {
    // Consome
    c = I.get();
    // Atribui o sinal
    B.neg = (c=='-');
    // Obtem o proximo caractere
    c = I.peek();
  }

  // Numero de digitos que foram digitados
  int numDigitos = 0;

  // Testa se eh um caractere valido: digitos 0 a 9
  while (isdigit(c))
  {
    // Consome do buffer
    c = I.get();
    ++numDigitos;

    if (numDigitos>1)
        {
        // Faz o BigInt manter o sinal e passar a ter size()+1 digitos,
        // avancando todos para uma posicao mais significativa aa frente.
        BigInt prov(B.neg, B.size() + 1);
            for (int i = 0; i < B.size(); ++i)
            {
                prov.d[i + 1] = B.d[i];
            }
            B = prov;
        }


    // Acrescenta o novo digito como sendo o primeiro (o menos significativo)
    B.d[0] = static_cast<int8_t>(c-'0');

    // Inspeciona o proximo caractere que vai ser lido
    c = I.peek();
  }

  // Assinala erro na stream se nenhum digito foi lido
  if (numDigitos==0) I.setstate(ios::failbit);

  // Corrige eventuais erros na digitacao
  B.correct();

  // Encerra a digitacao
  return I;
}

/// ******************
/// * FIM DA PARTE 3 *
/// ******************

/// Teste de igualdade
bool BigInt::operator==(const BigInt& B) const
{
    if (neg != B.neg) {return false;}
    if (nDig != B.nDig) {return false;}

    for (int i = 0; i < nDig; ++i)
    {
        if (d[i] != B.d[i]) {return false;}
    }

    return true;
}

bool BigInt::operator!=(const BigInt& B) const
{
    return !((*this) == B);
}

/// Menor que
bool BigInt::operator<(const BigInt& B) const
{
    if (neg != B.neg)
    {
        return neg;
    }

    if (!neg)
    {
        if (nDig != B.nDig)
        {
            return nDig < B.nDig;
        }

        for (int i = nDig - 1; i >= 0; --i)
        {
            if (d[i] != B.d[i])
            {
                return d[i] < B.d[i];
            }
        }
    }
    else
    {
        if (nDig != B.nDig)
        {
            return nDig > B.nDig;
        }

        for (int i = nDig - 1; i >= 0; --i)
        {
            if (d[i] != B.d[i])
            {
                return d[i] > B.d[i];
            }
        }
    }

    return false;
}
bool BigInt::operator>(const BigInt& B) const
{
    return B < (*this);
}

bool BigInt::operator>=(const BigInt& B) const
{
    return !((*this) < B);
}

bool BigInt::operator<=(const BigInt& B) const
{
    return !(B < (*this));
}
/// ******************
/// * FIM DA PARTE 4 *
/// ******************

/// Funcao privada que incrementa os digitos (o modulo) do numero
void BigInt::increment() {
    int k = 0;

    while (k < nDig && d[k] == 9) {
        d[k] = 0;
        k++;
    }

    if (k < nDig) {
        d[k] += 1;
    }
    else {

        int8_t* novo = new int8_t[nDig + 1];

        for (int i = 0; i < nDig; i++) {
            novo[i] = 0;
        }

        novo[nDig] = 1;

        delete[] d;
        d = novo;
        nDig++;
    }
}

/// Funcao privada que decrementa os digitos (o modulo) do numero
void BigInt::decrement() {


    if (isZero()) {
        neg = true;
        d[0] = 1;
        return;
    }

    int k = 0;

    while (k < nDig && d[k] == 0) {
        d[k] = 9;
        k++;
    }

    if (k < nDig) {
        d[k] -= 1;
    }

    if (d[nDig - 1] == 0) {
        correct();
    }
}
/// Operador de incremento pre-fixado
/// NAO PODE SER MODIFICADO
BigInt& BigInt::operator++()
{
  if (!isNeg()) increment();
  else decrement();
  return *this;
}

/// Operador de decremento pre-fixado
/// NAO PODE SER MODIFICADO
BigInt& BigInt::operator--()
{
  if (isNeg()) increment();
  else decrement();
  return *this;
}

/// Operador de incremento pos-fixado
BigInt BigInt::operator++(int) {
    BigInt temp(*this);

    ++(*this);

    return temp;
}

/// Operador de decremento pos-fixado
BigInt BigInt::operator--(int) {
    BigInt temp(*this);

    --(*this);

    return temp;
}

/// ******************
/// * FIM DA PARTE 5 *
/// ******************

/// Modulo (abs)
BigInt abs(const BigInt& X)
{
    BigInt R(X);
    R.neg = false;
    return R;
}
/// Soma (unario)
const BigInt& operator+(const BigInt& X)
{
    return X;
}
/// Negativo (unario)
BigInt operator-(const BigInt& X)
{
    BigInt R(X);

    if (!R.isZero()) {
        R.neg = !R.neg;
    }

    return R;
}

/// Soma (binário)
BigInt operator+(const BigInt& A, const BigInt& B)
{
    if (A.isNeg() == B.isNeg()) {

        BigInt C;

        C.neg = A.isNeg();
        C.nDig = 1 + std::max(A.size(), B.size());

        delete[] C.d;
        C.d = new int8_t[C.nDig];

        for (int i = 0; i < C.nDig; i++)
            C.d[i] = 0;

        int carry = 0;
        // soma digito a digito (igual conta normal)
        for (int i = 0; i < C.nDig; i++) {

            C.d[i] = A[i] + B[i] + carry;

            if (C.d[i] > 9) {
                C.d[i] -= 10;
                carry = 1;
            } else {
                carry = 0;
            }
        }

        C.correct();
        return C;
    }

    else {

        if (abs(A) >= abs(B)) {

            BigInt C;

            C.neg = A.isNeg();
            C.nDig = A.size();

            delete[] C.d;
            C.d = new int8_t[C.nDig];

            for (int i = 0; i < C.nDig; i++)
                C.d[i] = 0;

            int borrow = 0;

            for (int i = 0; i < C.nDig; i++) {

                C.d[i] = A[i] - B[i] - borrow;

                if (C.d[i] < 0) {
                    C.d[i] += 10;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
            }

            C.correct();
            return C;
        }
        else {
            return B + A;
        }
    }
}
///Subtração(binário)
BigInt operator-(const BigInt& A, const BigInt& B)
{
    return A + (-B);
}

/// ******************
/// * FIM DA PARTE 6 *
/// ******************

/// Multiplicacao
BigInt BigInt::operator*(const BigInt& B) const
{
    if (isZero() || B.isZero())
        return BigInt();

    bool sinal = (neg != B.neg);

    BigInt C(sinal, nDig + B.nDig);

    for (int i = 0; i < nDig; ++i)
    {
        if (d[i] != 0)
        {
            for (int j = 0; j < B.nDig; ++j)
            {
                int k = i + j;
                C.d[k] += d[i] * B.d[j];

                while (C.d[k] > 9)
                {
                    int carry = C.d[k] / 10;
                    C.d[k] %= 10;
                    ++k;
                    C.d[k] += carry;
                }
            }
        }
    }

    C.correct();
    return C;
}

/// Fatorial
BigInt BigInt::operator!() const
{
    if (neg)
    {
        cerr << "Erro: fatorial de numero negativo.\n";
        return BigInt(0);
    }

    BigInt C(1);

    for (BigInt N(2); N <= *this; ++N)
    {
        C = C * N;
    }

    return C;
}

/// ******************
/// * FIM DA PARTE 7 *
/// ******************

/// Deslocamento aa esquerda
BigInt BigInt::operator<<(int N) const
{
    if (N <= 0 || isZero())
        return *this;

    BigInt C(neg, nDig + N);

    // desloca para esquerda (multiplica por 10^N)
    for (int i = 0; i < N; ++i)
        C.d[i] = 0;

    // copia os digitos deslocados
    for (int i = N; i < C.nDig; ++i)
        C.d[i] = d[i - N];

    C.correct();
    return C;
}

/// Deslocamento ah direita
BigInt BigInt::operator>>(int N) const
{
    if (N <= 0)
        return *this;

    if (N >= nDig)
        return BigInt(0);

    BigInt C(neg, nDig - N);

    for (int i = 0; i < C.nDig; ++i)
        C.d[i] = d[i + N];

    C.correct();
    return C;
}

/// Divisao de *this por D.
/// Armazena o resultado (quociente) em Q e o resto da divisao em R.
void BigInt::division(const BigInt& D, BigInt& Q, BigInt& R) const
{
    Q = BigInt(0);
    R = BigInt(0);

    if (D.isZero())
    {
        cerr << "Erro: divisao por zero.\n";
        return;
    }

    if (isZero())
        return;

    BigInt A = *this;
    A.neg = false;

    BigInt absD = D;
    absD.neg = false;

    if (A < absD)
    {
        R = A;
        if (!R.isZero())
            R.neg = neg;
        return;
    }

    for (int i = A.nDig - 1; i >= 0; --i)
    {
        // atualiza o resto parcial (desce um digito)
        R = R << 1;
        R.d[0] = A.d[i];
        R.correct();

        int div = 0;
        // subtrai ate nao dar mais
        while (R >= absD)
        {
            R = R - absD;
            ++div;
        }
        // atualiza o quociente
        Q = Q << 1;
        Q.d[0] = div;
        Q.correct();
    }

    Q.correct();
    R.correct();

    if (!Q.isZero())
        Q.neg = (neg != D.neg);

    if (!R.isZero())
        R.neg = neg;
}

/// Quociente da divisao inteira
BigInt BigInt::operator/(const BigInt& D) const
{
    BigInt Q, R;
    division(D, Q, R);
    return Q;
}

/// Resto da divisao inteira
BigInt BigInt::operator%(const BigInt& D) const
{
    BigInt Q, R;
    division(D, Q, R);
    return R;
}

/// ******************
/// * FIM DA PARTE 8 *
/// ******************
