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

namespace oceanbase
{
namespace sql
{

class ObDASCacheKey : public common::ObIKVCacheKey
{
public:
	ObDASCacheKey();
	ObDASCacheKey(
      const uint64_t tenant_id,
      const ObTabletID &tablet_id,
      const ObRowKey &rowkey);
  virtual ~ObDASCacheKey() = default;
  virtual int equal(const ObIKVCacheKey &other, bool &equal) const override;
  virtual int hash(uint64_t &hash_value) const override;
  virtual uint64_t get_tenant_id() const override;
  virtual int64_t size() const override;
  virtual int deep_copy(char *buf, const int64_t buf_len, ObIKVCacheKey *&key) const override;
  bool is_valid() const;
  TO_STRING_KV(K_(tenant_id), K_(tablet_id), K_(rowkey_size), K_(rowkey));
private:
  uint64_t tenant_id_;
  ObTabletID tablet_id_;
  int64_t rowkey_size_;
  ObRowKey rowkey_;
  DISALLOW_COPY_AND_ASSIGN(ObDASCacheKey);
};

class ObDASCacheValue : public common::ObIKVCacheValue
{
//public:
//	ObDASCacheValue();
//  virtual ~ObDASCacheValue() = default;
//  int init(const blocksstable::ObDatumRow &row, const int64_t read_snapshot_version);
//  virtual int64_t size() const override;
//  virtual int deep_copy(char *buf, const int64_t buf_len, ObIKVCacheValue *&value) const override;
//  bool is_valid() const { return (nullptr != datums_ && 0 != column_cnt_) || (nullptr == datums_ && 0 == column_cnt_); }
//  OB_INLINE ObStorageDatum *get_datums() const { return datums_; }
//  OB_INLINE int64_t get_column_cnt() const { return column_cnt_; }
//  OB_INLINE int64_t get_read_snapshot_version() const { return read_snapshot_version_; }
//  ObDmlRowFlag get_flag() const { return flag_; }
//  TO_STRING_KV(KP_(datums), K_(size), K_(column_cnt), K_(read_snapshot_version), K_(flag));
//private:
//  ObStorageDatum *datums_;
//  int64_t size_;
//  int32_t column_cnt_;
//  int64_t read_snapshot_version_;
//  ObDmlRowFlag flag_;
};

class ObDASCache : public common::ObKVCache<ObDASCacheKey, ObDASCacheValue> {
public:

private:

};

}  // namespace sql
}  // namespace oceanbase
#endif /* OBDEV_SRC_SQL_DAS_OB_DAS_CACHE_H_ */
