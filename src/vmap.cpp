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

/**
 *  here placed supporting classes for var:map ( var:stg in perspective )
 *  
 *  ! internal use only !
 *  
 *  changed: 10.13.2023
 * 
 */

typedef const char cch;
typedef unsigned long long ull;

/**
 *  Ostg -> objects storage   
 * 
 */

#define _Ostg_dISZ 1000 // objects segment init size

template<typename t, ull isz = _Ostg_dISZ> class Ostg {
	
	typedef void (*fun)(t &x, void *ctx);

public:

	struct seg;

	class idx {
		friend Ostg;
	public:
		seg *bs = 0;
		idx *prv = 0;
		idx *nxt = 0;
		t val;
		operator t*() {
			void *x = (void*) (this);
			if (x != 0) return &val;
			return 0;
		}
	};

	struct seg {
		idx _idx[isz];
		ull _act = 0;
		ull _free = 0;
		seg *prv = 0;
		seg *nxt = 0;
		seg(seg *_prv) {
			prv = _prv;
		}
		~seg() {
			delete nxt;
		}
	};

	ull _act = 0;
	ull _free = 0;
	seg *fst = 0;
	seg *lst = 0;

	idx *Af = 0;
	idx *Al = 0;
	idx *Ff = 0;
	idx *Fl = 0;

	fun get_fun = 0; // 'init function'
	void *get_ctx = 0;
	fun rem_fun = 0; // 'deinit function'
	void *rem_ctx = 0;
	bool SMF = 0;    // 'skip memory free' flag
	
	class _itr_A {
		int op = 0;
		Ostg *bs = 0;
		idx *pidx = 0;
	public:
		_itr_A(Ostg *_bs) {
			bs = _bs;
		}
		void init() {
			if (bs->Af) {
				pidx = bs->Af, op = 0;
			} else op = 2;
		}
		idx* nxt() {
			if (op == 1 && pidx->nxt) {
				pidx = pidx->nxt;
				return pidx;
			} else if (op == 0) { // fst
				op = 1;
				return pidx;
			} else if (op != 2) { // lst
				op = 2;
				return 0;
			}
			return 0;
		}
	};

	class _itr_F {
		int op = 0;
		Ostg *bs = 0;
		idx *pidx = 0;
	public:
		_itr_F(Ostg *_bs) {
			bs = _bs;
		}
		void init() {
			if (bs->Ff) {
				pidx = bs->Ff, op = 0;
			} else op = 2;
		}
		idx* nxt() {
			if (op == 1 && pidx->nxt) {
				pidx = pidx->nxt;
				return pidx;
			} else if (op == 0) { // fst
				op = 1;
				return pidx;
			} else if (op != 2) { // lst
				op = 2;
				return 0;
			}
			return 0;
		}
	};

	void unlink_F(seg *s) {
		
		ull i = 0;
		idx *_idx;
		
		while (i < isz) {
			
			if (s->_free) {
				
				_idx = &s->_idx[i];
				
				if (_idx->prv && _idx->nxt) { // mid
					_idx->prv->nxt = _idx->nxt;
					_idx->nxt->prv = _idx->prv;
				} else if (!_idx->prv && _idx->nxt) { // fst
					Ff = _idx->nxt, Ff->prv = 0;
				} else if (!_idx->nxt && _idx->prv) { // lst
					Fl = _idx->prv, Fl->nxt = 0;
				} else { // fst=lst
				}
				--s->_free, --_free;
				_idx->prv = _idx->nxt = 0, _idx->bs = 0;
				
			} else break;
			++i;
		}
		if (_free == 0) Ff = Fl = 0;
	}
	
	void free_seg(seg *s) {
		
		if (!SMF && s->_act == 0) {
			// skip fist [decision]
			if (s == fst) return;
			
			unlink_F(s);
			
			if (s->prv && s->nxt) { // mid
				s->prv->nxt = s->nxt;
				s->nxt->prv = s->prv;
			} else if (!s->prv && s->nxt) { // fst
				fst = s->nxt, fst->prv = 0;
			} else if (!s->nxt && s->prv) { // lst
				lst = s->prv, lst->nxt = 0;
			} else { // fst=lst
				fst = lst = 0;
			}
			s->nxt = 0;
			
			delete s;
		}
	}
	
public:

	Ostg(bool smf = 0) :
			SMF(smf), itr_A(this), itr_F(this) {
	}
	
	~Ostg() {
		delete fst;
	}
	
	/** -> Active index iterator <-
	 */
	_itr_A itr_A;

	/** -> Free index iterator <-
	 */
	_itr_F itr_F;

	/** -> set get function (constr) <-
	 */
	void set_get_fun(fun f, void *ctx = 0) {
		get_fun = f;
		get_ctx = ctx;
	}
	
	/** -> set remove function (destr) <-
	 */
	void set_rem_fun(fun f, void *ctx = 0) {
		rem_fun = f;
		get_ctx = ctx;
	}
	
	/** -> get index <-
	 * 
	 * 'mode' 0-Ff 1-Fl  (get-from)
	 * return index-pointer
	 */
	idx* get(bool mode = 0) {
		
		if (fst == 0) {
			fst = lst = new seg(0);
		}
		seg *cur = fst;
		idx *_idx = 0;
		
		if (Ff) {
			if (!mode) {
				_idx = Ff;
				// unlink F
				if (Ff->nxt) {
					Ff = Ff->nxt, Ff->prv = 0;
				} else { // fst=lst
					Ff = Fl = 0;
				}
			} else {
				_idx = Fl;
				// unlink F
				if (Fl->prv) {
					Fl = Fl->prv, Fl->nxt = 0;
				} else { // fst=lst
					Ff = Fl = 0;
				}
			}
			cur = _idx->bs;
			--cur->_free, --_free;
			
		} else {
			cur = lst;
			// new-seg
			if (cur->_act == isz) {
				cur->nxt = new seg(cur);
				lst = cur = cur->nxt;
			}
			_idx = &cur->_idx[cur->_act];
		}
		// set
		_idx->prv = _idx->nxt = 0;
		_idx->bs = cur;
		++cur->_act, ++_act;
		// link
		if (Af) {
			Al->nxt = _idx;
			_idx->prv = Al;
			Al = _idx;
		} else { // fist
			Af = Al = _idx;
		}
		
		if (get_fun) get_fun(_idx->val, get_ctx);
		
		return _idx;
	}
	
	/** -> remove index <- 
	 * 
	 * return arr-length , 0 -> empty
	 */
	int rem(idx *_idx) {
		//unlink A
		if (_idx->prv && _idx->nxt) { // mid
			_idx->prv->nxt = _idx->nxt;
			_idx->nxt->prv = _idx->prv;
		} else if (!_idx->prv && _idx->nxt) { // fst
			Af = _idx->nxt, Af->prv = 0;
		} else if (!_idx->nxt && _idx->prv) { // lst
			Al = _idx->prv, Al->nxt = 0;
		} else { // fst=lst
			Af = Al = 0;
		}
		// set
		seg *cur = _idx->bs;
		_idx->prv = _idx->nxt = 0;
		--cur->_act, --_act, ++cur->_free, ++_free;
		// link F
		if (Ff) {
			Fl->nxt = _idx;
			_idx->prv = Fl;
			Fl = _idx;
		} else { // fst
			Ff = Fl = _idx;
		}
		
		if (rem_fun) rem_fun(_idx->val, rem_ctx);
		
		// free-seg-prc [!order!]
		if (!SMF && cur->_act == 0) free_seg(cur);
		
		return 1;
	}
	
	/** -> free storage <-
	 *
	 * 'sfs' (skip-fist-segment-free) def=0 
	 * return void
	 */
	void free(bool sfsf = 0) {
		if (!sfsf) {
			delete fst;
			fst = lst = 0;
		} else {
			if (fst) {
				if (fst->nxt) {
					delete fst->nxt;
				}
				fst->_act = fst->_free = 0;
				fst->nxt = fst->prv = 0;
				lst = fst;
			}
		}
		Af = Al = Ff = Fl = 0, _act = _free = 0;
	}
	
	/** -> get Active-chain size <-
	 * 
	 * return size , 0 -> empty
	 */
	ull sizeA() const {
		return _act;
	}
	
	/** -> get Free-chain size <-
	 * 
	 * return size , 0 -> empty
	 */
	ull sizeF() {
		return _free;
	}
};

