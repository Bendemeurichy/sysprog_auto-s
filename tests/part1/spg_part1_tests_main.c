#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>

#include "min_unit.h"
#include "test_binary_game_levels.h"


static char * all_tests() {
    mu_run_test(test_read_level_0);
    mu_run_test(test_read_level_1);
    mu_run_test(test_read_level_2);
    mu_run_test(test_read_level_3);
    mu_run_test(test_read_level_demo);

    mu_run_test(test_write_level_0_nosame);
    mu_run_test(test_write_level_1_nosame);
    mu_run_test(test_write_level_2_nosame);
    mu_run_test(test_write_level_3_nosame);
    mu_run_test(test_write_level_demo_nosame);

    mu_run_test(test_write_level_0);
    mu_run_test(test_write_level_1);
    mu_run_test(test_write_level_2);
    mu_run_test(test_write_level_3);
    mu_run_test(test_write_level_demo);

    return 0;
}


int main() {
    printf("Running tests:\n\n");
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
        printf("\nAT LEAST ONE TEST FAILED\n");
    }
    else {
        printf("\nALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", mu_tests_run);

#ifdef _WIN32
    system("pause");
#endif

    return result != 0;
}
