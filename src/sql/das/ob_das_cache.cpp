/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#define USING_LOG_PREFIX SQL_DAS
#include "sql/das/ob_das_cache.h"
#include "share/rc/ob_tenant_base.h"

namespace oceanbase
{
namespace sql
{


int ObDASCacheKey::init(uint64_t tenant_id, ObTabletID &tablet_id, const ObRowkey &rowkey) {
  int ret = OB_SUCCESS;
  tenant_id_ = tenant_id;
  tablet_id_ = tablet_id;
  rowkey_ = rowkey;
  rowkey_size_ = rowkey_.get_deep_copy_size();
  return ret;
}

int ObDASCacheKey::equal(const ObIKVCacheKey &other, bool &equal) const {
  int ret = OB_SUCCESS;
  const ObDASCacheKey &other_key = reinterpret_cast<const ObDASCacheKey &>(other);
  equal = tenant_id_ == other_key.tenant_id_;
  equal &= tablet_id_ == other_key.tablet_id_;
  equal &= (rowkey_size_ == other_key.rowkey_size_);
  if (equal && rowkey_size_ > 0) {
      if (OB_FAIL(rowkey_.equal(other_key.rowkey_, equal))) {
        LOG_WARN("failed to check das rowkey equal", K(ret));
      }
  }
  return ret;
}

int ObDASCacheKey::hash(uint64_t &hash_val) const {
  int ret = OB_SUCCESS;
  if (rowkey_.is_valid()) {
    if (OB_FAIL(rowkey_.hash(hash_val))) {
      LOG_WARN("das cache key hash error", K(ret));
    }
    hash_val = common::murmurhash(&tenant_id_, sizeof(tenant_id_), hash_val);
    hash_val = common::murmurhash(&tablet_id_, sizeof(tablet_id_), hash_val);
  } else {
    LOG_WARN("das cache key invalid", K(ret));
  }
  return ret;
}

uint64_t ObDASCacheKey::get_tenant_id() const {
  return tenant_id_;
}

int64_t ObDASCacheKey::size() const {
  return sizeof(*this) + rowkey_size_;
}

int ObDASCacheKey::deep_copy(char *buf, const int64_t buf_len, ObIKVCacheKey *&key) const {
  int ret = OB_SUCCESS;
    if (OB_UNLIKELY(nullptr == buf || buf_len < size())) {
      ret = OB_INVALID_ARGUMENT;
      LOG_WARN("invalid arguments", K(ret), KP(buf), K(buf_len), "request_size", size());
    } else if (OB_UNLIKELY(!is_valid())) {
      ret = OB_INVALID_DATA;
      LOG_WARN("invalid fuse row cache key", K(ret), K(*this));
    } else {
      ObDASCacheKey *pfuse_key = new (buf) ObDASCacheKey();
      pfuse_key->tenant_id_ = tenant_id_;
      pfuse_key->tablet_id_ = tablet_id_;
      if (rowkey_.is_valid() && rowkey_size_ > 0) {
        ObRawBufAllocatorWrapper tmp_buf(buf + sizeof(*this), rowkey_size_);
        if (OB_FAIL(rowkey_.deep_copy(pfuse_key->rowkey_, tmp_buf))) {
          LOG_WARN("fail to deep copy rowkey", K(ret));
        } else {
          pfuse_key->rowkey_size_ = rowkey_size_;
          key = pfuse_key;
        }
      }
      if (OB_FAIL(ret)) {
        pfuse_key->~ObDASCacheKey();
        pfuse_key = nullptr;
      }
    }
    return ret;
}

bool ObDASCacheKey::is_valid() const {
  return OB_LIKELY(tenant_id_ != 0 && tablet_id_.is_valid() && rowkey_size_ > 0);
}


int ObDASCacheValue::init(const ObChunkDatumStore::StoredRow &row) {
	int ret = OB_SUCCESS;

	// TODO: @kongye add more sanity check.
	col_cnt_ = row.cnt_;
	row_size_ = row.row_size_;
	mark_datums_ = row.cells();

	return ret;
}

int64_t ObDASCacheValue::size() const
{
  return sizeof(*this) + row_size_;
}

int ObDASCacheValue::deep_copy(char *buf, const int64_t buf_len, ObIKVCacheValue *&value) const {
  int ret = OB_SUCCESS;

  if (OB_UNLIKELY(nullptr == buf || buf_len < size())) {
    ret = OB_INVALID_ARGUMENT;
    LOG_WARN("invalid arguments", K(ret), KP(buf), K(buf_len), "request_size", size());
  } else if (OB_UNLIKELY(!is_valid())) {
    ret = OB_INVALID_DATA;
    LOG_WARN("invalid row cache value", K(ret));
  } else {
    int64_t pos = 0;
    ObDASCacheValue *pcache_value = new (buf) ObDASCacheValue();
    const ObDatum *src = nullptr;
    if (nullptr == mark_datums_) {
      src = datums_;
    } else {
      src = mark_datums_;
    }

    if (src == nullptr) {
      ret = OB_INVALID_DATA;
    } else {
      char *tmp_buf = buf + sizeof(*this);
      MEMCPY(tmp_buf, src, sizeof(ObDatum) * col_cnt_);
      pcache_value->datums_ = reinterpret_cast<ObDatum *>(tmp_buf);
      pcache_value->col_cnt_ = col_cnt_;
      pcache_value->row_size_ = row_size_;

      pos = sizeof(*this) + sizeof(ObDatum) * col_cnt_;
      for (int64_t i = 0; OB_SUCC(ret) && i < col_cnt_; ++i) {
        if (OB_FAIL(pcache_value->datums_[i].deep_copy(src[i], buf, buf_len, pos))) {
          LOG_WARN("Failed to deep copy datum", K(ret), K(i));
        }
      }
    }

    if (OB_SUCC(ret)) {
      value = pcache_value;
    } else if (nullptr != pcache_value) {
        pcache_value->~ObDASCacheValue();
        pcache_value = nullptr;
    }
  }
  return ret;
}


ObDASCache &ObDASCache::get_instance() {
  static ObDASCache instance;
  static bool init = false;
  if (!init) {
    // TODO: consider data race.
    instance.init("das_row_cache");
    init = true;
  }
  return instance;
}

int ObDASCache::get_row(const ObDASCacheKey &key, ObDASCacheValueHandle &handle) {
  int ret = OB_SUCCESS;
  const ObDASCacheValue *value = nullptr;
  if (OB_UNLIKELY(!key.is_valid())) {
    ret = OB_INVALID_ARGUMENT;
    LOG_WARN("invalid arguments", K(ret), K(key));
  } else if (OB_FAIL(get(key, value, handle.handle_))) {
    if (OB_UNLIKELY(OB_ENTRY_NOT_EXIST != ret)) {
      LOG_WARN("fail to get key from row cache", K(ret));
    }
  } else {
    if (OB_ISNULL(value)) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("unexpected error, the value must not be NULL", K(ret));
    } else {
      handle.value_ = const_cast<ObDASCacheValue *>(value);
    }
  }

