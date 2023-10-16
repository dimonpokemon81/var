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

/*
 * Any suggestions, improvements, are welcome, especially performance and code style.
 * 
 * P.S. I'm not a professional, this is my hobby :)
 * 
 * changed: 10.16.2023
 * 
 */ 

#include<random>
#include<fstream>

#include"var.h"  
#include"vmap.cpp"

//--------------------------- dbg

#define _VAR_show_cnstr_destr_n
#define _VAR_show_cnstr_destr_cnt

//------------------------------------------------------------ macros/define

#ifdef WIN32
#define _vendl "\r\n" 
#define __vendl \r\n
#else
#define _vendl '\n'
#define __vendl \n
#endif

#if defined(_VAR_DBG)
#if defined(_VAR_show_cnstr_destr_cnt)
int VAR_CD_TOP = 0;
int VAR_CD_TYP = 0;
int VAR_LI_CD = 0;
#define __VAR_C_TOP() VAR_CD_TOP++
#define __VAR_D_TOP() VAR_CD_TOP--
#define __VAR_C_TYP() VAR_CD_TYP++
#define __VAR_D_TYP() VAR_CD_TYP--
#define __VAR_LI_C() VAR_LI_CD++
#define __VAR_LI_D() VAR_LI_CD--
#else
#define __VAR_C_TOP()
#define __VAR_D_TOP()
#define __VAR_C_TYP()
#define __VAR_D_TYP()
#define __VAR_LI_C()
#define __VAR_LI_D()
#endif
#if defined(_VAR_show_cnstr_destr)
#define __cnstr_cll(x,xx) std::cout << x << xx << " "<< std::endl;
#else
#define __cnstr_cll(x,xx) 
#endif
//
#define __upd_val()	upd_val()
//
#else
// reset to empty
#define __VAR_C_TOP()
#define __VAR_D_TOP()
#define __VAR_C_TYP()
#define __VAR_D_TYP()
#define __VAR_LI_C()
#define __VAR_LI_D()
#define __cnstr_cll(x,xx)  
#define __upd_val()
#endif

//---------------------------------------------------- func/class hlp
//-------------------------------------------------------------------

struct var_li {
	
	struct idx {
		idx *prv = 0;
		idx *nxt = 0;
		var *val = 0;
		idx() {
			__VAR_LI_C();
		}
		~idx() {
			__VAR_LI_D();
		}
	};

	idx *fst = 0;
	idx *lst = 0;

	var_li() {
	}
	~var_li() {
		free();
	}
	
	idx* get() {
		if (fst == 0) {
			fst = lst = new idx();
			return fst;
		}
		idx *_new = new idx();
		lst->nxt = _new;
		_new->prv = lst;
		lst = _new;
		return lst;
	}
	
	void rem(idx *_idx) {
		if (_idx->prv && _idx->nxt) {
			_idx->prv->nxt = _idx->nxt;
			_idx->nxt->prv = _idx->prv;
			delete _idx;
		} else if (_idx->nxt) {
			fst = _idx->nxt;
			fst->prv = 0;
			delete _idx;
		} else if (_idx->prv) {
			_idx->prv->nxt = 0;
			lst = _idx->prv;
			delete _idx;
		} else { // fst==lst
			delete _idx;
			fst = lst = 0;
		}
	}
	
	void free() {
		idx *cur = fst, *_cur;
		while (cur) {
			_cur = cur;
			cur = cur->nxt;
			delete _cur;
		}
	}
};
template<typename T> void tcpps(T x, char *buf);
struct varF {
	
	static ullong cstrlen(cch *x) {
		ullong i = 0;
		while (*(x + i) != 0)
			++i;
		return i;
	}
	static ullong ccpy(char *buf, cch *src, int buf_spos = 0, bool end0 = 1) {
		int i = 0;
		while (*(src + i) != 0) {
			buf[buf_spos] = src[i];
			++i, ++buf_spos;
		}
		if (end0) {
			//++buf_spos;
			buf[buf_spos] = 0; // end-mark
		}
		//                            v
		return i; // point end -> ....0
	}
	static bool cmp(cch *a1, cch *a2) {
		int i = 0;
		char x, y;
		while (true) {
			x = *(a1 + i), y = *(a2 + i);
			if (x != y) return false;
			// x == y -> case
			else if (x == 0) return true; //end
			++i;
		}
		return false; // cch*-end-err
	}
	static int var_rnd(int min, int max) {

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(min, max);
		return dist(gen);
	}
	static void slsh(str &s1, str &s2) {
		//
		// s2 -> s1
		//
		ull i = 0, l = s2.length();
		cch *src = s2.data();
		//
		while (i < l) {
			if (src[i] == '\"') {
				s1 += "\\\"";
			} else if (src[i] == '\\') {
				s1 += "\\\\";
			} else s1 += src[i];
			i++;
		}
	}
};

//---------------------------------------------------- dbg (out)
//--------------------------------------------------------------------

dbg::dbg() {
}

//---------------------------------------------------- VarExcp (out)
//---------------------------------------------------------------------

VarExcp::VarExcp() noexcept {
}
VarExcp::~VarExcp() {
}

cch* VarExcp::what() const noexcept {
	
	const_cast<var&>(out) = var( { "\n  excp: [ ", exc[0], " ] ", //
			var(exc[1] ? var("[ var:") += exc[1] : ""), //
			exc[1] ? " ]" : "", (exc[2] ? var("\n  at:   ") += exc[2] : ""), //
			(exc[3] ? var("\n  dscr: ") += exc[3] : ""), //
			(exc[4] ? var("\n  std: ") += exc[4] : "") }).sjoin();
	
	return out.data();
}

var VarExcp::_mk_excp(var excp_tp, std::exception &e, var msg) {
	if (!excp_tp) excp_tp = "[ std::exception ]";
	return {excp_tp, 0, 0, msg, var(e.what())};
}
var VarExcp::_mk_excp(var excp_tp, var msg) {
	return {excp_tp, 0, 0, msg, 0};
}
var VarExcp::_mk_excp(const var *x, var excp_tp, var at, var msg) {
	return {excp_tp, x->gtid(),at, msg, 0};
}
var VarExcp::_mk_excp(var excp_tp, var at, var msg) {
	return {excp_tp, 0, at, msg, 0};
}
void VarExcp::_top_catch(const var *x, var at, VarExcp &e) {
	e.exc[1] = (x != 0 ? x->gtid() : "");
	e.exc[2] = at;
	throw VarExcp(e.exc, 1);
}
void VarExcp::_top_catch_unex(const var *x, var at) {
	throw VarExcp( { "unexpected_exception", (x ? x->gtid() : ""), at, 0, 0 }, 1);
}
void VarExcp::_un_ini_var(var at) {
	throw VarExcp( { "using_an_uninitialized_variable", "unknown", at, 0, 0 }, 1);
}

#define	__LL_exc() \
		if (rv > LLONG_MAX) throw VarExcp(VarExcp::_mk_excp("out_of_number_range", \
	    "'rv' > LLONG_MAX")); \
	   
