# GetNextLine

## 📋 Tabla de Contenidos
- [📝 Descripción](#-descripción)
- [🛠️ Compilación](#️-compilación)
- [📚 Uso](#-uso)
- [✅ Pruebas](#-pruebas)
- [🔍 Características](#-características)
- [🧪 Ejemplos](#-ejemplos)
- [📋 Evaluación](#-evaluación)

## 📝 Descripción

Get Next Line (GNL) es una función que lee líneas desde un descriptor de archivo. El proyecto forma parte del plan de estudios de 42 y su objetivo principal es introducir el concepto de variables estáticas en C y mejorar la comprensión de operaciones de archivos, asignación de memoria y gestión de strings.

La función `get_next_line` lee una línea completa hasta un salto de línea desde un descriptor de archivo (fd) y la devuelve, incluyendo el salto de línea si está presente. En llamadas sucesivas con el mismo descriptor, la función continúa leyendo el archivo, una línea a la vez.

## 🛠️ Compilación

Para incluir esta función en tu proyecto, simplemente compila los archivos fuente junto con tu código:

```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 your_program.c get_next_line.c get_next_line_utils.c
```

Puedes modificar el valor de `BUFFER_SIZE` según tus necesidades. Este parámetro determina el tamaño del buffer utilizado para leer desde el descriptor de archivo.

## 📚 Uso

```c
#include "get_next_line.h"

int main(void)
{
    int     fd;
    char    *line;
    
    fd = open("archivo.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);  // Importante liberar la memoria
    }
    close(fd);
    return (0);
}
```

## ✅ Pruebas

El proyecto ha sido probado exhaustivamente con diferentes tamaños de BUFFER_SIZE (1, 5, 32, 42, 1024, 10000) y ha pasado todos los tests. Las pruebas cubren:

- Lectura de archivos línea por línea
- Manejo de archivos vacíos
- Líneas sin salto de línea al final
- Descriptores de archivo inválidos
- Líneas muy largas (10,000 caracteres)
- Lectura simultánea desde múltiples descriptores de archivo

Para ejecutar las pruebas, utiliza el archivo `test_gnl.c` proporcionado:

```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 test_gnl.c get_next_line.c get_next_line_utils.c -o test_gnl
./test_gnl
```

### Verificación de fugas de memoria

Para asegurarte de que no hay fugas de memoria, puedes utilizar Valgrind. Primero, compila el programa con las banderas de depuración:

```bash
gcc -g -Wall -Wextra -Werror -D BUFFER_SIZE=42 test_gnl.c get_next_line.c get_next_line_utils.c -o test_gnl
```

Luego, ejecuta Valgrind con las siguientes opciones:

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test_gnl
```

También puedes probar con diferentes tamaños de BUFFER_SIZE para asegurarte de que no hay fugas en ninguna configuración:

```bash
# Compilar y verificar con BUFFER_SIZE=1
gcc -g -Wall -Wextra -Werror -D BUFFER_SIZE=1 test_gnl.c get_next_line.c get_next_line_utils.c -o test_gnl
valgrind --leak-check=full ./test_gnl

# Compilar y verificar con BUFFER_SIZE=10000
gcc -g -Wall -Wextra -Werror -D BUFFER_SIZE=10000 test_gnl.c get_next_line.c get_next_line_utils.c -o test_gnl
valgrind --leak-check=full ./test_gnl
```

Si Valgrind muestra "All heap blocks were freed -- no leaks are possible" o "still reachable: X bytes in Y blocks" (donde estos bloques corresponden a las variables estáticas del programa), significa que tu implementación está gestionando correctamente la memoria.

## 🔍 Características

- Lee líneas completas desde cualquier descriptor de archivo válido
- Maneja correctamente múltiples descriptores de archivo simultáneamente
- Funciona con cualquier tamaño de BUFFER_SIZE
- Maneja eficientemente la memoria, sin fugas
- Gestiona correctamente errores (descriptores inválidos, problemas de lectura)
- Implementa variables estáticas para mantener el estado entre llamadas

## 🧪 Ejemplos

### Ejemplo 1: Leer un archivo línea por línea

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;
    int     line_count;
    
    fd = open("ejemplo.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    
    line_count = 1;
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("Línea %d: %s", line_count++, line);
        free(line);
    }
    
    close(fd);
    return (0);
}
```

### Ejemplo 2: Leer desde múltiples archivos

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd1, fd2;
    char    *line1, *line2;
    
    fd1 = open("archivo1.txt", O_RDONLY);
    fd2 = open("archivo2.txt", O_RDONLY);
    
    // Leer alternando entre los dos archivos
    line1 = get_next_line(fd1);
    printf("Archivo 1, línea 1: %s", line1);
    free(line1);
    
    line2 = get_next_line(fd2);
    printf("Archivo 2, línea 1: %s", line2);
    free(line2);
    
    line1 = get_next_line(fd1);
    printf("Archivo 1, línea 2: %s", line1);
    free(line1);
    
    line2 = get_next_line(fd2);
    printf("Archivo 2, línea 2: %s", line2);
    free(line2);
    
    close(fd1);
    close(fd2);
    return (0);
}
```

## 📋 Evaluación

Este proyecto cumple con todos los requisitos de la asignación de 42:

- Implementa correctamente la función `get_next_line` que lee una línea de un descriptor de archivo
- Maneja múltiples descriptores de archivo simultáneamente
- Funciona con diferentes tamaños de BUFFER_SIZE
- No tiene fugas de memoria
- Sigue las normas de codificación de 42
- No utiliza funciones prohibidas (solo malloc, free y read están permitidas)
- Gestiona correctamente los errores

Si encuentras algún problema o tienes sugerencias de mejora, no dudes en contactar o abrir un issue.