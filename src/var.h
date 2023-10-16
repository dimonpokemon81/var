/*
 * Copyright 2023 dimonpokemon81  mail: dimon19811@outlook.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***********************************************************************************
 * 
 *  major  bugs(patches)
 *   |    /
 *  v1.2.0  [at: 08.06.2022]
 *     |
 *  feature package
 *  
 *  Please help to improve the project:
 *  
 *  if it seems to you, or something is clearly not working as it should,
 *  please write me: as short example/description/other way.
 *  
 *                   email:  dimon19811@outlook.com               
 *                   GitHub: https://github.com/dimonpokemon81/var/issues
 *
 *  Actual version on GitHUb: https://github.com/dimonpokemon81/var
 *
 ***********************************************************************************  
 *   
 *  local typedef:
 *   
 *         const char         -> cch;
 *         std::string        -> str;
 *         long long          -> llong
 *         unsigned long long -> ullong
 *  
 *  local define:
 *  
 *         std::vector            -> __vct 
 *         std::initializer_list  -> __iniL 
 *  
 ***********************************************************************************
 *  
 * .-[N1] Supported types -> SUPT (cross-types) --------------------------------.
 * |                                                                            |
 * |   .- inner C++: -------------------------------------------------------.   | 
 * |   | bool                                                               |   |
 * |   | int                                                                |   |
 * |   | long                                                               |   |
 * |   | llong                                                              |   |
 * |   | ullong                                                             |   |
 * |   | char                                                               |   |
 * |   | float                                                              |   |
 * |   | double                                                             |   | 
 * |   | cch*                                                               |   |
 * |   | std::string                                                        |   |
 * |   | std::initializer_list<var>                                         |   |
 * |   '--------------------------------------------------------------------'   |
 * |   .- var:X [N2] -------------------------------------------------------.   |
 * |   |                                                                    |   |
 * |   | var:bool    -> envelope of   bool                                  |   |
 * |   | var:int     -> envelope of:  int,long,longlong,unsigned longlong   |   |
 * |   | var:double  -> envelope of:  double,float                          |   |
 * |   | var:char    -> envelope of   char                                  |   |
 * |   | var:str     -> envelope of   std::string                           |   |
 * |   | var:arr     -> envelope of   std::vector                           |   |
 * |   | var:map     -> my implementation                                   |   |
 * |   | var:P_map   -> my implementation                                   |   |
 * |   | var:I_str   -> envelope of   std::string::const_iterator           |   |
 * |   | var:Ir_str  -> envelope of   std::string::const_reverse_iterator   |   |
 * |   | var:I_arr   -> envelope of   std::vector<var>::const_iterator      |   |
 * |   | var:Ir_arr  -> envelope of   std::vector<var>::const_reverse_iterator  |
 * |   | or var:itr  -> as a generic representation of all iterators        |   |
 * |   | var:udf     -> intuitvely: - operation result                      |   |
 * |   |                            - bad use                               |   |
 * |   |                            - other ...                             |   |
 * |   '--------------------------------------------------------------------'   |
 * |                                                                            |
 * '----------------------------------------------------------------------------'
 *  
 ***********************************************************************************
 * 
 *  integer range (longlong) :
 *         
 *         -9223372036854775808 ... 9223372036854775807
 *         
 *  note: if result/value is out of the range ,then [ out_of_number_range ]
 *        exception will be thrown
 *  
 *  double range -> default behavior: longlong with double
 *  
 ***********************************************************************************
 *  
 *  local definitions:  
 *         
 *         SUPT      -> all supported types [N1]
 *  
 *         var:X     -> any of the [N2]
 *        
 *         var:self& -> current var object ('*this')
 *        
 * 
 ************************************************************************************
 * 
 *  notes:  - all changes regarding 'const var' -> closed as private, 
 *            other behavior will be noted 
 *          
 *          - [ use_of_undefined_var ]/[ unsupported_interface ]
 *            These exceptions serve as warnings about misuse of the 'type'.
 *            Adds strictness to don't fall into the 'abyss' of results check.
 *            That is, not worth to capture every 'var-variable' in the try/catch,
 *            just be careful        
 *            
 *  P.S. Sorry about my English :)            
 * 
 *************************************************************************************
 */
//
#ifndef _VAR_

#include <string>
#include <vector>
#include <iostream>  
#include <climits>
#include <initializer_list>

#define _VAR_           
#define _VAR_DBG_n        // inner use
#define _VAR_PRNT_EXC 1   // 1 -> print exceptions 
#ifdef _VAR_DBG     
#define _MAP_da_DBG
#define MAP_da_dbg
#endif

class var;
template<typename T> class ttyp;
typedef const char cch;
typedef std::string str;
typedef unsigned long long ullong;
typedef long long llong;
#define __vct std::vector
#define __iniL std::initializer_list

typedef std::string::iterator I_str;
typedef std::string::const_iterator Ic_str;

typedef std::string::reverse_iterator Ir_str;
typedef std::string::const_reverse_iterator Irc_str;

typedef std::vector<var>::iterator I_arr;
typedef std::vector<var>::const_iterator Ic_arr;

typedef std::vector<var>::reverse_iterator Ir_arr;
typedef std::vector<var>::const_reverse_iterator Irc_arr;

#define vm_deep 256
#define vm_field 64

template<typename t, int id_isz, int sli_isz, int sstg_isz, //
		int rstg_isz, int _vm_deep> class MAP;
#define _VMAP MAP<var, vm_field, 4, 10, 10, vm_deep>
typedef _VMAP VMAP;
class P_map;

class dbg {
	
	friend var;
	friend class ttyp<double> ;
	friend class ttyp<llong> ;
	friend class ttyp<bool> ;
	friend class ttyp<str> ;
	friend class ttyp<char> ;
	friend class ttyp<Ic_str> ;
	friend class ttyp<Irc_str> ;
	friend class ttyp<Ic_arr> ;
	friend class ttyp<Irc_arr> ;
	friend class ttyp<__vct<var>> ;
	friend class ttyp<VMAP> ;
	friend class ttyp<P_map> ;

private:

	cch *sval = 0;
	var *vval = 0;

public:

	dbg();
};

class var {
	
	friend class ttyp<double> ;
	friend class ttyp<llong> ;
	friend class ttyp<bool> ;
	friend class ttyp<str> ;
	friend class ttyp<char> ;
	friend class ttyp<Ic_str> ;
	friend class ttyp<Irc_str> ;
	friend class ttyp<Ic_arr> ;
	friend class ttyp<Irc_arr> ;
	friend class ttyp<__vct<var>> ;
	friend class ttyp<VMAP> ;
	friend class _VMAP;
	friend class ttyp<P_map>;

protected:

	var *pttyp = 0;
	cch *tid = 0;

#ifdef _VAR_DBG     
			dbg *pdbg = 0;
#endif
			
		private:

			template<typename T> var(T *x);
			template<typename t> var(const ttyp<t> &x);	 // [ inner use ]
			var(var *x);// [ inner use ]
			var(str *x);// [ inner use ]
			var(const __vct <var> &x);// [ inner use ]
			var& operator=(var *rv);// [ inner use ]
			var& operator=(var &rv) const;// not allowed [ const var ]
			var& operator=(const var &rv) const;// not allowed [ const var ]
			var& operator=(int rv) const;// not allowed [ const var ]
			var& operator=(float rv) const;// not allowed [ const var ]
			var& operator=(double rv) const;// not allowed [ const var ]
			var& operator=(long rv) const;// not allowed [ const var ]
			var& operator=(llong rv) const;// not allowed [ const var ]
			var& operator=(ullong rv) const;// not allowed [ const var ]
			var& operator=(char rv) const;// not allowed [ const var ]
			var& operator=(cch *rv) const;// not allowed [ const var ]
			var& operator=(const str &rv) const;// not allowed [ const var ]
			var& operator=(const __iniL <var> &rv) const;// not allowed [ const var ]
			var& operator=(const I_str &rv) const;// not allowed [ const var ]
			var& operator=(const Ic_str &rv) const;// not allowed [ const var ]
			var& operator=(const Ir_str &rv) const;// not allowed [ const var ]
			var& operator=(const Irc_str &rv) const;// not allowed [ const var ]
			var& operator=(const I_arr &rv) const;// not allowed [ const var ]
			var& operator=(const Ic_arr &rv) const;// not allowed [ const var ]
			var& operator=(const Ir_arr &rv) const;// not allowed [ const var ]
			var& operator=(const Irc_arr &rv) const;// not allowed [ const var ]
			
			var(const I_str &I, const ttyp<str> *bs);// [ inner use ]
			var(const Ic_str &I, const ttyp<str> *bs);// [ inner use ]
			var(const Ir_str &I, const ttyp<str> *bs);// [ inner use ]
			var(const Irc_str &I, const ttyp<str> *bs);// [ inner use ]
			
			var(const I_arr &I, const ttyp<__vct<var>> *bs);// [ inner use ]
			var(const Ic_arr &I, const ttyp<__vct<var>> *bs);// [ inner use ]
			var(const Ir_arr &I, const ttyp<__vct<var>> *bs);// [ inner use ]
			var(const Irc_arr &I, const ttyp<__vct<var>> *bs);// [ inner use ]
			
			virtual operator var*();// [ inner use ]
			virtual operator __vct<var>*();// [ inner use ]
			virtual operator str*() const;// [ inner use ]
			virtual operator Ic_str();// [ inner use ]
			virtual operator Ic_arr();// [ inner use ]
			virtual operator Ic_str*();// [ inner use ]
			virtual operator Ic_arr*();// [ inner use ]
			virtual operator Irc_str();// [ inner use ]
			virtual operator Irc_arr();// [ inner use ]
			virtual operator Irc_str*();// [ inner use ]
			virtual operator Irc_arr*();// [ inner use ]
			
			const var& operator +=(const var &rv) const;// not allowed [ const var ]
			const var& operator +=(int rv) const;// not allowed [ const var ]
			const var& operator +=(float rv) const;// not allowed [ const var ]
			const var& operator +=(double rv) const;// not allowed [ const var ]
			const var& operator +=(long rv) const;// not allowed [ const var ]
			const var& operator +=(llong rv) const;// not allowed [ const var ]
			const var& operator +=(ullong rv) const;// not allowed [ const var ]
			const var& operator +=(str &rv) const;// not allowed [ const var ]
			const var& operator +=(str *rv) const;// not allowed [ const var ]
			const var& operator +=(cch *rv) const;// not allowed [ const var ]
			const var& operator +=(char rv) const;// not allowed [ const var ]	
			const var& operator -=(const var &rv) const;// not allowed [ const var ]
			const var& operator -=(int rv) const;// not allowed [ const var ]
			const var& operator -=(float rv) const;// not allowed [ const var ]
			const var& operator -=(double rv) const;// not allowed [ const var ]
			const var& operator -=(long rv) const;// not allowed [ const var ]
			const var& operator -=(llong rv) const;// not allowed [ const var ]
			const var& operator -=(ullong rv) const;// not allowed [ const var ]
			const var& operator -=(str &rv) const;// not allowed [ const var ]
			const var& operator -=(str *rv) const;// not allowed [ const var ]
			const var& operator -=(cch *rv) const;// not allowed [ const var ]
			const var& operator -=(char rv) const;// not allowed [ const var ]
			const var& operator *=(const var &rv) const;// not allowed [ const var ]
			const var& operator *=(int rv) const;// not allowed [ const var ]
			const var& operator *=(float rv) const;// not allowed [ const var ]
			const var& operator *=(double rv) const;// not allowed [ const var ]
			const var& operator *=(long rv) const;// not allowed [ const var ]
			const var& operator *=(llong rv) const;// not allowed [ const var ]
			const var& operator *=(ullong rv) const;// not allowed [ const var ]
			const var& operator *=(str &rv) const;// not allowed [ const var ]
			const var& operator *=(str *rv) const;// not allowed [ const var ]
			const var& operator *=(cch *rv) const;// not allowed [ const var ]
			const var& operator *=(char rv) const;// not allowed [ const var ]
			const var& operator /=(const var &rv) const;// not allowed [ const var ]
			const var& operator /=(int rv) const;// not allowed [ const var ]
			const var& operator /=(float rv) const;// not allowed [ const var ]
			const var& operator /=(double rv) const;// not allowed [ const var ]
			const var& operator /=(long rv) const;// not allowed [ const var ]
			const var& operator /=(llong rv) const;// not allowed [ const var ]
			const var& operator /=(ullong rv) const;// not allowed [ const var ]
			const var& operator /=(str &rv) const;// not allowed [ const var ]
			const var& operator /=(str *rv) const;// not allowed [ const var ]
			const var& operator /=(cch *rv) const;// not allowed [ const var ]
			const var& operator /=(char rv) const;// not allowed [ const var ]		
			
			var& operator<<(const var &src) const;// not allowed [ const var ];
			void reserve(ullong new_cap) const;// not allowed [ const var ]
			void shrink_to_fit() const;// not allowed [ const var ]
			void clear() const;// not allowed [ const var ]
			void swap(var &X) const;// not allowed [ const var ]
			
			void resize(ullong count) const;// not allowed [ const var ]
			void resize(ullong count, const var &x) const;// not allowed [ const var ]
			void resize(ullong count, int X) const;// not allowed [ const var ]
			void resize(ullong count, float X) const;// not allowed [ const var ]
			void resize(ullong count, double X) const;// not allowed [ const var ]
			void resize(ullong count, long X) const;// not allowed [ const var ]
			void resize(ullong count, llong X) const;// not allowed [ const var ]
			void resize(ullong count, ullong X) const;// not allowed [ const var ]
			void resize(ullong count, char X) const;// not allowed [ const var ]
			void resize(ullong count, cch *X) const;// not allowed [ const var ]
			void resize(ullong count, const str &X) const;// not allowed [ const var ]
			void resize(ullong count, __iniL<var> X) const;// not allowed [ const var ]	
			
			var insert(ullong index, const var &src_fist, const var &src_last) const;// not allowed [ const var ]
			var insert(ullong index, const var &X) const;// not allowed [ const var ]
			var insert(ullong index, int X) const;// not allowed [ const var ]
			var insert(ullong index, float X) const;// not allowed [ const var ]
			var insert(ullong index, double X) const;// not allowed [ const var ]
			var insert(ullong index, long X) const;// not allowed [ const var ]
			var insert(ullong index, llong X) const;// not allowed [ const var ]
			var insert(ullong index, ullong X) const;// not allowed [ const var ]
			var insert(ullong index, char X) const;// not allowed [ const var ]
			var insert(ullong index, cch *X) const;// not allowed [ const var ]
			var insert(ullong index, const str &X) const;// not allowed [ const var ]
			var insert(ullong index, __iniL<var> X) const;// not allowed [ const var ]
			var insert(ullong index, ullong count, const var &X) const;// not allowed [ const var ]
			var insert(ullong index, ullong count, int X) const;// not allowed [ const var ]
			var insert(ullong index, ullong count, float X) const;// not allowed [ const var ]
			var insert(ullong index, ullong count, double X) const;// not allowed [ const var ]
			var insert(ullong index, ullong count, long X) const;// not allowed [ const var ]
			var insert(ullong index, ullong count, llong X) const;// not allowed [ const var ]
			var insert(ullong index, ullong count, ullong X) const;// not allowed [ const var ]
			var insert(ullong index, ullong count, char X) const;// not allowed [ const var ]
			var insert(ullong index, ullong count, cch *X) const;// not allowed [ const var ]
			var insert(ullong index, ullong count, const str &X) const;// not allowed [ const var ]
			var insert(ullong index, ullong count, __iniL<var> X) const;// not allowed [ const var ]		
			var insert(ullong index, cch *src, ullong src_index, ullong src_count) const;// not allowed [ const var ]
			
			var erase(ullong index) const;// not allowed [ const var ]
			var erase(ullong first, ullong last) const;// not allowed [ const var ]
			
			var push_back(const var &X) const;// not allowed [ const var ]
			var push_back(int X) const;// not allowed [ const var ]
			var push_back(float X) const;// not allowed [ const var ]
			var push_back(double X) const;// not allowed [ const var ]
			var push_back(long X) const;// not allowed [ const var ]
			var push_back(llong X) const;// not allowed [ const var ]
			var push_back(ullong X) const;// not allowed [ const var ]
			var push_back(char X) const;// not allowed [ const var ]
			var push_back(cch *X) const;// not allowed [ const var ]
			var push_back(const str &x) const;// not allowed [ const var ]
			var push_back( __iniL<var> X) const;// not allowed [ const var ]
			
			var push_front(const var &X) const;// not allowed [ const var ]
			var push_front(int X) const;// not allowed [ const var ]
			var push_front(float X) const;// not allowed [ const var ]
			var push_front(double X) const;// not allowed [ const var ]
			var push_front(long X) const;// not allowed [ const var ]
			var push_front(llong X) const;// not allowed [ const var ]
			var push_front(ullong X) const;// not allowed [ const var ]
			var push_front(char X) const;// not allowed [ const var ]
			var push_front(cch *X) const;// not allowed [ const var ]
			var push_front(const str &X) const;// not allowed [ const var ]
			var push_front( __iniL<var> X) const;// not allowed [ const var ]
			
			var pop_back() const;// not allowed [ const var ]
			var pop_front() const;// not allowed [ const var ]
			
			var& ssort(bool (*cmp)(var &a, var &b), bool reverse = false) const;// not allowed [ const var ]
			var& ssort(bool reverse = false) const;// not allowed [ const var ]
			var& reverse() const;// not allowed [ const var ]
			
			var& fill(const var &X, ullong index = 0, ullong count = 0) const;// not allowed [ const var ]
			var& fill(int X, ullong index = 0, ullong count = 0) const;// not allowed [ const var ]
			var& fill(float X, ullong index = 0, ullong count = 0) const;// not allowed [ const var ]
			var& fill(double X, ullong index = 0, ullong count = 0) const;// not allowed [ const var ]
			var& fill(long X, ullong index = 0, ullong count = 0) const;// not allowed [ const var ]
			var& fill(llong X, ullong index = 0, ullong count = 0) const;// not allowed [ const var ]
			var& fill(ullong X, ullong index = 0, ullong count = 0) const;// not allowed [ const var ]
			var& fill(char X, ullong index = 0, ullong count = 0) const;// not allowed [ const var ]
			var& fill(cch *X, ullong index = 0, ullong count = 0) const;// not allowed [ const var ]
			var& fill(const str &X, ullong index = 0, ullong count = 0) const;// not allowed [ const var ]
			var& fill(__iniL<var> X, ullong index = 0, ullong count = 0) const;// not allowed [ const var ]
			
			var& initArr() const;// not allowed [ const var ]
			var& sjoin(char sep = 0) const;// not allowed [ const var ]
			var& ssplit(char sep) const;// not allowed [ const var ]
			var& ltrim() const;// not allowed [ const var ]
			var& rtrim() const;// not allowed [ const var ]
			var& trim() const;// not allowed [ const var ]
			
			void map_init() const;// not allowed [ const var ]
			var& map_set(cch *map) const;// not allowed [ const var ]
			var map_rem(cch *map) const;// not allowed [ const var ]
			var& map_free() const;// not allowed [ const var ]
			var map_fread(cch *fname) const;// not allowed [ const var ]
			
			var& append(char ch, ullong count) const;// not allowed [ const var ]
			var& append(const var &src, ullong src_index = 0, ullong src_count = 0) const;// not allowed [ const var ]
			var& append(cch *src, ullong src_index = 0, ullong src_count = 0) const;// not allowed [ const var ]
			var& append(str &src, ullong src_index = 0, ullong src_count = 0) const;// not allowed [ const var ]
			var& append(const var &src_first, const var &src_last) const;// not allowed [ const var ] 
			
			var& replace(ullong index, ullong count, const var &src, ullong src_index = 0, ullong src_count = 0) const;// not allowed [ const var ] 
			var& replace(ullong index, ullong count, cch *src, ullong src_index = 0, ullong src_count = 0) const;// not allowed [ const var ] 
			var& replace(ullong index, ullong count, str &src, ullong src_index = 0, ullong src_count = 0) const;// not allowed [ const var ] 
			var& replace(ullong index, ullong count, const var &src_first, const var &src_last) const;// not allowed [ const var ] 
			
			var substr(ullong index, ullong count = 0) const;// not allowed [ const var ] 
			
		public:

			var();
			var(bool x);
			var(int x);
			var(long);
			var(llong);
			var(ullong);
			var(float);
			var(double);
			var(str&);
			var(const str&);
			var(cch*);
			var(char*);
			var(char);
			var(__iniL<var> x);
			var(var &x);
			var(const var &x);
			var(var &&x);
			virtual ~var();

			var& operator=(const var &rv);
			var& operator=(var &rv);
			var& operator=(var &&rv);
			var& operator=(int rv);
			var& operator=(float rv);
			var& operator=(double rv);
			var& operator=(long rv);
			var& operator=(llong rv);
			var& operator=(ullong rv);
			var& operator=(bool rv);
			var& operator=(char rv);
			var& operator=(cch *rv);
			var& operator=(const str &rv);
			var& operator=(const __iniL <var> &rv);

			enum class map {
				nxt,skp,brk
			};

			template<typename t> friend var operator +(const t &lv, const var &rv);
			virtual var operator+(const var &rv) const;
			virtual var operator+(int rv) const;
			virtual var operator+(float rv) const;
			virtual var operator+(double rv) const;
			virtual var operator+(long rv) const;
			virtual var operator+(llong rv) const;
			virtual var operator+(ullong rv) const;
			virtual var operator+(cch *rv) const;
			virtual var operator+(str &rv) const;
			virtual var operator+(str *rv) const;
			virtual var operator+(char rv) const;

