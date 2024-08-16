#include <gtest/gtest.h>
#include <test_common.h>

extern "C" {
#include "otai.h"
}

extern void test_linecard();
extern void test_port();
extern void test_oa();
extern void test_transceiver();
extern void test_osc();
extern void test_aps();

log_level_t gLoglevel = INFO;

void print_usage(char *argv) {
    printf("Usage: %s -d <error|warning|info|debug>\n default debug level is <info>\n", argv);
}

int init_log_level(int argc, char **argv) {
    if (argc == 1 )
    {
        gLoglevel = INFO;
        return 0;
    }
    else if (argc == 2 )
    {
        if ((std::string(argv[1]) == "?") ||
                (std::string(argv[1]) == "-help") ||
                (std::string(argv[1]) == "-h") )
        {
            print_usage(argv[0]);
            return 1;
        }
    }
    else if (argc == 3)
    {
        if (std::string(argv[1]) == "-d")
        {
            if (std::string(argv[2]) == "error")
            {
                gLoglevel = ERROR;
            }
            else if (std::string(argv[2]) == "warning")
            {
                gLoglevel = WARNING;
            }
            else if (std::string(argv[2]) == "info")
            {
                gLoglevel = INFO;
            }
            else if (std::string(argv[2]) == "debug")
            {
                gLoglevel = DEBUG;
            }
            else
            {
                print_usage(argv[0]);
                return 1;
            }
            return 0;
        }
    }

    print_usage(argv[0]);
    return 1;
}

TEST(OtaiUnitTest, test_otn_otai) {
    test_linecard();
    test_port();
    test_oa();
    test_transceiver();
    test_osc();
    test_aps();
}

int main(int argc, char **argv) {
    if (init_log_level(argc, argv)) {
        return 1;
    }

    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}



