//
// Created by cheyao on 28.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef ASMOS_STDALIGN_H
#define ASMOS_STDALIGN_H

#if defined(__cplusplus)
extern "C" {
#endif

#define alignas _Alignas
#define alignof _Alignof
#define __alignas_is_defined 1
#define __alignof_is_defined 1

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif //ASMOS_STDALIGN_H
