Corra su solución y mida el tiempo de ejecución. 
¿Permite este mecanismo de control de concurrencia resolver este problema de manera predecible?

Si y no al mismo tiempo. El simple hecho de decirle "lock" al mutex, no me implica que le este diciendo
al primero por ejemplo (en el caso del primer ciclo), el CPU decide a quién darselo. Para eso es necesario
hacer la diferenciación por indices para saber cuando hacerle lock/unlock a un mutex. 
