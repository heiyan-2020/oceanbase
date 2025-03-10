add_subdirectory(compress)
add_subdirectory(restore)

ob_set_subtarget(oblib_lib ALONE
  timezone/ob_timezone_util.cpp
  geo/ob_geo_func_difference.cpp
  geo/ob_geo_func_covered_by.cpp
  geo/ob_geo_func_within.cpp
  geo/ob_geo_func_union.cpp
)

ob_set_subtarget(oblib_lib charset
  charset/ob_ctype_bin_os.cc
  charset/ob_ctype_gb18030_os.cc
  charset/ob_ctype_gbk_os.cc
  charset/ob_ctype_latin1_os.cc
  charset/ob_ctype_mb_os.cc
  charset/ob_ctype_simple_os.cc
  charset/ob_ctype_os.cc
  charset/ob_ctype_utf16_os.cc
  charset/ob_ctype_utf8_os.cc
  charset/ob_dtoa_os.cc
  charset/ob_charset.cpp
)

ob_set_subtarget(oblib_lib common
  ob_abort.cpp
  ob_date_unit_type.cpp
  ob_define.cpp
  ob_lib_config.cpp
  ob_name_id_def.cpp
  ob_replica_define.cpp
  ob_running_mode.cpp
  runtime.cpp
  worker.cpp
)

ob_set_subtarget(oblib_lib geo
  geo/ob_geo_common.cpp
  geo/ob_geo_bin.cpp
  geo/ob_geo_ibin.cpp
  geo/ob_geo_bin_iter.ipp
  geo/ob_geo_tree.cpp
  geo/ob_s2adapter.cpp
  geo/ob_geo.cpp
  geo/ob_wkt_parser.cpp
  geo/ob_geo_utils.cpp
  geo/ob_srs_wkt_parser.cpp
  geo/ob_srs_info.cpp
  geo/ob_geo_visitor.cpp
  geo/ob_geo_wkb_size_visitor.cpp
  geo/ob_geo_wkb_visitor.cpp
  geo/ob_geo_coordinate_range_visitor.cpp
  geo/ob_geo_latlong_check_visitor.cpp
  geo/ob_geo_longtitude_correct_visitor.cpp
  geo/ob_geo_to_tree_visitor.cpp
  geo/ob_geo_to_s2_visitor.cpp
  geo/ob_geo_to_wkt_visitor.cpp
  geo/ob_geo_reverse_coordinate_visitor.cpp
  geo/ob_geo_normalize_visitor.cpp
  geo/ob_geo_denormalize_visitor.cpp
  geo/ob_geo_wkb_check_visitor.cpp
  geo/ob_geo_zoom_in_visitor.cpp
  geo/ob_geo_func_envelope.cpp
  geo/ob_geo_func_intersects.cpp
  geo/ob_geo_func_area.cpp
  geo/ob_geo_func_correct.cpp
  geo/ob_geo_func_disjoint.cpp
  geo/ob_geo_func_utils.cpp
  geo/ob_geo_func_distance_sphere.cpp
  geo/ob_geo_func_transform.cpp
  geo/ob_geo_func_box.cpp
  geo/ob_geo_func_buffer.cpp
  geo/ob_geo_func_distance.cpp
  geo/ob_geo_func_isvalid.cpp
  geo/ob_geometry_cast.cpp
)

