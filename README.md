
# About type 'var'
   
  **It is a helper, lightweight JavaScript-like C++ type used for routine tasks.<br>
  The key word for the intention to use this type 'var' is 'sometimes', you should
  not plug all the 'holes' with this type
  ( this is especially true for beginners,experienced developers know the price of wrappers and will not abuse it ),
  but 'sometimes' some problems can be easily and conveniently solved by this type.**
  
  **In my opinion, the correct scope is:**
  
   - **while initialization process: ( for further use )**
     - **string parsing**
     - **creation of complex arrays, strings**
     - **...**
  
   - **sometimes as complex/variable return result and/or complex/variable
      parameter(s) for functions**
  
   - **complex arrays to maintain:**
     - **errors**
     - **logs**
     - **exceptions**
     - **...**
    
----

>### Please help to improve the project
  
**If it seems to you, or something is clearly not working as it should,
please write me. <br> ( as short example/description/other way ) <br>**
- **email:  dimon19811@outlook.com**             
- **GitHub: [https://github.com/dimonpokemon81/var/issues](https://github.com/dimonpokemon81/var/issues)**

---

>### Quick start

- **include "var.h" before using**

```java
#include "your_path/var.h"
```
- **use**: 
 
```java
var S = "Hello, World!";
...
```
- **example:**

```java
// HW.cpp

#include "var.h"

int main()
{
  
  var S = "Hello, World!";
  S.prnt();                 // "Hello, World!"
  
  return 0;
}
```
- **Compiling a program** ( all files in the same directory )

```
win-case:
c++ HW.cpp  -o HW.exe  -L. -lvar_win
./HW.exe 

linux-case:
c++ HW.cpp  -o HW  -L. -lvar_lnx
./HW 

bsd-case:
c++ HW.cpp  -o HW  -L. -lvar_bsd
./HW 

macOS-case:
c++ HW.cpp  -o HW  -L. -lvar_mac -std=c++17
./HW 
```
----

# Quick review ( some cases of usage and nuances )

>### Complex arrays

**Since var:arr( array ) is a wrapper of the std::vector, ( almost ) all <br>
std::vector interfaces are implemented and can be used. ( for more details see var.h )**
 
```java	
var s = "STR";
var a = { { "arr" }, { 2 } }, udf;

//    int(longlong)              
//        |            string    iterator: I_str   another arrays                 bool
//        |              |               |           |   / /    \                  |
var A = { 1, 0.555, "..text..", 'c', s.begin(), udf, a, { { 3 }, { a.begin() } }, true, var( { }) };
//             |                 |               |                     |                      |
//           double             char          undefined            iterator: I_arr        empty array
//
// note: all var:X are acceptable                 
//                                            
A._str().prnt(); // {1,0.555000,"..text..",'c',| [var:I_str] val: S |,|udf|,{ {"arr"},{2}},
//                  { {3},{| [var:I_arr] val: {"arr"} |}},true,{}}
// or
A.prnt();        // [ 9 ]{ 1,
//                         0.555000,
//                         "..text..",      ("STR" -> first character)
//                         'c',               ^
//                         | [var:I_str] val: S |,
//                         |udf|,
//                         [ 2 ]{ [ 1 ]{ "arr" },
//                                [ 1 ]{ 2 } },
//                         [ 2 ]{ [ 1 ]{ 3 },
//                                [ 1 ]{ | [var:I_arr] val: {"arr"} | } },
//                         true,
//                         { } }

// ...
```
**deep access**

```java
var A = { '>', { '>', { '>', { " !  ! " }, '<' }, '<' }, '<' };
//
A._str().prnt();  // {'>',{'>',{'>',{" !  ! "},'<'},'<'},'<'}
//
//   All var-interfaces (supported by this type) will work like this one.
//               ^      note: Some IDE (not the compiler) 
//               |      marking this way of access as error.
//               |      (need 'dancing with a tambourine' to solve this :) )
//               |
A[1][1][1][0].insert(3, "deep insert");
A._str().prnt();  // {'>',{'>',{'>',{" ! deep insert ! "},'<'},'<'},'<'}
A[1][1].at(1).at(0).push_back(":) ");
A._str().prnt();  // {'>',{'>',{'>',{" ! deep insert ! :) "},'<'},'<'},'<'}
//
var x = A.join();
x.prnt();         // >>> ! deep insert ! :) <<<

// ...
```
---
>### Strings

**Since var:str( string ) is a wrapper of the std::string, ( almost ) all <br>
std::string interfaces are implemented and can be used. ( for more details see var.h )**

```java

// Some features from high-level languages: ( new features will be added with time )

var S = " s t r i n g ";

S.trim().prnt();	// "s t r i n g"
S.ssplit(' ');
S._str().prnt();	// {'s','t','r','i','n','g'}
S.push_back('!');
S._str().prnt();	// {'s','t','r','i','n','g','!'}
S.sjoin();
S.prnt();	        // "string!"

var i = 12345678, s;
s = i._str();
s.prnt();                            // "12345678"
int ii = s.substr(3, 3)._int();
std::cout << ii << std::endl;	     //     456
				
// ...
```
---
>### Strings/Arrays

```java

var s = "str", sb = s.begin(), srb = s.rbegin(), udf;
var a = { 1 }, ab = a.begin(), arb = a.rbegin();
		
var A = { a, sb, "AB", ab, 2.5, "ABC", false, var({ }), srb, { 1, 2 }, // 
         'A', udf, 1, arb, 'B', true };
		
var ra = A.sort();   // ra = "[ 14 ]{ false,                     booleans
		     //               true,                         |
		     //               1,                         numbers 
		     //               2.500000,                     |
		     //               'A',                        chars
		     //               'B',                          |
		     //               "AB",                      strings
		     //               "ABC",                        |
		     //               { },                        arrays
		     //               [ 1 ]{ 1 },                   |
		     //               [ 2 ]{ 1,                     |
	             //                      2 },                   |
		     //               | [var:I_str] val: s |,    iterators
		     //               | [var:Ir_str] val: r |,      |
		     //               | [var:I_arr] val: 1 |,       |
		     //               | [var:Ir_arr] val: 1 |,      |                 
	             //               |udf| }"                   undefined
				
var S = "cB3bC2A1a";
var rs = S.sort();    // rs = "123ABCabc"

// or

a = { 6, 3, 7, 2, 9, 4, 1, 8, 0, 5 };
		
a.ssort_cmp([](auto a, auto b) {
	return b < a;
},true);
a._str().prnt(); //  {9,8,7,6,5,4,3,2,1,0}
a.ssort_cmp([](auto a, auto b) {
	return b < a;
},false);
a._str().prnt(); //  {0,1,2,3,4,5,6,7,8,9}
		
a = { 3, 7, "Abc", 2, 9, "ABC", 1, 0 };
		
a.ssort_cmp([](auto a, auto b) {
	if (a.is_str()) {
		if (b.is_str()) return b < a;
	} else if (a.is_int() || a.is_double()) {
		if (b.is_int() || b.is_double()) return b < a;
		else if (b.is_str()) return true;
	}
	return false;
});
a._str().prnt();  //  {"ABC","Abc",0,1,2,3,7,9}
                  //  {9,7,3,2,1,0,"Abc","ABC"} (reverse=true)

```

```java

var a = { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, x;
		
x = a.sample(5);   // x = "{7,8,2,7,6}"
x = a.sample();    // x = "{8,6,5,7,5,1,7,4,6}"
				
var s = "ABCDEFGHIJKLMNOPQ";
		
x = s.sample(10);  // x = "LMIEOLPAMN"
x = s.sample();    // x = "CCDQOMNLKHECMFJLN"
				
// unique sample
		
x = a.sample_unq(5);   // x = "{5,2,6,3,9}"
x = a.sample_unq();    // x = "{2,6,3,4,1,5,7,9,8}"
				
x = s.sample_unq(10);  // x = "NGCDBKJLHM"
x = s.sample_unq();    // x = "JMCQBAKNGILEOFHPD"

```

```java

// arr:
//         <--------------------
//         -------------------->
var a = { 'r', 'a', 'd', 'a', 'r' }, udf, x;
res = a.equal(a.rbegin(), a.rend(), 0); // true: "radar" is a palindrome

var a1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, a2 = { 3, 4, 5, 6, 7 };
//                 '''''''''                     '''''''''
x = a1.equal(a2.begin() += 1, a2.end() -= 1, 3, 3); // true

// different types example:
var a3 = { 1, 2.2, udf, true, { { { 1 } } }, "str", 'c', a.begin() };
var a4 = { 1, 2.2, udf, true, { { { 1 } } }, "str", 'c', a.begin() };
		
x = a3.equal(a4.begin(), a4.end());  // true
	   
// str: (same rules as for arrays)
  
var s = "level";
x = s.equal(s.rbegin(), s.rend()); // true: "level" is a palindrome

// ----
		
a1 = { 1, 2, 3, 4 };
a2 = { 1, 2, 3, 4 };
		
x = a1.mismatch(a2.begin(), a2.end()); //  false
				
a1 = { 1, 2, 3, 4 };                   // x = [ 4 ]{ | [var:I_arr] val: 4 |,
a2 = { 1, 2, 3, 5 };                   //            | [var:I_arr] val: 5 |,
	                               //            |udf|,
x = a1.mismatch(a2.begin(), a2.end()); //            |udf| }
				
a1 = { 1, 2, { 1, 2, 3 }, 3 };         // x = [ 4 ]{ | [var:I_arr] val: {1,2,3} |,
a2 = { 1, 2, { 1, 2, 4 }, 3 };         //            | [var:I_arr] val: {1,2,4} |,
				       //            | [var:I_arr] val: 3 |,
x = a1.mismatch(a2.begin(), a2.end()); //            | [var:I_arr] val: 4 | }
		
a1 = { 1, 2, { 1, 2, 3 }, 3 };         // x = [ 4 ]{ | [var:I_arr] val: {1,2,3} |,
a2 = { 1, 2, { 1, 2 }, 3 };            //            | [var:I_arr] val: {1,2} |,
				       //            | [var:I_arr] val: 3 |,
x = a1.mismatch(a2.begin(), a2.end()); //            | [var:I_arr] at the end() | }
				
a1 = { 1, { 1, { 1, { 2, 3, { { { { 5 } } } } } } }, 2 };
a2 = { 1, { 1, { 1, { 2, 3, { { { { 4 } } } } } } }, 2 };
		
x = a1.mismatch(a2.begin(), a2.end());
		
// x =  [ 4 ]{ | [var:I_arr] val: {1,{1,{2,3,{{{{5}}}}}}} |,
//             | [var:I_arr] val: {1,{1,{2,3,{{{{4}}}}}}} |,
//             | [var:I_arr] val: 5 |,
//             | [var:I_arr] val: 4 | }

```

```java
var a = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
a.sreverse();   // a = "{0,9,8,7,6,5,4,3,2,1}"
		
var s = "ABCDEFGHIJKLMNOPQ";
s.sreverse();   // s = "QPONMLKJIHGFEDCBA

```

```java

var a = { 1, 1, 1, 1, 1, 1, };
a.fill(2, 1, 2);   // "{1,2,2,1,1,1}"
a.fill(3, 6, 2);   // "{1,2,2,1,1,1,3,3}"

var s = "......";
s.fill('!', 1, 2);  // ".!!..."
s.fill('!', 6, 2);  // ".!!...??"

```

```java
...
```

---
>### Some ways to work with functions

```java
var base;
base.initArr();
//               strict types of input parameters   
//                          |
//             ...................................
auto add = [&](cch *item, double price, int amount) {
	if (price >= 0 && amount >= 0) {
		//
		//  simple addition of the complex arrays
		//        |
		base.push_back( { item, price, amount });
		//                    .-------------------> this is copy of the
		//           status   |     item            base[base.length() - 1]
		//return var( { "ok", base[base.length() - 1] });
		//      |             .-------------------> this is iterator (var:I_arr)  
		//  or  |             |                     pointing to the
		//      |    status   |     item            base[base.length() - 1] -> directly
		return var( { "ok", --base.end() });
		//      |                  
		//  verbose return 
		//   |
	    }   //   |    status           reason
	    return var( { "fail", "why: bad 'price' or 'amount'" });
};
		
add("book", 1.55, 10)._str().prnt();   // {"ok",| [var:I_arr] val: {"book",1.550000,10} |}
add("apple", 0.3, 100)._str().prnt();  // {"ok",| [var:I_arr] val: {"apple",0.300000,100} |}
add("cup", -5, 4)._str().prnt();       // {"fail","why: bad 'price' or 'amount'"} 

// JS approach
//                dynamic resolution of input parameters 
//                (additional checks required)
//                          |
//              ...............................
auto _add = [&](var item, var price, var amount) {
	if (price >= 0 && amount >= 0) {
				
		base.push_back( { item, price, amount });
		
		return var( { "ok", base[base.length() - 1] });
	}
	return var( { "fail", "why: bad 'price' or 'amount'" });
};
		
_add("book", 1.55, 10)._str().prnt();  // {"ok",{"book",1.550000,10}}
//_add(1.55, "apple", 10);             // exception thrown '[ unsupported_operation ]'
_add(0.3, 100, 777)._str().prnt();     // {"ok",{0.300000,100,777}} 
//                                     // (slippage of a bad argument '0.3')

// ...
		
//  P.S.
//  JS approach is bad in this case, given the capabilities of C++ 
```
---
>### Iterators ( loops )

```java
var S = "string", b = S.begin(), e = S.end();
		
while (b != e) {
	(*b).prnt();  // s t r i n g
	++b;
}
		
b = S.rbegin(), e = S.rend();
		
while (b != e) {
	(*b).prnt();  // g n i r t s
	++b;
}
		
var A = { 1, 2.2, 'A', "BCD", true };
		
b = A.begin(), e = A.end();
		
while (b != e) {
	(*b).prnt();  // 1 2.2 A BCD true
	++b;
}
		
b = A.rbegin(), e = A.rend();
		
while (b != e) {
	(*b).prnt();  // true BCD A 2.2 1
	++b;
}

// ...
```
**shorter versions:**

```java		
b = S.begin();
		
do {
	(*b).prnt();  // s t r i n g
} while (b.next());
		
b = A.rbegin();
		
do {
	(*b).prnt();  // true BCD A 2.2 1
} while (b.next());
		
// or
		
for (var i = 0; i < S.length(); ++i) {
	S[i].prnt();  // s t r i n g
}
		
// or
		
for (auto i : A) {
	i.prnt();     // 1 2.2 A BCD true
}
	
// or 

b = S.begin();

while (b) {
	(*b).prnt();  // s t r i n g
	++b;
}
	
//...
```
**note: Use prefix version of operator++,--() e.g  ++i,-\-i , because an additional temporary object is being created for i++,i-\- .**

**Invalidated iterators: ( some cases )**

```java	
var ab_ext;
var sb_ext;
		
    {
	var s = "STR"; //                                          
	var sb = s.begin(), se = s.end(); // <----------are become invalidate----------.  
	//                                                                             |  
	var a = { 1, 2 }; //                                                           |  
	var ab = a.begin(), ae = a.end(); // <----------are become invalidate----------|--.
	//                                                                             |  |
	var A = { 3, sb, 2, se, 1, { ab, ae } }; //                                    |  | 
	var Ab = A.begin(), Ae = A.end(); // <----------are become invalidate-------.  |  |
	//                                                                          |  |  |
	// push own-iterators                                                       |  |  |
	A.push_back( { Ab, Ae }); // -------------------cause---------------------->|  |  |
	//                                                                          |  |  |
	A.prnt(); //              [ 7 ]{ 3,                                         |  |  |
	//                               | [var:I_str] val: S |,                    |  |  |
	//                               2,                                         |  |  |
	//                               | [var:I_str] at the end() |,              |  |  |
	//                               1,                                         |  |  |
	//                               [ 2 ]{ | [var:I_arr] val: 1 |,             |  |  |
	//                                      | [var:I_arr] at the end() | },     |  |  |
	//                               [ 2 ]{ | [var:I_arr] is invalid |,      |--'  |  |
	//                                      | [var:I_arr] is invalid | } }   |     |  |
	//                                                                             |  |
	a.push_back("!"); // ---------------------------cause--------------------------|->|
	//                                                                             |  |
	s.resize(20); // -------------------------------cause------------------------->|  |
	//                                                                             |  |
	A.prnt(); //              [ 7 ]{ 3,                                            |  |
	//                               | [var:I_str] is invalid |,  |----------------|  |
	//                               2,                                            |  |
	//                               | [var:I_str] is invalid |,  |----------------'  |
	//                               1,                                               |
	//                               [ 2 ]{ | [var:I_arr] is invalid |,   |-----------'
	//                                      | [var:I_arr] is invalid | }, |
	//                               [ 2 ]{ | [var:I_arr] is invalid |,
	//                                      | [var:I_arr] is invalid | } }
	
	// any 'direct' operations on invalid iterators will throw exception:
			
	//*sb;     // excp: [ invalid_iterator ]
	//se--;    // excp: [ invalid_iterator ]
	//ab + 1;  // excp: [ invalid_iterator ]
	//ae -= 1; // excp: [ invalid_iterator ]
	// ...
	
	// It possible to check of the valid, but i don't recommend to do it.
	// This exceptions serve to show that the program has bad design (regarding iterators)
	// and can (have to) be fixed.
			
	(bool) sb; // false (if(sb) ...) 
	!sb;       // true  (if(!sb)...) 
	
	// NOTE:  **************************************************************************
			
	var q = ab;  // copy-constructor var(var&)/(const var&)  (1)          |       no
	var w;       //                                                       |  exceptions are
	w = ab;      // assignment operator operator=(var&)/(const var&) (2)  |     thrown
					
	//*q; // excp: [ invalid_iterator ]  |     'direct'
	//*w; // excp: [ invalid_iterator ]  |    operations   
			
	// As you can see from the above, only 'direct' operations on the iterators can throw
	// exceptions, and (1)/(2) look like  propagators of invalid iterators ,
	// but this is a compromise with memory leaks sometimes occurring while exceptions
	// in constructors (1) and  assignment operator (2) --> so keep it in mind.
			
	// **********************************************************************************
			
	// right now they are valid: (another case)
			
	ab_ext = s.begin(); // <--------------------------.
	//                         are become invalidate  |        
	sb_ext = a.begin(); // <--------------------------|       
	//                                                |
    }   //                                                | 
    //   cause:                                           |       
    //   end of the block scope -> source is destroyed -->'
		
//*ab_ext; // excp: [ invalid_iterator ]
//*sb_ext; // excp: [ invalid_iterator ]

// ...
```
**one more case**

```java		
var s = "string";
//           "trin"                    cause:
var itr = s.substr(1, 4).find("i"); // temporary substring (will be destroyed)---. 
//                                                                               |
//*itr; // excp: [ invalid_iterator ] <-------is become invalidate---------------' 

// one of the solutions ...

var ss;
//            "trin"
itr = (ss = s.substr(1, 4)).find("i");
*itr;       // 'i'
itr.pos();  //  2

// ...
```
**. . .** 

---
>### Self-transformations ( some cases )

```java
var i = 3, ii = 4;
		
i /= 2, i.prnt();      // int -> double : 1.5
		
i *= 2, i.prnt();      // double -> int : 3
		
i += "str", i.prnt();  // int -> string : "3str"
		
ii += 'c', ii.prnt();  // int -> string : "4c"
		
//
		
var c = 'a', cc = 'b', ccc = 'c';
		
c += 123, c.prnt();       // char -> string : a123
		
cc += ccc, cc.prnt();     // char -> string : bc
		
ccc += "!!!", ccc.prnt(); // char -> string : c!!!
		
//
		
var b = true, bb = false, bbb = true;
		
b -= 100, b.prnt();        // bool -> int    : -99
		
bb += 'c', bb.prnt();      // bool -> string : 0c
		
bbb += "XYZ", bbb.prnt();  // bool -> string : 1XYZ
		
// ... 
```
---
>### Few words about &,*

**Good idea: ( to improve performance )**

- **as function parameters:** 
    - `void some_fun(var &a, var *b)`
    - `void some_fun(const var &a, const var *b)`
    - **...**
- **maybe loops:**
 
```java
var A = { 1, 2, 3 }, *p;
int i = 0;
while (i < (int) A.length()) {
	//
	p = &A[i]; // take once
	//
	*p += 1;   //  | 
	p->prnt(); //  |  use as you want
	// ...         |
	i++;
}		
A._str().prnt();  // {2,3,4}

// ... 
```
**more details:**
- **arrays**

```java
// > & <
				
var rA = { 1, 'c', "str" };
//    .----^   ^-----. ^-------------.
//    |              |               |
var &ridx0 = rA[0], &ridx1 = rA[1], &ridx2 = rA[2];  // direct references (1)
				
ridx0.prnt(), ridx1.prnt(), ridx2.prnt();  // 1 c str
// bad use:
rA.push_back("push"); // -> memory relocation <- (or reassign e.g. rA="some val")
//                                V		
//ridx0.prnt();       //    undefined behavior
				
// > * < (same problem as with &)

var pA = { 1, 'c', "str" };
//    .---^    ^-----. ^---------------.
//    |              |                 |
var *pidx0 = &pA[0], *pidx1 = &pA[1], *pidx2 = &pA[2]; // direct pointers (2)
				
pidx0->prnt(), pidx1->prnt(), pidx2->prnt(); // 1 c str
// bad use:
pA.push_back("push"); // -> memory relocation  <- (or reassign e.g. pA="some val")
//                                V		
//pidx0->prnt();      //    undefined behavior

// ...
```

- **strings**

```java
// There is nuance:  ! keep it in mind !
				
// Strings in the current implementation cannot address each character access 
// as an array does for each element (1)/(2), so all accesses through
// opreator[..] and at(..) (3) functions calls go through a single internal
// buffer [var:char]  ----. , which means all references and pointers will contain
// the value (and address)| the last use of the functions (3).
//                        | 
// > & <                  |
//       ...............  |
//       |||           |  V
var S = "STR"; //  [var:char] -> single internal buffer
//                     ^
//                     | 
//       direct reference ( or pointer)   last use   
//       .------------>^<-------------.      |
//       |             |              |      |
var &ridx0 = S[0], &ridx1 = S[1], &ridx2 = S[2];
//  'S'->'T'->'R'  'T'->'R'       'R'   
//
ridx0.prnt(), ridx1.prnt(), ridx2.prnt(); // !!! all three refer to 'R' !!!
// bad use:
S.resize(20);     // -> memory relocation  <- (or reassign e.g. S="some val")
//                            V
//ridx0 = '?';    //    undefined behavior (write to unknown area)

// > * < (same problem as with &) ...

// ...
```
**Conclusion: If use of &,* is conscious -> then why not :)**