			template<typename t> friend var operator-(const t &lv, const var &rv);
			virtual var operator-(const var &rv) const;
			virtual var operator-(int rv) const;
			virtual var operator-(float rv) const;
			virtual var operator-(double rv) const;
			virtual var operator-(long rv) const;
			virtual var operator-(llong rv) const;
			virtual var operator-(ullong rv) const;

			template<typename t> friend var operator*(const t &lv, const var &rv);
			virtual var operator*(const var &rv) const;
			virtual var operator*(int rv) const;
			virtual var operator*(float rv) const;
			virtual var operator*(double rv) const;
			virtual var operator*(long rv) const;
			virtual var operator*(llong rv) const;
			virtual var operator*(ullong rv) const;

			template<typename t> friend var operator/(const t &lv, const var &rv);
			virtual var operator/(const var &rv) const;
			virtual var operator/(int rv) const;
			virtual var operator/(float rv) const;
			virtual var operator/(double rv) const;
			virtual var operator/(long rv) const;
			virtual var operator/(llong rv) const;
			virtual var operator/(ullong rv) const;

			template<typename t> friend t& operator+=(t &lv, const var &rv);
			template<typename t> friend t& operator+=(const t &lv, const var &rv);
			virtual var& operator+=(const var &rv);
			virtual var& operator+=(int rv);
			virtual var& operator+=(float rv);
			virtual var& operator+=(double rv);
			virtual var& operator+=(long rv);
			virtual var& operator+=(llong rv);
			virtual var& operator+=(ullong rv);
			virtual var& operator+=(str &rv);
			virtual var& operator+=(str *rv);
			virtual var& operator+=(cch *rv);
			virtual var& operator+=(char rv);

			template<typename t> friend t& operator-=(t &lv, const var &rv);
			template<typename t> friend t& operator-=(const t &lv, const var &rv);
			virtual var& operator-=(const var &rv);
			virtual var& operator-=(int rv);
			virtual var& operator-=(float rv);
			virtual var& operator-=(double rv);
			virtual var& operator-=(long rv);
			virtual var& operator-=(llong rv);
			virtual var& operator-=(ullong rv);

			template<typename t> friend t& operator*=(t &lv, const var &rv);
			template<typename t> friend t& operator*=(const t &lv, const var &rv);
			virtual var& operator*=(const var &rv);
			virtual var& operator*=(int rv);
			virtual var& operator*=(float rv);
			virtual var& operator*=(double rv);
			virtual var& operator*=(long rv);
			virtual var& operator*=(llong rv);
			virtual var& operator*=(ullong rv);

			template<typename t> friend t& operator/=(t &lv, const var &rv);
			template<typename t> friend t& operator/=(const t &lv, const var &rv);
			virtual var& operator/=(const var &rv);
			virtual var& operator/=(int rv);
			virtual var& operator/=(float rv);
			virtual var& operator/=(double rv);
			virtual var& operator/=(long rv);
			virtual var& operator/=(llong rv);
			virtual var& operator/=(ullong rv);

			virtual var& operator[](ullong index);
			const virtual var& operator[](ullong index) const;

			virtual var operator++(int rv);
			const virtual var operator++(int rv) const;

			virtual var& operator++();
			const virtual var& operator++() const;

			virtual var operator--(int rv);
			const virtual var operator--(int rv) const;

			virtual var& operator--();
			const virtual var& operator--() const;

			friend std::ostream& operator<<(std::ostream &os, const var &rv);
			friend std::istream& operator>>(std::istream &is, const var &rv);

			template<typename t> friend bool operator>(const t &lv, const var &rv);
			virtual bool operator>(const var &rv) const;
			virtual bool operator>(int rv) const;
			virtual bool operator>(float rv) const;
			virtual bool operator>(double rv) const;
			virtual bool operator>(long rv) const;
			virtual bool operator>(llong rv) const;
			virtual bool operator>(ullong rv) const;
			virtual bool operator>(cch *rv) const;
			virtual bool operator>(str &rv) const;
			virtual bool operator>(str *rv) const;
			virtual bool operator>(char rv) const;

			template<typename t> friend bool operator<(const t &lv, const var &rv);
			virtual bool operator<(const var &rv) const;
			virtual bool operator<(int rv) const;
			virtual bool operator<(float rv) const;
			virtual bool operator<(double rv) const;
			virtual bool operator<(long rv) const;
			virtual bool operator<(llong rv) const;
			virtual bool operator<(ullong rv) const;
			virtual bool operator<(cch *rv) const;
			virtual bool operator<(str &rv) const;
			virtual bool operator<(str *rv) const;
			virtual bool operator<(char rv) const;

			template<typename t> friend bool operator >=(const t &lv, const var &rv);
			virtual bool operator>=(const var &rv) const;
			virtual bool operator>=(int rv) const;
			virtual bool operator>=(float rv) const;
			virtual bool operator>=(double rv) const;
			virtual bool operator>=(long rv) const;
			virtual bool operator>=(llong rv) const;
			virtual bool operator>=(ullong rv) const;
			virtual bool operator>=(cch *rv) const;
			virtual bool operator>=(str &rv) const;
			virtual bool operator>=(str *rv) const;
			virtual bool operator>=(char rv) const;

			template<typename t> friend bool operator<=(const t &lv, const var &rv);
			virtual bool operator<=(const var &rv) const;
			virtual bool operator<=(int rv) const;
			virtual bool operator<=(float rv) const;
			virtual bool operator<=(double rv) const;
			virtual bool operator<=(long rv) const;
			virtual bool operator<=(llong rv) const;
			virtual bool operator<=(ullong rv) const;
			virtual bool operator<=(cch *rv) const;
			virtual bool operator<=(str &rv) const;
			virtual bool operator<=(str *rv) const;
			virtual bool operator<=(char rv) const;

			virtual bool operator!() const;

			template<typename t> friend bool operator==(const t &lv, const var &rv);
			virtual bool operator==(const var &rv) const;
			virtual bool operator==(int rv) const;
			virtual bool operator==(float rv) const;
			virtual bool operator==(double rv) const;
			virtual bool operator==(long rv) const;
			virtual bool operator==(llong rv) const;
			virtual bool operator==(ullong rv) const;
			virtual bool operator==(cch *rv) const;
			virtual bool operator==(str &rv) const;
			virtual bool operator==(str *rv) const;
			virtual bool operator==(char rv) const;

			template<typename t> friend bool operator!=(const t &lv, const var &rv);
			virtual bool operator!=(const var &rv) const;
			virtual bool operator!=(int rv) const;
			virtual bool operator!=(float rv) const;
			virtual bool operator!=(double rv) const;
			virtual bool operator!=(long rv) const;
			virtual bool operator!=(llong rv) const;
			virtual bool operator!=(ullong rv) const;
			virtual bool operator!=(cch *rv) const;
			virtual bool operator!=(str &rv) const;
			virtual bool operator!=(str *rv) const;
			virtual bool operator!=(char rv) const;

			virtual var& operator*();
			const virtual var& operator*() const;

			virtual operator bool() const noexcept;
			virtual operator bool() noexcept;
			virtual operator int() const;
			virtual operator int();
			virtual operator float() const;
			virtual operator float();
			virtual operator double() const;
			virtual operator double();
			virtual operator long() const;
			virtual operator long();
			virtual operator llong() const;
			virtual operator llong();
			virtual operator ullong() const;
			virtual operator ullong();
			virtual operator str() const;
			virtual operator str();
			virtual operator char() const;
			virtual operator char();

			//------------------------------------------------------------ IF:
			
			//-------------------------------- Common if: --------------------
			
			/** ->  int size() [var:X]  <-
			 * 
			 * return size of the underlying type (sizeof)   
			 *
			 */
			virtual int size() const noexcept;

			/** ->  cch* gtid() [var:X]  <-
			 * 
			 * return cch*:  "arr"    -> array  
			 *               "str"    -> string
			 *               "int"    -> local-int (llong)
			 *               "double" -> double
			 *               "bool"   -> bool
			 *               "char"   -> char 
			 *          
			 */
			cch* gtid() const noexcept;

			/** ->  bool is_def() [var:X]  <-
			 * 
			 * return -> true if an asking type is not undefined (var:udf),
			 *   else -> false
			 *  
			 */
			bool is_def() const noexcept;

			/** ->  bool is_bool() [var:X]  <-
			 * 
			 * return -> true if an asking type is equal to the current type (var:bool),
			 *   else -> false
			 *  
			 */
			bool is_bool() const noexcept;

			/** ->  bool is_int() [var:X]  <-
			 * 
			 * return -> true if an asking type is equal to the current type (var:int),
			 *   else -> false
			 *  
			 */
			bool is_int() const noexcept;

			/** ->  bool is_double() [var:X]  <-
			 * 
			 * return -> true if an asking type is equal to the current type (var:double),
			 *   else -> false
			 *  
			 */
			bool is_double() const noexcept;

			/** ->  bool is_char() [var:X]  <-
			 * 
			 * return -> true if an asking type is equal to the current type (var:char),
			 *   else -> false
			 *  
			 */
			bool is_char() const noexcept;

			/** ->  bool is_str() [var:X]  <-
			 * 
			 * return -> true if an asking type is equal to the current type (var:str),
			 *   else -> false
			 *  
			 */
			bool is_str() const noexcept;

			/** ->  bool is_arr() [var:X]  <-
			 * 
			 * return -> true if an asking type is equal to the current type (var:arr),
			 *   else -> false
			 *  
			 */
			bool is_arr() const noexcept;

			/** ->  bool is_map() [var:X]  <-
			 * 
			 * return -> true if an asking type is equal to the current type (var:map),
			 *   else -> false
			 *  
			 */
			bool is_map() const noexcept;

			/** ->  bool is_P_map() [var:X]  <-
			 * 
			 * return -> true if an asking type is equal to the current type (var:P_map),
			 *   else -> false
			 *  
			 */
			bool is_P_map() const noexcept;

			/** ->  bool is_itr() [var:X]  <-
			 * 
			 * return -> true if an asking type is the iteratotor: I_str,Ir_str,I_arr,Ir_arr
			 *   else -> false
			 *  
			 */
			bool is_itr() const noexcept;

			/** ->  bool is_I_str() [var:X]  <-
			 * 
			 * return -> true if an asking type is equal to the current type (var:I_str),
			 *   else -> false
			 *  
			 */
			bool is_I_str() const noexcept;

			/** ->  bool is_Ir_str() [var:X]  <-
			 * 
			 * return -> true if an asking type is equal to the current type (var:Ir_str),
			 *   else -> false
			 *  
			 */
			bool is_Ir_str() const noexcept;

			/** ->  bool is_I_arr() [var:X]  <-
			 * 
			 * return -> true if an asking type is equal to the current type (var:I_arr),
			 *   else -> false
			 *  
			 */
			bool is_I_arr() const noexcept;

			/** ->  bool is_Ir_arr() [var:X]  <-
			 * 
			 * return -> true if an asking type is equal to the current type (var:Ir_arr),
			 *   else -> false
			 *  
			 */
			bool is_Ir_arr() const noexcept;

			/** ->  void prnt() [var:X]  <-
			 *  
			 * - print current type value to the console     
			 *  
			 * ex:
			 *  var a = { 1, 2, 3 }, i = 123;
			 *  a.prnt();            // "{1,2,3}"
			 *  i.prnt();            // "123"                           
			 *  var("Hello").prnt(); // "Hello"
			 */
			virtual void prnt() const noexcept;

			//------------------------------- str/arr shared: ----------------
			
			// Element access:
			
			/** ->  var:X& at(ullong index) [var:arr/var:str]  <-
			 *  
			 * - get element at 'index'
			 *  
			 * param index -> index of the element to return
			 * return      -> var:X& (for var:arr) or var:char& (for var:str) 
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ use_of_undefined_var ] 
			 *             > [ unsupported_interface ]          
			 * ex:
			 *  var s = "string", c;                        
			 *  c = s.at(2);             // 'r'
			 *  s.at(1) = 'T';           // "sTring"
			 *  var a = { "str", 123, 'c', { 'z', 321 }, var() }, x;
			 *	x = a.at(3);             // "{'z',321}"                                    
			 *  x.at(1) = 'X';           // "{'z','X'}" 
			 */
			virtual var& at(ullong index);

			/** ->  var:X& at(ullong index) [var:arr/var:str]  <-
			 *  
			 * - get element at 'index'
			 *  
			 * param index -> index of the element to return
			 * return      -> var:X& (for var:arr) or var:char& (for var:str) 
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ use_of_undefined_var ] 
			 *             > [ unsupported_interface ]
			 *             
			 * ex: Look -> var& at(ullong index)
			 *  
			 */
			const virtual var& at(ullong index) const;

			/** ->  var:X& front() [var:str/var:arr]  <-
			 * 
			 * - get element at index [0]
			 * 
			 * return -> var:X& (for var:arr) or var:char& (for var:str)  
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ use_of_undefined_var ] 
			 *             > [ unsupported_interface ]      
			 * ex:
			 *  var s = "string", c;                        
			 *  c = s.front();                // 's'
			 *  var a = { "str", 123, 'c' }, x;
			 *	x = a.front();                // "str"                                    
			 * 
			 */
			virtual var& front();

			/** ->  var:X& front() [var:str/var:arr]  <-
			 * 
			 * - get element at index [0]
			 * 
			 * return -> var:X& (for var:arr) or var:char& (for var:str)   
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ use_of_undefined_var ] 
			 *             > [ unsupported_interface ]
			 *             
			 * ex: Look -> var& front()                                
			 * 
			 */
			const virtual var& front() const;

			/** ->  var:X& back() [var:str/var:arr]  <-
			 * 
			 * - get element at index [length()-1]
			 * 
			 * return -> var:X& (for var:arr) or var:char& (for var:str)   
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ use_of_undefined_var ] 
			 *             > [ unsupported_interface ]          
			 * ex:
			 *  var s = "string", c;                        
			 *  c = s.back();                // 'g'
			 *  var a = { "str", 123, 'c' }, x;
			 *	x = a.back();                // 'c' 
			 * 
			 */
			virtual var& back();

			/** ->  var:X& back() [var:str/var:arr]  <-
			 * 
			 * - get element at index [length()-1]
			 * 
			 * return -> var:X& (for var:arr) or var:char& (for var:str)   
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ use_of_undefined_var ] 
			 *             > [ unsupported_interface ]
			 *             
			 * ex: Look -> var& back()
			 * 
			 */
			const virtual var& back() const;

			// Iterators:
			
			//forward:
			/** ->  var:itr begin() [var:str/var:arr]  <-
			 * 
			 * - returns an iterator to the beginning of the given var:str/var:arr
			 *   
			 *   begin
			 *     V
			 *     X X X X X .
			 *               ^ end (past-the-last element)
			 *             
			 * return -> var:itr
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]   
			 * ex:
			 *  [begin]--->[end]    
			 *  
			 *  var s = "string", sbeg = s.begin(), send = s.end();
			 *	while (sbeg != send) {
			 *		(*sbeg).prnt();    // items in turn
			 *		sbeg++;
			 *	}
			 *	var a = { "str", 123, 12.5, 'c', { 1, 2, 3 } };
			 *	var abeg = a.begin(), aend = a.end();
			 *	while (abeg != aend) {
			 *		(*abeg).prnt();    // items in turn
			 *		abeg++;
			 *	}                    
			 *        
			 */
			virtual var begin();

			/** ->  var:itr begin() [var:str/var:arr]  <-
			 * 
			 * - returns an iterator to the beginning of the given var:str/var:arr
			 * 
			 *   begin
			 *     V
			 *     X X X X X .
			 *               ^ end (past-the-last element)
			 * 
			 * return -> var:itr
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]   
			 *  
			 * ex: Look -> var begin()         
			 *        
			 */
			const virtual var begin() const;

			/** ->  var:itr end() [var:str/var:arr]  <-
			 * 
			 * - returns an iterator to the end (i.e. the element after the last element) 
			 *   of the given var:str/var:arr
			 *   
			 *   begin
			 *     V
			 *     X X X X X .
			 *               ^ end (past-the-last element)
			 * 
			 * return -> var:itr
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]   
			 *  
			 * ex: Look -> var begin()         
			 *        
			 */
			virtual var end();

			/** ->  var:itr end() [var:str/var:arr]  <-
			 * 
			 * - returns an iterator to the end (i.e. the element after the last element) 
			 *   of the given var:str/var:arr
			 * 
			 *   begin
			 *     V
			 *     X X X X X .
			 *               ^ end (past-the-last element)
			 * 
			 * return -> var:itr
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]   
			 *  
			 * ex: Look -> var begin()         
			 *        
			 */
			const virtual var end() const;

			//revers:
			/** ->  var:itr rbegin() [var:str/var:arr]  <-
			 * 
			 * - returns an iterator to the reverse-beginning of the given var:str/var:arr
			 * 
			 *   rend (past-the-last element)
			 *     V
			 *     . X X X X X
			 *               ^ rbegin 
			 * 
			 * return -> var:itr
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]   
			 *       
			 * ex:     
			 *  [end]<---[begin]  
			 *     
			 *  var s = "string", sbeg = s.rbegin(), send = s.rend();
			 *	while (sbeg != send) {
			 *		(*sbeg).prnt();      // items in turn
			 *		sbeg++;
			 *	}
			 *	var a = { "str", 123, 12.5, 'c', { 1, 2, 3 } };
			 *	var abeg = a.rbegin(), aend = a.rend();
			 *	while (abeg != aend) {
			 *		(*abeg).prnt();      // items in turn
			 *		abeg++;
			 *	}                    
			 *        
			 */
			virtual var rbegin();

			/** ->  var:itr rbegin() [var:str/var:arr]  <-
			 * 
			 * - returns an iterator to the reverse-beginning of the given var:str/var:arr
			 * 
			 *   rend (past-the-last element)
			 *     V
			 *     . X X X X X
			 *               ^ rbegin 
			 *               
			 * return -> var:itr
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]   
			 *  
			 * ex: Look -> var rbegin()         
			 *        
			 */
			const virtual var rbegin() const;

			/** ->  var:itr rend() [var:str/var:arr]  <-
			 * 
			 * - returns an iterator to the reverse-end of the given var:str/var:arr
			 * 
			 *   rend (past-the-last element)
			 *     V
			 *     . X X X X X
			 *               ^ rbegin 
			 * 
			 * return -> var:itr
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]   
			 *  
			 * ex: Look -> var rbegin()         
			 *        
			 */
			virtual var rend();

			/** ->  var:itr rend() [var:str/var:arr]  <-
			 * 
			 * - returns an iterator to the reverse-end of the given var:str/var:arr
			 * 
			 *   rend (past-the-last element)
			 *     V
			 *     . X X X X X
			 *               ^ rbegin 
			 * 
			 * return -> var:itr
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]   
			 *  
			 * ex: Look -> var rbegin()         
			 *        
			 */
			const virtual var rend() const;

			/** ->  bool next() [var:itr (var:str/var:arr)]  <-
			 * 
			 * - 'just convenient way of iterations'
			 * 
			 * return -> true  -> ok-next
			 *           false -> end
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]                   
			 * ex:
			 *  var s = "string";
			 *	var sbeg = s.begin();   // or sbeg.rbegin() -> for back iteration
			 *	do {
			 *		(*sbeg).prnt();     // items in turn
			 *	} while (sbeg.next());
			 *	var a = { "str", 123, 12.5, 'c', { 1, 2, 3 } };
			 *	var abeg = a.begin();   // or abeg.rbegin() -> for back iteration
			 *	do {
			 *		(*abeg).prnt();     // items in turn
			 *	} while (abeg.next());
			 * 
			 */
			virtual bool next() const;

			/** ->  llong pos() [var:itr (var:str/var:arr)]  <-
			 * 
			 *                                      [0]->[5] (forward)
			 * return -> current iterator position:  xxxxxx
			 *                                      [5]<-[0] (revers)
			 *                                         
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]                       
			 * ex:
			 *         (or array)
			 *  var s = "string", sbeg = s.begin() + 2, send = s.end() - 2;
			 *  llong pos;
			 *  pos = sbeg.pos();  // 2
			 *  pos = send.pos();  // 4
			 * 
			 */
			virtual llong pos() const;

			// Capacity:
			
			/** ->  bool empty() [var:str/var:arr]  <-
			 *  
			 * return -> true if var:str or var:arr are initialized but empty
			 *   else -> false
			 *    
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ] 
			 *           
			 */
			virtual bool empty() const;

			/** ->  ullong length() [var:str/var:arr]  <-
			 *  
			 * return -> var:str -> total chars
			 *           var:arr -> total elements
			 *    
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]     
			 *  
			 */
			virtual ullong length() const;

			/** ->  ullong max_size() [var:str/var:arr]  <-
			 * 
			 * return -> the maximum number of elements the var:str/var:arr is able to hold 
			 *           due to system or library implementation limitations
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]  
			 *  
			 */
			virtual ullong max_size() const;

			/** ->  void reserve(ullong new_cap) [var:str/var:arr]  <-
			 * 
			 * - increase the capacity of the var:str/var:arr to a value that's greater or equal
			 *   to 'new_cap'. if new_cap is greater than the current capacity(), new storage 
			 *   is allocated, otherwise the method does nothing
			 * - reserve() does not change the size of the var:str/var:arr
			 * - iterators may be invalidated
			 * 
			 * param new_cap -> new capacity
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *             > [ capacity_error ] 
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]              
			 *             
			 */
			virtual void reserve(ullong new_cap);

