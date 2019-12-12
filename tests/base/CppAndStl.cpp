#include <catch2/catch.hpp>
#include <initializer_list>
#include <string>
#include <fmt/format.h>
#include <vector>
#include <unordered_set>

template <class T>
struct unimplemented;

template <class T>
void foo(T &&t1) {
    unimplemented<T> un1;
    return;
}

//字符串或数组作为参数传递给'万用模板参数'
TEST_CASE("make_pair传字符串做参数测试", "cppstl") {
	// foo("abcd");
	fmt::print("{}\n", R"(
		字符串作为实参传递给引用型模板形参时，推导结果是T为数组类型，并非string或者char *类型!
		具体参考<Effective Morden C++>条款1 -- "模板参数推导"
		本例中foo("abcd")推导T类型为: const char (&)[5]
	)");
}

//std::decay_t的使用
TEST_CASE("decay_t的使用", "cppstl") {
    int array[] = {1,2,3,4};
    auto t2 = std::make_pair(array, 1);
    t2.first[0] = 5;
    REQUIRE(array[0] == 5);

	fmt::print("{}\n", R"(
		非常量数组作为实参传递给引用型模板形参时，推导结果是T为数组类型引用：int (&)[4]
		std::make_pair基于效率考虑采用了std::decay_t将其退化为指针
		实现类似如下：
		template <class T1, class T2> 
		inline pair< typename decay<T1>::type, typename decay<T2>::type > 
		make_pair(T1&& x, T2&& y) { 
			return pair< std::decay_t<T1>, std::decay_t<T2>>(std::forward<T1>(x), std::forward<T2>(y)); 
		}

		因此本例中通过pair中first元素修改同样影响原array, 因为他们都指向同一块地址!!
	)");
}

TEST_CASE("initialist test", "cppstl") {
    std::initializer_list<std::string> strlist{"abc", "def", "ghk"};
    (*(const_cast<std::string *>(strlist.begin())))[0] = 'x';
    fmt::print("{}\n", *strlist.begin());
    
    std::initializer_list<const char *> strlist2{"abc", "def", "ghk"};
    fmt::print("{}\n", *strlist2.begin());

	fmt::print("{}\n", R"(
		本例中std::initializer_list<std::string>编译器会生成一个std::string型数组
		而std::initializer_list<const char *>只会生成const char *型数组,效率要明显高于第一种方法！
	)");
}

//rbegin().base() == end()
TEST_CASE("vector reverse-iterator test", "parsebase") {
    std::vector<int> vec{1,2,3,4,5};
	fmt::print("{}\n", *vec.begin());

	fmt::print("{}\n", R"(
		通过本例验证 rbegin().base() == end(), 不能直接使用!
	)");
}

TEST_CASE("set merge test", "parsebase") {
    std::unordered_set<int> seta{1,2,3};
    std::unordered_set<int> setb{3,4,5};
    
    REQUIRE(seta.size() == 3);
    REQUIRE(setb.size() == 3);
    
    seta.merge(setb);
    
    REQUIRE(seta.size() == 5);
    REQUIRE(setb.size() == 1);
    REQUIRE(*setb.begin() == 3);

	fmt::print("{}\n", R"(
		本例中执行seta.merge(setb)后，setb部分元素'剪切'到了seta中
	)");
}