#define	__LL_exc_p(par) \
	    if (par > LLONG_MAX) throw VarExcp(VarExcp::_mk_excp("out_of_number_range", \
	    "'"#par"' > LLONG_MAX")); \

//----------------------------------------------------------- var:str
//------------------------------------------------------------------

template<> class ttyp<str> : public var {
	
	friend class var;
	friend class ttyp<double> ;
	friend class ttyp<llong> ;
	friend class ttyp<bool> ;
	friend class ttyp<char> ;
	friend class ttyp<__vct<var>> ;
	friend class ttyp<Ic_str> ;
	friend class ttyp<Irc_str> ;
	friend class ttyp<VMAP> ;
	friend std::istream& operator>>(std::istream &is, const var &rv);

private:
	str val;
	cch *tid = "str";
	int isyn = 0;
	var_li itrs;
	var idx;

#ifdef _VAR_DBG 
	dbg ddbg;
#endif
	ttyp(const str &x) {
		__cnstr_cll("ttyp<str>(const str&): ", "");
		__VAR_C_TYP();
		val = x, set_cmm();
	}
	ttyp(str *x) {	//inner use
		__cnstr_cll("ttyp<str>(str*) ", "");
		__VAR_C_TYP();
		val = *x, set_cmm();
	}
	ttyp(cch *x) {
		__cnstr_cll("ttyp<str>(cch*)", "");
		__VAR_C_TYP();
		val = x;
		set_cmm();
	}
	ttyp(char *x) {
		__cnstr_cll("ttyp<str>(char*)", "");
		__VAR_C_TYP();
		val = str(x), set_cmm();
	}
	inline void set_cmm() {
		var::tid = tid;
#ifdef _VAR_DBG
		pdbg = &ddbg;
		upd_val();
#endif
	}
	
#ifdef _VAR_DBG
	inline void upd_val() {
		ddbg.sval = val.data();
	}
#endif
	~ttyp() {
		__cnstr_cll("~ttyp<str>()", "");
		__VAR_D_TYP();
		set_ivalid_itrs();
	}
	
	var operator+(const var &rv) const {
		if (rv.tid[0] == 's') return val + *getPv<str>(rv.pttyp);
		else if (rv.tid[0] == 'c') return val + *getPv<char>(rv.pttyp);
		else if (rv.tid[0] == 'i') return //
		val + std::to_string(*getPv<llong>(rv.pttyp));
		else if (rv.tid[0] == 'd') return //
		val + std::to_string(*getPv<double>(rv.pttyp));
		else if (rv.tid[0] == 'b') return //
		val + std::to_string(*getPv<bool>(rv.pttyp));
		else OE_solver(0, this, 0, 0, "unsupported 'rv' [ var:" + //
				var(rv.gtid()) += " ]");
		return var();
	}
	var operator+(int rv) const {
		return val + std::to_string(rv);
	}
	var operator+(float rv) const {
		return val + std::to_string(rv);
	}
	var operator+(double rv) const {
		return val + std::to_string(rv);
	}
	var operator+(long rv) const {
		return val + std::to_string(rv);
	}
	var operator+(llong rv) const {
		return val + std::to_string(rv);
	}
	var operator+(ullong rv) const {
		__LL_exc()
		return val + std::to_string(rv);
	}
	var operator+(cch *rv) const {
		return val + rv;
	}
	var operator+(str &rv) const {
		return val + rv;
	}
	var operator+(str *rv) const {
		return val + *rv;
	}
	var operator+(char rv) const {
		return val + std::to_string(rv);
	}
	
	var& operator+=(const var &rv) {
		if (rv.tid[0] == 's') {
			val += *getPv<str>(rv.pttyp);
			__upd_val();
		} else if (rv.tid[0] == 'c') {
			val += *getPv<char>(rv.pttyp);
			__upd_val();
		} else if (rv.tid[0] == 'i') {
			val += std::to_string(*getPv<llong>(rv.pttyp));
			__upd_val();
		} else if (rv.tid[0] == 'd') {
			val += std::to_string(*getPv<double>(rv.pttyp));
			__upd_val();
		} else if (rv.tid[0] == 'b') {
			val += std::to_string(*getPv<bool>(rv.pttyp));
			__upd_val();
		} else OE_solver(0, this, 0, 0, "unsupported 'rv' [ var:" + //
				var(rv.gtid()) += " ]");
		return *this;
	}
	var& operator+=(int rv) {
		val += std::to_string(rv);
		__upd_val();
		return *this;
	}
	var& operator+=(float rv) {
		val += std::to_string(rv);
		__upd_val();
		return *this;
	}
	var& operator+=(double rv) {
		val += std::to_string(rv);
		__upd_val();
		return *this;
	}
	var& operator+=(long rv) {
		val += std::to_string(rv);
		__upd_val();
		return *this;
	}
	var& operator+=(llong rv) {
		val += std::to_string(rv);
		__upd_val();
		return *this;
	}
	var& operator+=(ullong rv) {
		__LL_exc()
		val += std::to_string(rv);
		__upd_val();
		return *this;
	}
	var& operator+=(cch *rv) {
		val += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(str &rv) {
		val += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(str *rv) {
		val += *rv;
		__upd_val();
		return *this;
	}
	var& operator+=(char rv) {
		val += rv;
		__upd_val();
		return *this;
	}
	
	bool operator<(const var &rv) const {
		if (rv.tid[0] == 's') return val < *getPv<str>(rv.pttyp);
		else if (rv.tid[0] == 'c') return val[0] < *getPv<char>(rv.pttyp);
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator<(cch *rv) const {
		return val < rv;
	}
	bool operator<(str &rv) const {
		return val < rv;
	}
	bool operator<(str *rv) const {
		return val < *rv;
	}
	bool operator<(char rv) const {
		return val[0] < rv;
	}
	
	bool operator>(const var &rv) const {
		if (rv.tid[0] == 's') return val > *getPv<str>(rv.pttyp);
		else if (rv.tid[0] == 'c') return val[0] > *getPv<char>(rv.pttyp);
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator>(cch *rv) const {
		return val > rv;
	}
	bool operator>(str &rv) const {
		return val > rv;
	}
	bool operator>(str *rv) const {
		return val > *rv;
	}
	bool operator>(char rv) const {
		return val[0] > rv;
	}
	
	bool operator<=(const var &rv) const {
		if (rv.tid[0] == 's') return val <= *getPv<str>(rv.pttyp);
		else if (rv.tid[0] == 'c') return val[0] <= *getPv<char>(rv.pttyp);
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator<=(cch *rv) const {
		return val <= rv;
	}
	bool operator<=(str &rv) const {
		return val <= rv;
	}
	bool operator<=(str *rv) const {
		return val <= *rv;
	}
	bool operator<=(char rv) const {
		return val[0] <= rv;
	}
	
	bool operator>=(const var &rv) const {
		if (rv.tid[0] == 's') return val >= *getPv<str>(rv.pttyp);
		else if (rv.tid[0] == 'c') return val[0] >= *getPv<char>(rv.pttyp);
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator>=(cch *rv) const {
		return val >= rv;
	}
	bool operator>=(str &rv) const {
		return val >= rv;
	}
	bool operator>=(str *rv) const {
		return val >= *rv;
	}
	bool operator>=(char rv) const {
		return val[0] >= rv;
	}
	
	bool operator==(const var &rv) const {
		if (rv.tid[0] == 's') return val == *getPv<str>(rv.pttyp);
		else if (rv.tid[0] == 'c') return val[0] == *getPv<char>(rv.pttyp);
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator==(cch *rv) const {
		return val == rv;
	}
	bool operator==(str &rv) const {
		return val == rv;
	}
	bool operator==(str *rv) const {
		return val == *rv;
	}
	bool operator==(char rv) const {
		return val[0] == rv;
	}
	
	bool operator!=(const var &rv) const {
		if (rv.tid[0] == 's') return val != *getPv<str>(rv.pttyp);
		else if (rv.tid[0] == 'c') return val[0] != *getPv<char>(rv.pttyp);
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator!=(cch *rv) const {
		return val != rv;
	}
	bool operator!=(str &rv) const {
		return val != rv;
	}
	bool operator!=(str *rv) const {
		return val != *rv;
	}
	bool operator!=(char rv) const {
		return val[0] != rv;
	}
	
	bool operator!() const {
		return !bool(val.length());
	}
	
	var& operator[](ullong index) {
		auto l = val.length();
		if (l == 0 || index >= l) throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : var( { "bad 'index'", //
						" ( ", idx_solver(index), " )" }).join())));
		return ret_idx(index);
	}
	
	operator bool() noexcept {
		if (val.length() > 0) return true;
		return false;
	}
	operator char() {
		return val.front();
	}
	operator str() {
		return val;
	}
	operator str() const {
		return val;
	}
	operator str*() const {
		return const_cast<str*>(&val);
	}
	// hlp:
	var* _mkcopy() {
		return new ttyp<str>(val);
	}
	void setv(var &rv) {
		val = *getPv<str>(rv.pttyp);
		__upd_val();
	}
	void setv(cch *rv) {
		val = rv;
		__upd_val();
	}
	void setv(const str &rv) {
		val = rv;
		__upd_val();
	}
	var& ret_idx(ullong index);
	void set_ivalid_itrs();
	// common if:
	int size() const noexcept {
		return sizeof(val);
	}
	void prnt() const noexcept {
		if (val.length()) std::cout << val << std::endl;
		else std::cout << "\"\"" << std::endl;
	}
	//  if:
	// Element access:
	var& at(ullong index) {
		auto l = val.length();
		if (l == 0 || index >= l) throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : var( { "bad 'index'", //
						" ( ", idx_solver(index), " )" }).join())));
		return ret_idx(index);
	}
	var& front() {
		auto l = val.length();
		if (l == 0) throw VarExcp(VarExcp::_mk_excp("out_of_range", "empty"));
		return ret_idx(0);
	}
	var& back() {
		auto l = val.length();
		if (l == 0) throw VarExcp(VarExcp::_mk_excp("out_of_range", "empty"));
		return ret_idx(l - 1);
	}
	// Iterators:
	//-frw-
	var begin() {
		return var(val.begin(), this);
	}
	var end() {
		return var(val.end(), this);
	}
	//-rvr-
	var rbegin() {
		return var(val.rbegin(), this);
	}
	var rend() {
		return var(val.rend(), this);
	}
	// Capacity:
	bool empty() const {
		return val.empty();
	}
	ullong length() const {
		return val.length();
	}
	ullong max_size() const {
		return val.max_size();
	}
	void reserve(ullong new_cap = 0) {
		try {
			++isyn;
			val.reserve(new_cap);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("capacity_error", "'new_cap' > max_size"));
		}
	}
	ullong capacity() const {
		return val.capacity();
	}
	void shrink_to_fit() {
		++isyn;
		val.shrink_to_fit();
		__upd_val();
	}
	// Operations:
	void clear() {
		val.clear();
		__upd_val();
	}
	//..	
	void resize(ullong count) {
		try {
			++isyn;
			val.resize(count, ' ');
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "'count' > max_size"));
		}
	}
	void resize(ullong count, const var &X) {
		if (X.tid[0] == 'c') {
			try {
				++isyn;
				val.resize(count, (char) X);
				__upd_val();
			} catch (VarExcp &e) {
				throw;
			} catch (std::exception &e) {
				throw VarExcp(VarExcp::_mk_excp("length_error", //
						"'count' > max_size"));
			}
		} else throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
				"bad 'X' [ var:" + var(X.gtid()) + " ]"));
	}
	void resize(ullong count, char ch) {
		try {
			++isyn;
			val.resize(count, ch);
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	//..
	cch* data() const {
		return val.data();
	}
	void swap(var &with) {
		auto z = dcst<str>(with.pttyp);
		if (z) {
			++isyn;
			val.swap(z->val);
#ifdef _VAR_DBG
			upd_val();
			z->upd_val();
#endif
		} else throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
				"bad 'with' [ var:" + var(with.gtid()) + " ]"));
	}
	//..
	var insert(ullong index, const var &src_first, const var &src_last);  //out
	var insert(ullong index, const var &X) {
		
#define __ins_str_rng_chk() \
		auto l = val.length();\
		if ((l == 0 && index !=0 ) || index > l)\
		throw VarExcp(VarExcp::_mk_excp( "out_of_range",\
	    var( { "bad 'index'"," ( ", idx_solver(index), " )" }).join()));		
		
		__ins_str_rng_chk()
		
		try {
			++isyn;
			if (X.pttyp) val.insert(index, *X.pttyp);
			else val.insert(index, "|udf|");
			__upd_val();
			return var(val.begin() += index, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	var insert(ullong index, int X) {
		
		__ins_str_rng_chk()
		
		try {
			++isyn;
			val.insert(index, std::to_string(X));
			__upd_val();
			return var(val.begin() += index, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	var insert(ullong index, float X) {
		
		__ins_str_rng_chk()
		
		try {
			++isyn;
			val.insert(index, std::to_string(X));
			__upd_val();
			return var(val.begin() += index, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	var insert(ullong index, double X) {
		
		__ins_str_rng_chk()
		
		try {
			++isyn;
			val.insert(index, std::to_string(X));
			__upd_val();
			return var(val.begin() += index, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	var insert(ullong index, long X) {
		
		__ins_str_rng_chk()
		
		try {
			++isyn;
			val.insert(index, std::to_string(X));
			__upd_val();
			return var(val.begin() += index, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	var insert(ullong index, llong X) {
		
		__ins_str_rng_chk()
		
		try {
			++isyn;
			val.insert(index, std::to_string(X));
			__upd_val();
			return var(val.begin() += index, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	var insert(ullong index, ullong X) {
		
		__LL_exc_p(X)
		__ins_str_rng_chk()
		
		try {
			++isyn;
			val.insert(index, std::to_string(X));
			__upd_val();
			return var(val.begin() += index, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	var insert(ullong index, char X) {
		
		__ins_str_rng_chk()
		
		try {
			++isyn;
			auto i = val.insert(val.begin() += index, X);
			__upd_val();
			return var(i, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	var insert(ullong index, cch *X) {
		
		__ins_str_rng_chk()
		
		try {
			++isyn;
			val.insert(index, X);
			__upd_val();
			return var(val.begin() += index, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return var();
	}
	var insert(ullong index, const str &X) {
		
		__ins_str_rng_chk()
		
		try {
			++isyn;
			val.insert(index, X);
			__upd_val();
			return var(val.begin() += index, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	var insert(ullong index, __iniL<var> X) {
		
		__ins_str_rng_chk()
		
		auto z = var(X).pttyp->join(); // [ord]
				
		try {
			++isyn;
			val.insert(index, z.data());
			__upd_val();
			return var(val.begin() += index, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	var insert(ullong index, ullong count, const var &X) {
		if (X.tid[0] == 'c') {
			
			__ins_str_rng_chk()
			
			try {
				++isyn;
				auto y = val.insert(val.begin() += index, count, X);
				__upd_val();
				return var(y, this);
			} catch (VarExcp &e) {
				throw;
			} catch (std::exception &e) {
				throw VarExcp(VarExcp::_mk_excp("length_error", //
						"index + 'count' > max_size"));
			}
		} else throw VarExcp(VarExcp::_mk_excp("unsupported_type", //
				"bad 'X' [ var:" + var(X.gtid()) + " ]"));
		return var();
	}
	var insert(ullong index, ullong count, char X) {
		
		__ins_str_rng_chk()
		
		try {
			++isyn;
			auto y = val.insert(val.begin() += index, count, X);
			__upd_val();
			return var(y, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var insert(ullong index, cch *src, ullong src_index, ullong src_count) {
		
		__ins_str_rng_chk()
		
		try {
			++isyn;
			val.insert(index, src += src_index, src_count);
			__upd_val();
			return var(val.begin() += index, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("std::exception", e));
		}
	}
	//..
	var erase(ullong index) {
		auto l = val.length();
		if (l == 0 || index >= l) throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : var( { "bad 'index'", //
						" ( ", idx_solver(index), " )" }).join())));
		++isyn;
		auto z = val.erase(val.begin() += index);
		__upd_val();
		if (l > 1) return var(z, this);
		return var();
	}
	var erase(ullong first, ullong last) {
		auto l = val.length();
		if (l == 0 || first >= last || last > l) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : "bad: 'fist' or 'last' or both")));
		++isyn;
		auto z = val.erase(val.begin() += first, val.begin() += last);
		__upd_val();
		if ((l - (last - first)) > 1) return var(z, this);
		return var();
	}
	//..
	var push_back(const var &X) {
		try {
			++isyn;
			if (tid[0] == 's') //
			val.append((str) X);
			else if (tid[0] == 'c') //
			val.push_back((char) X);
			else val.append((str) X);
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return var(val.end() -= 1, this);
	}
	var push_back(int X) {
		try {
			++isyn;
			val.append(std::to_string(X));
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return var(val.end() -= 1, this);
	}
	var push_back(float X) {
		try {
			++isyn;
			val.append(std::to_string(X));
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return var(val.end() -= 1, this);
	}
	var push_back(double X) {
		try {
			++isyn;
			val.append(std::to_string(X));
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return var(val.end() -= 1, this);
	}
	var push_back(long X) {
		try {
			++isyn;
			val.append(std::to_string(X));
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return var(val.end() -= 1, this);
	}
	var push_back(llong X) {
		try {
			++isyn;
			val.append(std::to_string(X));
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return var(val.end() -= 1, this);
	}
	var push_back(ullong X) {
		
		__LL_exc_p(X)
		
		try {
			++isyn;
			val.append(std::to_string(X));
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return var(val.end() -= 1, this);
	}
	var push_back(char X) {
		try {
			++isyn;
			val.push_back(X);
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return var(val.end() -= 1, this);
	}
	var push_back(cch *X) {
		try {
			++isyn;
			val.append(X);
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return var(val.end() -= 1, this);
	}
	var push_back(const str &X) {
		try {
			++isyn;
			val.append(X);
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return var(val.end() -= 1, this);
	}
	var push_back( __iniL<var> X) {
		
		auto z = var(X).pttyp->join(); // [ord]
				
		try {
			++isyn;
			val.append(z.data());
			__upd_val();
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return var(val.end() -= 1, this);
	}
	//..
	var push_front(const var &X) {
		return insert(0, X);
	}
	var push_front(int X) {
		return insert(0, X);
	}
	var push_front(float X) {
		return insert(0, X);
	}
	var push_front(double X) {
		return insert(0, X);
	}
	var push_front(long X) {
		return insert(0, X);
	}
	var push_front(llong X) {
		return insert(0, X);
	}
	var push_front(ullong X) {
		__LL_exc_p(X)
		return insert(0, X);
	}
	var push_front(char X) {
		return insert(0, X);
	}
	var push_front(cch *X) {
		return insert(0, X);
	}
	var push_front(const str &X) {
		return insert(0, X);
	}
	var push_front( __iniL<var> X) {
		return insert(0, X);
	}
	//..
	var pop_back() {
		var ret;
		auto l = val.length();
		if (l > 0) {
			++isyn;
			ret = val[l - 1];
			val.pop_back();
		} else {
			throw VarExcp(VarExcp::_mk_excp("out_of_range", "empty"));
		}
		__upd_val();
		return ret;
	}
	var pop_front() {
		var ret;
		auto l = val.length();
		if (l > 0) {
			++isyn;
			ret = val[0];
			auto a = val.begin(), b = val.begin(), e = val.end();
			++a;
			while (a != e) {
				*b = *a;
				++b;
				++a;
			}
			val.resize(l - 1);
		} else {
			throw VarExcp(VarExcp::_mk_excp("out_of_range", "empty"));
		}
		__upd_val();
		return ret;
	}
	//..
	
#define __sort_rev_str_rng()\
   	    auto l = val.size();\
		if (l == 0) throw VarExcp(VarExcp::_mk_excp("out_of_range", "empty"));\
	
#define __sort_str_cmp()\
		if (cmp == 0) throw VarExcp(VarExcp::_mk_excp("bad_argument", "bad 'cmp'"));\
		
	
	var sort_cmp(bool (*cmp)(var &a, var &b), bool reverse) const {
		__sort_rev_str_rng()
		__sort_str_cmp()
		ttyp<str> *ctx = new ttyp<str>(val);
		_sort_cstm(ctx, cmp, reverse);
		return *ctx;
	}
	var& ssort_cmp(bool (*cmp)(var &a, var &b), bool reverse) {
		__sort_rev_str_rng()
		__sort_str_cmp()
		_sort_cstm(this, cmp, reverse);
		return *this;
	}
	var sort(bool reverse) const {
		__sort_rev_str_rng()
		ttyp<str> *ctx = new ttyp<str>(val);
		_sort_def(ctx, reverse);
		return *ctx;
	}
	var& ssort(bool reverse) {
		__sort_rev_str_rng()
		_sort_def(this, reverse);
		return *this;
	}
	void _sort_def(ttyp<str> *ctx, bool reverse) const {
		
		var a = !reverse ? var(ctx->val.begin(), ctx) : var(ctx->val.rbegin(), ctx),  //
		e = !reverse ? var(ctx->val.end(), ctx) : var(ctx->val.rend(), ctx), b;
		char x;
		while (a != e) {
			b = a;
			++b;
			while (b != e) {
				if (*b < *a) {
					x = *a;
					*a = *b;
					*b = x;
				}
				++b;
			}
			++a;
		}
	}
	void _sort_cstm(ttyp<str> *ctx, bool (*cmp)(var &a, var &b), bool reverse) const {
		
		var a = !reverse ? var(ctx->val.begin(), ctx) : var(ctx->val.rbegin(), ctx),  //
		e = !reverse ? var(ctx->val.end(), ctx) : var(ctx->val.rend(), ctx), b;
		char x;
		while (a != e) {
			b = a;
			++b;
			while (b != e) {
				if (cmp(a, b)) {
					x = *a;
					*a = *b;
					*b = x;
				}
				++b;
			}
			++a;
		}
	}
	var sample(ullong num) const {
		
		auto l = val.size();
		if (l == 0 || num > l) throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : var( { "bad 'num'", //
						" ( ", idx_solver(num), " )" }).join())));
		
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(0, l - 1);
		ttyp<str> *smp = new ttyp<str>("");
		if (num == 0) num = l;
		
		ullong i = 0;
		while (i < num) {
			smp->push_back(val[dist(gen)]);
			++i;
		}
		
		return *smp;
	}
	var sample_unq(ullong num) const {
		
		auto l = val.size();
		if (l == 0 || num > l) throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : var( { "bad 'num'", //
						" ( ", idx_solver(num), " )" }).join())));
		
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(0, l - 1);
		ttyp<str> *smp = new ttyp<str>(val);
		if (num == 0) num = l;
		
		ullong i = 0;
		int a, b;
		char *_x, *_y, _z;
		
		while (i < l) {
			a = dist(gen);
			b = dist(gen);
			if (a != b) {
				_x = &smp->val[a];
				_y = &smp->val[b];
				_z = *_x;
				*_x = *_y;
				*_y = _z;
			}
			++i;
		}
		if (num != l) smp->val.resize(num);
#ifdef _VAR_DBG
		smp->upd_val();
#endif
		
		return *smp;
	}
	var reverse() {
		
		__sort_rev_str_rng()
		
		ttyp<str> *ret = new ttyp<str>(val);
		auto b = ret->val.begin(), e = ret->val.end();
		char z;
		
		e--;
		while (b != e) {
			z = *b;
			*b = *e;
			*e = z;
			++b;
			if (b == e) break;
			--e;
		}
		
		return *ret;
	}
	var& sreverse() {
		
		__sort_rev_str_rng()
		
		auto b = val.begin(), e = val.end();
		char z;
		
		e--;
		while (b != e) {
			z = *b;
			*b = *e;
			*e = z;
			++b;
			if (b == e) break;
			--e;
		}
		
		return *this;
	}
	bool equal(const var &src_first, const var &src_last, ullong index, //
			ullong count) const; // out
	var mismatch(const var &src_first, const var &src_last, ullong index, //
			ullong count) const; // out
	//..
	
#define __fill_str()\
		auto l = val.size();\
		if (index > l) throw VarExcp(VarExcp::_mk_excp("out_of_range",\
				var( { "bad 'index'", " ( ", idx_solver(index), " )" }).join()));\
		if (count == 0) count = l - index;\
		ullong end;\
		if ((end = index + count) > l) {\
			try {\
				++isyn;\
				val.resize(end, X);\
			} catch (VarExcp &e) {\
				throw;\
			} catch (std::exception &e) {\
				throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));\
			}\
			char *src = const_cast<char*>(val.data());\
			while (index < l) {\
				src[index] = X;\
				++index;\
			}\
		} else {\
			char *src = const_cast<char*>(val.data());\
			while (index < end) {\
				src[index] = X;\
				++index;\
			}}\
	
	
	var& fill(const var &X, ullong index, ullong count) {
		if (X.tid[0] == 'c') {
			__fill_str()
		} else throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
				"bad 'X' [ var:" + var(X.gtid()) + " ]"));
		__upd_val();
		return *this;
	}
	var& fill(char X, ullong index, ullong count) {
		__fill_str()
		__upd_val();
		return *this;
	}
	//..
	var split(char sep) const;   // out
	var& ssplit(char sep);       // out
	var& ltrim() {
		int l = val.length();
		if (l > 0) {
			int i = 0;
			++isyn;
			char *p = const_cast<char*>(val.data());
			while (i < l) {
				if (p[i] != ' ' && p[i] != '\t') {
					int _i = 0, __i = i;
					while (__i < l) {
						p[_i] = p[__i];
						++_i, ++__i;
					}
					val.resize(l - i);
					__upd_val();
					return *this;
				}
				++i;
			}
			if (i == l) val.resize(0);
			__upd_val();
		}
		return *this;
	}
	var& rtrim() {
		int l = val.length();
		if (l > 0) {
			++isyn;
			--l;
			while (l >= 0) {
				if (val[l] != ' ' && val[l] != '\t') {
					val.resize(l + 1);
					__upd_val();
					return *this;
				}
				--l;
			}
			resize(0);
			__upd_val();
		}
		return *this;
	}
	var& trim() {
		ltrim(), rtrim();
		return *this;
	}
	//..
	var& append(char ch, ullong count) {
		try {
			++isyn;
			val.append(count, ch);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		__upd_val();
		return *this;
	}
	var& append(const var &src, ullong src_index, ullong src_count) {
		
#define __apnd_rng_chk(par)\
		if (l == 0 || src_index >= l) throw VarExcp(VarExcp::_mk_excp( "out_of_range",\
		   (l == 0 ? "'src' -> empty" : var( { "bad 'src_index'",\
		    " ( ", idx_solver(src_index), " )" }).join())));\
		if (src_count == 0) src_count = l - src_index;\
		else if (src_count > l - src_index)\
		throw VarExcp(VarExcp::_mk_excp( "out_of_range","bad 'src_count'"));\
		
		str *ps;
		if (src.tid[0] == 's') {
			ps = src;
		} else {
			throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
					"bad 'src' [ var:" + var(src.gtid()) + " ]"));
		}
		
		auto l = ps->length();
		__apnd_rng_chk()
		
		try {
			++isyn;
			val.append(*ps, src_index, src_count);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		__upd_val();
		return *this;
	}
	var& append(cch *src, ullong src_index, ullong src_count) {
		
		auto l = varF::cstrlen(src);
		__apnd_rng_chk()
		
		try {
			++isyn;
			val.append(src + src_index, src_count);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		__upd_val();
		return *this;
	}
	var& append(str &src, ullong src_index, ullong src_count) {
		
		auto l = src.length();
		__apnd_rng_chk(__apnd_fnm(str &src))
		
		try {
			++isyn;
			val.append(src, src_index, src_count);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		__upd_val();
		return *this;
	}
	var& append(const var &src_first, const var &src_last); // out	
	//..
	var& replace(ullong index, ullong count, const var &src, ullong src_index, ullong src_count) {
		
#define __rpl_rng_chk1() \
		if (l1 == 0 || index >= l1) \
		throw VarExcp(VarExcp::_mk_excp("out_of_range",\
		 (l1 == 0 ? "empty" : var( { "bad 'index'",\
		" ( ", idx_solver(index), " )" }).join())));\
		else if (count > l1 - index)\
		throw VarExcp(VarExcp::_mk_excp("out_of_range", "bad 'count'"));\
		
#define __rpl_rng_chk2() \
		if (l2 == 0 || src_index >= l2)\
		throw VarExcp(VarExcp::_mk_excp("out_of_range",\
		(l2 == 0 ? "'src' -> empty" : var( { "bad 'src_index'",\
		" ( ", idx_solver(src_index), " )" }).join())));\
		if (src_count == 0) src_count = l2 - src_index;\
		else if (src_count > l2 - src_index)\
		throw VarExcp(VarExcp::_mk_excp("out_of_range","bad 'src_count'"));\
		
		
		str *ps;
		if (src.tid[0] == 's') {
			ps = src;
		} else {
			throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
					"bad 'src' [ var:" + var(src.gtid()) + " ]"));
		}
		
		auto l1 = val.length();
		__rpl_rng_chk1()
		
		auto l2 = ps->length();
		__rpl_rng_chk2()
		
		try {
			++isyn;
			val.replace(index, count, *ps, src_index, src_count);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return *this;
	}
	var& replace(ullong index, ullong count, cch *src, ullong src_index, ullong src_count) {
		
		auto l1 = val.length();
		__rpl_rng_chk1()
		
		auto l2 = varF::cstrlen(src);
		__rpl_rng_chk2()
		
		try {
			++isyn;
			val.replace(index, count, src + src_index, src_count);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return *this;
	}
	var& replace(ullong index, ullong count, str &src, ullong src_index, ullong src_count) {
		
		auto l1 = val.length();
		__rpl_rng_chk1()
		
		auto l2 = src.length();
		__rpl_rng_chk2()
		
		try {
			++isyn;
			val.replace(index, count, src, src_index, src_count);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
		return *this;
	}
	var& replace(ullong index, ullong count, const var &src_first, const var &src_last); // out 
	//..	
	int compare(const var &with) const {
		if (with.tid[0] == 's') {
			return val.compare(*((str*) with));
		} else throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
				"bad 'with' [ var:" + var(with.gtid()) + " ]"));
		return 0;
	}
	int compare(cch *with) const {
		return val.compare(with);
	}
	int compare(str &with) const {
		return val.compare(with);
	}
	int compare(ullong index, ullong count, const var &with, ullong with_index, ullong with_count) const {
		
#define __cmp_rng_chk1()\
		if (l1 == 0 || index >= l1)\
		throw VarExcp(VarExcp::_mk_excp("out_of_range",\
	    (l1 == 0 ? "empty" : var( { "bad 'index'",\
		" ( ", idx_solver(index), " )" }).join())));\
		else if (count > l1 - index)\
		throw VarExcp(VarExcp::_mk_excp("out_of_range","bad 'count'"));\
		
		
#define __cmp_rng_chk2()\
		if (l2 == 0 || with_index >= l2)\
		throw VarExcp(VarExcp::_mk_excp("out_of_range",\
		(l2 == 0 ? "'with' -> empty" : var( { "bad 'with_index'",\
		" ( ", idx_solver(with_index), " )" }).join())));\
		if (with_count == 0) with_count = l2 - with_index;\
		else if (with_count > l2 - with_index)\
		throw VarExcp(VarExcp::_mk_excp("out_of_range","bad 'with_count'"));\
	
		
		if (with.tid[0] != 's')  //
		throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
				"bad 'with' [ var:" + var(with.gtid()) + " ]"));
		
		auto l1 = val.length();
		__cmp_rng_chk1()
		
		auto l2 = with.length();
		__cmp_rng_chk2()
		
		try {
			return val.compare(index, count, *((str*) with), with_index, with_count);
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	int compare(ullong index, ullong count, cch *with, ullong with_index, ullong with_count) const {
		
		auto l1 = val.length();
		__cmp_rng_chk1()
		
		auto l2 = varF::cstrlen(with);
		__cmp_rng_chk2()
		
		try {
			return val.compare(index, count, with + with_index, with_count);
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	int compare(ullong index, ullong count, str &with, ullong with_index, ullong with_count) const {
		
		auto l1 = val.length();
		__cmp_rng_chk1()
		
		auto l2 = with.length();
		__cmp_rng_chk2()
		//
		try {
			return val.compare(index, count, with, with_index, with_count);
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	}
	//..
	bool starts_with(const var &prefix) const {
		if (prefix.tid[0] == 's') {
			return starts_with(val.data());
		} else if (prefix.tid[0] == 'c') {
			return starts_with((char) prefix);
		} else {
			throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
					"bad 'prefix' [ var:" + var(prefix.gtid()) + " ]"));
		}
		return false;
	}
	bool starts_with(char prefix) const {
		auto l = val.length();
		if (l > 0) {
			if (val[0] == prefix) return true;
		}
		return false;
	}
	bool starts_with(cch *prefix) const {
		auto l = val.length();
		if (l > 0) {
			ullong xl = 0;
			while (*prefix != 0) {
				if (xl < l) {
					if (val[xl] != *prefix) return false;
					++prefix, ++xl;
				} else return false;
			}
			if (xl > 0) return true;
		}
		return false;
	}
	bool ends_with(const var &suffix) const {
		if (suffix.tid[0] == 's') {
			return ends_with(val.data());
		} else if (suffix.tid[0] == 'c') {
			return ends_with((char) suffix);
		} else {
			throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
					"bad 'suffix' [ var:" + var(suffix.gtid()) + " ]"));
		}
		return false;
	}
	bool ends_with(char suffix) const {
		auto l = val.length();
		if (l > 0) {
			if (val[l - 1] == suffix) return true;
		}
		return false;
	}
	bool ends_with(cch *suffix) const {
		auto l = val.length();
		if (l > 0) {
			auto xl = varF::cstrlen(suffix);
			//
			if (xl > 0 && xl <= l) {
				--xl, --l;
				while (l >= 0) {
					if (val[l] != *(suffix + xl)) break;
					if (xl == 0) return true;
					--l, --xl;
				}
			}
		}
		return false;
	}
	//..
	var substr(ullong index, ullong count) {
		auto l = val.length();
		if (l == 0 || index >= l) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : var( { "bad 'index'", //
						" ( ", idx_solver(index), " )" }).join())));
		if (count == 0) count = l - index;
		else if (count > l - index) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", "bad 'count'"));
		return val.substr(index, count);
	}
	ullong copy(char *dest, ullong index, ullong count, bool zero) const {
		auto l = val.length();
		if (l == 0 || index >= l) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : var( { "bad 'index'", //
						" ( ", idx_solver(index), " )" }).join())));
		if (count == 0) count = l - index;
		else if (count > l - index) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", "bad 'count'"));
		if (zero) *(dest + count) = 0;
		return val.copy(dest, count, index);
	}
	//..
	var find(const var &src, ullong index) const {
		
#define __fnd_rng_chk(par)\
		auto l = val.length();\
		if (l == 0 || index >= l)\
		throw VarExcp(VarExcp::_mk_excp("out_of_range",\
		     (l == 0 ? "empty" : var( { "bad 'index'",\
			" ( ", idx_solver(index), " )" }).join())));\
		
		
		__fnd_rng_chk()
		
		ullong pos;
		if (src.tid[0] == 's') {
			pos = val.find(*((str*) src), index);
		} else if (src.tid[0] == 'c') {
			pos = val.find((char) src, index);
		} else {
			throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
					"bad 'src' [ var:" + var(src.gtid()) + " ]"));
		}
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find(char src, ullong index) const {
		
		__fnd_rng_chk()
		
		ullong pos = val.find(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find(cch *src, ullong index) const {
		
		__fnd_rng_chk()
		
		ullong pos = val.find(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find(str &src, ullong index) const {
		
		__fnd_rng_chk()
		
		ullong pos = val.find(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var rfind(const var &src, ullong index) const {
		
		__fnd_rng_chk()
		
		if (index == 0) index = l - 1;
		ullong pos;
		if (src.tid[0] == 's') {
			pos = val.rfind(*((str*) src), index);
		} else if (src.tid[0] == 'c') {
			pos = val.rfind((char) src, index);
		} else {
			throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
					"bad 'src' [ var:" + var(src.gtid()) + " ]"));
		}
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var rfind(char src, ullong index) const {
		
		__fnd_rng_chk()
		
		if (index == 0) index = l - 1;
		ullong pos = val.rfind(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var rfind(cch *src, ullong index) const {
		
		__fnd_rng_chk()
		
		if (index == 0) index = l - 1;
		ullong pos = val.rfind(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var rfind(str &src, ullong index) const {
		
		__fnd_rng_chk()
		
		if (index == 0) index = l - 1;
		ullong pos = val.rfind(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	//..
	var find_first_of(const var &src, ullong index) const {
		
		__fnd_rng_chk()
		
		ullong pos;
		if (src.tid[0] == 's') {
			pos = val.find_first_of(*((str*) src), index);
		} else if (src.tid[0] == 'c') {
			pos = val.find_first_of((char) src, index);
		} else {
			throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
					"bad 'src' [ var:" + var(src.gtid()) + " ]"));
		}
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_first_of(char src, ullong index) const {
		
		__fnd_rng_chk()
		
		ullong pos = val.find_first_of(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_first_of(cch *src, ullong index) const {
		
		__fnd_rng_chk()
		
		ullong pos = val.find_first_of(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_first_of(str &src, ullong index) const {
		
		__fnd_rng_chk()
		
		ullong pos = val.find_first_of(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_first_not_of(const var &src, ullong index) const {
		
		__fnd_rng_chk()
		
		ullong pos;
		if (src.tid[0] == 's') {
			pos = val.find_first_not_of(*((str*) src), index);
		} else if (src.tid[0] == 'c') {
			pos = val.find_first_not_of((char) src, index);
		} else {
			throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
					"bad 'src' [ var:" + var(src.gtid()) + " ]"));
		}
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_first_not_of(char src, ullong index) const {
		
		__fnd_rng_chk()
		
		ullong pos = val.find_first_not_of(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_first_not_of(cch *src, ullong index) const {
		
		__fnd_rng_chk()
		
		ullong pos = val.find_first_not_of(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_first_not_of(str &src, ullong index) const {
		
		__fnd_rng_chk()
		
		ullong pos = val.find_first_not_of(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_last_of(const var &src, ullong index) const {
		
		__fnd_rng_chk()
		
		if (index == 0) index = l - 1;
		ullong pos;
		if (src.tid[0] == 's') {
			pos = val.find_last_of(*((str*) src), index);
		} else if (src.tid[0] == 'c') {
			pos = val.find_last_of((char) src, index);
		} else {
			throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
					"bad 'src' [ var:" + var(src.gtid()) + " ]"));
		}
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_last_of(char src, ullong index) const {
		
		__fnd_rng_chk()
		
		if (index == 0) index = l - 1;
		ullong pos = val.find_last_of(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_last_of(cch *src, ullong index) const {
		
		__fnd_rng_chk()
		
		if (index == 0) index = l - 1;
		ullong pos = val.find_last_of(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_last_of(str &src, ullong index) const {
		
		__fnd_rng_chk()
		
		if (index == 0) index = l - 1;
		ullong pos = val.find_last_of(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_last_not_of(const var &src, ullong index) const {
		
		__fnd_rng_chk()
		
		if (index == 0) index = l - 1;
		ullong pos;
		if (src.tid[0] == 's') {
			pos = val.find_last_not_of(*((str*) src), index);
		} else if (src.tid[0] == 'c') {
			pos = val.find_last_not_of((char) src, index);
		} else {
			throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
					"bad 'src' [ var:" + var(src.gtid()) + " ]"));
		}
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_last_not_of(char src, ullong index) const {
		
		__fnd_rng_chk()
		
		if (index == 0) index = l - 1;
		ullong pos = val.find_last_not_of(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_last_not_of(cch *src, ullong index) const {
		
		__fnd_rng_chk()
		
		if (index == 0) index = l - 1;
		ullong pos = val.find_last_not_of(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	var find_last_not_of(str &src, ullong index) const {
		
		__fnd_rng_chk()
		
		if (index == 0) index = l - 1;
		ullong pos = val.find_last_not_of(src, index);
		if (pos != ullong(-1)) {
			return var(val.begin() += pos, this);
		}
		return var();
	}
	//..
	int _int() const {
		try {
			return std::stoi(val);
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("conversion_err"));
		}
		return 0;
	}
	llong _llong() const {
		try {
			return std::stoll(val);
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("conversion_err"));
		}
		return 0;
	}
	ullong _ullong() const {
		try {
			return std::stoull(val);
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("conversion_err"));
		}
		return 0;
	}
	float _float() const {
		try {
			return std::stof(val);
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("conversion_err"));
		}
		return 0;
	}
	double _double() const {
		try {
			return std::stod(val);
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("conversion_err"));
		}
		return 0;
	}
	var _str() const {
		return val;
	}
};
//-iterators-frw-
template<> class ttyp<Ic_str> : public var {
	
	friend class var;
	friend class ttyp<str> ;
	friend class ttyp<char> ;
	friend class ttyp<VMAP> ;

private:

	mutable Ic_str _Ic_str;
	cch *tid = "I_str";
	mutable ttyp<str> *bs = 0;
	mutable var_li::idx *stg = 0;
	mutable int isyn = 0;
	mutable llong idx = 0;
	mutable llong end = 0;
	var idx1;

#ifdef _VAR_DBG
	mutable dbg ddbg;
	void upd_val() const {
		if (idx < end) {
			ddbg.sval = &*_Ic_str;
		} else {
			ddbg.sval = 0;
		}
	}
#endif
	
	ttyp(const Ic_str &I, const ttyp<str> *_bs, bool flg = 0) :
			_Ic_str(I) {
		__cnstr_cll("ttyp<Ic_str>(const Ic_str,const ttyp<str>*)", "");
		__VAR_C_TYP();
		var::tid = tid;
		if (_bs) {
			bs = const_cast<ttyp<str>*>(_bs);
			stg = bs->itrs.get();
			stg->val = this;
			if (!flg) isyn = bs->isyn;
			end = bs->val.length();
			idx = (&(*_Ic_str) - &(*bs->val.begin()));
#ifdef _VAR_DBG  
			upd_val();
			pdbg = &ddbg;
#endif
		}
	}
	~ttyp() {
		__cnstr_cll("~ttyp<Ic_str>() ", "");
		__VAR_D_TYP();
		if (stg) bs->itrs.rem(stg);
	}
	
#define __is_valid() \
		if (!is_valid())\
		throw VarExcp(VarExcp::_mk_excp("invalid_iterator"));\
	
	
	var& operator*(); // out
	operator bool() noexcept {
		if (is_valid() && idx != end) return true;
		return false;
	}
	bool operator!() const {
		if (is_valid()) return false;
		return true;
	}
	
	bool operator!=(const var &rv) const {
		
		__is_valid()
		
		auto prv = dcst<Ic_str>(rv.pttyp);
		if (prv) return _Ic_str != prv->_Ic_str;
		else OE_solver(0, this, 0, 0, //
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator==(const var &rv) const {
		
		__is_valid()
		
		auto prv = dcst<Ic_str>(rv.pttyp);
		if (prv) return _Ic_str == prv->_Ic_str;
		else OE_solver(0, this, 0, 0, //
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	
#define __itr_rng_pls_chk() \
	   clc_prc_spc(idx, rv, 0);\
	   if (rv < 0) {\
			if ((llong) (-rv) > idx) {\
			throw VarExcp(VarExcp::_mk_excp("out_of_range",\
   	        var( { "bad 'rv'"," ( ", rv, " )"," [left-out]" }).join()));}\
		} else if (idx + (llong) rv > end)\
	  	  throw VarExcp(VarExcp::_mk_excp("out_of_range",\
	   	  var( { "bad 'rv'"," ( ", rv, " )"," [right-out]" }).join()));\
	
	
	var operator+(int rv) const {
		
		__is_valid()
		__itr_rng_pls_chk()
		
		return var(_Ic_str + rv, bs);
	}
	var operator+(long rv) const {
		
		__is_valid()
		__itr_rng_pls_chk()
		
		return var(_Ic_str + rv, bs);
	}
	var operator+(llong rv) const {
		
		__is_valid()
		__itr_rng_pls_chk()
		
		return var(_Ic_str + rv, bs);
	}
	var operator+(ullong rv) const {
		
		__LL_exc()
		__is_valid()
		__itr_rng_pls_chk()
		
		return var(_Ic_str + rv, bs);
	}
	
	var& operator+=(int rv) {
		
		__is_valid()
		__itr_rng_pls_chk()
		
		_Ic_str += rv, idx += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(long rv) {
		
		__is_valid()
		__itr_rng_pls_chk()
		
		_Ic_str += rv, idx += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(llong rv) {
		
		__is_valid()
		__itr_rng_pls_chk()
		
		_Ic_str += rv, idx += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(ullong rv) {
		
		__LL_exc()
		__is_valid()
		__itr_rng_pls_chk()
		
		_Ic_str += rv, idx += rv;
		__upd_val();
		return *this;
	}
	
#define __itr_rng_min_chk() \
	   clc_prc_spc(idx, rv, 1);\
	   if (rv < 0) {\
			if (idx - (llong) rv > end) {\
			throw VarExcp(VarExcp::_mk_excp("out_of_range",\
   	        var( { "bad 'rv'"," ( ", rv, " )"," [right-out]" }).join()));}\
		} else if ((llong) rv > idx)\
	  	  throw VarExcp(VarExcp::_mk_excp("out_of_range",\
	   	  var( { "bad 'rv'"," ( ", rv, " )"," [left-out]" }).join()));\
	
	
	var operator-(int rv) const {
		
		__is_valid()
		__itr_rng_min_chk()
		
		return var(_Ic_str - rv, bs);
	}
	var operator-(long rv) const {
		
		__is_valid()
		__itr_rng_min_chk()
		
		return var(_Ic_str - rv, bs);
	}
	var operator-(llong rv) const {
		
		__is_valid()
		__itr_rng_min_chk()
		
		return var(_Ic_str - rv, bs);
	}
	var operator-(ullong rv) const {
		
		__LL_exc()
		__is_valid()
		__itr_rng_min_chk()
		
		return var(_Ic_str - rv, bs);
	}
	
	var& operator-=(int rv) {
		
		__is_valid()
		__itr_rng_min_chk()
		
		_Ic_str -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	var& operator-=(long rv) {
		
		__is_valid()
		__itr_rng_min_chk()
		
		_Ic_str -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	var& operator-=(llong rv) {
		
		__is_valid()
		__itr_rng_min_chk()
		
		_Ic_str -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	var& operator-=(ullong rv) {
		
		__LL_exc()
		__is_valid()
		__itr_rng_min_chk()
		
		_Ic_str -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	
	operator Ic_str() {
		return _Ic_str;
	}
	operator Ic_str*() {
		return &_Ic_str;
	}
	operator str() {
		str s = "| [var:I_str]";
		if (is_valid()) {
			if (idx == end) {
				s += " at the end() |";
			} else {
				s += " val: '";
				s += *_Ic_str;
				s += "' |";
			}
		} else s += " is invalid |";
		return s;
	}
	operator char() {
		__is_valid()
		return *_Ic_str;
	}
	// hlp
	var* _mkcopy() {
		is_valid(1);
		return new ttyp<Ic_str>(_Ic_str, bs, 1);
	}
	void setv(var &rv) {
		bs->itrs.rem(stg);
		auto x = dcst<Ic_str>(rv.pttyp);
		_Ic_str = x->_Ic_str;
		bs = x->bs, idx = x->idx, end = x->end, isyn = x->isyn;
		stg = bs->itrs.get();
		stg->val = this;
		if (is_valid()) {
			__upd_val();
		}
	}
	
	var& operator++() {
		
		__is_valid()
		
		if (idx == end) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", "[right-out]"));
		++_Ic_str, ++idx;
		__upd_val();
		return *this;
	}
	var operator++(int) {
		
		__is_valid()
		auto tmp = _Ic_str;
		
		if (idx == end) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", "[right-out]"));
		++_Ic_str, ++idx;
		__upd_val();
		return var(tmp, bs);
	}
	var& operator--() {
		
		__is_valid()
		
		if (idx == 0) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", "[left-out]"));
		--_Ic_str, --idx;
		__upd_val();
		return *this;
	}
	var operator--(int) {
		
		__is_valid()
		auto tmp = _Ic_str;
		
		if (idx == 0) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", "[left-out]"));
		--_Ic_str, --idx;
		__upd_val();
		return var(tmp, bs);
	}
	// if:
	bool next() const {
		
		__is_valid()
		
		if (end > 0) {
			if (idx < end - 1) {
				(const_cast<ttyp<Ic_str>*>(this))->operator++();
				return true;
			}
		}
		return false;
	}
	llong pos() const {
		__is_valid()
		return idx;
	}
	inline bool is_valid(bool flg = 0) const {
		if (bs) {
			if (isyn != bs->isyn) {
				auto i = &*_Ic_str;
				if (i >= &*bs->val.begin() && i <= &*bs->val.end()) {
					if (!flg) isyn = bs->isyn;
					return true;
				}
				bs->itrs.rem(stg);
				bs = 0, stg = 0;
#ifdef _VAR_DBG   
				ddbg.sval = 0;
#endif
				return false;
			}
		} else return false;
		return true;
	}
	void prnt() const noexcept {
		if (is_valid()) {
			if (idx == end) {
				std::cout << "| [var:I_str] at the end() |" << std::endl;
			} else {
				std::cout << (const_cast<ttyp<Ic_str>*>(this))->operator str() //
						<< std::endl;
			}
		} else {
			std::cout << "| [var:I_str] is invalid |" << std::endl;
		}
	}
};
//-iterators-rvr-
template<> class ttyp<Irc_str> : public var {
	
	friend class var;
	friend class ttyp<str> ;
	friend class ttyp<char> ;
	friend class ttyp<VMAP> ;

private:

	mutable Irc_str _Irc_str;
	cch *tid = "Ir_str";
	mutable ttyp<str> *bs = 0;
	mutable var_li::idx *stg = 0;
	mutable int isyn = 0;
	mutable llong idx = 0;
	mutable llong end = 0;
	var idx1;

#ifdef _VAR_DBG
	mutable dbg ddbg;
	void upd_val() const {
		if (idx < end) {
			ddbg.sval = &*_Irc_str;
		} else {
			ddbg.sval = 0;
		}
	}
#endif
	
	ttyp(const Irc_str &I, const ttyp<str> *_bs, bool flg = 0) :
			_Irc_str(I) {
		__cnstr_cll("ttyp<Irc_str>(const Irc_str,const ttyp<str> *_bs)", "");
		__VAR_C_TYP();
		var::tid = tid;
		if (_bs) {
			bs = const_cast<ttyp<str>*>(_bs);
			if (!flg) isyn = bs->isyn;
			stg = bs->itrs.get();
			stg->val = this;
			end = bs->val.length();
			idx = (&(*bs->val.rbegin()) - &(*_Irc_str));
#ifdef _VAR_DBG
			upd_val();
			pdbg = &ddbg;
#endif
		}
	}
	~ttyp() {
		__cnstr_cll("~ttyp<Irc_str>() ", "");
		__VAR_D_TYP();
		if (stg) bs->itrs.rem(stg);
	}
	
#define __is_valid() \
		if (!is_valid())\
		throw VarExcp(VarExcp::_mk_excp("invalid_iterator"));\
	
	
	var& operator*();  // out
	operator bool() noexcept {
		if (is_valid() && idx != end) return true;
		return false;
	}
	bool operator!() const {
		if (is_valid()) return false;
		return true;
	}
	
	bool operator!=(const var &rv) const {
		
		__is_valid()
		
		auto prv = dcst<Irc_str>(rv.pttyp);
		if (prv) return _Irc_str != prv->_Irc_str;
		else OE_solver(0, this, 0, 0, //
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator==(const var &rv) const {
		
		__is_valid()
		
		auto prv = dcst<Irc_str>(rv.pttyp);
		if (prv) return _Irc_str == prv->_Irc_str;
		else OE_solver(0, this, 0, 0, //
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	
#define __itr_rng_pls_chk_rvr() \
	   clc_prc_spc(idx, rv, 0);\
	   if (rv < 0) {\
			if ((llong) (-rv) > idx) {\
			throw VarExcp(VarExcp::_mk_excp("out_of_range",\
   	        var( { "bad 'rv'"," ( ", rv, " )"," [right-out]" }).join()));}\
		} else if (idx + (llong) rv > end)\
	  	  throw VarExcp(VarExcp::_mk_excp("out_of_range",\
	   	  var( { "bad 'rv'"," ( ", rv, " )"," [left-out]" }).join()));\
	
	
	var operator+(int rv) const {
		
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		return var(_Irc_str + rv, bs);
	}
	var operator+(long rv) const {
		
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		return var(_Irc_str + rv, bs);
	}
	var operator+(llong rv) const {
		
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		return var(_Irc_str + rv, bs);
	}
	var operator+(ullong rv) const {
		
		__LL_exc()
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		return var(_Irc_str + rv, bs);
	}
	
	var& operator+=(int rv) {
		
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		_Irc_str += rv, idx += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(long rv) {
		
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		_Irc_str += rv, idx += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(llong rv) {
		
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		_Irc_str += rv, idx += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(ullong rv) {
		
		__LL_exc()
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		_Irc_str += rv, idx += rv;
		__upd_val();
		return *this;
	}
	
#define __itr_rng_min_chk_rvr() \
	   clc_prc_spc(idx, rv, 1);\
	   if (rv < 0) {\
			if (idx - (llong) rv > end) {\
			throw VarExcp(VarExcp::_mk_excp("out_of_range",\
   	        var( { "bad 'rv'"," ( ", rv, " )"," [left-out]" }).join()));}\
		} else if ((llong) rv > idx)\
	  	  throw VarExcp(VarExcp::_mk_excp("out_of_range",\
	   	  var( { "bad 'rv'"," ( ", rv, " )"," [right-out]" }).join()));\
	
	
	var operator-(int rv) const {
		
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		return var(_Irc_str - rv, bs);
	}
	var operator-(long rv) const {
		
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		return var(_Irc_str - rv, bs);
	}
	var operator-(llong rv) const {
		
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		return var(_Irc_str - rv, bs);
	}
	var operator-(ullong rv) const {
		
		__LL_exc()
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		return var(_Irc_str - rv, bs);
	}
	
	var& operator-=(int rv) {
		
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		_Irc_str -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	var& operator-=(long rv) {
		
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		_Irc_str -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	var& operator-=(llong rv) {
		
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		_Irc_str -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	var& operator-=(ullong rv) {
		
		__LL_exc()
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		_Irc_str -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	
	operator Irc_str() {
		return _Irc_str;
	}
	operator Irc_str*() {
		return &_Irc_str;
	}
	operator str() {
		str s = "| [var:Ir_str]";
		if (is_valid()) {
			if (idx == end) {
				s += " at the rend() |'";
			} else {
				s += " val: '";
				s += *_Irc_str;
				s += "' |";
			}
		} else s += " is invalid |";
		return s;
	}
	operator char() {
		__is_valid()
		return *_Irc_str;
	}
	// hlp
	var* _mkcopy() {
		is_valid(1);
		return new ttyp<Irc_str>(_Irc_str, bs, 1);
	}
	void setv(var &rv) {
		bs->itrs.rem(stg);
		auto x = dcst<Irc_str>(rv.pttyp);
		_Irc_str = x->_Irc_str;
		bs = x->bs, idx = x->idx, end = x->end, isyn = x->isyn;
		stg = bs->itrs.get();
		stg->val = this;
		if (is_valid()) {
			__upd_val();
		}
	}
	
	var& operator++() {
		
		__is_valid()
		
		if (idx == end) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", "[left-out]"));
		++_Irc_str, ++idx;
		__upd_val();
		return *this;
	}
	var operator++(int) {
		
		__is_valid()
		auto tmp = _Irc_str;
		
		if (idx == end) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", "[left-out]"));
		++_Irc_str, ++idx;
		__upd_val();
		return var(tmp, bs);
	}
	var& operator--() {
		
		__is_valid()
		
		if (idx == 0) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", "[right-out]"));
		--_Irc_str, --idx;
		__upd_val();
		return *this;
	}
	var operator--(int) {
		
		__is_valid()
		auto tmp = _Irc_str;
		
		if (idx == 0) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", "[right-out]"));
		--_Irc_str, --idx;
		__upd_val();
		return var(tmp, bs);
	}
	// if:
	bool next() const {
		
		__is_valid()
		
		if (end > 0) {
			if (idx < end - 1) {
				const_cast<ttyp<Irc_str>*>(this)->operator++();
				return true;
			}
		}
		return false;
	}
	llong pos() const {
		__is_valid()
		return idx;
	}
	inline bool is_valid(bool flg = 0) const {
		if (bs) {
			if (isyn != bs->isyn) {
				auto ri = &*_Irc_str;
				if (ri <= &*bs->val.rbegin() && ri >= &*bs->val.rend()) {
					if (!flg) isyn = bs->isyn;
					return true;
				}
				bs->itrs.rem(stg);
				bs = 0, stg = 0;
#ifdef _VAR_DBG   
				ddbg.sval = 0;
#endif	
				return false;
			}
		} else return false;
		return true;
	}
	void prnt() const noexcept {
		if (is_valid()) {
			if (idx == end) {
				std::cout << "| [var:Ir_str] at the rend() |" << std::endl;
			} else {
				std::cout << (const_cast<ttyp<Irc_str>*>(this))->operator str() //
						<< std::endl;
			}
		} else {
			std::cout << "| [var:Ir_str] is invalid |" << std::endl;
		}
	}
};

//------------------------------------------------------- var:common
//------------------------------------------------------------------

template<typename T> class ttyp: public var {
	
	friend class var;
	friend class ttyp<bool> ;

private:
	T val;
	cch *tid;
#ifdef _VAR_DBG
	char _sval[40];
	dbg ddbg;
#endif
	ttyp(double x) {
		__cnstr_cll("ttyp<double>(double)", "");
		__VAR_C_TYP();
		tid = "double", cmm_set(x);
	}
	ttyp(llong x) {
		__cnstr_cll("ttyp<llong>(llong)", "");
		__VAR_C_TYP();
		tid = "int", cmm_set(x);
	}
	inline void cmm_set(T x) {
		val = x, var::tid = tid;
#ifdef _VAR_DBG
		ddbg.sval = _sval, pdbg = &ddbg;
		upd_val();
#endif
	}
#ifdef _VAR_DBG
	inline void upd_val() {
		tcpps(val, _sval);
	}
#endif
	~ttyp() {
		__cnstr_cll("~ttyp<T>()", "");__VAR_D_TYP();
	}
	
	var operator+(const var &rv) const {
		if (rv.tid[0] == 'i') return //
		clc_prc(this, *(getPv<llong>(rv.pttyp)), 0, 0);
		else if (rv.tid[0] == 'd') return //
		clc_prc_db(this, *getPv<double>(rv.pttyp), 0, 0);
		else if (rv.tid[0] == 'b') return //
		clc_prc(this, (llong) *(getPv<bool>(rv.pttyp)), 0, 0);
		else if (rv.tid[0] == 's') return //
		std::to_string(val) += *getPv<str>(rv.pttyp);
		else if (rv.tid[0] == 'c') return //
		std::to_string(val) += *getPv<char>(rv.pttyp);
		else OE_solver(0, this, 0, 0, //
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return var();
	}
	var operator+(int rv) const {
		return clc_prc(this, rv, 0, 0);
	}
	var operator+(float rv) const {
		return clc_prc_db(this, rv, 0, 0);
	}
	var operator+(double rv) const {
		return clc_prc_db(this, rv, 0, 0);
	}
	var operator+(long rv) const {
		return clc_prc(this, rv, 0, 0);
	}
	var operator+(llong rv) const {
		return clc_prc(this, rv, 0, 0);
	}
	var operator+(ullong rv) const {
		__LL_exc()
		return clc_prc(this, (llong) rv, 0, 0);
	}
	var operator+(cch *rv) const {
		return std::to_string(val) += rv;
	}
	var operator+(str &rv) const {
		return std::to_string(val) += rv;
	}
	var operator+(str *rv) const {
		return std::to_string(val) += *rv;
	}
	var operator+(char rv) const {
		return std::to_string(val) += rv;
	}
	
	var& operator+=(const var &rv) {
		if (rv.tid[0] == 'i') return //
		*clc_prc(this, *(getPv<llong>(rv.pttyp)), 0, 1);
		else if (rv.tid[0] == 'd') return //
		*clc_prc_db(this, *(getPv<double>(rv.pttyp)), 0, 1);
		else if (rv.tid[0] == 'b') return //
		*clc_prc(this, (llong) *(getPv<bool>(rv.pttyp)), 0, 1);
		else if (rv.tid[0] == 's') return //
		*new ttyp<str>((std::to_string(val) += *getPv<str>(rv.pttyp)));
		else if (rv.tid[0] == 'c') return //
		*new ttyp<str>((std::to_string(val) += *getPv<char>(rv.pttyp)));
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return *this;
	}
	var& operator+=(int rv) {
		return *clc_prc(this, rv, 0, 1);
	}
	var& operator+=(float rv) {
		return *clc_prc_db(this, rv, 0, 1);
	}
	var& operator+=(double rv) {
		return *clc_prc_db(this, rv, 0, 1);
	}
	var& operator+=(long rv) {
		return *clc_prc(this, rv, 0, 1);
	}
	var& operator+=(llong rv) {
		return *clc_prc(this, rv, 0, 1);
	}
	var& operator+=(ullong rv) {
		__LL_exc()
		return *clc_prc(this, (llong) rv, 0, 1);
	}
	var& operator+=(cch *rv) {
		return *new ttyp<str>(std::to_string(val) += rv);
	}
	var& operator+=(str &rv) {
		return *new ttyp<str>(std::to_string(val) += rv);
	}
	var& operator+=(str *rv) {
		return *new ttyp<str>(std::to_string(val) += *rv);
	}
	var& operator+=(char rv) {
		return *new ttyp<str>(std::to_string(val) += rv);
	}
	
	var operator-(const var &rv) const {
		if (rv.tid[0] == 'i') return //
		clc_prc(this, *(getPv<llong>(rv.pttyp)), 2, 0);
		else if (rv.tid[0] == 'd') return //
		clc_prc_db(this, *(getPv<double>(rv.pttyp)), 2, 0);
		else if (rv.tid[0] == 'b') return //
		clc_prc(this, (llong) *(getPv<bool>(rv.pttyp)), 2, 0);
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return var();
	}
	var operator-(int rv) const {
		return clc_prc(this, rv, 2, 0);
	}
	var operator-(float rv) const {
		return clc_prc_db(this, rv, 2, 0);
	}
	var operator-(double rv) const {
		return clc_prc_db(this, rv, 2, 0);
	}
	var operator-(long rv) const {
		return clc_prc(this, rv, 2, 0);
	}
	var operator-(llong rv) const {
		return clc_prc(this, rv, 2, 0);
	}
	var operator-(ullong rv) const {
		__LL_exc()
		return clc_prc(this, (llong) rv, 2, 0);
	}
	
	var& operator-=(const var &rv) {
		if (rv.tid[0] == 'i') return	//
		*clc_prc(this, *(getPv<llong>(rv.pttyp)), 2, 1);
		else if (rv.tid[0] == 'd') return //
		*clc_prc_db(this, *(getPv<double>(rv.pttyp)), 2, 1);
		else if (rv.tid[0] == 'b') return //
		*clc_prc(this, (llong) *(getPv<bool>(rv.pttyp)), 2, 1);
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return *this;
	}
	var& operator-=(int rv) {
		return *clc_prc(this, rv, 2, 1);
	}
	var& operator-=(float rv) {
		return *clc_prc_db(this, rv, 2, 1);
	}
	var& operator-=(double rv) {
		return *clc_prc_db(this, rv, 2, 1);
	}
	var& operator-=(long rv) {
		return *clc_prc(this, rv, 2, 1);
	}
	var& operator-=(llong rv) {
		return *clc_prc(this, rv, 2, 1);
	}
	var& operator-=(ullong rv) {
		__LL_exc()
		return *clc_prc(this, (llong) rv, 2, 1);
	}
	
	var operator*(const var &rv) const {
		if (rv.tid[0] == 'i') return //
		clc_prc(this, *(getPv<llong>(rv.pttyp)), 1, 0);
		else if (rv.tid[0] == 'd') return //
		clc_prc_db(this, *(getPv<double>(rv.pttyp)), 1, 0);
		else if (rv.tid[0] == 'b') return //
		clc_prc(this, (llong) *(getPv<bool>(rv.pttyp)), 1, 0);
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return var();
	}
	var operator*(int rv) const {
		return clc_prc(this, rv, 1, 0);
	}
	var operator*(float rv) const {
		return clc_prc_db(this, rv, 1, 0);
	}
	var operator*(double rv) const {
		return clc_prc_db(this, rv, 1, 0);
	}
	var operator*(long rv) const {
		return clc_prc(this, rv, 1, 0);
	}
	var operator*(llong rv) const {
		return clc_prc(this, rv, 1, 0);
	}
	var operator*(ullong rv) const {
		__LL_exc()
		return clc_prc(this, (llong) rv, 1, 0);
	}
	
	var& operator*=(const var &rv) {
		if (rv.tid[0] == 'i') return //
		*clc_prc(this, *(getPv<llong>(rv.pttyp)), 1, 1);
		else if (rv.tid[0] == 'd') return //
		*clc_prc_db(this, *(getPv<double>(rv.pttyp)), 1, 1);
		else if (rv.tid[0] == 'b') return //
		*clc_prc(this, (llong) *(getPv<bool>(rv.pttyp)), 1, 1);
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return *this;
	}
	var& operator*=(int rv) {
		return *clc_prc(this, rv, 1, 1);
	}
	var& operator*=(float rv) {
		return *clc_prc_db(this, rv, 1, 1);
	}
	var& operator*=(double rv) {
		return *clc_prc_db(this, rv, 1, 1);
	}
	var& operator*=(long rv) {
		return *clc_prc(this, rv, 1, 1);
	}
	var& operator*=(llong rv) {
		return *clc_prc(this, rv, 1, 1);
	}
	var& operator*=(ullong rv) {
		__LL_exc()
		return *clc_prc(this, (llong) rv, 1, 1);
	}
	
#define __zero_chk() \
	   if (rv == 0) throw \
	   VarExcp(VarExcp::_mk_excp("zero_divide"));\

	
	var operator/(const var &_rv) const {
		if (_rv.tid[0] == 'i') {
			llong rv = *(getPv<llong>(_rv.pttyp));
			__zero_chk()
			return clc_prc_div_spc(this, rv, 0);
		} else if (_rv.tid[0] == 'd') {
			double rv = *(getPv<double>(_rv.pttyp));
			__zero_chk()
			return clc_prc_db(this, rv, 3, 0);
		} else if (_rv.tid[0] == 'b') {
			llong rv = *(getPv<bool>(_rv.pttyp));
			__zero_chk()
			return clc_prc_div_spc(this, rv, 0);
		} else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(_rv.gtid()) += " ]");
		return var();
	}
	var operator/(int rv) const {
		__zero_chk()
		return clc_prc_div_spc(this, rv, 0);
	}
	var operator/(float rv) const {
		__zero_chk()
		return clc_prc_db(this, rv, 3, 0);
	}
	var operator/(double rv) const {
		__zero_chk()
		return clc_prc_db(this, rv, 3, 0);
	}
	var operator/(long rv) const {
		__zero_chk()
		return clc_prc_div_spc(this, rv, 0);
	}
	var operator/(llong rv) const {
		__zero_chk()
		return clc_prc_div_spc(this, rv, 0);
	}
	var operator/(ullong rv) const {
		__zero_chk()
		__LL_exc()
		return clc_prc_div_spc(this, (llong) rv, 0);
	}
	
	var& operator/=(const var &_rv) {
		if (_rv.tid[0] == 'i') {
			llong rv = *(getPv<llong>(_rv.pttyp));
			__zero_chk()
			return *clc_prc_div_spc(this, rv, 1);
		} else if (_rv.tid[0] == 'd') {
			double rv = *(getPv<double>(_rv.pttyp));
			__zero_chk()
			return *clc_prc_db(this, rv, 3, 1);
		} else if (_rv.tid[0] == 'b') {
			llong rv = *(getPv<bool>(_rv.pttyp));
			__zero_chk()
			return *clc_prc_div_spc(this, rv, 1);
		} else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(_rv.gtid()) += " ]");
		return *this;
	}
	var& operator/=(int rv) {
		__zero_chk()
		return *clc_prc_div_spc(this, rv, 1);
	}
	var& operator/=(float rv) {
		__zero_chk()
		return *clc_prc_db(this, rv, 3, 1);
	}
	var& operator/=(double rv) {
		__zero_chk()
		return *clc_prc_db(this, rv, 3, 1);
	}
	var& operator/=(long rv) {
		__zero_chk()
		return *clc_prc_div_spc(this, rv, 1);
	}
	var& operator/=(llong rv) {
		__zero_chk()
		return *clc_prc_div_spc(this, rv, 1);
	}
	var& operator/=(ullong rv) {
		__zero_chk()
		__LL_exc()
		return *clc_prc_div_spc(this, (llong) rv, 1);
	}
	
	bool operator>(const var &rv) const {
		if (rv.tid[0] == 'i') return val > *(getPv<llong>(rv.pttyp));
		else if (rv.tid[0] == 'd') return val > *(getPv<double>(rv.pttyp));
		else if (rv.tid[0] == 'b') return val > *(getPv<bool>(rv.pttyp));
		else if (rv.tid[0] == 'c') return val > *(getPv<char>(rv.pttyp));
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator>(int rv) const {
		return val > rv;
	}
	bool operator>(float rv) const {
		return val > rv;
	}
	bool operator>(double rv) const {
		return val > rv;
	}
	bool operator>(long rv) const {
		return val > rv;
	}
	bool operator>(llong rv) const {
		return val > rv;
	}
	bool operator>(ullong rv) const {
		__LL_exc()
		return val > (llong) rv;
	}
	bool operator>(char rv) const {
		return val > rv;
	}
	
	bool operator<(const var &rv) const {
		if (rv.tid[0] == 'i') return val < *(getPv<llong>(rv.pttyp));
		else if (rv.tid[0] == 'd') return val < *(getPv<double>(rv.pttyp));
		else if (rv.tid[0] == 'b') return val < *(getPv<bool>(rv.pttyp));
		else if (rv.tid[0] == 'c') return val < *(getPv<char>(rv.pttyp));
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator<(int rv) const {
		return val < rv;
	}
	bool operator<(float rv) const {
		return val < rv;
	}
	bool operator<(double rv) const {
		return val < rv;
	}
	bool operator<(long rv) const {
		return val < rv;
	}
	bool operator<(llong rv) const {
		return val < rv;
	}
	bool operator<(ullong rv) const {
		__LL_exc()
		return val < (llong) rv;
	}
	bool operator<(char rv) const {
		return val < rv;
	}
	
	bool operator>=(const var &rv) const {
		if (rv.tid[0] == 'i') return val >= *(getPv<llong>(rv.pttyp));
		else if (rv.tid[0] == 'd') return val >= *(getPv<double>(rv.pttyp));
		else if (rv.tid[0] == 'b') return val >= *(getPv<bool>(rv.pttyp));
		else if (rv.tid[0] == 'c') return val >= *(getPv<char>(rv.pttyp));
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator>=(int rv) const {
		return val >= rv;
	}
	bool operator>=(float rv) const {
		return val >= rv;
	}
	bool operator>=(double rv) const {
		return val >= rv;
	}
	bool operator>=(long rv) const {
		return val >= rv;
	}
	bool operator>=(llong rv) const {
		return val >= rv;
	}
	bool operator>=(ullong rv) const {
		__LL_exc()
		return val >= (llong) rv;
	}
	bool operator>=(char rv) const {
		return val >= rv;
	}
	
	bool operator<=(const var &rv) const {
		if (rv.tid[0] == 'i') return val <= *(getPv<llong>(rv.pttyp));
		else if (rv.tid[0] == 'd') return val <= *(getPv<double>(rv.pttyp));
		else if (rv.tid[0] == 'b') return val <= *(getPv<bool>(rv.pttyp));
		else if (rv.tid[0] == 'c') return val <= *(getPv<char>(rv.pttyp));
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator<=(int rv) const {
		return val <= rv;
	}
	bool operator<=(float rv) const {
		return val <= rv;
	}
	bool operator<=(double rv) const {
		return val <= rv;
	}
	bool operator<=(long rv) const {
		return val <= rv;
	}
	bool operator<=(llong rv) const {
		return val <= rv;
	}
	bool operator<=(ullong rv) const {
		__LL_exc()
		return val <= (llong) rv;
	}
	bool operator<=(char rv) const {
		return val <= rv;
	}
	
	bool operator!() const {
		return !val;
	}
	
	bool operator==(const var &rv) const {
		if (rv.tid[0] == 'i') return val == *(getPv<llong>(rv.pttyp));
		else if (rv.tid[0] == 'd') return val == *(getPv<double>(rv.pttyp));
		else if (rv.tid[0] == 'b') return val == *(getPv<bool>(rv.pttyp));
		else if (rv.tid[0] == 'c') return val == *(getPv<char>(rv.pttyp));
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator==(int rv) const {
		return val == rv;
	}
	bool operator==(float rv) const {
		return val == rv;
	}
	bool operator==(double rv) const {
		return val == rv;
	}
	bool operator==(long rv) const {
		return val == rv;
	}
	bool operator==(llong rv) const {
		return val == rv;
	}
	bool operator==(ullong rv) const {
		__LL_exc()
		return val == (llong) rv;
	}
	bool operator==(char rv) const {
		return val == rv;
	}
	
	bool operator!=(const var &rv) const {
		if (rv.tid[0] == 'i') return val != *(getPv<llong>(rv.pttyp));
		else if (rv.tid[0] == 'd') return val != *(getPv<double>(rv.pttyp));
		else if (rv.tid[0] == 'b') return val != *(getPv<bool>(rv.pttyp));
		else if (rv.tid[0] == 'c') return val != *(getPv<char>(rv.pttyp));
		else OE_solver(0, this, 0, 0,	//
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator!=(int rv) const {
		return val != rv;
	}
	bool operator!=(float rv) const {
		return val != rv;
	}
	bool operator!=(double rv) const {
		return val != rv;
	}
	bool operator!=(long rv) const {
		return val != rv;
	}
	bool operator!=(llong rv) const {
		return val != rv;
	}
	bool operator!=(ullong rv) const {
		__LL_exc()
		return val != (llong) rv;
	}
	bool operator!=(char rv) const {
		return val != rv;
	}
	
	var& operator++() {
		if (val < LLONG_MAX) {
			++val; //                                 
			__upd_val();
		} else {
			throw VarExcp(VarExcp::_mk_excp("out_of_number_range", //
					"value > LLONG_MAX"));
		}
		return *this;
	}
	var operator++(int) {
		llong tmp = val;
		if (val < LLONG_MAX) {
			++val; //                                 
			__upd_val();
		} else {
			throw VarExcp(VarExcp::_mk_excp("out_of_number_range", //
					"value > LLONG_MAX"));
		}
		return tmp;
	}
	var& operator--() {
		if (val > LLONG_MIN) {
			--val; //                                 
			__upd_val();
		} else {
			throw VarExcp(VarExcp::_mk_excp("out_of_number_range", //
					"value < LLONG_MIN"));
		}
		return *this;
	}
	var operator--(int) {
		llong tmp = val;
		if (val > LLONG_MIN) {
			--val; //                                 
			__upd_val();
		} else {
			throw VarExcp(VarExcp::_mk_excp("out_of_number_range", //
					"value < LLONG_MIN"));
		}
		return tmp;
	}
	
	operator bool() noexcept {
		return val;
	}
	operator char() {
		return val;
	}
	operator int() {
		return val;
	}
	operator float() {
		return val;
	}
	operator double() {
		return val;
	}
	operator long() {
		return val;
	}
	operator llong() {
		return val;
	}
	operator ullong() {
		return val;
	}
	operator str() {
		return std::to_string(val);
	}
	// if:
	int size() const noexcept {
		return sizeof(val);
	}
	int _int() const {
		return val;
	}
	llong _llong() const {
		return val;
	}
	ullong _ullong() const {
		return val;
	}
	float _float() const {
		return val;
	}
	double _double() const {
		return val;
	}
	var _str() const {
		return std::to_string(val);
	}
	void prnt() const noexcept {
		std::cout << val << std::endl;
	}
	// hlp:
	var* _mkcopy() {
		return new ttyp<T>(val);
	}
	void setv(var &rv) {
		val = rv;
		__upd_val();
	}
	void setv(double rv) {
		val = rv;
		__upd_val();
	}
	void setv(llong rv) {
		val = rv;
		__upd_val();
	}
};

//---------------------------------------------- other-speciafiations 
//-------------------------------------------------------------------

template<> class ttyp<bool> : public var {
	
	friend class var;

private:
	bool val = 0;
	cch *tid = "bool";
#ifdef _VAR_DBG
	cch *_sval = 0;
	dbg ddbg;
#endif
	ttyp(bool x) {
		__cnstr_cll("ttyp<bool>(bool)", "");
		__VAR_C_TYP();
		val = x, var::tid = tid;
#ifdef _VAR_DBG
		pdbg = &ddbg;
		upd_val();
#endif
	}
#ifdef _VAR_DBG
	inline void upd_val() {
		if (val) _sval = "true";
		else _sval = "false";
		ddbg.sval = _sval;
	}
#endif
	~ttyp() {
		__cnstr_cll("~ttyp<bool>()", "");__VAR_D_TYP();
	}
	
#define __bool_slv(op)\
	var typ = (llong) val;\
	auto _typ = &typ.pttyp->operator op(rv);\
	if (typ.pttyp != _typ) {\
		return _typ;\
	}\
	return typ;\
	
	
#define __bool_slv_(op)\
    var typ = (llong) val;\
	auto _typ = &typ.pttyp->operator op(rv);\
	if (typ.pttyp != _typ) {\
		return *_typ;\
	}\
	auto ref = typ.pttyp;\
	typ.pttyp = 0;\
	return *ref;\
	
	
	var operator+(const var &rv) const {
		__bool_slv(+=);
	}
	var operator+(int rv) const {
		__bool_slv(+=);
	}
	var operator+(float rv) const {
		__bool_slv(+=);
	}
	var operator+(double rv) const {
		__bool_slv(+=);
	}
	var operator+(long rv) const {
		__bool_slv(+=);
	}
	var operator+(llong rv) const {
		__bool_slv(+=);
	}
	var operator+(ullong rv) const {
		__LL_exc()
		__bool_slv(+=);
	}
	var operator+(cch *rv) const {
		return std::to_string(val) += rv;
	}
	var operator+(str &rv) const {
		return std::to_string(val) += rv;
	}
	var operator+(str *rv) const {
		return std::to_string(val) += *rv;
	}
	var operator+(char rv) const {
		return std::to_string(val) += rv;
	}
	
	var& operator+=(const var &rv) {
		__bool_slv_(+=)
	}
	var& operator+=(int rv) {
		__bool_slv_(+=)
	}
	var& operator+=(float rv) {
		__bool_slv_(+=)
	}
	var& operator+=(double rv) {
		__bool_slv_(+=)
	}
	var& operator+=(long rv) {
		__bool_slv_(+=)
	}
	var& operator+=(llong rv) {
		__bool_slv_(+=)
	}
	var& operator+=(ullong rv) {
		__LL_exc()
		__bool_slv_(+=)
	}
	var& operator+=(cch *rv) {
		__bool_slv_(+=)
	}
	var& operator+=(str &rv) {
		__bool_slv_(+=)
	}
	var& operator+=(str *rv) {
		__bool_slv_(+=)
	}
	var& operator+=(char rv) {
		__bool_slv_(+=)
	}
	
	var operator-(const var &rv) const {
		__bool_slv(-=);
	}
	var operator-(int rv) const {
		__bool_slv(-=);
	}
	var operator-(float rv) const {
		__bool_slv(-=);
	}
	var operator-(double rv) const {
		__bool_slv(-=);
	}
	var operator-(long rv) const {
		__bool_slv(-=);
	}
	var operator-(llong rv) const {
		__bool_slv(-=);
	}
	var operator-(ullong rv) const {
		__LL_exc()
		__bool_slv(-=);
	}
	
	var& operator-=(const var &rv) {
		__bool_slv_(-=);
	}
	var& operator-=(int rv) {
		__bool_slv_(-=);
	}
	var& operator-=(float rv) {
		__bool_slv_(-=);
	}
	var& operator-=(double rv) {
		__bool_slv_(-=);
	}
	var& operator-=(long rv) {
		__bool_slv_(-=);
	}
	var& operator-=(llong rv) {
		__bool_slv_(-=);
	}
	var& operator-=(ullong rv) {
		__LL_exc()
		__bool_slv_(-=);
	}
	
	var operator*(const var &rv) const {
		__bool_slv(*=);
	}
	var operator*(int rv) const {
		__bool_slv(*=);
	}
	var operator*(float rv) const {
		__bool_slv(*=);
	}
	var operator*(double rv) const {
		__bool_slv(*=);
	}
	var operator*(long rv) const {
		__bool_slv(*=);
	}
	var operator*(llong rv) const {
		__bool_slv(*=);
	}
	var operator*(ullong rv) const {
		__LL_exc()
		__bool_slv(*=);
	}
	
	var& operator*=(const var &rv) {
		__bool_slv_(*=);
	}
	var& operator*=(int rv) {
		__bool_slv_(*=);
	}
	var& operator*=(float rv) {
		__bool_slv_(*=);
	}
	var& operator*=(double rv) {
		__bool_slv_(*=);
	}
	var& operator*=(long rv) {
		__bool_slv_(*=);
	}
	var& operator*=(llong rv) {
		__bool_slv_(*=);
	}
	var& operator*=(ullong rv) {
		__LL_exc()
		__bool_slv_(*=);
	}
	
	var operator/(const var &rv) const {
		__bool_slv(/=);
	}
	var operator/(int rv) const {
		__bool_slv(/=);
	}
	var operator/(float rv) const {
		__bool_slv(/=);
	}
	var operator/(double rv) const {
		__bool_slv(/=);
	}
	var operator/(long rv) const {
		__bool_slv(/=);
	}
	var operator/(llong rv) const {
		__bool_slv(/=);
	}
	var operator/(ullong rv) const {
		__LL_exc()
		__bool_slv(/=);
	}
	
	var& operator/=(const var &rv) {
		__bool_slv_(/=);
	}
	var& operator/=(int rv) {
		__bool_slv_(/=);
	}
	var& operator/=(float rv) {
		__bool_slv_(/=);
	}
	var& operator/=(double rv) {
		__bool_slv_(/=);
	}
	var& operator/=(long rv) {
		__bool_slv_(/=);
	}
	var& operator/=(llong rv) {
		__bool_slv_(/=);
	}
	var& operator/=(ullong rv) {
		__LL_exc()
		__bool_slv_(/=);
	}
	
	bool operator>(const var &rv) const {
		return const_cast<var&>(rv) < llong(val);
	}
	bool operator>(int rv) const {
		return llong(val) > rv;
	}
	bool operator>(float rv) const {
		return llong(val) > rv;
	}
	bool operator>(double rv) const {
		return llong(val) > rv;
	}
	bool operator>(long rv) const {
		return llong(val) > rv;
	}
	bool operator>(llong rv) const {
		return llong(val) > rv;
	}
	bool operator>(ullong rv) const {
		__LL_exc()
		return llong(val) > rv;
	}
	
	bool operator<(const var &rv) const {
		return const_cast<var&>(rv) > llong(val);
	}
	bool operator<(int rv) const {
		return llong(val) < rv;
	}
	bool operator<(float rv) const {
		return llong(val) < rv;
	}
	bool operator<(double rv) const {
		return llong(val) < rv;
	}
	bool operator<(long rv) const {
		return llong(val) < rv;
	}
	bool operator<(llong rv) const {
		return llong(val) < rv;
	}
	bool operator<(ullong rv) const {
		__LL_exc()
		return llong(val) < rv;
	}
	
	bool operator>=(const var &rv) const {
		return const_cast<var&>(rv) <= llong(val);
	}
	bool operator>=(int rv) const {
		return llong(val) >= rv;
	}
	bool operator>=(float rv) const {
		return llong(val) >= rv;
	}
	bool operator>=(double rv) const {
		return llong(val) >= rv;
	}
	bool operator>=(long rv) const {
		return llong(val) >= rv;
	}
	bool operator>=(llong rv) const {
		return llong(val) >= rv;
	}
	bool operator>=(ullong rv) const {
		__LL_exc()
		return llong(val) >= rv;
	}
	
	bool operator<=(const var &rv) const {
		return const_cast<var&>(rv) >= llong(val);
	}
	bool operator<=(int rv) const {
		return llong(val) <= rv;
	}
	bool operator<=(float rv) const {
		return llong(val) <= rv;
	}
	bool operator<=(double rv) const {
		return llong(val) <= rv;
	}
	bool operator<=(long rv) const {
		return llong(val) <= rv;
	}
	bool operator<=(llong rv) const {
		return llong(val) <= rv;
	}
	bool operator<=(ullong rv) const {
		__LL_exc()
		return llong(val) <= rv;
	}
	
	bool operator!() const {
		return !val;
	}
	
	bool operator==(const var &rv) const {
		return const_cast<var&>(rv) == llong(val);
	}
	bool operator==(int rv) const {
		return llong(val) == rv;
	}
	bool operator==(float rv) const {
		return llong(val) == rv;
	}
	bool operator==(double rv) const {
		return llong(val) == rv;
	}
	bool operator==(long rv) const {
		return llong(val) == rv;
	}
	bool operator==(llong rv) const {
		return llong(val) == rv;
	}
	bool operator==(ullong rv) const {
		__LL_exc()
		return llong(val) == rv;
	}
	
	bool operator!=(const var &rv) const {
		return const_cast<var&>(rv) != llong(val);
	}
	bool operator!=(int rv) const {
		return llong(val) != rv;
	}
	bool operator!=(float rv) const {
		return llong(val) != rv;
	}
	bool operator!=(double rv) const {
		return llong(val) != rv;
	}
	bool operator!=(long rv) const {
		return llong(val) != rv;
	}
	bool operator!=(llong rv) const {
		return llong(val) != rv;
	}
	bool operator!=(ullong rv) const {
		__LL_exc()
		return llong(val) != rv;
	}
	
	operator bool() noexcept {
		return val;
	}
	operator char() {
		return val;
	}
	operator int() {
		return val;
	}
	operator float() {
		return val;
	}
	operator double() {
		return val;
	}
	operator long() {
		return val;
	}
	operator llong() {
		return val;
	}
	operator ullong() {
		return val;
	}
	operator str() {
		return val ? "true" : "false";
	}
	// if:
	int size() const noexcept {
		return sizeof(val);
	}
	void prnt() const noexcept {
		std::cout << (val ? "true" : "false") << std::endl;
	}
	int _int() const {
		return val;
	}
	llong _llong() const {
		return val;
	}
	ullong _ullong() const {
		return val;
	}
	float _float() const {
		return val;
	}
	double _double() const {
		return val;
	}
	var _str() const {
		return val ? "true" : "false";
	}
	// hlp 
	var* _mkcopy() {
		return new ttyp<bool>(val);
	}
	void setv(var &rv) {
		val = (bool) rv;
		__upd_val();
	}
	void setv(bool rv) {
		val = rv;
		__upd_val();
	}
};
template<> class ttyp<char> : public var {
	
	friend class var;
	friend class ttyp<str> ;
	friend class ttyp<Ic_str> ;
	friend class ttyp<Irc_str> ;
	friend class ttyp<VMAP> ;

private:
	mutable char val = 0;
	cch *tid = "char";
	char *p1 = 0;
	char *p2 = 0;
	bool trg = 0;
	//
#ifdef _VAR_DBG
	char _sval[2] = { 0, 0 };
	dbg ddbg;
#endif
	ttyp(char x) {
		__cnstr_cll("ttyp<char>(char)", "");
		__VAR_C_TYP();
		val = x, var::tid = tid;
#ifdef _VAR_DBG
		upd_val();
		pdbg = &ddbg;
#endif
	}
#ifdef _VAR_DBG
	inline void upd_val() {
		_sval[0] = val;
		ddbg.sval = _sval;
	}
#endif
	~ttyp() {
		__cnstr_cll("~ttyp<char>()", val);__VAR_D_TYP();
	}
	
	var operator+(const var &rv) const {
		auto typ = new ttyp<str>(operator str() //
				+= rv.pttyp->operator str());
		return *typ;
	}
	var operator+(int rv) const {
		auto typ = new ttyp<str>(operator str() //
				+= std::to_string(rv));
		return *typ;
	}
	var operator+(float rv) const {
		auto typ = new ttyp<str>(operator str() //
				+= std::to_string(rv));
		return *typ;
	}
	var operator+(double rv) const {
		auto typ = new ttyp<str>(operator str() //
				+= std::to_string(rv));
		return *typ;
	}
	var operator+(long rv) const {
		auto typ = new ttyp<str>(operator str() //
				+= std::to_string(rv));
		return *typ;
	}
	var operator+(llong rv) const {
		auto typ = new ttyp<str>(operator str() //
				+= std::to_string(rv));
		return *typ;
	}
	var operator+(ullong rv) const {
		__LL_exc()
		auto typ = new ttyp<str>(operator str() //
				+= std::to_string(rv));
		return *typ;
	}
	var operator+(cch *rv) const {
		auto typ = new ttyp<str>(operator str() += rv);
		return *typ;
	}
	var operator+(str &rv) const {
		auto typ = new ttyp<str>(operator str() += rv);
		return *typ;
	}
	var operator+(str *rv) const {
		auto typ = new ttyp<str>(operator str() += *rv);
		return *typ;
	}
	var operator+(char rv) const {
		auto typ = new ttyp<str>(operator str() += rv);
		return *typ;
	}
	
	var& operator+=(const var &rv) {
		auto typ = new ttyp<str>(operator str() //
				+= rv.pttyp->operator str());
		return *typ;
	}
	var& operator+=(int rv) {
		auto typ = new ttyp<str>(operator str() //
				+= std::to_string(rv));
		return *typ;
	}
	var& operator+=(float rv) {
		auto typ = new ttyp<str>(operator str() //
				+= std::to_string(rv));
		return *typ;
	}
	var& operator+=(double rv) {
		auto typ = new ttyp<str>(operator str() //
				+= std::to_string(rv));
		return *typ;
	}
	var& operator+=(long rv) {
		auto typ = new ttyp<str>(operator str() //
				+= std::to_string(rv));
		return *typ;
	}
	var& operator+=(llong rv) {
		auto typ = new ttyp<str>(operator str() //
				+= std::to_string(rv));
		return *typ;
	}
	var& operator+=(ullong rv) {
		__LL_exc()
		auto typ = new ttyp<str>(operator str() //
				+= std::to_string(rv));
		return *typ;
	}
	var& operator+=(cch *rv) {
		auto typ = new ttyp<str>(operator str() += rv);
		return *typ;
	}
	var& operator+=(str &rv) {
		auto typ = new ttyp<str>(operator str() += rv);
		return *typ;
	}
	var& operator+=(str *rv) {
		auto typ = new ttyp<str>(operator str() += *rv);
		return *typ;
	}
	var& operator+=(char rv) {
		auto typ = new ttyp<str>(operator str() += rv);
		return *typ;
	}
	
	bool operator<(const var &rv) const {
		if (rv.tid[0] == 's') {
			return val < (*getPv<str>(rv.pttyp))[0];
		}
		return const_cast<var&>(rv) > llong(val);
	}
	bool operator<(char rv) const {
		return val < rv;
	}
	bool operator<(int rv) const {
		return llong(val) < rv;
	}
	bool operator<(long rv) const {
		return llong(val) < rv;
	}
	bool operator<(llong rv) const {
		return llong(val) < rv;
	}
	bool operator<(ullong rv) const {
		__LL_exc()
		return llong(val) < llong(rv);
	}
	bool operator<(cch *rv) const {
		return val < rv[0];
	}
	bool operator<(str &rv) const {
		return val < rv[0];
	}
	bool operator<(str *rv) const {
		return val < (*rv)[0];
	}
	
	bool operator>(const var &rv) const {
		if (rv.tid[0] == 's') {
			return val > (*getPv<str>(rv.pttyp))[0];
		}
		return rv < llong(val);
	}
	bool operator>(char rv) const {
		return val > rv;
	}
	bool operator>(int rv) const {
		return llong(val) > rv;
	}
	bool operator>(long rv) const {
		return llong(val) > rv;
	}
	bool operator>(llong rv) const {
		return llong(val) > rv;
	}
	bool operator>(ullong rv) const {
		__LL_exc()
		return llong(val) > llong(rv);
	}
	bool operator>(cch *rv) const {
		return val > rv[0];
	}
	bool operator>(str &rv) const {
		return val > rv[0];
	}
	bool operator>(str *rv) const {
		return val > (*rv)[0];
	}
	
	bool operator<=(const var &rv) const {
		if (rv.tid[0] == 's') {
			return val <= (*getPv<str>(rv.pttyp))[0];
		}
		return rv >= llong(val);
	}
	bool operator<=(char rv) const {
		return val <= rv;
	}
	bool operator<=(int rv) const {
		return llong(val) <= rv;
	}
	bool operator<=(long rv) const {
		return llong(val) <= rv;
	}
	bool operator<=(llong rv) const {
		return llong(val) <= rv;
	}
	bool operator<=(ullong rv) const {
		__LL_exc()
		return llong(val) <= llong(rv);
	}
	bool operator<=(cch *rv) const {
		return val <= rv[0];
	}
	bool operator<=(str &rv) const {
		return val <= rv[0];
	}
	bool operator<=(str *rv) const {
		return val <= (*rv)[0];
	}
	
	bool operator>=(const var &rv) const {
		if (rv.tid[0] == 's') {
			return val >= (*getPv<str>(rv.pttyp))[0];
		}
		return rv <= llong(val);
	}
	bool operator>=(char rv) const {
		return val >= rv;
	}
	bool operator>=(int rv) const {
		return llong(val) >= rv;
	}
	bool operator>=(long rv) const {
		return llong(val) >= rv;
	}
	bool operator>=(llong rv) const {
		return llong(val) >= rv;
	}
	bool operator>=(ullong rv) const {
		__LL_exc()
		return llong(val) >= llong(rv);
	}
	bool operator>=(cch *rv) const {
		return val >= rv[0];
	}
	bool operator>=(str &rv) const {
		return val >= rv[0];
	}
	bool operator>=(str *rv) const {
		return val >= (*rv)[0];
	}
	
	bool operator==(const var &rv) const {
		if (rv.tid[0] == 's') {
			return val == (*getPv<str>(rv.pttyp))[0];
		}
		return rv == llong(val);
	}
	bool operator==(char rv) const {
		return val == rv;
	}
	bool operator==(int rv) const {
		return llong(val) == rv;
	}
	bool operator==(long rv) const {
		return llong(val) == rv;
	}
	bool operator==(llong rv) const {
		return llong(val) == rv;
	}
	bool operator==(ullong rv) const {
		__LL_exc()
		return llong(val) == llong(rv);
	}
	bool operator==(cch *rv) const {
		return val == rv[0];
	}
	bool operator==(str &rv) const {
		return val == rv[0];
	}
	bool operator==(str *rv) const {
		return val == (*rv)[0];
	}
	
	bool operator!=(const var &rv) const {
		if (rv.tid[0] == 's') {
			return val != (*getPv<str>(rv.pttyp))[0];
		}
		return rv != llong(val);
	}
	bool operator!=(char rv) const {
		return val != rv;
	}
	bool operator!=(int rv) const {
		return llong(val) != rv;
	}
	bool operator!=(long rv) const {
		return llong(val) != rv;
	}
	bool operator!=(llong rv) const {
		return llong(val) != rv;
	}
	bool operator!=(ullong rv) const {
		__LL_exc()
		return llong(val) != llong(rv);
	}
	bool operator!=(cch *rv) const {
		return val != rv[0];
	}
	bool operator!=(str &rv) const {
		return val != rv[0];
	}
	bool operator!=(str *rv) const {
		return val != (*rv)[0];
	}
	
	bool operator!() const {
		return !val;
	}
	
	operator bool() noexcept {
		return val;
	}
	operator char() {
		return val;
	}
	operator int() {
		return val;
	}
	operator float() {
		return val;
	}
	operator double() {
		return val;
	}
	operator long() {
		return val;
	}
	operator llong() {
		return val;
	}
	operator str() {
		return str(1, this->val);
	}
	operator str() const {
		return str(1, this->val);
	}
	// if:
	int size() const noexcept {
		return sizeof(val);
	}
	void prnt() const noexcept {
		std::cout << val << std::endl;
	}
	int _int() const {
		try {
			return std::stoi(str(1, val));
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("conversion_err"));
		}
		return 0;
	}
	llong _llong() const {
		try {
			return std::stoll(str(1, val));
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("conversion_err"));
		}
		return 0;
	}
	ullong _ullong() const {
		try {
			return std::stoull(str(1, val));
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("conversion_err"));
		}
		return 0;
	}
	float _float() const {
		try {
			return std::stof(str(1, val));
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("conversion_err"));
		}
		return 0;
	}
	double _double() const {
		try {
			return std::stod(str(1, val));
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("conversion_err"));
		}
		return 0;
	}
	var _str() const {
		return str(1, val);
	}
	// hlp:    
	var* _mkcopy() {
		return new ttyp<char>(val);
	}
	void setv(var &rv) {
		ttyp<char> *p = (ttyp<char>*) rv.pttyp;
		if (p == this) {
			if (!trg) {
				val = p2[0] = p->p1[0];
			} else {
				val = p1[0] = p->p2[0];
			}
		} else {
			if (!trg) {
				if (p2) p2[0] = p->val;
			} else {
				if (p1) p1[0] = p->val;
			}
			val = p->val;
		}__upd_val();
	}
	void setv(char rv) {
		if (!trg) {
			if (p2) p2[0] = rv;
		} else {
			if (p1) p1[0] = rv;
		}
		val = rv;
		__upd_val();
	}
	void setv_sp(char rv) {
		val = rv;
		__upd_val();
	}
	char* get_poi() const {
		if (p1 || p2) {
			if (!trg) return p2;
			else return p1;
		}
		return 0;
	}
};

//----------------------------------------------------------- var:arr
//-------------------------------------------------------------------

template<> class ttyp<__vct<var>> : public var {
	
	friend class var;
	friend class ttyp<str> ;
	friend class ttyp<Ic_arr> ;
	friend class ttyp<Irc_arr> ;
	friend class ttyp<VMAP> ;

private:
	__vct<var> val;
	cch *tid = "arr";
	int isyn = 0;
	var_li itrs;

#ifdef _VAR_DBG
	cch *_sval = 0;
	dbg ddbg;
#endif
	
	ttyp(const ttyp<__vct<var>> &x) {
		__cnstr_cll("ttyp<__vct<var>>(const ttyp<__vct<var>>&)", "");
		__VAR_C_TYP();
		val = x.val;
		set_cmm();
	}
	ttyp(const __iniL <var> &x) :
			val(x) {
		
		__cnstr_cll("ttyp<__vct<var>>(__iniL<var>&)", "");
		__VAR_C_TYP();
		
		set_cmm();
	}
	ttyp(const __vct <var> &x) {
		__cnstr_cll("ttyp<__vct<var>>(const __vct<var>&)", "");
		__VAR_C_TYP();
		val = *const_cast<__vct<var>*>(&x);
		set_cmm();
	}
	~ttyp() {
		__cnstr_cll("~ttyp<__vct<var>>()", "");
		__VAR_D_TYP();
		set_ivalid_itrs();
	}
	
	inline void set_cmm() {
		var::tid = tid;
#ifdef _VAR_DBG
		pdbg = &ddbg;
		pdbg->sval = "[...]";
		upd_val();
#endif
	}
	
#ifdef _VAR_DBG
	inline void upd_val() {
	}
#endif
	
	var operator+(const var &rv) const {
		auto x = dcst<__vct<var>>(rv.pttyp);
		if (x) {
			int l1 = val.size(), l2 = x->val.size();
			int l3 = l1 + l2, i = 0, ii = 0;
			__vct<var> v(l3);
			while (i < l1) {
				v[ii] = val[i];
				++i, ++ii;
			}
			i = 0;
			while (i < l2) {
				v[ii] = x->val[i];
				++i, ++ii;
			}
			return var(v);
		} else throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
				"bad 'rv' [ var:" + var(rv.gtid()) + " ]"));
		return var();
	}
	var& operator+=(const var &rv) {
		auto x = dcst<__vct<var>>(rv.pttyp);
		if (x) {
			int l = x->val.size();
			if (l > 0) {
				int i = 0;
				while (i < l) {
					val.push_back(x->val[i]);
					++i;
				}
			}
			return *this;
		} else throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
				"bad 'rv' [ var:" + var(rv.gtid()) + " ]"));
		return *this; //do nothing
	}
	
	bool operator==(const var &rv) const {
		if (this == rv.pttyp) return true;
		return false;
	}
	bool operator!=(const var &rv) const {
		if (this != rv.pttyp) return true;
		return false;
	}
	bool operator!() const {
		if (val.size() == 0) return true;
		return false;
	}
	operator bool() noexcept {
		if (val.size() > 0) return true;
		return false;
	}
	
	var& operator[](ullong index) {
		auto l = val.size();
		if (l == 0 || index >= l) throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : var( { "bad 'index'", //
						" ( ", idx_solver(index), " )" }).join())));
		return val.at(index);
	}
	
	operator int() {
		return val.size();
	}
	operator float() {
		return val.size();
	}
	operator double() {
		return val.size();
	}
	operator long() {
		return val.size();
	}
	operator llong() {
		return val.size();
	}
	operator ullong() {
		return val.size();
	}
	operator str() {
		str s = "{";
		int l = val.size();
		if (l > 0) {
			int i = 0;
			while (i < l) {
				if (val[i].tid[0] == 's') s += "\"" + str(val[i]) + "\"";
				else if (val[i].tid[0] == 'c') s += "\'" + str(val[i]) + "\'";
				else if (val[i].tid[0] == 'u' && val[i].tid[1] == 'd') s += str("|udf|");
				else s += str(val[i]);
				s += ",";
				++i;
			}
			s.pop_back();
			s += "}";
			return s;
		} else {
			s += "}";
			return s;
		}
		return "";
	}
	operator __vct<var>*() {
		return &val;
	}
	// hlp:  
	var* _mkcopy() {
		auto x = new ttyp<__vct<var>>(*this);
		x->var::tid = tid;
#ifdef _VAR_DBG
		x->var::pdbg = &(x->ddbg);
#endif
		return x;
	}
	void setv(var &rv) {
		val = *getPv<__vct<var>>(rv.pttyp);
	}
	// common if:
	int size() const noexcept {
		return sizeof(val);
	}
	var _str() const {
		return //
		const_cast<ttyp<__vct<var>>*>(this)->operator str();
	}
	void prnt() const noexcept {
		str s;
		arr_prnt_rec(val, s, 0, 0);
		std::cout << s << std::endl;
	}
	// if:
	//Element access:
	var& at(ullong index) {
		auto l = val.size();
		if (l == 0 || index >= l) throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : var( { "bad 'index'", //
						" ( ", idx_solver(index), " )" }).join())));
		return val.at(index);
	}
	var& front() {
		auto l = val.size();
		if (l == 0) throw VarExcp(VarExcp::_mk_excp("out_of_range", "empty"));
		return val.at(0);
	}
	var& back() {
		auto l = val.size();
		if (l == 0) throw VarExcp(VarExcp::_mk_excp("out_of_range", "empty"));
		return val.at(val.size() - 1);
	}
	//Iterators:
	//-frw-
	var begin() {
		return var(val.begin(), this);
	}
	var end() {
		return var(val.end(), this);
	}
	//-rvr-
	var rbegin() {
		return var(val.rbegin(), this);
	}
	var rend() {
		return var(val.rend(), this);
	}
	//Capacity:
	bool empty() const {
		return val.empty();
	}
	ullong length() const {
		return val.size();
	}
	ullong max_size() const {
		return val.max_size();
	}
	void reserve(ullong new_cap) {
		try {
			++isyn;
			val.reserve(new_cap);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("capacity_error", //
					"'new_cap' > max_size"));
		}
	}
	ullong capacity() const {
		return val.capacity();
	}
	void shrink_to_fit() {
		++isyn;
		val.shrink_to_fit();
	}
	//Modifiers:
	void clear() {
		++isyn;
		val.clear();
	}
	//..
	void resize(ullong count) {
		try {
			++isyn;
			val.resize(count);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	void resize(ullong count, const var &X) {
		try {
			++isyn;
			val.resize(count, X);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	void resize(ullong count, int X) {
		try {
			++isyn;
			val.resize(count, (llong) X);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	void resize(ullong count, float X) {
		try {
			++isyn;
			val.resize(count, (double) X);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	void resize(ullong count, double X) {
		try {
			++isyn;
			val.resize(count, X);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	void resize(ullong count, long X) {
		try {
			++isyn;
			val.resize(count, (llong) X);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	void resize(ullong count, llong X) {
		try {
			++isyn;
			val.resize(count, X);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	void resize(ullong count, ullong X) {
		
		__LL_exc_p(X)
		
		try {
			++isyn;
			val.resize(count, (llong(X)));
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	void resize(ullong count, char X) {
		try {
			++isyn;
			val.resize(count, X);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	void resize(ullong count, cch *X) {
		try {
			++isyn;
			val.resize(count, X);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	void resize(ullong count, const str &X) {
		try {
			++isyn;
			val.resize(count, X);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	void resize(ullong count, __iniL<var> X) {
		try {
			++isyn;
			val.resize(count, X);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"'count' > max_size"));
		}
	}
	//..
	var insert(ullong index, const var &vitr_src_first, const var &vitr_src_last); //out
			
#define __ins_arr_rng_var() \
		auto l = val.size();\
		if ((l == 0 && index != 0) || index > l)\
		throw VarExcp(VarExcp::_mk_excp("out_of_range",\
	    var( { "bad 'index'"," ( ", idx_solver(index), " )" }).join()));
	
	var insert(ullong index, const var &X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"max_size"));
		}
	}
	var insert(ullong index, int X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"max_size"));
		}
	}
	var insert(ullong index, float X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"max_size"));
		}
	}
	var insert(ullong index, double X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"max_size"));
		}
	}
	var insert(ullong index, long X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"max_size"));
		}
	}
	var insert(ullong index, llong X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"max_size"));
		}
	}
	var insert(ullong index, ullong X) {
		
		__LL_exc_p(X)
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"max_size"));
		}
	}
	var insert(ullong index, char X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"max_size"));
		}
	}
	var insert(ullong index, cch *X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"max_size"));
		}
	}
	var insert(ullong index, const str &X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"max_size"));
		}
	}
	var insert(ullong index, __iniL<var> X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, __vct<var>(X)), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"max_size"));
		}
	}
	var insert(ullong index, ullong count, const var &X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, count, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var insert(ullong index, ullong count, int X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, count, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var insert(ullong index, ullong count, float X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, count, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var insert(ullong index, ullong count, double X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, count, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var insert(ullong index, ullong count, long X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, count, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var insert(ullong index, ullong count, llong X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, count, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var insert(ullong index, ullong count, ullong X) {
		
		__LL_exc_p(X)
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, count, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var insert(ullong index, ullong count, char X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, count, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var insert(ullong index, ullong count, cch *X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, count, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var insert(ullong index, ullong count, const str &X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, count, X), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var insert(ullong index, ullong count, __iniL<var> X) {
		
		__ins_arr_rng_var()
		
		try {
			++isyn;
			return var(val.insert(val.begin() += index, count, __vct<var>(X)), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	//..
	var erase(ullong index) {
		auto l = val.size();
		if (l == 0 || index >= l) throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : var( { "bad 'index'", //
						" ( ", idx_solver(index), " )" }).join())));
		++isyn;
		if (l > 1) return var(val.erase(val.begin() += index), this);
		else val.erase(val.begin() += index);
		return var();
	}
	var erase(ullong first, ullong last) {
		auto l = val.size();
		if (l == 0 || (first >= last || last > l))  //
		throw VarExcp(VarExcp::_mk_excp("out_of_range",  //
				(l == 0 ? "empty" : "bad: 'first' or 'last' or both")));
		++isyn;
		if ((l - (last - first)) > 1) //
		return var(val.erase(val.begin() += first, val.begin() += last), this);
		else val.erase(val.begin() += first, val.begin() += last);
		return var();
	}
	//..
	var push_back(const var &X) {
		try {
			++isyn;
			val.push_back(X);
			return var(val.end() -= 1, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var push_back(int X) {
		try {
			++isyn;
			val.push_back(X);
			return var(val.end() -= 1, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var push_back(float X) {
		try {
			++isyn;
			val.push_back(X);
			return var(val.end() -= 1, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var push_back(double X) {
		try {
			++isyn;
			val.push_back(X);
			return var(val.end() -= 1, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var push_back(long X) {
		try {
			++isyn;
			val.push_back(X);
			return var(val.end() -= 1, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var push_back(llong X) {
		try {
			++isyn;
			val.push_back(X);
			return var(val.end() -= 1, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var push_back(ullong X) {
		
		__LL_exc_p(X);
		
		try {
			++isyn;
			val.push_back(X);
			return var(val.end() -= 1, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var push_back(char X) {
		try {
			++isyn;
			val.push_back(X);
			return var(val.end() -= 1, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var push_back(cch *X) {
		try {
			++isyn;
			val.push_back(X);
			return var(val.end() -= 1, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var push_back(const str &X) {
		try {
			++isyn;
			val.push_back(X);
			return var(val.end() -= 1, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	var push_back( __iniL<var> X) {
		try {
			++isyn;
			val.push_back(X);
			return var(val.end() -= 1, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", //
					"index + 'count' > max_size"));
		}
	}
	//..
	var push_front(const var &X) {
		return insert(0, X);
	}
	var push_front(int X) {
		return insert(0, X);
	}
	var push_front(float X) {
		return insert(0, X);
	}
	var push_front(double X) {
		return insert(0, X);
	}
	var push_front(long X) {
		return insert(0, X);
	}
	var push_front(llong X) {
		return insert(0, X);
	}
	var push_front(ullong X) {
		__LL_exc_p(X)
		return insert(0, X);
	}
	var push_front(char X) {
		return insert(0, X);
	}
	var push_front(cch *X) {
		return insert(0, X);
	}
	var push_front(const str &X) {
		return insert(0, X);
	}
	var push_front( __iniL<var> X) {
		return insert(0, X);
	}
	//..	
	var pop_back() {
		var ret;
		auto l = val.size();
		if (l > 0) {
			++isyn;
			ret = val[l - 1];
			val.pop_back();
		} else throw VarExcp(VarExcp::_mk_excp("out_of_range", "empty"));
		
		return ret;
	}
	var pop_front() {
		var ret;
		auto l = val.size();
		if (l > 0) {
			++isyn;
			ret = val[0];
			auto a = val.begin(), b = val.begin(), e = val.end();
			var *_a, *_b;
			delete (*a).pttyp;
			(*a).pttyp = 0;
			++a;
			while (a != e) {
				_a = &(*a), _b = &(*b);
				_b->pttyp = _a->pttyp;
				_b->tid = _a->tid;
#ifdef _VAR_DBG
				_b->pdbg = _a->pdbg;
#endif
				++a;
				++b;
			}
			a--;
			(*a).pttyp = 0;
			val.resize(l - 1);
		} else {
			throw VarExcp(VarExcp::_mk_excp("out_of_range", "empty"));
		}
		
		return ret;
	}
	//..
	
#define __sort_rev_arr_rng()\
   	    auto l = val.size();\
		if (l == 0) throw VarExcp(VarExcp::_mk_excp("out_of_range", "empty"));\
	
#define __sort_arr_cmp()\
		if (cmp == 0) throw VarExcp(VarExcp::_mk_excp("bad_argument", "bad 'cmp'"));\
				
	
	var sort_cmp(bool (*cmp)(var &a, var &b), bool reverse) const {
		__sort_rev_arr_rng()
		__sort_arr_cmp()
		ttyp< __vct<var>> *ctx = new ttyp<__vct<var>>(val);
		_sort_cstm(ctx, cmp, reverse);
		return *ctx;
	}
	var& ssort_cmp(bool (*cmp)(var &a, var &b), bool reverse) {
		__sort_rev_arr_rng()
		__sort_arr_cmp()
		_sort_cstm(this, cmp, reverse);
		return *this;
	}
	var sort(bool reverse) const {
		__sort_rev_arr_rng()
		ttyp< __vct<var>> *ctx = new ttyp<__vct<var>>(val);
		_sort_def(ctx, reverse);
		return *ctx;
	}
	var& ssort(bool reverse) {
		__sort_rev_arr_rng()
		_sort_def(this, reverse);
		return *this;
	}
	void _sort_def(ttyp< __vct<var>> *ctx, bool reverse) const {
		
		var a = !reverse ? var(ctx->val.begin(), ctx) : var(ctx->val.rbegin(), ctx),  //
		e = !reverse ? var(ctx->val.end(), ctx) : var(ctx->val.rend(), ctx), b;
		var *_a, *_b, *x;
		bool ok = 0;
		while (a != e) {
			b = a;
			++b;
			while (b != e) {
				_a = &(*a), _b = &(*b);
				if (_a->tid[0] == 'b') {
					if (_b->tid[0] == 'b' && *_b < *_a) ok = true;
				} else if (_b->tid[0] == 'b') {
					ok = true;
				} else if (_a->tid[0] == 'i') {
					if ((_b->tid[0] == 'd' || _b->tid[0] == 'i') && *_b < *_a) ok = true;
				} else if (_a->tid[0] == 'd') {
					if ((_b->tid[0] == 'd' || _b->tid[0] == 'i') && *_b < *_a) ok = true;
				} else if (_a->tid[0] == 'c') {
					if (_b->tid[0] == 'c' && *_b < *_a) ok = true;
					else if (_b->tid[0] == 'd' || _b->tid[0] == 'i') ok = true;
				} else if (_a->tid[0] == 's') {
					if (_b->tid[0] == 's' && *_b < *_a) ok = true;
					else if (_b->tid[0] == 'd' || _b->tid[0] == 'i' || _b->tid[0] == 'c') //
					ok = true;
				} else if (_a->tid[0] == 'a') {
					if (_b->tid[0] == 'a' && _b->length() < _a->length()) ok = true;
					else if (_b->tid[0] == 'd' || _b->tid[0] == 'i' || _b->tid[0] == 'c' || //
							_b->tid[0] == 's') ok = true;
				} else if (_a->tid[0] == 'm') {
					if (_b->tid[0] == 'd' || _b->tid[0] == 'i' || _b->tid[0] == 'c' || //
							_b->tid[0] == 's' || _b->tid[0] == 'a') ok = true;
				} else if (_a->is_I_str()) {
					if (_b->tid[0] == 'd' || _b->tid[0] == 'i' || _b->tid[0] == 'c' || //
							_b->tid[0] == 's' || _b->tid[0] == 'a' || _b->tid[0] == 'm') ok = true;
				} else if (_a->is_Ir_str()) {
					if (_b->tid[0] == 'd' || _b->tid[0] == 'i' || _b->tid[0] == 'c' || //
							_b->tid[0] == 's' || _b->tid[0] == 'a' || _b->tid[0] == 'm' || //
							_b->is_I_str()) //
					ok = true;
				} else if (_a->is_I_arr()) {
					if (_b->tid[0] == 'd' || _b->tid[0] == 'i' || _b->tid[0] == 'c' || //
							_b->tid[0] == 's' || _b->tid[0] == 'a' || _b->tid[0] == 'm' || //
							_b->is_I_str() || _b->is_Ir_str()) ok = true;
				} else if (_a->is_Ir_arr()) {
					if (_b->tid[0] == 'd' || _b->tid[0] == 'i' || _b->tid[0] == 'c' || //
							_b->tid[0] == 's' || _b->tid[0] == 'a' || _b->tid[0] == 'm' || //
							_b->is_I_str() || _b->is_Ir_str() || _b->is_I_arr()) ok = true;
				} else if (_a->is_P_map()) {
					if (_b->tid[0] == 'd' || _b->tid[0] == 'i' || _b->tid[0] == 'c' || //
							_b->tid[0] == 's' || _b->tid[0] == 'a' || _b->tid[0] == 'm' || //
							_b->tid[0] == 'I') ok = true;
				} else if (_a->tid[0] == 'u') {
					if (_b->tid[0] == 'd' || _b->tid[0] == 'i' || _b->tid[0] == 'c' || //
							_b->tid[0] == 's' || _b->tid[0] == 'a' || _b->tid[0] == 'm' //
							|| _b->tid[0] == 'I' || _b->tid[0] == 'P') ok = true;
				}
				if (ok) {
					x = _a->pttyp;
					_a->pttyp = _b->pttyp;
					_a->tid = _b->tid;
					_b->pttyp = x;
					_b->tid = x ? x->tid : "udf";
#ifdef _VAR_DBG
					_a->pdbg = _b->pdbg;
					_b->pdbg = x ? x->pdbg : 0;
#endif
					ok = false;
				}
				++b;
			}
			++a;
		}
	}
	void _sort_cstm(ttyp< __vct<var>> *ctx, bool (*cmp)(var &a, var &b), bool reverse) const {
		
		var a = !reverse ? var(ctx->val.begin(), ctx) : var(ctx->val.rbegin(), ctx),  //
		e = !reverse ? var(ctx->val.end(), ctx) : var(ctx->val.rend(), ctx), b;
		var *_a, *_b, *x;
		while (a != e) {
			b = a;
			++b;
			while (b != e) {
				_a = &(*a), _b = &(*b);
				if (cmp(*_a, *_b)) {
					x = _a->pttyp;
					_a->pttyp = _b->pttyp;
					_a->tid = _b->tid;
					_b->pttyp = x;
					_b->tid = x->tid;
#ifdef _VAR_DBG
					_a->pdbg = _b->pdbg;
					_b->pdbg = x->pdbg;
#endif
				}
				++b;
			}
			++a;
		}
		return;
	}
	var sample(ullong num) const {
		
		auto l = val.size();
		if (l == 0 || num > l) throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : var( { "bad 'num'", //
						" ( ", idx_solver(num), " )" }).join())));
		
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(0, l - 1);
		ttyp< __vct<var>> *smp = new ttyp<__vct<var>>(__vct<var>());
		if (num == 0) num = l;
		
		ullong i = 0;
		while (i < num) {
			smp->push_back(val[dist(gen)]);
			++i;
		}
		
		return *smp;
	}
	var sample_unq(ullong num) const {
		
		auto l = val.size();
		if (l == 0 || num > l) throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				(l == 0 ? "empty" : var( { "bad 'num'", //
						" ( ", idx_solver(num), " )" }).join())));
		
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(0, l - 1);
		ttyp< __vct<var>> *smp = new ttyp<__vct<var>>(val);
		if (num == 0) num = l;
		
		ullong i = 0;
		int a, b;
		var *_x, *_y, *_z;
		
		while (i < l) {
			a = dist(gen);
			b = dist(gen);
			if (a != b) {
				_x = &smp->val[a];
				_y = &smp->val[b];
				_z = _x->pttyp;
				_x->pttyp = _y->pttyp;
				_x->tid = _y->tid;
				_y->pttyp = _z;
				_y->tid = _z ? _z->tid : "udf";
#ifdef _VAR_DBG
				_x->pdbg = _y->pdbg;
				_y->pdbg = _z ? _z->pdbg : 0;
#endif	
			}
			++i;
		}
		if (num != l) smp->val.resize(num);
		
		return *smp;
	}
	var reverse() {
		
		__sort_rev_arr_rng()
		
		var *_b, *_e, *_z;
		ttyp< __vct<var>> *ret = new ttyp<__vct<var>>(val);
		auto b = ret->val.begin(), e = ret->val.end();
		
		e--;
		while (b != e) {
			_b = &(*b), _e = &(*e);
			_z = _b->pttyp;
			_b->pttyp = _e->pttyp;
			_b->tid = _e->tid;
			_e->pttyp = _z;
			_e->tid = _z ? _z->tid : "udf";
#ifdef _VAR_DBG
			_b->pdbg = _e->pdbg;
			_e->pdbg = _z ? _z->pdbg : 0;
#endif	
			++b;
			if (b == e) break;
			--e;
		}
		
		return *ret;
	}
	var& sreverse() {
		
		__sort_rev_arr_rng()
		
		var *_b, *_e, *_z;
		auto b = val.begin(), e = val.end();
		
		e--;
		while (b != e) {
			_b = &(*b), _e = &(*e);
			_z = _b->pttyp;
			_b->pttyp = _e->pttyp;
			_b->tid = _e->tid;
			_e->pttyp = _z;
			_e->tid = _z ? _z->tid : "udf";
#ifdef _VAR_DBG
			_b->pdbg = _e->pdbg;
			_e->pdbg = _z ? _z->pdbg : 0;
#endif	
			++b;
			if (b == e) break;
			--e;
		}
		
		return *this;
	}
	bool equal(const var &src_first, const var &src_last, ullong index, //
			ullong count) const; // out
	var mismatch(const var &src_first, const var &src_last, ullong index, //
			ullong count) const; // out
	var _mismatch(const ttyp<__vct<var>> *ctx, const var &src_first, const var &src_last, //
			ullong index, ullong end, int rec) const; // out 
	//..
	
#define __fill_arr()\
	auto l = val.size();\
	if (index > l) \
	throw VarExcp(VarExcp::_mk_excp("out_of_range",\
			var( { "bad 'index'", " ( ", idx_solver(index), " )" }).join()));\
	if (count == 0) count = l - index;\
	ullong end;\
	if ((end = index + count) > l) {\
		try {\
			++isyn;\
			val.resize(end, X);\
		} catch (VarExcp &e) {\
			throw;\
		} catch (std::exception &e) {\
			throw VarExcp(VarExcp::_mk_excp("length_error",\
							"max_size"));\
		}\
		while (index < l) {\
			val[index] = X;\
			++index;\
		}\
	} else {\
		while (index < end) {\
			val[index] = X;\
			++index;\
		}\
	}\
	return *this;\
	
	
	var& fill(const var &X, ullong index, ullong count) {
		__fill_arr()
	}
	var& fill(int X, ullong index, ullong count) {
		__fill_arr()
	}
	var& fill(float X, ullong index, ullong count) {
		__fill_arr()
	}
	var& fill(double X, ullong index, ullong count) {
		__fill_arr()
	}
	var& fill(long X, ullong index, ullong count) {
		__fill_arr()
	}
	var& fill(llong X, ullong index, ullong count) {
		__fill_arr()
	}
	var& fill(ullong X, ullong index, ullong count) {
		__fill_arr()
	}
	var& fill(char X, ullong index, ullong count) {
		__fill_arr()
	}
	var& fill(cch *X, ullong index, ullong count) {
		__fill_arr()
	}
	var& fill(const str &X, ullong index, ullong count) {
		__fill_arr()
	}
	var& fill(__iniL<var> X, ullong index, ullong count) {
		__fill_arr()
	}
	//..
	var join(char sep) const;  //out
	var& sjoin(char sep);      //out
	void swap(var &with) {
		if (dcst< __vct<var>>(with.pttyp)) {
			++isyn;
			__vct<var> *x = with;
			val.swap(*x);
		} else throw VarExcp(VarExcp::_mk_excp("unsupported_type",  //
				"bad 'with' [ var:" + var(with.gtid()) + " ]"));
	}
	// hlp
	void arr_prnt_rec(const __vct <var> &x, str &s, int rec, int spc) const; //out;
	void join_rec(char sep, const var *el, ttyp<str> *x) const;
	void setv(const __iniL <var> &rv) {
		val = rv;
	}
	void setv(const __vct <var> &rv) {
		val = rv;
	}
	void set_ivalid_itrs();
	
};
//-iterators-frw-
template<> class ttyp<Ic_arr> : public var {
	
	friend class var;
	friend class ttyp<str> ;
	friend class ttyp<__vct<var>> ;
	friend class ttyp<VMAP> ;

private:

	mutable Ic_arr _Ic_arr;
	cch *tid = "I_arr";
	mutable ttyp<__vct<var>> *bs = 0;
	mutable var_li::idx *stg = 0;
	mutable int isyn = 0;
	mutable llong idx = 0;
	mutable llong end = 0;

#ifdef _VAR_DBG
	mutable dbg ddbg;
	void upd_val() const {
		if (idx < end) {
			ddbg.vval = const_cast<var*>(&*_Ic_arr);
			if (ddbg.vval->pttyp && ddbg.vval->pdbg) {
				ddbg.sval = ddbg.vval->pdbg->sval;
			}
		} else {
			ddbg.vval = 0;
			ddbg.sval = 0;
		}
	}
#endif
	
	ttyp(const Ic_arr &I, const ttyp<__vct<var>> *_bs, bool flg = 0) :
			_Ic_arr(I) {
		__cnstr_cll("ttyp<Ic_arr>(Ic_arr,const ttyp<__vct<var>>*)", "");
		__VAR_C_TYP();
		var::tid = tid;
		if (_bs) {
			bs = const_cast<ttyp<__vct<var>>*>(_bs);
			stg = bs->itrs.get();
			stg->val = this;
			if (!flg) isyn = bs->isyn;
			end = bs->val.size();
			idx = (&(*_Ic_arr) - &(*bs->val.begin()));
#ifdef _VAR_DBG
			upd_val();
			pdbg = &ddbg;
#endif
		}
	}
	~ttyp() {
		__cnstr_cll("~ttyp<Ic_arr>() ", "");
		__VAR_D_TYP();
		if (stg) bs->itrs.rem(stg);
	}
	
#define __is_valid() \
		if (!is_valid())\
		throw VarExcp(VarExcp::_mk_excp("invalid_iterator"));\
	
	
	var& operator *() {
		
		__is_valid()
		
		if (idx >= end) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				"[left/right-out]"));
		return const_cast<var&>(*_Ic_arr);
	}
	operator bool() noexcept {
		if (is_valid() && idx != end) return true;
		return false;
	}
	bool operator!() const {
		if (is_valid()) return false;
		return true;
	}
	
	bool operator !=(const var &rv) const {
		
		__is_valid()
		
		auto prv = dcst<Ic_arr>(rv.pttyp);
		if (prv) return _Ic_arr != prv->_Ic_arr;
		else OE_solver(0, this, 0, 0, //
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator ==(const var &rv) const {
		
		__is_valid()
		
		auto prv = dcst<Ic_arr>(rv.pttyp);
		if (prv) return _Ic_arr == prv->_Ic_arr;
		else OE_solver(0, this, 0, 0, //
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	
	var operator+(int rv) const {
		
		__is_valid()
		__itr_rng_pls_chk()
		
		return var(_Ic_arr + rv, bs);
	}
	var operator+(long rv) const {
		
		__is_valid()
		__itr_rng_pls_chk()
		
		return var(_Ic_arr + rv, bs);
	}
	var operator+(llong rv) const {
		
		__is_valid()
		__itr_rng_pls_chk()
		
		return var(_Ic_arr + rv, bs);
	}
	var operator+(ullong rv) const {
		
		__LL_exc()
		__is_valid()
		__itr_rng_pls_chk()
		
		return var(_Ic_arr + rv, bs);
	}
	
	var& operator+=(int rv) {
		
		__is_valid()
		__itr_rng_pls_chk()
		
		_Ic_arr += rv, idx += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(long rv) {
		
		__is_valid()
		__itr_rng_pls_chk()
		
		_Ic_arr += rv, idx += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(llong rv) {
		
		__is_valid()
		__itr_rng_pls_chk()
		
		_Ic_arr += rv, idx += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(ullong rv) {
		
		__LL_exc()
		__is_valid()
		__itr_rng_pls_chk()
		
		_Ic_arr += rv, idx += rv;
		__upd_val();
		return *this;
	}
	
	var operator-(int rv) const {
		
		__is_valid()
		__itr_rng_min_chk()
		
		return var(_Ic_arr - rv, bs);
	}
	var operator-(long rv) const {
		
		__is_valid()
		__itr_rng_min_chk()
		
		return var(_Ic_arr - rv, bs);
	}
	var operator-(llong rv) const {
		
		__is_valid()
		__itr_rng_min_chk()
		
		return var(_Ic_arr - rv, bs);
	}
	var operator-(ullong rv) const {
		
		__LL_exc()
		__is_valid()
		__itr_rng_min_chk()
		
		return var(_Ic_arr - rv, bs);
	}
	
	var& operator-=(int rv) {
		
		__is_valid()
		__itr_rng_min_chk()
		
		_Ic_arr -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	var& operator-=(long rv) {
		
		__is_valid()
		__itr_rng_min_chk()
		
		_Ic_arr -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	var& operator-=(llong rv) {
		
		__is_valid()
		__itr_rng_min_chk()
		
		_Ic_arr -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	var& operator-=(ullong rv) {
		
		__LL_exc()
		__is_valid()
		__itr_rng_min_chk()
		
		_Ic_arr -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	
	operator Ic_arr() {
		return _Ic_arr;
	}
	operator Ic_arr*() {
		return &_Ic_arr;
	}
	operator str() {
		str s = "| [var:I_arr]";
		if (is_valid()) {
			if (idx == end) {
				s += " at the end() |";
			} else {
				s += " val: ";
				if ((*_Ic_arr).is_itr()) {
					s += "[var:";
					s += (*_Ic_arr).tid;
					s += "]";
				} else {
					char tid = (*_Ic_arr).tid[0];
					if (tid == 's') {
						s += "\"";
						s += *_Ic_arr;
						s += "\"";
					} else if (tid == 'c') {
						s += "'";
						s += *_Ic_arr;
						s += "'";
					} else s += *_Ic_arr;
				}
				s += " |";
			}
		} else s += " is invalid |";
		return s;
	}
	// hlp
	var* _mkcopy() {
		is_valid(1);
		return new ttyp<Ic_arr>(_Ic_arr, bs, 1);
	}
	void setv(var &rv) {
		bs->itrs.rem(stg);
		auto x = dcst<Ic_arr>(rv.pttyp);
		_Ic_arr = x->_Ic_arr;
		bs = x->bs, idx = x->idx, end = x->end, isyn = x->isyn;
		stg = bs->itrs.get();
		stg->val = this;
		if (is_valid()) {
			__upd_val();
		}
	}
	
	var& operator++() {
		
		__is_valid()
		
		if (idx == end) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				"[right-out]"));
		++_Ic_arr, ++idx;
		__upd_val();
		return *this;
	}
	var operator++(int) {
		
		__is_valid()
		auto tmp = _Ic_arr;
		
		if (idx == end) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				"[right-out]"));
		++_Ic_arr, ++idx;
		__upd_val();
		return var(tmp, bs);
	}
	var& operator--() {
		
		__is_valid()
		
		if (idx == 0) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				"[left-out]"));
		--_Ic_arr, --idx;
		__upd_val();
		return *this;
	}
	var operator--(int) {
		
		__is_valid()
		auto tmp = _Ic_arr;
		
		if (idx == 0) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				"[left-out]"));
		--_Ic_arr, --idx;
		__upd_val();
		return var(tmp, bs);
	}
	// if:
	bool next() const {
		
		__is_valid()
		
		if (end > 0) {
			if (idx < end - 1) {
				const_cast<ttyp<Ic_arr>*>(this)->operator++();
				return true;
			}
		}
		return false;
	}
	llong pos() const {
		__is_valid()
		return idx;
	}
	inline bool is_valid(bool flg = 0) const {
		if (bs) {
			if (isyn != bs->isyn) {
				auto i = &*_Ic_arr;
				if (i >= &*bs->val.begin() && i <= &*bs->val.end()) {
					if (!flg) isyn = bs->isyn;
					return true;
				}
				bs->itrs.rem(stg);
				bs = 0, stg = 0;
#ifdef _VAR_DBG   
				ddbg.sval = 0, ddbg.vval = 0;
#endif
				return false;
			}
		} else return false;
		return true;
	}
	
	void prnt() const noexcept {
		if (is_valid()) {
			if (idx == end) {
				std::cout << "| [var:I_arr] at the end() |" << std::endl;
			} else {
				if ((*_Ic_arr).is_itr()) {
					std::cout << "[var:" //
							<< (*_Ic_arr).tid << "]" << std::endl;
				} else {
					std::cout << (const_cast<ttyp<Ic_arr>*>(this))->operator str() //
							<< std::endl;
				}
			}
		} else {
			std::cout << "| [var:I_arr] is invalid |" << std::endl;
		}
	}
	
};
//-iterators-rvr-
template<> class ttyp<Irc_arr> : public var {
	
	friend class var;
	friend class ttyp<str> ;
	friend class ttyp<__vct<var>> ;
	friend class ttyp<VMAP> ;

private:

	mutable Irc_arr _Irc_arr;
	cch *tid = "Ir_arr";
	mutable ttyp<__vct<var>> *bs = 0;
	mutable var_li::idx *stg = 0;
	mutable int isyn = 0;
	mutable llong idx = 0;
	mutable llong end = 0;

#ifdef _VAR_DBG
	mutable dbg ddbg;
	void upd_val() const {
		if (idx < end) {
			ddbg.vval = const_cast<var*>(&*_Irc_arr);
			if (ddbg.vval->pttyp && ddbg.vval->pdbg) {
				ddbg.sval = ddbg.vval->pdbg->sval;
			}
		} else {
			ddbg.vval = 0;
			ddbg.sval = 0;
		}
	}
#endif
	
	ttyp(const Irc_arr &I, const ttyp<__vct<var>> *_bs, bool flg = 0) :
			_Irc_arr(I) {
		__cnstr_cll("ttyp<Irc_arr>(Irc_arr,const ttyp<__vct<var>>*)", "");
		__VAR_C_TYP();
		var::tid = tid;
		if (_bs) {
			bs = const_cast<ttyp<__vct<var>>*>(_bs);
			if (!flg) isyn = bs->isyn;
			stg = bs->itrs.get();
			stg->val = this;
			end = bs->val.size();
			idx = (&(*bs->val.rbegin()) - &(*_Irc_arr));
#ifdef _VAR_DBG
			upd_val();
			pdbg = &ddbg;
#endif
		}
	}
	~ttyp() {
		__cnstr_cll("~ttyp<Irc_arr>() ", "");
		__VAR_D_TYP();
		if (stg) bs->itrs.rem(stg);
	}
	
#define __is_valid() \
		if (!is_valid())\
		throw VarExcp(VarExcp::_mk_excp("invalid_iterator"));\
	
	
	var& operator*() {
		
		__is_valid()
		
		if (idx >= end) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				"[left/right-out]"));
		return const_cast<var&>(*_Irc_arr);
	}
	operator bool() noexcept {
		if (is_valid() && idx != end) return true;
		return false;
	}
	bool operator!() const {
		if (is_valid()) return false;
		return true;
	}
	
	bool operator!=(const var &rv) const {
		
		__is_valid()
		
		auto prv = dcst<Irc_arr>(rv.pttyp);
		if (prv) return _Irc_arr != prv->_Irc_arr;
		else OE_solver(0, this, 0, 0, //
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator==(const var &rv) const {
		
		__is_valid()
		
		auto prv = dcst<Irc_arr>(rv.pttyp);
		if (prv) return _Irc_arr == prv->_Irc_arr;
		else OE_solver(0, this, 0, 0, //
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	
	var operator+(int rv) const {
		
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		return var(_Irc_arr + rv, bs);
	}
	var operator+(long rv) const {
		
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		return var(_Irc_arr + rv, bs);
	}
	var operator+(llong rv) const {
		
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		return var(_Irc_arr + rv, bs);
	}
	var operator+(ullong rv) const {
		
		__LL_exc()
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		return var(_Irc_arr + rv, bs);
	}
	
	var& operator+=(int rv) {
		
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		_Irc_arr += rv, idx += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(long rv) {
		
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		_Irc_arr += rv, idx += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(llong rv) {
		
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		_Irc_arr += rv, idx += rv;
		__upd_val();
		return *this;
	}
	var& operator+=(ullong rv) {
		
		__LL_exc()
		__is_valid()
		__itr_rng_pls_chk_rvr()
		
		_Irc_arr += rv, idx += rv;
		__upd_val();
		return *this;
	}
	
	var operator-(int rv) const {
		
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		return var(_Irc_arr - rv, bs);
	}
	var operator-(long rv) const {
		
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		return var(_Irc_arr - rv, bs);
	}
	var operator-(llong rv) const {
		
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		return var(_Irc_arr - rv, bs);
	}
	var operator-(ullong rv) const {
		
		__LL_exc()
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		return var(_Irc_arr - rv, bs);
	}
	
	var& operator-=(int rv) {
		
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		_Irc_arr -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	var& operator-=(long rv) {
		
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		_Irc_arr -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	var& operator-=(llong rv) {
		
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		_Irc_arr -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	var& operator-=(ullong rv) {
		
		__LL_exc()
		__is_valid()
		__itr_rng_min_chk_rvr()
		
		_Irc_arr -= rv, idx -= rv;
		__upd_val();
		return *this;
	}
	
	operator Irc_arr() {
		return _Irc_arr;
	}
	operator Irc_arr*() {
		return &_Irc_arr;
	}
	operator str() {
		str s = "| [var:Ir_arr]";
		if (is_valid()) {
			if (idx == end) {
				s += " at the rend() |";
			} else {
				s += " val: ";
				if ((*_Irc_arr).is_itr()) {
					s += "[var:";
					s += (*_Irc_arr).tid;
					s += "]";
				} else {
					char tid = (*_Irc_arr).tid[0];
					if (tid == 's') {
						s += "\"";
						s += *_Irc_arr;
						s += "\"";
					} else if (tid == 'c') {
						s += "'";
						s += *_Irc_arr;
						s += "'";
					} else s += *_Irc_arr;
				}
				s += " |";
			}
		} else s += " is invalid |";
		return s;
	}
	// hlp
	var* _mkcopy() {
		is_valid(1);
		return new ttyp<Irc_arr>(_Irc_arr, bs, 1);
	}
	void setv(var &rv) {
		bs->itrs.rem(stg);
		auto x = dcst<Irc_arr>(rv.pttyp);
		_Irc_arr = x->_Irc_arr;
		bs = x->bs, idx = x->idx, end = x->end, isyn = x->isyn;
		stg = bs->itrs.get();
		stg->val = this;
		if (is_valid()) {
			__upd_val();
		}
	}
	
	var& operator++() {
		
		__is_valid()
		
		if (idx == end) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				"[left-out]"));
		++_Irc_arr, ++idx;
		__upd_val();
		return *this;
	}
	var operator++(int) {
		
		__is_valid()
		auto tmp = _Irc_arr;
		
		if (idx == end) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				"[left-out]"));
		++_Irc_arr, ++idx;
		__upd_val();
		return var(tmp, bs);
	}
	var& operator--() {
		
		__is_valid()
		
		if (idx == 0) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				"[right-out]"));
		--_Irc_arr, --idx;
		__upd_val();
		return *this;
	}
	var operator--(int) {
		
		__is_valid()
		auto tmp = _Irc_arr;
		
		if (idx == 0) //
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				"[right-out]"));
		--_Irc_arr, --idx;
		__upd_val();
		return var(tmp, bs);
	}
	// if:
	bool next() const {
		
		__is_valid()
		
		if (end > 0) {
			if (idx < end - 1) {
				const_cast<ttyp<Irc_arr>*>(this)->operator++();
				return true;
			}
		}
		return false;
	}
	llong pos() const {
		__is_valid()
		return idx;
	}
	inline bool is_valid(bool flg = 0) const {
		if (bs) {
			if (isyn != bs->isyn) {
				auto ri = &*_Irc_arr;
				if (ri <= &*bs->val.rbegin() && ri >= &*bs->val.rend()) {
					if (!flg) isyn = bs->isyn;
					return true;
				}
				bs->itrs.rem(stg);
				bs = 0, stg = 0;
#ifdef _VAR_DBG   
				ddbg.sval = 0, ddbg.vval = 0;
#endif			
				return false;
			}
		} else return false;
		return true;
	}
	void prnt() const noexcept {
		if (is_valid()) {
			if (idx == end) {
				std::cout << "| [var:Ir_arr] at the rend() |" << std::endl;
			} else {
				std::cout << (const_cast<ttyp<Irc_arr>*>(this))->operator str() //
						<< std::endl;
			}
		} else {
			std::cout << "| [var:Ir_arr] is invalid |" << std::endl;
		}
	}
};

//----------------------------------------------------------- var:map 
//-------------------------------------------------------------------

struct var_map_ctx {
	str _str = "";
	int depth = 0;
	bool m = 0;
};
struct var_map_ctx_ {
	ttyp<str> *_str = 0;
	int depth = 0;
	bool m = 0;
};

template<> class ttyp<VMAP> : public var {
	
	friend class var;
	friend class ttyp<P_map> ;
	friend class ttyp<__vct<var>> ;
	friend class _VMAP ;
	friend _VMAP* var_map_get_val_ref(var *_x);

private:

	VMAP val;
	cch *tid = "map";
	var_li vmps;

#ifdef _VAR_DBG
	cch *_sval = 0;
	dbg ddbg;
#endif	
	
	ttyp() {
		__cnstr_cll("ttyp<MAP<var>>()", "");
		__VAR_C_TYP();
		set_cmm();
	}
	ttyp(const ttyp<VMAP> &x) {
		__cnstr_cll("ttyp<VMAP>(const ttyp<VMAP> &x)", "");
		__VAR_C_TYP();
		val = x.val;
	}
	~ttyp() {
		__cnstr_cll(
				"~ttyp<MAP<var>>()", "");
		__VAR_D_TYP();
		set_ivalid_vmp();
	}
	
	struct _get_prc {
		ttyp<VMAP> *x = 0;
		var *_y = 0;
		var *y = 0;
		char bf[vm_field + 1];
	};

	inline void set_cmm() {
		var::tid = tid;
#ifdef _VAR_DBG
		pdbg = &ddbg;
		upd_val();
#endif
	}
	
#ifdef _VAR_DBG
	inline void upd_val() {
	}
#endif
	
	// cmm
	var _str() const {
		return //
		const_cast<ttyp<VMAP>*>(this)->operator str();
	}
	void prnt() const noexcept {
		
		var_map_ctx _ctx;
		_prnt(_ctx);
		std::cout << _ctx._str << std::endl;
	}
	
	// op:
	bool operator==(const var &rv) const {
		if (this == rv.pttyp) return true;
		return false;
	}
	bool operator!=(const var &rv) const {
		if (this != rv.pttyp) return true;
		return false;
	}
	bool operator!() const {
		if (val.size() == 0) return true;
		return false;
	}
	operator bool() noexcept {
		if (val.size() > 0) return true;
		return false;
	}
	
	operator str() {
		var_map_ctx _ctx;
		_ctx.m = 1;
		_prnt(_ctx);
		return _ctx._str;
	}
	
	// if:
	var& map_set(cch *map) {
		
		parser(map);
		return *this;
	}
	var map_get(cch *map) const; // out	
	var map_rem(cch *map) {
		
		_get_prc prc;
		
		if (_map_get(map, &prc)) {
			prc.x->val.remId(prc.bf, [](auto idx, auto ctx) {
				delete idx->pttyp;
				idx->pttyp = 0;
				idx->tid = "udf";
#ifdef _VAR_DBG
				idx->pdbg = 0;
#endif				
			});
			
			prc.x->set_ivalid_vmp(prc.y);
			
			if (prc.x->val.size() == 0) {
				if (prc.x != this) {
					delete prc._y->pttyp;
					prc._y->pttyp = 0;
					prc._y->tid = "udf";
#ifdef _VAR_DBG
					prc._y->pdbg = 0;
#endif	
				}
			}
			return {1,0};
		} else {
			return {2,"map field hadn't exist"};
		}
		
		return {0,"unknown"};
	}
	var& map_free() {
		
		val.free();
		return *this;
	}
	void map_foreach(var::map (*fun)(char *field, const var &val, void *ctx), //
	void *ctx, int depth) const {
		
		struct CTX {
			var::map (*fun)(char *field, const var &val, void *ctx);
			void *ctx;
			bool depth;
			CTX(var::map (*_fun)(char *field, const var &val, void *ctx), //
			void *_ctx, bool _deep) :
					fun(_fun), ctx(_ctx), depth(_deep) {
			}
		} _ctx(fun, ctx, depth);
		
		val.foreach([](auto bf, auto *val, int rec_ud, auto tot, auto depth, //
				auto ctx) {
			
			CTX *_ctx = (CTX*) ctx;
			return _ctx->fun(bf, *val, _ctx->ctx);
			
		},depth, 1, &_ctx);
	}
	var map_fread(cch *fname) {
		
		std::ifstream f(fname, std::ios::ate);
		if (f.is_open()) {
			
			ull l = f.tellg();
			f.seekg(0, std::ios_base::beg);
			char *buf = new char[l + 2];
			f.read(buf, l);
			buf[f.gcount()] = 0;
			f.close();
			
			try {
				parser(buf);
				return {1,0};
			} catch (...) {
				delete[] buf;
				throw;
			}
		} else {
			return {0,"Unable to open file"};
		}
		return var();
	}
	var map_fwrite(cch *fname) const {
		
		var _str = map_str(1);
		
		std::ofstream f(fname, std::ios::trunc);
		if (f.is_open()) {
			
			f.write(_str.data(), _str.length());
			f.close();
			return {1,0};
			
		} else {
			return {0,"Unable to open file"};
		}
		
		return var();
	}
	var map_str(bool format) const {
		
		var_map_ctx_ _ctx;
		_ctx._str = new ttyp<str>("");
		_ctx._str->val.reserve(512);
		_ctx.m = !format ? 1 : 0;
		__prnt(_ctx);
		
#ifdef _VAR_DBG 
		_ctx._str->upd_val();
#endif
		return *_ctx._str;
	}
	
	struct idx {
		char *bf;
		ttyp<VMAP> *cur_lb = 0, *cur_rb = 0;
		var *v_l = 0, *v_r = 0;
	};
	struct _STK_ {
		idx _idx[vm_deep];
		int stk = 0;
		int err = 0;
		var path = "";
		var vmp1;
		var vmp2;
		bool m;
		const ttyp<VMAP> *th;
		_STK_(int _m = 0, const ttyp<VMAP> *_th = 0) :
				m(_m), th(_th) {
		}
	};
	bool map_equal(const var &map) const {
		
		_STK_ STK(1);
		_cmp(map, STK);
		if (STK.err != 0) return false;
		return true;
	}
	var map_mismatch(const var &map) const {
		
		_STK_ STK(0, this);
		_cmp(map, STK);
		if (STK.err != 0) {
			return {std::move(STK.path),std::move(STK.vmp1), //
				std::move(STK.vmp2)};
		}
		return false;
	}
	
	// hlp:
	cch* _scnt(char *bf, int cnt) {
		bf[cnt] = 0;
		return bf;
	}
	
#define _lft_max 128
	
	var mk_str(cch *bf, int i, int trg, int m = 0) const {
		
		int _i = i, lft = 0;
		char ch;
		var _str = "";
		
		// <---------- get start
		while (_i >= 0) {
			ch = bf[_i];
			if (ch == '\n' || _i == 0) break;
			else if (lft == _lft_max) {
				_str += " ... ";
				break;
			}
			--_i, ++lft;
		}
		if (ch == '\n') ++_i;
		// ----------> skip 
		while (_i < trg) {
			ch = bf[_i];
			if (ch != ' ' && ch != '\t') break;
			++_i;
		}
		// ----------> write
		while (_i <= trg) {
			_str += bf[_i];
			++_i;
		}
		
		if (m == 0) _str += " <<<\n";
		else if (m == 1) _str += " ???\n";
		
		return _str;
	}
	var mk_str_slsh_str(cch *bf, int i, int trg) const {
		
		int _i = i, lft = 0, x1 = 0, x2 = 0;
		char ch;
		var _str = "";
		
		// <---------- get start
		while (_i >= 0) {
			ch = bf[_i];
			if (ch == '\n' || _i == 0) break;
			else if (lft == _lft_max) {
				_str += " ... ";
				break;
			}
			if (!x1) {
				if (ch == '\\' && !x2) {
					x2 = _i;
				} else if (x2 && ch != '\\') {
					x1 = _i;
				}
			}
			--_i, ++lft;
		}
		if (ch == '\n') ++_i;
		// ----------> skip 
		while (_i < trg) {
			ch = bf[_i];
			if (ch != ' ' && ch != '\t') break;
			++_i;
		}
		// ----------> write
		while (_i <= trg) {
			
			if (_i <= x1 || _i > x2) {
				_str += bf[_i];
			} else if (_i == x2) _str += " ??? ";
			++_i;
		}
		
		_str += " <<<\n";
		
		return _str;
	}
	
#define __map_prs_err(err,row,str)\
	    var _err="";\
	    _err+=var(err)+=var("\n  [ row: ")+=var(row)+=var(" ]: ")+=str;\
		throw VarExcp(VarExcp::_mk_excp("map_parsing_error",_err));\
	
	
#define __dbl(s) \
	    ++i;\
		while ((ch = map[i]) != 0) {\
	        if(ch==s){\
	         __map_prs_err("repeating character: "#s"",row,mk_str(map,i,i))\
            }else if(ch!=' ') break;\
	     ++i;\
        }\
		if(ch==0) {--i;}\

	
#define _skp (ch==' ' || ch=='\t')	
#define _num (ch >= 48 && ch <= 57)	
	
	void parser(cch *map) {
		
		char ch;
		char bf[256]; // exc-text,map-fields,int,double
		int rs, z = 0, _z = 0, rec = 0, _rec = 0, cnt = 0;
		ttyp<VMAP> *_x = this;
		var *stk[vm_deep + 1], *x = 0;
		ttyp<VMAP> *stk_x[vm_deep + 1];
		int stk_rec[vm_deep + 1];
		int stk_z[vm_deep + 1];
		ullong i = 0, row = 1;
		bool mp = 0, rec_up = 0, ok = 0;
		
		if (map[i] == 0) {
			throw VarExcp(VarExcp::_mk_excp("map_parsing_error", //
					"empty map_set()"));
		}
		
		beg:
		
		while ((ch = map[i]) != 0) {
			
			if (ch == '{') {
				__dbl('{')
				++z, _rec = 0, _z = 1;
				continue;
			} else if (ch == ':') {
				if (cnt == 0) { //
					__map_prs_err("bad map field: 'empty'", //
							row, mk_str(map, i, i))
				}
				__dbl(':')
				rs = chk_map_val(map, i, row, bf, cnt);
				
				if (rs == 1) { // map
					mp = 1;
					++i;
					var_map_new(x);
					stk_x[rec] = _x;
					_x = (ttyp<VMAP>*) x->pttyp;
					x = _x->val.setId(bf);
					stk[rec] = x;
					stk_rec[rec] = _rec;
					stk_z[rec] = _z;
					++rec, ++_rec;
					rec_up = 0;
					goto beg;
					
				} else if (rs == 2) { // int
					try {
						*x = std::stoll(bf);
					} catch (std::exception &exc) {
						__map_prs_err(var("bad number: '") += var(bf) += //
								var("' (maybe too big)"), row, mk_str(map, i, i))
					}
				} else if (rs == 3) { // double	
					try {
						*x = std::stod(bf);
					} catch (std::exception &exc) {
						__map_prs_err(var("bad number: '") += var(bf) += //
								var("' (maybe too big)"), row, mk_str(map, i, i))
					}
				} else if (rs == 4) { // str/char
					*x = str_ch_parser(map, bf, i, row, cnt);
					ok = true;
				} else if (rs == 6) { // arr
					*x = arr_parser(map, bf, i, row, cnt);
				} else if (rs == 7) { // {
					mp = 1, ++z;
					__dbl('{')
					if ((rs = chk_map_val(map, i, row, bf, cnt)) == 1) {
						var_map_new(x);
						++i, _z = 1;
						stk_x[rec] = _x;
						_x = (ttyp<VMAP>*) x->pttyp;
						x = _x->val.setId(bf);
						stk[rec] = x;
						stk_rec[rec] = _rec;
						stk_z[rec] = _z;
						++rec, _rec = 0, rec_up = 0;
						goto beg;
					}
					continue;
				} else if (rs == 8) { // force back
					continue;
				} else if (rs == 9) { // pop up ';'
					continue;
				}
			} else if (ch == '}') {
				--z;
				if (z >= 0 && rec > 0) {
					--rec;
					x = stk[rec];
					_x = stk_x[rec];
					_rec = stk_rec[rec];
					_z = stk_z[rec];
					--_z;
					goto nxt;
				} else {
					if (rec == 0 && z == 0) {
						__map_prs_err(var("unbalanced '{ }': namely '") += var(z < 0 ? "}" //
								: "{") += "' (may be misuse of '<')", //
						row, mk_str(map, i, i))
					}
					__map_prs_err(var("unbalanced '{ }': namely '") //
							+= var(z < 0 ? "}" : "{") += "'", //
							row, mk_str(map, i, i))
				}
			} else if (ch == ';') {
				if (mp && cnt == 0 && !ok) {
					*x = var(); // set |udf|
				} else if (cnt == 0 && !rec_up && !ok) {
					__map_prs_err(var("unexpected character: '") //
							+= var(ch) += "'", row, mk_str(map, i, i))
				}
				cnt = 0, mp = 0, ok = 0;
				int _i = i;
				__dbl(';')
				i = _i;
				if (_rec > _z) {
					--i, --rec;
					x = stk[rec];
					_x = stk_x[rec];
					_rec = stk_rec[rec];
					_z = stk_z[rec];
					rec_up = true;
					goto nxt;
				} else rec_up = 0;
			} else if (ch == '<') { // force back
				int _i = i;
				while (_i >= 0) {
					if ((ch = map[_i]) != ' ' && ch != '<') {
						if (ch == '}' && rec > 0) {
							--rec;
							x = stk[rec];
							_x = stk_x[rec];
							_rec = stk_rec[rec];
							_z = stk_z[rec];
							goto nxt;
						} else break;
					}
					--_i;
				}
				if (rec == 0) {
					__map_prs_err("unexpected or too many: '<'", //
							row, mk_str(map, i, i))
				} else {
					__map_prs_err( //
							"force back: '<' used only after '}' -> '}<'",//
							row, mk_str(map, i, i))
				}
			} else if (_skp) {
				// ...
			} else if (ch == '\n') {
				++row;
			} else if (ch == '\r') {
				if (map[i + 1] != '\n') {
					__map_prs_err("unexpected '\\r' (carriage return)", //
							row, mk_str(map, i - 1, i - 1))
				}
			} else if (ch > 0 && ch <= 31) {
				__map_prs_err("unsupported ascii: 0 to 31 ( except: \\n,\\r,\\t )", //
						row, mk_str(map, i, i))
			} else {
				if ((rs = chk_map_val(map, i, row, bf, cnt)) != 1) {
					__map_prs_err(var("map field was expected: '") //
							+= var(bf) += "'", row, mk_str(map, i, i))
				}
				mp = 1;
				x = _x->val.setId(bf);
			}
			nxt:
			++i;
		}
		--i;
		if (rec > 0) {
			--rec;
			x = stk[rec];
			_x = stk_x[rec];
			_rec = stk_rec[rec];
			_z = stk_z[rec];
			goto nxt;
		}
		if (rec == 0 && z != 0) {
			__map_prs_err(var("unbalanced '{ }': namely '") += var(z < 0 ? "}" : "{") //
					+= "'", row, mk_str(map, i - 1, i - 1))
		}
		return;
	}
	int chk_map_val(cch *map, ullong &i, ullong &row, //
			char *bf, int &cnt) {
		
#define _cntr (ch==' ' || ch==';' || ch==':' || ch=='{' || \
         ch=='}' || ch=='.'  || ch=='\"')
#define _map ((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122) || (ch == 95))		
		
#define _unexp_end()\
		if(ch==0) {\
		if(cnt==0){\
		__map_prs_err("unexpected end of the data :(",\
			  row, mk_str(map,i-1,i-1,1))\
	    }\
		__map_prs_err(var("unexpected character(s): '") +=var(_scnt(bf,cnt))+=\
		"'",  row,mk_str(map,i,i-1))\
	    }\

		
#define __unexp_end(_i)\
		if(ch==0) {\
		if(cnt==0){\
		__map_prs_err("unexpected end of the data :(",\
		  				     row, mk_str(map,i,_i))\
		}\
		__map_prs_err(var("unexpected character(s): '") += var(_scnt(bf,cnt))+=\
		"'",  row,mk_str(map,i,_i))\
	    }\
			
		
#define _rd_while_(_ch)\
		while ((ch = map[i]) != 0) {\
		if (ch == _ch) break;\
		else if (_skp) {\
		} else if (ch == '\n') {\
		__map_prs_err(var("expected "#_ch" after '")\
		+= var(_scnt(bf, cnt)) += "' (before newline)",\
		 row, mk_str(map, i-1, i-2))\
		} \
		else if (ch == '\r') {\
		if (map[i + 1] != '\n') {\
	    __map_prs_err("unexpected '\\r' (carriage return)", \
		 row, mk_str(map, i-1, i-2))\
		}}\
		else {\
		cnt = 0;\
	    while ((ch = map[i]) != 0) {\
		if (ch == ' ' || ch == '\n') {\
		__map_prs_err(var("unexpected character(s): '")\
		+= var(_scnt(bf, cnt)) += "'", row, mk_str(map, i-1, i-1))\
	    } else {\
		bf[cnt] = ch, ++cnt;\
		}\
	    ++i;\
		}\
		_unexp_end()\
		}\
		++i;\
		}\
		_unexp_end()\
		
		
#define _unexp_map()\
	    if(!_num && !_map && !_cntr){\
	    if (ch == '\n' || (ch == '\r' && map[i+1]=='\n')) {\
	    __map_prs_err("unexpected break (map field)",\
	    row, mk_str(map,i-1,i-1))\
	    }\
		else if (ch == '\r') {\
		__map_prs_err("unexpected '\\r' (carriage return)",\
	    row,  mk_str(map,i-1,i-1))\
		}\
		__map_prs_err(var("unexpected character: '") += var(ch)\
	    += "' ( ex: ^[a-zA-z_][a-zA-Z_0-9]+ )",row,mk_str(map,i,i))\
	    }\
		
		
#define _val_field_slv(__i)\
	    _i=__i; \
		while ((ch = map[i]) != 0) {\
		if(_skp){}\
		else if(ch=='\n'){\
		__map_prs_err(var("expected ';' after '")\
		+= var(_scnt(bf, cnt)) += "' (before new line)",\
		row, mk_str(map, i-1, _i+cnt))\
	    }\
		else if (ch == '\r') {\
		if (map[i + 1] != '\n') {\
		__map_prs_err("unexpected '\\r' (carriage return)", \
		row, mk_str(map, i-1, _i+cnt))\
		}}\
	    else if(ch==':'){\
		__map_prs_err(var("bad map field: '")\
	    += var(_scnt(bf,cnt)) += "' ( ex: ^[a-zA-z_][a-zA-Z_0-9]+ )",\
		row,mk_str(map,i,_i+cnt))}\
	    else if(ch==';') {\
	    __map_prs_err(var("bad number: '")+= var(_scnt(bf,cnt)) += "'",\
	    row,mk_str(map,i,_i+cnt))}\
	    else {bf[cnt]=ch,++cnt;}\
			++i;\
		}\
		__unexp_end(_i+cnt)\
		
		
		cnt = 0;
		char ch = 0;
		int _i = 0;
		
		while ((ch = map[i]) != 0) {
			if (_map) { // map
				while ((ch = map[i]) != 0) {
					if (_map || _num) {
						bf[cnt] = ch, ++cnt;
					} else {
						if (ch != ' ' && ch != ':') {
							_unexp_map()
							_rd_while_(':')
						} else break;
					}
					++i;
				}
				_unexp_end()
				_i = i - 1;
				while ((ch = map[i]) != 0) {
					if (_skp) {
						// ...
					} else if (ch == ':') {
						--i;
						bf[cnt] = 0;
						return 1;
					} else {
						_rd_while_(':')
					}
					++i;
				}
				__unexp_end(_i)
			} else if (_num || (ch == '-')) { // num: int/double
				bool pnt = 0, sg = 0;
				if (ch == '-') {
					sg = 1;
					bf[cnt] = ch, ++cnt;
					++i;
				}
				while ((ch = map[i]) != 0) {
					if (_num) {
						bf[cnt] = ch, ++cnt;
					} else if (ch == '.') {
						if (pnt) {
							_val_field_slv((i - cnt) - 1)
						}
						bf[cnt] = ch, ++cnt;
						pnt = true;
					} else {
						if (ch != ' ' && ch != ';' && ch != '\t') {
							_val_field_slv((i - cnt) - 1)
						} else break;
					}
					++i;
				}
				_unexp_end()
				_i = i - 1;
				while ((ch = map[i]) != 0) {
					if (_skp) {
						// ...
					} else if (ch == ';') {
						--i;
						bf[cnt] = 0;
						if (sg && cnt == 1) {
							__map_prs_err("unexpected character: '-' ", //
									row, mk_str(map, _i, _i))
						}
						if (pnt) return 3; // double
						else return 2;     // int
					} else {
						_rd_while_(';')
					}
					++i;
				}
				__unexp_end(_i)
			} else if (ch == '\"') { //str or char
				return 4;
			} else if (ch == '[') { // arr
				//
				//
				__map_prs_err(var("unexpected character: '")				//
						+= var(ch) += "'", row, mk_str(map, i, i))
				return 6;
			} else if (ch == '{') { // block-map
				return 7;
			} else if (ch == '<') { // force back
				return 8;
			} else if (ch == ';') { // pop up ';'
				return 9;
			} else if (_skp) {
			} else if (ch == '\n') {
				__map_prs_err("unexpected '\\n' (new line) ", //
						row, mk_str(map, i - 1, i - 2))
			} else if (ch == '\r') {
				__map_prs_err("unexpected '\\r' (carriage return) ", //
						row, mk_str(map, i - 1, i - 2))
			} else if (ch == ':') {
				if (map[i + 1] == 0) {
					__map_prs_err("unexpected end of the data :(", //
							row, mk_str(map, i, i, 1))
				}
			} else if (ch > 0 && ch <= 31) {
				__map_prs_err("unsupported ascii: 0 to 31 ( except: \\n,\\r,\\t )", //
						row, mk_str(map, i, i))
			} else {
				__map_prs_err(var("unexpected character: '") //
						+= var(ch) += "'", row, mk_str(map, i, i))
			}
			++i;
		}
		_unexp_end()
		
		return 0;
		
		// return:  0 -> error 
		//          1 -> map
		//          2 -> int-val
		//          3 -> double-val
		//          4 -> str-val
		//          5 -> char-val -> 4
		//          6 -> arr-val
		//          7 -> '{' block-map-start
		//          8 -> '<' force back
		//          9 -> pop up ';'
		
	}
	var arr_parser(cch *map, char *bf, ullong &i, ullong &row, int &cnt) {
		
		/// NOT NOW !
		
		return var();
	}
	var str_ch_parser(cch *map, char *bf, ullong &i, ullong &row, int &cnt) {
		
#define __map_prs_err_str(err,row,str)\
	    var _err="";\
	    _err+=var(err)+=var("\n  [ row: ")+=var(row)+=var(" ]: ")+=str;\
		throw VarExcp(VarExcp::_mk_excp("map_parsing:string_parsing_error"\
		,_err));\
		
		
		cnt = 0;
		char ch = map[i];
		int z = 0, y = 0, slsh = 0, _i = 0;
		str _str;
		_str.reserve(512);
		
		if (ch == '\"') {
			
			++i, ++z;
			
			while ((ch = map[i]) != 0) {
				
				if (ch == '\"') {
					if (map[i - 1] != '\\') { // end  char
						_i = i;
						--z, ++i;
						while ((ch = map[i]) != 0) {
							if (ch == ' ') {
							} else if (ch != ';') {
								__map_prs_err_str("expected ';' after string value" //
										,row, mk_str(map, _i, _i))
							} else {
								--i;
								if (y == 1) return _str[0];
								else return _str;
							}
							++i;
						}
						if (ch == 0) {
							__map_prs_err_str("expected ';' after string value" //
									,row, mk_str(map, _i, _i))
						}
					}
				} else if (ch == '\\') {
					slsh = 0;
					while ((ch = map[i]) != 0) {
						
						if (ch != '\\') break;
						++i, ++slsh;
						
						if (slsh == 2) {
							_str += ch, ++y;
							slsh = 0;
						}
					}
					//
					if (ch == '\"') {
						if (slsh == 1) {
							_str += ch, ++y;
						} else {
							__map_prs_err_str(
									"unknown escape sequence: check slashes" "\n  note: \\\\\\\" -> \" , \\\\\\\\ -> \\  (const char* literals)" "\n        \\\\\"  -> \" , \\\\\\  -> \\  (file, direct 'map:var:s' assign)", //
									row, mk_str_slsh_str(map, i, i))
						}
					} else if (slsh > 0) {
						__map_prs_err_str(
								"unknown escape sequence: check slashes" "\n  note: \\\\\\\" -> \" , \\\\\\\\ -> \\  (const char* literals)" "\n        \\\\\"  -> \" , \\\\\\  -> \\  (file, direct 'map:var:s' assign)", //
								row, mk_str_slsh_str(map, i, i))
					}
					//
					if (ch == 0) goto end;
					continue;
					//... skip
				} else if (ch == '\n') {
					++row;
					_str += ch, ++y;
				} else {
					_str += ch, ++y;
				}
				++i;
			}
			end:
			__map_prs_err_str(var("end of data is reached" //
							" ( expected:  \"; or \\\"; (while inner use) )"),//
			row, mk_str(map, i - 1, i - 1, 1))
		}
		
		return var();
	}
	void _prnt(var_map_ctx &_ctx) const {
		
		if (const_cast<VMAP&>(val).size()) {
			
			val.foreach([](auto n, auto v, int rec_ud, ullong tot, //
					int depth, auto ctx) {
				
				var_map_ctx *_ctx = (var_map_ctx*) ctx;
				str &vl = _ctx->_str;
				bool m = _ctx->m;
				
				if (rec_ud == 1) {
					if (!m) {
						vl += "{ ", vl += n, vl += ": ";
						_ctx->depth += depth + 5;
					} else {
						vl += "{", vl += n, vl += ":";
					}
				} else if (rec_ud == 2) {
					if (!m) {
						_ctx->depth -= depth;
						_ctx->depth -= 5;
						vl.resize(vl.length() - (depth + 5));
						vl += "}\n";
						vl.append(_ctx->depth, ' ');
					} else vl += "}";
				} else if (v->tid[0] == 's') {
					vl += n;
					if (!m) {
						vl += ": \"";
					} else vl += ":\"";
					vl += (str) *v;
					if (!m) {
						vl += "\";\n";
						vl.append(_ctx->depth, ' ');
					} else vl += "\";";
					
				} else if (v->tid[0] == 'c') {
					vl += n;
					if (!m) {
						vl += ": '";
					} else vl += ":'";
					vl += (str) *v;
					if (!m) {
						vl += "';\n";
						vl.append(_ctx->depth, ' ');
					} else vl += "';";
					
				} else { // arr,int,double
					vl += n;
					if (!m) {
						vl += ": ";
					} else vl += ":";
					vl += (str) *v;
					if (!m) {
						vl += ";\n";
						vl.append(_ctx->depth, ' ');
					} else vl += ";";
					
				}
				return var::map::nxt;
			},0, 0, (void*) &_ctx);
		} else {
			_ctx._str = "{ }";
		}
	}
	void __prnt(var_map_ctx_ &_ctx) const {
		
		if (const_cast<VMAP&>(val).size()) {
			
			val.foreach([](auto n, auto v, int rec_ud, ullong tot, int depth, //
					auto ctx) {
				
				var_map_ctx_ *_ctx = (var_map_ctx_*) ctx;
				str &vl = _ctx->_str->val;
				bool m = _ctx->m;
				
				if (rec_ud == 1) {
					if (!m) {
						vl += "{ ", vl += n, vl += ": ";
						_ctx->depth += depth + 5;
					} else {
						vl += "{", vl += n, vl += ":";
					}
				} else if (rec_ud == 2) {
					if (!m) {
						_ctx->depth -= depth;
						_ctx->depth -= 5;
						vl.resize(vl.length() - (depth + 5));
						vl += "}\n";
						vl.append(_ctx->depth, ' ');
					} else vl += "}";
				} else if (v->tid[0] == 's') {
					vl += n;
					if (!m) vl += ": \"";
					else vl += ":\"";
					//-------------------------------------
					ttyp<str> *_v = (ttyp<str>*) v->pttyp;
					varF::slsh(vl, _v->val);
					//-------------------------------------
					if (!m) {
						vl += "\";\n";
						vl.append(_ctx->depth, ' ');
					} else vl += "\";";
					
				} else if (v->tid[0] == 'c') {
					vl += n;
					if (!m) vl += ": '";
					else vl += ":'";
					//---------------------------------------
					ttyp<char> *_v = (ttyp<char>*) v->pttyp;
					if (_v->val == '\\') {
						vl += "\\\\";
					} else if (_v->val == '\"') {
						vl += "\\\"";
					} else vl += _v->val;
					//---------------------------------------		
					if (!m) {
						vl += "';\n";
						vl.append(_ctx->depth, ' ');
					} else vl += "';";
					
				} else if (v->tid[0] == 'u') {
					vl += n;
					if (!m) {
						vl += ":;\n";
						vl.append(_ctx->depth, ' ');
					} else vl += ":;";
				} else if (v->tid[0] == 'I' || v->tid[0] == 'P' || v->tid[0] == 'a') {
					vl += n;
					if (!m) {
						vl += ":;\n";
						vl.append(_ctx->depth, ' ');
					} else vl += ":;";
				} else { // int,double  
					vl += n;
					if (!m) vl += ": ";
					else vl += ":";
					vl += (str) *v;
					if (!m) {
						vl += ";\n";
						vl.append(_ctx->depth, ' ');
					} else vl += ";";
				}
				return var::map::nxt;
			}, 0, 0, (void*) &_ctx);
		} else {
			_ctx._str->val = "{ }";
		}
	}
	str from_arr_prn(int depth) {
		
		var_map_ctx _ctx;
		_ctx.depth = depth + 1;
		
		_prnt(_ctx);
		
		int i = 0, _l = _ctx._str.length(), l = _l - 1;
		while (l >= 0) {
			if (_ctx._str[l] != ' ') {
				_ctx._str.resize((_l - i) - 1);
				break;
			}
			--l, ++i;
		}
		return _ctx._str;
	}
	var* _map_get(cch *map, _get_prc *prc = 0) const {
		
#define _rd_while()\
		while ((ch = map[i]) != 0) {\
			if(_skp || ch=='\n' || ch=='\r' || ch==':') break;\
			bf[cnt]=ch,++cnt,++i;\
		}\
		
		
		char bf[vm_field + 1];
		int i = 0, cnt = 0;
		char ch;
		ttyp<VMAP> *x = const_cast<ttyp<VMAP>*>(this);
		var *y = 0, *_y = 0;
		bool skp = 0;
		
		while ((ch = map[i]) != 0) {
			
			if (_map || _num) {
				if (skp) {
					cnt = 0;
					_rd_while()
					bf[cnt] = 0;
					__map_prs_err(var("unexpected character(s): '") //
							+= var(bf) += "'", //
							1, mk_str(map, i - 1, i - 1))
				}
				if (cnt == 0) {
					if (_num) {
						_rd_while()
						bf[cnt] = 0;
						__map_prs_err(var("bad map field: '") += var(bf) += //
								var("' ( ex: ^[a-zA-z_][a-zA-Z_0-9]+ )"), //
						1, mk_str(map, i - 1, i - 1))
					}
				}
				
				bf[cnt] = ch, ++cnt;
				
			} else if (ch == ':') {
				skp = 0;
				if (map[i + 1] == 0) goto exc;
				if (cnt) {
					bf[cnt] = 0;
					y = x->val.getId(bf);
					if (!y) goto end;
					if (y->tid[0] != 'm') {
						__map_prs_err("there is no path further :(", 1, //
								mk_str(map, i - 1, i - 1))
					}
					_y = y;
					x = (ttyp<VMAP>*) y->pttyp;
					cnt = 0; //, y = 0;
				} else goto exc;
			} else if (_skp) {
				if (cnt > 0) skp = 1;
			} else if (ch == '\n') {
				__map_prs_err("unexpected character: '\\n'", 1, //
						mk_str(map, i - 1, i - 1))
			} else if (ch == '\r') {
				__map_prs_err("unexpected character: '\\r'", 1, //
						mk_str(map, i - 1, i - 1))
			} else {
				__map_prs_err(var("unexpected character: '") //
						+= var(ch) += "'", 1, mk_str(map, i, i))
			}
			++i;
		}
		if (cnt) {
			bf[cnt] = 0;
			y = x->val.getId(bf);
		}
		
		end:
		if (prc) {
			varF::ccpy(prc->bf, bf);
			prc->x = x;
			prc->_y = _y;
			prc->y = y;
		}
		return y;
		
		exc:
		__map_prs_err("bad format ( ex: [ xxx:yyy:zzz ... ] )", //
				1, mk_str(map, i, i))
		
		return 0;
	}
	void var_map_new(var *x) {
		
		if (!x->pttyp || x->tid[0] != 'm') {
			delete x->pttyp;
			ttyp<VMAP> *typ = new ttyp<VMAP>();
			x->tid = typ->tid, x->pttyp = typ;
			
#ifdef _VAR_DBG 
			x->pdbg = typ->pdbg;
			x->pdbg->sval = "{...}";
#endif
		}
	}
	var* _mkcopy() {
		
		auto x = new ttyp<VMAP>(*this);
		x->var::tid = tid;
#ifdef _VAR_DBG
		x->var::pdbg = &(x->ddbg);
#endif
		return x;
		
	}
	void setv(var &rv) {
		
		auto _x = (ttyp<VMAP>*) rv.pttyp;
		val = _x->val;
	}
	void set_ivalid_vmp(); // out
	void set_ivalid_vmp(var *v); // out 		
	void _cmp(const var &map, _STK_ &STK) const {
		//
		// m = 0 -> mismatch
		// m = 1 -> equal
		//
		
		STK._idx[STK.stk].cur_lb = const_cast<ttyp<VMAP>*>(this);
		STK._idx[STK.stk].cur_rb = (ttyp<VMAP>*) map.pttyp;
		
		val.foreach([](auto n, auto v_l, int rec_ud, ull tot, int deep, auto ctx) {
			
			_STK_ *STK = (_STK_*) ctx;
			var *v_r;
			
			STK->_idx[STK->stk].bf = n;
			STK->_idx[STK->stk].v_l = v_l;
			
			if (rec_ud == 1) {
				
				v_r = STK->_idx[STK->stk].cur_rb->val.getId(n);
				STK->_idx[STK->stk].v_r = v_r;
				
				if (v_r) {
					if (v_r->tid[0] == 'm') {
						
						if (((ttyp<VMAP>*) (v_l->pttyp))->val.size() ==		//
								((ttyp<VMAP>*) (v_r->pttyp))->val.size()) {
							++STK->stk;
							STK->_idx[STK->stk].cur_lb = (ttyp<VMAP>*) v_l->pttyp;
							STK->_idx[STK->stk].cur_rb = (ttyp<VMAP>*) v_r->pttyp;
							
						} else {
							STK->err = 1; // bad size
							if (!STK->m) {
								STK->th->_cmp_set_dt(STK);
							}
							return var::map::brk;
						}
					} else {
						STK->err = 2; // rval not map / !=type
						if (!STK->m) {
							STK->th->_cmp_set_dt(STK);
						}
						return var::map::brk;
					}
				} else {
					STK->err = 3; // field mismatch
					if (!STK->m) {
						STK->th->_cmp_set_dt(STK);
					}
					return var::map::brk;
				}
			} else if (rec_ud == 2) {
				--STK->stk;
			} else {
				
				v_r = STK->_idx[STK->stk].cur_rb->val.getId(n);
				STK->_idx[STK->stk].v_r = v_r;
				
				if (v_r) {
					if (varF::cmp(v_l->tid, v_r->tid)) {
			
						if (v_l->tid[0] == 'a') {
							auto x = (ttyp<__vct<var>>*) v_l->pttyp;
							if (!x->equal(v_r->begin(), v_r->end(), 0, 0)) {
								STK->err = 3; // !=val
								if (!STK->m) {
									STK->th->_cmp_set_dt(STK);
								}
							}
						} else if (v_l->tid[0] == 'I') {
							if (v_l->tid[2] == 'a' || v_l->tid[3] == 'a') {
								if (v_l->tid[1] == 'r') {
									auto x = (ttyp<Irc_arr>*) v_l->pttyp;
									auto y = (ttyp<Irc_arr>*) v_r->pttyp;
									if (&(*x->_Irc_arr) != &(*y->_Irc_arr)) {
										STK->err = 3; // !=val
										if (!STK->m) {
											STK->th->_cmp_set_dt(STK);
										}
									}
								} else {
									auto x = (ttyp<Ic_arr>*) v_l->pttyp;
									auto y = (ttyp<Ic_arr>*) v_r->pttyp;
									if (&(*x->_Ic_arr) != &(*y->_Ic_arr)) {
										STK->err = 3; // !=val
										if (!STK->m) {
											STK->th->_cmp_set_dt(STK);
										}
									}
								}
							} else { //str
								if (v_l->tid[1] == 'r') {
									auto x = (ttyp<Irc_str>*) v_l->pttyp;
									auto y = (ttyp<Irc_str>*) v_r->pttyp;
									if (&(*x->_Irc_str) != &(*y->_Irc_str)) {
										STK->err = 3; // !=val
										if (!STK->m) {
											STK->th->_cmp_set_dt(STK);
										}
									}
								} else {
									auto x = (ttyp<Ic_str>*) v_l->pttyp;
									auto y = (ttyp<Ic_str>*) v_r->pttyp;
									if (&(*x->_Ic_str) != &(*y->_Ic_str)) {
										STK->err = 3; // !=val
										if (!STK->m) {
											STK->th->_cmp_set_dt(STK);
										}
									}
								}
							}
						} else if (v_l->tid[0] == 'u') {
							//.. ok
						} else {
							if (*v_l != *v_r) {
								STK->err = 3; // !=val
								if (!STK->m) {
									STK->th->_cmp_set_dt(STK);
								}
								return var::map::brk;
							}
						}
					} else {
						STK->err = 5; // err: !=type
						if (!STK->m) {
							STK->th->_cmp_set_dt(STK);
						}
						return var::map::brk;
					}
				} else {
					STK->err = 3; // field mismatch
					if (!STK->m) {
						STK->th->_cmp_set_dt(STK);
					}
					return var::map::brk;
				}
			}
			return var::map::nxt;
		},0, 0, &STK);
	}
	inline void _cmp_set_dt(_STK_ *STK) const; // out
			
};
template<> class ttyp<P_map> : public var {
	
	friend class var;
	friend class ttyp<VMAP> ;

	var *val = 0;
	cch *tid = "P_map";
	mutable ttyp<VMAP> *bs = 0;
	mutable var_li::idx *stg = 0;

#ifdef _VAR_DBG
	cch *_sval = 0;
	dbg ddbg;
#endif	
	
	ttyp(var *x, const ttyp<VMAP> *_bs) {
		__cnstr_cll("ttyp<P_map>(var *_x, const ttyp<VMAP>)", "");
		__VAR_C_TYP();
		bs = const_cast<ttyp<VMAP>*>(_bs);
		stg = bs->vmps.get();
		stg->val = this;
		val = x;
		set_cmm();
	}
	~ttyp() {
		__cnstr_cll("~ttyp<P_map>()", "");
		__VAR_D_TYP();
		if (stg) {
			bs->vmps.rem(stg);
		}
	}
	
	inline void set_cmm() {
		var::tid = tid;
#ifdef _VAR_DBG
		upd_val();
		pdbg = &ddbg;
#endif
	}
	
#ifdef _VAR_DBG
	inline void upd_val() {
		if (val) {
			ddbg.vval = val;
			if (val->pdbg) ddbg.sval = val->pdbg->sval;
		}
	}
#endif
	
	// op:
	bool operator!=(const var &rv) const {
		
		if (val == 0) throw VarExcp(VarExcp::_mk_excp("invalid_map_pointer", //
				"field not exist or became invalid"));
		
		auto prv = dcst<P_map>(rv.pttyp);
		if (prv) {
			if (val != prv->val) return true;
		} else OE_solver(0, this, 0, 0, //
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	bool operator==(const var &rv) const {
		
		if (val == 0) throw VarExcp(VarExcp::_mk_excp("invalid_map_pointer", //
				"field not exist or became invalid"));
		
		auto prv = dcst<P_map>(rv.pttyp);
		if (prv) {
			if (val == prv->val) return true;
		} else OE_solver(0, this, 0, 0, //
				"unsupported 'rv' [ var:" + var(rv.gtid()) += " ]");
		return false;
	}
	var& operator*() {
		if (val) return *val;
		throw VarExcp(VarExcp::_mk_excp("invalid_map_pointer", //
				"field not exist or became invalid"));
		return *this;
	}
	const var& operator*() const {
		if (val) return *val;
		throw VarExcp(VarExcp::_mk_excp("invalid_map_pointer", //
				"field not exist or became invalid"));
		return *this;
	}
	operator bool() noexcept {
		if (val) return true;
		return false;
	}
	bool operator!() const {
		if (!val) return true;
		return false;
	}
	operator str() {
		str s = "| [var:P_map]";
		if (val) {
			s += " val: ";
			if ((*val).is_itr()) {
				s += "[var:";
				s += (*val).tid;
				s += "]";
			} else {
				char tid = (*val).tid[0];
				if (tid == 's') {
					s += "\"";
					s += *val;
					s += "\"";
				} else if (tid == 'c') {
					s += "'";
					s += *val;
					s += "'";
				} else s += *val;
			}
			s += " |";
		} else s += " is invalid |";
		return s;
	}
	
	// hlp
	var* _mkcopy() {
		
		auto x = new ttyp<P_map>(val, bs);
		x->var::tid = tid;
#ifdef _VAR_DBG
		x->var::pdbg = &(x->ddbg);
		upd_val();
#endif
		return x;
	}
	void setv(var &rv) {
		auto _x = (ttyp<P_map>*) rv.pttyp;
		val = _x->val;
		__upd_val();
	}
	void prnt() const noexcept {
		if (val) {
			std::cout << "| [var:P_map] val: " << (*val)._str() << " |" << std::endl;
		} else {
			std::cout << "| [var:P_map] is invalid |" << std::endl;
		}
	}
	
};

inline VMAP* var_map_get_val_ref(var *_x) {
	
	ttyp<VMAP> *x = (ttyp<VMAP>*) _x;
	return &x->val;
}

//--------------------------------------------------- ttyp<VMAP> (out)
//--------------------------------------------------------------------

var ttyp<VMAP>::map_get(cch *map) const {
	_get_prc prc;
	_map_get(map, &prc);
	return *(new ttyp<P_map>(prc.y, prc.x));
}
void ttyp<VMAP>::set_ivalid_vmp() {
	var_li::idx *_idx;
	ttyp<P_map> *x;
	_idx = vmps.fst;
	while (_idx) {
		x = (ttyp<P_map>*) _idx->val;
		x->bs = 0, x->stg = 0, x->val = 0;
#ifdef _VAR_DBG   
		x->ddbg.sval = 0, x->ddbg.vval = 0;
#endif
		_idx = _idx->nxt;
	}
}
void ttyp<VMAP>::set_ivalid_vmp(var *v) {
	var_li::idx *_idx;
	ttyp<P_map> *x;
	_idx = vmps.fst;
	while (_idx) {
		x = (ttyp<P_map>*) _idx->val;
		if (x->val == v) {
			x->bs = 0, x->stg = 0, x->val = 0;
#ifdef _VAR_DBG   
			x->ddbg.sval = 0, x->ddbg.vval = 0;
#endif
			vmps.rem(_idx);
			break;
		}
		_idx = _idx->nxt;
	}
}
inline void ttyp<VMAP>::_cmp_set_dt(_STK_ *STK) const {
	
	STK->path.reserve(255);
	int i = 0, stk = STK->stk;
	while (i <= STK->stk) {
		STK->path += STK->_idx[i].bf;
		if (i != STK->stk) {
			STK->path += ':';
		}
		i++;
	}
	ttyp<P_map> *pL = new ttyp<P_map>(STK->_idx[stk].v_l, STK->_idx[stk].cur_lb);
	STK->vmp1.tid = pL->tid;
	STK->vmp1.pttyp = pL;
	ttyp<P_map> *pR = new ttyp<P_map>(STK->_idx[stk].v_r, STK->_idx[stk].cur_rb);
	STK->vmp2.tid = pR->tid;
	STK->vmp2.pttyp = pR;
	
#ifdef _VAR_DBG
	STK->vmp1.pdbg = &pL->ddbg;
	STK->vmp2.pdbg = &pR->ddbg;
#endif
	
}

//---------------------------------------------------------- var (out)
//--------------------------------------------------------------------

#ifdef _VAR_DBG 
#define __bset_x pdbg = typ->pdbg; 
#else 
#define __bset_x
#endif 

#define __bset(typ) \
    tid = typ->tid, pttyp = typ;\
    __bset_x \

#define __free_ttyp(pttyp) \
    { delete pttyp; \
	pttyp = 0;} \

var::var() {
	__cnstr_cll("var()", "");
	__VAR_C_TOP();
	tid = "udf";
}
var::var(bool x) {
	__cnstr_cll("var(int)", "");
	__VAR_C_TOP();
	ttyp<bool> *typ = new ttyp<bool>(x);
	__bset(typ);
}
var::var(int x) {
	__cnstr_cll("var(int)", "");
	__VAR_C_TOP();
	ttyp<llong> *typ = new ttyp<llong>((llong) x);
	__bset(typ);
}
var::var(long x) {
	__cnstr_cll("var(long)", "");
	__VAR_C_TOP();
	ttyp<llong> *typ = new ttyp<llong>((llong) x);
	__bset(typ);
}
var::var(llong x) {
	__cnstr_cll("var(llong)", "");
	__VAR_C_TOP();
	ttyp<llong> *typ = new ttyp<llong>((llong) x);
	__bset(typ);
}
var::var(ullong x) {
	__cnstr_cll("var(ullong)", "");
	__VAR_C_TOP();
	if (x > LLONG_MAX) //
	throw VarExcp(VarExcp::_mk_excp("out_of_number_range", //
			"constructor: var(ullong)", "value > LLONG_MAX"), 1);
	ttyp<llong> *typ = new ttyp<llong>((llong) x);
	__bset(typ);
}
var::var(double x) {
	__cnstr_cll("var(double)", "");
	__VAR_C_TOP();
	ttyp<double> *typ = new ttyp<double>((double) x);
	__bset(typ);
}
var::var(float x) {
	__cnstr_cll("var(float)", "");
	__VAR_C_TOP();
	ttyp<double> *typ = new ttyp<double>((double) x);
	__bset(typ);
}
template<typename t> var::var(const ttyp<t> &__pttyp) { //inner use
	__cnstr_cll("var<t>(const ttyp<t>&)", "");
	__VAR_C_TOP();
	var *typ = const_cast<ttyp<t>*>(&__pttyp);
	__bset(typ)
}
var::var(var *x) {
	__cnstr_cll("var<t>(var*)", "");
	__VAR_C_TOP();
	tid = x->tid, pttyp = x;
#ifdef _VAR_DBG 
	pdbg = x->pdbg;
#endif 
}
var::var(str &x) {
	__cnstr_cll("var(str&)", "");
	__VAR_C_TOP();
	ttyp<str> *typ = new ttyp<str>(x);
	__bset(typ);
}
var::var(const str &x) {
	__cnstr_cll("var(const str&): ", "");
	__VAR_C_TOP();
	ttyp<str> *typ = new ttyp<str>(const_cast<str&>(x));
	__bset(typ);
}
var::var(cch *x) {
	__cnstr_cll("var(cch*)", "");
	__VAR_C_TOP();
	if (x) {
		ttyp<str> *typ = new ttyp<str>(x);
		__bset(typ);
	} else {
		tid = "udf";
	}
}
var& var::operator=(var *rv) {	//inner use
	pttyp = rv->pttyp;
	tid = rv->tid;
#ifdef _VAR_DBG
	pdbg = rv->pdbg;
#endif
	return *this;
}
var::var(char *x) {
	__cnstr_cll("var(char*)", "");
	__VAR_C_TOP();
	ttyp<str> *typ = new ttyp<str>(x);
	__bset(typ);
}
var::var(char x) {
	__cnstr_cll("var(char)", "");
	__VAR_C_TOP();
	ttyp<char> *typ = new ttyp<char>(x);
	__bset(typ);
}
var::var(__iniL<var> x) {
	__cnstr_cll("var(_iniL<var>)", "");
	__VAR_C_TOP();
	ttyp< __vct<var>> *typ = new ttyp<__vct<var>>(x);
	__bset(typ);
}
var::var(const __vct <var> &x) { //inner use
	__cnstr_cll("var<t>(const __vct< t >&)", "");
	__VAR_C_TOP();
	ttyp< __vct<var>> *typ = new ttyp<__vct<var>>(x);
	__bset(typ);
}
//
var::var(var &x) {
	__cnstr_cll("var(var&)", "");
	__VAR_C_TOP();
	if (x.pttyp != 0) {
		var *typ = const_cast<var&>(x).pttyp->_mkcopy();
		__bset(typ)
	} else tid = "udf";
}
var::var(const var &x) {
	__cnstr_cll("var(const var&)", "");
	__VAR_C_TOP();
	if (x.pttyp != 0) {
		var *typ = const_cast<var&>(x).pttyp->_mkcopy();
		__bset(typ)
	} else tid = "udf";
}
var::var(var &&x) {
	__cnstr_cll("var(const var&&)", "");
	__VAR_C_TOP();
	var *typ = x.pttyp;
	if (typ) {
		x.pttyp = 0;
		__bset(typ)
	} else tid = "udf";
}
var::~var() {
	__cnstr_cll("~var()", "");
	__VAR_D_TOP();
	if (pttyp != 0) __free_ttyp(pttyp);
	tid = 0;
}
//
var::var(const I_str &I, const ttyp<str> *bs) { //inner use
	__cnstr_cll("var(const I_str&,ttyp<str>*)", "");
	__VAR_C_TOP();
	ttyp<Ic_str> *typ = new ttyp<Ic_str>(I, bs);
	__bset(typ);
}
var::var(const Ic_str &I, const ttyp<str> *bs) { //inner use
	__cnstr_cll("var(const Ic_str&,ttyp<str>*)", "");
	__VAR_C_TOP();
	ttyp<Ic_str> *typ = new ttyp<Ic_str>(I, bs);
	__bset(typ);
}
var::var(const Ir_str &I, const ttyp<str> *bs) { //inner use
	__cnstr_cll("var(const Ir_str&,ttyp<str>*)", "");
	__VAR_C_TOP();
	ttyp<Irc_str> *typ = new ttyp<Irc_str>(I, bs);
	__bset(typ);
}
var::var(const Irc_str &I, const ttyp<str> *bs) { //inner use
	__cnstr_cll("var(const Irc_str&,ttyp<str>*)", "");
	__VAR_C_TOP();
	ttyp<Irc_str> *typ = new ttyp<Irc_str>(I, bs);
	__bset(typ);
}
var::var(const I_arr &I, const ttyp<__vct<var>> *bs) { //inner use
	__cnstr_cll("var(const I_arr&, __vct<var>&)", "");
	__VAR_C_TOP();
	ttyp<Ic_arr> *typ = new ttyp<Ic_arr>(I, bs);
	__bset(typ);
}
var::var(const Ic_arr &I, const ttyp<__vct<var>> *bs) { //inner use
	__cnstr_cll("var(const Ic_arr&, __vct<var>&)", "");
	__VAR_C_TOP();
	ttyp<Ic_arr> *typ = new ttyp<Ic_arr>(I, bs);
	__bset(typ);
}
var::var(const Ir_arr &I, const ttyp<__vct<var>> *bs) { //inner use
	__cnstr_cll("var(const Ir_arr&, __vct<var>&)", "");
	__VAR_C_TOP();
	ttyp<Irc_arr> *typ = new ttyp<Irc_arr>(I, bs);
	__bset(typ);
}
var::var(const Irc_arr &I, const ttyp<__vct<var>> *bs) { //inner use
	__cnstr_cll("var(const Irc_arr&, __vct<var>&)", "");
	__VAR_C_TOP();
	ttyp<Irc_arr> *typ = new ttyp<Irc_arr>(I, bs);
	__bset(typ);
}
//
var& var::operator=(const var &rv) {
	if (rv.pttyp != 0) {
		if (pttyp != rv.pttyp) {
			if (!pttyp || !varF::cmp(tid, rv.tid)) {
				var *typ = const_cast<var&>(rv).pttyp->_mkcopy();
				if (pttyp) __free_ttyp(pttyp);
				__bset(typ)
			} else { //same  
				pttyp->setv(const_cast<var&>(rv));
			}
		} else if (tid[0] == 'c') pttyp->setv(const_cast<var&>(rv));
	} else {
		if (pttyp) __free_ttyp(pttyp)
		tid = "udf";
#ifdef _VAR_DBG
		pdbg = 0;
#endif
		
	}
	return *this;
}
var& var::operator=(var &rv) {
	if (rv.pttyp != 0) {
		if (pttyp != rv.pttyp) {
			if (!pttyp || !varF::cmp(tid, rv.tid)) {
				var *typ = const_cast<var&>(rv).pttyp->_mkcopy();
				if (pttyp != 0) __free_ttyp(pttyp)
				__bset(typ)
			} else { //same  
				pttyp->setv(rv);
			}
		} else if (tid[0] == 'c') pttyp->setv(rv);
	} else {
		if (pttyp) __free_ttyp(pttyp)
		tid = "udf";
#ifdef _VAR_DBG
		pdbg = 0;
#endif
	}
	return *this;
}
var& var::operator=(var &&rv) {
	if (pttyp) __free_ttyp(pttyp);
	var *typ = rv.pttyp;
	if (typ) {
		rv.pttyp = 0;
		__bset(typ)
	} else {
		tid = "udf";
#ifdef _VAR_DBG
		pdbg = 0;
#endif
	}
	return *this;
}
//
var& var::operator=(int rv) {
	if (tid[0] == 'i') {
		pttyp->setv((llong) rv);
		return *this;
	}
	var *typ = new ttyp<llong>((llong) rv);
	if (pttyp) __free_ttyp(pttyp);
	__bset(typ)
	return *this;
}
var& var::operator=(float rv) {
	if (tid[0] == 'd') {
		pttyp->setv((double) rv);
		return *this;
	}
	var *typ = new ttyp<double>((double) rv);
	if (pttyp) __free_ttyp(pttyp);
	__bset(typ)
	return *this;
}
var& var::operator=(double rv) {
	if (tid[0] == 'd') {
		pttyp->setv(rv);
		return *this;
	}
	var *typ = new ttyp<double>((double) rv);
	if (pttyp) __free_ttyp(pttyp);
	__bset(typ)
	return *this;
}
var& var::operator=(long rv) {
	if (tid[0] == 'i') {
		pttyp->setv((llong) rv);
		return *this;
	}
	var *typ = new ttyp<llong>((llong) rv);
	if (pttyp) __free_ttyp(pttyp);
	__bset(typ)
	return *this;
}
var& var::operator=(llong rv) {
	if (tid[0] == 'i') {
		pttyp->setv(rv);
		return *this;
	}
	var *typ = new ttyp<llong>(rv);
	if (pttyp) __free_ttyp(pttyp);
	__bset(typ)
	return *this;
}
var& var::operator=(ullong rv) {
	if (rv > LLONG_MAX) //
	throw VarExcp(VarExcp::_mk_excp("out_of_number_range", //
			"var& operator=(ullong rv)", "value > LLONG_MAX"), 1);
	if (tid[0] == 'i') {
		pttyp->setv((llong) rv);
		return *this;
	}
	var *typ = new ttyp<llong>((llong) rv);
	if (pttyp) __free_ttyp(pttyp);
	__bset(typ)
	return *this;
}
var& var::operator=(bool rv) {
	if (tid[0] == 'b') {
		pttyp->setv(rv);
		return *this;
	}
	var *typ = new ttyp<bool>(rv);
	if (pttyp) __free_ttyp(pttyp);
	__bset(typ)
	return *this;
}
var& var::operator=(char rv) {
	if (tid[0] == 'c') {
		pttyp->setv(rv);
		return *this;
	}
	var *typ = new ttyp<char>(rv);
	if (pttyp) __free_ttyp(pttyp);
	__bset(typ)
	return *this;
}
var& var::operator=(cch *rv) {
	if (tid[0] == 's') {
		pttyp->setv(rv);
		return *this;
	}
	var *typ = new ttyp<str>(rv);
	if (pttyp) __free_ttyp(pttyp);
	__bset(typ)
	return *this;
}
var& var::operator=(const str &rv) {
	if (tid[0] == 's') {
		pttyp->setv(rv);
		return *this;
	}
	var *typ = new ttyp<str>(rv);
	if (pttyp) __free_ttyp(pttyp);
	__bset(typ)
	return *this;
}
var& var::operator=(const __iniL <var> &rv) {
	if (tid[0] == 'a') {
		pttyp->setv(rv);
		return *this;
	}
	var *typ = new ttyp<__vct<var>>(rv);
	if (pttyp) __free_ttyp(pttyp);
	__bset(typ)
	return *this;
}

#define __exc_catch_op(cnd,f,fn,par)\
	  if (cnd){\
      try {\
      return f;\
      } catch (VarExcp &e) {\
      VarExcp::_top_catch(this,fn,e);\
      }catch (...) {\
       VarExcp::_top_catch_unex(this,fn);\
      }}\
	  else OE_solver(1,this, par,  fn );\
	  return var();\


#define __exc_catch_op_(cnd,f,fn,par)\
	  if (cnd){\
      try {\
      f;\
      } catch (VarExcp &e) {\
      VarExcp::_top_catch(this,fn,e);\
      }catch (...) {\
       VarExcp::_top_catch_unex(this,fn);\
      }}\
	  else OE_solver(1,this, par,  fn );\
      return *this;\


#define __exc_catch_op_b(cnd,f,fn,par)\
	  if (cnd){\
      try {\
      return f;\
      } catch (VarExcp &e) {\
      VarExcp::_top_catch(this,fn,e);\
      }catch (...) {\
       VarExcp::_top_catch_unex(this,fn);\
      }}\
	  else OE_solver(1,this, par,  fn );\
	  return false;\
	  

var var::operator+(const var &rv) const {
	__exc_catch_op(pttyp && rv.pttyp, pttyp->operator+(rv), //
	"var operator+(const var &rv) const", &rv)
}
var var::operator+(int rv) const {
	__exc_catch_op(pttyp, pttyp->operator+(rv),	//
	"var operator+(int rv) const", 0)
}
var var::operator+(float rv) const {
	__exc_catch_op(pttyp, pttyp->operator+(rv),	//
	"var operator+(float rv) const", 0)
	return var();
}
var var::operator+(double rv) const {
	__exc_catch_op(pttyp, pttyp->operator+(rv),	//
	"var operator+(double rv) const", 0)
}
var var::operator+(long rv) const {
	__exc_catch_op(pttyp, pttyp->operator+(rv),	//
	"var operator+(long rv) const", 0)
}
var var::operator+(llong rv) const {
	__exc_catch_op(pttyp, pttyp->operator+(rv),	//
	"var operator+(llong rv) const", 0)
}
var var::operator+(ullong rv) const {
	__exc_catch_op(pttyp, pttyp->operator+(rv),	//
	"var operator+(ullong rv) const", 0)
}
var var::operator+(cch *rv) const {
	__exc_catch_op(pttyp, pttyp->operator+(rv),	//
	"var operator+(cch *rv) const", 0)
}
var var::operator+(str &rv) const {
	__exc_catch_op(pttyp, pttyp->operator+(rv),	//
	"var operator+(str &rv) const", 0)
}
var var::operator+(str *rv) const {
	__exc_catch_op(pttyp, pttyp->operator+(rv),	//
	"var operator+(str *rv) const", 0)
}
var var::operator+(char rv) const {
	__exc_catch_op(pttyp, pttyp->operator+(rv),	//
	"var operator+(char rv) const", 0)
}
var var::operator-(const var &rv) const {
	__exc_catch_op(pttyp && rv.pttyp, pttyp->operator-(rv),	//
	"var operator-(const var &rv) const", &rv)
}
var var::operator-(int rv) const {
	__exc_catch_op(pttyp, pttyp->operator-(rv),	//
	"var operator-(int rv) const", 0)
}
var var::operator-(float rv) const {
	__exc_catch_op(pttyp, pttyp->operator-(rv),	//
	"var operator-(float rv) const", 0)
}
var var::operator-(double rv) const {
	__exc_catch_op(pttyp, pttyp->operator-(rv),	//
	"var operator-(double rv) const", 0)
}
var var::operator-(long rv) const {
	__exc_catch_op(pttyp, pttyp->operator-(rv),	//
	"var operator-(long rv) const", 0)
}
var var::operator-(llong rv) const {
	__exc_catch_op(pttyp, pttyp->operator-(rv),	//
	"var operator-(llong rv) const", 0)
}
var var::operator-(ullong rv) const {
	__exc_catch_op(pttyp, pttyp->operator-(rv),	//
	"var operator-(ullong rv) const", 0)
}
var var::operator*(const var &rv) const {
	__exc_catch_op(pttyp && rv.pttyp, pttyp->operator*(rv),	//
	"var operator*(const var &rv) const", &rv)
}
var var::operator*(int rv) const {
	__exc_catch_op(pttyp, pttyp->operator*(rv),	//
	"var operator*(int rv) const", 0)
}
var var::operator*(float rv) const {
	__exc_catch_op(pttyp, pttyp->operator*(rv),	//
	"var operator*(float rv) const", 0)
}
var var::operator*(double rv) const {
	__exc_catch_op(pttyp, pttyp->operator*(rv),	//
	"var operator*(double rv) const", 0)
}
var var::operator*(long rv) const {
	__exc_catch_op(pttyp, pttyp->operator*(rv),	//
	"var operator*(long rv) const", 0)
}
var var::operator*(llong rv) const {
	__exc_catch_op(pttyp, pttyp->operator*(rv),	//
	"var operator*(llong rv) const", 0)
}
var var::operator*(ullong rv) const {
	__exc_catch_op(pttyp, pttyp->operator*(rv),	//
	"var operator*(ullong rv) const", 0)
}
var var::operator/(const var &rv) const {
	__exc_catch_op(pttyp && rv.pttyp, pttyp->operator/(rv),	//
	"var operator/(const var &rv) const", &rv)
}
var var::operator/(int rv) const {
	__exc_catch_op(pttyp, pttyp->operator/(rv),	//
	"var operator/(int rv) const", 0)
}
var var::operator/(float rv) const {
	__exc_catch_op(pttyp, pttyp->operator/(rv),	//
	"var operator/(float rv) const", 0)
}
var var::operator/(double rv) const {
	__exc_catch_op(pttyp, pttyp->operator/(rv),	//
	"var operator/(double rv) const", 0)
}
var var::operator/(long rv) const {
	__exc_catch_op(pttyp, pttyp->operator/(rv),	//
	"var operator/(long rv) const", 0)
}
var var::operator/(llong rv) const {
	__exc_catch_op(pttyp, pttyp->operator/(rv),	//
	"var operator/(llong rv) const", 0)
}
var var::operator/(ullong rv) const {
	__exc_catch_op(pttyp, pttyp->operator/(rv),	//
	"var operator/(ullong rv) const", 0)
}
var& var::operator+=(const var &rv) { //
	__exc_catch_op_(pttyp && rv.pttyp, sp_op_eq_chk(pttyp->operator+=(rv)), //
	"var operator+=(const var &rv)", &rv)
}
var& var::operator+=(int rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator+=(rv)), //
	"var& operator+=(int rv)", 0)
}
var& var::operator+=(float rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator+=(rv)), //
	"var& operator+=(float rv)", 0)
}
var& var::operator+=(double rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator+=(rv)),	//
	"var& operator+=(double rv)", 0)
}
var& var::operator+=(long rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator+=(rv)),	//
	"var& operator+=(long rv)", 0)
}
var& var::operator+=(llong rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator+=(rv)),	//
	"var& operator+=(llong rv)", 0)
}
var& var::operator+=(ullong rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator+=(rv)),	//
	"var& operator+=(ullong rv)", 0)
}
var& var::operator+=(str &rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator+=(rv)),	//
	"var& operator+=(str &rv)", 0)
}
var& var::operator+=(str *rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator+=(rv)),	//
	"var& operator+=(str *rv)", 0)
}
var& var::operator+=(cch *rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator+=(rv)),	//
	"var& operator+=(cch *rv)", 0)
}
var& var::operator+=(char rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator+=(rv)),	//
	"var& operator+=(char rv)", 0)
}
var& var::operator-=(const var &rv) {
	__exc_catch_op_(pttyp && rv.pttyp, sp_op_eq_chk(pttyp->operator-=(rv)),	//
	"var& operator-=(const var &rv)", &rv)
}
var& var::operator-=(int rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator-=(rv)),	//
	"var& operator-=(int rv)", 0)
}
var& var::operator-=(float rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator-=(rv)),	//
	"var& operator-=(float rv)", 0)
}
var& var::operator-=(double rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator-=(rv)),	//
	"var& operator-=(double rv)", 0)
}
var& var::operator-=(long rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator-=(rv)),	//
	"var& operator-=(long rv)", 0)
}
var& var::operator-=(llong rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator-=(rv)),	//
	"var& operator-=(llong rv)", 0)
}
var& var::operator-=(ullong rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator-=(rv)),	//
	"var& operator-=(ullong rv)", 0)
}
var& var::operator*=(const var &rv) {
	__exc_catch_op_(pttyp && rv.pttyp, sp_op_eq_chk(pttyp->operator*=(rv)),	//
	"var& operator*=(const var &rv)", &rv)
}
var& var::operator*=(int rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator*=(rv)),	//
	"var& operator*=(int rv)", 0)
}
var& var::operator*=(float rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator*=(rv)),	//
	"var& operator*=(float rv)", 0)
}
var& var::operator*=(double rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator*=(rv)),	//
	"var& operator*=(double rv)", 0)
}
var& var::operator*=(long rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator*=(rv)),	//
	"var& operator*=(long rv)", 0)
}
var& var::operator*=(llong rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator*=(rv)),	//
	"var& operator*=(llong rv)", 0)
}
var& var::operator*=(ullong rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator*=(rv)),	//
	"var& operator*=(ullong rv)", 0)
}
var& var::operator/=(const var &rv) {
	__exc_catch_op_(pttyp && rv.pttyp, sp_op_eq_chk(pttyp->operator/=(rv)),	//
	"var& operator/=(const var &rv)", &rv)
}
var& var::operator/=(int rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator/=(rv)),	//
	"var& operator/=(int rv)", 0)
}
var& var::operator/=(float rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator/=(rv)),	//
	"var& operator/=(float rv)", 0)
}
var& var::operator/=(double rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator/=(rv)),	//
	"var& operator/=(double rv)", 0)
}
var& var::operator/=(long rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator/=(rv)),	//
	"var& operator/=(long rv)", 0)
}
var& var::operator/=(llong rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator/=(rv)),	//
	"var& operator/=(llong rv)", 0)
}
var& var::operator/=(ullong rv) {
	__exc_catch_op_(pttyp, sp_op_eq_chk(pttyp->operator/=(rv)),	//
	"var& operator/=(ullong rv)", 0)
}
var& var::operator[](ullong index) {
	__exc_catch_op_(pttyp, return pttyp->operator[](index),	//
			"var& operator[](ullong index)", 0)
}
const var& var::operator[](ullong index) const {
	__exc_catch_op_(pttyp, return pttyp->operator[](index),	//
			"const var& operator[](ullong index) const", 0)
}
var var::operator++(int) {
	__exc_catch_op(pttyp, pttyp->operator++(1), //
	"var operator++(int)", 0)
}
const var var::operator++(int) const {
	__exc_catch_op(pttyp, const_cast<var*>(pttyp)->operator++(1), //
	"const var operator++(int) const", 0)
}
var& var::operator++() {
	__exc_catch_op_(pttyp, pttyp->operator++(), //
			"var& operator++()", 0)
}
const var& var::operator++() const {
	__exc_catch_op_(pttyp, const_cast<var*>(pttyp)->operator++(), //
			"const var& operator++() const", 0)
}
var var::operator--(int) {
	__exc_catch_op(pttyp, pttyp->operator--(1), //
	"var operator--(int)", 0)
}
const var var::operator--(int) const {
	__exc_catch_op(pttyp, const_cast<var*>(pttyp)->operator--(1), //
	"const var operator--(int) const", 0)
}
var& var::operator--() {
	__exc_catch_op_(pttyp, pttyp->operator--(), //
			"var& operator--()", 0)
}
const var& var::operator--() const {
	__exc_catch_op_(pttyp, const_cast<var*>(pttyp)->operator--(), //
			"const var& operator--() const", 0)
}
bool var::operator>(const var &rv) const {
	__exc_catch_op_b(pttyp && rv.pttyp, pttyp->operator>(rv), //
	"bool operator>(const var &rv) const", &rv)
}
bool var::operator>(int rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>(rv),	//
	"bool operator>(int rv) const", 0)
}
bool var::operator>(float rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>(rv),	//
	"bool operator>(float rv) const", 0)
}
bool var::operator>(double rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>(rv),	//
	"bool operator>(double rv) const", 0)
}
bool var::operator>(long rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>(rv),	//
	"bool operator>(long rv) const", 0)
}
bool var::operator>(llong rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>(rv),	//
	"bool operator>(llong rv) const", 0)
}
bool var::operator>(ullong rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>(rv),	//
	"bool operator>(ullong rv) const", 0)
}
bool var::operator>(cch *rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>(rv),	//
	"bool operator>(cch *rv) const", 0)
}
bool var::operator>(str &rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>(rv),	//
	"bool operator>(str &rv) const", 0)
}
bool var::operator>(str *rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>(rv),	//
	"bool operator>(str *rv) const", 0)
}
bool var::operator>(char rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>(rv),	//
	"bool operator>(char rv) const", 0)
}
bool var::operator<(const var &rv) const {
	__exc_catch_op_b(pttyp && rv.pttyp, pttyp->operator<(rv),	//
	"bool operator<(const var &rv) const", &rv)
}
bool var::operator<(int rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<(rv),	//
	"bool operator<(int rv) const", 0)
}
bool var::operator<(float rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<(rv),	//
	"bool operator<(float rv) const", 0)
}
bool var::operator<(double rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<(rv),	//
	"bool operator<(double rv) const", 0)
}
bool var::operator<(long rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<(rv),	//
	"bool operator<(long rv) const", 0)
}
bool var::operator<(llong rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<(rv),	//
	"bool operator<(llong rv) const", 0)
}
bool var::operator<(ullong rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<(rv),	//
	"bool operator<(ullong rv) const", 0)
}
bool var::operator<(cch *rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<(rv),	//
	"bool operator<(cch *rv) const", 0)
}
bool var::operator<(str &rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<(rv),	//
	"bool operator<(str &rv) const", 0)
}
bool var::operator<(str *rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<(rv),	//
	"bool operator<(str *rv) const", 0)
}
bool var::operator<(char rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<(rv),	//
	"bool operator<(char rv) const", 0)
}
bool var::operator>=(const var &rv) const {
	__exc_catch_op_b(pttyp && rv.pttyp, pttyp->operator>=(rv),	//
	"bool operator>=(const var &rv) const", &rv)
}
bool var::operator>=(int rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>=(rv),	//
	"bool operator>=(int rv) const", 0)
}
bool var::operator>=(float rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>=(rv),	//
	"bool operator>=(float rv) const", 0)
}
bool var::operator>=(double rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>=(rv),	//
	"bool operator>=(double rv) const", 0)
}
bool var::operator>=(long rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>=(rv),	//
	"bool operator>=(long rv) const", 0)
}
bool var::operator>=(llong rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>=(rv),	//
	"bool operator>=(llong rv) const", 0)
}
bool var::operator>=(ullong rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>=(rv),	//
	"bool operator>=(ullong rv) const", 0)
}
bool var::operator>=(cch *rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>=(rv),	//
	"bool operator>=(cch *rv) const", 0)
}
bool var::operator>=(str &rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>=(rv),	//
	"bool operator>=(str &rv) const", 0)
}
bool var::operator>=(str *rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>=(rv),	//
	"bool operator>=(str *rv) const", 0)
}
bool var::operator>=(char rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator>=(rv),	//
	"bool operator>=(char rv) const", 0)
}
bool var::operator<=(const var &rv) const {
	__exc_catch_op_b(pttyp && rv.pttyp, pttyp->operator<=(rv),	//
	"bool operator<=(const var &rv) const", &rv)
}
bool var::operator<=(int rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<=(rv),	//
	"bool operator<=(int rv) const", 0)
}
bool var::operator<=(float rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<=(rv),	//
	"bool operator<=(float rv) const", 0)
}
bool var::operator<=(double rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<=(rv),	//
	"bool operator<=(double rv) const", 0)
}
bool var::operator<=(long rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<=(rv),	//
	"bool operator<=(long rv) const", 0)
}
bool var::operator<=(llong rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<=(rv),	//
	"bool operator<=(llong rv) const", 0)
}
bool var::operator<=(ullong rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<=(rv),	//
	"bool operator<=(ullong rv) const", 0)
}
bool var::operator<=(cch *rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<=(rv),	//
	"bool operator<=(cch *rv) const", 0)
}
bool var::operator<=(str &rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<=(rv),	//
	"bool operator<=(str &rv) const", 0)
}
bool var::operator<=(str *rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<=(rv),	//
	"bool operator<=(str *rv) const", 0)
}
bool var::operator<=(char rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator<=(rv),	//
	"bool operator<=(char rv) const", 0)
}
bool var::operator!() const {
	if (pttyp) return pttyp->operator!();
	return true;
}
bool var::operator==(const var &rv) const {
	__exc_catch_op_b(pttyp && rv.pttyp, pttyp->operator==(rv),	//
	"bool operator==(const var &rv) const", &rv)
}
bool var::operator==(int rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator==(rv),	//
	"bool operator==(int rv) const", 0)
}
bool var::operator==(float rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator==(rv),	//
	"bool operator==(float rv) const", 0)
}
bool var::operator==(double rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator==(rv),	//
	"bool operator==(double rv) const", 0)
}
bool var::operator==(long rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator==(rv),	//
	"bool operator==(long rv) const", 0)
}
bool var::operator==(llong rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator==(rv),	//
	"bool operator==(llong rv) const", 0)
}
bool var::operator==(ullong rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator==(rv),	//
	"bool operator==(ullong rv) const", 0)
}
bool var::operator==(cch *rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator==(rv),	//
	"bool operator==(cch *rv) const", 0)
}
bool var::operator==(str &rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator==(rv),	//
	"bool operator==(str &rv) const", 0)
}
bool var::operator==(str *rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator==(rv),	//
	"bool operator==(str *rv) const", 0)
}
bool var::operator==(char rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator==(rv),	//
	"bool operator==(char rv) const", 0)
}
bool var::operator!=(const var &rv) const {
	__exc_catch_op_b(pttyp && rv.pttyp, pttyp->operator!=(rv),	//
	"bool operator!=(const var &rv) const", &rv)
}
bool var::operator!=(int rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator!=(rv),	//
	"bool operator!=(int rv) const, &rv)", 0)
}
bool var::operator!=(float rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator!=(rv),	//
	"bool operator!=(float rv) const", 0)
}
bool var::operator!=(double rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator!=(rv),	//
	"bool operator!=(double rv) const", 0)
}
bool var::operator!=(long rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator!=(rv),	//
	"bool operator!=(long rv) const", 0)
}
bool var::operator!=(llong rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator!=(rv),	//
	"bool operator!=(llong rv) const", 0)
}
bool var::operator!=(ullong rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator!=(rv),	//
	"bool operator!=(ullong rv) const", 0)
}
bool var::operator!=(cch *rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator!=(rv),	//
	"bool operator!=(cch *rv) const", 0)
}
bool var::operator!=(str &rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator!=(rv),	//
	"bool operator!=(str &rv) const", 0)
}
bool var::operator!=(str *rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator!=(rv),	//
	"bool operator!=(str *rv) const", 0)
}
bool var::operator!=(char rv) const {
	__exc_catch_op_b(pttyp, pttyp->operator!=(rv),	//
	"bool operator!=(char rv) const", 0)
}
var& var::operator*() {
	__exc_catch_op_(pttyp, return pttyp->operator*(),	//
			"var& operator*()", 0)
}
const var& var::operator*() const {
	__exc_catch_op_(pttyp, return pttyp->operator*(),	//
			"const var& operator*() const", 0)
}

