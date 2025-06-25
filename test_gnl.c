/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_gnl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iportill <iportill@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:00:00 by iportill          #+#    #+#             */
/*   Updated: 2024/06/25 10:00:00 by iportill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

// Colores para la salida
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

// Tamaños de BUFFER a probar
#define NUM_BUFFER_SIZES 6
int buffer_sizes[NUM_BUFFER_SIZES] = {1, 5, 32, 42, 1024, 10000};

void create_test_file(const char *filename, const char *content)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        printf("Error: No se pudo crear el archivo de prueba %s\n", filename);
        exit(1);
    }
    write(fd, content, strlen(content));
    close(fd);
}

void test_basic_functionality(int buffer_size)
{
    printf(YELLOW "\n=== Prueba con BUFFER_SIZE=%d ===\n" RESET, buffer_size);
    
    // Crear un archivo de prueba
    const char *test_content = "Línea 1\nLínea 2\nLínea 3\nLínea 4\nLínea 5";
    create_test_file("test_basic.txt", test_content);
    
    // Abrir el archivo y leer línea por línea
    int fd = open("test_basic.txt", O_RDONLY);
    if (fd < 0)
    {
        printf(RED "Error: No se pudo abrir el archivo de prueba\n" RESET);
        return;
    }
    
    char *line;
    int line_count = 0;
    const char *expected_lines[] = {
        "Línea 1\n", "Línea 2\n", "Línea 3\n", "Línea 4\n", "Línea 5"
    };
    
    printf("Leyendo líneas del archivo...\n");
    while ((line = get_next_line(fd)) != NULL)
    {
        if (line_count < 5)
        {
            if (strcmp(line, expected_lines[line_count]) == 0)
                printf(GREEN "✓ Línea %d correcta: %s" RESET, line_count + 1, line);
            else
                printf(RED "✗ Línea %d incorrecta.\n  Esperado: '%s'\n  Obtenido: '%s'" RESET, 
                       line_count + 1, expected_lines[line_count], line);
        }
        else
        {
            printf(RED "✗ Se encontraron más líneas de las esperadas: %s" RESET, line);
        }
        
        free(line);
        line_count++;
    }
    
    if (line_count < 5)
        printf(RED "✗ Se esperaban 5 líneas, pero solo se leyeron %d\n" RESET, line_count);
    else if (line_count == 5)
        printf(GREEN "✓ Se leyeron correctamente las 5 líneas del archivo\n" RESET);
    
    close(fd);
}

void test_empty_file(int buffer_size)
{
    printf(YELLOW "\n=== Prueba de archivo vacío con BUFFER_SIZE=%d ===\n" RESET, buffer_size);
    
    // Crear un archivo vacío
    create_test_file("test_empty.txt", "");
    
    // Abrir el archivo y tratar de leer
    int fd = open("test_empty.txt", O_RDONLY);
    if (fd < 0)
    {
        printf(RED "Error: No se pudo abrir el archivo de prueba\n" RESET);
        return;
    }
    
    char *line = get_next_line(fd);
    if (line == NULL)
        printf(GREEN "✓ Correcto: get_next_line retornó NULL para un archivo vacío\n" RESET);
    else
    {
        printf(RED "✗ Incorrecto: get_next_line no retornó NULL para un archivo vacío\n" RESET);
        free(line);
    }
    
    close(fd);
}

void test_single_line_no_newline(int buffer_size)
{
    printf(YELLOW "\n=== Prueba de línea única sin salto de línea con BUFFER_SIZE=%d ===\n" RESET, buffer_size);
    
    // Crear un archivo con una sola línea sin salto de línea al final
    const char *test_content = "Esta es una línea sin salto de línea al final";
    create_test_file("test_single_line.txt", test_content);
    
    // Abrir el archivo y leer
    int fd = open("test_single_line.txt", O_RDONLY);
    if (fd < 0)
    {
        printf(RED "Error: No se pudo abrir el archivo de prueba\n" RESET);
        return;
    }
    
    char *line = get_next_line(fd);
    if (line != NULL)
    {
        if (strcmp(line, test_content) == 0)
            printf(GREEN "✓ Correcto: get_next_line leyó correctamente la línea sin salto de línea\n" RESET);
        else
            printf(RED "✗ Incorrecto: get_next_line no leyó correctamente la línea\n  Esperado: '%s'\n  Obtenido: '%s'\n" RESET, 
                   test_content, line);
        
        free(line);
    }
    else
    {
        printf(RED "✗ Incorrecto: get_next_line retornó NULL para una línea sin salto de línea\n" RESET);
    }
    
    // Verificar que no hay más líneas
    line = get_next_line(fd);
    if (line == NULL)
        printf(GREEN "✓ Correcto: get_next_line retornó NULL después de leer la única línea\n" RESET);
    else
    {
        printf(RED "✗ Incorrecto: get_next_line no retornó NULL después de leer la única línea\n" RESET);
        free(line);
    }
    
    close(fd);
}

void test_invalid_fd(int buffer_size)
{
    printf(YELLOW "\n=== Prueba de descriptor de archivo inválido con BUFFER_SIZE=%d ===\n" RESET, buffer_size);
    
    char *line = get_next_line(-1);
    if (line == NULL)
        printf(GREEN "✓ Correcto: get_next_line retornó NULL para un FD inválido\n" RESET);
    else
    {
        printf(RED "✗ Incorrecto: get_next_line no retornó NULL para un FD inválido\n" RESET);
        free(line);
    }
}

void test_large_line(int buffer_size)
{
    printf(YELLOW "\n=== Prueba de línea grande con BUFFER_SIZE=%d ===\n" RESET, buffer_size);
    
    // Crear un archivo con una línea muy larga
    char *large_line = malloc(10000 + 1);
    if (!large_line)
    {
        printf(RED "Error: No se pudo asignar memoria para la prueba\n" RESET);
        return;
    }
    
    // Llenar la línea con caracteres
    for (int i = 0; i < 10000; i++)
        large_line[i] = 'A' + (i % 26);
    large_line[10000] = '\0';
    
    create_test_file("test_large_line.txt", large_line);
    
    // Abrir el archivo y leer
    int fd = open("test_large_line.txt", O_RDONLY);
    if (fd < 0)
    {
        printf(RED "Error: No se pudo abrir el archivo de prueba\n" RESET);
        free(large_line);
        return;
    }
    
    char *line = get_next_line(fd);
    if (line != NULL)
    {
        if (strcmp(line, large_line) == 0)
            printf(GREEN "✓ Correcto: get_next_line leyó correctamente la línea grande\n" RESET);
        else
            printf(RED "✗ Incorrecto: get_next_line no leyó correctamente la línea grande\n" RESET);
        
        free(line);
    }
    else
    {
        printf(RED "✗ Incorrecto: get_next_line retornó NULL para una línea grande\n" RESET);
    }
    
    free(large_line);
    close(fd);
}

void test_multiple_fds(int buffer_size)
{
    printf(YELLOW "\n=== Prueba de múltiples descriptores de archivo con BUFFER_SIZE=%d ===\n" RESET, buffer_size);
    
    // Crear dos archivos diferentes
    create_test_file("test_file1.txt", "Archivo 1, línea 1\nArchivo 1, línea 2\n");
    create_test_file("test_file2.txt", "Archivo 2, línea 1\nArchivo 2, línea 2\n");
    
    // Abrir ambos archivos
    int fd1 = open("test_file1.txt", O_RDONLY);
    int fd2 = open("test_file2.txt", O_RDONLY);
    
    if (fd1 < 0 || fd2 < 0)
    {
        printf(RED "Error: No se pudieron abrir los archivos de prueba\n" RESET);
        if (fd1 >= 0) close(fd1);
        if (fd2 >= 0) close(fd2);
        return;
    }
    
    // Leer alternando entre los dos archivos
    char *line1, *line2;
    
    // Primera línea del primer archivo
    line1 = get_next_line(fd1);
    if (line1 && strcmp(line1, "Archivo 1, línea 1\n") == 0)
        printf(GREEN "✓ Correcto: Primera línea del archivo 1\n" RESET);
    else
        printf(RED "✗ Incorrecto: Primera línea del archivo 1\n" RESET);
    free(line1);
    
    // Primera línea del segundo archivo
    line2 = get_next_line(fd2);
    if (line2 && strcmp(line2, "Archivo 2, línea 1\n") == 0)
        printf(GREEN "✓ Correcto: Primera línea del archivo 2\n" RESET);
    else
        printf(RED "✗ Incorrecto: Primera línea del archivo 2\n" RESET);
    free(line2);
    
    // Segunda línea del primer archivo
    line1 = get_next_line(fd1);
    if (line1 && strcmp(line1, "Archivo 1, línea 2\n") == 0)
        printf(GREEN "✓ Correcto: Segunda línea del archivo 1\n" RESET);
    else
        printf(RED "✗ Incorrecto: Segunda línea del archivo 1\n" RESET);
    free(line1);
    
    // Segunda línea del segundo archivo
    line2 = get_next_line(fd2);
    if (line2 && strcmp(line2, "Archivo 2, línea 2\n") == 0)
        printf(GREEN "✓ Correcto: Segunda línea del archivo 2\n" RESET);
    else
        printf(RED "✗ Incorrecto: Segunda línea del archivo 2\n" RESET);
    free(line2);
    
    close(fd1);
    close(fd2);
}

void run_all_tests()
{
    for (int i = 0; i < NUM_BUFFER_SIZES; i++)
    {
        // Recompilar con el BUFFER_SIZE actual
        char compile_cmd[200];
        sprintf(compile_cmd, "gcc -Wall -Wextra -Werror -D BUFFER_SIZE=%d -o test_gnl test_gnl.c get_next_line.c get_next_line_utils.c", 
                buffer_sizes[i]);
        system(compile_cmd);
        
        printf(BLUE "\n==========================================\n");
        printf("EJECUTANDO PRUEBAS CON BUFFER_SIZE = %d\n", buffer_sizes[i]);
        printf("==========================================\n" RESET);
        
        test_basic_functionality(buffer_sizes[i]);
        test_empty_file(buffer_sizes[i]);
        test_single_line_no_newline(buffer_sizes[i]);
        test_invalid_fd(buffer_sizes[i]);
        test_large_line(buffer_sizes[i]);
        test_multiple_fds(buffer_sizes[i]);
    }
    
    // Limpiar archivos de prueba
    system("rm -f test_basic.txt test_empty.txt test_single_line.txt test_large_line.txt test_file1.txt test_file2.txt");
}

int main(void)
{
    printf(BLUE "\n==============================================\n");
    printf("PRUEBAS PARA GET_NEXT_LINE (PROYECTO 42)\n");
    printf("==============================================\n" RESET);
    
    run_all_tests();
    
    printf(BLUE "\n==============================================\n");
    printf("PRUEBAS COMPLETADAS\n");
    printf("==============================================\n" RESET);
    
    return (0);
}
