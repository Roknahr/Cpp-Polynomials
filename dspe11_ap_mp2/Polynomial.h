#pragma once

//Predeclare class and operator<< to overload the << operator.
template<class T>
class Polynomial;

template<class T>
std::ostream& operator<< (std::ostream& os, const Polynomial<T>& pol);

//2. Make a template class of Polynomial for a given type of coefficients
template<class T>
class Polynomial
{
    private:
        std::vector<T> coefficients_;
        mutable std::unique_ptr<std::map<T, T>> integral_cache;
        mutable std::mutex m;

        T integrate_help(const T x) const;
        std::vector<T> multiply_by_term(const T coef, const int degree) const;

    public:
        Polynomial();
        template<class C> Polynomial(const C& container);
        Polynomial(const std::initializer_list<T> list);
        Polynomial(const Polynomial<T>& other);
        Polynomial(Polynomial<T>&& other);
        Polynomial<T>& operator= (Polynomial<T>&& rhs);

        std::vector<T> get_coefficients() const;
        friend std::ostream& operator<< <>(std::ostream& os,
                                           const Polynomial<T>& pol);
        void scale(const T scalar);
        void add_root(const T root);
        void add_to_cache(const T key, const T value) const;
        template<class C> void add_roots(const C& container);
        T eval(const T& x) const;
        T integrate(const T a, const T b) const;
        Polynomial<T> derive() const;
        Polynomial<T> operator+ (const Polynomial<T>& rhs);
        Polynomial<T> operator* (const Polynomial<T>& rhs);
        
        
};


//Implementation here because template classes are not compiled.

//Return coefficient vector
template<class T>
std::vector<T> Polynomial<T>::get_coefficients() const
{
    return coefficients_;
}


//Overload << to give a human readable representation of the polynomial
template<class T>
std::ostream& operator<<(std::ostream& os, const Polynomial<T>& pol)
{

    os << pol.coefficients_.front();

    if (pol.coefficients_.size() > 1)
    {
        for (auto it = pol.coefficients_.cbegin() + 1;
            it != pol.coefficients_.cend(); ++it)
        {
            if (*it != 0)
            {
                os << " + " << *it << "x^" << it - pol.coefficients_.begin();
            }
        }
    }

    return os;
}


//Copy constructor
template<class T>
Polynomial<T>::Polynomial(const Polynomial<T>& other) 
    : coefficients_(other.coefficients_), 
    integral_cache(new std::map<T, T>(*other.integral_cache))
{
    //Print for demonstration purposes.
    std::cout << "(Copy ctor) ";
}


//7. Implement move semantics by using [...] smart pointers.
//Move constructor
template<class T>
Polynomial<T>::Polynomial(Polynomial<T>&& other) 
    : coefficients_(other.coefficients_),
    integral_cache(std::move(other.integral_cache))
{
    //Print for demonstration purposes.
    std::cout << "(Move ctor) ";
}

//Move assignment operator
template<class T>
Polynomial<T>& Polynomial<T>::operator= (Polynomial<T>&& other)
{
    coefficients_ = other.coefficients_;
    integral_cache = std::move(other.integral_cache);
    return *this;
}


//1. (a) Default constructor to create a trivial polynomial: 0.
template<class T>
Polynomial<T>::Polynomial() : coefficients_(1, 0), integral_cache(new std::map<T, T>){}  //


//1. (b) Constructor for specific degree term coefficients.
//5. Member functions accepting containers should support any type of 
//   container, including array types(Item 13); also support braced 
//   initializers (Item 7).
template<class T>
Polynomial<T>::Polynomial(const std::initializer_list<T> list) 
    : coefficients_(list), integral_cache(new std::map<T, T>){}

template<class T>
template<class C>
Polynomial<T>::Polynomial(const C& container) 
    : coefficients_(0), integral_cache(new std::map<T, T>)
{
    for (auto it = std::begin(container); it != std::cend(container); ++it)
    {
        coefficients_.push_back(*it);
    }
}

//1. (c) A method to scale the polynomial, i.e. multiply by a scalar value.
//4. Use const where applicable
template<class T>
void Polynomial<T>::scale(const T scalar)
{
    std::transform(coefficients_.begin(), 
        coefficients_.end(), 
        coefficients_.begin(),
        [scalar](T c){ return c * scalar; });

    //Previous integrations are useless now.
    integral_cache->clear();
}

//1. (d) A method to add a root r, i.e. multiply by a term (x-r).
template<class T>
void Polynomial<T>::add_root(const T root)
{
    //Save a copy of the current coefficients
    std::vector<T> temp = coefficients_;

    //Multiply coefficients by negative root
    scale(-root);

    //Add a 0 to the beginning, efficiently multiplying by x
    temp.insert(temp.cbegin(), 0);

    //Add an extra 0 to the end match the sizes
    coefficients_.resize(coefficients_.size() + 1, 0);

    //Adding the 0 to the beginning and end, respectively,
    //ensures the right term degrees are added together here:
    std::transform(coefficients_.begin(), 
        coefficients_.end(), temp.begin(), 
        coefficients_.begin(), std::plus<T>());

    //Previous integrations are useless now.
    integral_cache->clear();
}