			/** ->  ullong capacity() [var:str/var:arr]  <-
			 * 
			 * return -> returns the number of elements that the
			 *           container has currently allocated space for
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ]  
			 *  
			 */
			virtual ullong capacity() const;

			/** ->  void shrink_to_fit() [var:str/var:arr]  <-
			 *
			 * - requests the removal of unused capacity 
			 * - iterators may be invalidated
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ] 
			 *  
			 */
			virtual void shrink_to_fit();

			// Operations/Modifiers:
			
			/** ->  void clear() [var:str/var:arr]  <-
			 *
			 * - erases all elements from the container
			 * - iterators may be invalidated
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *             > [ use_of_undefined_var ]   
			 *             > [ unsupported_interface ] 
			 *  
			 */
			virtual void clear();

			/** ->  void swap(var:arr/str &with) [var:str/var:arr]  <-
			 *
			 * - exchanges the contents of the 'this' with 'with' , does not invoke any 
			 *   move, copy, or swap operations on individual elements
			 * - iterators may be invalidated
			 * 
			 * param with -> swap 'with' (var:arr/var:str only)        
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ unsupported_type ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex:
			 *  var a1 = { 1, 2, 3 }, a2 = { 'a', 'b', 'c' }, 
			 *      s1 = "string", s2 = "string2";
			 *  a1.swap(a2);     // "{'a','b','c'}"
			 *  s1.swap(s2);     // "string2"
			 *  
			 */
			virtual void swap(var &with);

			/** ->  void resize(ullong count) [var:str/var:arr]  <-
			 *
			 * - resizes the container to contain 'count' elements 
			 * - in extension case var:udf for var:arr or spaces for var:str will be appended
			 * - in compress case all elements above the 'count' will be discarded
			 * - iterators may be invalidated
			 * 
			 * param count -> new size           
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]           
			 * ex:
			 *  var a = { 1, 2, 3 }, s = "string";
			 *  a.resize(5);     // "{1,2,3,udf,udf}"
			 *  a.resize(2);     // "{1,2}"
			 *  s.resize(8);     // "string  "
			 *  s.resize(3);     // "str"
			 *  
			 */
			virtual void resize(ullong count);

			/** ->  void resize(ullong count, const var &X) [var:str/var:arr]  <-
			 *
			 * - resizes the container to contain 'count' elements 
			 * - in extension case 'X' element(s) will be appended      
			 * - in compress case all elements above the 'count' will be discarded
			 * - iterators may be invalidated
			 * 
			 * param count -> new size
			 * param X     -> the value to initialize the new elements with
			 *                for: var:arr -> var:X
			 *                     var:str -> char or var:char (only)  
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ unsupported_type ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 * ex:    
			 *  var a = { 1 }, c = 'X', _a = { 1, 2 }, s = "s"; 
			 *  a.resize(3, _a);   // "{1,{1,2},{1,2}}"
			 *  s.resize(4, c);    // "sXXX"
			 *  
			 */
			virtual void resize(ullong count, const var &X);

			/** ->  void resize(ullong count, int X) [var:arr]  <-
			 *
			 * - resizes the container to contain 'count' elements 
			 * - in extension case 'X' element(s) will be appended      
			 * - in compress case all elements above the 'count' will be discarded
			 * - iterators may be invalidated
			 * 
			 * param count -> new size
			 * param X     -> the value to initialize the new elements with
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 * ex:
			 *  var a = { 1 };
			 *  a.resize(3, 3);  // "{1,3,3}"
			 *  
			 */
			virtual void resize(ullong count, int X);

			/** ->  void resize(ullong count, float X) [var:arr]  <-
			 *
			 * - resizes the container to contain 'count' elements 
			 * - in extension case 'X' element(s) will be appended      
			 * - in compress case all elements above the 'count' will be discarded
			 * - iterators may be invalidated
			 * 
			 * param count -> new size
			 * param X     -> the value to initialize the new elements with
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 * ex:
			 *  var a = { 1 };
			 *  a.resize(3, (float) 3.3);  // "{1,3.300000,3.300000}"
			 *  
			 */
			virtual void resize(ullong count, float X);

			/** ->  void resize(ullong count, double X) [var:arr]  <-
			 *
			 * - resizes the container to contain 'count' elements 
			 * - in extension case 'X' element(s) will be appended      
			 * - in compress case all elements above the 'count' will be discarded
			 * - iterators may be invalidated
			 * 
			 * param count -> new size
			 * param X     -> the value to initialize the new elements with
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 * ex:
			 *  var a = { 1 };
			 *  a.resize(3, 3.3);  // "{1,3.300000,3.300000}"
			 *  
			 */
			virtual void resize(ullong count, double X);

			/** ->  void resize(ullong count, long X) [var:arr]  <-
			 *
			 * - resizes the container to contain 'count' elements 
			 * - in extension case 'X' element(s) will be appended      
			 * - in compress case all elements above the 'count' will be discarded
			 * - iterators may be invalidated
			 * 
			 * param count -> new size
			 * param X     -> the value to initialize the new elements with
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 * ex:
			 *  var a = { 1 };
			 *  a.resize(3, (long) 3);  // "{1,3,3}"
			 *  
			 */
			virtual void resize(ullong count, long X);

			/** ->  void resize(ullong count, llong X) [var:arr]  <-
			 *
			 * - resizes the container to contain 'count' elements 
			 * - in extension case 'X' element(s) will be appended      
			 * - in compress case all elements above the 'count' will be discarded
			 * - iterators may be invalidated
			 * 
			 * param count -> new size
			 * param X     -> the value to initialize the new elements with
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 * ex:
			 *  var a = { 1 };
			 *  a.resize(3, (llong) 3);  // "{1,3,3}"
			 *  
			 */
			virtual void resize(ullong count, llong X);

			/** ->  void resize(ullong count, ullong X) [var:arr]  <-
			 *
			 * - resizes the container to contain 'count' elements 
			 * - in extension case 'X' element(s) will be appended      
			 * - in compress case all elements above the 'count' will be discarded
			 * - iterators may be invalidated
			 * 
			 * param count -> new size
			 * param X     -> the value to initialize the new elements with
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ out_of_number_range ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 * ex:
			 *  var a = { 1 };
			 *  a.resize(3, (ullong) 3);  // "{1,3,3}"
			 *  
			 */
			virtual void resize(ullong count, ullong X);

			/** ->  void resize(ullong count, char X) [var:str/var:arr]  <-
			 *
			 * - resizes the container to contain 'count' elements 
			 * - in extension case 'X' element(s) will be appended      
			 * - in compress case all elements above the 'count' will be discarded
			 * - iterators may be invalidated
			 * 
			 * param count -> new size
			 * param X     -> the value to initialize the new elements with
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 * ex:
			 *  var a = { 1 }, c = '!';
			 *  a.resize(3, '!');  // "{1,'!','!'}"
			 *  var s = "s";
			 *  s.resize(4, c);    // "s!!!"
			 *  
			 */
			virtual void resize(ullong count, char X);

			/** ->  void resize(ullong count, cch *X) [var:arr]  <-
			 *
			 * - resizes the container to contain 'count' elements 
			 * - in extension case 'X' element(s) will be appended      
			 * - in compress case all elements above the 'count' will be discarded
			 * - iterators may be invalidated
			 * 
			 * param count -> new size
			 * param X     -> the value to initialize the new elements with
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 * ex:
			 *  var a = { 1 };
			 *  a.resize(3, "www");  // "{1,"www","www"}"
			 *  
			 */
			virtual void resize(ullong count, cch *X);

			/** ->  void resize(ullong count, const str &X) [var:arr]  <-
			 *
			 * - resizes the container to contain 'count' elements 
			 * - in extension case 'X' element(s) will be appended      
			 * - in compress case all elements above the 'count' will be discarded
			 * - iterators may be invalidated
			 * 
			 * param count -> new size
			 * param X     -> the value to initialize the new elements with
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 * ex:
			 *  var a = { 1 };
			 *  str S = "www";
			 *  a.resize(3, S);  // "{1,"www","www"}"
			 *  
			 */
			virtual void resize(ullong count, const str &X);

			/** ->  void resize(ullong count, __iniL<var> X) [var:arr]  <-
			 *
			 * - resizes the container to contain 'count' elements 
			 * - in extension case 'X' element(s) will be appended      
			 * - in compress case all elements above the 'count' will be discarded
			 * - iterators may be invalidated
			 * 
			 * param count -> new size
			 * param X     -> the value to initialize the new elements with
			 *
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 * ex:
			 *  var a = { 1 };
			 *  a.resize(3, { 7 });  // "{1,{7},{7}}"
			 *  
			 */
			virtual void resize(ullong count, __iniL<var> X);

			/** ->  var:itr insert(ullong index, const var:itr &src_first,
			 *                                   const var:itr &src_last) [var:str/var:arr]  <-
			 *
			 * - inserts elements from range [src_first, src_last) before 'index'
			 * - iterators may be invalidated
			 * 
			 * param index     -> position at which the content will be inserted
			 * param src_first -> source start from (inclusive)
			 * param src_last  -> source last       (not inclusive)
			 * return          -> var:itr (set to 'index')
			 *                                        
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]
			 *	           > [ length_error ]
			 *	           > [ bad_iterator ] 
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 * ex:  
			 *  var itr, a = { 1, 2, 3 }, ains = { 'a', 'b', 'c' };
			 *	var s = "string", sins = "ABC";
			 *  itr = a.insert(1, ains.begin(), ains.end()); // "{1,'a','b','c',2,3}" 
			 *  itr = s.insert(1, sins.begin(), sins.end()); // "sABCtring" 
			 *  
			 *  note: rbegin()/rend() will work too (revers)
			 *   
			 */
			virtual var insert(ullong index, const var &src_fist, const var &src_last);

			/** ->  var:itr insert(ullong index, const var &X) [var:str/var:arr]  <-
			 *
			 * - inserts 'X' before 'index'   
			 * - iterators may be invalidated    
			 *
			 * param index -> position at which the content will be inserted
			 * param X     -> value to insert
			 *                for: var:arr -> var:X 
			 *                     var:str -> var:X
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ] 
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]      
			 * ex: 
			 *  var a = { 1, 2, 3 }, s = "string", itr, c = 'A';
			 *  itr = a.insert(1, s);  // "{1,"string",2,3,4}"
			 *  itr = s.insert(0, c);  // "Astring"
			 *  
			 */
			virtual var insert(ullong index, const var &X);

			/** ->  var:itr insert(ullong index, int X) [var:str/var:arr]  <-
			 *
			 * - inserts 'X' before 'index'   
			 * - iterators may be invalidated    
			 *
			 * param index -> position at which the content will be inserted
			 * param X     -> value to insert
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ] 
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]      
			 * ex: 
			 *  var a = { 1, 2, 3 }, s = "string", itr;
			 *  itr = a.insert(1, 7);  // "{1,7,2,3,4}"
			 *  itr = s.insert(0, 7);  // "7string"
			 *  
			 */
			virtual var insert(ullong index, int X);

			/** ->  var:itr insert(ullong index, float X) [var:str/var:arr]  <-
			 *
			 * - inserts 'X' before 'index'   
			 * - iterators may be invalidated    
			 *
			 * param index -> position at which the content will be inserted
			 * param X     -> value to insert
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ] 
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]      
			 * ex: 
			 *  var a = { 1, 2, 3 }, s = "string", itr;
			 *  itr = a.insert(1, (float) 3.3);  // "{1,3.300000,2,3,4}"
			 *  itr = s.insert(0, (float) 3.3);  // "3.300000string"
			 *  
			 */
			virtual var insert(ullong index, float X);

			/** ->  var:itr insert(ullong index, double X) [var:str/var:arr]  <-
			 *
			 * - inserts 'X' before 'index'   
			 * - iterators may be invalidated    
			 *
			 * param index -> position at which the content will be inserted
			 * param X     -> value to insert
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ] 
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]      
			 * ex: 
			 *  var a = { 1, 2, 3 }, s = "string", itr;
			 *  itr = a.insert(1, 3.3);  // "{1,3.300000,2,3,4}"
			 *  itr = s.insert(0, 3.3);  // "3.300000string"
			 *  
			 */
			virtual var insert(ullong index, double X);

			/** ->  var:itr insert(ullong index, long X) [var:str/var:arr]  <-
			 *
			 * - inserts 'X' before 'index'   
			 * - iterators may be invalidated    
			 *
			 * param index -> position at which the content will be inserted
			 * param X     -> value to insert
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ] 
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]                   
			 * ex:
			 *  var a = { 1, 2, 3 }, s = "string", itr;
			 *  itr = a.insert(1, (long) 7);  // "{1,7,2,3,4}"
			 *  itr = s.insert(0, (long) 7);  // "7string"
			 *  
			 */
			virtual var insert(ullong index, long X);

			/** ->  var:itr insert(ullong index, llong X) [var:str/var:arr]  <-
			 *
			 * - inserts 'X' before 'index'   
			 * - iterators may be invalidated    
			 *
			 * param index -> position at which the content will be inserted
			 * param X     -> value to insert
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ] 
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]                   
			 * ex: 
			 *  var a = { 1, 2, 3 }, s = "string", itr;
			 *  itr = a.insert(1, (llong) 7);  // "{1,7,2,3,4}"
			 *  itr = s.insert(0, (llong) 7);  // "7string"
			 *  
			 */
			virtual var insert(ullong index, llong X);

			/** ->  var:itr insert(ullong index, ullong X) [var:str/var:arr]  <-
			 *
			 * - inserts 'X' before 'index'   
			 * - iterators may be invalidated    
			 *
			 * param index -> position at which the content will be inserted
			 * param X     -> value to insert
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ] 
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]                 
			 * ex:
			 *  var a = { 1, 2, 3 }, s = "string", itr;
			 *  itr = a.insert(1, (ullong) 7);  // "{1,7,2,3,4}"
			 *  itr = s.insert(0, (ullong) 7);  // "7string"
			 *  
			 */
			virtual var insert(ullong index, ullong X);

			/** ->  var:itr insert(ullong index, char X) [var:str/var:arr]  <-
			 *
			 * - inserts 'X' before 'index'   
			 * - iterators may be invalidated    
			 *
			 * param index -> position at which the content will be inserted
			 * param X     -> value to insert
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ] 
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]     
			 * ex: 
			 *  var a = { 1, 2, 3 }, s = "string", itr;
			 *  itr = a.insert(1, 'c');  // "{1,'c',2,3,4}"
			 *  itr = s.insert(0, 'W');  // "Wstring"
			 *  
			 */
			virtual var insert(ullong index, char X);

			/** ->  var:itr insert(ullong index, cch *X) [var:str/var:arr]  <-
			 *
			 * - inserts 'X' before 'index'   
			 * - iterators may be invalidated    
			 *
			 * param index -> position at which the content will be inserted
			 * param X     -> value to insert
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ] 
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]                
			 * ex: 
			 *  var a = { 1, 2, 3 }, s = "string", itr;
			 *  itr = a.insert(1, "INS");  // "{1,"INS",2,3,4}"
			 *  itr = s.insert(0, "INS");  // "INSstring"
			 *  
			 */
			virtual var insert(ullong index, cch *X);

			/** ->  var:itr insert(ullong index, const str &X) [var:str/var:arr]  <-
			 *
			 * - inserts 'X' before 'index'   
			 * - iterators may be invalidated    
			 *
			 * param index -> position at which the content will be inserted
			 * param X     -> value to insert
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ] 
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]                
			 * ex: 
			 *  var a = { 1, 2, 3 }, s = "string", itr;
			 *  std::string S = "INS";
			 *  itr = a.insert(1, S);  // "{1,"INS",2,3,4}"
			 *  itr = s.insert(0, S);  // "INSstring"
			 *  
			 */
			virtual var insert(ullong index, const str &X);

			/** ->  var:itr insert(ullong index, __iniL<var> X) [var:str/var:arr]  <-
			 *
			 * - inserts 'X' before 'index'   
			 * - iterators may be invalidated    
			 *
			 * param index -> position at which the content will be inserted
			 * param X     -> value to insert
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ] 
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]                
			 * ex: 
			 *  var a = { 1, 2, 3 }, s = "string", itr;
			 *  itr = a.insert(1, { "INS1", "INS2" });  // "{1,{"INS1","INS2"},2,3,4}"
			 *  itr = s.insert(0, { "INS1", "INS2" });  // "INS1INS2string"
			 *  
			 */
			virtual var insert(ullong index, __iniL<var> X);

			/** ->  var:itr insert(ullong index, ullong count, const var &X) [var:str/var:arr]  <-
			 *
			 * - inserts 'count' copies of the 'X' before 'index'  
			 * - iterators may be invalidated     
			 *  
			 * param index -> position at which the content will be inserted
			 * param count -> repeat-count
			 * param X     -> value to insert
			 *                for: var:arr -> var:X
			 *                     var:str -> char or var:char (only)   
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]  
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex: 
			 *  var a = { 1, 2, 3 }, ins = 'X', itr, s = "string";
			 *  itr = a.insert(1, 3, ins);   // "{1,'X','X','X',2,3}"
			 *  itr = s.insert(1, 3, ins);   // "sXXXtring"
			 *  
			 */
			virtual var insert(ullong index, ullong count, const var &X);

			/** ->  var:I_arr insert(ullong index, ullong count, int X) [var:arr]  <-
			 *
			 * - inserts 'count' copies of the 'X' before 'index'  
			 * - iterators may be invalidated     
			 *  
			 * param index -> position at which the content will be inserted
			 * param count -> repeat-count
			 * param X     -> value to insert
			 * return      -> var:I_arr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]  
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex: 
			 *  var a = { 1, 2, 3 }, itr;
			 *  itr = a.insert(1, 3, 7);   // "{1,7,7,7,2,3}"
			 *  
			 */
			virtual var insert(ullong index, ullong count, int X);

			/** ->  var:I_arr insert(ullong index, ullong count, float X) [var:arr]  <-
			 *
			 * - inserts 'count' copies of the 'X' before 'index'  
			 * - iterators may be invalidated     
			 *  
			 * param index -> position at which the content will be inserted
			 * param count -> repeat-count
			 * param X     -> value to insert
			 * return      -> var:I_arr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]  
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex:
			 *  var a = { 1, 2, 3 }, itr;
			 *  itr = a.insert(1, 3, (float) 7.7);   // "{1,7.700000,7.700000,7.700000,2,3}"
			 *  
			 */
			virtual var insert(ullong index, ullong count, float X);

			/** ->  var:I_arr insert(ullong index, ullong count, double X) [var:arr]  <-
			 *
			 * - inserts 'count' copies of the 'X' before 'index'  
			 * - iterators may be invalidated     
			 *  
			 * param index -> position at which the content will be inserted
			 * param count -> repeat-count
			 * param X     -> value to insert
			 * return      -> var:I_arr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]  
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex:
			 *  var a = { 1, 2, 3 }, itr;
			 *  itr = a.insert(1, 3, 7.7);   // "{1,7.700000,7.700000,7.700000,2,3}"
			 */
			virtual var insert(ullong index, ullong count, double X);

			/** ->  var:I_arr insert(ullong index, ullong count, long X) [var:arr]  <-
			 *
			 * - inserts 'count' copies of the 'X' before 'index'  
			 * - iterators may be invalidated     
			 *  
			 * param index -> position at which the content will be inserted
			 * param count -> repeat-count
			 * param X     -> value to insert
			 * return      -> var:I_arr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]  
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex: 
			 *  var a = { 1, 2, 3 }, itr;
			 *  itr = a.insert(1, (long) 3, 7);   // "{1,7,7,7,2,3}"
			 *  
			 */
			virtual var insert(ullong index, ullong count, long X);

			/** ->  var:I_arr insert(ullong index, ullong count, llong X) [var:arr]  <-
			 *
			 * - inserts 'count' copies of the 'X' before 'index'  
			 * - iterators may be invalidated     
			 *  
			 * param index -> position at which the content will be inserted
			 * param count -> repeat-count
			 * param X     -> value to insert
			 * return      -> var:I_arr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]  
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex:
			 *  var a = { 1, 2, 3 }, itr;
			 *  itr = a.insert(1, (llong) 3, 7);   // "{1,7,7,7,2,3}"
			 *  
			 */
			virtual var insert(ullong index, ullong count, llong X);

			/** ->  var:I_arr insert(ullong index, ullong count, ullong X) [var:arr]  <-
			 *
			 * - inserts 'count' copies of the 'X' before 'index'  
			 * - iterators may be invalidated     
			 *  
			 * param index -> position at which the content will be inserted
			 * param count -> repeat-count
			 * param X     -> value to insert
			 * return      -> var:I_arr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]  
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex:
			 *  var a = { 1, 2, 3 }, itr;
			 *  itr = a.insert(1, (ullong) 3, 7);   // "{1,7,7,7,2,3}"
			 *  
			 */
			virtual var insert(ullong index, ullong count, ullong X);

			/** ->  var:itr insert(ullong index, ullong count, char X) [var:arr/var:str]  <-
			 *
			 * - inserts 'count' copies of the 'X' before 'index'  
			 * - iterators may be invalidated     
			 *  
			 * param index -> position at which the content will be inserted
			 * param count -> repeat-count
			 * param X     -> value to insert
			 * return      -> var:itr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]  
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex: 
			 *  var a = { 1, 2, 3 }, itrm s = "string";
			 *  itr = a.insert(1, 3, 'X');   // "{1,'X','X','X',2,3}"
			 *  itr = s.insert(1, 3, 'X');   // "sXXXtring"
			 *  
			 */
			virtual var insert(ullong index, ullong count, char X);

