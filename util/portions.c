//
// Created by cheyao on 14.11.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Round: needs at least one argument\n");
        return 1;
    }

    long long l = atol(argv[1]);
    printf("%lld\n", (l - l % 513) / 512 + 1);

    return 0;
}
