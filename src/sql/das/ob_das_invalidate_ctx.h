/*
 * ob_das_invalidate_ctx.h
 *
 *  Created on: Sep 18, 2023
 *      Author: Administrator
 */

#ifndef OB_DAS_INVALIDATE_CTX_H_
#define OB_DAS_INVALIDATE_CTX_H_

#include "lib/lock/ob_thread_cond.h"
#include "lib/atomic/ob_atomic.h"

namespace oceanbase
{
namespace sql
{


class ObInvalidateCtx
{
public:
  ObInvalidateCtx() : succ_(false) {
    int ret = OB_SUCCESS;
    if (OB_FAIL(cond_.init(ObWaitEventIds::DAS_ASYNC_RPC_LOCK_WAIT))) {
      LOG_ERROR("Failed to init thread cond", K(ret), K(MTL_ID()));
    }
  }

  void set_succ() {
    ObThreadCondGuard guard(cond_);
    ATOMIC_STORE(succ_, 1);
    cond_.signal();
  }

  int32_t get_succ() {
    return ATOMIC_LOAD(&succ_);
  }

  int wait_until_succ() {
    int ret = OB_SUCCESS;
    ObThreadCondGuard guard(cond_);
    while (OB_SUCC(ret) && get_succ() == 0) {
      if (OB_FAIL(cond_.wait())) {
        LOG_WARN("das cache: failed to wait invalidate");
      }
    }
    return ret;
  }

private:
  common::ObThreadCond cond_;
  int32_t succ_; // It would be a number in general case where txn may wait for multiple invalidate msgs.
};

}
}


#endif /* OB_DAS_INVALIDATE_CTX_H_ */