			/** ->  var:I_arr insert(ullong index, ullong count, cch *X) [var:arr]  <-
			 *
			 * - inserts 'count' copies of the 'X' before 'index'  
			 * - iterators may be invalidated     
			 *  
			 * param index -> position at which the content will be inserted
			 * param count -> repeat-count
			 * param X     -> value to insert
			 * return      -> var:I_arr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]  
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex: 
			 *  var a = { 1, 2, 3 }, itr;
			 *  itr = a.insert(1, 3, "INS");  // "{1,"INS","INS","INS",2,3}"
			 *  
			 */
			virtual var insert(ullong index, ullong count, cch *X);

			/** ->  var:I_arr insert(ullong index, ullong count, const str &X) [var:arr]  <-
			 *
			 * - inserts 'count' copies of the 'X' before 'index'  
			 * - iterators may be invalidated     
			 *  
			 * param index -> position at which the content will be inserted
			 * param count -> repeat-count
			 * param X     -> value to insert
			 * return      -> var:I_arr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]  
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex: 
			 *  var a = { 1, 2, 3 }, itr;
			 *  std::string S = "INS";
			 *  itr = a.insert(1, 3, S);   // "{1,"INS","INS","INS",2,3}"
			 *  
			 */
			virtual var insert(ullong index, ullong count, const str &X);

			/** ->  var:I_arr insert(ullong index, ullong count, __iniL<var> X) [var:arr]  <-
			 *
			 * - inserts 'count' copies of the 'X' before 'index'  
			 * - iterators may be invalidated     
			 *  
			 * param index -> position at which the content will be inserted
			 * param count -> repeat-count
			 * param X     -> value to insert
			 * return      -> var:I_arr (set to index)
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]  
			 *	           > [ length_error ]
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex: 
			 *  var a = { 1, 2, 3 }, itr;
			 *  itr = a.insert(1, 3, { 'X', 7 });   // "{1,{'X',7},{'X',7},{'X',7},2,3}"
			 *  
			 */
			virtual var insert(ullong index, ullong count, __iniL<var> X);

			/** ->  var:itr erase(ullong index) [var:str/var:arr]  <- 
			 *
			 * - removes the element at 'index'
			 * - iterators may be invalidated
			 * 
			 * param index -> target element
			 * return      -> var:itr (set to index)
			 *                                     
			 * note: returns var:udf when 'last' element are erased
			 *    
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ] 
			 *             > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex:
			 *  var a = { 1, 2, 3 }, s = "string";
			 *  itr = a.erase(1);   // "{1,3}"
			 *  itr = s.erase(1);   // "sring"
			 *  
			 */
			virtual var erase(ullong index);

			/** ->  var:itr erase(ullong first, ullong last) [var:str/var:arr]  <-
			 *
			 * - removes the elements in the range [first, last) (first!=last)
			 * - iterators may be invalidated
			 * 
			 * param first -> start from (inclusive)
			 * param last  ->            (not inclusive)
			 * return      -> var:itr (set to last)  
			 *                                     
			 * note: returns var:udf when 'last' element in the range of elements are erased
			 *    
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 2, 3, 4 }, s = "string";
			 *  itr = a.erase(1, 3);   // "{1,4}"
			 *  itr = s.erase(1, 3);   // "sing"
			 *  
			 */
			virtual var erase(ullong first, ullong last);

			/** ->  var:itr push_back(const var &X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the end
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 *            for: var:arr -> var:X 
			 *                 var:str -> var:X  
			 *             
			 * return  -> var:itr (set to end)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 2 }, s = "string", itr, push = "PUSH";
			 *  itr = a.push_back(push);   // "{1,2,"PUSH"}"
			 *  itr = s.push_back(push);   // "stringPUSH"
			 *  
			 */
			virtual var push_back(const var &X);

			/** ->  var:itr push_back(int X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the end
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to end)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var a = { 1, 2 }, s = "string", itr;
			 *  itr = a.push_back(777);   // "{1,2,777}"
			 *  itr = s.push_back(777);   // "string777"
			 *  
			 */
			virtual var push_back(int X);

			/** ->  var:itr push_back(float X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the end
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to end)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var a = { 1, 2 }, s = "string", itr;
			 *  itr = a.push_back( (float) 777.7);   // "{1,2,777.700012}"
			 *  itr = s.push_back( (float) 777.7);   // "string777.700012"
			 *  
			 */
			virtual var push_back(float X);

			/** ->  var:itr push_back(double X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the end
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to end)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var a = { 1, 2 }, s = "string", itr;
			 *  itr = a.push_back(777.7);   // "{1,2,777.700000}"
			 *  itr = s.push_back(777.7);   // "string777.700000"
			 */
			virtual var push_back(double X);

			/** ->  var:itr push_back(long X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the end
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to end)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var a = { 1, 2 }, s = "string", itr;
			 *  itr = a.push_back( (long) 777);   // "{1,2,777}"
			 *  itr = s.push_back( (long) 777);   // "string777"
			 *  
			 */
			virtual var push_back(long X);

			/** ->  var:itr push_back(llong X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the end
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to end)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex: 
			 *  var a = { 1, 2 }, s = "string", itr;
			 *  itr = a.push_back( (llong) 777);   // "{1,2,777}"
			 *  itr = s.push_back( (llong) 777);   // "string777"
			 *  
			 */
			virtual var push_back(llong X);

			/** ->  var:itr push_back(ullong X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the end
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to end)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var a = { 1, 2 }, s = "string", itr;
			 *  itr = a.push_back( (ullong) 777);   // "{1,2,777}"
			 *  itr = s.push_back( (ullong) 777);   // "string777"
			 *  
			 */
			virtual var push_back(ullong X);

			/** ->  var:itr push_back(char X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the end
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to end)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var a = { 1, 2 }, s = "string", itr;
			 *  itr = a.push_back('X');    // "{1,2,'X'}"
			 *  itr = s.push_back('X');    // "stringX"
			 *  
			 */
			virtual var push_back(char X);

			/** ->  var:itr push_back(cch* X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the end
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to end)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var a = { 1, 2 }, s = "string", itr;
			 *  itr = a.push_back("PUSH");    // "{1,2,"PUSH"}"
			 *  itr = s.push_back("PUSH");    // "stringPUSH"
			 *  
			 */
			virtual var push_back(cch *X);

			/** ->  var:itr push_back(const str &x) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the end
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to end)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var a = { 1, 2 }, s = "string", itr;
			 *  std::string S = "PUSH";
			 *  itr = a.push_back(S);    // "{1,2,"PUSH"}"
			 *  itr = s.push_back(S);    // "stringPUSH"
			 *  
			 */
			virtual var push_back(const str &X);

			/** ->  var:itr push_back(__iniL<var> X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the end
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to end)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var a = { 1, 2 }, s = "string", itr;
			 *  itr = a.push_back( {'X', 'Y'} );   // "{1,2,{'X','Y'}}"
			 *  itr = s.push_back( {'X', 'Y'} );   // "stringXY"
			 *  
			 */
			virtual var push_back( __iniL<var> X);

			/** ->  var:itr push_front(const var &X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the begin
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 *            for: var:arr -> var:X 
			 *                 var:str -> var:X  
			 *             
			 * return  -> var:itr (set to begin)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var s = "123456", a = { 'B', 'C', 'D' }, itr;
			 *	itr = s.push_front("id=");   // "id=123456"
			 *	itr = a.push_front('A');     // "{'A','B','C','D'}"
			 *  
			 */
			virtual var push_front(const var &X);

			/** ->  var:itr push_front(int X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the begin
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to begin)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var s = "456", a = { 1, 2, 3 }, itr;
			 *	itr = s.push_front(123);   // "123456"
			 *	itr = a.push_front(0);     // "{0,1,2,3}"
			 *  
			 */
			virtual var push_front(int X);

			/** ->  var:itr push_front(float X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the begin
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to begin)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *	var s = "$", a = { "$" }, itr;
			 *  itr = s.push_front((float) 1.5);   // "1.5$"
			 *  itr = a.push_front((float) 2.5);
			 *  a.sjoin();                         // "2.500000$"
			 *  
			 */
			virtual var push_front(float X);

			/** ->  var:itr push_front(double X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the begin
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to begin)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var s = "$", a = { "$" }, itr;
			 *  itr = s.push_front(1.5);   // "1.5$"
			 *  itr = a.push_front(2.5);
			 *  a.sjoin();                 // "2.500000$"
			 * 
			 */
			virtual var push_front(double X);

			/** ->  var:itr push_front(long X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the begin
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to begin)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var s = "456", a = { 1, 2, 3 }, itr;
			 *	itr = s.push_front( (long) 123);   // "123456"
			 *	itr = a.push_front( (long) 0);     // "{0,1,2,3}"
			 *  
			 */
			virtual var push_front(long X);

			/** ->  var:itr push_front(llong X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the begin
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to begin)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex: 
			 *  var s = "456", a = { 1, 2, 3 }, itr;
			 *	itr = s.push_front( (llong) 123);   // "123456"
			 *	itr = a.push_front( (llong) 0);     // "{0,1,2,3}"
			 *  
			 */
			virtual var push_front(llong X);

			/** ->  var:itr push_front(ullong X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the begin
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to begin)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var s = "456", a = { 1, 2, 3 }, itr;
			 *	itr = s.push_front( (llong) 123);   // "123456"
			 *	itr = a.push_front( (llong) 0);     // "{0,1,2,3}"
			 *  
			 */
			virtual var push_front(ullong X);

			/** ->  var:itr push_front(char X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the begin
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to begin)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var s = "tring", a = { 'B', 'C', 'D' }, itr;
			 *	itr = s.push_front('S');   // "String"
			 *	itr = a.push_front('A');   // "{'A','B','C','D'}"
			 *  
			 */
			virtual var push_front(char X);

			/** ->  var:itr push_front(cch* X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the begin
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to begin)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var s = "ing", a = {  "CD" }, itr;
			 *	itr = s.push_front("STR");   // "STRing"
			 *	itr = a.push_front("AB");    // "{'AB','CD'}"
			 *  
			 */
			virtual var push_front(cch *X);

			/** ->  var:itr push_front(const str &x) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the begin
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to begin)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  std::string S = "Hello";
			 *  var s = " World", a = { "World" }, itr;
			 *	itr = s.push_front(S);   // "Hello World"
			 *	itr = a.push_front(S);   // "{'Hello','World'}"
			 *  
			 */
			virtual var push_front(const str &X);

			/** ->  var:itr push_front(__iniL<var> X) [var:str/var:arr]  <- 
			 *
			 * - appends the given element 'X' to the begin
			 * - iterators may be invalidated
			 *  
			 * param X -> value to push
			 * return  -> var:itr (set to begin)
			 *  
			 * exceptions: catch(VarExcp &ex) 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 * var s = "Z", a = { "Z" }, itr;
			 * itr = s.push_front( { 'X', 'Y' });   // "XYZ"
			 * itr = a.push_front( { 'X', 'Y' });   // "{{'X','Y'},"Z"}"
			 *  
			 */
			virtual var push_front( __iniL<var> X);

			/** ->  var:X pop_back() [var:str/var:arr]  <-
			 *
			 * - removes the last element of the container
			 * - iterators may be invalidated
			 * 
			 * return -> var:X            
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]      
			 * ex:
			 *  var s = "STR", a = { 1, 2, 3 };
			 *	
			 *	x = s.pop_back();   // R   "ST"
			 *  x = s.pop_back();   // T   "S"
			 *  x = s.pop_back();   // S   ""
			 *	
			 *  x = a.pop_back();   // 3   "{1,2}"
			 *  x = a.pop_back();   // 2   "{1}"
			 *  x = a.pop_back();   // 1   "{}"
			 *  
			 */
			virtual var pop_back();

			/** ->  var:X pop_front() [var:str/var:arr]  <-
			 *
			 * - removes the first element of the container
			 * - iterators may be invalidated
			 * 
			 * return -> var:X            
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var s = "STR", a = { 1, 2, 3 };
			 *	
			 *	x = s.pop_front();   // S   "TR"
			 *  x = s.pop_front();   // T   "R"
			 *  x = s.pop_front();   // R   ""
			 *	
			 *  x = a.pop_front();   // 1   "{2,3}"
			 *  x = a.pop_front();   // 2   "{3}"
			 *  x = a.pop_front();   // 3   "{}"
			 *  
			 */
			virtual var pop_front();

			/** ->  var:str/arr sort_cmp(bool (*cmp)(var:X/itr &a, var:X/itr &b),
			 *                       bool reverse = false) [var:str/var:arr]  <-
			 *                           
			 * - sorts the elements using the function 'cmp' and returns the result
			 * 
			 * param cmp     -> comparison function
			 * passed arguments: for: var:arr -> a,b are var:X    
			 *                        var:str -> a,b are var:itr
			 * param reverse -> reverses the order of the iterations
			 * return        -> for: var:arr -> new sorted array              
			 *                       var:str -> new sorted string
			 *  
			 * note: (internal behavior)
			 *       if the 'cmp' function returns: true  -> swap(a,b) 
			 *                                      false -> does nothing
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 6, 3, 7, 2, 9, 4, 1, 8, 0, 5 }, ra;
			 *	ra = a.sort_cmp([](auto a, auto b) {
			 *	    return b < a;           // ra = "{0,1,2,3,4,5,6,7,8,9}"  
			 *  });                         // ra = "{9,8,7,6,5,4,3,2,1,0}" (reverse=true)   
			 *  
			 *  var s = "FBEDCA", rs;
			 *  rs = s.sort_cmp([](auto a, auto b) {
			 *      return *b < *a;          // rs = "ABCDEF"
			 *  });                          // rs = "FEDCBA" (reverse=true)
			 * 
			 *  
			 */
			virtual var sort_cmp(bool (*cmp)(var &a, var &b), bool reverse = false) const;

			/** ->  var:self& s(elf)sort_cmp(bool (*cmp)(var:X/itr &a, var:X/itr &b),
			 *                               bool reverse = false) [var:str/var:arr]  <-
			 *                           
			 * - sorts the own elements using the function 'cmp'
			 * 
			 * param cmp    -> comparison function
			 * passed arguments: for: var:arr -> a,b are var:X    
			 *                        var:str -> a,b are var:itr
			 * param reverse -> reverses the order of the iterations
			 * return        -> var:self&
			 *  
			 * note: (internal behavior)
			 *       if the 'cmp' function returns: true  -> swap(a,b) 
			 *                                      false -> does nothing
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 6, 3, 7, 2, 9, 4, 1, 8, 0, 5 };
			 *	a.ssort_cmp([](auto a, auto b) {
			 *	    return b < a;           // a = "{0,1,2,3,4,5,6,7,8,9}"  
			 *  });                         // a = "{9,8,7,6,5,4,3,2,1,0}" (reverse=true)   
			 *  
			 *  var s = "FBEDCA";
			 *  s.ssort_cmp([](auto a, auto b) {
			 *      return *b < *a;          // s = "ABCDEF"
			 *  });                          // s = "FEDCBA" (reverse=true)
			 * 
			 *  
			 */
			virtual var& ssort_cmp(bool (*cmp)(var &a, var &b), bool reverse = false);

			/** ->  var:str/arr sort(bool reverse = false) [var:str/var:arr]  <-
			 *                           
			 * - sorts the elements and returns the result (default sorting)
			 * 
			 * param reverse -> reverses the order of the iterations
			 * return        -> for: var:arr -> new sorted array              
			 *                       var:str -> new sorted string
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var s = "str", sb = s.begin(), srb = s.rbegin(), udf;
			 *  var a = { 1 }, ab = a.begin(), arb = a.rbegin();
			 *
			 *  var map, pmap1, pmap2;
			 *  map.map_set(""
			 *	  	  " { cars: { germany: MercedesBenz: 6; "
			 *		  "                    Opel: 4;         "
			 *		  "                    BMW: 5;          "
			 *		  "         }"
			 *		  " }");
			 *  pmap1 = map.map_get("cars:germany:BMW");
			 *  pmap2 = map.map_get("cars:germany:Opel");
			 *
			 *  var A = { a, sb, "AB", ab, 2.5, "ABC", false, var( { }), srb, { 1, 2 }, //
			 *	  	  pmap2, 'A', udf, 1, map, var(), arb, 'B', true, pmap1 };
			 *
			 *  var ra = A.sort(); // ra = "[ 20 ]{ false,                            booleans
			 *					   //               true,                                |
			 *					   //               1,                                numbers 
			 *					   //               2.500000,                            |
			 *					   //               'A',                               chars
			 *					   //               'B',                                 |
			 *					   //               "AB",                             strings
			 *					   //               "ABC",                               |
			 *					   //               { },                               arrays
			 *					   //               [ 1 ]{ 1 },                          |
			 *					   //               [ 2 ]{ 1,                            |
			 *					   //                      2 },                          |
			 *					   //               { cars: { american: Dodge: 5;       maps
			 *					   //                                   Ford: 7;         |
			 *					   //                                   Lincoln: |udf|;  | 
			 *					   //                       }                            |
			 *					   //               },                                   |
			 *					   //               | [var:I_str] val: s |,          iterators
			 *					   //               | [var:Ir_str] val: r |,             |
			 *					   //               | [var:I_arr] val: 1 |,              |
			 *					   //               | [var:Ir_arr] val: 1 |,             |
			 *					   //               | [var:P_map] val: 4 |,         map-pointers
			 *					   //               | [var:P_map] val: 5 |,              |
			 *					   //               |udf|,                           undefined
			 *					   //               |udf| }                              |               
			 *
			 *  var S = "cB3bC2A1a";
			 *  var rs = S.sort();    // rs = "123ABCabc"
			 *  
			 */
			virtual var sort(bool reverse = false) const;

			/** ->  var:self& s(elf)sort(bool reverse = false) [var:str/var:arr]  <-
			 *                           
			 * - sorts the own elements (default sorting)
			 * 
			 * param reverse -> reverses the order of the iterations
			 * return        -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * 
			 * ex: ( for eaxamples look: var.h -> var sort(bool reverse = false) const )
			 *  
			 */
			virtual var& ssort(bool reverse = false);

			/** ->  var:str/arr sample(ullong num = 0) [var:str/var:arr]  <-
			 *                           
			 * - selects 'num' elements from the underlying sequence
			 * 
			 * param num  -> number of samples to make  
			 *               0 (def) -> will be taken length of the sequence
			 * return     -> for: var:arr -> new array sample              
			 *                    var:str -> new string sample
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, x;
			 *  x = a.sample(5);   // x = "{7,8,2,7,6}"
			 *  x = a.sample();    // x = "{8,6,5,7,5,1,7,4,6}"
			 *  
			 *  var s = "ABCDEFGHIJKLMNOPQ"; 
			 *  x = s.sample(10);  // x = "LMIEOLPAMN"
			 *  x = s.sample();    // x = "CCDQOMNLKHECMFJLN"
			 *  
			 */
			virtual var sample(ullong num = 0) const;

			/** ->  var:str/arr sample_unq(ullong num = 0) [var:str/var:arr]  <-
			 *                           
			 * - selects "num" unique elements from the underlying sequence
			 *   
			 * note: sample uniqued by the positions(indexes), not by the values(repeated)
			 * 
			 * param num  -> number of samples to make  
			 *               0 (def) -> will be taken length of the sequence
			 * return     -> for: var:arr -> new array  sample              
			 *                    var:str -> new string sample
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, x;
			 *  x = a.sample_unq(5);   // x = "{5,2,6,3,9}"
			 *  x = a.sample_unq();    // x = "{2,6,3,4,1,5,7,9,8}"
			 *		
			 *  var s = "ABCDEFGHIJKLMNOPQ";
			 *  x = s.sample_unq(10);  // x = "NGCDBKJLHM"
			 *  x = s.sample_unq();    // x = "JMCQBAKNGILEOFHPD"
			 *  
			 */
			virtual var sample_unq(ullong num = 0) const;

			/** ->  var:str/arr reverse() [var:str/var:arr]  <-
			 *                           
			 * - reverses the order of the elements 
			 * 
			 * return  -> for: var:arr -> new reverse array              
			 *                 var:str -> new reverse string
			 *  
			 * exceptions: catch(VarExcp &ex)  
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 }, x;
			 *  x = a.reverse();   // x = "{0,9,8,7,6,5,4,3,2,1}"
			 *
			 *  var s = "ABCDEFGHIJKLMNOPQ";
			 *  x = s.reverse();   // x = "QPONMLKJIHGFEDCBA"
			 *  
			 */
			virtual var reverse();

			/** ->  var:self& reverse() [var:str/var:arr]  <-
			 *                           
			 * - reverses the order of the elements 
			 * 
			 * return  -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
			 *  a.sreverse();   // a = "{0,9,8,7,6,5,4,3,2,1}"
			 *
			 *  var s = "ABCDEFGHIJKLMNOPQ";
			 *  s.sreverse();   // s = "QPONMLKJIHGFEDCBA"
			 *  
			 */
			virtual var& sreverse();