/**
 * 
 *  MAP_da -> MAP dynamic array    
 * 
 */

#ifdef _MAP_da_DBG

ull Mda_I = 0, Mda_MI = 0;

#define __MAP_da_seg_Inc ++MAP_dacd.da_seg  
#define __MAP_da_da_Inc	 ++MAP_dacd.da_da
#define __MAP_da_seg_Dnc --MAP_dacd.da_seg
#define __MAP_da_da_Dnc	 --MAP_dacd.da_da

struct MAP_da_cd {
	int da_seg = 0;
	int da_da = 0;
};
MAP_da_cd MAP_dacd;

#else   
#define __MAP_da_seg_Inc
#define __MAP_da_da_Inc
#define __MAP_da_seg_Dnc
#define __MAP_da_da_Dnc
#endif

_VMAP* var_map_get_val_ref(var *_x);

template<typename t, ull isz> class MAP_da;

template<typename t, ull isz> struct MAP_da_idx {
	t val;
};

template<typename t, ull isz> struct MAP_da_seg {
	
	ull l = 0;
	MAP_da_idx<t, isz> _li[isz];
	ull ss = 0;    //segment start
	MAP_da_seg *prv = 0;
	MAP_da_seg *nxt = 0;
	// seg-init
	explicit MAP_da_seg(MAP_da<t, isz> *_bs, MAP_da_seg<t, isz> *_prv, int m = 0) {
		// m=1 -> spc middle-new-seg
		prv = _prv;
		prv->nxt = this;
		if (!m) { //add to end case
			_bs->lst = this;
			ss = prv->l + prv->ss;
		}__MAP_da_seg_Inc;
	}
	// first-seg-init
	explicit MAP_da_seg(MAP_da<t, isz> *_bs) {
		_bs->lst = this;
		_bs->fst = this;
		__MAP_da_seg_Inc;
	}
	
	~MAP_da_seg() {
		delete nxt;
		__MAP_da_seg_Dnc;
	}
	
};