#define __exc_catch_cst(cnd,f,fn,par)\
	 if (cnd){\
	 try {\
	 return f;\
	 } catch (VarExcp &e) {\
	 VarExcp::_top_catch(this,fn,e);\
	 }catch (...) {\
	 VarExcp::_top_catch_unex(this,fn);\
	 }}\
     else OE_solver(1,this, par,  fn );\
     

var::operator int() const {
	__exc_catch_cst(pttyp, const_cast<var*>(pttyp)->operator int(),	//
			"operator int() const", 0)
	return 0;
}
var::operator int() {
	__exc_catch_cst(pttyp, pttyp->operator int(),	//
			"operator int()", 0)
	return 0;
}
var::operator float() const {
	__exc_catch_cst(pttyp, const_cast<var*>(pttyp)->operator float(),	//
			"operator float() const", 0)
	return 0;
}
var::operator float() {
	__exc_catch_cst(pttyp, pttyp->operator float(),	//
			"operator float()", 0)
	return 0;
}
var::operator double() const {
	__exc_catch_cst(pttyp, const_cast<var*>(pttyp)->operator double(),	//
			"operator double() const", 0)
	return 0;
}
var::operator double() {
	__exc_catch_cst(pttyp, pttyp->operator double(),	//
			"operator double()", 0)
	return 0;
}
var::operator long() const {
	__exc_catch_cst(pttyp, const_cast<var*>(pttyp)->operator long(),	//
			"operator long() const", 0)
	return 0;
}
var::operator long() {
	__exc_catch_cst(pttyp, pttyp->operator long(),	//
			"operator long()", 0)
	return 0;
}
var::operator llong() const {
	__exc_catch_cst(pttyp, const_cast<var*>(pttyp)->operator llong(),	//
			"operator llong() const", 0)
	return 0;
}
var::operator llong() {
	__exc_catch_cst(pttyp, pttyp->operator llong(),	//
			"operator llong()", 0)
	return 0;
}
var::operator ullong() const {
	__exc_catch_cst(pttyp, const_cast<var*>(pttyp)->operator ullong(),	//
			"operator ullong() const", 0)
	return 0;
}
var::operator ullong() {
	__exc_catch_cst(pttyp, pttyp->operator ullong(),	//
			"operator ullong()", 0)
	return 0;
}
var::operator bool() const noexcept {
	if (pttyp) return pttyp->operator bool();
	return false;
}
var::operator bool() noexcept {
	if (pttyp) return pttyp->operator bool();
	return false;
}
var::operator char() const {
	__exc_catch_cst(pttyp, const_cast<var*>(pttyp)->operator char(),	//
			"operator char() const", 0)
	return char(0);
}
var::operator char() {
	__exc_catch_cst(pttyp, pttyp->operator char(),	//
			"operator char()", 0)
	return char(0);
}
var::operator str() const {
	
#define __op_str_cst_fn "operator str() const"
	
	if (pttyp) {
		try {
			return const_cast<var*>(pttyp)->operator str();
		} catch (VarExcp &e) {
			VarExcp::_top_catch(this, __op_str_cst_fn, e);
		} catch (...) {
			VarExcp::_top_catch_unex(this, __op_str_cst_fn);
		}
	} else if (tid[0] == 'u' && tid[1] == 'd') return "|udf|";
	else OE_solver(1, this, 0, __op_str_cst_fn);
	return "";
}
var::operator str() {
	if (pttyp) {
		try {
			return pttyp->operator str();
		} catch (VarExcp &e) {
			VarExcp::_top_catch(this, __op_str_cst_fn, e);
		} catch (...) {
			VarExcp::_top_catch_unex(this, __op_str_cst_fn);
		}
	} else if (tid[0] == 'u' && tid[1] == 'd') return "|udf|";
	else OE_solver(1, this, 0, __op_str_cst_fn);
	return "";
}
var::operator var*() { // inner use
	return this;
}
var::operator str*() const { // inner use
	return pttyp->operator str*();
}
var::operator __vct<var>*() { // inner use
	return pttyp->operator __vct<var>*();
}
var::operator Ic_str() { // inner use
	return pttyp->operator Ic_str();
}
var::operator Ic_str*() { // inner use
	return pttyp->operator Ic_str*();
}
var::operator Ic_arr() { // inner use
	return pttyp->operator Ic_arr();
}
var::operator Ic_arr*() { // inner use
	return pttyp->operator Ic_arr*();
}
var::operator Irc_arr() { // inner use
	return pttyp->operator Irc_arr();
}
var::operator Irc_arr*() { // inner use
	return pttyp->operator Irc_arr*();
}
var::operator Irc_str() { // inner use
	return pttyp->operator Irc_str();
	
}
var::operator Irc_str*() { // inner use
	return pttyp->operator Irc_str*();
}

