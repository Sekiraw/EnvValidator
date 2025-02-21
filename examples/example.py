# SPDX-License-Identifier: Apache-2.0
# Author: Qiyaya

import envvalidator

schema = {
    "DATABASE_URL": "string",
    "DEBUG": "bool",
    "PORT": "int",
    "USERNAME": { "regex": "^[a-zA-Z0-9_]+$" }
}

try:
    ENV = envvalidator.validate_env(".env.example", schema)
    print("Validated environment:", ENV)
except Exception as e:
    print("Error:", e)
