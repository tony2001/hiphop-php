/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2013 Facebook, Inc. (http://www.facebook.com)     |
   | Copyright (c) 1998-2010 Zend Technologies Ltd. (http://www.zend.com) |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.00 of the Zend license,     |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.zend.com/license/2_00.txt.                                |
   | If you did not receive a copy of the Zend license and are unable to  |
   | obtain it through the world-wide-web, please send a note to          |
   | license@zend.com so we can mail you a copy immediately.              |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/base/comparisons.h"
#include "hphp/runtime/base/zend_functions.h"
#include "hphp/runtime/base/zend_string.h"

namespace HPHP {

//////////////////////////////////////////////////////////////////////
// Variant

bool same(CVarRef v1, bool v2) {
  return v1.isBoolean() && v2 == v1.getBoolean();
}

bool same(CVarRef v1, int64_t v2) {
  auto const cell = v1.asCell();
  switch (cell->m_type) {
  case KindOfInt64:
    return v2 == cell->m_data.num;
  default:
    break;
  }
  return false;
}

bool same(CVarRef v1, double d) {
  return v1.isDouble() && v1.getDouble() == d;
}

bool same(CVarRef v1, const StringData* v2) {
  bool null1 = v1.isNull();
  bool null2 = (v2 == nullptr);
  if (null1 && null2) return true;
  if (null1 || null2) return false;
  if (!v1.isString()) return false;
  auto const sdata = v1.getStringData();
  return sdata == v2 || v2->same(sdata);
}

// TODO(#2298051) litstr must die
bool same(CVarRef v1, litstr v2) {
  StackStringData sd2(v2);
  return same(v1, &sd2);
}

bool same(CVarRef v1, CStrRef v2) {
  const StringData* sd = v2.get();
  return same(v1, sd);
}

bool same(CVarRef v1, CArrRef v2) {
  bool null1 = v1.isNull();
  bool null2 = v2.isNull();
  if (null1 && null2) return true;
  if (null1 || null2) return false;
  if (!v1.isArray()) return false;
  auto const ad = v1.getArrayData();
  return v2->equal(ad, true);
}

bool same(CVarRef v1, CObjRef v2) {
  bool null1 = v1.isNull();
  bool null2 = v2.isNull();
  if (null1 && null2) return true;
  if (null1 || null2) return false;
  if (!v1.isObject()) return false;
  auto const od = v1.getObjectData();
  return od == v2.get();
}

bool same(CVarRef v1, CResRef v2) {
  bool null1 = v1.isNull();
  bool null2 = v2.isNull();
  if (null1 && null2) return true;
  if (null1 || null2) return false;
  if (!v1.isResource()) return false;
  auto const rd = v1.getResourceData();
  return rd == v2.get();
}

//////////////////////////////////////////////////////////////////////

bool equal(int v1, const StringData *v2) {
  return equal((int64_t)v1, v2);
}

bool equal(int64_t v1, const StringData *v2) {
  int64_t lval; double dval;
  DataType ret = v2->isNumericWithVal(lval, dval, 1);
  if (ret == KindOfInt64) {
    return v1 == lval;
  } else if (ret == KindOfDouble) {
    return (double)v1 == dval;
  } else {
    return v1 == 0;
  }
}

bool less(int v1, const StringData *v2) {
  return less((int64_t)v1, v2);
}

bool less(int64_t v1, const StringData *v2) {
  int64_t lval; double dval;
  DataType ret = v2->isNumericWithVal(lval, dval, 1);
  if (ret == KindOfInt64) {
    return v1 < lval;
  } else if (ret == KindOfDouble) {
    return (double)v1 < dval;
  } else {
    return v1 < 0;
  }
}

bool more(int v1, const StringData *v2) {
  return more((int64_t)v1, v2);
}

bool more(int64_t v1, const StringData *v2) {
  int64_t lval; double dval;
  DataType ret = v2->isNumericWithVal(lval, dval, 1);
  if (ret == KindOfInt64) {
    return v1 > lval;
  } else if (ret == KindOfDouble) {
    return (double)v1 > dval;
  } else {
    return v1 > 0;
  }
}

//////////////////////////////////////////////////////////////////////
}
