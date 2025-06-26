#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "audio_player.h"
/*
int main()
{
    FILE* archivo = fopen("C:\\Users\\santy\\Desktop\\Ejercicio\\audio_list.raw", "rb");
    if(!archivo)
    {
        perror("\nNo se pudo abrir el archivo");
        return 1;
    }
    uint32_t header;
    fread(&header, sizeof(uint32_t), 1, archivo);
    uint32_t SampleRate = header & 0xF; 
    uint32_t SampleCount = (header >>4) & 0xFFFFFFF;

    //verifico lo que tengo en las variables
    printf("\nLo que hay en las variables es:\n");
    printf("Sample Rate: %u\n", SampleRate);
    printf("Sample Couunt: %u\n", SampleCount);

    //dentro de sample count esta la cantidad de muestras tipo float que sacamos de sample audio luego 
    float * muestra = malloc(SampleCount * sizeof(float)); //aca reservamos memoria con el espacio para la cantidad de muestras segun samplecount 
    if(!muestra) //verificacion
    {
        fprintf(stderr, "No se pudo reservar memoria\n");
        fclose(archivo);
        return 1;
    }
    uint32_t leidos = fread(muestra, sizeof(float), SampleCount, archivo); //aca hacemos dos cosas al mismo tiempo, registro los datos en muestra y preparo una verificacion
    if(leidos != SampleCount)
    {
        fprintf(stderr,"Error: no se leyeron todas las muestras\n");
        fclose(archivo);
        free(muestra);
        return 1;
    }

    //queda sacar el numero que esta en samplerate para determinar la frecuencia
    int sample_rates[] = {32000, 44100, 48000, 88200};

    //pruebo si anda con el primero
    play_audio(sample_rates[SampleRate],SampleCount,muestra);

    return 0;
}*/





//para la estructura general primero debemos saber cuantos archivos hay 
int main()
{
    int contador=0;

    FILE* archivo = fopen("C:\\Users\\santy\\Desktop\\Ejercicio\\audio_list.raw", "rb");//abro el archivo fuera del while 
    if(!archivo) //comprobacion 
    {
        perror("\nNo se pudo abrir el archivo");
        return 1;
    }
    while(1) //re loca esta forma de usar while xd 
    {
        uint32_t header;  //estas variables se van a ir perdiendo tras cada iteracion 
        size_t bandera = fread(&header, sizeof(uint32_t), 1, archivo);
        if(bandera != 1) break; //esto sale del contador y proque bandera sera = 0 cuando no encuentre nada
        uint32_t SampleCount = (header >>4) & 0xFFFFFFF; //esta es la duracion de audiosamples que le siguie a cada header, este valor debemos saltarlo para pasar al siguiente header
        fseek(archivo, SampleCount * sizeof(float), SEEK_CUR ); //fseek(origen, cantidad de movimientos (el tipo es importante), desde el ultimo valor de file pointer)
        contador++;
    }
    /*esto va a inicializar un contador en cero y va a ir abriendo el archivo y registrando cada header que si se pudo leer
    cuando un header no se pueda leer(porque ya no hay mas) cuando esto pase break nos va a sacar del while haciendo que el contador 
    no se vuelva a iterar mas, en cada iteracion vamos a leer "header", pero luego de cada header hay un audio sample, el cual tiene la misma duracion que sample count
    ese espacio es necesario saltarlo y es por eso que implementamos fseek*/
    
    //ahora que ya sabemos la cantidad de pistas podemos armar un algoritmo para que el usuario elija la que quiera oir

    printf("********BIENVENIDO AL REPRODUCTOR DE AUDIO********\n");
    printf("La cantidad de pistas encontradas fue: %d\n", contador);
    
    int seleccion;
    do{
        do//verificacion
        {
            printf("Que cancion desea escuchar? (0=salir):\n");
            scanf("%d",&seleccion);
            if(seleccion == 0){
                printf("Gracias por usar el reproductor\n");
                fclose(archivo);
                return 0; //se cierra todo el programa

            }else if(seleccion < 1){
                printf("Error: Debe seleccionar un numero entre 0 y %d\n", contador);

            }else if(seleccion > contador){
                printf("Error: Debe seleccionar un numero entre 0 y %d\n", contador);
            }
        } while (seleccion<1 || seleccion>contador); //el usuario solo puede elegir un numero entre 0 y el valor de contador

        printf("Reproduciendo la cancion num %d...\n", seleccion);

        //vuelvo a usar el mismo algoritmo del contador de pistas pero esta vez para abrir la cancion
        //primero vuelvo el puntero del archivo al inicio 
        fseek(archivo, 0, SEEK_SET);

        //despues comienzo todo otra vez
        int contador2=1;
        while(contador2!=seleccion){ //si se eligio la cancion numero 1 este while no se va a ejecutar 
            uint32_t header;  //estas variables se van a ir perdiendo tras cada iteracion 
            fread(&header, sizeof(uint32_t), 1, archivo);
            uint32_t SampleCount = (header >>4) & 0xFFFFFFF; 
            fseek(archivo, SampleCount * sizeof(float), SEEK_CUR ); //deja el puntero en el inicio del siguiente header
            contador2++;
        }
        //ya estamos parados en la posicion deseada, ahora hacemos lo mismo que al inicio con el codigo de prueba
        uint32_t header;
        fread(&header, sizeof(uint32_t), 1, archivo);
        uint32_t SampleRate = header & 0xF; 
        uint32_t SampleCount = (header >>4) & 0xFFFFFFF;

        /*verifico lo que tengo en las variables
        printf("\nLo que hay en las variables es:\n");
        printf("Sample Rate: %u\n", SampleRate);
        printf("Sample Couunt: %u\n", SampleCount);*/ //esta parte no necesitamos verla, pero es parte del codigo de prueba

        //dentro de sample count esta la cantidad de muestras tipo float que sacamos de sample audio luego 
        float * muestra = malloc(SampleCount * sizeof(float)); //aca reservamos memoria con el espacio para la cantidad de muestras segun samplecount 
        if(!muestra) //verificacion
        {
            fprintf(stderr, "No se pudo reservar memoria\n");
            fclose(archivo);
            return 1;
        }
        uint32_t leidos = fread(muestra, sizeof(float), SampleCount, archivo); //aca hacemos dos cosas al mismo tiempo, registro los datos en muestra y preparo una verificacion
        if(leidos != SampleCount)
        {
            fprintf(stderr,"Error: no se leyeron todas las muestras\n");
            fclose(archivo);
            free(muestra);
            return 1;
        }

        //queda sacar el numero que esta en samplerate para determinar la frecuencia
        int sample_rates[] = {32000, 44100, 48000, 88200};

        //llamo a la funcion play_audio
        play_audio(sample_rates[SampleRate],SampleCount,muestra);
        free(muestra);
        
    //una vez reproducido el tema, volvemos al menu principal. No se puede usar una funcion switch porque no sabemos cuantas opciones hay hasta que el programa se ejecuta 
    }while(seleccion!=0);
    fclose(archivo);
}