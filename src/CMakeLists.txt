add_library(httplib STATIC)

set_target_properties(
    httplib PROPERTIES
    C_EXTENSIONS ON
    C_STANDARD 11
    C_STANDARD_REQUIRED ON)

target_compile_definitions(
    httplib PUBLIC
    ALTERNATIVE_QUEUE
    # CRYPTOLIB=
    # HAVE_POLL
    # MAX_REQUEST_SIZE=16384
    # MD5_STATIC
    # MSG_NOSIGNAL
    $<$<NOT:$<BOOL:${HTTPLIB_ENABLE_SSL}>>:NO_SSL>
    # NO_THREAD_NAME
    # QUEUE_SIZE=50
    # SOMAXCONN=128
    # SSL_ALREADY_INITIALIZED
    # SSL_LIB=
    # TCP_USER_TIMEOUT=
    # USE_MASTER_THREAD_PRIORITY=
    USE_STACK_SIZE=102400)
target_compile_options(
    httplib PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wcast-align -Wcast-qual -Wconversion -Werror -Wextra -Wformat-security -Winit-self -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -funsigned-char>)
target_include_directories(
    httplib PUBLIC
    ${httplib_SOURCE_DIR}/include
    $<$<BOOL:${HTTPLIB_ENABLE_SSL}>:${OPENSSL_INCLUDE_DIR}>)
target_link_libraries(
    httplib PUBLIC
    $<$<BOOL:${HTTPLIB_ENABLE_SSL}>:${OPENSSL_LIBRARIES}>
    $<$<BOOL:${HTTPLIB_ENABLE_SSL}>:${ZLIB_LIBRARIES}>)