//------------- Common if: ------------

int var::size() const noexcept {
	if (pttyp) return pttyp->size();
	return 0;
}
cch* var::gtid() const noexcept {
	return this->tid;
}
bool var::is_def() const noexcept {
	if (pttyp) return true;
	return false;
}
bool var::is_bool() const noexcept {
	if (tid[0] == 'b') return true;
	return false;
}
bool var::is_int() const noexcept {
	if (tid[0] == 'i') return true;
	return false;
}
bool var::is_double() const noexcept {
	if (tid[0] == 'd') return true;
	return false;
}
bool var::is_char() const noexcept {
	if (tid[0] == 'c') return true;
	return false;
}
bool var::is_str() const noexcept {
	if (tid[0] == 's') return true;
	return false;
}
bool var::is_arr() const noexcept {
	if (tid[0] == 'a') return true;
	return false;
}
bool var::is_map() const noexcept {
	if (tid[0] == 'm') return true;
	return false;
}
bool var::is_P_map() const noexcept {
	if (varF::cmp(tid, "P_map")) return true;
	return false;
}
bool var::is_itr() const noexcept {
	if (tid[0] == 'I') return true;
	return false;
}
bool var::is_I_str() const noexcept {
	if (varF::cmp(tid, "I_str")) return true;
	return false;
}
bool var::is_Ir_str() const noexcept {
	if (varF::cmp(tid, "Ir_str")) return true;
	return false;
}
bool var::is_I_arr() const noexcept {
	if (varF::cmp(tid, "I_arr")) return true;
	return false;
}
bool var::is_Ir_arr() const noexcept {
	if (varF::cmp(tid, "Ir_arr")) return true;
	return false;
}
void var::prnt() const noexcept {
	if (pttyp) pttyp->prnt();
	else {
		std::cout << "|udf|" << std::endl;
	}
}
var var::_str() const {
	
#define __op_str_if_fn "var _str() const"
	
	if (pttyp) {
		try {
			return pttyp->_str();
		} catch (VarExcp &e) {
			VarExcp::_top_catch(this, __op_str_if_fn, e);
		} catch (...) {
			VarExcp::_top_catch_unex(this, __op_str_if_fn);
		}
	} else if (tid[0] == 'u' && tid[1] == 'd') return "|udf|";
	else IE_solver(this, __op_str_if_fn);
	return "";
}

