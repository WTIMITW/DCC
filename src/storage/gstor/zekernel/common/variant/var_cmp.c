/*
 * Copyright (c) 2022 Huawei Technologies Co.,Ltd.
 *
 * openGauss is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * -------------------------------------------------------------------------
 *
 * var_cmp.c
 *    variant compare
 *
 * IDENTIFICATION
 *    src/storage/gstor/zekernel/common/variant/var_cmp.c
 *
 * -------------------------------------------------------------------------
 */
#include "var_cmp.h"
#include "var_opr.h"
#include "var_defs.h"

#define __CMP_TYPE_DEF(lt, rt, rst, st, compat) \
    [GS_TYPE_I((lt))][GS_TYPE_I((rt))] = {.cmp_type = (rst), .same_type = (st), .compatible = (compat)}

cmp_rule_t g_cmp_rules[VAR_TYPE_ARRAY_SIZE][VAR_TYPE_ARRAY_SIZE] = {
    __CMP_TYPE_DEF(GS_TYPE_UINT32, GS_TYPE_UINT32,  GS_TYPE_UINT32, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_UINT32, GS_TYPE_INTEGER,  GS_TYPE_BIGINT, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_UINT32, GS_TYPE_BIGINT,   GS_TYPE_BIGINT, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_UINT32, GS_TYPE_REAL,     GS_TYPE_REAL, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_UINT32, GS_TYPE_NUMBER,   GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_UINT32, GS_TYPE_DECIMAL,  GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_UINT32, GS_TYPE_CHAR,     GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_UINT32, GS_TYPE_VARCHAR,  GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_UINT32, GS_TYPE_STRING,   GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_UINT32, GS_TYPE_BOOLEAN,  GS_TYPE_INTEGER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_UINT32, GS_TYPE_BINARY,     GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_UINT32, GS_TYPE_VARBINARY,  GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_INTEGER, GS_TYPE_UINT32,   GS_TYPE_BIGINT, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTEGER, GS_TYPE_INTEGER,  GS_TYPE_INTEGER, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTEGER, GS_TYPE_BIGINT,   GS_TYPE_BIGINT, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTEGER, GS_TYPE_REAL,     GS_TYPE_REAL, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTEGER, GS_TYPE_NUMBER,   GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTEGER, GS_TYPE_DECIMAL,  GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTEGER, GS_TYPE_CHAR,     GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTEGER, GS_TYPE_VARCHAR,  GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTEGER, GS_TYPE_STRING,   GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTEGER, GS_TYPE_BOOLEAN,  GS_TYPE_INTEGER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTEGER, GS_TYPE_BINARY,     GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTEGER, GS_TYPE_VARBINARY,  GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_BIGINT, GS_TYPE_UINT32, GS_TYPE_BIGINT, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BIGINT, GS_TYPE_INTEGER, GS_TYPE_BIGINT, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BIGINT, GS_TYPE_BIGINT, GS_TYPE_BIGINT, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BIGINT, GS_TYPE_REAL, GS_TYPE_REAL, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BIGINT, GS_TYPE_NUMBER, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BIGINT, GS_TYPE_DECIMAL, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BIGINT, GS_TYPE_CHAR, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BIGINT, GS_TYPE_VARCHAR, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BIGINT, GS_TYPE_STRING, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BIGINT, GS_TYPE_BOOLEAN, GS_TYPE_BIGINT, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BIGINT, GS_TYPE_BINARY, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BIGINT, GS_TYPE_VARBINARY, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_REAL, GS_TYPE_UINT32, GS_TYPE_REAL, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_REAL, GS_TYPE_INTEGER, GS_TYPE_REAL, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_REAL, GS_TYPE_BIGINT, GS_TYPE_REAL, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_REAL, GS_TYPE_REAL, GS_TYPE_REAL, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_REAL, GS_TYPE_NUMBER, GS_TYPE_REAL, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_REAL, GS_TYPE_DECIMAL, GS_TYPE_REAL, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_REAL, GS_TYPE_CHAR, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_REAL, GS_TYPE_VARCHAR, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_REAL, GS_TYPE_STRING, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_REAL, GS_TYPE_BINARY, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_REAL, GS_TYPE_VARBINARY, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_NUMBER, GS_TYPE_UINT32, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_NUMBER, GS_TYPE_INTEGER, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_NUMBER, GS_TYPE_BIGINT, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_NUMBER, GS_TYPE_REAL, GS_TYPE_REAL, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_NUMBER, GS_TYPE_NUMBER, GS_TYPE_NUMBER, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_NUMBER, GS_TYPE_DECIMAL, GS_TYPE_NUMBER, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_NUMBER, GS_TYPE_CHAR, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_NUMBER, GS_TYPE_VARCHAR, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_NUMBER, GS_TYPE_STRING, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_NUMBER, GS_TYPE_BINARY, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_NUMBER, GS_TYPE_VARBINARY, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_DECIMAL, GS_TYPE_UINT32, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DECIMAL, GS_TYPE_INTEGER, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DECIMAL, GS_TYPE_BIGINT, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DECIMAL, GS_TYPE_REAL, GS_TYPE_REAL, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DECIMAL, GS_TYPE_NUMBER, GS_TYPE_NUMBER, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DECIMAL, GS_TYPE_DECIMAL, GS_TYPE_NUMBER, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DECIMAL, GS_TYPE_CHAR, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DECIMAL, GS_TYPE_VARCHAR, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DECIMAL, GS_TYPE_STRING, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DECIMAL, GS_TYPE_BINARY, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DECIMAL, GS_TYPE_VARBINARY, GS_TYPE_NUMBER, GS_FALSE, GS_TRUE),

    // BINARY not support bool yet
    __CMP_TYPE_DEF(GS_TYPE_BOOLEAN, GS_TYPE_UINT32, GS_TYPE_UINT32, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_BOOLEAN, GS_TYPE_INTEGER, GS_TYPE_INTEGER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_BOOLEAN, GS_TYPE_BIGINT, GS_TYPE_BIGINT, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_BOOLEAN, GS_TYPE_CHAR, GS_TYPE_BOOLEAN, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BOOLEAN, GS_TYPE_VARCHAR, GS_TYPE_BOOLEAN, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BOOLEAN, GS_TYPE_STRING, GS_TYPE_BOOLEAN, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BOOLEAN, GS_TYPE_BOOLEAN, GS_TYPE_BOOLEAN, GS_TRUE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_UINT32, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_INTEGER, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_BIGINT, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_REAL, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_NUMBER, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_DECIMAL, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_CHAR, GS_TYPE_CHAR, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_VARCHAR, GS_TYPE_STRING, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_STRING, GS_TYPE_STRING, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_BINARY, GS_TYPE_BINARY, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_VARBINARY, GS_TYPE_VARBINARY, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_RAW, GS_TYPE_CHAR, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_DATE, GS_TYPE_DATE, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_BOOLEAN, GS_TYPE_BOOLEAN, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_INTERVAL_YM, GS_TYPE_INTERVAL_YM, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_INTERVAL_DS, GS_TYPE_INTERVAL_DS, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CHAR, GS_TYPE_ARRAY, GS_TYPE_ARRAY, GS_FALSE, GS_FALSE),

    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_INTEGER, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_BIGINT, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_REAL, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_NUMBER, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_DECIMAL, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_CHAR, GS_TYPE_STRING, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_VARCHAR, GS_TYPE_STRING, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_STRING, GS_TYPE_STRING, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_BINARY, GS_TYPE_BINARY, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_VARBINARY, GS_TYPE_VARBINARY, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_RAW, GS_TYPE_VARCHAR, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_DATE, GS_TYPE_DATE, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_BOOLEAN, GS_TYPE_BOOLEAN, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_INTERVAL_YM, GS_TYPE_INTERVAL_YM, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_INTERVAL_DS, GS_TYPE_INTERVAL_DS, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_VARCHAR, GS_TYPE_ARRAY, GS_TYPE_ARRAY, GS_FALSE, GS_FALSE),

    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_UINT32, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_INTEGER, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_BIGINT, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_REAL, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_NUMBER, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_DECIMAL, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_CHAR, GS_TYPE_STRING, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_VARCHAR, GS_TYPE_STRING, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_STRING, GS_TYPE_STRING, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_BINARY, GS_TYPE_BINARY, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_VARBINARY, GS_TYPE_VARBINARY, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_RAW, GS_TYPE_STRING, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_DATE, GS_TYPE_DATE, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_BOOLEAN, GS_TYPE_BOOLEAN, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_INTERVAL_YM, GS_TYPE_INTERVAL_YM, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_INTERVAL_DS, GS_TYPE_INTERVAL_DS, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_STRING, GS_TYPE_ARRAY, GS_TYPE_ARRAY, GS_FALSE, GS_FALSE),

    __CMP_TYPE_DEF(GS_TYPE_BINARY, GS_TYPE_CHAR, GS_TYPE_BINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BINARY, GS_TYPE_VARCHAR, GS_TYPE_BINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BINARY, GS_TYPE_STRING, GS_TYPE_BINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BINARY, GS_TYPE_BINARY, GS_TYPE_BINARY, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BINARY, GS_TYPE_VARBINARY, GS_TYPE_VARBINARY, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BINARY, GS_TYPE_RAW, GS_TYPE_BINARY, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_RAW, GS_TYPE_CHAR, GS_TYPE_CHAR, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_RAW, GS_TYPE_VARCHAR, GS_TYPE_VARCHAR, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_RAW, GS_TYPE_STRING, GS_TYPE_STRING, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_RAW, GS_TYPE_RAW, GS_TYPE_RAW, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_RAW, GS_TYPE_BINARY, GS_TYPE_BINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_RAW, GS_TYPE_VARBINARY, GS_TYPE_VARBINARY, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_BLOB, GS_TYPE_CHAR, GS_TYPE_VARBINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BLOB, GS_TYPE_VARCHAR, GS_TYPE_VARBINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BLOB, GS_TYPE_STRING, GS_TYPE_VARBINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BLOB, GS_TYPE_BINARY, GS_TYPE_VARBINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BLOB, GS_TYPE_RAW, GS_TYPE_RAW, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BLOB, GS_TYPE_VARBINARY, GS_TYPE_VARBINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_BLOB, GS_TYPE_BLOB, GS_TYPE_VARBINARY, GS_TRUE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_UINT32, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_INTEGER, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_BIGINT, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_REAL, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_NUMBER, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_DECIMAL, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_CHAR, GS_TYPE_STRING, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_VARCHAR, GS_TYPE_STRING, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_STRING, GS_TYPE_STRING, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_BINARY, GS_TYPE_BINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_VARBINARY, GS_TYPE_VARBINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_RAW, GS_TYPE_STRING, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_DATE, GS_TYPE_DATE, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_BOOLEAN, GS_TYPE_BOOLEAN, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_INTERVAL_YM, GS_TYPE_INTERVAL_YM, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_INTERVAL_DS, GS_TYPE_INTERVAL_DS, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_CLOB, GS_TYPE_STRING, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_CLOB, GS_TYPE_IMAGE, GS_TYPE_STRING, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_UINT32, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_INTEGER, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_BIGINT, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_REAL, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_NUMBER, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_DECIMAL, GS_TYPE_NUMBER, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_CHAR, GS_TYPE_STRING, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_VARCHAR, GS_TYPE_STRING, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_STRING, GS_TYPE_STRING, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_BINARY, GS_TYPE_BINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_VARBINARY, GS_TYPE_VARBINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_RAW, GS_TYPE_RAW, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_DATE, GS_TYPE_DATE, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_BOOLEAN, GS_TYPE_BOOLEAN, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_INTERVAL_YM, GS_TYPE_INTERVAL_YM, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_INTERVAL_DS, GS_TYPE_INTERVAL_DS, GS_FALSE, GS_FALSE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_CLOB, GS_TYPE_STRING, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_IMAGE, GS_TYPE_IMAGE, GS_TYPE_STRING, GS_TRUE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_VARBINARY, GS_TYPE_CHAR, GS_TYPE_VARBINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_VARBINARY, GS_TYPE_VARCHAR, GS_TYPE_VARBINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_VARBINARY, GS_TYPE_STRING, GS_TYPE_VARBINARY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_VARBINARY, GS_TYPE_BINARY, GS_TYPE_VARBINARY, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_VARBINARY, GS_TYPE_VARBINARY, GS_TYPE_VARBINARY, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_VARBINARY, GS_TYPE_RAW, GS_TYPE_VARBINARY, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_DATE, GS_TYPE_CHAR, GS_TYPE_DATE, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DATE, GS_TYPE_VARCHAR, GS_TYPE_DATE, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DATE, GS_TYPE_STRING, GS_TYPE_DATE, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DATE, GS_TYPE_DATE, GS_TYPE_DATE, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DATE, GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DATE, GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DATE, GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_DATE, GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP, GS_TYPE_CHAR, GS_TYPE_TIMESTAMP, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP, GS_TYPE_VARCHAR, GS_TYPE_TIMESTAMP, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP, GS_TYPE_STRING, GS_TYPE_TIMESTAMP, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP, GS_TYPE_DATE, GS_TYPE_TIMESTAMP, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_CHAR, GS_TYPE_TIMESTAMP, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_VARCHAR, GS_TYPE_TIMESTAMP, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_STRING, GS_TYPE_TIMESTAMP, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_DATE, GS_TYPE_TIMESTAMP, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ, GS_TYPE_CHAR, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ, GS_TYPE_VARCHAR, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ, GS_TYPE_STRING, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ, GS_TYPE_DATE, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_TZ, GS_TRUE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_CHAR, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_VARCHAR, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_STRING, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_DATE, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_TZ_FAKE, GS_TYPE_TIMESTAMP_LTZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_TZ, GS_TYPE_TIMESTAMP_TZ, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_LTZ, GS_TYPE_TIMESTAMP_LTZ, GS_TRUE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_INTERVAL_YM, GS_TYPE_CHAR, GS_TYPE_INTERVAL_YM, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTERVAL_YM, GS_TYPE_VARCHAR, GS_TYPE_INTERVAL_YM, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTERVAL_YM, GS_TYPE_STRING, GS_TYPE_INTERVAL_YM, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTERVAL_YM, GS_TYPE_INTERVAL_YM, GS_TYPE_INTERVAL_YM, GS_TRUE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_INTERVAL_DS, GS_TYPE_CHAR, GS_TYPE_INTERVAL_DS, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTERVAL_DS, GS_TYPE_VARCHAR, GS_TYPE_INTERVAL_DS, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTERVAL_DS, GS_TYPE_STRING, GS_TYPE_INTERVAL_DS, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_INTERVAL_DS, GS_TYPE_INTERVAL_DS, GS_TYPE_INTERVAL_DS, GS_TRUE, GS_TRUE),

    __CMP_TYPE_DEF(GS_TYPE_ARRAY, GS_TYPE_STRING, GS_TYPE_ARRAY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_ARRAY, GS_TYPE_CHAR, GS_TYPE_ARRAY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_ARRAY, GS_TYPE_VARCHAR, GS_TYPE_ARRAY, GS_FALSE, GS_TRUE),
    __CMP_TYPE_DEF(GS_TYPE_ARRAY, GS_TYPE_ARRAY, GS_TYPE_ARRAY, GS_TRUE, GS_TRUE),
};   // end g_cmp_rules

