#define scheme_init_jmpup_buf (scheme_extension_table->scheme_init_jmpup_buf)
#define scheme_setjmpup_relative (scheme_extension_table->scheme_setjmpup_relative)
#define scheme_longjmpup (scheme_extension_table->scheme_longjmpup)
#define scheme_reset_jmpup_buf (scheme_extension_table->scheme_reset_jmpup_buf)
#ifdef USE_MZ_SETJMP
#define scheme_mz_setjmp (scheme_extension_table->scheme_mz_setjmp)
#define scheme_mz_longjmp (scheme_extension_table->scheme_mz_longjmp)
#endif
#define scheme_clear_escape (scheme_extension_table->scheme_clear_escape)
#define scheme_new_jmpupbuf_holder (scheme_extension_table->scheme_new_jmpupbuf_holder)
#define scheme_make_config (scheme_extension_table->scheme_make_config)
#define scheme_branch_config (scheme_extension_table->scheme_branch_config)
#define scheme_new_param (scheme_extension_table->scheme_new_param)
#define scheme_param_config (scheme_extension_table->scheme_param_config)
#define scheme_register_parameter (scheme_extension_table->scheme_register_parameter)
#define scheme_get_env (scheme_extension_table->scheme_get_env)
#ifndef LINK_EXTENSIONS_BY_TABLE
#define scheme_current_thread (scheme_extension_table->scheme_current_thread)
#define scheme_fuel_counter (scheme_extension_table->scheme_fuel_counter)
#else
#define scheme_current_thread_ptr (scheme_extension_table->scheme_current_thread_ptr)
#define scheme_fuel_counter_ptr (scheme_extension_table->scheme_fuel_counter_ptr)
#endif
#define scheme_out_of_fuel (scheme_extension_table->scheme_out_of_fuel)
#define scheme_thread (scheme_extension_table->scheme_thread)
#define scheme_thread_w_custodian (scheme_extension_table->scheme_thread_w_custodian)
#define scheme_thread_w_custodian_killkind (scheme_extension_table->scheme_thread_w_custodian_killkind)
#define scheme_kill_thread (scheme_extension_table->scheme_kill_thread)
#define scheme_break_thread (scheme_extension_table->scheme_break_thread)
#define scheme_thread_block (scheme_extension_table->scheme_thread_block)
#define scheme_swap_thread (scheme_extension_table->scheme_swap_thread)
#define scheme_making_progress (scheme_extension_table->scheme_making_progress)
#define scheme_weak_suspend_thread (scheme_extension_table->scheme_weak_suspend_thread)
#define scheme_weak_resume_thread (scheme_extension_table->scheme_weak_resume_thread)
#define scheme_block_until (scheme_extension_table->scheme_block_until)
#define scheme_in_main_thread (scheme_extension_table->scheme_in_main_thread)
#define scheme_cancel_sleep (scheme_extension_table->scheme_cancel_sleep)
#define scheme_tls_allocate (scheme_extension_table->scheme_tls_allocate)
#define scheme_tls_set (scheme_extension_table->scheme_tls_set)
#define scheme_tls_get (scheme_extension_table->scheme_tls_get)
#define scheme_make_custodian (scheme_extension_table->scheme_make_custodian)
#define scheme_add_managed (scheme_extension_table->scheme_add_managed)
#define scheme_remove_managed (scheme_extension_table->scheme_remove_managed)
#define scheme_close_managed (scheme_extension_table->scheme_close_managed)
#define scheme_schedule_custodian_close (scheme_extension_table->scheme_schedule_custodian_close)
#define scheme_add_atexit_closer (scheme_extension_table->scheme_add_atexit_closer)
#define scheme_add_waitable (scheme_extension_table->scheme_add_waitable)
#define scheme_add_waitable_through_sema (scheme_extension_table->scheme_add_waitable_through_sema)
#define scheme_is_waitable (scheme_extension_table->scheme_is_waitable)
#define scheme_object_wait_multiple (scheme_extension_table->scheme_object_wait_multiple)
#define scheme_object_wait_multiple_enable_break (scheme_extension_table->scheme_object_wait_multiple_enable_break)
#define scheme_make_waitable_set (scheme_extension_table->scheme_make_waitable_set)
#define scheme_add_swap_callback (scheme_extension_table->scheme_add_swap_callback)
#define scheme_call_enable_break (scheme_extension_table->scheme_call_enable_break)
#define scheme_close_should_force_port_closed (scheme_extension_table->scheme_close_should_force_port_closed)
#define scheme_push_kill_action (scheme_extension_table->scheme_push_kill_action)
#define scheme_pop_kill_action (scheme_extension_table->scheme_pop_kill_action)
#define scheme_signal_error (scheme_extension_table->scheme_signal_error)
#define scheme_raise_exn (scheme_extension_table->scheme_raise_exn)
#define scheme_warning (scheme_extension_table->scheme_warning)
#define scheme_raise (scheme_extension_table->scheme_raise)
#define scheme_wrong_count (scheme_extension_table->scheme_wrong_count)
#define scheme_wrong_count_m (scheme_extension_table->scheme_wrong_count_m)
#define scheme_case_lambda_wrong_count (scheme_extension_table->scheme_case_lambda_wrong_count)
#define scheme_wrong_type (scheme_extension_table->scheme_wrong_type)
#define scheme_arg_mismatch (scheme_extension_table->scheme_arg_mismatch)
#define scheme_wrong_return_arity (scheme_extension_table->scheme_wrong_return_arity)
#define scheme_unbound_global (scheme_extension_table->scheme_unbound_global)
#define scheme_dynamic_wind (scheme_extension_table->scheme_dynamic_wind)
#define scheme_make_type (scheme_extension_table->scheme_make_type)
#define scheme_get_type_name (scheme_extension_table->scheme_get_type_name)
#define scheme_eof (scheme_extension_table->scheme_eof)
#define scheme_null (scheme_extension_table->scheme_null)
#define scheme_true (scheme_extension_table->scheme_true)
#define scheme_false (scheme_extension_table->scheme_false)
#define scheme_void (scheme_extension_table->scheme_void)
#define scheme_undefined (scheme_extension_table->scheme_undefined)
#define scheme_tail_call_waiting (scheme_extension_table->scheme_tail_call_waiting)
#define scheme_multiple_values (scheme_extension_table->scheme_multiple_values)
#define scheme_eval (scheme_extension_table->scheme_eval)
#define scheme_eval_multi (scheme_extension_table->scheme_eval_multi)
#define scheme_eval_compiled (scheme_extension_table->scheme_eval_compiled)
#define scheme_eval_compiled_multi (scheme_extension_table->scheme_eval_compiled_multi)
#define _scheme_eval_compiled (scheme_extension_table->_scheme_eval_compiled)
#define _scheme_eval_compiled_multi (scheme_extension_table->_scheme_eval_compiled_multi)
#define scheme_apply (scheme_extension_table->scheme_apply)
#define scheme_apply_multi (scheme_extension_table->scheme_apply_multi)
#define scheme_apply_eb (scheme_extension_table->scheme_apply_eb)
#define scheme_apply_multi_eb (scheme_extension_table->scheme_apply_multi_eb)
#define scheme_apply_to_list (scheme_extension_table->scheme_apply_to_list)
#define scheme_eval_string (scheme_extension_table->scheme_eval_string)
#define scheme_eval_string_multi (scheme_extension_table->scheme_eval_string_multi)
#define scheme_eval_string_all (scheme_extension_table->scheme_eval_string_all)
#define _scheme_apply_known_closed_prim (scheme_extension_table->_scheme_apply_known_closed_prim)
#define _scheme_apply_known_closed_prim_multi (scheme_extension_table->_scheme_apply_known_closed_prim_multi)
#define _scheme_apply_closed_prim (scheme_extension_table->_scheme_apply_closed_prim)
#define _scheme_apply_closed_prim_multi (scheme_extension_table->_scheme_apply_closed_prim_multi)
#define scheme_values (scheme_extension_table->scheme_values)
#define scheme_check_one_value (scheme_extension_table->scheme_check_one_value)
#define scheme_tail_apply (scheme_extension_table->scheme_tail_apply)
#define scheme_tail_apply_no_copy (scheme_extension_table->scheme_tail_apply_no_copy)
#define scheme_tail_apply_to_list (scheme_extension_table->scheme_tail_apply_to_list)
#define scheme_tail_eval_expr (scheme_extension_table->scheme_tail_eval_expr)
#define scheme_set_tail_buffer_size (scheme_extension_table->scheme_set_tail_buffer_size)
#define scheme_force_value (scheme_extension_table->scheme_force_value)
#define scheme_set_cont_mark (scheme_extension_table->scheme_set_cont_mark)
#define scheme_push_continuation_frame (scheme_extension_table->scheme_push_continuation_frame)
#define scheme_pop_continuation_frame (scheme_extension_table->scheme_pop_continuation_frame)
#define scheme_temp_dec_mark_depth (scheme_extension_table->scheme_temp_dec_mark_depth)
#define scheme_temp_inc_mark_depth (scheme_extension_table->scheme_temp_inc_mark_depth)
#define scheme_current_continuation_marks (scheme_extension_table->scheme_current_continuation_marks)
#define scheme_do_eval (scheme_extension_table->scheme_do_eval)
#define scheme_eval_compiled_stx_string (scheme_extension_table->scheme_eval_compiled_stx_string)
#define scheme_load_compiled_stx_string (scheme_extension_table->scheme_load_compiled_stx_string)
#define scheme_compiled_stx_symbol (scheme_extension_table->scheme_compiled_stx_symbol)
#define scheme_eval_compiled_sized_string (scheme_extension_table->scheme_eval_compiled_sized_string)
#ifndef SCHEME_NO_GC
# ifndef SCHEME_NO_GC_PROTO
#define GC_malloc (scheme_extension_table->GC_malloc)
#define GC_malloc_atomic (scheme_extension_table->GC_malloc_atomic)
#  ifdef MZ_PRECISE_GC
#define GC_malloc_one_tagged (scheme_extension_table->GC_malloc_one_tagged)
#define GC_malloc_atomic_uncollectable (scheme_extension_table->GC_malloc_atomic_uncollectable)
#define GC_malloc_array_tagged (scheme_extension_table->GC_malloc_array_tagged)
#  else
#define GC_malloc_stubborn (scheme_extension_table->GC_malloc_stubborn)
#define GC_malloc_uncollectable (scheme_extension_table->GC_malloc_uncollectable)
#  endif
# endif
#endif
#define scheme_malloc_eternal (scheme_extension_table->scheme_malloc_eternal)
#define scheme_end_stubborn_change (scheme_extension_table->scheme_end_stubborn_change)
#define scheme_calloc (scheme_extension_table->scheme_calloc)
#define scheme_strdup (scheme_extension_table->scheme_strdup)
#define scheme_strdup_eternal (scheme_extension_table->scheme_strdup_eternal)
#define scheme_malloc_fail_ok (scheme_extension_table->scheme_malloc_fail_ok)
#ifndef MZ_PRECISE_GC
#define scheme_weak_reference (scheme_extension_table->scheme_weak_reference)
#define scheme_weak_reference_indirect (scheme_extension_table->scheme_weak_reference_indirect)
#define scheme_unweak_reference (scheme_extension_table->scheme_unweak_reference)
#endif
#define scheme_add_finalizer (scheme_extension_table->scheme_add_finalizer)
#define scheme_add_finalizer_once (scheme_extension_table->scheme_add_finalizer_once)
#define scheme_add_scheme_finalizer (scheme_extension_table->scheme_add_scheme_finalizer)
#define scheme_add_scheme_finalizer_once (scheme_extension_table->scheme_add_scheme_finalizer_once)
#define scheme_register_finalizer (scheme_extension_table->scheme_register_finalizer)
#define scheme_remove_all_finalization (scheme_extension_table->scheme_remove_all_finalization)
#define scheme_dont_gc_ptr (scheme_extension_table->scheme_dont_gc_ptr)
#define scheme_gc_ptr_ok (scheme_extension_table->scheme_gc_ptr_ok)
#define scheme_collect_garbage (scheme_extension_table->scheme_collect_garbage)
#ifdef MZ_PRECISE_GC
#define GC_variable_stack (scheme_extension_table->GC_variable_stack)
#define GC_register_traversers (scheme_extension_table->GC_register_traversers)
#define GC_resolve (scheme_extension_table->GC_resolve)
#define GC_mark (scheme_extension_table->GC_mark)
#define GC_fixup (scheme_extension_table->GC_fixup)
#endif
#define scheme_make_bucket_table (scheme_extension_table->scheme_make_bucket_table)
#define scheme_add_to_table (scheme_extension_table->scheme_add_to_table)
#define scheme_change_in_table (scheme_extension_table->scheme_change_in_table)
#define scheme_lookup_in_table (scheme_extension_table->scheme_lookup_in_table)
#define scheme_bucket_from_table (scheme_extension_table->scheme_bucket_from_table)
#define scheme_bucket_table_equal (scheme_extension_table->scheme_bucket_table_equal)
#define scheme_make_hash_table (scheme_extension_table->scheme_make_hash_table)
#define scheme_make_hash_table_equal (scheme_extension_table->scheme_make_hash_table_equal)
#define scheme_hash_set (scheme_extension_table->scheme_hash_set)
#define scheme_hash_get (scheme_extension_table->scheme_hash_get)
#define scheme_hash_table_equal (scheme_extension_table->scheme_hash_table_equal)
#define scheme_is_hash_table_equal (scheme_extension_table->scheme_is_hash_table_equal)
#define scheme_make_prim (scheme_extension_table->scheme_make_prim)
#define scheme_make_noneternal_prim (scheme_extension_table->scheme_make_noneternal_prim)
#define scheme_make_closed_prim (scheme_extension_table->scheme_make_closed_prim)
#define scheme_make_prim_w_arity (scheme_extension_table->scheme_make_prim_w_arity)
#define scheme_make_folding_prim (scheme_extension_table->scheme_make_folding_prim)
#define scheme_make_noneternal_prim_w_arity (scheme_extension_table->scheme_make_noneternal_prim_w_arity)
#define scheme_make_closed_prim_w_arity (scheme_extension_table->scheme_make_closed_prim_w_arity)
#define scheme_make_folding_closed_prim (scheme_extension_table->scheme_make_folding_closed_prim)
#define scheme_prim_is_method (scheme_extension_table->scheme_prim_is_method)
#define scheme_make_pair (scheme_extension_table->scheme_make_pair)
#define scheme_make_immutable_pair (scheme_extension_table->scheme_make_immutable_pair)
#define scheme_make_string (scheme_extension_table->scheme_make_string)
#define scheme_make_sized_string (scheme_extension_table->scheme_make_sized_string)
#define scheme_make_sized_offset_string (scheme_extension_table->scheme_make_sized_offset_string)
#define scheme_make_immutable_sized_string (scheme_extension_table->scheme_make_immutable_sized_string)
#define scheme_make_string_without_copying (scheme_extension_table->scheme_make_string_without_copying)
#define scheme_alloc_string (scheme_extension_table->scheme_alloc_string)
#define scheme_append_string (scheme_extension_table->scheme_append_string)
#define scheme_make_vector (scheme_extension_table->scheme_make_vector)
#define scheme_make_integer_value (scheme_extension_table->scheme_make_integer_value)
#define scheme_make_integer_value_from_unsigned (scheme_extension_table->scheme_make_integer_value_from_unsigned)
#define scheme_make_integer_value_from_long_long (scheme_extension_table->scheme_make_integer_value_from_long_long)
#define scheme_make_integer_value_from_unsigned_long_long (scheme_extension_table->scheme_make_integer_value_from_unsigned_long_long)
#define scheme_make_double (scheme_extension_table->scheme_make_double)
#ifdef MZ_USE_SINGLE_FLOATS
#define scheme_make_float (scheme_extension_table->scheme_make_float)
#endif
#define scheme_make_char (scheme_extension_table->scheme_make_char)
#define scheme_make_sema (scheme_extension_table->scheme_make_sema)
#define scheme_post_sema (scheme_extension_table->scheme_post_sema)
#define scheme_post_sema_all (scheme_extension_table->scheme_post_sema_all)
#define scheme_wait_sema (scheme_extension_table->scheme_wait_sema)
#define scheme_char_constants (scheme_extension_table->scheme_char_constants)
#define scheme_make_channel (scheme_extension_table->scheme_make_channel)
#define scheme_get_int_val (scheme_extension_table->scheme_get_int_val)
#define scheme_get_unsigned_int_val (scheme_extension_table->scheme_get_unsigned_int_val)
#define scheme_real_to_double (scheme_extension_table->scheme_real_to_double)
#define scheme_make_cptr (scheme_extension_table->scheme_make_cptr)
#define scheme_get_proc_name (scheme_extension_table->scheme_get_proc_name)
#define scheme_make_bignum (scheme_extension_table->scheme_make_bignum)
#define scheme_make_bignum_from_unsigned (scheme_extension_table->scheme_make_bignum_from_unsigned)
#define scheme_bignum_to_double (scheme_extension_table->scheme_bignum_to_double)
#define scheme_bignum_from_double (scheme_extension_table->scheme_bignum_from_double)
#ifdef MZ_USE_SINGLE_FLOATS
#define scheme_bignum_to_float (scheme_extension_table->scheme_bignum_to_float)
#define scheme_bignum_from_float (scheme_extension_table->scheme_bignum_from_float)
#else
# define scheme_bignum_to_float scheme_bignum_to_double
# define scheme_bignum_from_float scheme_bignum_from_double
#endif
#define scheme_bignum_to_string (scheme_extension_table->scheme_bignum_to_string)
#define scheme_bignum_to_allocated_string (scheme_extension_table->scheme_bignum_to_allocated_string)
#define scheme_read_bignum (scheme_extension_table->scheme_read_bignum)
#define scheme_bignum_normalize (scheme_extension_table->scheme_bignum_normalize)
#define scheme_make_rational (scheme_extension_table->scheme_make_rational)
#define scheme_rational_to_double (scheme_extension_table->scheme_rational_to_double)
#define scheme_rational_from_double (scheme_extension_table->scheme_rational_from_double)
#ifdef MZ_USE_SINGLE_FLOATS
#define scheme_rational_to_float (scheme_extension_table->scheme_rational_to_float)
#define scheme_rational_from_float (scheme_extension_table->scheme_rational_from_float)
#else
# define scheme_rational_to_float scheme_rational_to_double
# define scheme_rational_from_float scheme_rational_from_double
#endif
#define scheme_rational_normalize (scheme_extension_table->scheme_rational_normalize)
#define scheme_rational_numerator (scheme_extension_table->scheme_rational_numerator)
#define scheme_rational_denominator (scheme_extension_table->scheme_rational_denominator)
#define scheme_make_complex (scheme_extension_table->scheme_make_complex)
#define scheme_complex_normalize (scheme_extension_table->scheme_complex_normalize)
#define scheme_complex_real_part (scheme_extension_table->scheme_complex_real_part)
#define scheme_complex_imaginary_part (scheme_extension_table->scheme_complex_imaginary_part)
#define scheme_is_exact (scheme_extension_table->scheme_is_exact)
#define scheme_is_inexact (scheme_extension_table->scheme_is_inexact)
#define scheme_expand (scheme_extension_table->scheme_expand)
#define scheme_compile (scheme_extension_table->scheme_compile)
#define scheme_read (scheme_extension_table->scheme_read)
#define scheme_read_syntax (scheme_extension_table->scheme_read_syntax)
#define scheme_write (scheme_extension_table->scheme_write)
#define scheme_display (scheme_extension_table->scheme_display)
#define scheme_write_w_max (scheme_extension_table->scheme_write_w_max)
#define scheme_display_w_max (scheme_extension_table->scheme_display_w_max)
#define scheme_write_string (scheme_extension_table->scheme_write_string)
#define scheme_put_string (scheme_extension_table->scheme_put_string)
#define scheme_write_to_string (scheme_extension_table->scheme_write_to_string)
#define scheme_display_to_string (scheme_extension_table->scheme_display_to_string)
#define scheme_write_to_string_w_max (scheme_extension_table->scheme_write_to_string_w_max)
#define scheme_display_to_string_w_max (scheme_extension_table->scheme_display_to_string_w_max)
#define scheme_debug_print (scheme_extension_table->scheme_debug_print)
#define scheme_flush_output (scheme_extension_table->scheme_flush_output)
#define scheme_format (scheme_extension_table->scheme_format)
#define scheme_printf (scheme_extension_table->scheme_printf)
#define scheme_getc (scheme_extension_table->scheme_getc)
#define scheme_peekc (scheme_extension_table->scheme_peekc)
#define scheme_getc_special_ok (scheme_extension_table->scheme_getc_special_ok)
#define scheme_peekc_special_ok (scheme_extension_table->scheme_peekc_special_ok)
#define scheme_ungetc (scheme_extension_table->scheme_ungetc)
#define scheme_char_ready (scheme_extension_table->scheme_char_ready)
#define scheme_peekc_is_ungetc (scheme_extension_table->scheme_peekc_is_ungetc)
#define scheme_need_wakeup (scheme_extension_table->scheme_need_wakeup)
#define scheme_get_string (scheme_extension_table->scheme_get_string)
#define scheme_get_chars (scheme_extension_table->scheme_get_chars)
#define scheme_tell (scheme_extension_table->scheme_tell)
#define scheme_output_tell (scheme_extension_table->scheme_output_tell)
#define scheme_tell_line (scheme_extension_table->scheme_tell_line)
#define scheme_tell_column (scheme_extension_table->scheme_tell_column)
#define scheme_count_lines (scheme_extension_table->scheme_count_lines)
#define scheme_close_input_port (scheme_extension_table->scheme_close_input_port)
#define scheme_close_output_port (scheme_extension_table->scheme_close_output_port)
#define scheme_make_port_type (scheme_extension_table->scheme_make_port_type)
#define scheme_make_input_port (scheme_extension_table->scheme_make_input_port)
#define scheme_make_output_port (scheme_extension_table->scheme_make_output_port)
#define scheme_open_input_file (scheme_extension_table->scheme_open_input_file)
#define scheme_open_output_file (scheme_extension_table->scheme_open_output_file)
#define scheme_make_file_input_port (scheme_extension_table->scheme_make_file_input_port)
#define scheme_make_named_file_input_port (scheme_extension_table->scheme_make_named_file_input_port)
#define scheme_make_file_output_port (scheme_extension_table->scheme_make_file_output_port)
#define scheme_make_string_input_port (scheme_extension_table->scheme_make_string_input_port)
#define scheme_make_sized_string_input_port (scheme_extension_table->scheme_make_sized_string_input_port)
#define scheme_make_string_output_port (scheme_extension_table->scheme_make_string_output_port)
#define scheme_get_string_output (scheme_extension_table->scheme_get_string_output)
#define scheme_get_sized_string_output (scheme_extension_table->scheme_get_sized_string_output)
#define scheme_pipe (scheme_extension_table->scheme_pipe)
#define scheme_pipe_with_limit (scheme_extension_table->scheme_pipe_with_limit)
#define scheme_set_file_position (scheme_extension_table->scheme_set_file_position)
#define scheme_file_exists (scheme_extension_table->scheme_file_exists)
#define scheme_directory_exists (scheme_extension_table->scheme_directory_exists)
#define scheme_expand_filename (scheme_extension_table->scheme_expand_filename)
#define scheme_os_getcwd (scheme_extension_table->scheme_os_getcwd)
#define scheme_os_setcwd (scheme_extension_table->scheme_os_setcwd)
#define scheme_getdrive (scheme_extension_table->scheme_getdrive)
#define scheme_split_pathname (scheme_extension_table->scheme_split_pathname)
#define scheme_build_pathname (scheme_extension_table->scheme_build_pathname)
#ifdef MACINTOSH_EVENTS
#define scheme_mac_spec_to_path (scheme_extension_table->scheme_mac_spec_to_path)
#define scheme_mac_path_to_spec (scheme_extension_table->scheme_mac_path_to_spec)
#endif
#define scheme_alloc_fdset_array (scheme_extension_table->scheme_alloc_fdset_array)
#define scheme_init_fdset_array (scheme_extension_table->scheme_init_fdset_array)
#define scheme_get_fdset (scheme_extension_table->scheme_get_fdset)
#define scheme_fdzero (scheme_extension_table->scheme_fdzero)
#define scheme_fdset (scheme_extension_table->scheme_fdset)
#define scheme_fdclr (scheme_extension_table->scheme_fdclr)
#define scheme_fdisset (scheme_extension_table->scheme_fdisset)
#define scheme_add_fd_handle (scheme_extension_table->scheme_add_fd_handle)
#define scheme_add_fd_eventmask (scheme_extension_table->scheme_add_fd_eventmask)
#define scheme_security_check_file (scheme_extension_table->scheme_security_check_file)
#define scheme_security_check_network (scheme_extension_table->scheme_security_check_network)
#define scheme_get_host_address (scheme_extension_table->scheme_get_host_address)
#define scheme_make_namespace (scheme_extension_table->scheme_make_namespace)
#define scheme_add_namespace_option (scheme_extension_table->scheme_add_namespace_option)
#define scheme_require_from_original_env (scheme_extension_table->scheme_require_from_original_env)
#define scheme_add_global (scheme_extension_table->scheme_add_global)
#define scheme_add_global_symbol (scheme_extension_table->scheme_add_global_symbol)
#define scheme_make_envunbox (scheme_extension_table->scheme_make_envunbox)
#define scheme_lookup_global (scheme_extension_table->scheme_lookup_global)
#define scheme_global_bucket (scheme_extension_table->scheme_global_bucket)
#define scheme_global_keyword_bucket (scheme_extension_table->scheme_global_keyword_bucket)
#define scheme_module_bucket (scheme_extension_table->scheme_module_bucket)
#define scheme_exptime_global_bucket (scheme_extension_table->scheme_exptime_global_bucket)
#define scheme_exptime_module_bucket (scheme_extension_table->scheme_exptime_module_bucket)
#define scheme_builtin_value (scheme_extension_table->scheme_builtin_value)
#define scheme_set_global_bucket (scheme_extension_table->scheme_set_global_bucket)
#define scheme_install_macro (scheme_extension_table->scheme_install_macro)
#define scheme_save_initial_module_set (scheme_extension_table->scheme_save_initial_module_set)
#define scheme_primitive_module (scheme_extension_table->scheme_primitive_module)
#define scheme_finish_primitive_module (scheme_extension_table->scheme_finish_primitive_module)
#define scheme_make_modidx (scheme_extension_table->scheme_make_modidx)
#define scheme_declare_module (scheme_extension_table->scheme_declare_module)
#define scheme_intern_symbol (scheme_extension_table->scheme_intern_symbol)
#define scheme_intern_exact_symbol (scheme_extension_table->scheme_intern_exact_symbol)
#define scheme_make_symbol (scheme_extension_table->scheme_make_symbol)
#define scheme_make_exact_symbol (scheme_extension_table->scheme_make_exact_symbol)
#define scheme_symbol_name (scheme_extension_table->scheme_symbol_name)
#define scheme_symbol_name_and_size (scheme_extension_table->scheme_symbol_name_and_size)
#define scheme_symbol_val (scheme_extension_table->scheme_symbol_val)
#define scheme_make_struct_values (scheme_extension_table->scheme_make_struct_values)
#define scheme_make_struct_names (scheme_extension_table->scheme_make_struct_names)
#define scheme_make_struct_type (scheme_extension_table->scheme_make_struct_type)
#define scheme_make_struct_instance (scheme_extension_table->scheme_make_struct_instance)
#define scheme_make_struct_exptime (scheme_extension_table->scheme_make_struct_exptime)
#define scheme_is_struct_instance (scheme_extension_table->scheme_is_struct_instance)
#define scheme_struct_ref (scheme_extension_table->scheme_struct_ref)
#define scheme_struct_set (scheme_extension_table->scheme_struct_set)
#define scheme_make_struct_type_property (scheme_extension_table->scheme_make_struct_type_property)
#define scheme_make_struct_type_property_w_guard (scheme_extension_table->scheme_make_struct_type_property_w_guard)
#define scheme_struct_type_property_ref (scheme_extension_table->scheme_struct_type_property_ref)
#define scheme_eq (scheme_extension_table->scheme_eq)
#define scheme_eqv (scheme_extension_table->scheme_eqv)
#define scheme_equal (scheme_extension_table->scheme_equal)
#ifdef MZ_PRECISE_GC
#define scheme_hash_key (scheme_extension_table->scheme_hash_key)
#endif
#define scheme_equal_hash_key (scheme_extension_table->scheme_equal_hash_key)
#define scheme_equal_hash_key2 (scheme_extension_table->scheme_equal_hash_key2)
#define scheme_build_list (scheme_extension_table->scheme_build_list)
#define scheme_make_list_immutable (scheme_extension_table->scheme_make_list_immutable)
#define scheme_list_length (scheme_extension_table->scheme_list_length)
#define scheme_proper_list_length (scheme_extension_table->scheme_proper_list_length)
#define scheme_alloc_list (scheme_extension_table->scheme_alloc_list)
#define scheme_map_1 (scheme_extension_table->scheme_map_1)
#define scheme_car (scheme_extension_table->scheme_car)
#define scheme_cdr (scheme_extension_table->scheme_cdr)
#define scheme_cadr (scheme_extension_table->scheme_cadr)
#define scheme_caddr (scheme_extension_table->scheme_caddr)
#define scheme_vector_to_list (scheme_extension_table->scheme_vector_to_list)
#define scheme_list_to_vector (scheme_extension_table->scheme_list_to_vector)
#define scheme_append (scheme_extension_table->scheme_append)
#define scheme_box (scheme_extension_table->scheme_box)
#define scheme_unbox (scheme_extension_table->scheme_unbox)
#define scheme_set_box (scheme_extension_table->scheme_set_box)
#define scheme_make_weak_box (scheme_extension_table->scheme_make_weak_box)
#define scheme_load (scheme_extension_table->scheme_load)
#define scheme_load_extension (scheme_extension_table->scheme_load_extension)
#define scheme_register_extension_global (scheme_extension_table->scheme_register_extension_global)
#define scheme_get_seconds (scheme_extension_table->scheme_get_seconds)
#define scheme_get_milliseconds (scheme_extension_table->scheme_get_milliseconds)
#define scheme_get_process_milliseconds (scheme_extension_table->scheme_get_process_milliseconds)
#define scheme_banner (scheme_extension_table->scheme_banner)
#define scheme_version (scheme_extension_table->scheme_version)
#define scheme_check_proc_arity (scheme_extension_table->scheme_check_proc_arity)
#define scheme_make_provided_string (scheme_extension_table->scheme_make_provided_string)
#define scheme_make_args_string (scheme_extension_table->scheme_make_args_string)
#define scheme_no_dumps (scheme_extension_table->scheme_no_dumps)
#define scheme_system_library_subpath (scheme_extension_table->scheme_system_library_subpath)
#define scheme_signal_received (scheme_extension_table->scheme_signal_received)