//-------------- str/arr shared: -------

#define __exc_catch_if(cnd,f,fn,ret)\
	  if (cnd){\
      try {\
       f;\
      } catch (VarExcp &e) {\
      VarExcp::_top_catch(this,fn,e);\
      }catch (...) {\
       VarExcp::_top_catch_unex(this,fn);\
      }}\
	  else IE_solver(this,fn);\
	  ret;\


var& var::at(ullong index) {
	__exc_catch_if(pttyp, return pttyp->at(index), //
			"var& at(ullong index)", return *this)
}
const var& var::at(ullong index) const {
	__exc_catch_if(pttyp, return pttyp->at(index), //
			"const var& at(ullong index) const", return *this)
}
var& var::front() {
	__exc_catch_if(pttyp, return pttyp->front(), //
			"var& front()", return *this)
}
const var& var::front() const {
	__exc_catch_if(pttyp, return pttyp->front(), //
			"const var& front() const", return *this)
}
var& var::back() {
	__exc_catch_if(pttyp, return pttyp->back(), //
			"var& back()", return *this)
}
const var& var::back() const {
	__exc_catch_if(pttyp, return pttyp->back(), //
			"const var& back() const", return *this)
}
var var::begin() {
	__exc_catch_if(pttyp, return pttyp->begin(), //
			"var begin()", return var())
}
const var var::begin() const {
	__exc_catch_if(pttyp, return pttyp->begin(), //
			"const var begin() const", return var())
}
var var::end() {
	__exc_catch_if(pttyp, return pttyp->end(), //
			"var end()", return var())
}
const var var::end() const {
	__exc_catch_if(pttyp, return pttyp->end(), //
			"const var end() const", return var())
}
var var::rbegin() {
	__exc_catch_if(pttyp, return pttyp->rbegin(), //
			"var rbegin()", return var())
}
const var var::rbegin() const {
	__exc_catch_if(pttyp, return pttyp->rbegin(), //
			"const var rbegin() const", return var())
}
var var::rend() {
	__exc_catch_if(pttyp, return pttyp->rend(), //
			"var rend()", return var())
}
const var var::rend() const {
	__exc_catch_if(pttyp, return pttyp->rend(), //
			"const var rend() const", return var())
}
bool var::next() const {
	__exc_catch_if(pttyp, return pttyp->next(), "bool next() const", return false)
}
llong var::pos() const {
	__exc_catch_if(pttyp, return pttyp->pos(), "llong pos() const", return 0)
}
bool var::empty() const {
	__exc_catch_if(pttyp, return pttyp->empty(), //
			"bool empty() const", return false)
}
ullong var::length() const {
	__exc_catch_if(pttyp, return pttyp->length(), //
			"ullong length() const", return 0)
}
ullong var::max_size() const {
	__exc_catch_if(pttyp, return pttyp->max_size(), //
			"ullong max_size() const", return 0)
}
void var::reserve(ullong new_cap = 0) {
	__exc_catch_if(pttyp, pttyp->reserve(new_cap), //
	"void reserve(ullong new_cap)",)
}
ullong var::capacity() const {
	__exc_catch_if(pttyp, return pttyp->capacity(), //
			"ullong capacity() const", return 0)
}
void var::shrink_to_fit() {
	__exc_catch_if(pttyp, pttyp->shrink_to_fit(), //
			"void shrink_to_fit()",)
}
void var::clear() {
	__exc_catch_if(pttyp, pttyp->clear(), //
			"void clear()",)
}
void var::swap(var &with) {
	__exc_catch_if(pttyp, pttyp->swap(with), //
	"void swap(var &with)",)
}
//..
void var::resize(ullong count) {
	__exc_catch_if(pttyp, pttyp->resize(count), //
	"void resize(ullong count)",)
}
void var::resize(ullong count, const var &X) {
	__exc_catch_if(pttyp, pttyp->resize(count, X), //
	"void resize(ullong count, const var &X)",)
}
void var::resize(ullong count, int X) {
	__exc_catch_if(pttyp, pttyp->resize(count, X), //
	"void resize(ullong count, int X)",)
}
void var::resize(ullong count, float X) {
	__exc_catch_if(pttyp, pttyp->resize(count, X), //
	"void resize(ullong count, float X)",)
}
void var::resize(ullong count, double X) {
	__exc_catch_if(pttyp, pttyp->resize(count, X), //
	"void resize(ullong count, double X)",)
}
void var::resize(ullong count, long X) {
	__exc_catch_if(pttyp, pttyp->resize(count, X), //
	"void resize(ullong count, long X)",)
}
void var::resize(ullong count, llong X) {
	__exc_catch_if(pttyp, pttyp->resize(count, X), //
	"void resize(ullong count, llong X)",)
}
void var::resize(ullong count, ullong X) {
	__exc_catch_if(pttyp, pttyp->resize(count, X), //
	"void resize(ullong count, ullong X)",)
}
void var::resize(ullong count, char X) {
	__exc_catch_if(pttyp, pttyp->resize(count, X), //
	"void resize(ullong count, char X)",)
}
void var::resize(ullong count, cch *X) {
	__exc_catch_if(pttyp, pttyp->resize(count, X), //
	"void resize(ullong count, cch *X)",)
}
void var::resize(ullong count, const str &X) {
	__exc_catch_if(pttyp, pttyp->resize(count, X), //
	"void resize(ullong count, const str &X)",)
}
void var::resize(ullong count, __iniL<var> X) {
	__exc_catch_if(pttyp, pttyp->resize(count, X), //
	"void resize(ullong count, __iniL<var> X)",)
}
//..
var var::insert(ullong index, const var &src_fist, const var &src_last) {
	__exc_catch_if(pttyp, return pttyp->insert(index, src_fist, src_last), //
			"var insert(ullong index, const var &src_fist, const var &src_last)",//
			return var())
}
var var::insert(ullong index, const var &X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, X), //
			"var insert(ullong index, const var &X)", return var())
}
var var::insert(ullong index, int X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, X), //
			"var insert(ullong index, int X)", return var())
}
var var::insert(ullong index, float X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, X), //
			"var insert(ullong index, float X)", return var())
}
var var::insert(ullong index, double X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, X), //
			"var insert(ullong index, double X)", return var())
}
var var::insert(ullong index, long X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, X), //
			"var insert(ullong index, long X)", return var())
}
var var::insert(ullong index, llong X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, X), //
			"var insert(ullong index, llong X)", return var())
}
var var::insert(ullong index, ullong X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, X), //
			"var insert(ullong index, ullong X)", return var())
}
var var::insert(ullong index, char X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, X), //
			"var insert(ullong index, char X)", return var())
}
var var::insert(ullong index, cch *X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, X), //
			"var insert(ullong index, cch *X)", return var())
}
var var::insert(ullong index, const str &X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, X), //
			"var insert(ullong index, const str &X)", return var())
}
var var::insert(ullong index, __iniL<var> X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, X), //
			"var insert(ullong index, __iniL<var> X)", return var())
}
var var::insert(ullong index, ullong count, const var &X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, count, X), //
			"var insert(ullong index, ullong count, const var &X)",//
			return var())
}
var var::insert(ullong index, ullong count, int X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, count, X), //
			"var insert(ullong index, ullong count, int X)",//
			return var())
}
var var::insert(ullong index, ullong count, float X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, count, X), //
			"var insert(ullong index, ullong count, float X)",//
			return var())
}
var var::insert(ullong index, ullong count, double X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, count, X), //
			"var insert(ullong index, ullong count, double X)",//
			return var())
}
var var::insert(ullong index, ullong count, long X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, count, X), //
			"var insert(ullong index, ullong count, long X)",//
			return var())
}
var var::insert(ullong index, ullong count, llong X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, count, X), //
			"var insert(ullong index, ullong count, llong X)",//
			return var())
}
var var::insert(ullong index, ullong count, ullong X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, count, X), //
			"var insert(ullong index, ullong count, ullong X)",//
			return var())
}
var var::insert(ullong index, ullong count, char X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, count, X), //
			"var insert(ullong index, ullong count, char X)",//
			return var())
}
var var::insert(ullong index, ullong count, cch *X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, count, X), //
			"var insert(ullong index, ullong count, cch *X)",//
			return var())
}
var var::insert(ullong index, ullong count, const str &X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, count, X), //
			"var insert(ullong index, ullong count, const str &X)",//
			return var())
}
var var::insert(ullong index, ullong count, __iniL<var> X) {
	__exc_catch_if(pttyp, return pttyp->insert(index, count, X), //
			"var insert(ullong index, ullong count, __iniL<var> X)",//
			return var())
}
//..
var var::erase(ullong index) {
	__exc_catch_if(pttyp, return pttyp->erase(index), //
			"var erase(ullong index)", return var())
}
var var::erase(ullong first, ullong last) {
	__exc_catch_if(pttyp, return pttyp->erase(first, last), //
			"var erase(ullong first, ullong last)", return var())
}
//..
var var::push_back(const var &X) {
	__exc_catch_if(pttyp, return pttyp->push_back(X), //
			"var push_back(const var &X)", return var())
}
var var::push_back(int X) {
	__exc_catch_if(pttyp, return pttyp->push_back(X), //
			"var push_back(int X)", return var())
}
var var::push_back(float X) {
	__exc_catch_if(pttyp, return pttyp->push_back(X), //
			"var push_back(float X)", return var())
}
var var::push_back(double X) {
	__exc_catch_if(pttyp, return pttyp->push_back(X), //
			"var push_back(double X)", return var())
}
var var::push_back(long X) {
	__exc_catch_if(pttyp, return pttyp->push_back(X), //
			"var push_back(long X)", return var())
}
var var::push_back(llong X) {
	__exc_catch_if(pttyp, return pttyp->push_back(X), //
			"var push_back(llong X)", return var())
}
var var::push_back(ullong X) {
	__exc_catch_if(pttyp, return pttyp->push_back(X), //
			"var push_back(ullong X)", return var())
}
var var::push_back(char X) {
	__exc_catch_if(pttyp, return pttyp->push_back(X), //
			"var push_back(char X)", return var())
}
var var::push_back(cch *X) {
	__exc_catch_if(pttyp, return pttyp->push_back(X), //
			"var push_back(cch *X)", return var())
}
var var::push_back(const str &X) {
	__exc_catch_if(pttyp, return pttyp->push_back(X), //
			"var push_back(const str &X)", return var())
}
var var::push_back( __iniL<var> X) {
	__exc_catch_if(pttyp, return pttyp->push_back(X), //
			"var push_back( __iniL<var> X)", return var())
}
//..
var var::push_front(const var &X) {
	__exc_catch_if(pttyp, return pttyp->push_front(X), //
			"var push_front(const var &X)", return var())
}
var var::push_front(int X) {
	__exc_catch_if(pttyp, return pttyp->push_front(X), //
			"var push_front(int X)", return var())
}
var var::push_front(float X) {
	__exc_catch_if(pttyp, return pttyp->push_front(X), //
			"var push_front(float X)", return var())
}
var var::push_front(double X) {
	__exc_catch_if(pttyp, return pttyp->push_front(X), //
			"var push_front(double X)", return var())
}
var var::push_front(long X) {
	__exc_catch_if(pttyp, return pttyp->push_front(X), //
			"var push_front(long X)", return var())
}
var var::push_front(llong X) {
	__exc_catch_if(pttyp, return pttyp->push_front(X), //
			"var push_front(llong X)", return var())
}
var var::push_front(ullong X) {
	__exc_catch_if(pttyp, return pttyp->push_front(X), //
			"var push_front(ullong X)", return var())
}
var var::push_front(char X) {
	__exc_catch_if(pttyp, return pttyp->push_front(X), //
			"var push_front(char X)", return var())
}
var var::push_front(cch *X) {
	__exc_catch_if(pttyp, return pttyp->push_front(X), //
			"var push_front(cch *X)", return var())
}
var var::push_front(const str &X) {
	__exc_catch_if(pttyp, return pttyp->push_front(X), //
			"var push_front(const str &X)", return var())
}
var var::push_front( __iniL<var> X) {
	__exc_catch_if(pttyp, return pttyp->push_front(X), //
			"var push_front(__iniL<var> X)", return var())
}
//..
var var::pop_back() {
	__exc_catch_if(pttyp, return pttyp->pop_back(), //
			"var pop_back()", return var())
}
var var::pop_front() {
	__exc_catch_if(pttyp, return pttyp->pop_front(), //
			"var pop_front()", return var())
}
// ..
var var::sort_cmp(bool (*cmp)(var &a, var &b), bool reverse) const {
	__exc_catch_if(pttyp, return pttyp->sort_cmp(cmp,reverse), //
			"var sort_cmp(bool (*cmp)(var &a, var &b), bool reverse = false) const",//
			return var())
}
var& var::ssort_cmp(bool (*cmp)(var &a, var &b), bool reverse) {
	__exc_catch_if(pttyp, return pttyp->ssort_cmp(cmp,reverse), //
			"var& ssort_cmp(bool (*cmp)(var &a, var &b), bool reverse = false)",//
			return *this)
}
var var::sort(bool reverse) const {
	__exc_catch_if(pttyp, return pttyp->sort(reverse), //
			"var sort(bool reverse = false) const", return var())
}
var& var::ssort(bool reverse) {
	__exc_catch_if(pttyp, return pttyp->ssort(reverse), //
			"var ssort(bool reverse = false)", return *this)
}
// ..
var var::sample(ullong num) const {
	__exc_catch_if(pttyp, return pttyp->sample(num), //
			"var sample(ullong num = 0) const", return var())
}
var var::sample_unq(ullong num) const {
	__exc_catch_if(pttyp, return pttyp->sample_unq(num), //
			"var sample_unq(ullong num = 0) const", return var())
}
// ..
var var::reverse() {
	__exc_catch_if(pttyp, return pttyp->reverse(), //
			"var reverse()", return var())
}
var& var::sreverse() {
	__exc_catch_if(pttyp, return pttyp->sreverse(), //
			"var& sreverse()", return *this)
}
// ..
bool var::equal(const var &src_first, const var &src_last, ullong index, ullong count) const {
	__exc_catch_if(pttyp, return pttyp->equal(src_first,src_last,index,count), //
			"bool equal(const var &src_first, const var &src_last,"//
			" ullong index = 0, ullong count = 0) const", return false)
}
var var::mismatch(const var &src_first, const var &src_last, ullong index, ullong count) const {
	__exc_catch_if(pttyp, return pttyp->mismatch(src_first,src_last,index,count), //
			"var mismatch(const var &src_first, const var &src_last,"//
			" ullong index = 0, ullong count = 0) const", return false)
}
// ..
var& var::fill(const var &X, ullong index, ullong count) {
	__exc_catch_if(pttyp, return pttyp->fill(X,index,count), //
			"var& fill(const var &X, ullong index = 0, ullong count = 0)", return *this)
}
var& var::fill(int X, ullong index, ullong count) {
	__exc_catch_if(pttyp, return pttyp->fill(X,index,count), //
			"var& fill(int X, ullong index = 0, ullong count = 0)", return *this)
}
var& var::fill(float X, ullong index, ullong count) {
	__exc_catch_if(pttyp, return pttyp->fill(X,index,count), //
			"var& fill(float X, ullong index = 0, ullong count = 0)", return *this)
}
var& var::fill(double X, ullong index, ullong count) {
	__exc_catch_if(pttyp, return pttyp->fill(X,index,count), //
			"var& fill(double X, ullong index = 0, ullong count = 0)", return *this)
}
var& var::fill(long X, ullong index, ullong count) {
	__exc_catch_if(pttyp, return pttyp->fill(X,index,count), //
			"var& fill(long X, ullong index = 0, ullong count = 0)", return *this)
}
var& var::fill(llong X, ullong index, ullong count) {
	__exc_catch_if(pttyp, return pttyp->fill(X,index,count), //
			"var& fill(llong X, ullong index = 0, ullong count = 0)", return *this)
}
var& var::fill(ullong X, ullong index, ullong count) {
	__exc_catch_if(pttyp, return pttyp->fill(X,index,count), //
			"var& fill(ullong X, ullong index = 0, ullong count = 0)", return *this)
}
var& var::fill(char X, ullong index, ullong count) {
	__exc_catch_if(pttyp, return pttyp->fill(X,index,count), //
			"var& fill(char X, ullong index = 0, ullong count = 0)", return *this)
}
var& var::fill(cch *X, ullong index, ullong count) {
	__exc_catch_if(pttyp, return pttyp->fill(X,index,count), //
			"var& fill(cch *X, ullong index = 0, ullong count = 0)", return *this)
}
var& var::fill(const str &X, ullong index, ullong count) {
	__exc_catch_if(pttyp, return pttyp->fill(X,index,count), //
			"var& fill(const str &X, ullong index = 0, ullong count = 0)",//
			return *this)
}
var& var::fill(__iniL<var> X, ullong index, ullong count) {
	__exc_catch_if(pttyp, return pttyp->fill(X,index,count), //
			"var& fill(__iniL<var> X, ullong index = 0, ullong count = 0)", return *this)
}

