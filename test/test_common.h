#pragma once

extern "C" {
#include "otai.h"
}

using namespace std;

#define UNREFERENCED_PARAMETER(P)   (P)

typedef struct _otai_test_u32_data_t {
    int otai_attr_id;
    otai_uint32_t otai_attr_val;
    otai_status_t status;
} otai_test_u32_data_t;

typedef struct _otai_test_d64_data_t {
    int otai_attr_id;
    otai_double_t otai_attr_val;
    otai_status_t status;
} otai_test_d64_data_t;

typedef struct _otai_test_char_data_t {
    int otai_attr_id;
    char otai_attr_val[512];
    otai_status_t status;
} otai_test_char_data_t;

typedef struct _otai_test_bool_data_t {
    int otai_attr_id;
    bool otai_attr_val;
    otai_status_t status;
} otai_test_bool_data_t;

typedef struct _otai_get_attr_t {
    int otai_attr_id;
    otai_status_t status;
} otai_get_attr_t;

typedef struct _otai_get_stat_t {
    int otai_stat_id;
    otai_status_t status;
} otai_get_stat_t;

typedef enum _log_level_t {
    ERROR, 
    WARNING, 
    INFO, 
    DEBUG
}log_level_t;

class Logger
{
public:
    Logger(log_level_t _loglevel = ERROR) {
        _buffer << log_levels[_loglevel] << ": ";
    }

    template <typename T>
    Logger & operator<<(T const & value) {
        _buffer << value;
        return *this;
    }

    ~Logger() {
        _buffer << std::endl;
        std::cerr << _buffer.str();
    }

private:
    std::ostringstream _buffer;
    string log_levels[4] = {"ERROR", "WARNING", "INFO", "DEBUG"};

};

extern log_level_t gLoglevel;

#define Logg(level) \
if (level > gLoglevel) ; \
else Logger(level)