template<typename t, ull isz> struct MAP_da {
	
	ull L = 0;
	MAP_da_seg<t, isz> *fst = 0;
	MAP_da_seg<t, isz> *lst = 0;

	// --------------------- hlp:
	
	inline MAP_da_seg<t, isz>* gseg(ull i, MAP_da_seg<t, isz> *cur = 0) {
		
		if (!cur) cur = fst;
		if (cur) {
			while (cur->nxt != 0) {
				if (i >= cur->ss && i < cur->ss + cur->l) {
					break;
				}
#ifdef MAP_da_dbg
				++Mda_I;
#endif
				cur = cur->nxt;
			}
		}
		return cur;
	}
	inline MAP_da_seg<t, isz>* gseg_sp(MAP_da_idx<t, isz> *pidx, ull *t_idx) {
		
		MAP_da_seg<t, isz> *cur = fst;
		
		while (cur) {
			if (pidx >= &cur->_li[0] && pidx <= &cur->_li[cur->l - 1]) {
				ull i = 0;
				while (i < cur->l) {
					if (pidx == &cur->_li[i]) {
						*t_idx = i;
						return cur;
					}
#ifdef MAP_da_dbg
					++Mda_I;
#endif
					++i;
				}
			}
#ifdef MAP_da_dbg
			++Mda_I;
#endif
			cur = cur->nxt;
		}
		return 0;
	}
	inline ull dsp(MAP_da_seg<t, isz> *cur, ull cnt, ull beg = 0, bool flg = 0) {
		// one-seg-displace
		ull _cnt = cur->l - beg;
		cur->l += cnt;
		if (!flg) L += cnt; //*
		ull dst = cur->l - 1, i = 0;
		
		while (i < _cnt) {
			cur->_li[dst] = cur->_li[dst - cnt];
#ifdef MAP_da_dbg
			++Mda_I;
#endif
			--dst, ++i;
		}
		return dst;
	}
	inline ull pcp(MAP_da_seg<t, isz> *dst, MAP_da_seg<t, isz> *src, ull dst_st,    //
			ull src_st, ull src_cnt = -1) {
		
		ull i1 = src_st, i2 = dst_st, tot = 0, //
				l = (src_cnt == (ull) -1) ? src->l : src_st + src_cnt;
		
		while (i1 < l) {
			dst->_li[i2] = src->_li[i1];
			++i1, ++i2, ++tot;
			if (i2 > dst->l) //
			dst->l = i2; //expand
#ifdef MAP_da_dbg
			++Mda_I;
#endif
		}
		return tot;
	}
	inline ull ucp(MAP_da_seg<t, isz> *dst, ull dst_st, MAP_da_seg<t, isz> *src, //
			ull src_st, ull cnt) {
		
		ull i = 0, i1 = dst_st, i2 = src_st;
		
		while (i < cnt) {
			// ---------------
			dst->_li[i1] = src->_li[i2];
			// ---------------
			if (i1 == 0) {
				dst = dst->prv;
				L += isz - dst->l;
				dst->l = isz, i1 = isz;
			}
			if (i2 == 0) {
				if (src->prv) {
					src = src->prv;
					i2 = src->l;
				}
			}
			
			++i, --i1, --i2;
#ifdef MAP_da_dbg
			++Mda_I;
#endif
		}
		
		return i;
	}
	inline void mnxt(MAP_da_seg<t, isz> *cur) {
		
		if (cur->prv) cur->ss = cur->prv->ss + cur->prv->l;
		cur = cur->nxt;
		while (cur) {
			cur->ss = cur->prv->ss + cur->prv->l;
			cur = cur->nxt;
#ifdef MAP_da_dbg
			++Mda_I;
#endif
		}
	}
	MAP_da_idx<t, isz>* _ins(ull beg, const MAP_da<t, isz> *src, ull sbeg, //
			t ival, ull icnt, bool f = 0, MAP_da_seg<t, isz> *pseg = 0) {
		
		MAP_da_seg<t, isz> *cur = 0;
		ull x, y = 0, _beg;
		
		if (fst == 0) { // empty-ins
				
			cur = new MAP_da_seg<t, isz>(this);
			
			_beg = beg;
		} else {
			if (!pseg) {
				cur = gseg(beg);
				_beg = beg - cur->ss;
			} else {
				cur = pseg;
				_beg = beg;
			}
		}
	
		x = isz - cur->l;
		if (cur->nxt) {
			y = isz - cur->nxt->l;
			if (y > x) y -= x;
		}
		
		if (icnt <= x) { // 1.
			
			dsp(cur, icnt, _beg);
			
		} else if (y > 0 && icnt <= y) { // 2.
		
			ull dst_st = dsp(cur->nxt, icnt - x);
			ucp(cur->nxt, dst_st, cur, cur->l - 1, cur->l - _beg);
			
		} else { // 3.
			
			MAP_da_seg<t, isz> *_new = cur, *till = cur->nxt;
			ull _cnt = x, till_free = 0;
			
			if (till) till_free = isz - till->l;
			
			while (true) {
				
				_new = new MAP_da_seg<t, isz>(this, _new, 1);
				
				_new->nxt = till;
				if (till) till->prv = _new;
				else lst = _new;
				_cnt += isz;
				if (_cnt >= icnt) {
					till = _new;
					_new->l = isz - (_cnt - icnt);
					L += _new->l;
					
					if (_beg == isz) {
						cur = _new, _beg = 0;
					} else {
						ucp(_new, _new->l - 1, cur, cur->l - 1, cur->l - _beg);
					}
		
					break;
				} else if (till && icnt - _cnt < till_free) {
					
					ull dst_st = dsp(till, icnt - _cnt);
					ucp(till, dst_st, cur, cur->l - 1, cur->l - _beg);
					
					break;
				}
#ifdef MAP_da_dbg
				++Mda_I;
#endif
			}
		}
		
		cur->_li[_beg].val = ival;
		mnxt(cur);
		
		return &cur->_li[_beg];
	}
	inline MAP_da_seg<t, isz>* _U_(MAP_da_seg<t, isz> *cur, ull _beg, ull _end, ull cnt) {
		
		ull x = 0, req = cur->l - cnt;
		// prv-unite-case
		if (cur->prv) {
			MAP_da_seg<t, isz> *prv = cur->prv;
			x = isz - prv->l;
			if (req <= x) {
				// fst-part
				pcp(prv, cur, prv->l, 0, _beg);
				// sec-part
				pcp(prv, cur, prv->l, _end, -1);
				// linki
				if (cur->nxt) {
					prv->nxt = cur->nxt;
					cur->nxt->prv = cur->prv;
				} else {
					prv->nxt = 0; // last case
					lst = prv;
				}
				cur->nxt = 0;
				delete cur;
				L -= cnt;
				
				return prv;
			}
		}
		// nxt-unite-case    |-> !fst
		if (cur->nxt && cur->prv != 0) {
			MAP_da_seg<t, isz> *nxt = cur->nxt;
			x = isz - nxt->l;
			if (req <= x) {
				// nxt-shift-down
				dsp(nxt, req, 0, 1);	      //*
				// fst-part
				x = 0;
				x += pcp(nxt, cur, x, 0, _beg);
				// sec-part
				pcp(nxt, cur, x, _end, -1);
				// linki
				cur->prv->nxt = cur->nxt;
				nxt->prv = cur->prv;
				cur->nxt = 0;
				delete cur;
				L -= cnt;
				//
				return nxt;
			}
		}
		// same-seg-case
		pcp(cur, cur, _beg, _end);
		
		cur->l -= cnt, L -= cnt;
		
		return cur;
	}
	ull _rem(ull beg, ull cnt = 0, MAP_da_seg<t, isz> *pseg = 0) {
		
		cnt == 0 ? cnt = 1 : 0;
		ull end = beg + cnt, _beg, _end;
		
		if (beg >= 0 && end <= L) {
			
			MAP_da_seg<t, isz> *cur;
			if (pseg) { // direct-seg-case
				cur = pseg;
				_beg = beg;
				_end = end;
			} else {
				cur = gseg(beg);
				_beg = beg - cur->ss;
				_end = end - cur->ss;
			}
			// one seg
			if (_end <= cur->l) {
				
				cur = _U_(cur, _beg, _end, cnt);
				
				if (cur->l == 0) {
					if (L == 0) {
						delete fst;
						fst = lst = 0;
					} else {
						MAP_da_seg<t, isz> *_cur = cur->nxt;
						cur->nxt = 0;
						delete cur;
						_cur->prv = 0, _cur->ss = 0, fst = _cur;
						mnxt(_cur);
					}
				}
				
				else mnxt(cur);
				
			} else {
				// move to end-seg and rem mid-seg
				MAP_da_seg<t, isz> *_cur = 0, *_st = cur;
				ull z = 0;
				// spc-case-check
				bool spc = _beg == 0 && _st->prv;
				if (spc) {
					_st = _st->prv;
				} else {
					cur = cur->nxt;
				}
				
				while (cur->nxt != 0) {
					if (end >= cur->ss && end < cur->ss + cur->l) break;
					else {
						++z;
						_cur = cur;
						cur = cur->nxt;
						cnt -= _cur->l, L -= _cur->l;
						_cur->nxt = 0;
						delete _cur;
					}
#ifdef MAP_da_dbg
					++Mda_I;
#endif
				}
				if (z > 0) {
					_st->nxt = cur;
					cur->prv = _st;
				}
				// _st:
				if (!spc) { // !spc-case
					cnt -= _st->l - _beg;
					L -= _st->l - _beg;
					_st->l = _beg;
				}
				// cur:
				_beg = 0;
				_end = _beg + cnt;
				
				cur = _U_(cur, _beg, _end, cnt);
				
				mnxt(cur);
			}
		}
		return L;
	}
	ull _dfrg() {
		MAP_da_seg<t, isz> *cur = fst, *nxt;
		ull fspc, tot = 0;
		
		while (cur) {
			
			nxt = cur->nxt;
			fspc = isz - cur->l;
			
			if (fspc && nxt) {
				if (nxt->l <= fspc) {
					tot += pcp(cur, nxt, cur->l, 0, nxt->l);
					if (nxt->nxt) {
						nxt->nxt->prv = cur, cur->nxt = nxt->nxt;
						nxt->nxt = 0;
					} else {
						cur->nxt = 0, lst = cur;
					}
					delete nxt;
					continue;
					
				} else {
					tot += pcp(cur, nxt, cur->l, 0, fspc);
					nxt->l -= fspc;
					tot += pcp(nxt, nxt, 0, fspc, nxt->l);
				}
			}
			cur = cur->nxt;
#ifdef MAP_da_dbg
			++Mda_I;
#endif
		}
		
		if (tot) mnxt(fst);
		
		return tot;
	}
	
	MAP_da() {
		__MAP_da_da_Inc;
	}
	~MAP_da() {
		delete fst;
		__MAP_da_da_Dnc;
	}
	
	// ---------------------  if:
	
	MAP_da_idx<t, isz>* ins(ull idx, const t ival) {
#ifdef MAP_da_dbg
		Mda_I = 0;
		MAP_da_idx<t, isz> *v = _ins(idx, 0, 0, ival, 1);
		if (Mda_I > Mda_MI) Mda_MI = Mda_I;
		return v;
#else
		return _ins(idx, 0, 0, ival, 1);
#endif
		
	}
	MAP_da_idx<t, isz>* ins_sp(ull t_idx, MAP_da_seg<t, isz> *pseg, const t ival) {
#ifdef MAP_da_dbg
		Mda_I = 0;
		MAP_da_idx<t, isz> *v = _ins(t_idx, 0, 0, ival, 1, 0, pseg);
		if (Mda_I > Mda_MI) Mda_MI = Mda_I;
		return v;
#else
		return _ins(t_idx, 0, 0, ival, 1, 0, pseg);
#endif
	}
	
	ull rem_sp(ull t_idx, MAP_da_seg<t, isz> *pseg) {
#ifdef MAP_da_dbg
		Mda_I = 0;
		ull v = _rem(t_idx, 1, pseg);
		if (Mda_I > Mda_MI) Mda_MI = Mda_I;
		return v;
#else
		return _rem(t_idx, 1, pseg);
#endif
	}
	ull rem_sp(MAP_da_idx<t, isz> *pidx) {
#ifdef MAP_da_dbg
		Mda_I = 0;
		ull t_idx;
		MAP_da_seg<t, isz> *pseg = gseg_sp(pidx, &t_idx);
		ull v = _rem(t_idx, 1, pseg);
		if (Mda_I > Mda_MI) Mda_MI = Mda_I;
		return v;
#else
		ull t_idx = 0;
		MAP_da_seg<t, isz> *pseg = gseg_sp(pidx, &t_idx);
		return _rem(t_idx, 1, pseg);
#endif
	}
	
	ull rem(ull idx) {
#ifdef MAP_da_dbg
		Mda_I = 0;
		ull v = _rem(idx, 1);
		if (Mda_I > Mda_MI) Mda_MI = Mda_I;
		return v;
#else
		return _rem(idx, 1);
#endif
	}
	
	ull dfrg() {
#ifdef MAP_da_dbg
		Mda_I = 0;
		ull v = _dfrg();
		if (Mda_I > Mda_MI) Mda_MI = Mda_I;
		return v;
#else
		return _dfrg();
#endif	
	}
	
	MAP_da_idx<t, isz>* push(t val) {
		
		MAP_da_seg<t, isz> *cur = lst;
		
		if (cur) {
			
			if (cur->l == isz) { // new-seg
				cur = new MAP_da_seg<t, isz>(this, cur);
			}
		} else { // fst-seg
			cur = new MAP_da_seg<t, isz>(this);
		}
		
		cur->_li[cur->l].val = val;
		++cur->l, ++L;
		
		return &cur->_li[cur->l - 1];
	}
	
	t pop() {
		if (L > 0) {
			--lst->l, --L;
			
			t x = (lst->_li[lst->l].val);
			
			if (lst->l == 0) { // try-free-seg
				if (lst->prv) {
					lst = lst->prv;
					delete lst->nxt;
					lst->nxt = 0;
				} else { // seg0
					delete fst;
					fst = lst = 0;
				}
			}
			return x;
		}
		return 0;
	}
	
	void free() {
		delete fst;
		fst = 0, lst = 0;
		L = 0;
	}
	
};