//--------------- arr if: ------------

var& var::initArr() noexcept {
	*this = __vct<var>();
	return *this;
}
var var::join(char sep) const {
	__exc_catch_if(pttyp, return pttyp->join(sep), //
			"var join(char sep = 0) const", return var())
}
var& var::sjoin(char sep) {
	__exc_catch_if(pttyp, return sp_op_eq_chk(pttyp->sjoin(sep)), //
			"var sjoin(char sep = 0)", return *this)
}

//--------------- map if: ------------- 

var& var::map_init() noexcept {
	
	delete pttyp;
	ttyp<VMAP> *typ = new ttyp<VMAP>();
	tid = typ->tid, pttyp = typ;
	
#ifdef _VAR_DBG 
	pdbg = typ->pdbg;
	pdbg->sval = "{...}";
#endif	
	return *this;
}
var& var::map_set(cch *map) {
	
	if (!pttyp || tid[0] != 'm') {
		delete pttyp; 
		ttyp<VMAP> *typ = new ttyp<VMAP>();
		tid = typ->tid, pttyp = typ;
		
#ifdef _VAR_DBG 
		pdbg = typ->pdbg;
		pdbg->sval = "{...}";
#endif
	}
	try {
		pttyp->map_set(map);
	} catch (VarExcp &e) {
		VarExcp::_top_catch(this, "var& map_set(cch *map)", e);
	} catch (...) {
		VarExcp::_top_catch_unex(this, "var& map_set(cch *map)");
	}
	return *this;
}
var var::map_get(cch *map) const {
	__exc_catch_if(pttyp, return pttyp->map_get(map), //
			"var map_get(cch *map) const", return var())
}
var var::map_rem(cch *map) {
	__exc_catch_if(pttyp, return pttyp->map_rem(map), //
			"var map_rem(cch *map)", return var())
}
var& var::map_free() {
	__exc_catch_if(pttyp, pttyp->map_free(), //
			"var& map_free()", return *this)
}
void var::map_foreach(var::map (*fun)(char *field, const var &val, void *ctx), void *ctx, //
		int depth) const {
	__exc_catch_if(pttyp, pttyp->map_foreach(fun, ctx, depth), //
	"void map_foreach(var::map (*fun)(char *field, const var &val, void *ctx),"//
			" void *ctx = 0, int depth = 0) const", return)
}
var var::map_str(bool format) const {
	__exc_catch_if(pttyp, return pttyp->map_str(format), //
			"var map_str(bool format = 0) const", return var())
}
var var::map_fread(cch *fname) {
	if (!pttyp || tid[0] != 'm') {
		delete pttyp;
		ttyp<VMAP> *typ = new ttyp<VMAP>();
		tid = typ->tid, pttyp = typ;
		
#ifdef _VAR_DBG 
		pdbg = typ->pdbg;
		pdbg->sval = "{...}";
#endif
	}
	try {
		return pttyp->map_fread(fname);
	} catch (VarExcp &e) {
		VarExcp::_top_catch(this, "var map_fread(cch *fname)", e);
	} catch (...) {
		VarExcp::_top_catch_unex(this, "var map_fread(cch *fname)");
	}
	return var();
}
var var::map_fwrite(cch *fname) const {
	__exc_catch_if(pttyp, return pttyp->map_fwrite(fname), //
			"var map_fwrite(cch *fname) const",//
			return var())
}

