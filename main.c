#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *digits;
    int length;
} BigInt;

BigInt *createBigInt(const char *str) {
    BigInt *bigInt = (BigInt *) malloc(sizeof(BigInt));
    if (!bigInt) {
        printf("err: mem alloc err.\n");
        exit(1);
    }
    bigInt->length = strlen(str);
    bigInt->digits = (char *) malloc(bigInt->length + 1);
    if (!bigInt->digits) {
        printf("err: mem alloc err.\n");
        free(bigInt);
        exit(1);
    }
    strcpy(bigInt->digits, str);
    return bigInt;
}

void freeBigInt(BigInt *bigInt) {
    if (bigInt) {
        free(bigInt->digits);
        free(bigInt);
    }
}

int numberValidator(const char *str) {
    if (*str == '\0') return 0;
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

BigInt *multiplier(BigInt *a, BigInt *b) {
    int resultLength = a->length + b->length;
    int *result = (int *) calloc(resultLength, sizeof(int));
    if (!result) {
        printf("err: mem alloc err for res.\n");
        exit(1);
    }

    for (int i = a->length - 1; i >= 0; i--) {
        for (int j = b->length - 1; j >= 0; j--) {
            const int mul = (a->digits[i] - '0') * (b->digits[j] - '0');
            const int sum = mul + result[i + j + 1];

            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }

    int start = 0;
    while (start < resultLength && result[start] == 0) {
        start++;
    }

    BigInt *product = (BigInt *) malloc(sizeof(BigInt));
    if (!product) {
        printf("err: mem alloc err.\n");
        free(result);
        exit(1);
    }

    product->length = resultLength - start;
    if (product->length == 0) {
        product->length = 1;
        product->digits = strdup("0");
    } else {
        product->digits = (char *) malloc(product->length + 1);
        if (!product->digits) {
            printf("err: mem alloc err.\n");
            free(product);
            free(result);
            exit(1);
        }
        for (int i = 0; i < product->length; i++) {
            product->digits[i] = result[start + i] + '0';
        }
        product->digits[product->length] = '\0';
    }

    free(result);
    return product;
}

void handleError(const char *message) {
    printf("%s\n", message);
    exit(1);
}

void inputValidator(BigInt **a, BigInt **b) {
    char input[1024];
    printf("Enter 2 numbers (use space)\n");
    if (!fgets(input, sizeof(input), stdin)) {
        handleError("err: input");
    }

    char *firstNumber = strtok(input, " ");
    char *secondNumber = strtok(NULL, " ");

    if (!firstNumber || !secondNumber || strtok(NULL, " ")) {
        handleError("Only 2 numbers");
    }

    if (!numberValidator(firstNumber) || !numberValidator(secondNumber)) {
        handleError("err: in format");
    }

    *a = createBigInt(firstNumber);
    *b = createBigInt(secondNumber);
}

int main() {
    BigInt *a = NULL, *b = NULL;

    inputValidator(&a, &b);

    BigInt *result = multiplier(a, b);

    printf("res: %s\n", result->digits);

    freeBigInt(a);
    freeBigInt(b);
    freeBigInt(result);

    return 0;
}