			/** ->  bool equal(const var:itr &src_first, const var:itr &src_last, 
			 *                       ullong index = 0, ullong count = 0)  [var:str/var:arr]  <-
			 *                           
			 * - returns true if the range [src_first, src_last) is equal to the range 
			 *   [index, index + count), and false otherwise
			 * 
			 * param src_first -> start-iterator of a source                    (inclusive)
			 * param src_last  -> end-iterator of a source                      (not inclusive)
			 * param index     -> start index     (0 -> from [0])               (inclusive)
			 * param count     -> number of elements after index: 'index+count' (not inclusive)
			 *                                    (0 -> to end)
			 * return  -> if they are equal (by type and value) return true,
			 *            otherwise false
			 *  
			 * note: iterators are equal if they have same type and pointed to the
			 *       same element(more precisely -> the same address) 
			 * note: when comparing arrays, recursion is used
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ bad_iterator ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  bool res = false;
			 *  // arr:
			 *  //         <--------------------
			 *  //         -------------------->
			 *  var a = { 'r', 'a', 'd', 'a', 'r' };
			 *  res = a.equal(a.rbegin(), a.rend(), 0); // true: "radar" is a palindrome
			 *			
			 *  var a1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, a2 = { 3, 4, 5, 6, 7 };
			 *  //                 '''''''''                     '''''''''
			 *  res = a1.equal(a2.begin() += 1, a2.end() -= 1, 3, 3); // true
			 *	
			 *  // different types example:
			 *  
			 *  var m = var().map_set("x:1;y:2;z:3;");
			 *  var a3 = { 1, 2.2, udf, true, { { { 1 } } }, "str", m, 'c', a.begin() };
			 *  var a4 = { 1, 2.2, udf, true, { { { 1 } } }, "str", m, 'c', a.begin() };
			 *	
			 *  res = a3.equal(a4.begin(), a4.end());  // true
			 *  
			 *  // str: (same rules as for arrays)
			 *  
			 *  var s = "level";
			 *  res = s.equal(s.rbegin(), s.rend()); // true: "level" is a palindrome
			 *  
			 *  ... and so on
			 *  
			 */
			virtual bool equal(const var &src_first, const var &src_last, //
					ullong index = 0, ullong count = 0) const;

			/** ->  var:arr/bool mismatch(const var:itr &src_first, const var:itr &src_last, 
			 *                           ullong index = 0, ullong count = 0)  [var:str/var:arr]  <-
			 *   
			 * - returns pair of 'top-level iterators' of the first mismatching pair of elements 
			 *   from two ranges: one defined by [src_first, src_last) and another 
			 *   defined by [index, index + count) 
			 * - if [var:arr]-case contains nested arrays, then it will be additionally returned
			 *   pair of 'inner-level iterators' of the first mismatching pair of elements  
			 * 
			 * param src_first -> start-iterator of a source                    (inclusive)
			 * param src_last  -> end-iterator of a source                      (not inclusive)
			 * param index     -> start index     (0 -> from [0])               (inclusive)
			 * param count     -> number of elements after index: 'index+count' (not inclusive)
			 *                                    (0 -> to end)
			 * return  -> var:arr = { itr_top ,   or   { itr_top ,   or   var:bool:false 
			 *                        src_itr_top,       src_itr_top,     (no mismatch)
			 *                        |udf|,             itr_inn,
			 *                        |udf| }            src_itr_inn }
			 *              (only top level mismatch)    +(inner level mismatch)    
			 *                   
			 * note: iterators are equal if they have same type and pointed to the
			 *       same element(more precisely -> the same address) 
			 * note: when comparing arrays, recursion is used
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ bad_iterator ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var x;
			 *  var a1 = { 1, 2, 3, 4 };      
			 *  var a2 = { 1, 2, 3, 4 };            
			 *                                       
			 *  x = a1.mismatch(a2.begin(), a2.end()); //  false
			 *  
			 *  a1 = { 1, 2, 3, 4 };                   // x = [ 4 ]{ | [var:I_arr] val: 4 |,
			 *  a2 = { 1, 2, 3, 5 };                   //            | [var:I_arr] val: 5 |,
			 *                                         //            |udf|,
			 *  x = a1.mismatch(a2.begin(), a2.end()); //            |udf| }
			 *  
			 *  a1 = { 1, 2, { 1, 2, 3 }, 3 };         // x = [ 4 ]{ | [var:I_arr] val: {1,2,3} |,
			 *  a2 = { 1, 2, { 1, 2, 4 }, 3 };         //            | [var:I_arr] val: {1,2,4} |,
			 *                                         //            | [var:I_arr] val: 3 |,
			 *  x = a1.mismatch(a2.begin(), a2.end()); //            | [var:I_arr] val: 4 | }
			 *  
			 *  a1 = { 1, 2, { 1, 2, 3 }, 3 };         // x = [ 4 ]{ | [var:I_arr] val: {1,2,3} |,
			 *  a2 = { 1, 2, { 1, 2 }, 3 };            //            | [var:I_arr] val: {1,2} |,
			 *                                         //            | [var:I_arr] val: 3 |,
			 *  x = a1.mismatch(a2.begin(), a2.end()); //            | [var:I_arr] at the end() | }
			 *  
			 *  a1 = { 1, { 1, { 1, { 2, 3, { { { { 5 } } } } } } }, 2 }; 
			 *	a2 = { 1, { 1, { 1, { 2, 3, { { { { 4 } } } } } } }, 2 };
			 *                 
			 *  x = a1.mismatch(a2.begin(), a2.end());             
			 *                 
			 *  // x =  [ 4 ]{ | [var:I_arr] val: {1,{1,{2,3,{{{{5}}}}}}} |,
			 *  //             | [var:I_arr] val: {1,{1,{2,3,{{{{4}}}}}}} |,
			 *  //             | [var:I_arr] val: 5 |,
			 *  //             | [var:I_arr] val: 4 | }
			 *  
			 *  a1 = { 1, 2, 3, { 5, var().map_set("x:1;y:2;z:3;"), 6 }, 4 };
			 *  a2 = { 1, 2, 3, { 5, var().map_set("x:1;y:777;z:3;"), 6 }, 4 };
			 * 
			 *  x = a1.mismatch(a2.begin(), a2.end());
			 *  
			 *  [ 4 ]{ | [var:I_arr] val: {5,x:1;y:2;z:3;,6} |,
			 *         | [var:I_arr] val: {5,x:1;y:777;z:3;,6} |,
			 *         | [var:I_arr] val: x:1;y:2;z:3; |,
			 *         | [var:I_arr] val: x:1;y:777;z:3; | }
			 *  
			 *  // ... 
			 *  
			 *  var s1 = "string";
			 *  var s2 = "string";
			 *  
			 *  x = s1.mismatch(s2.begin(), s2.end()); //  false
			 *  
			 *  s2 = "strIng";                         //  x = [ 4 ]{ | [var:I_str] val: 'i' |,
			 *	                                       //             | [var:I_str] val: 'I' |,
			 *	x = s1.mismatch(s2.begin(), s2.end()); //             |udf|,
			 *                                         //             |udf| }
			 *                                          
			 *  ----------------------------------------------------------------------------
			 *  
			 *  // note: (arrays follow the same rules)
			 *  //      - such an 'incomprehensible' output means comparing unequal internal  
			 *  //          \.....     ranges (relative to the ends)  
			 *  //                |
			 *  //        ---->   '-->   x = [ 4 ]{ | [var:I_str] val: 'g' |,                      
			 *  // s1 = "string"                    | [var:I_str] val: 'g' |,
			 *  // s2 = "string"                    |udf|,
			 *  //        --->                      |udf| }
			 *  //
			 *  x = s1.mismatch(s2.begin() += 1, s2.end() -= 1, 1, s1.length() - 1);
			 *  
			 *  -----------------------------------------------------------------------------
			 *  
			 */
			virtual var mismatch(const var &src_first, const var &src_last, //
					ullong index = 0, ullong count = 0) const;

			/** ->  var:self& fill(const var:X &X, ullong index = 0, 
			 *                                     ullong count = 0)  [var:str/var:arr]  <-
			 *                           
			 * - assigns the given 'X' value to the elements in the range [index, index + count)
			 * - iterators may be invalidated
			 * 
			 * param X -> value to assigns for: var:arr -> var:X              
			 *                                  var:str -> var:char or char  (only)
			 * param index  -> start index      (0 -> from [0])              (inclusive)
			 * param count  -> number of elements after index: 'index+count' (not inclusive)
			 *                                  (0 -> to end)                                
			 * note: if (index+count > var:arr/str.length) then var:arr/str will be increased
			 * 
			 * return  -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var c = '!';
			 *  var a = { 1, 1, 1, 1, 1, 1, };
			 *  a.fill(c, 1, 2);   // "{1,'!','!',1,1,1}"
			 *  a.fill(c, 6, 2);   // "{1,'!','!',1,1,1,'!','!'}"
			 *  
			 *  var s = "......";
			 *  s.fill(c, 1, 2), s.prnt();  // ".!!..."
			 *  s.fill(c, 6, 2), s.prnt();  // ".!!...!!"
			 *  
			 */
			virtual var& fill(const var &X, ullong index = 0, ullong count = 0);

			/** ->  var:self& fill(int X, ullong index = 0, ullong count = 0)  [var:arr]  <-
			 *                           
			 * - assigns the given 'X' value to the elements in the range [index, index + count)
			 * - iterators may be invalidated
			 * 
			 * param X      -> value to assigns 
			 * param index  -> start index      (0 -> from [0])              (inclusive)
			 * param count  -> number of elements after index: 'index+count' (not inclusive)
			 *                                  (0 -> to end)                                
			 * note: if (index+count > var:arr.length) then var:arr will be increased
			 * 
			 * return  -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 1, 1, 1, 1, 1, };
			 *  a.fill(2, 1, 2);   // "{1,2,2,1,1,1}"
			 *  a.fill(3, 6, 2);   // "{1,2,2,1,1,1,3,3}"
			 *  
			 */
			virtual var& fill(int X, ullong index = 0, ullong count = 0);

			/** ->  var:self& fill(float X, ullong index = 0, ullong count = 0)  [var:arr]  <-
			 *                           
			 * - assigns the given 'X' value to the elements in the range [index, index + count)
			 * - iterators may be invalidated
			 * 
			 * param X      -> value to assigns 
			 * param index  -> start index      (0 -> from [0])              (inclusive)
			 * param count  -> number of elements after index: 'index+count' (not inclusive)
			 *                                  (0 -> to end)                                
			 * note: if (index+count > var:arr.length) then var:arr will be increased
			 * 
			 * return  -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 1, 1, 1, 1, 1, };
			 *  a.fill( (float) 2.5, 1, 2);   // "{1,2.500000,2.500000,1,1,1}"
			 *  a.fill( (float) 3.5, 6, 2);   // "{1,2.500000,2.500000,1,1,1,3.500000,3.500000}"
			 *  
			 */
			virtual var& fill(float X, ullong index = 0, ullong count = 0);

			/** ->  var:self& fill(double X, ullong index = 0, ullong count = 0)  [var:arr]  <-
			 *                           
			 * - assigns the given 'X' value to the elements in the range [index, index + count)
			 * - iterators may be invalidated
			 * 
			 * param X      -> value to assigns 
			 * param index  -> start index      (0 -> from [0])              (inclusive)
			 * param count  -> number of elements after index: 'index+count' (not inclusive)
			 *                                  (0 -> to end)                                
			 * note: if (index+count > var:arr.length) then var:arr will be increased
			 * 
			 * return  -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 1, 1, 1, 1, 1, };
			 *  a.fill(2.5, 1, 2);   // "{1,2.500000,2.500000,1,1,1}"
			 *  a.fill(3.5, 6, 2);   // "{1,2.500000,2.500000,1,1,1,3.500000,3.500000}"
			 *  
			 */
			virtual var& fill(double X, ullong index = 0, ullong count = 0);

			/** ->  var:self& fill(long X, ullong index = 0, ullong count = 0)  [var:arr]  <-
			 *                           
			 * - assigns the given 'X' value to the elements in the range [index, index + count)
			 * - iterators may be invalidated
			 * 
			 * param X      -> value to assigns 
			 * param index  -> start index      (0 -> from [0])              (inclusive)
			 * param count  -> number of elements after index: 'index+count' (not inclusive)
			 *                                  (0 -> to end)                                
			 * note: if (index+count > var:arr.length) then var:arr will be increased
			 * 
			 * return  -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 1, 1, 1, 1, 1, };
			 *  a.fill( (long) 2, 1, 2);   // "{1,2,2,1,1,1}"
			 *  a.fill( (long) 3, 6, 2);   // "{1,2,2,1,1,1,3,3}"
			 *  
			 */
			virtual var& fill(long X, ullong index = 0, ullong count = 0);

			/** ->  var:self& fill(llong X, ullong index = 0, ullong count = 0)  [var:arr]  <-
			 *                           
			 * - assigns the given 'X' value to the elements in the range [index, index + count)
			 * - iterators may be invalidated
			 * 
			 * param X      -> value to assigns 
			 * param index  -> start index      (0 -> from [0])              (inclusive)
			 * param count  -> number of elements after index: 'index+count' (not inclusive)
			 *                                  (0 -> to end)                                
			 * note: if (index+count > var:arr.length) then var:arr will be increased
			 * 
			 * return  -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 1, 1, 1, 1, 1, };
			 *  a.fill( (llong) 2, 1, 2);   // "{1,2,2,1,1,1}"
			 *  a.fill( (llong) 3, 6, 2);   // "{1,2,2,1,1,1,3,3}"
			 *  
			 */
			virtual var& fill(llong X, ullong index = 0, ullong count = 0);

			/** ->  var:self& fill(ullong X, ullong index = 0, ullong count = 0)  [var:arr]  <-
			 *                           
			 * - assigns the given 'X' value to the elements in the range [index, index + count)
			 * - iterators may be invalidated
			 * 
			 * param X      -> value to assigns 
			 * param index  -> start index      (0 -> from [0])              (inclusive)
			 * param count  -> number of elements after index: 'index+count' (not inclusive)
			 *                                  (0 -> to end)                                
			 * note: if (index+count > var:arr.length) then var:arr will be increased
			 * 
			 * return  -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 1, 1, 1, 1, 1, };
			 *  a.fill( (ullong) 2, 1, 2);   // "{1,2,2,1,1,1}"
			 *  a.fill( (ullong) 3, 6, 2);   // "{1,2,2,1,1,1,3,3}"
			 *  
			 */
			virtual var& fill(ullong X, ullong index = 0, ullong count = 0);

			/** ->  var:self& fill(char X, ullong index = 0, ullong count = 0)  [var:str/var:arr]  <-
			 *                           
			 * - assigns the given 'X' value to the elements in the range [index, index + count)
			 * - iterators may be invalidated
			 * 
			 * param X      -> value to assigns 
			 * param index  -> start index      (0 -> from [0])              (inclusive)
			 * param count  -> number of elements after index: 'index+count' (not inclusive)
			 *                                  (0 -> to end)                                
			 * note: if (index+count > var:arr/str.length) then var:arr/str will be increased
			 * 
			 * return  -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 1, 1, 1, 1, 1, };
			 *  a.fill('!', 1, 2);   // "{1,'!','!',1,1,1}"
			 *  a.fill('?', 6, 2);   // "{1,'!','!',1,1,1,'?','?'}"
			 *  
			 *  var s = "......";
			 *  s.fill('!', 1, 2), s.prnt();   // ".!!..."
			 *  s.fill('!', 6, 2), s.prnt();   // ".!!...??"
			 *  
			 */
			virtual var& fill(char X, ullong index = 0, ullong count = 0);

			/** ->  var:self& fill(cch* X, ullong index = 0, ullong count = 0)  [var:arr]  <-
			 *                           
			 * - assigns the given 'X' value to the elements in the range [index, index + count)
			 * - iterators may be invalidated
			 * 
			 * param X      -> value to assign
			 * param index  -> start index      (0 -> from [0])              (inclusive)
			 * param count  -> number of elements after index: 'index+count' (not inclusive)
			 *                                  (0 -> to end)                                
			 * note: if (index+count > var:arr.length) then var:arr will be increased
			 * 
			 * return  -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 1, 1, 1, 1, 1, };
			 *  a.fill("!", 1, 2);   // "{1,"!","!",1,1,1}"
			 *  a.fill("?", 6, 2);   // "{1,"!","!",1,1,1,"?","?"}"
			 *  
			 */
			virtual var& fill(cch *X, ullong index = 0, ullong count = 0);

			/** ->  var:self& fill(const str &X, ullong index = 0, ullong count = 0)  [var:arr]  <-
			 *                           
			 * - assigns the given 'X' value to the elements in the range [index, index + count)
			 * - iterators may be invalidated
			 * 
			 * param X      -> value to assign 
			 * param index  -> start index      (0 -> from [0])              (inclusive)
			 * param count  -> number of elements after index: 'index+count' (not inclusive)
			 *                                  (0 -> to end)                                
			 * note: if (index+count > var:arr.length) then var:arr will be increased
			 * 
			 * return  -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:
			 *  var a = { 1, 1, 1, 1, 1, 1, };
			 *  std::string s = "!";
			 *  a.fill(s, 1, 2);   // "{1,"!","!",1,1,1}"
			 *  a.fill(s, 6, 2);   // "{1,"!","!",1,1,1,"!","!"}"
			 *  
			 */
			virtual var& fill(const str &X, ullong index = 0, ullong count = 0);

			/** ->  var:self& fill(__iniL<var> X, ullong index = 0, ullong count = 0)  [var:arr]  <-
			 *                           
			 * - assigns the given 'X' value to the elements in the range [index, index + count)
			 * - iterators may be invalidated
			 * 
			 * param X      -> value to assigns 
			 * param index  -> start index      (0 -> from [0])              (inclusive)
			 * param count  -> number of elements after index: 'index+count' (not inclusive)
			 *                                  (0 -> to end)                                
			 * note: if (index+count > var:arr.length) then var:arr will be increased
			 * 
			 * return  -> var:self&
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * ex:
			 *  var a = { 1, 1, 1, 1, 1, 1, };
			 *  std::string s = "!";
			 *  a.fill( { 1, 2 }, 1, 2);   // "{1,{1,2},{1,2},1,1,1}"
			 *  a.fill( { 3, 4 }, 6, 2);   // "{1,{1,2},{1,2},1,1,1,{3,4},{3,4}}"
			 *  
			 */
			virtual var& fill(__iniL<var> X, ullong index = 0, ullong count = 0);

			//*************************************************** waiting for implementation
			
			//
			
			//virtual var copy(bool (*pred)(var &a)=0) const;
			
			//virtual var copy(ullong index, ullong count, bool (*pred)(var &a)=0) const;
			
			//virtual var copy_to(const var &dst_start, bool (*pred)(var &a)=0) const;
			
			//virtual var copy_to(const var &dst_start, ullong index, ullong count, //
			//		bool (*pred)(var &a)=0) const;
			
			//
			
			//******************************************************************************
			
			//---------------------------- arr if: ---------------------------
			
			/** ->  var:self& initArr() [var:arr]  <-
			 *
			 * - because of 'var a = {}' won't work and sometimes it's needed 
			 *   to have empty array -> this interface exists        
			 * 
			 * return -> var:self&
			 * 
			 * ex: 
			 *  var array;
			 *  array.initArr();  // "{ }" 
			 *  
			 */
			virtual var& initArr() noexcept;

			// Operations/Modifiers:
			
			/** ->  var:str join(char sep = 0) [var:arr]  <-
			 *
			 * param sep (separator) -> any char or (0(default) -> no spaces)
			 * return -> the concatenated var:str
			 *   else -> var:udf 
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]           
			 * ex: 
			 *  var a = { '-', "hello", " ", "world", '-' }, s;
			 *  s = a.join();     // "-hello world-"
			 *  s = a.join('|');  // "-|hello| |world|-"
			 *  
			 */
			virtual var join(char sep = 0) const;

			/** ->  var:self& s(elf)join(char sep = 0) [var:arr]  <-
			 * 
			 * - self-transforms var:arr -> to concatenated var:s
			 * - source var:arr will be destroyed (if no references)
			 * - iterators may be invalidated
			 *
			 * param sep (separator) -> any char or (0(default) -> no spaces)
			 * return                -> var:self&
			 *    
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * ex: 
			 *  var a = { '-', "hello", " ", "world", '-' }, b;
			 *  b = a;
			 *  a.sjoin();     // "-hello world-"
			 *  b.sjoin('|');  // "-|hello| |world|-"
			 *  
			 */
			virtual var& sjoin(char sep = 0);

			//---------------------------- map if: ---------------------------
			
			/** ->  var:self& map_init() [var:X]  <-
			 *   
			 *  - initialization of the new type var:map
			 *    (current value(var:type) will be destroyed)
			 *    
			 *  return var:self&
			 * 
			 */
			var& map_init() noexcept;