bool var::map_equal(const var &map) const {
	__exc_catch_if(pttyp, return pttyp->map_equal(map), //
			"bool map_equal(const var& map) const", return false)
}

var var::map_mismatch(const var &map) const {
	__exc_catch_if(pttyp, return pttyp->map_mismatch(map), //
			"var map_mismatch(const var& map) const", return var())
}

//--------------- str if: ------------- 

cch* var::data() const {
	__exc_catch_if(pttyp, return static_cast<const var*>(pttyp)->data(), //
			"cch* data() const", return 0)
}
var var::split(char sep) const {
	__exc_catch_if(pttyp, return pttyp->split(sep), //
			"var split(char sep) const", return var())
}
var& var::ssplit(char sep) {
	__exc_catch_if(pttyp, return sp_op_eq_chk(pttyp->ssplit(sep)), //
			"var& ssplit(char sep)", return *this)
}
var& var::ltrim() {
	__exc_catch_if(pttyp, pttyp->ltrim(), //
			"var& ltrim()", return *this)
}
var& var::rtrim() {
	__exc_catch_if(pttyp, pttyp->rtrim(), //
			"var& rtrim()", return *this)
}
var& var::trim() {
	__exc_catch_if(pttyp, pttyp->trim(), //
			"var& trim()", return *this)
}
//..
var var::insert(ullong index, cch *src, ullong src_index, ullong src_count) {
	__exc_catch_if(pttyp, return pttyp->insert(index, src, src_index, src_count), //
			"var insert(ullong index, cch *src, ullong src_index, ullong src_count)",//
			return var())
}
//..
var& var::append(char ch, ullong count) {
	__exc_catch_if(pttyp, pttyp->append(ch, count), //
	"var& append(char ch, ullong count)", return *this)
}
var& var::append(const var &src, ullong src_index, ullong src_count) {
	__exc_catch_if(pttyp, pttyp->append(src, src_index, src_count), //
	"var& append(const var &src, ullong src_index = 0, ullong src_count = 0)",//
			return *this)
}
var& var::append(cch *src, ullong src_index, ullong src_count) {
	__exc_catch_if(pttyp, pttyp->append(src, src_index, src_count), //
	"var& append(cch *src, ullong src_index = 0, ullong src_count = 0)",//
			return *this)
}
var& var::append(str &src, ullong src_index, ullong src_count) {
	__exc_catch_if(pttyp, pttyp->append(src, src_index, src_count), //
	"var& append( str &src, ullong src_index = 0, ullong src_count = 0)",//
			return *this)
}
var& var::append(const var &src_first, const var &src_last) {
	__exc_catch_if(pttyp, pttyp->append(src_first, src_last), //
	"var& append(const var &src_first, const var &src_last)",//
			return *this)
}
//..
var& var::replace(ullong index, ullong count, const var &src, ullong src_index, ullong src_count) {
	__exc_catch_if(pttyp, pttyp->replace(index, count, src, src_index, src_count), //
	"var& replace(ullong index, ullong count, "//
			"const var &src, ullong src_index = 0, ullong src_count = 0)",//
			return *this)
}
var& var::replace(ullong index, ullong count, cch *src, ullong src_index, ullong src_count) {
	__exc_catch_if(pttyp, pttyp->replace(index, count, src, src_index, src_count), //
	"var& replace(ullong index, ullong count, cch *src, "//
			"ullong src_index = 0, ullong src_count = 0)",//
			return *this)
}
var& var::replace(ullong index, ullong count, str &src, ullong src_index, ullong src_count) {
	__exc_catch_if(pttyp, pttyp->replace(index, count, src, src_index, src_count), //
	"var& replace(ullong index, ullong count,  str &src, "//
			"ullong src_index = 0, ullong src_count = 0)",//
			return *this)
}
var& var::replace(ullong index, ullong count, const var &src_first, const var &src_last) {
	__exc_catch_if(pttyp, pttyp->replace(index, count, src_first, src_last), //
	"var& replace(ullong index, ullong count, const var &src_first, "//
			"const var &src_last)",//
			return *this)
}
// ..
int var::compare(const var &with) const {
	__exc_catch_if(pttyp, return pttyp->compare(with), //
			"int compare(const var &with) const", return 0)
}
int var::compare(cch *with) const {
	__exc_catch_if(pttyp, return pttyp->compare(with), //
			"int compare(cch *with) const", return 0)
}
int var::compare(str &with) const {
	__exc_catch_if(pttyp, return pttyp->compare(with), //
			"int compare(str &with) const", return 0)
}
int var::compare(ullong index, ullong count, const var &with, ullong with_index, ullong with_count) const {
	__exc_catch_if(pttyp, return pttyp->compare(index, count, with, with_index, with_count), //
			"int compare(ullong index, ullong count, const var &with, "//
			"ullong with_index = 0, ullong with_count = 0) const", return 0)
}
int var::compare(ullong index, ullong count, cch *with, ullong with_index, ullong with_count) const {
	__exc_catch_if(pttyp, return pttyp->compare(index, count, with, with_index, with_count), //
			"int compare(ullong index, ullong count, cch *with, "//
			"ullong with_index = 0, ullong with_count = 0 ) const", return 0)
}
int var::compare(ullong index, ullong count, str &with, ullong with_index, ullong with_count) const {
	__exc_catch_if(pttyp, return pttyp->compare(index, count, with, with_index, with_count), //
			"int compare(ullong index, ullong count, str &with, "//
			"ullong with_index = 0, ullong with_count = 0) const", return 0)
}
//..
bool var::starts_with(const var &prefix) const {
	__exc_catch_if(pttyp, return pttyp->starts_with(prefix), //
			"bool starts_with(const var &prefix) const", return false)
}
bool var::starts_with(char prefix) const {
	__exc_catch_if(pttyp, return pttyp->starts_with(prefix), //
			"bool starts_with(char prefix) const", return false)
}
bool var::starts_with(cch *prefix) const {
	__exc_catch_if(pttyp, return pttyp->starts_with(prefix), //
			"bool starts_with(cch *prefix) const", return false)
}
bool var::ends_with(const var &suffix) const {
	__exc_catch_if(pttyp, return pttyp->ends_with(suffix), //
			"bool ends_with(const var &suffix) const", return false)
}
bool var::ends_with(char suffix) const {
	__exc_catch_if(pttyp, return pttyp->ends_with(suffix), //
			"bool ends_with(char suffix) const", return false)
}
bool var::ends_with(cch *suffix) const {
	__exc_catch_if(pttyp, return pttyp->ends_with(suffix), //
			"bool ends_with(cch *suffix) const", return false)
}
//..
var var::substr(ullong index, ullong count) {
	__exc_catch_if(pttyp, return pttyp->substr(index, count), //
			"var substr(ullong index = 0, ullong count = 0)", return var())
}
ullong var::copy(char *dest, ullong index, ullong count, bool zero) const {
	__exc_catch_if(pttyp, return pttyp->copy(dest, index, count, zero), //
			"ullong copy(char *dest, ullong index = 0, ullong count = 0, "//
			"bool zero = false) const", return 0)
}
//..
var var::find(const var &src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find(src, index), //
			"var find(const var &src, ullong index = 0) const", return var())
}
var var::find(char src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find(src, index), //
			"var find(char src, ullong index = 0) const", return var())
}
var var::find(cch *src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find(src, index), //
			"var find(cch *src, ullong index = 0) const", return var())
}
var var::find(str &src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find(src, index), //
			"var find(str &src, ullong index = 0) const", return var());
}
var var::rfind(const var &src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->rfind(src, index), //
			"var rfind(const var &src, ullong index = 0) const", return var())
}
var var::rfind(char src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->rfind(src, index), //
			"var rfind(char src, ullong index = 0) const", return var())
}
var var::rfind(cch *src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->rfind(src, index), //
			"var rfind(cch *src, ullong index = 0) const", return var())
}
var var::rfind(str &src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->rfind(src, index), //
			"var rfind(str &src, ullong index = 0) const", return var())
}
//..
var var::find_first_of(const var &src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_first_of(src, index), //
			"var find_first_of(const var &src, ullong index = 0) const", return var())
}
var var::find_first_of(char src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_first_of(src, index), //
			"var find_first_of(char src, ullong index = 0) const", return var())
}
var var::find_first_of(cch *src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_first_of(src, index), //
			"var find_first_of(cch *src, ullong index = 0) const", return var())
}
var var::find_first_of(str &src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_first_of(src, index), //
			"var find_first_of(str &src, ullong index = 0) const", return var())
}
var var::find_first_not_of(const var &src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_first_not_of(src, index), //
			"var find_first_not_of(const var &src, ullong index = 0) const",//
			return var())
}
var var::find_first_not_of(char src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_first_not_of(src, index), //
			"var find_first_not_of(char src, ullong index = 0) const",//
			return var())
}
var var::find_first_not_of(cch *src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_first_not_of(src, index), //
			"var find_first_not_of(cch *src, ullong index = 0) const",//
			return var())
}
var var::find_first_not_of(str &src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_first_not_of(src, index), //
			"var find_first_not_of(str &src, ullong index = 0) const",//
			return var())
}
var var::find_last_of(const var &src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_last_of(src, index), //
			"var find_last_of(const var &src, ullong index = 0) const",//
			return var())
}
var var::find_last_of(char src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_last_of(src, index), //
			"var find_last_of(char src, ullong index = 0) const",//
			return var())
}
var var::find_last_of(cch *src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_last_of(src, index), //
			"var find_last_of(cch *src, ullong index = 0) const",//
			return var())
}
var var::find_last_of(str &src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_last_of(src, index), //
			"var find_last_of(str &src, ullong index = 0) const",//
			return var())
}
var var::find_last_not_of(const var &src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_last_not_of(src, index), //
			"var find_last_not_of(const var &src, ullong index = 0) const",//
			return var())
}
var var::find_last_not_of(char src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_last_not_of(src, index), //
			"var find_last_not_of(char src, ullong index = 0) const",//
			return var())
}
var var::find_last_not_of(cch *src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_last_not_of(src, index), //
			"var find_last_not_of(cch *src, ullong index = 0) const",//
			return var())
}
var var::find_last_not_of(str &src, ullong index) const {
	__exc_catch_if(pttyp, return pttyp->find_last_not_of(src, index), //
			"var find_last_not_of(str &src, ullong index = 0) const",//
			return var())
}
// ..
int var::_int() const {
	__exc_catch_if(pttyp, return pttyp->_int(), //
			"int _int() const", return 0)
}
llong var::_llong() const {
	__exc_catch_if(pttyp, return pttyp->_llong(), //
			"llong _llong() const", return 0)
}
ullong var::_ullong() const {
	__exc_catch_if(pttyp, return pttyp->_ullong(), //
			"ullong _ullong() const", return 0)
}
float var::_float() const {
	__exc_catch_if(pttyp, return pttyp->_float(), //
			"float _float() const", return 0)
}
double var::_double() const {
	__exc_catch_if(pttyp, return pttyp->_double(), //
			"double _double() const", return 0)
}

var* var::_mkcopy() {
	return 0;
}
template<typename t1, typename t2> var* var::clc_prc(const ttyp<t1> *lv, t2 rv, int op, int m) const {
//
// op:  0-> + , 1-> * , 2->-
//  m:  0-> 'x op y' 1-> 'x op= y'
//
	
	if (lv->tid[0] == 'd') return clc_prc_db(lv, rv, op, m);
	
	ullong x;
	
#define exc(cf,dscr) \
	throw VarExcp(VarExcp::_mk_excp("out_of_number_range", dscr));\
	
	
	if (op == 0) { // +	
		if (lv->val >= 0 && rv >= 0) { // x+y
			x = (ullong) lv->val + (ullong) rv;
			if (x > LLONG_MAX) exc(cf, "result > LLONG_MAX")
			if (!m) return new ttyp<llong>((llong) x);
			else {
				const_cast<ttyp<t1>*>(lv)->setv((llong) x);
				return const_cast<ttyp<t1>*>(lv);
			}
		} else if (lv->val < 0 && rv < 0) { // -x+-y <-> -x-y
			x = (ullong) lv->val + rv;
			if (x <= LLONG_MAX) exc(cf, "result < LLONG_MIN")
			if (!m) return new ttyp<llong>((llong) x);
			else {
				const_cast<ttyp<t1>*>(lv)->setv((llong) x);
				return const_cast<ttyp<t1>*>(lv);
			}
		} else { // other cases: -x+y , x+-y 
			if (!m) return new ttyp<llong>(lv->val + rv);
			else {
				const_cast<ttyp<t1>*>(lv)->setv(lv->val + rv);
				return const_cast<ttyp<t1>*>(lv);
			}
		}
	} else if (op == 1) { // *
		llong z;
		if (lv->val > 0 && rv > 0) { // x*y
			z = LLONG_MAX / lv->val;
			if (rv > z) exc(cf, "result > LLONG_MAX")
			if (!m) return new ttyp<llong>(lv->val * rv);
			else {
				const_cast<ttyp<t1>*>(lv)->setv(lv->val * rv);
				return const_cast<ttyp<t1>*>(lv);
			}
		} else if (lv->val < 0 && rv < 0) { // -x*-y <-> x*y
			z = LLONG_MAX / lv->val;
			if (rv < z) exc(cf, "result > LLONG_MAX")
			if (!m) return new ttyp<llong>(lv->val * rv);
			else {
				const_cast<ttyp<t1>*>(lv)->setv(lv->val * rv);
				return const_cast<ttyp<t1>*>(lv);
			}
		} else {
			z = LLONG_MIN / lv->val;
			if ((lv->val < 0 && rv > z) || (rv < 0 && rv < z)) //
			exc(cf, "result < LLONG_MIN")
			if (!m) return new ttyp<llong>(lv->val * rv);
			else {
				const_cast<ttyp<t1>*>(lv)->setv(lv->val * rv);
				return const_cast<ttyp<t1>*>(lv);
			}
		}
	} else if (op == 2) { // -
		if (lv->val < 0) {
			if (rv > 0) { // -x-y
				x = (ullong) lv->val - rv;
				if (x <= LLONG_MAX) {
					exc(cf, "result < LLONG_MIN")
				}
				if (!m) return new ttyp<llong>((llong) x);
				else {
					const_cast<ttyp<t1>*>(lv)->setv((llong) x);
					return const_cast<ttyp<t1>*>(lv);
				}
			} else { // -x-(-y) <-> -x+y 
				if (!m) return new ttyp<llong>(lv->val - rv);
				else {
					const_cast<ttyp<t1>*>(lv)->setv(lv->val - rv);
					return const_cast<ttyp<t1>*>(lv);
				}
			}
		} else if (lv->val >= 0) {
			if (rv >= 0) { // x-y
				if (!m) return new ttyp<llong>(lv->val - rv);
				else {
					const_cast<ttyp<t1>*>(lv)->setv(lv->val - rv);
					return const_cast<ttyp<t1>*>(lv);
				}
			} else if (rv < 0) { // x-(-y) <-> x+y
				x = (ullong) lv->val + (ULLONG_MAX - (ullong) rv + 1);
				if (x > LLONG_MAX) //
				exc(cf, "result > LLONG_MAX")
				if (!m) return new ttyp<llong>((llong) x);
				else {
					const_cast<ttyp<t1>*>(lv)->setv((llong) x);
					return const_cast<ttyp<t1>*>(lv);
				}
			}
		}
	}
	return 0; // err
}
llong var::clc_prc_spc(llong lv, llong rv, int op) const {
	
//
// op:  0-> + , 1-> -
//
	
	ullong x;
	
	if (op == 0) { // +	
		if (lv >= 0 && rv >= 0) { // x+y
			x = (ullong) lv + (ullong) rv;
			if (x > LLONG_MAX) exc(cf, "result > LLONG_MAX")
			return (llong) x;
		} else if (lv < 0 && rv < 0) { // -x+-y <-> -x-y
			x = (ullong) lv + rv;
			if (x <= LLONG_MAX) exc(cf, "result < LLONG_MIN")
			return (llong) x;
		} else { // other cases: -x+y , x+-y 
			return lv + rv;
		}
	} else if (op == 2) { // -
		if (lv < 0) {
			if (rv > 0) { // -x-y
				x = (ullong) lv - rv;
				if (x <= LLONG_MAX) exc(cf, "result < LLONG_MIN")
				return (llong) x;
			} else { // -x-(-y) <-> -x+y 
				return lv - rv;
			}
		} else if (lv >= 0) {
			if (rv >= 0) {   // x-y
				return lv - rv;
			} else if (rv < 0) { // x-(-y) <-> x+y
				x = (ullong) lv + (ULLONG_MAX - (ullong) rv + 1);
				if (x > LLONG_MAX) exc(cf, "result > LLONG_MAX")
				return (llong) x;
			}
		}
	}
	return 0; // err
	
}
template<typename t1, typename t2> var* var::clc_prc_db(const ttyp<t1> *lv, t2 rv, int op, int m) const {
	
	double rs = 0;
	llong _rs;
	if (op == 0) { // +
		rs = lv->val + rv;
	} else if (op == 1) { // *
		rs = lv->val * rv;
	} else if (op == 2) { // -
		rs = lv->val - rv;
	} else if (op == 3) { // /
		rs = lv->val / rv; 
	}
	_rs = (llong) rs;
	if (m == 0) {
		if (rs == _rs) {
			return new ttyp<llong>(_rs);
		} else {
			return new ttyp<double>(rs);
		}
	} else {
		if (rs == _rs) {
			if (lv->tid[0] == 'i') {
				const_cast<ttyp<t1>*>(lv)->setv(_rs);
				return const_cast<ttyp<t1>*>(lv);
			} else return new ttyp<llong>(_rs);
		} else {
			if (lv->tid[0] == 'd') {
				const_cast<ttyp<t1>*>(lv)->setv(rs);
				return const_cast<ttyp<t1>*>(lv);
			} else return new ttyp<double>(rs);
		}
	}
	return 0; // err
}
template<typename t1, typename t2> //
var* var::clc_prc_div_spc(const ttyp<t1> *lv, t2 rv, int m) const {
	
	if (tid[0] == 'i') {
		if (!((llong) lv->val % rv)) {
			if (!m) {
				return new ttyp<llong>(lv->val / (llong) rv);
			} else {
				const_cast<ttyp<t1>*>(lv)->setv(lv->val / rv);
				return const_cast<ttyp<t1>*>(lv);
			}
		} else //
		return new ttyp<double>(double(lv->val) / double(rv));
		//
	} else if (tid[0] == 'd') {
		if (!m) {
			return new ttyp<double>(double(lv->val) / double(rv));
		} else {
			const_cast<ttyp<t1>*>(lv)->setv(lv->val / double(rv));
			return const_cast<ttyp<t1>*>(lv);
		}
	}
	return 0; // err
}
var& var::sp_op_eq_chk(const var &rv) {
	//                   var&(top) | ttyp<..>*
	if (pttyp != &rv) {
		var &_rv = const_cast<var&>(rv);
		if (pttyp) __free_ttyp(pttyp);
		//
		if (_rv.pttyp == 0) {	// ttyp<..>*
			pttyp = &_rv;
		} else {	            // var&(top)   !used but persp
			pttyp = _rv.pttyp;
			_rv.pttyp = 0;	    // cut link with ttyp<..>*
		}
		tid = _rv.tid;
#ifdef _VAR_DBG
		pdbg = _rv.pdbg;
#endif
	}
	return *this;
}
template<typename t> ttyp<t>* var::dcst(var *pttyp) const {
	return dynamic_cast<ttyp<t>*>(pttyp);
}
void var::IE_solver(const var *x, var at, var msg) const {
	if (tid == 0) {
		VarExcp::_un_ini_var(at);
	} else if (tid[0] == 'u' && tid[1] == 'd') {
		throw(VarExcp(VarExcp::_mk_excp(x, "use_of_undefined_var", at, msg), 1));
	} else {
		throw(VarExcp(VarExcp::_mk_excp(x, "unsupported_interface", at, msg), //
		x->pttyp ? 1 : 0));
	}
}
void var::OE_solver(bool top, const var *x, const var *rv, var at, var msg) const {
	if (tid == 0) {
		VarExcp::_un_ini_var(at);
	} else if ((tid[0] == 'u' && tid[1] == 'd') || //
			(rv && (rv->tid[0] == 'u' && rv->tid[1] == 'd'))) {
		throw(VarExcp(VarExcp::_mk_excp(x, "use_of_undefined_var", at, msg), 1));
	} else {
		throw(VarExcp(VarExcp::_mk_excp(x, "unsupported_operation", at, msg), //
		top && x->pttyp ? 1 : 0));
	}
}
void var::setv(var &rv) {
}
void var::setv(double rv) {
}
void var::setv(llong rv) {
}
void var::setv(bool rv) {
}
void var::setv(char rv) {
}
void var::setv(cch *rv) {
}
void var::setv(const str &rv) {
}
void var::setv(const __iniL <var> &rv) {
}
void var::setv(const __vct <var> &rv) {
}
template<typename t> t* var::getPv(const var *pttyp) const {
	return &(dynamic_cast<ttyp<t>*>(const_cast<var*>(pttyp))->val);
}
var var::idx_solver(ullong index) const {
	var s = "";
	if (index <= LLONG_MAX) {
		s += std::to_string(index);
	} else {
		s += std::to_string(index) += " or ";
		s += llong(index);
	}
	return s;
}
var* var::ret_bs() {
	return this;
}
char* var::get_poi() const {
	return pttyp->get_poi();
}

//------------------------------------------------------------- ttyp<str> (out)
//-----------------------------------------------------------------------------