---
>### Simple parsing example

```java
var S = "v1 : i = 1 ; v2:d = 3.141 ; v3:s = string ; v4:c = A ;";  // incoming string
		
(var("in :  \"") += S + "\"").prnt();
		
var in = S.split(';'), beg = in.begin(), l, r, A;
A.initArr();
		
do {
	l = (*beg).split(':'), r = l[1].split('='), l[1].trim();
	if (l[1] == 'i') A.push_back( { l[0].trim(), r[1].trim()._int() });
	else if (l[1] == 'd') A.push_back( { l[0].trim(), r[1].trim()._double() });
	else if (l[1] == 's') A.push_back( { l[0].trim(), r[1].trim() });
	else if (l[1] == 'c') A.push_back( { l[0].trim(), r[1].trim()[0] });
} while (beg.next());
		
(var("out:  ") += A._str()).prnt();
		
// in : "v1 : i = 1 ; v2:d = 3.141 ; v3:s = string ;  v4:c = A ;"
// out: { {"v1",1},{"v2",3.141000},{"v3","string"},{"v4",'A'}} 
		
// and back again:
		
beg = A.begin();
S = "";
		
do {
	l = (*beg)[0], r = (*beg)[1];
	if (r.is_int()) S += l += var(":i=") += r;
	else if (r.is_double()) {
		S += l += var(":d=");
		var a = r._str().split('.');
		S += a[0];
		var res = a[1].find_last_not_of("0");    // <---.
		if (res) {                               //     |
		    S += '.';                            //     |
		    S += a[1].substr(0, res.pos() + 1);  // <--discard zeros
		}
	} else if (r.is_str()) S += l += var(":s=") += r;
	else if (r.is_char()) S += l += var(":c=") += r;
	S += ';';
} while (beg.next());

(var("back: \"") += S + "\"").prnt();  

// back: "v1:i=1;v2:d=3.141;v3:s=string;v4:c=A;" 
```
>###  Currently supported interfaces: ( for more details see var.h ) 

