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

#ifndef OBDEV_SRC_SQL_DAS_OB_DAS_CACHE_H_
#define OBDEV_SRC_SQL_DAS_OB_DAS_CACHE_H_

#include "share/cache/ob_kv_storecache.h"
#include "common/rowkey/ob_rowkey.h"
#include "storage/access/ob_table_read_info.h"
#include "sql/engine/basic/ob_chunk_datum_store.h"

namespace oceanbase
{
namespace sql
{

class ObDASCacheKey : public common::ObIKVCacheKey
{
public:
  ObDASCacheKey() : rowkey_size_(0), tenant_id_(0) {}
  virtual ~ObDASCacheKey() = default;
  int init(uint64_t tenant_id, ObTabletID &tablet_id, const ObRowkey &rowkey);
  virtual int equal(const ObIKVCacheKey &other, bool &equal) const override;
  virtual int hash(uint64_t &hash_value) const override;
  virtual uint64_t get_tenant_id() const override;
  virtual int64_t size() const override;
  virtual int deep_copy(char *buf, const int64_t buf_len, ObIKVCacheKey *&key) const override;
  bool is_valid() const;
  TO_STRING_KV(K_(tenant_id), K_(tablet_id), K_(rowkey));
private:
  uint64_t tenant_id_;
  ObTabletID tablet_id_;
  ObRowkey rowkey_;
  uint64_t rowkey_size_;
  DISALLOW_COPY_AND_ASSIGN(ObDASCacheKey);
};


class ObDASCacheValue : public common::ObIKVCacheValue
{
public:
  ObDASCacheValue() = default;
  virtual ~ObDASCacheValue() = default;
  int init(const ObChunkDatumStore::StoredRow &row);
  virtual int64_t size() const override;
  bool is_valid() const { return (nullptr != datums_ && 0 != col_cnt_) || (nullptr == datums_ && 0 == col_cnt_); }
  virtual int deep_copy(char *buf, const int64_t buf_len, ObIKVCacheValue *&value) const override;
  uint32_t get_col_count() const { return col_cnt_; }
  ObDatum *get_datums() const { return datums_; }
  TO_STRING_KV(KP_(datums), K_(col_cnt), K_(row_size));
private:
  uint32_t col_cnt_;
  uint32_t row_size_;
  // mark_datums_ is used to reduce memory copy, more specifically, it just
  // save pointer to const ObChunkDatumStore temporarily.
  // when deep copying, mark_datums_ is the source of data.
  const ObDatum *mark_datums_;
  ObDatum *datums_;
};

struct ObDASCacheValueHandle
{
	ObDASCacheValueHandle() : value_(nullptr), handle_() {}
	~ObDASCacheValueHandle() = default;
	ObDASCacheValue *value_;
	ObKVCacheHandle handle_;
};



class ObDASCache : public common::ObKVCache<ObDASCacheKey, ObDASCacheValue> {
public:
  ObDASCache() = default;
  virtual ~ObDASCache() = default;
  static ObDASCache &get_instance();
  int get_row(const ObDASCacheKey &key, ObDASCacheValueHandle &handle);
  int put_row(const ObDASCacheKey &key, ObDASCacheValue &value);

  ObArenaAllocator rowkey_allocator_;
private:
  DISALLOW_COPY_AND_ASSIGN(ObDASCache);
};


class ObDASCacheFetcher {
public:
  ObDASCacheFetcher() : is_inited_(false) {}
  ~ObDASCacheFetcher() = default;
  int init(ObTabletID &tablet_id);
  int get_row(const ObRowkey &key, ObDASCacheValueHandle &handler);
  int put_row(const ObChunkDatumStore::StoredRow *row, const ObIArray<ObColDesc> *desc);

private:
  /**
   * Extract primary key of a row.
   */
  int extract_key(const ObChunkDatumStore::StoredRow *row, const ObIArray<ObColDesc> *desc, ObRowkey &key);
private:
  ObTabletID tablet_id_;
  bool is_inited_;
};


class ObDASCacheResult : public common::ObNewRowIterator {
public:
  ObDASCacheResult() : output_exprs_(nullptr), eval_ctx_(nullptr) {}
  int init(const ExprFixedArray *output_exprs, ObEvalCtx *eval_ctx, ObDASCacheValueHandle &handle);
  virtual ~ObDASCacheResult() = default;
  virtual int get_next_row() override;
  virtual void reset() override;

private:
  const ExprFixedArray *output_exprs_;
  ObEvalCtx *eval_ctx_;
  ObDASCacheValueHandle handle_;
};

}  // namespace sql
}  // namespace oceanbase
#endif /* OBDEV_SRC_SQL_DAS_OB_DAS_CACHE_H_ */