			/** ->  var:self& map_set(cch *map) [var:map]  <-
			 * 
			 * - takes c++ 'like' string (null-terminated) and parse it to the var:map 
			 * - pointers to map (var:P_map) may be invalidated
			 * 
			 * param map -> const char* , char*(null-terminated)
			 * return    -> var:self& 
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 *             > [ map_parsing_error ]
			 *             > [ map_parsing:string_parsing_error ]
			 *             > [ map_max_field_size ]
			 * ex:
			 *  var x;
			 *  
			 *  // I. common map_set syntax:
			 *  
			 *  x.map_set(""
			 *		    " { cars: { germany: { MercedesBenz: B_Class: 2; "
			 *		    "                                    C_Class: 3; "
			 *			"                    } "
			 *			"                    Opel: 4; "
			 *			"                    BMW: 5;  "
			 *			"         } "
			 *			"         { american: Dodge: 5;    "
			 *			"                     Lincoln: 9;  "
			 *			"                     Ford: 7;     "
			 *			"         } "
			 *			"         other: 6; "  // ................... (1)
			 *			" } "                  // |                 |
			 *			" { movies: { action: _1: \" No Time to Die \"; "
			 *			"                     _2: \" Mission: Impossible - Fallout \"; "
			 *			"                     _3: \"...\"; "
			 *			"           } "
			 *			"           { fiction: _1: \" Edge Of Tomorrow \"; "
			 *			"                      _2: \" The Martian \"; "
			 *			"                      _3: \"...\"; "
			 *			"           } "
			 *			"           undefined:; "
			 *			" } "
			 *			" { math: pi: 3.141593; "
			 *			"         e:  2.718282; "
			 *			" }");
			 *	                   ^
			 *	x.prnt(); // the output will have the same format, but without extra slashes
			 *	
			 *	!!!!!!!!!!!!! note: data are always stored in sorted ascii order !!!!!!!!!!!!!
			 *	
			 *	// II. simple map_set syntax:             // ...
			 *                                            // { germany: BMW: 5;
			 *	x.map_set("cars:germany:Volkswagen:3;");  //            { MercedesBenz: B_Class: 2;
			 *	                                          //                            C_Class: 3;
			 *	                                          //            }
			 *	                                          //            Opel: 4;
			 *	                                          //            Volkswagen: 3;  <-- reassigned
			 *	                                          //  }
			 *	                                          //  ...
			 *	                                           
			 *	// III. complex-mask map_set syntax:       // { cars: { Italy: Fiat: 2;  <-- new
			 *                                             //         }
			 *	x.map_set("{ cars: { france: Renault: 7;"  //         { american: Dodge: 0;   <-- reassigned
			 *		      "                  Peugeot: 4;"  //                     Ford: 0;    <-- reassigned
			 *			  "        }"                      //                     Lincoln: 0; <-- reassigned
			 *			  "        { american: Dodge: 0;"  //         }
			 *	          "                    Lincoln: 0;"//         { france: Peugeot: 7;  <-- new
			 *         	  "                    Ford: 0;"   //                   Renault: 4;  <-- new
			 *	          "        }"                      //         }
			 *			  "        other: 10;"             //         { germany: BMW: 4;         
			 *			  "        Italy: Fiat: 2;"        //                    { MercedesBenz: B_Class: 2;         
			 *			  "}");                            //                                    C_Class: 3;
			 *			                                   //                    } 
			 *			                                   //                    Opel: 4;
			 *				                               //                    Volkswagen: 3;
			 *				                               //         } 
			 *				                               //         other: 10;  <-- reassigned
			 *				                               // }
			 *				                               // ...
			 *				      
			 *				      
			 *	// IV. direct map_set case
			 *
			 *  if (var w = x.map_get(   //                         
			 *	        "cars:germany:MercedesBenz:C_Class")) {  // ...
			 *												     // { MercedesBenz: B_Class: 2;
			 *      *w = 100;                                    //                 C_Class: 100;
			 *											   	     //                           ^
			 *   } else var("fail").prnt();                      // }                     reassigned
			 *											         // ...
			 *	// V. list map_set case
			 *	
			 *	var lm;                                                               // s1: "s1";
			 *	                                                                      // s2: "s2";
			 *	lm = var().set_map("");                                               // s3: "...";
			 *	lm = var().map_set("v1:1;v2:2;v3:3;s1:\"s1\";s2:\"s2\";s3:\"...\";"); // v1: 1;										         
			 *											                              // v2: 2;
			 *											                              // v3: 3;
			 *	-----
			 *	
			 *	notes about slashes '\':
			 *	
			 *  you have to use slashes for const char* literals (1)  and use 
			 *  map_fread()-rules ( see map_fread() [R] ) for hand made strings 
			 *  like: char*(null-terminated),std::string
			 * 
			 *  const char* literals:
			 *	                3+"            3+\
	         *  the rules are: \\\" for " and \\\\ for \
             *	                                                // ...
			 *	x.map_set("dir: \" D:\\\\music\\\\Accept \";"); // dir: " D:\music\Accept "; // (win) 
			 *	                                                // ...
			 *	                                                
			 *	x.map_set("txt: \" Saying \\\"Stop the car\\\" was a mistake \";");
			 *	// ...
			 *	// txt: " Saying "Stop the car" was a mistake ";
			 *	// ...
			 *                   3+3+\\           3+\+3+"
			 *  also possible: \\\\\\\\ -> \\ or \\\\\\\" -> \" and so on
			 *  
			 *  note: for using slashes in a file -> look: map_fread(cch *fname)
			 *  
			 */
			virtual var& map_set(cch *map);

			/** ->  var:P_map map_get(cch *map) [var:map]  <-
			 * 
			 * - getting values from the var:map 
			 * 
			 * param map -> const char* , char*(null-terminated)
			 * return    -> var:P_map (P_map -> pointer to another var:map or a value)
			 * note: you must use the dereference operator (*) to access data referenced
			 *       by the var:P_Map
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 *             > [ map_parsing_error ]
			 * ex:
			 *  var x;
			 *  x.map_set(""
			 *		    " { cars: { germany: { MercedesBenz: B_Class: 2; "
			 *		    "                                    C_Class: 3; "
			 *			"                    } "
			 *			"         } "
			 *			" } "                    
			 *			" { movies: { action: _1: \" No Time to Die \"; "
			 *			"           } "
			 *			" } ");
			 * 
			 * 	// good to use: (with checks)
			 *		
			 *  var w;
			 *		                                       // ...
			 *	if ((w = x.map_get("cars:germany"))) {     // { MercedesBenz: B_Class: 2;
			 *		(*w).prnt();  // print -->             //                 C_Class: 3;
			 *	} else var("fail").prnt();                 // }
			 *		                                       // ...
			 *		
			 *	if ((w = x.map_get("cars:germany:MercedesBenz:C_Class"))) {
			 *	
			 *		*w = 100;  // IV. direct map_set case
			 *		// or 
			 *		*w = " string "; 
			 *	
			 *	} else var("fail").prnt();
			 *		
			 *	if ((w = x.map_get("movies:action:_1"))) {
			 *	
			 *		(*w).push_back(":) :) :)"); // just changes value
			 *		
			 *	} else var("fail").prnt();
			 *              // { cars: { germany: { MercedesBenz: B_Class: 2;
			 *  x.prnt();   //                                    C_Class: 100; <-- reassigned
			 *              //                    }     
			 *              //         }               
			 *              // }
			 *              // { movies: { action: _1: " No Time to Die :) :) :)"; <-- changed
			 *              //           }
			 *              // }
			 *                
			 *   // excp:                              // excp: [ invalid_map_pointer ] [ var:P_map ]                         
			 *   w = x.map_get("movies: fiction:_1");  // at:   var& operator*()
			 *	 (*w).prnt();                          // dscr: field not exist or became invalid
			 *                                        
			 */
			virtual var map_get(cch *map) const;

			/** ->  var:arr map_rem(cch *map) [var:map]  <-
			 * 
			 * - remove value(s) from the var:map
			 * - pointers to map (var:P_map) may be invalidated
			 *  
			 * param map    -> const char* , char*(null-terminated)
			 * return ok    -> {1,0} ok
			 *        fail  -> {2,"map field hadn't exist"} 
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 *             > [ map_parsing_error ]
			 *             
			 * ex:
			 *  var x;
			 *  x.map_set(""
			 *		    " { cars: { germany: { MercedesBenz: B_Class: 2; "
			 *		    "                                    C_Class: 3; "
			 *		    "                    } "
			 *		    "                    Opel: 4; "
			 *		    "                    BMW: 5;  "
			 *		    "         } "
			 *		    "         { american: Dodge: 5;    "
			 *		    "                     Lincoln: 9;  "
			 *		    "                     Ford: 7;     "
			 *		    "         } "
			 *		    "         other: 6; "
			 *		    " } ");
			 *		    
			 *  I. removes a field 		    
			 *  
			 *  x.map_rem("cars:germany:MercedesBenz:C_Class");
			 *   
			 *   // { cars: { american: Dodge: 5;
			 *   //                     Ford: 7;
			 *   //                     Lincoln: 9;
			 *   //         }
			 *   //         { germany: BMW: 5;
			 *   //                    { MercedesBenz: B_Class: 2;
			 *   //                    }
			 *   //                    Opel: 4;
			 *   //         }
			 *   //         other: 6;
			 *   // }
			 *                       
			 *   II. removes a group of fields                 
			 *                                                  
			 *   x.map_rem("cars:american");
			 *   
			 *   // { cars: { germany: BMW: 5;
			 *   //                    { MercedesBenz: B_Class: 2;
			 *   //                    }
			 *   //                    Opel: 4;
			 *   //         }
			 *   //         other: 6;
			 *   // }                                             
			 *          
			 *   III. removes a complex group of fields     
			 *                                                  
			 *   x.map_rem("cars");
			 *   
			 *   // { }                                         
			 *                                                  
			 */
			virtual var map_rem(cch *map);

			/** ->  var:self& map_free() [var:map]  <-
			 * 
			 * - removes all map-fields from the current var:map
			 * - pointers to map (var:P_map) may be invalidated
			 * 
			 * return var:self&
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 * 
			 */
			virtual var& map_free();

			/** ->  void foreach(var::map (*fun)(char *field, const var &val, void *ctx), 
			 *                        void ctx = 0, int depth = 0)  [var:map]  <-
			 * 
			 * - iterations on the var:map fields (main purpose is collection of information)
			 *  
			 * param fun  -> function that gets data:
			 *            -> var::map (*fun)(char *field, const var &val, void *ctx) <-
			 *               param field -> null terminated char-buffer
			 *               param val   -> value (var:X)
			 *               param ctx   -> context to use  <----------------------------.
			 *               return      -> var::map:                                    |
			 *                              var::map::nxt -> continue                    |
			 *                              var::map::brk -> break iteration             |
			 *                              var::map::skp -> skip this iteration         |
			 *                                               (actual only for var:map    |
			 *                                                                see ex*)   |
			 *                                                                           |
			 * param ctx    -> context to use (will be passed to fun) -------------------'
			 * param depth  -> limit of the depth
			 * return       -> void
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 *             > [ map_max_depth ]
			 *                    
			 * ex:
			 *  var x;
			 *  
			 *  //depth:     1        2             3            4 
			 *  x.map_set(""
			 *		    " { cars: { germany: { MercedesBenz: B_Class: 2; "
			 *		    "                                    C_Class: 3; "
			 *	        "                    } "
			 *		    "                    Opel: 4; "
			 *		    "                    BMW: 5;  "
			 *		    "         } "
			 *		    " } "
			 *	//depth:      1         2      3
			 *		    " { movies: { action: _1: \" No Time to Die \"; "
			 *		    "                     _2: \" Mission: Impossible - Fallout \"; "
			 *		    "                     _3: \"...\"; "
			 *		    "           } "
			 *		    " } "
			 *		    " val: 123; ");
			 *  
			 *  struct CTX {
			 *	    int tot = 0;
			 *	    // ...
			 *  } _ctx;
			 *  
			 *  // -----                                             iterations order: 
			 *                                                   
			 *  x.map_foreach([](auto field, auto &val, auto ctx) {  // cars         |
			 *	                                                     // germany      |
			 *      CTX *_ctx_ = (CTX*) ctx; // explicit cast        // BMW          |
			 *                                                       // MercedesBenz |
			 *      ++_ctx_->tot;            // doing the work       // B_Class      |
			 *	    var(field).prnt();                               // C_Class      |
			 *	                                                     // Opel         |
			 *	    return var::map::nxt;    // continue             // movies       |
			 *	                                                     // action       |
			 *  },&_ctx);                                            // _1           |
			 *  //   ^ pass the context pointer                      // _2           |
			 *                                                       // _3           |
			 *                                                       // val          V
			 *  // -----
			 *                                                    
			 *  x.map_foreach([](auto field, auto val, auto ctx) {   // cars         |
			 *	                                                     // germany      |
			 *	    var(field).prnt();                               // movies       |
			 *	    return var::map::nxt;                            // action       |
			 *	                                                     // val          V
			 *	},0, 2);     
			 *  //   ^ set the depth
			 *  
			 *  // ----- ex*  if you really want to change value while iterating you need 
			 *                to drop 'const' i.e. const_cast<var&>(val).do_something()
			 *                ( it's safe, but (!) )
			 *  
			 * 	x.map_foreach([](auto field, auto &val, auto ctx) {
			 *	
			 *	    if (val.is_map()) {
			 *	    	const_cast<var&>(val) =	 //
			 *		          var().map_set("{new_field: pi: 3.141593; e: 2.718282;}");
			 *		    //
			 *		    // (!)
			 *		    // when you set one var:map to another var:map(current) you can get 
			 *		    // 'infinite recursion' and catch [ map_max_depth ]
			 *		    // (because 'foreach' will go into newly created var:map etc) 
			 *		    // so you need to use var::map::skp to skip the newly created var:map
			 *		    // or take control of it by yourself
			 *		    //
			 *		    // you can also skip (by var::map::skp) the current var:map if
			 *		    // it doesn't make sense to go into it
			 *		    //
			 *		    return var::map::skp; 
			 *	     }
			 *	     
			 *	     // other cases
			 *	
			 *	     const_cast<var&>(val) = 777;
			 *	     return var::map::nxt;
			 *	
			 *  },&_ctx);
			 *  //            reassigned
			 *  //                ^
			 *  //                |
			 *  // { cars: { new_field: e: 2.718282;
			 *  //                |     pi: 3.141593;
			 *  //         }      |
			 *  // }              |
			 *  // { movies: { new_field: e: 2.718282;
			 *  //                        pi: 3.141593;
			 *  //           }
			 *  // }
			 *  // val: 777; <-- reassigned
			 * 
			 */
			virtual void map_foreach(var::map (*fun)(char *field,const var &val, void *ctx),			//
					void *ctx = 0, int depth = 0) const;

			/** ->  var:arr map_fread(cch *fname) [var:map]  <-
			 * 
			 * - getting data from the file with name 'fname' and parse it to the var:map
			 * - not thread safe (at the moment)
			 * - pointers to map (var:P_map) may be invalidated (if make changes)
			 * 
			 * param fname -> const char* , char*(null-terminated)
			 * return ok   -> {1,0}
			 *        fail -> {0,error string}
			 *        
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 *             > [ map_parsing_error ]
			 *             > [ map_parsing:string_parsing_error ]
			 * 
			 * ex:
			 *  file: map.tst
			 * 
			 *  { cars: { germany: { MercedesBenz: B_Class: 2;
			 *                                     C_Class: 3;
			 *                     }
			 *                     Opel: 4;
			 *                     BMW: 5;
			 *           }
			 *           { american: Dodge: 5;
			 *                       Lincoln: 9;
			 *                       Ford: 7;
			 *           }
			 *           other: 6;  //  .................. (1)
			 *  }                   //  |                |
			 *  { movies: { action: _1: " No Time to Die ";
			 *                      _2: " Mission: Impossible - Fallout ";
			 *                      _3: "...";
			 *            }
			 *            { fiction: _1: " Edge Of Tomorrow ";
			 *                       _2: " The Martian ";
			 *                       _3: "...";
			 *            }
			 *            undefined:;
			 *  }
			 *  { math: pi: 3.141593;
			 *          e:  2.718282;
			 *  }
			 *  temperature: -20;
			 *  
			 *  // good to use: (with checks)
			 *  
			 *  var x, w;
			 *  
			 *  if((w = x.map_fread("bad_map_name.tst"))[0]){
			 *     x.prnt();
			 *  } else w[1].prnt(); // err: Unable to open file
			 *  
			 *  if((w = x.map_fread("map.tst"))[0]){
			 *     x.prnt(); // ok ->  now it can be used like var:map
			 *  } else w[1].prnt();
			 *  
			 *  notes about slashes '\':
			 *                                  
			 *  no need to use slashes for string values (1)
			 *                1+"           1+\
	         *  the rules are: \" for " and \\ for \  [R]
			 *                1+\+1+\       1+\+1+"
			 *  also possible: \\\\ -> \\ or \\\" -> \" and so on
			 * 
			 */
			virtual var map_fread(cch *fname);

			/** ->  var:arr map_fwrite(cch *fname) [var:map]  <-
			 * 
			 * NOTE: - all var:I_str,var:Ir_str,var:I_arr,var:Ir_arr,var:P_map 
			 *         will be defined as var::udf (undefined)
			 *         (because of nothing to store)
			 *       - var:arr (array) also will be defined as var::udf (at the moment)
			 *       ! but you can freely use them at run time
			 * 
			 * - writes data to the file with name 'fname' 
			 * - not thread safe (at the moment)
			 * 
			 * note: the rule is: one var:map -> one file
			 * 
			 * param fname -> const char* , char*(null-terminated)
			 * return ok   -> {1,0}
			 *        fail -> {0,error string}
			 *        
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 * 
			 * ex:
			 *  var x, w;
			 *  x.map_set(""
			 *		    " { cars: { germany: { MercedesBenz: B_Class: 2; "
			 *		    "                                    C_Class: 3; "
			 *			"                    } "
			 *			"         } "
			 *			" } "                    
			 *			" { movies: { action: _1: \" No Time to Die \"; "
			 *			"           } "
			 *			" } "); 
			 *  
			 *  if((w = x.map_fwrite("map.tst"))[0]){
			 *     var("ok").prnt();
			 *  } else w[1].prnt(); // err-string
			 * 
			 */
			virtual var map_fwrite(cch *fname) const;

			/** -> var:str map_str(bool format = 0)  <-
			 * 
			 * NOTE: - all var:I_str,var:Ir_str,var:I_arr,var:Ir_arr,var::P_map 
			 *         will be defined as var::udf (undefined)
			 *         (because of nothing to store)
			 *       - var:arr (array) also will be defined as var::udf (at the moment)
			 *       ! but you can freely use them at run time
			 * 
			 * - makes special 'map-string' that can be easily parse back to the var:map
			 *   
			 * on my opinion:
			 * 'map-string' can be used: for network transfer, in files defined by user,
			 * in databases, other ways ... 
			 * 
			 * param format -> 0 - no format (no spaces,no newlines) 1 - otherwise
			 * return       -> var:str
			 *        
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ] 
			 *             
			 * ex:
			 *  var x, s;
			 *  x.map_set(""
			 *          " { cars: { germany: { MercedesBenz: B_Class: 2; "
			 *          "                                    C_Class: 3; "
			 *          "                    } "
			 *          "                    Opel: 4; "
			 *          "                    BMW: 5;  "
			 *          "          } "
			 *          "          { american: Dodge: 5;    "
			 *          "                      Lincoln: 9;  "
			 *          "                      Ford: 7;     "
			 *          "          } "
			 *          "          str: \" Saying \\\"Stop the car\\\" \"; "
			 *          "          udf:; "
			 *          " } ");
			 *                     // {cars:{american:Dodge:5;Ford:7;Lincoln:9;}{germany:BMW:5;
			 *   s = x.map_str();  // {MercedesBenz:B_Class:2;C_Class:3;}Opel:4;}str:" Saying \"Stop
			 *	 s.prnt();         // the car\" ";udf:;}
			 *                     
			 *   s = x.map_str(true); 
			 *   s.prnt();         // formatted output (with spaces and newlines)
			 * 
			 */
			virtual var map_str(bool format = 0) const;

			/** -> bool  map_equal(const var:map& map) [var:map]  <-
			 *
			 * return -> true  if maps are equal 
			 *           false otherwise
			 *                
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 *             
			 * ex:
			 *  var m1, m2, x;
			 *  m1.map_set(""
			 *	         " { cars: { germany: MercedesBenz: 6; "
			 *	         "                    Opel: 4;         "
			 *	         "                    BMW: 5;          "
			 *	         "         }"
			 *	         " }");
			 *  m2 = m1;
			 *
			 *  x = m1.map_equal(m2); // true
			 *	
			 *  m2.map_set("cars:germany:Opel:777;");
			 *  x = m1.map_equal(m2); // false 
			 *             
			 */
			virtual bool map_equal(const var& map) const;

			/** ->  cch* data() [var:str]  <-
			 *
			 * return -> the path and pair of elements where the mismatch occurred
			 *           false otherwise
			 *    
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]     
			 * 
			 * ex:
			 *  var m1, m2, m3, m4, x;
			 *  m1.map_set(""
			 *	         " { cars: { germany: MercedesBenz: 6; "
			 *	         "                    Opel: 4;         "
			 *	         "                    BMW: 5;          "
			 *	         "         }"
			 *	         " }");
			 *	         
			 *  m3.map_set(""
			 *	         " { cars: { germany: MercedesBenz: 6; "
			 *	         "                    Opel: 4;         "
			 *	         "         }"
			 *	         " }");
			 *	         
			 *  m4.map_set(""
			 *	         " { cars: { germany: MercedesBenz: 6; "
			 *	         "                    Opel: 4;         "
			 *	         "                    BMW_x: 5;        "
			 *	         "         }"
			 *	         " }");	     
			 *	     
			 *  m2 = m1;
			 *
			 *  no mismatch:
			 *  
			 *  x = m1.map_mismatch(m2); // false
			 *	 
			 *	value mismatch: 
			 *	 
			 *  m2.map_set("cars:germany:Opel:777;");   // [ 3 ]{ "cars:germany:Opel",
			 *  x = m1.map_mismatch(m2);                //        | [var:P_map] val: 4 |,
			 *                                          //        | [var:P_map] val: 777 | } 
			 *     
			 *  size mismatch:   
			 *                 
			 *  x = m1.map_mismatch(m3); // [ 3 ]{ "cars:germany",               
			 *                           // | [var:P_map] val: BMW:5;MercedesBenz:6;Opel:4; |,
			 *                           // | [var:P_map] val: MercedesBenz:6;Opel:4; | }
			 * 
			 *  field mismatch:
			 *  
			 *  x = m1.map_mismatch(m4); // [ 3 ]{ "cars:germany:BMW",           
			 *                           // | [var:P_map] val: 5 |,
			 *                           // | [var:P_map] is invalid | } 
			 *                                               ^
			 *                                  it means that field "cars:germany:BMW" 
			 *                                  in 'm4' not exists
			 *  
			 *  ... 
			 *             
			 */
			virtual var map_mismatch(const var& map) const;
			