var ttyp<str>::insert(ullong index, const var &src_first, const var &src_last) {
	auto l = val.length();
	if ((l == 0 && index != 0) || index > l) //
	throw VarExcp(VarExcp::_mk_excp("out_of_range", //
			var( { "bad 'index'", " ( ", idx_solver(index), " )" }).join()));
	ttyp<Ic_str> *y, *z;
	ttyp<Irc_str> *_y, *_z;
	
	if ((y = dcst<Ic_str>(src_first.pttyp)) && //   
			(z = dcst<Ic_str>(src_last.pttyp)) && (y->bs == z->bs) && //
			(&(*y->_Ic_str) < &(*z->_Ic_str))) {
		try {
			++isyn;
			auto w = val.insert(val.begin() += index, y->_Ic_str, z->_Ic_str);
			__upd_val();
			return var(w, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	} else if ((_y = dcst<Irc_str>(src_first.pttyp)) //
	&& (_z = dcst<Irc_str>(src_last.pttyp)) && (_y->bs == _z->bs) && //
			(&(*_y->_Irc_str) > &(*_z->_Irc_str))) {
		try {
			++isyn;
			auto w = val.insert(val.begin() += index, _y->_Irc_str, _z->_Irc_str);
			__upd_val();
			return var(w, this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	} else throw VarExcp(VarExcp::_mk_excp("bad_iterator", //
			"bad: 'src_first' or 'src_last' or both"));
	return var();
}
var ttyp<str>::split(char sep) const {
	int l = val.length();
	if (l > 0) {
		auto x = new ttyp<__vct<var>>(__vct<var>());
		str s = "";
		int i = 0, ii = 0;
		char ch, _ch;
		
		while (i < l) {
			ch = val[i];
			if (ch != sep) {
				_ch = ch;
				s += ch, ++ii;
			} else {
				if (ii) {
					if (ii == 1) x->push_back(_ch);
					else x->push_back(s);
				}
				s = "", ii = 0;
			}
			++i;
		}
		if (ii) {
			if (ii == 1) x->push_back(_ch);
			else x->push_back(s);
		}
		return *x;
	}
	return var();
}
var& ttyp<str>::ssplit(char sep) {
	int l = val.length();
	if (l > 0) {
		auto x = new ttyp<__vct<var>>(__vct<var>());
		++isyn;
		var s = "";
		int i = 0, ii = 0;
		char ch, _ch;
		while (i < l) {
			ch = val[i];
			if (ch != sep) {
				_ch = ch;
				s += ch, ++ii;
			} else {
				if (ii) {
					if (ii == 1) x->push_back(_ch);
					else x->push_back(s);
				}
				s = "", ii = 0;
			}
			++i;
		}
		if (ii) {
			if (ii == 1) x->push_back(_ch);
			else x->push_back(s);
		}
		return *x;
	}
	return *this;
}
var& ttyp<str>::append(const var &src_first, const var &src_last) {
	ttyp<Ic_str> *y, *z;
	ttyp<Irc_str> *_y, *_z;
	
	if ((y = dcst<Ic_str>(src_first.pttyp)) && //   
			(z = dcst<Ic_str>(src_last.pttyp)) && (y->bs == z->bs) && //
			(&(*y->_Ic_str) < &(*z->_Ic_str))) {
		try {
			++isyn;
			val.append(y->_Ic_str, z->_Ic_str);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	} else if ((_y = dcst<Irc_str>(src_first.pttyp)) //
	&& (_z = dcst<Irc_str>(src_last.pttyp)) && (_y->bs == _z->bs) && //
			(&(*_y->_Irc_str) > &(*_z->_Irc_str))) {
		try {
			++isyn;
			val.append(_y->_Irc_str, _z->_Irc_str);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	} else //
	throw VarExcp(VarExcp::_mk_excp("bad_iterator", //
			"bad: 'src_first' or 'src_last' or both"));
	return *this;
}
var& ttyp<str>::replace(ullong index, ullong count, const var &src_first, const var &src_last) {
#define __rpl2_fnm "var& replace(ullong index, ullong count,"\
                	"const var &src_first, const var &src_last)"	
	
	auto l = val.length();
	if (l == 0 || index >= l) throw VarExcp(VarExcp::_mk_excp("out_of_range", //
			(l == 0 ? "empty" : var( { "bad 'index'", //
					" ( ", idx_solver(index), " )" }).join())));
	else if (count > l - index) //
	throw VarExcp(VarExcp::_mk_excp("out_of_range", "bad 'count'"));
	ttyp<Ic_str> *y, *z;
	ttyp<Irc_str> *_y, *_z;
	
	if ((y = dcst<Ic_str>(src_first.pttyp)) && //   
			(z = dcst<Ic_str>(src_last.pttyp)) && (y->bs == z->bs) && //
			(&(*y->_Ic_str) < &(*z->_Ic_str))) {
		try {
			++isyn;
			val.replace(val.begin() += index, val.begin() += (index + count), y->_Ic_str, z->_Ic_str);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	} else if ((_y = dcst<Irc_str>(src_first.pttyp)) //
	&& (_z = dcst<Irc_str>(src_last.pttyp)) && (_y->bs == _z->bs) && //
			(&(*_y->_Irc_str) > &(*_z->_Irc_str))) {
		try {
			++isyn;
			val.replace(val.begin() += index, val.begin() += (index + count), _y->_Irc_str, _z->_Irc_str);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	} else //
	throw VarExcp(VarExcp::_mk_excp("bad_iterator", //
			"bad: 'src_first' or 'src_last' or both"));
	return *this;
}
var& ttyp<Ic_str>::operator*() {
	
	__is_valid()
	
	if (idx == end) //
	throw VarExcp(VarExcp::_mk_excp("out_of_range", "[left/right-out]"));
	if (idx1.pttyp == 0) idx1 = char();
	ttyp<char> *p = (ttyp<char>*) idx1.pttyp;
	p->p2 = &const_cast<char&>(*_Ic_str);
	p->setv_sp(p->p2[0]);
	return idx1;
}
var& ttyp<Irc_str>::operator*() {
	
	__is_valid()
	
	if (idx == end) //
	throw VarExcp(VarExcp::_mk_excp("out_of_range", "[left/right-out]"));
	if (idx1.pttyp == 0) idx1 = char();
	ttyp<char> *p = (ttyp<char>*) idx1.pttyp;
	p->p2 = &const_cast<char&>(*_Irc_str);
	p->setv_sp(p->p2[0]);
	return idx1;
}
var& ttyp<str>::ret_idx(ullong index) {
	
	char *pc = const_cast<char*>(&val.at(index));
	if (idx.pttyp == 0) const_cast<var&>(idx) = char();
	ttyp<char> *p = (ttyp<char>*) idx.pttyp;
	
	if (!p->trg) {
		p->p1 = pc;
		p->setv_sp(p->p1[0]);
		p->trg = 1;
	} else {
		p->p2 = pc;
		p->setv_sp(p->p2[0]);
		p->trg = 0;
	}
	return idx;
}
void ttyp<str>::set_ivalid_itrs() {
	var_li::idx *_idx;
	_idx = itrs.fst;
	while (_idx) {
		if (_idx->val->tid[1] == 'r') { // Ir_str
			auto x = dcst<Irc_str>(_idx->val);
			x->bs = 0, x->stg = 0;
#ifdef _VAR_DBG   
			x->ddbg.sval = 0;
#endif
		} else { // I_str
			auto x = dcst<Ic_str>(_idx->val);
			x->bs = 0, x->stg = 0;
#ifdef _VAR_DBG   
			x->ddbg.sval = 0;
#endif
		}
		_idx = _idx->nxt;
	}
}
bool ttyp<str>::equal(const var &src_first, const var &src_last, ullong index, //
		ullong count) const {
	
	auto l = val.size();
	if ((l == 0 && index != 0) || index > l) //
	throw VarExcp(VarExcp::_mk_excp("out_of_range", //
			var( { "bad 'index'", " ( ", idx_solver(index), " )" }).join()));
	ullong end;
	if (count == 0) end = l;
	else if ((end = index + count) > l) {
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				var( { "bad 'count'", " ( ", idx_solver(count), " )" }).join()));
	}
	
	ttyp<Ic_str> *y, *z;
	ttyp<Irc_str> *_y, *_z;
	auto b = val.begin() += index, e = val.begin() += end;
	
	if ((y = dcst<Ic_str>(src_first.pttyp)) && //   
			(z = dcst<Ic_str>(src_last.pttyp)) && (y->bs == z->bs) && //
			(&(*y->_Ic_str) < &(*z->_Ic_str))) {
		
		while (y->_Ic_str != z->_Ic_str) {
			
			if (*b != *y->_Ic_str) return false;
			
			++y->_Ic_str;
			++b;
			if (b == e) {
				if (y->_Ic_str == z->_Ic_str) return true;
				else break;
			}
		}
	} else if ((_y = dcst<Irc_str>(src_first.pttyp)) //
	&& (_z = dcst<Irc_str>(src_last.pttyp)) && (_y->bs == _z->bs) && //
			(&(*_y->_Irc_str) > &(*_z->_Irc_str))) {
		
		while (_y->_Irc_str != _z->_Irc_str) {
			
			if (*b != *_y->_Irc_str) return false;
			
			++_y->_Irc_str;
			++b;
			if (b == e) {
				if (_y->_Irc_str == _z->_Irc_str) return true;
				else break;
			}
		}
	} else //
	throw VarExcp(VarExcp::_mk_excp("bad_iterator", //
			"bad: 'src_first' or 'src_last' or both"));
	
	return false;
}
var ttyp<str>::mismatch(const var &src_first, const var &src_last, ullong index, //
		ullong count) const {
	
	auto l = val.size();
	if ((l == 0 && index != 0) || index > l) //
	throw VarExcp(VarExcp::_mk_excp("out_of_range", //
			var( { "bad 'index'", " ( ", idx_solver(index), " )" }).join()));
	ullong end;
	if (count == 0) end = l;
	else if ((end = index + count) > l) {
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				var( { "bad 'count'", " ( ", idx_solver(count), " )" }).join()));
	}
	
	ttyp<Ic_str> *y, *z;
	ttyp<Irc_str> *_y, *_z;
	auto b = val.begin() += index, e = val.begin() += end;
	
	if ((y = dcst<Ic_str>(src_first.pttyp)) && //   
			(z = dcst<Ic_str>(src_last.pttyp)) && (y->bs == z->bs) && //
			(&(*y->_Ic_str) < &(*z->_Ic_str))) {
		
		while (y->_Ic_str != z->_Ic_str) {
			
			if (*b != *y->_Ic_str) //
			return {var(b,this), var(y->_Ic_str, y->bs), var(),
				var()};
			
			++y->_Ic_str;
			++b;
			if (b == e) {
				if (y->_Ic_str == z->_Ic_str) return false;
				else break;
			}
		}
		return {var(b,this),
			var(y->_Ic_str, y->bs), var(),
			var()};
		
	} else if ((_y = dcst<Irc_str>(src_first.pttyp)) //
	&& (_z = dcst<Irc_str>(src_last.pttyp)) && (_y->bs == _z->bs) && //
			(&(*_y->_Irc_str) > &(*_z->_Irc_str))) {
		
		while (_y->_Irc_str != _z->_Irc_str) {
			
			if (*b != *_y->_Irc_str) //
			return {var(b,this), var(_y->_Irc_str, _y->bs), var(),
				var()};
			
			++_y->_Irc_str;
			++b;
			if (b == e) {
				if (_y->_Irc_str == _z->_Irc_str) return false;
				else break;
			}
		}
		return {var(b,this),
			var(_y->_Irc_str, _y->bs), var(),
			var()};
	} else //
	throw VarExcp(VarExcp::_mk_excp("bad_iterator", //
			"bad: 'src_first' or 'src_last' or both"));
	
	return false;
}

//------------------------------------------------------ ttyp<__vct<var>> (out)
//-----------------------------------------------------------------------------

var ttyp<__vct<var>>::insert(ullong index, const var &src_first, const var &src_last) {
	auto l = val.size();
	if ((l == 0 && index != 0) || index > l) //
	throw VarExcp(VarExcp::_mk_excp("out_of_range", //
			var( { "bad 'index'", " ( ", idx_solver(index), " )" }).join()));
	ttyp<Ic_arr> *y, *z;
	ttyp<Irc_arr> *_y, *_z;
	
	if ((y = dcst<Ic_arr>(src_first.pttyp)) && //   
			(z = dcst<Ic_arr>(src_last.pttyp)) && (y->bs == z->bs) && //
			(&(*y->_Ic_arr) < &(*z->_Ic_arr))) {
		try {
			++isyn;
			return var(val.insert(val.begin() += index, y->_Ic_arr, z->_Ic_arr), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	} else if ((_y = dcst<Irc_arr>(src_first.pttyp)) //
	&& (_z = dcst<Irc_arr>(src_last.pttyp)) && (_y->bs == _z->bs) && //
			(&(*_y->_Irc_arr) > &(*_z->_Irc_arr))) {
		try {
			++isyn;
			return var(val.insert(val.begin() += index, _y->_Irc_arr, _z->_Irc_arr), this);
		} catch (VarExcp &e) {
			throw;
		} catch (std::exception &e) {
			throw VarExcp(VarExcp::_mk_excp("length_error", "max_size"));
		}
	} else //
	throw VarExcp(VarExcp::_mk_excp("bad_iterator", //
			"bad: 'src_first' or 'src_last' or both"));
	return var();
}
var ttyp<__vct<var>>::join(char sep) const {
	if (val.size() > 0) {
		auto x = new ttyp<str>("");
		join_rec(sep, this, x);
		return *x;
	}
	return var();
}
var& ttyp<__vct<var>>::sjoin(char sep) {
	if (val.size() > 0) {
		++isyn;
		auto x = new ttyp<str>("");
		join_rec(sep, this, x);
#ifdef _VAR_DBG   
		x->upd_val();
#endif
		return *x;
	}
	return *this;
}
void ttyp<__vct<var>>::join_rec(char sep, const var *el, ttyp<str> *x) const {
	auto y = dynamic_cast<ttyp<__vct<var>>*>(const_cast<var*>(el));
	auto l = y->val.size();
	ullong i = 0, _l = l - 1;
	while (i < l) {
		el = &y->val[i];
		if (el->is_arr()) //
		join_rec(sep, el->pttyp, x);
		else {
			x->val += *el;
			if (sep != 0 && i < _l) x->val += sep;
		}
		++i;
	}
}
void ttyp<__vct<var>>::set_ivalid_itrs() {
	var_li::idx *_idx;
	_idx = itrs.fst;
	while (_idx) {
		if (_idx->val->tid[1] == 'r') { // Ir_arr
			auto x = dcst<Irc_arr>(_idx->val);
			x->bs = 0, x->stg = 0;
#ifdef _VAR_DBG   
			x->ddbg.sval = 0, x->ddbg.vval = 0;
#endif
		} else { // I_arr
			auto x = dcst<Ic_arr>(_idx->val);
			x->bs = 0, x->stg = 0;
#ifdef _VAR_DBG   
			x->ddbg.sval = 0, x->ddbg.vval = 0;
#endif
		}
		_idx = _idx->nxt;
	}
}
bool ttyp<__vct<var>>::equal(const var &src_first, const var &src_last, //
		ullong index, ullong count) const {
	
	auto l = val.size();
	if ((l == 0 && index != 0) || index >= l) //
	throw VarExcp(VarExcp::_mk_excp("out_of_range", //
			var( { "bad 'index'", " ( ", idx_solver(index), " )" }).join()));
	ullong end;
	if (count == 0) end = l;
	else if ((end = index + count) > l) {
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				var( { "bad 'count'", " ( ", idx_solver(count), " )" }).join()));
	}
	
	ttyp<Ic_arr> *y, *z;
	ttyp<Irc_arr> *_y, *_z;
	
	if ((y = dcst<Ic_arr>(src_first.pttyp)) && //   
			(z = dcst<Ic_arr>(src_last.pttyp)) && (y->bs == z->bs) && //
			(&(*y->_Ic_arr) < &(*z->_Ic_arr))) {
		
		auto b = val.begin() += index, e = val.begin() += end;
		while (y->_Ic_arr != z->_Ic_arr) {
			
			if (varF::cmp(b->tid, (y->_Ic_arr)->tid)) {
				if (b->tid[0] == 'a') {
					auto l1 = (*b).length(), l2 = (*y->_Ic_arr).length();
					if (l1 != l2) return false;
					else if (l1 > 0 && l2 > 0) {
						if (!b->equal(y->_Ic_arr->begin(), y->_Ic_arr->end(), 0)) //
						return false;
					}
				} else if (b->tid[0] == 'I') {
					if (b->tid[2] == 'a' || b->tid[3] == 'a') {
						if (&(**b) != &(**y->_Ic_arr)) return false;
					} else {
						if ((**b).get_poi() != (**y->_Ic_arr).get_poi()) return false;
					}
				} else if (b->tid[0] == 'u') {
					//...
				} else if (b->tid[0] == 'm') {
					
					return ((ttyp<VMAP>*) ((*b).pttyp))->map_equal(*y->_Ic_arr);
					
				} else if (*b != *y->_Ic_arr) return false;
			} else return false;
			++y->_Ic_arr;
			++b;
			if (b == e) {
				if (y->_Ic_arr == z->_Ic_arr) return true;
				else break;
			}
		}
	} else if ((_y = dcst<Irc_arr>(src_first.pttyp)) //
	&& (_z = dcst<Irc_arr>(src_last.pttyp)) && (_y->bs == _z->bs) && //
			(&(*_y->_Irc_arr) > &(*_z->_Irc_arr))) {
		
		auto b = val.begin() += index, e = val.begin() += end;
		while (_y->_Irc_arr != _z->_Irc_arr) {
			
			if (varF::cmp(b->tid, (_y->_Irc_arr)->tid)) {
				if (b->tid[0] == 'a') {
					auto l1 = (*b).length(), l2 = (*_y->_Irc_arr).length();
					if (l1 != l2) return false;
					if (l1 > 0 && l2 > 0) {
						if (!b->equal(_y->_Irc_arr->begin(), _y->_Irc_arr->end(), 0)) //
						return false;
					}
				} else if (b->tid[0] == 'I') {
					if (b->tid[2] == 'a' || b->tid[3] == 'a') {
						if (&(**b) != &(**_y->_Irc_arr)) return false;
					} else {
						if ((**b).get_poi() != (**_y->_Irc_arr).get_poi()) return false;
					}
				} else if (b->tid[0] == 'u') {
					//...
				} else if (b->tid[0] == 'm') {
					
					return ((ttyp<VMAP>*) ((*b).pttyp))->map_equal(*_y->_Irc_arr);
					
				} else if (*b != *_y->_Irc_arr) return false;
			} else return false;
			++_y->_Irc_arr;
			++b;
			if (b == e) {
				if (_y->_Irc_arr == _z->_Irc_arr) return true;
				else break;
			}
		}
	} else //
	throw VarExcp(VarExcp::_mk_excp("bad_iterator", //
			"bad: 'src_first' or 'src_last' or both"));
	
	return false;
}
var ttyp<__vct<var>>::mismatch(const var &src_first, const var &src_last, ullong index, //
		ullong count) const {
	
	auto l = val.size();
	if ((l == 0 && index != 0) || index >= l) //
	throw VarExcp(VarExcp::_mk_excp("out_of_range", //
			var( { "bad 'index'", " ( ", idx_solver(index), " )" }).join()));
	ullong end;
	if (count == 0) end = l;
	else if ((end = index + count) > l) {
		throw VarExcp(VarExcp::_mk_excp("out_of_range", //
				var( { "bad 'count'", " ( ", idx_solver(count), " )" }).join()));
	}
	
	return _mismatch(this, src_first, src_last, index, end, 0);
	
}
var ttyp<__vct<var>>::_mismatch(const ttyp<__vct<var>> *ctx, const var &src_first, //
		const var &src_last, ullong index, ullong end, int rec) const {
	
	ttyp<Ic_arr> *y, *z;
	ttyp<Irc_arr> *_y, *_z;
	var x;
	
	auto b = ctx->val.begin() += index, e = ctx->val.begin() += end;
	
	if ((y = dcst<Ic_arr>(src_first.pttyp)) && //   
			(z = dcst<Ic_arr>(src_last.pttyp)) && (y->bs == z->bs) && //
			(&(*y->_Ic_arr) < &(*z->_Ic_arr))) {
		
		while (y->_Ic_arr != z->_Ic_arr) {
			
			if (varF::cmp(b->tid, (y->_Ic_arr)->tid)) {
				if (b->tid[0] == 'a') {
					auto l1 = (*b).length(), l2 = (*y->_Ic_arr).length();
					if (l1 > 0 && l2 > 0) {
						x = _mismatch((ttyp<__vct<var>>*) (*b).pttyp, //
								y->_Ic_arr->begin(), y->_Ic_arr->end(), 0, l1, ++rec);
						--rec;
						if (x) {
							if (rec == 0) return //
							{	var(b,ctx), var(y->_Ic_arr, y->bs), x[0], x[1]};
							else return {x[0], x[1],
								var(),
								var()};
						} else return false;
					} else if (l1 == 0 && l2 == 0) {
						//..
					} else return {var(b,ctx), var(y->_Ic_arr, y->bs), var(),
						var()};
				} else if (b->tid[0] == 'I') {
					if (b->tid[2] == 'a' || b->tid[3] == 'a') {
						if (&(**b) != &(**y->_Ic_arr)) //
						return {var(b,ctx), var(y->_Ic_arr, y->bs), var(),
							var()};
					} else {
						if ((**b).get_poi() != (**y->_Ic_arr).get_poi()) //
						return {var(b,ctx), var(y->_Ic_arr, y->bs), var(),
							var()};
					}
				} else if (b->tid[0] == 'u') {
					//.. ok
				} else if (b->tid[0] == 'm') {
					
					if (!((ttyp<VMAP>*) ((*b).pttyp))->map_equal(*y->_Ic_arr)) {
						return {var(b,ctx), var(y->_Ic_arr, y->bs), var(), var()};
					}
				} else if (*b != *y->_Ic_arr) //
				return {var(b,ctx), var(y->_Ic_arr, y->bs), var(), var()};
				
			} else return {var(b,ctx), var(y->_Ic_arr, y->bs), var(),
				var()};
			
			++y->_Ic_arr;
			++b;
			if (b == e) {
				if (y->_Ic_arr == z->_Ic_arr) return false;
				else break;
			}
		}
		return {var(b,ctx),
			var(y->_Ic_arr, y->bs), var(),
			var()};
		
	} else if ((_y = dcst<Irc_arr>(src_first.pttyp)) //
	&& (_z = dcst<Irc_arr>(src_last.pttyp)) && (_y->bs == _z->bs) && //
			(&(*_y->_Irc_arr) > &(*_z->_Irc_arr))) {
		
		while (_y->_Irc_arr != _z->_Irc_arr) {
			
			if (varF::cmp(b->tid, (_y->_Irc_arr)->tid)) {
				if (b->tid[0] == 'a') {
					auto l1 = (*b).length(), l2 = (*_y->_Irc_arr).length();
					if (l1 > 0 && l2 > 0) {
						x = _mismatch((ttyp<__vct<var>>*) (*b).pttyp, //
								_y->_Irc_arr->rbegin(), _y->_Irc_arr->rend(), 0, l1, ++rec);
						--rec;
						if (x) {
							if (rec == 0) return //
							{	var(b,ctx), var(_y->_Irc_arr, _y->bs), x[0], x[1]};
							else return {x[0], x[1],
								var(),
								var()};
						} else return false;
					} else if (l1 == 0 && l2 == 0) {
						//.. ok
					} else return {var(b,ctx), var(_y->_Irc_arr, _y->bs), var(),
						var()};
				} else if (b->tid[0] == 'I') {
					if (b->tid[2] == 'a' || b->tid[3] == 'a') {
						if (&(**b) != &(**_y->_Irc_arr)) //
						return {var(b,ctx), var(_y->_Irc_arr, _y->bs), var(),
							var()};
					} else {
						if ((**b).get_poi() != (**_y->_Irc_arr).get_poi()) //
						return {var(b,ctx), var(_y->_Irc_arr, _y->bs), var(),
							var()};
					}
				} else if (b->tid[0] == 'u') {
					//...
				} else if (b->tid[0] == 'm') {
					
					if (!((ttyp<VMAP>*) ((*b).pttyp))->map_equal(*_y->_Irc_arr)) {
						return {var(b,ctx), var(_y->_Irc_arr, _y->bs), var(), var()};
					}
				} else if (*b != *_y->_Irc_arr) //
				return {var(b,ctx), var(_y->_Irc_arr, _y->bs), var(), var()};
				
			} else return {var(b,ctx), var(_y->_Irc_arr, _y->bs), var(),
				var()};
			
			++_y->_Irc_arr;
			++b;
			if (b == e) {
				if (_y->_Irc_arr == _z->_Irc_arr) return false;
				else break;
			}
		}
		return {var(b,ctx),
			var(_y->_Irc_arr, _y->bs), var()
			, var()};
		
	} else //
	throw VarExcp(VarExcp::_mk_excp("bad_iterator", //
			"bad: 'src_first' or 'src_last' or both"));
	
	return false;
}
void ttyp<__vct<var>>::arr_prnt_rec(const __vct <var> &x, str &s, int rec, int spc) const {
	
	int i = 0, l = x.size();
	
	if (l > 0) {
		str _s = " ";
		if (rec == 0) _s = "";
		_s += "[ " + std::to_string(l) + " ]{";
		spc += _s.length();
		s += _s;
		
		while (i < l) {
			if (x[i].is_arr()) {
				arr_prnt_rec(*(getPv<__vct<var>>(x[i].pttyp)), s, ++rec, spc);
				--rec;
			} else {
				s += " ";
				if (x[i].is_def()) {
					if (x[i].is_str()) {
						s += "\"", s += (str) x[i], s += "\"";
					} else if (x[i].is_char()) {
						s += "'", s += (str) x[i], s += "'";
					} else if (x[i].is_map()) {
						auto _x = (ttyp<VMAP>*) x[i].pttyp;
						s += _x->from_arr_prn(spc);
					} else {
						s += (str) x[i];
					}
				} else s += "|udf|";
			}
			if (i < l - 1) {
				s += ",\n";
				s.append(spc, ' ');
			}
			++i;
		}
		s += " }";
	} else {
		if (rec == 0) s += "{ }";
		else s += " { }";
	}
}

//------------------------------------------------- GLOBAL OP: + , - , * , /
//--------------------------------------------------------------------------

void GL_EX_solver(VarExcp &ex, var at, const var *rv) {
	if (ex.exc[0].find("undefined")) {
		throw(VarExcp(VarExcp::_mk_excp("use_of_undefined_var", at, //
				"bad 'rv'"), 1));
	} else if (ex.exc[0].find("unsupported")) {
		throw(VarExcp(VarExcp::_mk_excp("unsupported_operation", at, //
				"unsupported 'rv' [ var:" + var(rv->gtid()) += " ]"), 1));
	} else throw VarExcp(VarExcp::_mk_excp(ex.exc[0], at, ex.exc[3]), 1);
}

#define __pls_var(tp) \
    try { \
	return var(lv) + rv; \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, "var operator+(const "#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return var(); \

template<typename t> var operator+(const t &lv, const var &rv) {
	__pls_var(t)
}
template<> var operator+(const int &lv, const var &rv) {
	__pls_var(int)
}
template<> var operator+(const long &lv, const var &rv) {
	__pls_var(long)
}
template<> var operator+(const float &lv, const var &rv) {
	__pls_var(float)
}
template<> var operator+(const double &lv, const var &rv) {
	__pls_var(double)
}
template<> var operator+(const llong &lv, const var &rv) {
	__pls_var(llong)
}
template<> var operator+(const ullong &lv, const var &rv) {
	__pls_var(ullong)
}
var operator+(cch *lv, const var &rv) {
	try {
		return var(lv) + rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "var operator+(cch *lv,"
				" const var &rv) [GLB]", &rv);
	}
	return var();
}
var operator+(char lv, const var &rv) {
	try {
		return var(lv) + rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "var operator+(char lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}
var operator+(const str &lv, const var &rv) {
	try {
		return var(lv) + rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "var operator+(const str &lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}

#define __min_var(tp) \
    try { \
	return var(lv) - rv; \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, "var operator-(const "#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return var(); \

template<typename t> var operator-(const t &lv, const var &rv) {
	__min_var(t)
}
template<> var operator-(const int &lv, const var &rv) {
	__min_var(int)
}
template<> var operator-(const long &lv, const var &rv) {
	__min_var(long)
}
template<> var operator-(const float &lv, const var &rv) {
	__min_var(float)
}
template<> var operator-(const double &lv, const var &rv) {
	__min_var(double)
}
template<> var operator-(const llong &lv, const var &rv) {
	__min_var(llong)
}
template<> var operator-(const ullong &lv, const var &rv) {
	__min_var(ullong)
}

#define __mul_var(tp) \
    try { \
	return var(lv) * rv; \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, "var operator*(const "#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return var(); \

template<typename t> var operator*(const t &lv, const var &rv) {
	__mul_var(t)
}
template<> var operator*(const int &lv, const var &rv) {
	__mul_var(int)
}
template<> var operator*(const long &lv, const var &rv) {
	__mul_var(long)
}
template<> var operator*(const float &lv, const var &rv) {
	__mul_var(float)
}
template<> var operator*(const double &lv, const var &rv) {
	__mul_var(double)
}
template<> var operator*(const llong &lv, const var &rv) {
	__mul_var(llong)
}
template<> var operator*(const ullong &lv, const var &rv) {
	__mul_var(ullong)
}

#define __div_var(tp) \
    try { \
	return var(lv) / rv; \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, "var operator/(const "#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return var(); \

template<typename t> var operator/(const t &lv, const var &rv) {
	__div_var(t)
}
template<> var operator/(const int &lv, const var &rv) {
	__div_var(int)
}
template<> var operator/(const long &lv, const var &rv) {
	__div_var(long)
}
template<> var operator/(const float &lv, const var &rv) {
	__div_var(float)
}
template<> var operator/(const double &lv, const var &rv) {
	__div_var(double)
}
template<> var operator/(const llong &lv, const var &rv) {
	__div_var(llong)
}
template<> var operator/(const ullong &lv, const var &rv) {
	__div_var(ullong)
}

// ---------------------- GLOBAL OP: +-*/ =

#define __pls_eq_var(tp) \
    try { \
	return lv += (tp) (rv); \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, #tp"& operator+=("#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return lv; \

template<typename t> t& operator+=(t &lv, const var &rv) {
	__pls_eq_var(t)
}
template<> int& operator+=(int &lv, const var &rv) {
	__pls_eq_var(int)
}
template<> long& operator+=(long &lv, const var &rv) {
	__pls_eq_var(long)
}
template<> float& operator+=(float &lv, const var &rv) {
	__pls_eq_var(float)
}
template<> double& operator +=(double &lv, const var &rv) {
	__pls_eq_var(double)
}
template<> llong& operator+=(llong &lv, const var &rv) {
	__pls_eq_var(llong)
}
template<> ullong& operator+=(ullong &lv, const var &rv) {
	__pls_eq_var(ullong)
}
str& operator+=(str &lv, const var &rv) {
	try {
		return lv += (str) rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "str& operator+=(const str &lv,"
				" const var &rv) [GLB]", &rv);
	}
	return lv;
}

#define __min_eq_var(tp) \
    try { \
	return lv -= (tp) (rv); \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, #tp"& operator-=("#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return lv; \

template<typename t> t& operator-=(t &lv, const var &rv) {
	__min_eq_var(t)
}
template<> int& operator-=(int &lv, const var &rv) {
	__min_eq_var(int)
}
template<> long& operator-=(long &lv, const var &rv) {
	__min_eq_var(long)
}
template<> float& operator-=(float &lv, const var &rv) {
	__min_eq_var(float)
}
template<> double& operator-=(double &lv, const var &rv) {
	__min_eq_var(double)
}
template<> llong& operator-=(llong &lv, const var &rv) {
	__min_eq_var(llong)
}
template<> ullong& operator-=(ullong &lv, const var &rv) {
	__min_eq_var(ullong)
}

#define __mul_eq_var(tp) \
    try { \
	return lv *= (tp) (rv); \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, #tp"& operator*=("#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return lv; \

template<typename t> t& operator*=(t &lv, const var &rv) {
	__mul_eq_var(t)
}
template<> int& operator*=(int &lv, const var &rv) {
	__mul_eq_var(int)
}
template<> long& operator*=(long &lv, const var &rv) {
	__mul_eq_var(long)
}
template<> float& operator*=(float &lv, const var &rv) {
	__mul_eq_var(float)
}
template<> double& operator*=(double &lv, const var &rv) {
	__mul_eq_var(double)
}
template<> llong& operator*=(llong &lv, const var &rv) {
	__mul_eq_var(llong)
}
template<> ullong& operator*=(ullong &lv, const var &rv) {
	__mul_eq_var(ullong)
}

#define __div_eq_var(tp) \
    try { \
	return lv /= (tp) (rv); \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, #tp"& operator/=("#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return lv; \

template<typename t> t& operator/=(t &lv, const var &rv) {
	__div_eq_var(t)
}
template<> int& operator/=(int &lv, const var &rv) {
	__div_eq_var(int)
}
template<> long& operator/=(long &lv, const var &rv) {
	__div_eq_var(long)
}
template<> float& operator/=(float &lv, const var &rv) {
	__div_eq_var(float)
}
template<> double& operator/=(double &lv, const var &rv) {
	__div_eq_var(double)
}
template<> llong& operator/=(llong &lv, const var &rv) {
	__div_eq_var(llong)
}
template<> ullong& operator/=(ullong &lv, const var &rv) {
	__div_eq_var(ullong)
}

//-------------------------- GLOBAL OP: < > <= >= !=

#define __ab_var(tp) \
    try { \
	return lv > (tp) (rv); \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, "bool operator>(const "#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return 0; \

template<typename t> bool operator>(const t &lv, const var &rv) {
	__ab_var(t)
}
template<> bool operator>(const int &lv, const var &rv) {
	__ab_var(int)
}
template<> bool operator>(const long &lv, const var &rv) {
	__ab_var(long)
}
template<> bool operator>(const float &lv, const var &rv) {
	__ab_var(float)
}
template<> bool operator>(const double &lv, const var &rv) {
	__ab_var(double)
}
template<> bool operator>(const llong &lv, const var &rv) {
	__ab_var(llong)
}
template<> bool operator>(const ullong &lv, const var &rv) {
	__ab_var(ullong)
}
template<> bool operator>(const bool &lv, const var &rv) {
	__ab_var(bool)
}
bool operator>(cch *lv, const var &rv) {
	try {
		return var(lv) > rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator>(cch *lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}
bool operator>(char lv, const var &rv) {
	try {
		return var(lv) > rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator>(char lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}
bool operator>(const str &lv, const var &rv) {
	try {
		return var(lv) > rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator>(const str &lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}

#define __bl_var(tp) \
    try { \
	return lv < (tp) (rv); \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, "bool operator<(const "#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return 0; \

template<typename t> bool operator<(const t &lv, const var &rv) {
	__bl_var(t)
}
template<> bool operator<(const int &lv, const var &rv) {
	__bl_var(int)
}
template<> bool operator<(const long &lv, const var &rv) {
	__bl_var(long)
}
template<> bool operator<(const float &lv, const var &rv) {
	__bl_var(float)
}
template<> bool operator<(const double &lv, const var &rv) {
	__bl_var(double)
}
template<> bool operator<(const llong &lv, const var &rv) {
	__bl_var(llong)
}
template<> bool operator<(const ullong &lv, const var &rv) {
	__bl_var(ullong)
}
template<> bool operator<(const bool &lv, const var &rv) {
	__bl_var(bool)
}
bool operator<(cch *lv, const var &rv) {
	try {
		return var(lv) < rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator<(cch *lv, const var &rv) [GLB]", &rv);
	}
	return 0;
}
bool operator<(char lv, const var &rv) {
	try {
		return var(lv) < rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator<(char lv, const var &rv) [GLB]", &rv);
	}
	return 0;
}
bool operator<(const str &lv, const var &rv) {
	try {
		return var(lv) < rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator<(const str &lv, const var &rv) [GLB]", &rv);
	}
	return 0;
}

#define __bl_eq_var(tp) \
    try { \
	return lv <= (tp) (rv); \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, "bool operator<=(const "#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return 0; \

template<typename t> bool operator<=(const t &lv, const var &rv) {
	__bl_eq_var(t)
}
template<> bool operator<=(const int &lv, const var &rv) {
	__bl_eq_var(int)
}
template<> bool operator<=(const long &lv, const var &rv) {
	__bl_eq_var(long)
}
template<> bool operator<=(const float &lv, const var &rv) {
	__bl_eq_var(float)
}
template<> bool operator<=(const double &lv, const var &rv) {
	__bl_eq_var(double)
}
template<> bool operator<=(const llong &lv, const var &rv) {
	__bl_eq_var(llong)
}
template<> bool operator<=(const ullong &lv, const var &rv) {
	__bl_eq_var(ullong)
}
template<> bool operator<=(const bool &lv, const var &rv) {
	__bl_eq_var(bool)
}
bool operator<=(cch *lv, const var &rv) {
	try {
		return var(lv) <= rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator<=(cch *lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}
bool operator<=(char lv, const var &rv) {
	try {
		return var(lv) <= rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator<=(char lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}
bool operator<=(const str &lv, const var &rv) {
	try {
		return var(lv) <= rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator<=(const str &lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}

#define __ab_eq_var(tp) \
    try { \
	return lv >= (tp) (rv); \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, "bool operator>=(const "#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return 0; \

template<typename t> bool operator>=(const t &lv, const var &rv) {
	__ab_eq_var(t)
}
template<> bool operator>=(const int &lv, const var &rv) {
	__ab_eq_var(int)
}
template<> bool operator>=(const long &lv, const var &rv) {
	__ab_eq_var(long)
}
template<> bool operator>=(const float &lv, const var &rv) {
	__ab_eq_var(float)
}
template<> bool operator>=(const double &lv, const var &rv) {
	__ab_eq_var(double)
}
template<> bool operator>=(const llong &lv, const var &rv) {
	__ab_eq_var(llong)
}
template<> bool operator>=(const ullong &lv, const var &rv) {
	__ab_eq_var(ullong)
}
template<> bool operator>=(const bool &lv, const var &rv) {
	__ab_eq_var(bool)
}
bool operator>=(cch *lv, const var &rv) {
	try {
		return var(lv) >= rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator>=(cch *lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}
bool operator>=(char lv, const var &rv) {
	try {
		return var(lv) >= rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator>=(char lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}
bool operator>=(const str &lv, const var &rv) {
	try {
		return var(lv) >= rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator>=(const str &lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}

#define __eq_var(tp) \
    try { \
	return lv == (tp) (rv); \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, "bool operator==(const "#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return 0; \

template<typename t> bool operator==(const t &lv, const var &rv) {
	__eq_var(t)
}
template<> bool operator==(const int &lv, const var &rv) {
	__eq_var(int)
}
template<> bool operator==(const long &lv, const var &rv) {
	__eq_var(long)
}
template<> bool operator==(const float &lv, const var &rv) {
	__eq_var(float)
}
template<> bool operator==(const double &lv, const var &rv) {
	__eq_var(double)
}
template<> bool operator==(const llong &lv, const var &rv) {
	__eq_var(llong)
}
template<> bool operator==(const ullong &lv, const var &rv) {
	__eq_var(ullong)
}
template<> bool operator==(const bool &lv, const var &rv) {
	__eq_var(bool)
}
bool operator==(cch *lv, const var &rv) {
	try {
		return var(lv) == rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator==(cch *lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}
bool operator==(char lv, const var &rv) {
	try {
		return var(lv) == rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator==(char lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}
bool operator==(const str &lv, const var &rv) {
	try {
		return var(lv) == rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator==(const str &lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}

#define __not_eq_var(tp) \
    try { \
	return lv != (tp) (rv); \
    } catch (VarExcp &ex) { \
	GL_EX_solver(ex, "bool operator!=(const "#tp \
				" &lv, const var &rv) [GLB]", &rv); \
    } \
    return 0; \

template<typename t> bool operator!=(const t &lv, const var &rv) {
	__not_eq_var(t)
}
template<> bool operator!=(const int &lv, const var &rv) {
	__not_eq_var(int)
}
template<> bool operator!=(const long &lv, const var &rv) {
	__not_eq_var(long)
}
template<> bool operator!=(const float &lv, const var &rv) {
	__not_eq_var(float)
}
template<> bool operator!=(const double &lv, const var &rv) {
	__not_eq_var(double)
}
template<> bool operator!=(const llong &lv, const var &rv) {
	__not_eq_var(llong)
}
template<> bool operator!=(const ullong &lv, const var &rv) {
	__not_eq_var(ullong)
}
template<> bool operator!=(const bool &lv, const var &rv) {
	__not_eq_var(bool)
}
bool operator!=(cch *lv, const var &rv) {
	try {
		return var(lv) != rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator!=(cch *lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}
bool operator!=(char lv, const var &rv) {
	try {
		return var(lv) != rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator!=(char lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}
bool operator!=(const str &lv, const var &rv) {
	try {
		return var(lv) != rv;
	} catch (VarExcp &ex) {
		GL_EX_solver(ex, "bool operator!=(const str &lv,"
				" const var &rv) [GLB]", &rv);
	}
	return 0;
}

//---------------------- GLOBAL op: <<

std::ostream& operator <<(std::ostream &os, const var &rv) {
	if (rv.pttyp) {
		if (rv.tid[0] == 'i') return os << int(rv);
		else if (rv.tid[0] == 'd') return os << double(rv);
		else if (rv.tid[0] == 'b') return os << (str) rv;
		else if (rv.tid[0] == 'c') return os << char(rv);
		else if (rv.tid[0] == 's') {
			str *ps = rv;
			return os << *ps;
		} else if (rv.tid[0] == 'a') return os << str(rv);
		else if (rv.tid[0] == 'm') return os << str(rv);
		else if (rv.tid[0] == 'I') return os << *rv;
	}
	return os << "|udf|";
}
std::istream& operator >>(std::istream &is, const var &rv) {
	var &_rv = const_cast<var&>(rv);
	_rv = "";
	auto p = dynamic_cast<ttyp<str>*>(_rv.pttyp);
	return is >> (p->val);
}

//-------------------------------------------------------------- hlp
//------------------------------------------------------------------

template<typename T> void tcpps(T x, char *buf) {
}
template<> void tcpps<>(int x, char *buf) {
	std::sprintf(buf, "%i", x);
}
template<> void tcpps<>(float x, char *buf) {
	std::sprintf(buf, "%g", x);
}
template<> void tcpps<>(double x, char *buf) {
	std::sprintf(buf, "%f", x);
}
template<> void tcpps<>(long x, char *buf) {
	std::sprintf(buf, "%li", x);
}
template<> void tcpps<>(llong x, char *buf) {
	std::sprintf(buf, "%lli", x);
}
template<> void tcpps<>(ullong x, char *buf) {
	std::sprintf(buf, "%llu", x);
}