class MAPExcp: public std::exception {
	
	cch *exc = 0;

public:

	MAPExcp(cch *msg) {
		exc = msg;
	}
	~MAPExcp() {
	}
	
	const char* what() const noexcept {
		return exc;
	}
};

/**
 *
 * MAP 
 * 
 */

template<typename t, int sli_isz, int sstg_isz, int rstg_isz> struct MAP_seg {
	
	char sid = 0;                                                 // segment-id
	typename Ostg<t, rstg_isz>::idx *rec = 0;                     // record
	MAP_da<typename Ostg<MAP_seg<t, sli_isz, sstg_isz, rstg_isz>, sstg_isz>::idx*, //
			sli_isz> li;                                          // segments-list
	
	void rst() {
		sid = 0, rec = 0;
		li.free();
	}
};

/**
 * MAP 
 * 
 * t           work-type
 * id_sz       id-size 
 * sli_isz     segment child list init-size
 * sstg_isz    segment storage init-size
 * rstg_isz    record  storage init-size
 * _vm_deep    var-map-deep
 * 
 */

template<typename t, int id_isz, int sli_isz, int sstg_isz, int rstg_isz, int _vm_deep> //
class MAP {
	
	friend _VMAP* var_map_get_val_ref(var *_x);

	typedef typename Ostg<MAP_seg<t, sli_isz, sstg_isz, rstg_isz>, sstg_isz>::idx _idx;