ob_set_subtarget(oblib_lib common_mixed
  atomic/ob_atomic_reference.cpp
  checksum/ob_crc64.cpp
  checksum/ob_parity_check.cpp
  container/ob_bitmap.cpp
  container/ob_vector.ipp
  cpu/ob_cpu_topology.cpp
  encode/ob_base64_encode.cpp
  encode/ob_quoted_printable_encode.cpp
  encode/ob_uuencode.cpp
  encrypt/ob_encrypted_helper.cpp
  file/file_directory_utils.cpp
  file/ob_file.cpp
  file/ob_string_util.cpp
  hash/ob_concurrent_hash_map.cpp
  hash/ob_hashutils.cpp
  hash/xxhash.c
  hash_func/murmur_hash.cpp
  json/ob_json.cpp
  json/ob_json_print_utils.cpp
  json/ob_yson.cpp
  json_type/ob_json_path.cpp
  json_type/ob_json_tree.cpp
  json_type/ob_json_bin.cpp
  json_type/ob_json_base.cpp
  json_type/ob_json_parse.cpp
  lds/ob_lds_define.cpp
  net/ob_addr.cpp
  net/ob_net_util.cpp
  number/ob_number_v2.cpp
  objectpool/ob_pool.ipp
  objectpool/ob_server_object_pool.cpp
  profile/ob_atomic_event.cpp
  profile/ob_perf_event.cpp
  profile/ob_profile_log.cpp
  profile/ob_trace_id.cpp
  profile/ob_trace_id_adaptor.cpp
  queue/ob_dedup_queue.cpp
  queue/ob_lighty_queue.cpp
  queue/ob_link_queue.cpp
  queue/ob_ms_queue.cpp
  random/ob_random.cpp
  rc/context.cpp
  rc/ob_rc.cpp
  rowid/ob_urowid.cpp
  stat/ob_di_tls.cpp
  stat/ob_di_cache.cpp
  stat/ob_diagnose_info.cpp
  stat/ob_latch_define.cpp
  stat/ob_session_stat.cpp
  stat/ob_stat_template.cpp
  statistic_event/ob_stat_event.cpp
  string/ob_hex_utils_base.cpp
  string/ob_sql_string.cpp
  string/ob_string.cpp
  string/ob_strings.cpp
  string/ob_string_buffer.cpp
  task/ob_timer.cpp
  task/ob_timer_monitor.cpp
  thread_local/ob_tsi_utils.cpp
  thread_local/thread_buffer.cpp
  time/Time.cpp
  time/ob_cur_time.cpp
  time/ob_time_utility.cpp
  time/ob_tsc_timestamp.cpp
  timezone/ob_oracle_format_models.cpp
  timezone/ob_time_convert.cpp
  timezone/ob_timezone_info.cpp
  trace/ob_trace.cpp
  trace/ob_trace_event.cpp
  wait_event/ob_wait_class.cpp
  wait_event/ob_wait_event.cpp
)

ob_set_subtarget(oblib_lib lock
  lock/cond.cpp
  lock/mutex.cpp
  lock/ob_bucket_lock.cpp
  lock/ob_latch.cpp
  lock/ob_thread_cond.cpp
  lock/ob_rwlock.cpp
  lock/ob_futex.cpp
  lock/ob_bucket_qsync_lock.cpp
  lock/ob_qsync_lock.cpp
)

ob_set_subtarget(oblib_lib mysqlclient
  mysqlclient/ob_isql_client.cpp
  mysqlclient/ob_dblink_error_trans.cpp
  mysqlclient/ob_mysql_connection.cpp
  mysqlclient/ob_mysql_connection_pool.cpp
  mysqlclient/ob_mysql_prepared_param.cpp
  mysqlclient/ob_mysql_prepared_result.cpp
  mysqlclient/ob_mysql_prepared_statement.cpp
  mysqlclient/ob_mysql_proxy.cpp
  mysqlclient/ob_mysql_proxy_util.cpp
  mysqlclient/ob_mysql_result.cpp
  mysqlclient/ob_mysql_result_impl.cpp
  mysqlclient/ob_mysql_statement.cpp
  mysqlclient/ob_mysql_transaction.cpp
  mysqlclient/ob_server_connection_pool.cpp
  mysqlclient/ob_single_connection_proxy.cpp
  mysqlclient/ob_single_mysql_connection_pool.cpp
)

