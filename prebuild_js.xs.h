/* XS GENERATED FILE; DO NOT EDIT! */

#include "xs.h"

#ifdef __cplusplus
extern "C" {
#endif
extern void xs_console_log(xsMachine* the);
extern void xs_timer_set(xsMachine* the);
extern void xs_timer_repeat(xsMachine* the);
extern void xs_timer_schedule(xsMachine* the);
extern void xs_timer_clear(xsMachine* the);
extern void xs_timer_delay(xsMachine* the);
extern void dom_element_dtor(void* data);
extern void dom_element_ctor(xsMachine* the);
extern void dom_element_set_tag_name(xsMachine* the);
extern void dom_element_get_tag_name(xsMachine* the);
extern void dom_element_set_left(xsMachine* the);
extern void dom_element_get_left(xsMachine* the);
extern void dom_element_set_top(xsMachine* the);
extern void dom_element_get_top(xsMachine* the);
extern void dom_element_set_right(xsMachine* the);
extern void dom_element_get_right(xsMachine* the);
extern void dom_element_set_bottom(xsMachine* the);
extern void dom_element_get_bottom(xsMachine* the);
extern void dom_element_set_width(xsMachine* the);
extern void dom_element_get_width(xsMachine* the);
extern void dom_element_set_height(xsMachine* the);
extern void dom_element_get_height(xsMachine* the);
extern void dom_element_set_margins(xsMachine* the);
extern void dom_element_get_margins(xsMachine* the);
extern void dom_element_set_paddings(xsMachine* the);
extern void dom_element_get_paddings(xsMachine* the);
extern void dom_element_set_borders(xsMachine* the);
extern void dom_element_get_borders(xsMachine* the);
extern void dom_element_set_attr(xsMachine* the);
extern void dom_element_get_attr(xsMachine* the);
extern void dom_element_set_style(xsMachine* the);
extern void dom_element_get_style(xsMachine* the);
extern void dom_element_append_child(xsMachine* the);
extern void dom_element_remove_child(xsMachine* the);
extern void dom_element_get_parent(xsMachine* the);
#ifdef __cplusplus
}
#endif