	_idx *seg0 = 0;
	Ostg<t, rstg_isz> rec_li;
	Ostg<MAP_seg<t, sli_isz, sstg_isz, rstg_isz>, sstg_isz> seg_li;
	MAP_da_idx<_idx*, sli_isz> *stck[id_isz + 1];

	// ------------------ hlp:  
	
	inline MAP_da_idx<_idx*, sli_isz>* //
	get_tree_seg(cch *id, MAP_da_idx<_idx*, sli_isz> *stck[] = 0, int *stck_i = 0) {
		
		MAP_da_idx<_idx*, sli_isz> *idx = 0;
		MAP_da_seg<_idx*, sli_isz> *nxt = seg0->val.li.fst, //
				*prv = seg0->val.li.lst, *cur = 0;
		ull i;
		char sid;
		
		while ((sid = *id) != 0) {
			lp:
			if (nxt) {
				if (sid < nxt->_li[0].val->val.sid || //
						sid > prv->_li[prv->l - 1].val->val.sid) //
				goto out;
				
				if (sid >= nxt->_li[0].val->val.sid && //
						sid <= nxt->_li[nxt->l - 1].val->val.sid) {
					cur = nxt;
					goto ok;
				} else if (nxt == prv) goto out;
				else if (sid >= prv->_li[0].val->val.sid && //
						sid <= prv->_li[prv->l - 1].val->val.sid) {
					cur = prv;
					goto ok;
				} else {
					nxt = nxt->nxt;
					if (nxt != prv) {
						prv = prv->prv;
						goto lp;
					} else goto out;
				}
				ok:
				i = 0;
				while (i < cur->l) {
					if (sid == cur->_li[i].val->val.sid) {
						idx = &cur->_li[i];
						if (stck) {
							stck[*stck_i] = idx;
							++(*stck_i);
						}
						nxt = cur->_li[i].val->val.li.fst;
						prv = cur->_li[i].val->val.li.lst;
						goto lb;
					}
					++i;
				}
				goto out;
				lb:
				++id;
			} else goto out;
		}
		// ok
		return idx;
		// not-match|empty
		out:
		return 0;
	}
	
