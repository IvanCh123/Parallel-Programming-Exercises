# transmission_time_2

Para conocer el tiempo de transmisión es necesario hacer el siguiente cálculo:
T = L + s/b

Donde L es la latencia, s es la cantidad de bytes y b es el ancho de banda.  

La latencia con la que se envían los datos es de 2 segundos.

La cantidad de bytes a enviar cada 60 segundos es de 1.5MB (1500000 bytes)

El ancho de banda es de 16mpbs, lo cual es equivalente a 16777216

Por lo tanto:

> T = 2 + 1500000/16777216

> T = 2.0894 segundos para un archivo de 1.5MB

Ahora lo importante, el ancho de banda mínimo se calcula de la siguiente manera:

> (70 - 1) * (1,5 / x) + 2 + (1,5 / x) = 60

> (70) * (1,5 / x) + 2 = 60

> x = (70 * 1,5) / 58

> x = 1,81 MBps = 14,48 Mbps

Por tanto, el ancho de banda actual es suficiente para transmitir los paquetes.