#define xsID_Array -32756
#define xsID_ArrayBuffer -32755
#define xsID_AsyncFunction -32754
#define xsID_Atomics -32753
#define xsID_Boolean -32752
#define xsID_Chunk -32751
#define xsID_DataView -32750
#define xsID_Date -32749
#define xsID_Error -32748
#define xsID_EvalError -32747
#define xsID_Float32Array -32746
#define xsID_Float64Array -32745
#define xsID_Function -32744
#define xsID_Infinity -32743
#define xsID_Int16Array -32742
#define xsID_Int32Array -32741
#define xsID_Int8Array -32740
#define xsID_JSON -32739
#define xsID_Map -32738
#define xsID_Math -32737
#define xsID_NaN -32736
#define xsID_Number -32735
#define xsID_Object -32734
#define xsID_Promise -32733
#define xsID_Proxy -32732
#define xsID_RangeError -32731
#define xsID_ReferenceError -32730
#define xsID_Reflect -32729
#define xsID_RegExp -32728
#define xsID_Set -32727
#define xsID_SharedArrayBuffer -32726
#define xsID_String -32725
#define xsID_Symbol -32724
#define xsID_SyntaxError -32723
#define xsID_TypeError -32722
#define xsID_TypedArray -32721
#define xsID_URIError -32720
#define xsID_Uint16Array -32719
#define xsID_Uint32Array -32718
#define xsID_Uint8Array -32717
#define xsID_Uint8ClampedArray -32716
#define xsID_WeakMap -32715
#define xsID_WeakSet -32714
#define xsID_decodeURI -32713
#define xsID_decodeURIComponent -32712
#define xsID_encodeURI -32711
#define xsID_encodeURIComponent -32710
#define xsID_escape -32709
#define xsID_eval -32708
#define xsID_isFinite -32707
#define xsID_isNaN -32706
#define xsID_parseFloat -32705
#define xsID_parseInt -32704
#define xsID_require -32703
#define xsID_trace -32702
#define xsID_undefined -32701
#define xsID_unescape -32700
#define xsID___proto__ -32699
#define xsID_BYTES_PER_ELEMENT -32698
#define xsID_E -32697
#define xsID_EPSILON -32696
#define xsID_Generator -32695
#define xsID_GeneratorFunction -32694
#define xsID_LN10 -32693
#define xsID_LN2 -32692
#define xsID_LOG10E -32691
#define xsID_LOG2E -32690
#define xsID_MAX_SAFE_INTEGER -32689
#define xsID_MAX_VALUE -32688
#define xsID_MIN_SAFE_INTEGER -32687
#define xsID_MIN_VALUE -32686
#define xsID_NEGATIVE_INFINITY -32685
#define xsID_PI -32684
#define xsID_POSITIVE_INFINITY -32683
#define xsID_SQRT1_2 -32682
#define xsID_SQRT2 -32681
#define xsID_UTC -32680
#define xsID_abs -32679
#define xsID_acos -32678
#define xsID_acosh -32677
#define xsID_add -32676
#define xsID_aliases -32675
#define xsID_all -32674
#define xsID_and -32673
#define xsID_append -32672
#define xsID_apply -32671
#define xsID_arguments -32670
#define xsID_asin -32669
#define xsID_asinh -32668
#define xsID_assign -32667
#define xsID_atan -32666
#define xsID_atanh -32665
#define xsID_atan2 -32664
#define xsID_bind -32663
#define xsID_boundArguments -32662
#define xsID_boundFunction -32661
#define xsID_boundThis -32660
#define xsID_buffer -32659
#define xsID_busy -32658
#define xsID_byteLength -32657
#define xsID_byteOffset -32656
#define xsID_cache -32655
#define xsID_call -32654
#define xsID_callee -32653
#define xsID_caller -32652
#define xsID_catch -32651
#define xsID_cbrt -32650
#define xsID_ceil -32649
#define xsID_charAt -32648
#define xsID_charCodeAt -32647
#define xsID_chunk -32646
#define xsID_chunkify -32645
#define xsID_clear -32644
#define xsID_closure -32643
#define xsID_clz32 -32642
#define xsID_codePointAt -32641
#define xsID_compare -32640
#define xsID_compareExchange -32639
#define xsID_compile -32638
#define xsID_concat -32637
#define xsID_configurable -32636
#define xsID_console -32635
#define xsID_construct -32634
#define xsID_constructor -32633
#define xsID_copyWithin -32632
#define xsID_cos -32631
#define xsID_cosh -32630
#define xsID_count -32629
#define xsID_create -32628
#define xsID_default -32627
#define xsID_defineProperties -32626
#define xsID_defineProperty -32625
#define xsID_delete -32624
#define xsID_deleteProperty -32623
#define xsID_done -32622
#define xsID_dotAll -32621
#define xsID_eachDown -32620
#define xsID_eachUp -32619
#define xsID_endsWith -32618
#define xsID_entries -32617
#define xsID_enumerable -32616
#define xsID_enumerate -32615
#define xsID_every -32614
#define xsID_exchange -32613
#define xsID_exec -32612
#define xsID_exp -32611
#define xsID_expm1 -32610
#define xsID_exports -32609
#define xsID_fill -32608
#define xsID_filter -32607
#define xsID_find -32606
#define xsID_findIndex -32605
#define xsID_flags -32604
#define xsID_floor -32603
#define xsID_for -32602
#define xsID_forEach -32601
#define xsID_free -32600
#define xsID_freeze -32599
#define xsID_from -32598
#define xsID_fromArrayBuffer -32597
#define xsID_fromCharCode -32596
#define xsID_fromCodePoint -32595
#define xsID_fromString -32594
#define xsID_fround -32593
#define xsID_function -32592
#define xsID_get -32591
#define xsID_getDate -32590
#define xsID_getDay -32589
#define xsID_getFloat32 -32588
#define xsID_getFloat64 -32587
#define xsID_getFullYear -32586
#define xsID_getHours -32585
#define xsID_getInt16 -32584
#define xsID_getInt32 -32583
#define xsID_getInt8 -32582
#define xsID_getMilliseconds -32581
#define xsID_getMinutes -32580
#define xsID_getMonth -32579
#define xsID_getOwnPropertyDescriptor -32578
#define xsID_getOwnPropertyDescriptors -32577
#define xsID_getOwnPropertyNames -32576
#define xsID_getOwnPropertySymbols -32575
#define xsID_getPrototypeOf -32574
#define xsID_getSeconds -32573
#define xsID_getTime -32572
#define xsID_getTimezoneOffset -32571
#define xsID_getUTCDate -32570
#define xsID_getUTCDay -32569
#define xsID_getUTCFullYear -32568
#define xsID_getUTCHours -32567
#define xsID_getUTCMilliseconds -32566
#define xsID_getUTCMinutes -32565
#define xsID_getUTCMonth -32564
#define xsID_getUTCSeconds -32563
#define xsID_getUint16 -32562
#define xsID_getUint32 -32561
#define xsID_getUint8 -32560
#define xsID_getUint8Clamped -32559
#define xsID_getYear -32558
#define xsID_global -32557
#define xsID_groups -32556
#define xsID_has -32555
#define xsID_hasInstance -32554
#define xsID_hasOwnProperty -32553
#define xsID_hypot -32552
#define xsID_id -32551
#define xsID_ignoreCase -32550
#define xsID_import -32549
#define xsID_imul -32548
#define xsID_includes -32547
#define xsID_index -32546
#define xsID_indexOf -32545
#define xsID_input -32544
#define xsID_is -32543
#define xsID_isArray -32542
#define xsID_isConcatSpreadable -32541
#define xsID_isExtensible -32540
#define xsID_isFrozen -32539
#define xsID_isInteger -32538
#define xsID_isLockFree -32537
#define xsID_isPrototypeOf -32536
#define xsID_isSafeInteger -32535
#define xsID_isSealed -32534
#define xsID_isView -32533
#define xsID_iterable -32532
#define xsID_iterator -32531
#define xsID_join -32530
#define xsID_keyFor -32529
#define xsID_keys -32528
#define xsID_lastIndex -32527
#define xsID_lastIndexOf -32526
#define xsID_length -32525
#define xsID_line -32524
#define xsID_load -32523
#define xsID_local -32522
#define xsID_localeCompare -32521
#define xsID_log -32520
#define xsID_log1p -32519
#define xsID_log10 -32518
#define xsID_log2 -32517
#define xsID_map -32516
#define xsID_match -32515
#define xsID_max -32514
#define xsID_message -32513
#define xsID_min -32512
#define xsID_multiline -32511
#define xsID_name -32510
#define xsID_next -32508
#define xsID_normalize -32507
#define xsID_now -32506
#define xsID_of -32505
#define xsID_or -32504
#define xsID_ownKeys -32503
#define xsID_padEnd -32502
#define xsID_padStart -32501
#define xsID_parse -32500
#define xsID_path -32499
#define xsID_peek -32498
#define xsID_poke -32497
#define xsID_pop -32496
#define xsID_pow -32495
#define xsID_preventExtensions -32494
#define xsID_propertyIsEnumerable -32493
#define xsID_propertyIsScriptable -32492
#define xsID_prototype -32491
#define xsID_proxy -32490
#define xsID_push -32489
#define xsID_race -32488
#define xsID_random -32487
#define xsID_raw -32486
#define xsID_reduce -32485
#define xsID_reduceRight -32484
#define xsID_reject -32483
#define xsID_repeat -32482
#define xsID_replace -32481
#define xsID_resolve -32480
#define xsID_result -32479
#define xsID_return -32478
#define xsID_reverse -32477
#define xsID_revocable -32476
#define xsID_revoke -32475
#define xsID_round -32474
#define xsID_seal -32473
#define xsID_search -32472
#define xsID_serialize -32471
#define xsID_set -32470
#define xsID_setDate -32469
#define xsID_setFloat32 -32468
#define xsID_setFloat64 -32467
#define xsID_setFullYear -32466
#define xsID_setHours -32465
#define xsID_setInt16 -32464
#define xsID_setInt32 -32463
#define xsID_setInt8 -32462
#define xsID_setMilliseconds -32461
#define xsID_setMinutes -32460
#define xsID_setMonth -32459
#define xsID_setPrototypeOf -32458
#define xsID_setSeconds -32457
#define xsID_setTime -32456
#define xsID_setUTCDate -32455
#define xsID_setUTCFullYear -32454
#define xsID_setUTCHours -32453
#define xsID_setUTCMilliseconds -32452
#define xsID_setUTCMinutes -32451
#define xsID_setUTCMonth -32450
#define xsID_setUTCSeconds -32449
#define xsID_setUint16 -32448
#define xsID_setUint32 -32447
#define xsID_setUint8 -32446
#define xsID_setUint8Clamped -32445
#define xsID_setYear -32444
#define xsID_shift -32443
#define xsID_sign -32442
#define xsID_sin -32441
#define xsID_sinh -32440
#define xsID_size -32439
#define xsID_slice -32438
#define xsID_some -32437
#define xsID_sort -32436
#define xsID_source -32435
#define xsID_species -32434
#define xsID_splice -32433
#define xsID_split -32432
#define xsID_sqrt -32431
#define xsID_startsWith -32430
#define xsID_sticky -32429
#define xsID_store -32428
#define xsID_stringify -32427
#define xsID_sub -32426
#define xsID_subarray -32425
#define xsID_substr -32424
#define xsID_substring -32423
#define xsID_tan -32422
#define xsID_tanh -32421
#define xsID_test -32420
#define xsID_then -32419
#define xsID_this -32418
#define xsID_throw -32417
#define xsID_toDateString -32416
#define xsID_toExponential -32415
#define xsID_toFixed -32414
#define xsID_toGMTString -32413
#define xsID_toISOString -32412
#define xsID_toJSON -32411
#define xsID_toLocaleDateString -32410
#define xsID_toLocaleLowerCase -32409
#define xsID_toLocaleString -32408
#define xsID_toLocaleTimeString -32407
#define xsID_toLocaleUpperCase -32406
#define xsID_toLowerCase -32405
#define xsID_toPrecision -32404
#define xsID_toPrimitive -32403
#define xsID_toString -32402
#define xsID_toStringTag -32401
#define xsID_toTimeString -32400
#define xsID_toUTCString -32399
#define xsID_toUpperCase -32398
#define xsID_transfers -32397
#define xsID_trim -32396
#define xsID_trunc -32395
#define xsID_unicode -32394
#define xsID_unscopables -32393
#define xsID_unshift -32392
#define xsID_uri -32391
#define xsID_value -32390
#define xsID_valueOf -32389
#define xsID_values -32388
#define xsID_wait -32387
#define xsID_wake -32386
#define xsID_weak -32385
#define xsID_writable -32384
#define xsID_xor -32383
#define xsID___dirname -32382
#define xsID___filename -32381
#define xsID_Console -32379
#define xsID_clearTimeout -32378
#define xsID_setTimeout -32377
#define xsID_Timer -32375
#define xsID_delay -32374
#define xsID_clearInterval -32373
#define xsID_setInterval -32372
#define xsID_callback -32371
#define xsID_schedule -32370
#define xsID_set_attr -32369
#define xsID_DOMElement -32368
#define xsID_height -32367
#define xsID_get_attr -32366
#define xsID_set_style -32365
#define xsID_bottom -32364
#define xsID_removeChild -32363
#define xsID_top -32362
#define xsID_parent -32361
#define xsID_paddings -32360
#define xsID_borders -32359
#define xsID_right -32358
#define xsID_width -32356
#define xsID_get_style -32355
#define xsID_margins -32354
#define xsID_tagName -32353
#define xsID_left -32352
#define xsID_appendChild -32351