**local typedef:**
```    
const char         -> cch;
std::string        -> str;
long long          -> llong
unsigned long long -> ullong
```
**local define:**
```   
std::initializer_list  -> __iniL 
```

- **common interfaces**: 

```java
int size() const noexcept;
cch* gtid() const noexcept;
bool is_def() const noexcept;
bool is_bool() const noexcept;
bool is_int() const noexcept;
bool is_double() const noexcept;
bool is_char() const noexcept;
bool is_str() const noexcept;
bool is_arr() const noexcept;
bool is_itr() const noexcept;
bool is_I_str() const noexcept;
bool is_Ir_str() const noexcept;
bool is_I_arr() const noexcept;
bool is_Ir_arr() const noexcept;
void prnt() const noexcept;
```
- **string/array shared interfaces:** 

```java
var& at(size_t index);
const var& at(size_t index) const;
var& front();
const var& front() const;
var& back();
const var& back() const;
var begin();
const var begin() const;
var end();
const var end() const;
var rbegin();
const var rbegin() const;
var rend();
const var rend() const;
bool next() const;
llong pos() const;
bool empty() const;
size_t length() const;
size_t max_size() const;
void reserve(size_t new_cap);
size_t capacity() const;
void shrink_to_fit();
void clear();
void swap(var &with);
..
void resize(size_t count);
void resize(size_t count, const var &X);
void resize(size_t count, int X);
void resize(size_t count, float X);
void resize(size_t count, double X);
void resize(size_t count, long X);
void resize(size_t count, llong X);
void resize(size_t count, ullong X);
void resize(size_t count, char X);
void resize(size_t count, cch *X);
void resize(size_t count, const str &X);
void resize(size_t count, __iniL<var> X);
..
var insert(size_t index, const var &src_fist, const var &src_last);
var insert(size_t index, const var &X);
var insert(size_t index, int X);
var insert(size_t index, float X);
var insert(size_t index, double X);
var insert(size_t index, long X);
var insert(size_t index, llong X);
var insert(size_t index, ullong X);
var insert(size_t index, char X);
var insert(size_t index, cch *X);
var insert(size_t index, const str &X);
var insert(size_t index, __iniL<var> X);
var insert(size_t index, size_t count, const var &X);
var insert(size_t index, size_t count, int X);
var insert(size_t index, size_t count, float X);
var insert(size_t index, size_t count, double X);
var insert(size_t index, size_t count, long X);
var insert(size_t index, size_t count, llong X);
var insert(size_t index, size_t count, ullong X);
var insert(size_t index, size_t count, char X);
var insert(size_t index, size_t count, cch *X);
var insert(size_t index, size_t count, const str &X);
var insert(size_t index, size_t count, __iniL<var> X);
..
var erase(size_t index);
var erase(size_t first, size_t last);
..
var push_back(const var &X);
var push_back(int X);
var push_back(float X);
var push_back(double X);
var push_back(long X);
var push_back(llong X);
var push_back(ullong X);
var push_back(char X);
var push_back(cch *X);
var push_back(const str &X);
var push_back( __iniL<var> X);
..
var push_front(const var &X);
var push_front(int X);
var push_front(float X);
var push_front(double X);
var push_front(long X);
var push_front(llong X);
var push_front(ullong X);
var push_front(char X);
var push_front(cch *X);
var push_front(const str &X);
var push_front( __iniL<var> X);
...
var pop_back();
var pop_front(); 
..
var sort_cmp(bool (*cmp)(var &a, var &b), bool reverse = false) const; 
var& ssort_cmp(bool (*cmp)(var &a, var &b),bool reverse = false);
var sort(bool reverse = false) const;
var& ssort(bool reverse = false);
var sample(size_t num = 0) const;
var sample_unq(size_t num = 0) const;
var reverse();
var& sreverse();
bool equal(const var &src_first, const var &src_last, //
                                 size_t first = 0, size_t count = 0) const;
var mismatch(const var &src_first, const var &src_last, //
                                 size_t index = 0, size_t count = 0) const;
..
var& fill(const var &X, size_t index, size_t count);
var& fill(int X, size_t index, size_t count);
var& fill(float X, size_t index, size_t count);
var& fill(double X, size_t index, size_t count);
var& fill(long X, size_t index, size_t count);
var& fill(llong X, size_t index, size_t count);
var& fill(ullong X, size_t index, size_t count);
var& fill(char X, size_t index, size_t count);
var& fill(cch *X, size_t index, size_t count);
var& fill(const str &X, size_t index, size_t count);
var& fill(__iniL<var> X, size_t index, size_t count);
```