			//------------------------- string if: ---------------------------
			
			// Operations:
			
			/** ->  cch* data() [var:str]  <-
			 *
			 * return -> returns a pointer to a null-terminated 
			 *           character array with data equivalent to those 
			 *           stored in the string  
			 *                
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]  
			 *             
			 */
			virtual cch* data() const;

			/** ->  var:arr split(char sep) [var:str]  <-
			 *          
			 * param sep (separator) -> any char
			 * return -> split var:str by 'sep'arator and returns var:arr
			 *   else -> var:udf  
			 * 
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]            
			 * ex: 
			 *  var s = "A-BC-D", a;
			 *  a = s.split('-');  // "{'A',"BC",'D'}"
			 *  
			 */
			virtual var split(char sep) const;

			/** ->  var:self& s(elf)split(char sep) [var:str]  <-
			 *
			 * - self-transforms of splitted var:str -> to var:arr
			 * - source var:str will be destroyed (if no references)
			 * - iterators may be invalidated
			 *
			 * param sep (separator) -> any char
			 * return                -> var:self&   
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]             
			 * ex:
			 *  var s = "H E LL  O";
			 *  s.ssplit(' ');    // "{'H','E',"LL",'O'}"
			 *  
			 */
			virtual var& ssplit(char sep);

			/** ->  var:self& ltrim() [var:str]  <-
			 *
			 * - trimming left-side spaces
			 * - iterators may be invalidated
			 *
			 * return -> var:self&   
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]            
			 * ex: 
			 *  var s = "  ! ltrim";
			 *  s.ltrim();   // "! ltrim"
			 *  
			 */
			virtual var& ltrim();

			/** ->  var:self& rtrim() [var:str]  <-
			 *
			 * - trimming right-side spaces
			 * - iterators may be invalidated
			 *
			 * return -> var:self&   
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]            
			 * ex: 
			 *  var s = "rtrim !  ";
			 *  s.rtrim();   // "rtrim !"
			 *  
			 */
			virtual var& rtrim();

			/** ->  var:self& trim() [var:str]  <-
			 *
			 * - trimming left/right-side spaces
			 * - iterators may be invalidated
			 *
			 * return -> var:self&   
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]              
			 * ex:
			 *  var s = "  ! trim !  ";
			 *  s.trim();    // "! trim !"
			 *  
			 */
			virtual var& trim();

			/** -> var:I_str insert(ullong index, cch* src, ullong src_index,
			 *                                            ullong src_count) [var:str]  <-
			 *
			 * - special to link raw-data with var:str
			 * - iterators may be invalidated
			 *
			 * param index     -> position at which the content will be inserted
			 * param src       -> source: char* , cch*
			 * param src_index -> source content start index
			 * param src_count -> number of characters to insert
			 *  
			 * note: control of the range of a source (and source itself)
			 *       is user responsibility
			 *  
			 * return -> var:I_str (set to index)      
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]  
			 *             > [ std::exception ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  char ca[] = { 'a', 'b', 'c', 'd' };
			 *	char *pca = ca;
			 *	cch *cs = "hello";  
			 *	var itr;
			 *	itr = s.insert(2, cs, 1, 2);   // "! el  !"
			 *	itr = s.insert(2, pca, 1, 2);  // "! bcel  !"
			 *  
			 */
			virtual var insert(ullong index, cch *src, ullong src_index, ullong src_count);

			/** ->  var:self& append(char ch, ullong count) [var:str]  <-
			 *
			 * - appends 'count' copies of character 'ch'
			 * - iterators may be invalidated
			 *
			 * param ch    -> character value to append
			 * param count -> number of characters to append (default count=1)
			 * return      -> var:self&    
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]             
			 * ex:
			 *  var s = "string";
			 *  s.append('!', 3);  // "string!!!"
			 *  s.append('?');     // "string!!!?"
			 *  
			 */
			virtual var& append(char ch, ullong count = 1);

			/** ->  var:self& append(const var:str &src, ullong src_index = 0, 
			 *                                         ullong src_count = 0) [var:str]  <-
			 *
			 * - appends a substring [src_index, src_index + src_count) of 'src'
			 * - iterators may be invalidated 
			 *
			 * param src       -> source of the string to append
			 * param src_index -> the index of the first character to append (0 -> from [0])
			 * param src_count -> number of characters to append             (0 -> to end)   
			 * return          -> var:self&    
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *             > [ unsupported_type ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]                        
			 * ex:
			 *  var s = "string", apps="WXYZ";
			 *  s.append(apps);        // "stringWXYZ"
			 *  s = "string";
			 *  s.append(apps, 2, 2);  // "stringYZ"
			 *  
			 */
			virtual var& append(const var &src, ullong src_index = 0, ullong src_count = 0);

			/** ->  var:self& append(cch* src, ullong src_index = 0, 
			 *                                 ullong src_count = 0) [var:str]  <-
			 *
			 * - appends a substring [src_index, src_index + src_count) of 'src'
			 * - iterators may be invalidated 
			 *
			 * param src       -> source of the string to append 
			 * param src_index -> the index of the first character to append (0 -> from [0])
			 * param src_count -> number of characters to append             (0 -> to end)   
			 * return          -> var:self&    
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]                        
			 * ex:
			 *  var s = "string";
			 *  s.append("WXYZ");        // "stringWXYZ"
			 *  s = "string";
			 *  s.append("WXYZ", 2, 2);  // "stringYZ"
			 *  
			 */
			virtual var& append(cch *src, ullong src_index = 0, ullong src_count = 0);

			/** ->  var:self& append(str &src, ullong src_index = 0, 
			 *                                 ullong src_count = 0) [var:str]  <-
			 *
			 * - appends a substring [src_index, src_index + src_count) of 'src'
			 * - iterators may be invalidated 
			 *
			 * param src       -> source of the string to append 
			 * param src_index -> the index of the first character to append (0 -> from [0])
			 * param src_count -> number of characters to append             (0 -> to end)   
			 * return          -> var:self&    
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]                        
			 * ex:
			 *  var s = "string";
			 *  std::string S = "WXYZ";
			 *  s.append(S);        // "stringWXYZ"
			 *  s = "string";
			 *  s.append(S, 2, 2);  // "stringYZ"
			 *  
			 */
			virtual var& append(str &src, ullong src_index = 0, ullong src_count = 0);

			/** ->  var:self& append(const var:I(r)_str &src_first, const var:I(r)_str &src_last)
			 *                                                                      [var:str]  <-
			 *
			 * - appends characters in the range [src_first, src_last) 
			 * - iterators may be invalidated
			 *
			 * param src_first -> source start from  (inclusive)
			 * param src_last  -> source last        (not inclusive)
			 * return          -> var:self&    
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *             > [ bad_iterator ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]           
			 * ex:
			 *  var s = ">>>", ss = "WXYZ";
			 *  var b = ss.begin(), e = ss.end();
			 *  s.append(b, e);  // ">>>WXYZ"
			 *  b = ss.rbegin(), e = ss.rend();
			 *  s.append(b, e);  // ">>>WXYZZYXW"                    
			 *                          
			 */
			virtual var& append(const var &src_first, const var &src_last);

			/** -> var:self& replace(ullong index, ullong count, const var:str &src,
			 *                       ullong src_index = 0, ullong src_count = 0) [var:str]  <-
			 *
			 * - replaces the part of the string indicated by either [index, index + count)
			 *   with a new string ('src') [src_index, src_index + src_count). 
			 * - iterators may be invalidated
			 * 
			 * param index -> start of the substring that is going to be replaced  
			 * param count -> length of the substring that is going to be replaced
			 * param src   -> source of the substring to use for replacement
			 * param src_index -> start of the substring to replace with (0 -> from [0])
			 * param src_count -> number of characters to replace with   (0 -> to end)
			 * return          -> var:self&    
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ] 
			 *             > [ length_error ]
			 *             > [ unsupported_type ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]    
			 * ex:
			 *  var s = "string", ss = "STRING";
			 *  s.replace(1, 4, ss);       // "sSTRINGg"
			 *  s = "string";
			 *  s.replace(1, 4, ss, 2, 4); // "sRINGg"
			 *             
			 */
			virtual var& replace(ullong index, ullong count, const var &src, ullong src_index = 0, ullong src_count = 0);

			/** -> var:self& replace(ullong index, ullong count, cch* src, ullong src_index = 0,
			 *               	     ullong src_count = 0) [var:str]  <-
			 *
			 * - replaces the part of the string indicated by either [index, index + count)
			 *   with a new string ('src') [src_index, src_index + src_count). 
			 * - iterators may be invalidated
			 * 
			 * param index -> start of the substring that is going to be replaced  
			 * param count -> length of the substring that is going to be replaced
			 * param src   -> source of the substring to use for replacement
			 * param src_index -> start of the substring to replace with (0 -> from [0])
			 * param src_count -> number of characters to replace with   (0 -> to end)
			 * return          -> var:self&    
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ] 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]    
			 * ex:
			 *  var s = "string";
			 *  s.replace(1, 4, "STRING");       // "sSTRINGg"
			 *  s = "string";
			 *  s.replace(1, 4, "STRING", 2, 4); // "sRINGg"
			 *             
			 */
			virtual var& replace(ullong index, ullong count, cch *src, ullong src_index = 0, ullong src_count = 0);

			/** -> var:self& replace(ullong index, ullong count, str &src, ullong src_index = 0,
			 *               	     ullong src_count = 0) [var:str]  <-
			 *
			 * - replaces the part of the string indicated by either [index, index + count)
			 *   with a new string ('src') [src_index, src_index + src_count). 
			 * - iterators may be invalidated
			 * 
			 * param index -> start of the substring that is going to be replaced  
			 * param count -> length of the substring that is going to be replaced
			 * param src   -> source of the substring to use for replacement
			 * param src_index -> start of the substring to replace with (0 -> from [0])
			 * param src_count -> number of characters to replace with   (0 -> to end)
			 * return          -> var:self&    
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ] 
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]    
			 * ex:
			 *  var s = "string";
			 *  std::string ss = "STRING";
			 *  s.replace(1, 4, "ss);       // "sSTRINGg"
			 *  s = "string";
			 *  s.replace(1, 4, ss, 2, 4);  // "sRINGg"
			 *             
			 */
			virtual var& replace(ullong index, ullong count, str &src, ullong src_index = 0, ullong src_count = 0);

			/** ->  var:self& replace(ullong index, ullong count, const var:I(r)_str &src_first, 
			 *                                      const var:I(r)_str &src_last) [var:str]  <-
			 *
			 * - replaces the part of the string indicated by either [index, index + count)
			 *   with a new string [src_first, src_last) 
			 * - iterators may be invalidated
			 * 
			 * param index -> start of the substring that is going to be replaced  
			 * param count -> length of the substring that is going to be replaced
			 * param src_first -> source start from  (inclusive)
			 * param src_last  -> source last        (not inclusive)
			 * return          -> var:self&    
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ] 
			 *             > [ length_error ]
			 *             > [ bad_iterator ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]       
			 * ex:
			 *  var s = "string", ss = "STRING";
			 *  var b = ss.begin(), e = ss.end();          
			 *  s.replace(1, 3, b, e);  // "sSTRINGng"
			 *  b = ss.rbegin(), e = ss.rend();
			 *	s.replace(1, 3, b, e);  // "sGNIRTSng"
			 *  
			 */
			virtual var& replace(ullong index, ullong count, const var &src_first, const var &src_last);

			/** ->  int compare(const var:str &with) [var:str]  <-
			 *
			 * - compares two character sequences
			 * 
			 * param with -> other string to compare to
			 * 
			 * ------------------------------------------------------------------------------- [!]
			 * return: (int)        
			 * first, calculate the number of characters to compare, as if by 
			 * size_type len = std::min(s1.length(), s2.length()). 
			 * Then compare the sequences by calling Traits::compare(str1, str2, len). 
			 * For standard strings this function performs character-by-character lexicographical
			 * comparison. If the result is zero (the character sequences are equal so far), 
			 * then their sizes are compared as follows:        
			 *   
			 *          Condition	                               Result         	   Return value
			 * compare(str1, str2, len)  < 0                  str1 is less than str2	    <0
			 * compare(str1, str2, len) == 0  size1 < size2	  str1 is less than str2	    <0
			 *                                size1 == size2  str1 is equal to str2	         ​0​
			 *                                size1 > size2	  str1 is greater than str2     >0
			 * compare(str1, str2, len)  > 0	              str1 is greater than str2     >0
			 *          
			 * -----------------------------------------------------------------------------------
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ unsupported_type ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]               
			 * ex:
			 *  var s = "XXB", s1 = "XXA", s2 = "XXC", s3 = "XXB";
			 *  int rs;
			 *  rs = s.compare(s1);  // XXA < XXB  (1  A<B) 
			 *  rs = s.compare(s2);  // XXB < XXC  (-1 B<C)
			 *  rs = s.compare(s3);  // XXB = XXB  (0)
			 *  
			 */
			virtual int compare(const var &with) const;

			/** ->  int compare(cch *with) [var:str]  <-
			 *
			 * - compares two character sequences
			 * 
			 * param with -> other string to compare to
			 * return     -> return condition and values see in 'compare(const var &with)' [!]
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]               
			 * ex:
			 *  var s = "XXB";
			 *  int rs;
			 *  rs = s.compare("XXA");  // XXA < XXB  (1  A<B) 
			 *  rs = s.compare("XXC");  // XXB < XXC  (-1 B<C)
			 *  rs = s.compare("XXB");  // XXB = XXB  (0)
			 *  
			 */
			virtual int compare(cch *with) const;

			/** ->  int compare(str &with) [var:str]  <-
			 *
			 * - compares two character sequences
			 * 
			 * param with -> other string to compare to
			 * return     -> return condition and values see in 'compare(const var &with)' [!]
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]               
			 * ex:
			 *  var s = "XXB";
			 *  std::string s1 = "XXA", s2 = "XXC", s3 = "XXB";
			 *  int rs;
			 *  rs = s.compare(s1);  // XXA < XXB  (1  A<B) 
			 *  rs = s.compare(s2);  // XXB < XXC  (-1 B<C)
			 *  rs = s.compare(s3);  // XXB = XXB  (0)
			 *  
			 */
			virtual int compare(str &with) const;

			/** ->  int compare(ullong index, ullong count, const var:str &with,
			 *                                ullong with_index = 0, ullong with_count = 0) [var:str]  <-
			 *
			 * - compares two character substring sequences
			 * 
			 * param index -> index of the first character in this string to compare
			 * param count -> number of characters of this string to compare 
			 * param with  -> other string (var:str) to compare to
			 * param with_index -> index of the first character of the given string to compare (0 -> from [0])
			 * param with_count -> number of characters of the given string to compare         (0 -> to end)
			 * return  -> return condition and values see in 'compare(const var &with)' [!]
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *             > [ unsupported_type ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]             
			 * ex:
			 *  var s = "..XXB..", s1 = "..XXA..", s2 = "..XXC..", s3 = "..XXB..";
			 *  int rs;
			 *	rs = s.compare(2, 3, s1, 2, 3);  // ..XXA.. < ..XXB..  (1)
			 *	rs = s.compare(2, 3, s2, 2, 3);  // ..XXB.. < ..XXC..  (-1)
			 *	rs = s.compare(2, 3, s3, 2, 3);  // ..XXB.. = ..XXB..  (0)  
			 *  
			 */
			virtual int compare(ullong index, ullong count, const var &with, ullong with_index = 0,
					ullong with_count = 0) const;

			/** ->  int compare(ullong index, ullong count, cch *with,
			 *                                ullong with_index = 0, ullong with_count = 0) [var:str]  <-
			 *
			 * - compares two character substring sequences
			 * 
			 * param index -> index of the first character in this string to compare
			 * param count -> number of characters of this string to compare 
			 * param with  -> other string to compare to
			 * param with_index -> index of the first character of the given string to compare (0 -> from [0])
			 * param with_count -> number of characters of the given string to compare         (0 -> to end)
			 * return  -> return condition and values see in 'compare(const var &with)' [!]
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]             
			 * ex:
			 *  var s = "..XXB..";
			 *  int rs;
			 *	rs = s.compare(2, 3, "..XXA..", 2, 3);  // ..XXA.. < ..XXB..  (1)
			 *	rs = s.compare(2, 3, "..XXC..", 2, 3);  // ..XXB.. < ..XXC..  (-1)
			 *	rs = s.compare(2, 3, "..XXB..", 2, 3);  // ..XXB.. = ..XXB..  (0)  
			 *  
			 */
			virtual int compare(ullong index, ullong count, cch *with, ullong with_index = 0, ullong with_count = 0) const;

			/** ->  int compare(ullong index, ullong count, str &with,
			 *                                ullong with_index = 0, ullong with_count = 0) [var:str]  <-
			 *
			 * - compares two character substring sequences
			 * 
			 * param index -> index of the first character in this string to compare
			 * param count -> number of characters of this string to compare 
			 * param with  -> other string to compare to
			 * param with_index -> index of the first character of the given string to compare (0 -> from [0])
			 * param with_count -> number of characters of the given string to compare         (0 -> to end)
			 * return  -> return condition and values see in 'compare(const var &with)' [!]
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ length_error ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]             
			 * ex:
			 *  var s = "..XXB..";
			 *  std::string s1 = "..XXA..", s2 = "..XXC..", s3 = "..XXB..";
			 *  int rs;
			 *	rs = s.compare(2, 3, s1, 2, 3);  // ..XXA.. < ..XXB..  (1)
			 *	rs = s.compare(2, 3, s2, 2, 3);  // ..XXB.. < ..XXC..  (-1)
			 *	rs = s.compare(2, 3, s3, 2, 3);  // ..XXB.. = ..XXB..  (0)  
			 *  
			 */
			virtual int compare(ullong index, ullong count, str &with, ullong with_index = 0, ullong with_count = 0) const;

			/** ->  bool starts_with(const var:str/char &prefix) [var:str]  <-
			 *
			 * - checks if the string begins with the given 'prefix'
			 * 
			 * param prefix -> compare with
			 * return:      -> true if the string begins with the provided 'prefix', 
			 *                 false otherwise
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ unsupported_type ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:
			 *  var s = "string", c = 's', cs = "str";
			 *  bool rs;
			 *  rs = s.starts_with(c);   // true
			 *  rs = s.starts_with(cs);  // true  
			 *	
			 */
			virtual bool starts_with(const var &prefix) const;

			/** ->  bool starts_with(char prefix) [var:str]  <-
			 *
			 * - checks if the string begins with the given 'prefix'
			 * 
			 * param prefix -> compare with
			 * return:      -> true if the string begins with the provided 'prefix', 
			 *                 false otherwise
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:
			 *  var s = "string";
			 *  bool rs;
			 *  rs = s.starts_with('s');  // true 
			 *	
			 */
			virtual bool starts_with(char prefix) const;

			/** ->  bool starts_with(cch *prefix) [var:str]  <-
			 *
			 * - checks if the string begins with the given 'prefix'
			 * 
			 * param prefix -> compare with
			 * return:      -> true if the string begins with the provided 'prefix', 
			 *                 false otherwise
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:
			 *  var s = "string";
			 *  bool rs;
			 *  rs = s.starts_with("str");  // true 
			 *	
			 */
			virtual bool starts_with(cch *prefix) const;

			/** ->  bool ends_with(const var:str/char &suffix) [var:str]  <-
			 *
			 * - checks if the string ends with the given 'suffix'
			 * 
			 * param suffix -> compare with
			 * return:      -> true if the string ends with the provided 'suffix', 
			 *                 false otherwise
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ unsupported_type ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]        
			 * ex:
			 *  var s = "string", c = 'g', cs = "ing";
			 *  bool rs;
			 *  rs = s.ends_with(c);   // true
			 *  rs = s.ends_with(cs);  // true
			 *  
			 */
			virtual bool ends_with(const var &suffix) const;

			/** ->  bool ends_with(char suffix) [var:str]  <-
			 *
			 * - checks if the string ends with the given 'suffix'
			 * 
			 * param suffix -> compare with
			 * return:      -> true if the string ends with the provided 'suffix', 
			 *                 false otherwise
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]        
			 * ex:
			 *  var s = "string";
			 *  bool rs;
			 *  rs = s.ends_with('g');  // true
			 *  
			 */
			virtual bool ends_with(char suffix) const;

			/** ->  bool ends_with(cch *suffix) [var:str]  <-
			 *
			 * - checks if the string ends with the given 'suffix'
			 * 
			 * param suffix -> compare with
			 * return:      -> true if the string ends with the provided 'suffix', 
			 *                 false otherwise
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]        
			 * ex:
			 *  var s = "string";
			 *  bool rs;
			 *  rs = s.ends_with("ing");  // true
			 *  
			 */
			virtual bool ends_with(cch *suffix) const;

			/** ->  var:s substr(ullong index = 0, ullong count = 0) [var:str]  <-
			 *
			 * - returns a substring [index, index + count)
			 * - iterators may be invalidated
			 * 
			 * param index -> index of the first character to include (0 -> from [0])
			 * param count -> length of the substring                 (0 -> to end)
			 * return:     -> var:str (substring)
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]        
			 * ex:
			 *  var s = "string", subs;
			 *  subs = s.substr();      // "string" 
			 *  subs = s.substr(1, 3);  // "tri"
			 *  
			 */
			virtual var substr(ullong index = 0, ullong count = 0);