//1. (e) A method to add several roots at once.
//5. Member functions accepting containers should support any type of 
//   container, including array types
template<class T>
template<class C>
void Polynomial<T>::add_roots(const C& container)
{
    //3. Use auto where applicable.
    for (auto it = std::cbegin(container); it != std::cend(container); ++it)
    {
        add_root(*it);
    }
}


//1. (f) A method to valuate the polynomial at a given point.
template<class T>
T Polynomial<T>::eval(const T& x) const
{
    //Starting at -1 instead of 0 to use ++degree
    //as it is more efficient than degree++
    T degree(-1);

    return std::accumulate(coefficients_.cbegin(),
        coefficients_.cend(),
        T(0.0),
        [&](T a, T c){ return a + c * (T)pow(x, ++degree); });
}



//1. (g) A method to compute a polynomial which is a derivative of the polynomial.
template<class T>
Polynomial<T> Polynomial<T>::derive() const
{
    T degree(1);
    std::vector<T> derived_coefficients;
    derived_coefficients.resize(coefficients_.size() - 1);

    //Multiply coefficient with term degree, but ignore the first one (degree 0).
    std::transform(coefficients_.begin() + 1,
        coefficients_.end(),
        derived_coefficients.begin(),
        [&degree](T c){ return c * degree++; });

    return Polynomial<T>{derived_coefficients};
}

//6. Cache the integral data to avoid repetitive integration(Items 16, 40).
//   Make it thread-safe.
template<class T>
void Polynomial<T>::add_to_cache(const T key, const T value) const
{
    std::lock_guard<std::mutex> g(m);
    integral_cache->insert(std::pair<T, T>(key, value));
}

//9. Use lambda expressions(Chapter 6, e.g.when computing sums during
//	  evaluation of a polynomial; dispatch asynchronous computation).
template<class T>
T Polynomial<T>::integrate_help(const T x) const
{
    T degree(0);

    auto it = integral_cache->find(x);

    if (it != integral_cache->end())
    {
        //Print for demonstration purposes
        std::cout << "Fetched from cache! ";
        return it->second;
    }

    T result = std::accumulate(coefficients_.cbegin(),
        coefficients_.cend(),
        T(0.0),
        [&](T acc, T c)
        {
            ++degree;
            return acc + ((c / degree) * (T)pow(x, degree)); 
        });

    add_to_cache(x, result);

    return result;
}

//1. (h) A method to compute an integral for given interval bounds.
template<class T>
T Polynomial<T>::integrate(const T a, const T b) const
{
    //8. Use type traits
    static_assert(!std::is_integral<T>::value, 
        "Cannot integrate integral polynomials.");

    //10. Use concurrency
    auto a_bound = std::async(&Polynomial<T>::integrate_help, this, a);
    auto b_bound = std::async(&Polynomial<T>::integrate_help, this, b);

    return b_bound.get() - a_bound.get();
}

//1. (i) A plus operator to return a polynomial equal to a sum of 
//      two polynomials.
template<class T>
Polynomial<T> Polynomial<T>::operator+(const Polynomial<T>& rhs)
{
    //To accomodate different size vectors we have to copy
    //as we do not want to alter the original polynomiums.
    std::vector<T> ls = coefficients_;
    std::vector<T> rs = rhs.coefficients_;

    const auto size = std::max(ls.size(), rs.size());
    std::vector<T> result(size);

    //Make sure the two coefficient vectors are the same size
    if (ls.size() < size)
    {
        ls.resize(size, 0);
    }
    else if (rs.size() < size)
    {
        rs.resize(size, 0);
    }

    std::transform(ls.cbegin(), ls.cend(), rs.cbegin(),
        result.begin(), std::plus<T>());
    
    return Polynomial<T>(result);
}

template<class T>
std::vector<T> Polynomial<T>::multiply_by_term(const T coef, const int degree) const
{
    auto result = coefficients_;

    //Multiply each term by the coefficient
    std::transform(result.begin(),
        result.end(),
        result.begin(),
        [&](T c){ return c * coef; });

    //Shift values right, multiplying each term with x^degree
    result.insert(result.begin(), degree, 0);

    return result;
}

//1. (j) A star operator to return a polynomial equal to a product of two
//       polynomials.
template<class T>
Polynomial<T> Polynomial<T>::operator*(const Polynomial<T>& rhs)
{
    std::vector<std::vector<T>> temp_vectors;

    for (auto it = rhs.coefficients_.cbegin();
        it != rhs.coefficients_.cend(); ++it)
    {
        int degree = it - rhs.coefficients_.begin();
        temp_vectors.push_back(multiply_by_term(*it, degree));
    }

    Polynomial<T> result;

    for (auto it = temp_vectors.begin(); it != temp_vectors.end(); ++it)
    {
        result = result + Polynomial<T>(*it);
    }

    return result;
}