	inline _idx* set_tree_seg(cch *id) {
		
		_idx *seg = seg0;
		MAP_da_seg<_idx*, sli_isz> *cur;
		_idx *dsi;
		int cnt = 0;
		
		char sid, _sid, _sid0;
		
		while ((sid = *id) != 0) {
			
			cur = seg->val.li.fst;
			
			if (!cur) {
				dsi = seg_li.get(), dsi->val.sid = sid;
				seg = seg->val.li.push(dsi)->val;
				goto lb;
			} else {
				while (cur) {
					
					ull l = cur->l, _l = l - 1, i;
					_sid0 = cur->_li[0].val->val.sid;
					
					if (sid >= _sid0 && sid <= cur->_li[_l].val->val.sid) {
						i = 0;
						while (i < l) {
							
							_sid = cur->_li[i].val->val.sid;
							
							if (sid == _sid) { // ==
								seg = cur->_li[i].val;
								goto lb;
							} else if (i < _l) { //
								if (sid > _sid && sid < cur->_li[i + 1].val->val.sid) {
									dsi = seg_li.get(), dsi->val.sid = sid;
									seg = seg->val.li.ins_sp(i + 1, cur, dsi)->val;
									goto lb;
								}
							}
							++i;
						}
					} else if (sid < _sid0) {
						dsi = seg_li.get(), dsi->val.sid = sid;
						seg = seg->val.li.ins_sp(0, cur, dsi)->val;
						goto lb;
					}
					cur = cur->nxt;
				}
				// add to end
				dsi = seg_li.get(), dsi->val.sid = sid;
				seg = seg->val.li.push(dsi)->val;
			}
			lb:
			++id, ++cnt;
			if (cnt > id_isz) {
				throw MAPExcp("[MAPExcp][ID-overflow]");
			}
		}
		if (!seg->val.rec) { // new-rec
			seg->val.rec = rec_li.get();
			return seg;
		} else {
			return seg; // reassign-case
		}
		
		return 0;
	}
	
	inline ull rem_tree_seg(cch *id, void (*fun)(t *idx, void *ctx)=0, void *ctx = 0) {
		
		int stck_i;
		stck_i = 0;
		ull tot = 0;
		
		MAP_da_idx<_idx*, sli_isz> *s =	//
				get_tree_seg(id, stck, &stck_i), *prv;
		
		if (s) {
			// ------------------------------ last
			--stck_i;
			s = stck[stck_i];
			// remove rec
			if (s->val->val.rec) {
				if (fun) fun(&s->val->val.rec->val, ctx);
				tot += rec_li.rem(s->val->val.rec);
				s->val->val.rec = 0;
			}
			// tree not ends -> out
			if (s->val->val.li.L > 0) return tot;
			else {
				tot += seg_li.rem(s->val);
			}
			// ------------------------------ back loop
			prv = s;
			
			if (stck_i > 0) {
				--stck_i;
				
				while (stck_i >= 0) {
					
					s = stck[stck_i];
					// --------- rem-idx-from-seg.li
					s->val->val.li.rem_sp(prv); //-----. 
					// --------- rec -> out     //     |
					if (s->val->val.rec != 0)   //     |
					return tot;                 //     |
					// --------- free-empty-seg //     |
					if (s->val->val.li.L == 0)  //-----'
					tot += seg_li.rem(s->val);
					else return tot;
					// ---------
					prv = s;
					--stck_i;
				}
			}
			// --------- rem-idx-from-seg0.li
			seg0->val.li.rem_sp(prv);
			// ---------
		}
		return tot;
	}
	