			/** ->  ullong copy(char *dest, ullong index = 0, ullong count = 0, 
			 *                                               bool zero = false) [var:str]  <-
			 *
			 * - copies a substring [index, index + count) to the buffer pointed to by 'dest'
			 * 
			 * param dest  -> pointer (char*) to the destination buffer
			 * param index -> index of the first character to include   (0 -> from [0])
			 * param count -> length of the substring                   (0 -> to end)
			 * param zero  -> true -> add 0 to the end (c++ string)
			 * return:     -> number of characters copied
			 *     
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]
			 *       
			 * note: control of the destination buffer size is the responsibility of the user            
			 *             
			 * ex:
			 *  char buf[255];
			 *  int tot;
			 *  tot = s.copy(buf);          // buf: string????...
			 *  tot = s.copy(buf, 1, 3, 1); // buf: tri(0) <- zero set 
			 *  
			 */
			virtual ullong copy(char *dest, ullong index = 0, ullong count = 0, bool zero = false) const;

			// Search: 
			
			/** ->  var:I_str find(const var:str/char &src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the first substring equal to the given character sequence 'src'
			 * 
			 * param src   -> source of the data to search for
			 * param index -> index at which to start the search  (0 -> from [0])
			 * return      -> var:I_str to the first character of the found substring
			 *                or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ unsupported_type ] 
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "string", fs = "in", fc = 't', itr;
			 *  itr = s.find(fs);    // 'i' at [3]
			 *  itr = s.find(fc);    // 't' at [1] 
			 *  
			 */
			virtual var find(const var &src, ullong index = 0) const;

			/** ->  var:I_str find(char src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the first substring equal to the given character 'src'
			 * 
			 * param src   -> source of the data to search for
			 * param index -> index at which to start the search  (0 -> from [0])
			 * return      -> var:I_str to the first character of the found substring
			 *                or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "string", itr;
			 *  itr = s.find('t');   // 't' at [1]
			 *  
			 */
			virtual var find(char src, ullong index = 0) const;

			/** ->  var:I_str find(cch *src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the first substring equal to the given character sequence 'src'
			 * 
			 * param src   -> source of the data to search for
			 * param index -> index at which to start the search  (0 -> from [0])
			 * return      -> var:I_str to the first character of the found substring
			 *                or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "string", var itr;
			 *  itr = s.find("in");   // 'i' at [3]
			 *  
			 */
			virtual var find(cch *src, ullong index = 0) const;

			/** ->  var:I_str find(str &src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the first substring equal to the given character sequence 'src'
			 * 
			 * param src   -> source of the data to search for
			 * param index -> index at which to start the search  (0 -> from [0])
			 * return      -> var:I_str to the first character of the found substring
			 *                or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "string", itr;
			 *  std::string S = "in";
			 *  itr = s.find(S);   // 'i' at [3]
			 *  
			 */
			virtual var find(str &src, ullong index = 0) const;

			/** ->  var:I_str rfind(const var:str/char &src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the last substring equal to the given character sequence 'src'
			 *   search begins at 'index', i.e. the found substring must not begin in
			 *   a position following 'index'
			 * 
			 * param src   -> source of the data to search for
			 * param index -> index at which to start the search  (0 -> from [length()-1])
			 * return      -> var:I_str to the first character of the found substring
			 *                or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ unsupported_type ] 
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]    
			 * ex:   
			 *  var s = "This is a string", fs = "is", fc = 'a', itr;
			 *  itr = s.rfind(fs);      // "is" at [5]
			 *  itr = s.rfind(fs, 4);   // "is" at [2]
			 *  itr = s.rfind(fc);      // 'a'  at [13]
			 *  
			 */
			virtual var rfind(const var &src, ullong index = 0) const;

			/** ->  var:I_str rfind(char src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the last substring equal to the given character 'src'
			 *   search begins at 'index', i.e. the found substring must not begin in
			 *   a position following 'index'
			 * 
			 * param src   -> source of the data to search for
			 * param index -> index at which to start the search  (0 -> from [length()-1])
			 * return      -> var:I_str to the first character of the found substring
			 *                or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]    
			 * ex:   
			 *  var s = "This is a string", itr;
			 *  itr = s.rfind('a');   // 'a'  at [13]
			 *  
			 */
			virtual var rfind(char src, ullong index = 0) const;

			/** ->  var:I_str rfind(cch *src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the last substring equal to the given character sequence 'src'
			 *   search begins at 'index', i.e. the found substring must not begin in
			 *   a position following 'index'
			 * 
			 * param src   -> source of the data to search for
			 * param index -> index at which to start the search  (0 -> from [length()-1])
			 * return      -> var:I_str to the first character of the found substring
			 *                or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]    
			 * ex:   
			 *  var s = "This is a string", itr;
			 *  itr = s.rfind("is");   // "is" at [5]
			 *  
			 */
			virtual var rfind(cch *src, ullong index = 0) const;

			/** ->  var:I_str rfind(str &src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the last substring equal to the given character sequence 'src'
			 *   search begins at 'index', i.e. the found substring must not begin in
			 *   a position following 'index'
			 * 
			 * param src   -> source of the data to search for
			 * param index -> index at which to start the search  (0 -> from [length()-1])
			 * return      -> var:I_str to the first character of the found substring
			 *                or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]    
			 * ex:   
			 *  std::string fs = "is";
			 *  var s = "This is a string", itr;
			 *  itr = s.rfind("is");   // "is" at [5]
			 *  
			 */
			virtual var rfind(str &src, ullong index = 0) const;

			/** ->  var:I_str find_first_of(const var:str/char &src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the first character equal to one of the characters in
			 *   the given character sequence 'src'
			 * 
			 * param src   -> source of the data to identifying characters to search for 
			 * param index -> index at which to begin searching  (0 -> from [0])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_type ] 
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]         
			 * ex:   
			 *  var s = "Hello World!", fs = "dW", fc = 'r', itr;
			 *  itr = s.find_first_of(fs);  // 'W' at [6]
			 *  itr = s.find_first_of(fc);  // 'r' at [8]
			 *  
			 */
			virtual var find_first_of(const var &src, ullong index = 0) const;

			/** ->  var:I_str find_first_of(char src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the first character equal to the given character 'src'
			 * 
			 * param src   -> source of the data to identifying characters to search for 
			 * param index -> index at which to begin searching  (0 -> from [0])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]         
			 * ex:   
			 *  var s = "Hello World!", itr;
			 *  itr = s.find_first_of('l', 4);  // 'l' at [9]
			 *  
			 */
			virtual var find_first_of(char src, ullong index = 0) const;

			/** ->  var:I_str find_first_of(cch *src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the first character equal to one of the characters in
			 *   the given character sequence 'src'
			 * 
			 * param src   -> source of the data to identifying characters to search for 
			 * param index -> index at which to begin searching  (0 -> from [0])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]         
			 * ex:   
			 *  var s = "Hello World!", itr;
			 *  itr = s.find_first_of("dW");  // 'W' at [6]
			 *  
			 */
			virtual var find_first_of(cch *src, ullong index = 0) const;

			/** ->  var:I_str find_first_of(str &src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the first character equal to one of the characters in
			 *   the given character sequence 'src'
			 * 
			 * param src   -> source of the data to identifying characters to search for 
			 * param index -> index at which to begin searching  (0 -> from [0])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]         
			 * ex:   
			 *  std::string fs = "or";
			 *  var s = "Hello World!", itr;
			 *  itr = s.find_first_of(fs);  // 'o' at [4] 
			 *  
			 */
			virtual var find_first_of(str &src, ullong index = 0) const;

			/** ->  var:I_str find_first_not_of(const var:str/char &src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the first character equal to none of the characters 
			 *   in the given character sequence 'src'
			 * 
			 * param src   -> source of the data identifying characters to search for
			 * param index -> index at which to begin searching  (0 -> from [0])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ unsupported_type ] 
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "Hello World!", fs = "olleH ", fc = 'H', itr;
			 *  itr = s.find_first_not_of(fs);  // 'W' at [6]
			 *  itr = s.find_first_not_of(fc);  // 'e' at [1]
			 *  
			 */
			virtual var find_first_not_of(const var &src, ullong index = 0) const;

			/** ->  var:I_str find_first_not_of(char src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the first character not equal to the given character 'src'
			 * 
			 * param src   -> source of the data identifying characters to search for
			 * param index -> index at which to begin searching  (0 -> from [0])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "Hello World!", itr;
			 *  itr = s.find_first_not_of('H');  // 'e' at [1]
			 *  
			 */
			virtual var find_first_not_of(char src, ullong index = 0) const;

			/** ->  var:I_str find_first_not_of(cch *src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the first character equal to none of the characters 
			 *   in the given character sequence 'src'
			 * 
			 * param src   -> source of the data identifying characters to search for
			 * param index -> index at which to begin searching  (0 -> from [0])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "Hello World!", itr;
			 *  itr = s.find_first_not_of("olleH ");  // 'W' at [6]
			 *  
			 */
			virtual var find_first_not_of(cch *src, ullong index = 0) const;

			/** ->  var:I_str find_first_not_of(str &src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the first character equal to none of the characters 
			 *   in the given character sequence 'src'
			 * 
			 * param src   -> source of the data identifying characters to search for
			 * param index -> index at which to begin searching  (0 -> from [0])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "Hello World!", itr;
			 *  std::string fs = "olleH ";
			 *  itr = s.find_first_not_of(fs);  // 'W' at [6]
			 *  
			 */
			virtual var find_first_not_of(str &src, ullong index = 0) const;

			/** ->  var:I_str find_last_of(const var:str/char &src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the last character equal to one of characters in 
			 *   the given character sequence 'src'
			 * 
			 * param src   -> source of the data identifying characters to search for
			 * param index -> index at which the search is to finish  (0 -> from [length()-1])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ unsupported_type ] 
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "/root/config", itr, fs = "rOOT", fc = '/';
			 *  itr = s.find_last_of(fs);          // 'r' at [1]
			 *  itr = s.find_last_of(fc);          // '/' at [5]
			 *	s.substr(itr.pos() + 1).prnt();    // "config"
			 *  
			 */
			virtual var find_last_of(const var &src, ullong index = 0) const;

			/** ->  var:I_str find_last_of(char src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the last character equal to the given character 'src'
			 * 
			 * param src   -> source of the data identifying characters to search for
			 * param index -> index at which the search is to finish  (0 -> from [length()-1])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "/root/config", itr;
			 *  itr = s.find_last_of('/');       // '/' at [5]
			 *	s.substr(itr.pos() + 1).prnt();  // "config"
			 *  
			 */
			virtual var find_last_of(char src, ullong index = 0) const;

			/** ->  var:I_str find_last_of(cch *src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the last character equal to one of characters in 
			 *   the given character sequence 'src'
			 * 
			 * param src   -> source of the data identifying characters to search for
			 * param index -> index at which the search is to finish  (0 -> from [length()-1])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "/root/config", itr;
			 *  itr = s.find_last_of("rOOT");  // 'r' at [1]
			 *  
			 */
			virtual var find_last_of(cch *src, ullong index = 0) const;

			/** ->  var:I_str find_last_of(str &src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the last character equal to one of characters in 
			 *   the given character sequence 'src'
			 * 
			 * param src   -> source of the data identifying characters to search for
			 * param index -> index at which the search is to finish  (0 -> from [length()-1])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "/root/config", itr;
			 *  std::string fs = "rOOT";
			 *  itr = s.find_last_of(fs);  // 'r' at [1]
			 *  
			 */
			virtual var find_last_of(str &src, ullong index = 0) const;

			/** ->  var:I_str find_last_not_of(const var:str/char &src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the last character equal to none of the characters in
			 *   the given character sequence 'src'
			 * 
			 * param src   -> source of the data identifying characters to search for
			 * param index -> index at which to begin searching  (0 -> from [length()-1])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *             > [ unsupported_type ] 
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "Hello World", itr, fs = " World", fc = 'd';
			 *  itr = s.find_last_not_of(fs);  // 'e' at [1]
			 *  itr = s.find_last_not_of(fc);  // 'l' at [9]
			 *  
			 */
			virtual var find_last_not_of(const var &src, ullong index = 0) const;

			/** ->  var:I_str find_last_not_of(char src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the last character not equal to the given character 'src'
			 * 
			 * param src   -> source of the data identifying characters to search for
			 * param index -> index at which to begin searching  (0 -> from [length()-1])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "Hello World", itr;
			 *  itr = s.find_last_not_of('l');  // 'd' at [10]
			 *  
			 */
			virtual var find_last_not_of(char src, ullong index = 0) const;

			/** ->  var:I_str find_last_not_of(cch *src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the last character equal to none of the characters in
			 *   the given character sequence 'src'
			 * 
			 * param src   -> source of the data identifying characters to search for
			 * param index -> index at which to begin searching  (0 -> from [length()-1])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "Hello World", itr;
			 *  itr = s.find_last_not_of(" World");  // 'e' at [1]
			 *  
			 */
			virtual var find_last_not_of(cch *src, ullong index = 0) const;

			/** ->  var:I_str find_last_not_of(cch *src, ullong index = 0) [var:str]  <-
			 * 
			 * - finds the last character equal to none of the characters in
			 *   the given character sequence 'src'
			 * 
			 * param src   -> source of the data identifying characters to search for
			 * param index -> index at which to begin searching  (0 -> from [length()-1])
			 * return      -> var:I_str to the found character or var:udf as fail
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ out_of_range ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var s = "Hello World", itr;
			 *  std::string fs = " World";
			 *  itr = s.find_last_not_of(fs);  // 'e' at [1]
			 *  
			 */
			virtual var find_last_not_of(str &src, ullong index = 0) const;

			// Explicit conversions:
			
			/** ->  int _int() [var:str/var:char]  <-
			 * 
			 * - converts string to int
			 * return -> conversion result
			 * 
			 * note: careful with size of the receiver type
			 *       sizeof(receiver type) >= sizeof(intended size)
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ conversion_err ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var i = "123";
			 *	int _i = i._int();  // 123
			 *  
			 */
			virtual int _int() const;

			/** ->  llong _llong() [var:str/var:char]  <-
			 * 
			 * - convert string to llong
			 * return -> conversion result
			 * 
			 * note: careful with size of the receiver type
			 *       sizeof(receiver type) >= sizeof(intended size)
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ conversion_err ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var ll = "321";
			 *  llong _ll = ll._llong();  // 321
			 *  
			 */
			virtual llong _llong() const;

			/** ->  ullong _ullong() [var:str/var:char]  <-
			 * 
			 * - convert string to ullong
			 * return -> conversion result
			 * 
			 * note: careful with size of the receiver type
			 *       sizeof(receiver type) >= sizeof(intended size)
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ conversion_err ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]     
			 * ex:   
			 *  var ull = "6789";
			 *  ullong _ull = ull._ullong();  // 6789
			 *  ull = "-1";
			 *  _ull = ull._ullong();         // 18446744073709551615 (-1)
			 *  
			 */
			virtual ullong _ullong() const;

			/** ->  float _float() [var:str/var:char]  <-
			 * 
			 * - convert string to float
			 * return -> conversion result
			 * 
			 * note: careful with size of the receiver type
			 *       sizeof(receiver type) >= sizeof(intended size)
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ conversion_err ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var f = "0.555";
			 *  float _f = d._float();  // 0.555000007
			 *  
			 */
			virtual float _float() const;

			/** ->  double _double()  [var:str/var:char]  <-
			 * 
			 * - convert string to double
			 * return -> conversion result
			 * 
			 * note: careful with size of the receiver type
			 *       sizeof(receiver type) >= sizeof(intended size)
			 *             
			 * exceptions: catch(VarExcp &ex)
			 *             > [ conversion_err ]
			 *	           > [ unsupported_interface ]
			 *             > [ use_of_undefined_var ]   
			 * ex:   
			 *  var d = "0.555";
			 *  double _d = d._double();  // 0.55500000000000005
			 *  
			 */
			virtual double _double() const;

			/** ->  var:s _str()  [var:X]  <-
			 *  
			 * - convert current type to var:str (var:string)
			 *  
			 * return -> var:str                                 
			 *  
			 * exceptions: catch(VarExcp &ex)
			 *	           > [ unsupported_interface ]
			 * ex:
			 *  var i = 123, a = { 1, 2, 'c', "str", { 'a', 3, 2 } }, 
			 *  b = true, d = 0.555, udf;
			 *  var s;
			 *  s = i._str();         // "123"
			 *  s = a._str();         // "{1,2,'c',"str",{'a',3,2}}"
			 *  s = b._str();         // "true"
			 *  s = d._str();         // "0.555000"
			 *  s = udf._str();       // "|udf|"
			 *  s = var(777)._str();  // "777"
			 *   
			 */
			virtual var _str() const;

			//------------------------------------------------ IF-end
			
		private:

			virtual var* _mkcopy();
			template<typename t1, typename t2> //
	var* clc_prc(const ttyp<t1> *lv, t2 rv, int op, int m) const;
	llong clc_prc_spc(llong lv, llong rv, int op) const;
	template<typename t1, typename t2>//
	var* clc_prc_db(const ttyp<t1> *lv, t2 rv, int op, int m) const;
	template<typename t1, typename t2>//
	var* clc_prc_div_spc(const ttyp<t1> *lv, t2 rv, int m) const;
	inline var& sp_op_eq_chk(const var &rv);
	template<typename t> inline ttyp<t>* dcst(var *pttyp) const;
	void IE_solver(const var *x, var at, var msg = 0) const;
	void OE_solver(bool top, const var *x, const var *rv, var at, var msg = 0) const;
	virtual void setv(var &rv);
	virtual void setv(double rv);
	virtual void setv(llong rv);
	virtual void setv(bool rv);
	virtual void setv(char rv);
	virtual void setv(cch *rv);
	virtual void setv(const str &rv);
	virtual void setv(const __iniL <var> &rv);
	virtual void setv(const __vct <var> &rv);
	template<typename t> t* getPv(const var *pttyp) const;
	var idx_solver(ullong index) const;
	var* ret_bs();
	virtual char* get_poi() const;
};

class VarExcp: public std::exception {
	friend void GL_EX_solver(VarExcp &ex, var at, const var *rv);
private:
	var exc;
	var out;
public:
	VarExcp() noexcept;
	VarExcp(const var &_exc) noexcept {
		exc = _exc;
	}
	VarExcp(const var &_exc, bool top) noexcept {
		exc = _exc;
		if (_VAR_PRNT_EXC && top) var(what()).prnt();
	}
	~VarExcp();
	cch* what() const noexcept;

	static var _mk_excp(var excp_tp, std::exception &e, var msg = "");
	static var _mk_excp(var excp_tp, var msg = "");
	static var _mk_excp(const var *x, var excp_tp, var at, var msg);
	static var _mk_excp(var excp_tp, var at, var msg);
	static void _top_catch(const var *x, var at, VarExcp &e);
	static void _top_catch_unex(const var *x, var at);
	static void _un_ini_var(var at);
};

template<typename t> var operator+(const t &lv, const var &rv);
var operator+(cch *lv, const var &rv);
var operator+(char lv, const var &rv);
var operator+(const str &lv, const var &rv);
template<typename t> var operator-(const t &lv, const var &rv);
template<typename t> var operator/(const t &lv, const var &rv);
template<typename t> var operator*(const t &lv, const var &rv);

template<typename t> t& operator+=(t &lv, const var &rv);
str& operator+=(str &lv, const var &rv);
template<typename t> t& operator+=(const t &lv, const var &rv) {
	lv += t(rv); // [ error excitation ] --^
	return const_cast<t&>(lv);
}

template<typename t> t& operator-=(t &lv, const var &rv);
template<typename t> t& operator-=(const t &lv, const var &rv) {
	lv -= t(rv); // [ error excitation ] --^
	return const_cast<t&>(lv);
}

template<typename t> t& operator*=(t &lv, const var &rv);
template<typename t> t& operator*=(const t &lv, const var &rv) {
	lv *= t(rv); // [ error excitation ] --^
	return const_cast<t&>(lv);
}

template<typename t> t& operator/=(t &lv, const var &rv);
template<typename t> t& operator/=(const t &lv, const var &rv) {
	lv /= t(rv); // [ error excitation ] --^
	return const_cast<t&>(lv);
}

template<typename t> bool operator>(const t &lv, const var &rv);
bool operator>(cch *lv, const var &rv);
bool operator>(char lv, const var &rv);
bool operator>(const str &lv, const var &rv);

template<typename t> bool operator<(const t &lv, const var &rv);
bool operator<(cch *lv, const var &rv);
bool operator<(char lv, const var &rv);
bool operator<(const str &lv, const var &rv);

template<typename t> bool operator<=(const t &lv, const var &rv);
bool operator<=(cch *lv, const var &rv);
bool operator<=(char lv, const var &rv);
bool operator<=(const str &lv, const var &rv);

template<typename t> bool operator>=(const t &lv, const var &rv);
bool operator>=(cch *lv, const var &rv);
bool operator>=(char lv, const var &rv);
bool operator>=(const str &lv, const var &rv);

template<typename t> bool operator==(const t &lv, const var &rv);
bool operator==(cch *lv, const var &rv);
bool operator==(char lv, const var &rv);
bool operator==(const str &lv, const var &rv);

template<typename t> bool operator!=(const t &lv, const var &rv);
bool operator!=(cch *lv, const var &rv);
bool operator!=(char lv, const var &rv);
bool operator!=(const str &lv, const var &rv);

std::ostream& operator<<(std::ostream &os, const var &rv);
std::istream& operator>>(std::istream &is, const var &rv);

//#include"var.cpp" // comment out this when making static-lib

#endif
//
//   
//
//