- **array interfaces:** 

```java
void initArr() noexcept;
var join(char sep = 0) const;
var& sjoin(char sep = 0);
```
- **string interfaces:** 

```java
cch* data() const;
var split(char sep) const;
var& ssplit(char sep);
var& ltrim();
var& rtrim();
var& trim();
var insert(size_t index, cch *src, size_t src_index, size_t src_count);
..
var& append(char ch, size_t count = 1);
var& append(const var &src, size_t src_index = 0, size_t src_count = 0);
var& append(cch *src, size_t src_index = 0, size_t src_count = 0);
var& append(str &src, size_t src_index = 0, size_t src_count = 0);
var& append(const var &src_first, const var &src_last);
..
var& replace(size_t index, size_t count, const var &src, size_t src_index = 0,
 			   size_t src_count = 0);
var& replace(size_t index, size_t count, cch *src, size_t src_index = 0,
			   size_t src_count = 0);
var& replace(size_t index, size_t count, str &src, size_t src_index = 0,
 			   size_t src_count = 0);
var& replace(size_t index, size_t count, const var &src_first,
 			   const var &src_last);
..
int compare(const var &with) const;
int compare(cch *with) const;
int compare(str &with) const;
int compare(size_t index, size_t count, const var &with, size_t with_index = 0,
 			  size_t with_count = 0) const;
int compare(size_t index, size_t count, cch *with, size_t with_index = 0,
			  size_t with_count = 0) const;
int compare(size_t index, size_t count, str &with, size_t with_index = 0,
 			  size_t with_count = 0) const;
..
bool starts_with(const var &prefix) const;
bool starts_with(char prefix) const;
bool starts_with(cch *prefix) const;
..
bool ends_with(const var &suffix) const;
bool ends_with(char suffix) const;
bool ends_with(cch *suffix) const;
..
var substr(size_t index = 0, size_t count = 0);
size_t copy(char *dest, size_t index = 0, size_t count = 0,
 		        bool zero = false) const;
..
var find(const var &src, size_t index = 0) const;
var find(char src, size_t index = 0) const;
var find(cch *src, size_t index = 0) const;
var find(str &src, size_t index = 0) const;
..
var rfind(const var &src, size_t index = 0) const;
var rfind(char src, size_t index = 0) const;
var rfind(cch *src, size_t index = 0) const;
var rfind(str &src, size_t index = 0) const;
..
var find_first_of(const var &src, size_t index = 0) const;
var find_first_of(char src, size_t index = 0) const;
var find_first_of(cch *src, size_t index = 0) const;
var find_first_of(str &src, size_t index = 0) const;
..
var find_first_not_of(const var &src, size_t index = 0) const;
var find_first_not_of(char src, size_t index = 0) const;
var find_first_not_of(cch *src, size_t index = 0) const;
..
var find_last_of(const var &src, size_t index = 0) const;
var find_last_of(char src, size_t index = 0) const;
var find_last_of(cch *src, size_t index = 0) const;
var find_last_of(str &src, size_t index = 0) const;
..
var find_last_not_of(const var &src, size_t index = 0) const;
var find_last_not_of(char src, size_t index = 0) const;
var find_last_not_of(cch *src, size_t index = 0) const;
var find_last_not_of(str &src, size_t index = 0) const;
```
- **explicit conversions:**

```java
int _int() const;
llong _llong() const;
ullong _ullong() const;
float _float() const;
double _double() const;
var _str() const;
```

># Plans
  
**If this project receives feedback ( if it will be of interest to at least 
someone at all ), or financially supported => this motivates me to devote 
more time to the project, up to full time.<br>
( current bugs will be fixed immediately, regardless of the above -^ )**
    
**to start:**
 - **extension of interfaces:**
    - **...**
    - **popular algorithms from the C++ Algorithm Library:<br>**
    - **...**
    - **your suggestions ... ( write me )<br>**
    
 - **extension of var:internal types:**                                                 
      - **var:map ( like:**  `x["item"]["something"]...` **)**                            
      - **var:storage: ( access to data through var:map )**
        - **var:storage <-> hdd/ssd**
        - **var:storage:client <-> var:storage:server (maybe)**
      - **...**
      - **your suggestions ... ( write me )**
      
**long-term:  ( to improve overall performance )**
 - **replace** `std::string` **with my own implementation**
 - **replace** `std::vector` **with my own implementation**
 - **...**

----
P.S. Sorry about my English :) 