	void id_ovfl_solver(cch *id) {
		
		char bf[id_isz + 1];
		int i = 0;
		while (i < id_isz) {
			bf[i] = id[i];
			++i;
		}
		bf[id_isz] = 0;
		remId(bf);
	}
	
	// dbg
	void ord_chk(MAP_da_seg<_idx*, sli_isz> *s = 0, ull cnt = 0) {
		
		char x = 0;
		ull i, l;
		MAP_da_seg<_idx*, sli_isz> *cur;
		if (s == 0) {
			cur = seg0->val.li.fst;
		} else cur = s;
		
		if (cur) {
			
			lp:
			l = cur->l, i = 0;
			
			while (i < l) {
				
				if (cur->_li[i].val->val.sid > x) {
					x = cur->_li[i].val->val.sid;
				} else {
					std::cout << "order-err" << std::endl;
					break;
				}
				
				if (cur->_li[i].val->val.li.L > 0) {
					if (cnt < 1)  //
					ord_chk(cur->_li[i].val->val.li.fst, cnt + 1);
				}
				++i;
			}
			
			if (cur->nxt) {
				cur = cur->nxt;
				goto lp;
			}
		}
	}
	ull _prnt(MAP_da_seg<_idx*, sli_isz> *s = 0, ull cnt = 0, ull deep = 0) {
		
		ull i, l, _i, tot = 0;
		MAP_da_seg<_idx*, sli_isz> *cur;
		if (s == 0) {
			cur = seg0->val.li.fst;
			std::cout << "-------------- start" << std::endl;
		} else cur = s;
		
		if (cur) {
			
			lp:
			l = cur->l, i = 0;
			
			while (i < l) {
				if (cnt == 0) std::cout << ">";
				else {
					std::cout << " ";
					_i = 0;
					while (_i < cnt) {
						std::cout << ".";
						++_i;
					}
				}
				
				std::cout << cur->_li[i].val->val.sid;
				if (cur->_li[i].val->val.rec) {
					std::cout << ":R";
					++tot;
				}
				std::cout << std::endl;
				
				if (cur->_li[i].val->val.li.L > 0) {
					if (deep == 0 || cnt + 1 < deep)  //
					tot += _prnt(cur->_li[i].val->val.li.fst, cnt + 1, deep);
				}
				
				++i;
			}
			
			if (cur->nxt) {
				cur = cur->nxt;
				goto lp;
			}
		}
		
		if (cnt == 0) {
			std::cout << "-------------- end" << std::endl;
		}
		
		return tot;
	}
	
public:

	typedef void (*fun)(t &x, void *ctx);
	typedef MAP<t, id_isz, sli_isz, sstg_isz, rstg_isz, _vm_deep> _MAP;

	MAP(fun set_fun = 0, fun rem_fun = 0) {
		// entr-point-init
		seg0 = seg_li.get();
		// get/rem fun-set
		rec_li.set_get_fun(set_fun);
		rec_li.set_rem_fun(rem_fun);
	}
	
	struct _STK_ {
		MAP *cur[_vm_deep];
		int stk = 0;
	};

	MAP(const _MAP &x) {
		
		_STK_ STK;
		STK.cur[STK.stk] = this;
		
		x.foreach([](auto n, auto v, int rec_ud, ull tot, int deep, auto ctx) {
			_STK_ *STK = (_STK_*) ctx;
			var *_v;
			if (rec_ud == 1) {
				_v = STK->cur[STK->stk]->setId(n);
				_v->map_init();
				++STK->stk;
				STK->cur[STK->stk] = var_map_get_val_ref(_v->pttyp);
			} else if (rec_ud == 2) {
				--STK->stk;
			} else {
				_v = STK->cur[STK->stk]->setId(n);
				*_v = *v;
			}
			return var::map::nxt;
		},0, 0, &STK);
		
	}
	
	// op:
	
	const _MAP& operator=(const _MAP &x) {
		
		free();
		
		_STK_ STK;
		STK.cur[STK.stk] = this;
		
		x.foreach([](auto n, auto v, int rec_ud, ull tot, int deep, auto ctx) {
			_STK_ *STK = (_STK_*) ctx;
			var *_v;
			if (rec_ud == 1) {
				_v = STK->cur[STK->stk]->setId(n);
				_v->map_init();
				++STK->stk;
				STK->cur[STK->stk] = var_map_get_val_ref(_v->pttyp);
			} else if (rec_ud == 2) {
				--STK->stk;
			} else {
				_v = STK->cur[STK->stk]->setId(n);
				*_v = *v;
			}
			return var::map::nxt;
		},0, 0, &STK);
		
		return x;
	}
	
	// ------------------------ IF:
	
	/** -> setId <-
	 * 
	 * 'id' -> identifier
	 * return: index-pointer <t>* or 0 - err
	 * exceptions: [MAPExcp][ID-overflow] , ...
	 */
	t* setId(cch *id) {
		
		try {
			_idx *x = set_tree_seg(id);
			return &x->val.rec->val;
		} catch (MAPExcp &excp) {
			throw VarExcp(VarExcp::_mk_excp("map_max_field_size", //
					var("max var:map field size is reached: >= ") //
					+= id_isz));
		}
		return 0;
	}
	
