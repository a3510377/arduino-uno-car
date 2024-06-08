#ifndef _UnoCar_H
#define _UnoCar_H

#include <Arduino.h>

#include "HC4067.h"

HC4067 mux(2, 3, 4, 12, A0);

#endif /* _UnoCar_H */