status_t var_like(variant_t *left, variant_t *right, bool32 *result, bool32 has_escape, char escape,
    charset_type_t type)
{
    text_t *l_text = NULL;
    text_t *r_text = NULL;

    l_text = VALUE_PTR(text_t, left);
    r_text = VALUE_PTR(text_t, right);

    if (has_escape) {
        int32 cmp_ret = 0;
        GS_RETURN_IFERR(CM_CHARSET_FUNC(type).escape_like(l_text->str, l_text->str + l_text->len,
            r_text->str, r_text->str + r_text->len, escape, &cmp_ret));
        *result = (cmp_ret == 0);
        return GS_SUCCESS;
    }
    *result = CM_CHARSET_FUNC(type).like(l_text, r_text);
    return GS_SUCCESS;
}

static status_t var_compare_diff_type(const nlsparams_t *nls, 
    variant_t *left, variant_t *right, int32 *result)
{
    char       buf[GS_STRING_BUFFER_SIZE];
    text_buf_t convert_buf;
    variant_t  l_var, r_var;

    gs_type_t  cmp_type = get_cmp_datatype(left->type, right->type);
    if (cmp_type == INVALID_CMP_DATATYPE) {
        GS_SET_ERROR_MISMATCH(left->type, right->type);
        return GS_ERROR;
    }

    CM_INIT_TEXTBUF(&convert_buf, GS_STRING_BUFFER_SIZE, buf);
    var_copy(left, &l_var);
    var_copy(right, &r_var);

    // !!!only one variant need use converting buffer
    GS_RETURN_IFERR(var_convert(nls, &l_var, cmp_type, &convert_buf));
    GS_RETURN_IFERR(var_convert(nls, &r_var, cmp_type, &convert_buf));

    return var_compare_same_type(&l_var, &r_var, result);
}

