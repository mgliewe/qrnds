/**
 * 
 */
template<class T> class int_value_type {
    T a;
public:

    inline int_value_type(int x) : a(x) {  }
    inline operator int() { return a; }
    inline operator double() { return a; }

    inline bool operator >(T b) { return a>b; }
    inline bool operator >=(T b) { return a>=b; }
    inline bool operator <(T b) { return a<b; }
    inline bool operator <=(T b) { return a<=b; }
    inline bool operator ==(T b) { return a==b; }
    inline bool operator !=(T b) { return a!=b; }

    inline int_value_type operator +(T b) { return a+b; }
    inline int_value_type operator -(T b) { return a-b; }
    inline int_value_type operator *(T b) { return a*b; }
    inline int_value_type operator /(T b) { return a/b; }
    inline int_value_type operator %(T b) { return a%b; }

    inline int_value_type abs() { return a>0 ? a:-a; }
    inline int_value_type floor() { return a; }

};

template<class T> class float_value_type {
    T a;
public:

    inline float_value_type(double x) : a(x) {  }
    inline operator int() { return a; }
    inline operator double() { return a; }

    inline bool operator >(T b) { return a>b; }
    inline bool operator >=(T b) { return a>=b; }
    inline bool operator <(T b) { return a<b; }
    inline bool operator <=(T b) { return a<=b; }
    inline bool operator ==(T b) { return a==b; }
    inline bool operator !=(T b) { return a!=b; }

    inline float_value_type operator +(T b) { return a+b; }
    inline float_value_type operator -(T b) { return a-b; }
    inline float_value_type operator *(T b) { return a*b; }
    inline float_value_type operator /(T b) { return a/b; }

    inline float_value_type operator %(T b) { return fmod(a,b); }

    inline float_value_type abs() { return fabs(a); }
    inline float_value_type floor() { return std::floor(a); }

};