	/** -> getId <-
	 * 
	 * 'id' -> identifier
	 * return index-pointer <t>* or 0 -> not exist
	 */
	t* getId(cch *id) {
		
		if (id) {
			MAP_da_idx<_idx*, sli_isz> *x = get_tree_seg(id);
			if (x && x->val->val.rec) return &x->val->val.rec->val;
		}
		return 0;
	}
	
	/** -> remId <-
	 * 
	 * 'id'  -> identifier
	 * 'fun' -> pre-remove action-function (addition)
	 * return 0 -> ok or not exist (ambiguous but ...)
	 */
	ull remId(cch *id, void (*fun)(t*, void *ctx)=0, void *ctx = 0) {
		
		return rem_tree_seg(id, fun, ctx);
	}
	
	/** -> size <-
	 * 
	 * return total records	 
	 */
	ull size() const {
		return rec_li.sizeA();
	}
	
	/** -> free map <-
	 * 
	 * free all resorces and re-init
	 */
	void free() {
		
		seg_li.free();
		rec_li.free();
		// entr-point-init
		seg0 = seg_li.get();
	}
	
	/** -> prnt <-
	 * 
	 * 'deep' -> right deep
	 * return total records discovered
	 */
	ull prnt(ull deep = 0) {
		
		return _prnt(0, 0, deep);
	}
	
	/** -> foreach_x <-
	 * 
	 * 'fun'       -> function to call
	 * 'deep'      -> value of the deep to stop 'recursion'
	 * 'skp_rec_d' -> skip 'recursion' down (perf)
	 * 'ctx'       -> context to use
	 * return total records discovered
	 */
	struct STK {
		MAP_da_seg<_idx*, sli_isz> *cur = 0;
		ull rec = 0, i = 0, l = 0;
		ull stk_i[id_isz + 1];
		ull stk_l[id_isz + 1];
		char buf[id_isz + 1];
		MAP_da_seg<_idx*, sli_isz> *stk[id_isz + 1];
	};

	ull foreach(var::map (*fun)(char *bf, t*, int rec_ud, ull tot, int deep, //
			void *ctx), int deep = 0, bool skp_rec_d = 0, void *ctx = 0) const {
		
		STK _STK[_vm_deep];
		int REC = 0;
		t *val = 0;
		bool skp = 0;
		var::map x;
		
		MAP_da_seg<_idx*, sli_isz> *cur = seg0->val.li.fst;
		ull tot = 0, i = 0, l = 0;
		int rec = 0, _deep = 0;
		
		if (deep <= 0) deep = 0;
		else _deep = deep - 1;
		
		beg:
		
		if (cur) {
			lp:
			if (!skp) {
				l = cur->l, i = 0;
			}
			while (i < l) {
				if (!skp) {
					_STK[REC].buf[rec] = cur->_li[i].val->val.sid;
					if (cur->_li[i].val->val.rec) {
						_STK[REC].buf[rec + 1] = 0;
						++tot;
						val = &cur->_li[i].val->val.rec->val;
						
						if (deep == 0 || REC < _deep) {
							if (val->tid[0] == 'm') { // REC-up
									
								x = fun(_STK[REC].buf, val, 1, tot, rec, ctx);
								if (x == var::map::skp) goto _nxt;
								else if (x == var::map::brk) return tot;
								
								if (val->tid[0] == 'm') {
									_STK[REC].cur = cur;
									_STK[REC].rec = rec;
									_STK[REC].i = i;
									_STK[REC].l = l;
									++REC;
									if (REC >= _vm_deep) {
										throw VarExcp(VarExcp::_mk_excp("map_max_depth",							//
												var("max var:map depth is reached: >= ") //
												+= _vm_deep));
									}
									rec = 0;
									cur = var_map_get_val_ref(val->pttyp)-> //
									seg0->val.li.fst;
									goto beg;
								}
								goto _nxt;
							}
						}
						if ((fun(_STK[REC].buf, val, 0, tot, rec, ctx) //
						== var::map::brk)) return tot;
					}
				} else skp = 0;
				_nxt:
				if (cur->_li[i].val->val.li.L > 0) { // rec-up
					_STK[REC].stk_i[rec] = i, _STK[REC].stk_l[rec] = l;
					_STK[REC].stk[rec] = cur;
					++rec;
					cur = cur->_li[i].val->val.li.fst;
					goto beg;
				}
				nxt:
				++i;
			}
			if (cur->nxt) {
				cur = cur->nxt;
				goto lp;
			}
			--rec;
			if (rec >= 0) { // rec-down
				cur = _STK[REC].stk[rec];
				i = _STK[REC].stk_i[rec];
				l = _STK[REC].stk_l[rec];
				goto nxt;
			} else {
				--REC;
				if (REC >= 0) { // REC-down
					cur = _STK[REC].cur;
					rec = _STK[REC].rec;
					i = _STK[REC].i;
					l = _STK[REC].l;
					skp = 1;
					if (!skp_rec_d) {
						if ((fun(_STK[REC].buf, val, 2, tot, rec, ctx) //
						== var::map::brk)) return tot;
					}
					goto beg;
				}
				return tot;
			}
		}
		return 0;
	}
	
};