target_sources(
    httplib PRIVATE
	extern_md5.c
	extern_sha1.c
	httplib_abort_start.c
	httplib_accept_new_connection.c
    httplib_alloc_printf.c
	httplib_atomic_dec.c
	httplib_atomic_inc.c
	httplib_authorize.c
	httplib_base64_encode.c
	httplib_check_acl.c
	httplib_check_authorization.c
	httplib_check_feature.c
	httplib_check_password.c
	httplib_close_all_listening_sockets.c
	httplib_close_connection.c
	httplib_close_socket_gracefully.c
	httplib_closedir.c
	httplib_compare_dir_entries.c
	httplib_connect_client.c
	httplib_connect_socket.c
	httplib_connect_websocket_client.c
	httplib_construct_etag.c
	httplib_consume_socket.c
	httplib_create_client_context.c
	httplib_cry.c
	httplib_delete_file.c
	httplib_destroy_client_context.c
	httplib_difftimespec.c
	httplib_dir_scan_callback.c
	httplib_discard_unread_request_data.c
	httplib_download.c
	httplib_error_string.c
	httplib_event_queue.c
	httplib_fclose_on_exec.c
	httplib_fclose.c
	httplib_fgets.c
	httplib_fopen.c
	httplib_forward_body_data.c
	httplib_free_config_options.c
	httplib_free_context.c
	httplib_get_builtin_mime_type.c
	httplib_get_cookie.c
	httplib_get_debug_level.c
	httplib_get_first_ssl_listener_index.c
	httplib_get_header.c
	httplib_get_mime_type.c
	httplib_get_option.c
	httplib_get_random.c
	httplib_get_rel_url_at_current_server.c
	httplib_get_remote_ip.c
	httplib_get_request_handler.c
	httplib_get_request_info.c
	httplib_get_request_len.c
	httplib_get_response_code_text.c
	httplib_get_response.c
	httplib_get_server_ports.c
	httplib_get_system_name.c
	httplib_get_uri_type.c
	httplib_get_user_connection_data.c
	httplib_get_user_data.c
	httplib_get_var.c
	httplib_getreq.c
	httplib_global_data.c
	httplib_gmt_time_string.c
	httplib_gmtime_r.c
	httplib_handle_directory_request.c
	httplib_handle_file_based_request.c
	httplib_handle_form_request.c
	httplib_handle_not_modified_static_file_request.c
	httplib_handle_propfind.c
	httplib_handle_request.c
	httplib_handle_static_file_request.c
	httplib_handle_websocket_request.c
	httplib_header_has_option.c
	httplib_inet_pton.c
	httplib_init_options.c
	httplib_initialize_ssl.c
	httplib_interpret_uri.c
	httplib_ipt_to_ip.c
	httplib_ipt_to_ip4.c
	httplib_ipt_to_ip6.c
	httplib_is_authorized_for_put.c
	httplib_is_file_in_memory.c
	httplib_is_file_opened.c
	httplib_is_not_modified.c
	httplib_is_put_or_delete_method.c
	httplib_is_valid_http_method.c
	httplib_is_valid_port.c
	httplib_is_websocket_protocol.c
	httplib_kill.c
	httplib_localtime_r.c
	httplib_lock_unlock_connection.c
	httplib_lock_unlock_context.c
	httplib_log_access.c
	httplib_lowercase.c
	httplib_malloc.c
	httplib_mask_data.c
	httplib_master_thread.c
	httplib_match_prefix.c
	httplib_md5.c
	httplib_mkcol.c
	httplib_mkdir.c
	httplib_modify_passwords_file.c
	httplib_must_hide_file.c
	httplib_next_option.c
	httplib_open_auth_file.c
	httplib_opendir.c
	httplib_option_value_to_bool.c
	httplib_option_value_to_int.c
	httplib_parse_auth_header.c
	httplib_parse_date_string.c
	httplib_parse_http_headers.c
	httplib_parse_http_message.c
	httplib_parse_net.c
    httplib_parse_port_string.c
	httplib_parse_range_header.c
	httplib_path_to_unicode.c
	httplib_poll.c
	httplib_print_dir_entry.c
	httplib_printf.c
	httplib_process_new_connection.c
	httplib_process_options.c
	httplib_produce_socket.c
	httplib_pthread_cond_broadcast.c
	httplib_pthread_cond_destroy.c
	httplib_pthread_cond_init.c
	httplib_pthread_cond_signal.c
	httplib_pthread_cond_timedwait.c
	httplib_pthread_cond_wait.c
	httplib_pthread_getspecific.c
	httplib_pthread_join.c
	httplib_pthread_key_create.c
	httplib_pthread_key_delete.c
	httplib_pthread_mutex_destroy.c
	httplib_pthread_mutex_init.c
	httplib_pthread_mutex_lock.c
	httplib_pthread_mutex_trylock.c
	httplib_pthread_mutex_unlock.c
	httplib_pthread_self.c
	httplib_pthread_setspecific.c
	httplib_pull_all.c
	httplib_pull.c
	httplib_push_all.c
	httplib_put_dir.c
	httplib_put_file.c
	httplib_read_auth_file.c
	httplib_read_request.c
	httplib_read_websocket.c
	httplib_read.c
	httplib_readdir.c
	httplib_redirect_to_https_port.c
	httplib_refresh_trust.c
	httplib_remove_bad_file.c
	httplib_remove_directory.c
	httplib_remove_double_dots.c
	httplib_remove.c
	httplib_reset_per_request_attributes.c
	httplib_scan_directory.c
	httplib_send_authorization_request.c
	httplib_send_file_data.c
	httplib_send_file.c
	httplib_send_http_error.c
	httplib_send_no_cache_header.c
	httplib_send_options.c
	httplib_send_static_cache_header.c
	httplib_send_websocket_handshake.c
	httplib_set_acl_option.c
	httplib_set_auth_handler.c
	httplib_set_close_on_exec.c
	httplib_set_debug_level.c
	httplib_set_gpass_option.c
	httplib_set_handler_type.c
	httplib_set_non_blocking_mode.c
	httplib_set_ports_option.c
	httplib_set_request_handler.c
	httplib_set_sock_timeout.c
	httplib_set_ssl_option.c
	httplib_set_tcp_nodelay.c
	httplib_set_thread_name.c
	httplib_set_uid_option.c
	httplib_set_user_connection_data.c
	httplib_set_websocket_handler.c
	httplib_should_decode_url.c
	httplib_should_keep_alive.c
	httplib_skip_quoted.c
	httplib_skip.c
	httplib_snprintf.c
	httplib_sockaddr_to_string.c
	httplib_ssl_error.c
	httplib_ssl_get_client_cert_info.c
	httplib_ssl_get_protocol.c
	httplib_ssl_use_pem_file.c
	httplib_sslize.c
	httplib_start_thread_with_id.c
	httplib_start_thread.c
	httplib_start.c
	httplib_stat.c
	httplib_stop.c
	httplib_store_body.c
	httplib_strcasecmp.c
	httplib_strcasestr.c
	httplib_strdup.c
	httplib_strlcpy.c
	httplib_strncasecmp.c
	httplib_strndup.c
	httplib_substitute_index_file.c
	httplib_suggest_connection_header.c
	httplib_system_exit.c
	httplib_system_init.c
	httplib_tls_dtor.c
	httplib_uninitialize_ssl.c
	httplib_url_decode.c
	httplib_url_encode.c
	httplib_version.c
	httplib_vprintf.c
	httplib_vsnprintf.c
	httplib_websocket_client_thread.c
	httplib_websocket_client_write.c
	httplib_websocket_write_exec.c
	httplib_websocket_write.c
	httplib_worker_thread.c
	httplib_write.c
	osx_clock_gettime.c
	win32_clock_gettime.c)
