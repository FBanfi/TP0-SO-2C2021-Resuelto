#include "client.h"
#include<readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;
	char* value;

	logger = iniciar_logger();


	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");

	config = iniciar_config();

	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor
	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	//Loggear valor de config
	log_info(logger, valor);
	leer_consola(logger);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	//enviar CLAVE al servirdor
	enviar_mensaje(valor, conexion);

	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* logger;

	if((logger = log_create("/home/utnso/workspace/tp0/tp0.log", "tp0", true, LOG_LEVEL_INFO)) == NULL){

		perror("No se pudo crear el log/n");
		abort();
	}
	return logger;
}

t_config* iniciar_config(void)
{
	t_config* config;

	if((config = config_create("/home/utnso/workspace/tp0/tp0.config")) == NULL){

		perror("No se pudo crear la config!/n");
		abort();
	}

	return config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	//El primero te lo dejo de yapa
	while (!string_is_empty(leido = readline(">"))) {

		if (leido == NULL) {

			perror("No se puede leer esa linea");
			abort();
		}

		log_info(logger, leido);
		free(leido);
	}

	free(leido);
}

void paquete(int conexion)
{
	//Ahora toca lo divertido!

	char* leido;
	t_paquete* paquete;
	t_log* logger;
	int tamanio;

	paquete = crear_super_paquete();

	while (!string_is_empty(leido = readline(">"))) {

			if (leido == NULL) {

				perror("No se puede leer esa linea");
				abort();
			}

			tamanio = strlen(leido) + 1;
			agregar_a_paquete(paquete, (void*) leido, tamanio);
			free(leido);
		}

		free(leido);

		enviar_paquete(paquete, conexion);

		eliminar_paquete(paquete);

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	close(conexion);
	log_destroy(logger);
	config_destroy(config);
}
