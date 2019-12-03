# transmission_time_1

La formula para este cálculo es la siguiente:
T = L + s/b

Donde L es la latencia, s es la cantidad de bytes y b es el ancho de banda.  

La cantidad de datos a transmitir son 7.2 TB, que convertidos a megabytes son 7200000. Al ser
la transmisión de 1mb por 1mb, lo cual sería 1000000 de bytes, el número resultado se multiplicará por este para saber el tiempo
transcurrido total.

La latencia es de 20 milisegundos, que convertido a segundos son 0.02 segundos. 

Finalmente, el ancho de banda en bytes sería 85000 bps

Datos:
> L = 0.02 seconds

> s = 1000000 bytes

> b = 85000 bps

Calculamos:
> T = 0.02 +  1000000/ 85000 = 11.78 segundos.

Se durarían 11.78 segundos para transmitir 1mb. Por lo tanto, para transmitir todos los 7.2TB de datos
duraría:

11.78 * 7200000 = 84816000 segundos para transmitir todos los datos, lo cual es equivalente a 32 meses (redondeado)