  uint64_t cache_cnt = count(key.get_tenant_id());
  LOG_INFO("das cache: get", K(cache_cnt), K(key));


  return ret;
}

int ObDASCache::put_row(const ObDASCacheKey &key, ObDASCacheValue &value) {
  int ret = OB_SUCCESS;
  if (OB_UNLIKELY(!key.is_valid() || !value.is_valid())) {
    ret = OB_INVALID_ARGUMENT;
    LOG_WARN("invalid arguments", K(ret), K(key), K(value));
  } else if (OB_FAIL(put(key, value, true/*overwrite*/))) {
    LOG_WARN("fail to put row to das row cache", K(ret), K(key), K(value));
  }

  uint64_t cache_cnt = count(key.get_tenant_id());
  LOG_INFO("das cache: put", K(cache_cnt), K(key), K(value));
  return ret;
}

int ObDASCache::invalidate_row(const ObDASCacheKey &key) {
  int ret = OB_SUCCESS;
  if (OB_UNLIKELY(!key.is_valid())) {
    ret = OB_INVALID_ARGUMENT;
    LOG_WARN("invalid arguments", K(ret), K(key));
  } else if (OB_FAIL(erase(key))) {
    if (OB_UNLIKELY(OB_ENTRY_NOT_EXIST == ret)) {
      ret = OB_SUCCESS;
    }
  }

  LOG_INFO("[das cache]: invalidate", K(key), K(ret));

  return ret;
}

int ObDASCacheFetcher::init(const ObTabletID &tablet_id) {
  tablet_id_ = tablet_id;
  is_inited_ = true;
  int ret = OB_SUCCESS;
  return ret;
}

int ObDASCacheFetcher::get_row(const ObRowkey &key, ObDASCacheValueHandle &handle) {
  int ret = OB_SUCCESS;

  if (OB_UNLIKELY(!is_inited_)) {
    ret = OB_NOT_INIT;
    LOG_WARN("ObDASCacheFetcher has not been inited", K(ret));
  } else {
    ObDASCacheKey cache_key;
    cache_key.init(MTL_ID(), tablet_id_, key);
    if (OB_FAIL(ObDASCache::get_instance().get_row(cache_key, handle))) {
      if (OB_ENTRY_NOT_EXIST != ret) {
        STORAGE_LOG(WARN, "fail to get row from das row cache", K(ret), K(key));
      } else {
        LOG_INFO("das cache: miss", K(key));
      }
    } else {
      LOG_INFO("das cache: hit", K(key));
    }
  }

  return ret;
}

int ObDASCacheFetcher::put_row(const ObChunkDatumStore::StoredRow *row, const ObIArray<ObColDesc> *desc) {
  int ret = OB_SUCCESS;

  ObRowkey rowkey;
  ObDASCacheKey cache_key;
  ObDASCacheValue cache_value;
  if (OB_FAIL(extract_key(row, desc, rowkey))) {
    LOG_WARN("Failed to extract cache key", K(ret));
  } else if (OB_FAIL(cache_key.init(MTL_ID(), tablet_id_, rowkey))) {
    LOG_WARN("Failed to init cache key", K(ret));
  } else if (OB_FAIL(cache_value.init(*row))) {
    LOG_WARN("Failed to init cache value", K(ret));
  } else if (OB_FAIL(ObDASCache::get_instance().put_row(cache_key, cache_value))) {
    LOG_WARN("Failed to init put cache", K(ret));
  }

  return ret;
}

int ObDASCacheFetcher::invalidate_row(const ObRowkey &key) {
  int ret = OB_SUCCESS;
  ObDASCacheKey cache_key;

  if (OB_FAIL(cache_key.init(MTL_ID(), tablet_id_, key))) {
    LOG_WARN("Failed to init cache key", K(ret));
  } else if (OB_FAIL(ObDASCache::get_instance().invalidate_row(cache_key))) {
    LOG_WARN("Failed to invalidate cache", K(ret));
  }

  return ret;
}

int ObDASCacheFetcher::extract_key(const ObChunkDatumStore::StoredRow *row, const ObIArray<ObColDesc> *desc, ObRowkey &key) {
  int ret = OB_SUCCESS;
  // In this temporary version, we assume that `exprs` represents full row and the first expr is primary key.
  if (row->cells() == nullptr) {
    ret = OB_ERROR;
    LOG_WARN("storerow is empty", K(ret));
  } else {
    const ObDatum &pk = row->cells()[0];
    const common::ObObjMeta pk_meta = desc->at(0).col_type_;
    // TODO: @kongye don't allocate memory this way, try to reuse the allocator of KVCache
    common::ObIAllocator &allocator = ObDASCache::get_instance().rowkey_allocator_;
    ObObj* ptr = reinterpret_cast<common::ObObj*>(allocator.alloc(sizeof(sizeof(common::ObObj))));
    pk.to_obj(*ptr, pk_meta);
    // assume primary key is one column.
    key.assign(ptr, 1);
  }
  return ret;
}


int ObDASCacheResult::init(const ExprFixedArray *output_exprs, ObEvalCtx *eval_ctx, ObDASCacheValueHandle &handle) {
  int ret = OB_SUCCESS;
  output_exprs_ = output_exprs;
  eval_ctx_ = eval_ctx;
  handle_.handle_ = handle.handle_;
  handle_.value_ = handle.value_;
  return ret;
}

int ObDASCacheResult::get_next_row() {
  int ret = OB_SUCCESS;

  ObDASCacheValue *value = handle_.value_;
  if (OB_UNLIKELY(value->get_col_count() != output_exprs_->count())) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("das cache warn: cache value is unmatched with expr", K(ret));
  } else if (!fetched_) {
    for (uint32_t i = 0; i < value->get_col_count(); i++) {
      ObExpr *expr = output_exprs_->at(i);
      if (expr->is_const_expr()) {
        continue;
      } else {
        const ObDatum &src = value->get_datums()[i];
        ObDatum &dst = expr->locate_expr_datum(*eval_ctx_);
        dst = src;
        expr->set_evaluated_projected(*eval_ctx_);
      }
    }

    fetched_ = true;
  } else {
    ret = OB_ITER_END;
  }

  return ret;
}

int ObDASCacheResult::get_next_row(ObNewRow *&row)
{
  UNUSED(row);
  return OB_NOT_IMPLEMENT;
}


void ObDASCacheResult::reset() {
  // TODO: when does this function be called?
  fetched_ = false;
  output_exprs_ = nullptr;
  eval_ctx_ = nullptr;
  handle_.handle_.reset();
  int ret = OB_SUCCESS;
  LOG_WARN("reset has been called");
}


} // namespace sql
} // namespace oceanbase
