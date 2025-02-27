#include "calculator.h"

int main(int argc, char** argv)
{

    for (int i = 1; i < argc; ++i)
        if (!strcmp(argv[i], "--float"))
            flag_float = 1;

    char input[STACK_SIZE];
    if (!fgets(input, sizeof(input), stdin))
        return 1;
    input[strcspn(input, "\n")] = 0;

    if (!validate_input(input))
        return 1;

    Number res = evaluate(input);
    flag_float ? printf("%.4f\n", res.float_val) : printf("%ld\n", res.int_val);

    return 0;
}
