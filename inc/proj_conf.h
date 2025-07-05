#ifndef PROJ_CONF_H
#define PROJ_CONF_H

// #define DEBUG_MODE

#ifdef DEBUG_MODE
#define DEBUG_PRINT(x) qDebug() << x    // Debugging macro
#else
#define DEBUG_PRINT(x)                  // No-op in release mode
#endif

#endif // PROJ_CONF_H
