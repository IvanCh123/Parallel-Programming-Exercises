# Rationale
La solución encontrada para las actividades hello_ordered_mutex y hello_ordered_semaphore permite resolver el problema de saludar desde 
cada núcleo de forma predecible. 
Por otra parte, en la solución encontrada para hello_ordered_mutex  al iniciar el problema, todos los mutex son bloqueados (excepto el del 
thread cero). Sin embargo, para desbloquear los mutex de los threads que no son cero, se accede a los datos privados de cada thread, 
lo cuál viola el encapsulamiento de los datos. Esta práctica podría resultar peligrosa en otros algoritmos en los que no se conoce el 
momento en que cada thread modifica sus datos, pues otro thread podría acceder a ellos y provocar la corrupción de esos datos.
Por último, podemos observar que con los semáforos no resulta necesario irrespetar el encapsulamiento de los datos, pues es posible 
controlar el paso del siguiente thread aumentando el contador del semáforo correspondiente.
