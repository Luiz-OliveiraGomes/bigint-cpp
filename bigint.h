#ifndef BIGINT_H_INCLUDED
#define BIGINT_H_INCLUDED


class BigInt {
    private:
        bool neg;
        int nDig;
        int8_t* d;
        BigInt(bool isNeg, int size);
        void correct();
        void increment();
    void decrement();
    public:
        /// Construtor default e destrutor
        BigInt();
        ~BigInt();
        ///Construtor e atribuição por cópia
        BigInt(const BigInt& B);
        BigInt& operator=(const BigInt& B);
        ///Construtor e atribuição por movimento
        BigInt(BigInt&& B) noexcept;
        BigInt& operator=(BigInt&& B) noexcept;
        ///Construtor específico long long int
        BigInt(long long int);
        ///Funções
        bool isNeg() const;
        int size() const;
        long long int toInt() const;
        bool isZero() const;
        ///Operator[]
        int operator[](int i) const;
        friend std::ostream& operator<<(std::ostream& out, const BigInt& B);
        friend std::istream& operator>>(std::istream& in, BigInt& B);
        ///Construtor que cria um bigint a partir de uma string
        explicit BigInt(const std::string& S);
        ///Operadores de lógica
        bool operator==(const BigInt& B) const;
        bool operator!=(const BigInt& B) const;
        bool operator<(const BigInt& B) const;
        bool operator>(const BigInt& B) const;
        bool operator>=(const BigInt& B) const;
        bool operator<=(const BigInt& B) const;
        ///Operadores pré-fixados
        BigInt& operator++();
        BigInt& operator--();
        ///Operadores pós-fixados
        BigInt operator++(int);
        BigInt operator--(int);
        ///Função módulo, friend, porque acessa privates da classe.
        friend BigInt abs(const BigInt& X);
        ///Operadores unários
        friend const BigInt& operator+(const BigInt& X);
        friend BigInt operator-(const BigInt& X);
        ///Operadores binários
        friend BigInt operator+(const BigInt& A, const BigInt& B);
        friend BigInt operator-(const BigInt& A, const BigInt& B);
        ///Operadores de multiplicação e fatorial
        BigInt operator*(const BigInt& B) const;
        BigInt operator!() const;
        /// Deslocamentos
        BigInt operator<<(int N) const;
        BigInt operator>>(int N) const;
        /// Divisão e resto
        BigInt operator/(const BigInt& D) const;
        BigInt operator%(const BigInt& D) const;
        ///Método auxiliar da divisão
        void division(const BigInt& D, BigInt& Q, BigInt& R) const;

};

#endif // BIGINT_H_INCLUDED
