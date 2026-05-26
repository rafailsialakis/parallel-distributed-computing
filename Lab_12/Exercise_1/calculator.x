/*
 * calculator.x - RPC Interface Definition for calculator service
 */

program CALCULATOR_PROG {
    version CALCULATOR_VERS {
        int ADD(intpair) = 1;
        int SUBTRACT(intpair) = 2;
        int MULTIPLY(intpair) = 3;
        float DIVIDE(intpair) = 4;  /* Division might result in a float */
    } = 1;
} = 0x20000001;

struct intpair {
    int a;
    int b;
};