status_t var_compare(const nlsparams_t *nls, variant_t *left, variant_t *right, int32 *result)
{
    if (left->is_null) {
        *result = 1;
        return GS_SUCCESS;
    } else if (right->is_null) {
        *result = -1;
        return GS_SUCCESS;
    }
    
    if (SECUREC_UNLIKELY(GS_IS_UNKNOWN_TYPE(left->type) || GS_IS_UNKNOWN_TYPE(right->type))) {
        GS_THROW_ERROR(ERR_INVALID_DATA_TYPE, "comparision");
        return GS_ERROR;
    }

    if (left->type == right->type || get_cmp_rule(left->type, right->type)->same_type) {
        return var_compare_same_type(left, right, result);
    }

    return var_compare_diff_type(nls, left, right, result);
}

bool32 var_const_equal(const variant_t *v1, const variant_t *v2)
{
    if (v1->type != v2->type || v1->is_null != v2->is_null) {
        return GS_FALSE;
    } else if (v1->is_null == GS_TRUE && v2->is_null == GS_TRUE) {
        return GS_TRUE;
    }

    switch (v1->type) {
        case GS_TYPE_UINT32:
        case GS_TYPE_INTEGER:
        case GS_TYPE_INTERVAL_YM:
            return (v1->v_int == v2->v_int);

        case GS_TYPE_BIGINT:
        case GS_TYPE_INTERVAL_DS:
            return (v1->v_bigint == v2->v_bigint);

        case GS_TYPE_REAL:
            return (fabs(v1->v_real - v2->v_real) < GS_REAL_PRECISION);

        case GS_TYPE_NUMBER:
        case GS_TYPE_DECIMAL:
            return (cm_dec8_cmp(&v1->v_dec, &v2->v_dec) == 0);

        case GS_TYPE_DATE:
        case GS_TYPE_TIMESTAMP_TZ_FAKE:
        case GS_TYPE_TIMESTAMP_LTZ:
        case GS_TYPE_INTERVAL:
            return (v1->v_date == v2->v_date);

        case GS_TYPE_TIMESTAMP_TZ:
            return (cm_tstz_cmp(VALUE_PTR(timestamp_tz_t, v1), VALUE_PTR(timestamp_tz_t, v2)) == 0);

        case GS_TYPE_TIMESTAMP:
            return (v1->v_tstamp == v2->v_tstamp);

        case GS_TYPE_CHAR:
        case GS_TYPE_VARCHAR:
        case GS_TYPE_STRING:
            return cm_text_equal(&v1->v_text, &v2->v_text);

        case GS_TYPE_BOOLEAN:
            return (v1->v_bool == v2->v_bool);

        case GS_TYPE_TYPMODE:
            return (memcmp(&v1->v_type, &v2->v_type, sizeof(typmode_t)) == 0);

        case GS_TYPE_ITVL_UNIT:
            return (v1->v_itvl_unit_id == v2->v_itvl_unit_id);

        default:
            return GS_FALSE;
    }
}

bool32 var_seq_equal(const var_seq_t *v1, const var_seq_t *v2)
{
    if (cm_text_equal_ins(&v1->user, &v2->user) && cm_text_equal(&v1->name, &v2->name) && v1->mode == v2->mode) {
        return GS_TRUE;
    }
    return GS_FALSE;
}