ob_set_subtarget(oblib_lib oblog
  oblog/ob_async_log_struct.cpp
  oblog/ob_base_log_buffer.cpp
  oblog/ob_base_log_writer.cpp
  oblog/ob_easy_log.cpp
  oblog/ob_log.cpp
  oblog/ob_log_module.ipp
  oblog/ob_log_time_fmt.cpp
  oblog/ob_trace_log.cpp
  oblog/ob_warning_buffer.cpp
  oblog/ob_syslog_rate_limiter.cpp
)


ob_set_subtarget(oblib_lib signal
  signal/ob_signal_handlers.cpp
  signal/ob_signal_processor.cpp
  signal/ob_signal_struct.cpp
  signal/ob_signal_utils.cpp
  signal/ob_signal_worker.cpp
  signal/safe_snprintf.c
  signal/ob_libunwind.c
)

ob_set_subtarget(oblib_lib thread
  thread/ob_async_task_queue.cpp
  thread/ob_dynamic_thread_pool.cpp
  thread/ob_reentrant_thread.cpp
  thread/ob_simple_thread_pool.cpp
  thread/ob_map_queue_thread_pool.cpp
  thread/ob_work_queue.cpp
  thread/thread_mgr.cpp
  thread/thread_mgr_interface.cpp
  thread/protected_stack_allocator.cpp
  thread/thread.cpp
  thread/threads.cpp
  thread/ob_tenant_hook.cpp
)

ob_set_subtarget(oblib_lib utility
  utility/ob_fast_convert.cpp
  utility/ob_hang_fatal_error.cpp
  utility/ob_print_utils.cpp
  utility/ob_serialization_helper.cpp
  utility/ob_tracepoint.cpp
  utility/ob_utility.cpp
  utility/utility.cpp
  utility/ob_backtrace.cpp
  utility/ob_proto_trans_util.cpp
)

ob_set_subtarget(oblib_lib ash
  ash/ob_active_session_guard.cpp
)

ob_set_subtarget(oblib_lib ssl
  ssl/ob_ssl_config.cpp
)

ob_lib_add_target(oblib_lib)

ob_set_subtarget(ob_malloc_object_list common_alloc
  alloc/abit_set.cpp
  alloc/alloc_assist.cpp
  alloc/alloc_failed_reason.cpp
  alloc/alloc_func.cpp
  alloc/alloc_struct.cpp
  alloc/block_set.cpp
  alloc/memory_dump.cpp
  alloc/ob_free_log_printer.cpp
  alloc/ob_malloc_allocator.cpp
  alloc/ob_malloc_callback.cpp
  alloc/ob_malloc_sample_struct.cpp
  alloc/ob_tenant_ctx_allocator.cpp
  alloc/object_mgr.cpp
  alloc/object_set.cpp
  alloc/memory_sanity.cpp
  alloc/ob_futex_v2.cpp
  alloc/ob_latch_v2.cpp
  resource/achunk_mgr.cpp
  resource/ob_resource_mgr.cpp
  allocator/ob_allocator_v2.cpp
  allocator/ob_block_alloc_mgr.cpp
  allocator/ob_concurrent_fifo_allocator.cpp
  allocator/ob_ctx_parallel_define.cpp
  allocator/ob_delay_free_allocator.cpp
  allocator/ob_fifo_allocator.cpp
  allocator/ob_hazard_ref.cpp
  allocator/ob_libeasy_mem_pool.cpp
  allocator/ob_malloc.cpp
  allocator/ob_mem_leak_checker.cpp
  allocator/ob_mod_define.cpp
  allocator/ob_page_manager.cpp
  allocator/ob_slice_alloc.cpp
  allocator/ob_tc_malloc.cpp
)
ob_add_new_object_target(ob_malloc_object ob_malloc_object_list)
target_link_libraries(ob_malloc_object oblib_base_base_base)
add_library(ob_malloc STATIC)
target_link_libraries(ob_malloc ob_malloc_object)

add_library(malloc_hook STATIC
  alloc/malloc_hook.cpp
  alloc/malloc_hook.h)
target_link_libraries(malloc_hook oblib_base)

target_link_libraries(oblib_lib
  PUBLIC ob_malloc
)
