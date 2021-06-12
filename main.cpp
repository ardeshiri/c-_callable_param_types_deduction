#include <iostream>
#include <typeinfo>
#include <functional>

using namespace std;



template <typename...T>
struct type_container;

template<typename T, typename ...Ts>
struct type_container<T, Ts...>
{
    static const size_t container_size = sizeof...(Ts)+1;
    using child_container = type_container<Ts...>;
    using current_type = T;
};

template<typename T>
struct type_container<T>
{
    static const size_t container_size = 1;
    using child_container = type_container<>;
    using current_type = T;
};

template<>
struct type_container<>
{
    static const size_t container_size = 0;
};





template<size_t n, typename T>
struct nth_type:nth_type<n-1, typename T::child_container>
{
    static const size_t nth_num = n;
};

template<typename T>
struct nth_type<0,T>
{
    static const size_t nth_num = 0;
    using type = typename T::current_type;
};

template<size_t n, typename T>
using nth_type_t = typename nth_type<n, T>::type;





template<typename T>
struct _callable_types;

template<typename T, typename...U>
struct _callable_types<std::function<T(U...)>>
{
    using return_type = T;
    using args = type_container<U...>;
};


template<typename F>
struct callable_types
{
    callable_types(F){}
    using function_type = decltype( std::function{declval<F>()} );
    using return_type = typename _callable_types<function_type>::return_type;
    using arguments = typename _callable_types<function_type>::args;
};







double func(pair<int,float>,string, int)
{
    return 33.1;
}

struct func_obj
{
    void* operator()(float, string, const tuple<int,char,double>&)
    {
        return nullptr;
    }
};

int main()
{
    auto lambda = [](int, uint64_t){ cout<<"running"<<endl; return string{"123asd"}; };

    using func_args = nth_type_t<0,callable_types<decltype(func)>::arguments>;
    using func_obj_arge = nth_type_t<2,callable_types<decltype(func_obj{})>::arguments>;
    using lambda_args = nth_type_t<1,callable_types<decltype(lambda)>::arguments>;

    using func_ret = callable_types<decltype(func)>::return_type;
    using func_obj_ret = callable_types<decltype(func_obj{})>::return_type;
    using lambda_ret = callable_types<decltype(lambda)>::return_type;


    cout<<typeid(func_args{}).name()<<endl;
    cout<<typeid(func_obj_arge{}).name()<<endl;
    cout<<typeid(lambda_args{}).name()<<endl;

    cout<<typeid(func_ret{}).name()<<endl;
    cout<<typeid(func_obj_ret{}).name()<<endl;
    cout<<typeid(lambda_ret{}).name()<<endl;
}
